#include "skripler.hpp"
#include "Tile.hpp"
#include <iostream>
#include <sstream>
#include <thread>
#include <memory>
#include <mutex>

//Windows version
#ifdef _WIN32
    #include <windows.h>
#endif
//Linux version
#ifdef __unix__
    #include <sys/ioctl.h>
    #include <stdio.h>
    #include <unistd.h>
#endif

namespace skripler
{
    std::mutex lock;

    bool running=false;
    bool screen_too_small=false;

    int h;
    int w;

    std::thread resizeThread;
    std::shared_ptr<Tile> myTile;

    //Get the size, and return true if the size has changed
    bool get_window_size()
    {

        int prev_h=h;
        int prev_w=w;

//Windows version
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        lock.lock();
        w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        h = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        lock.unlock();
#endif

//Linux version
#ifdef __unix__
        struct winsize W;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &W);
        lock.lock();
        w=W.ws_col;
        h=W.ws_row;
        lock.unlock();
#endif
        --h;//leave room for the command line

        return (w!=prev_w || h!=prev_h);
    }

    void display()
    {
        if (running)
        {

            myTile->update();

            std::cout<<'\n';
            for (const std::string& l : myTile->getTexture().content)
            {
                std::cout<<l<<'\n';
            }
            std::cout<<':'<<std::flush;
        }
    }

    void updateSizeAndDisplay()
    {
        if (get_window_size())
        {
            if (w<w_min || h<h_min)
            {
                screen_too_small=true;
                lock.lock();

                //Do not display too small screen, instead show error message
                if (w>=16)
                {
                    std::cout<<  "\nscreen too small\n";
                    if (h>1)
                        std::cout<<h<<"x"<<w<<" need "<<h_min<<"x"<<w_min<<'\n';
                    if (h>2)
                        std::cout<<"change fontsize\n";
                    if (h>3)
                        std::cout<<"or resize\n";
                    for (int y = 5; y < h; ++y)
                    {
                        std::cout<<'\n';
                    }
                    std::cout<<std::flush;
                }
                else if (w>=12)
                {
                    std::cout<<"\nsmall screen\n";
                    if (h>1)
                        std::cout<<h<<"x"<<w<<"<"<<h_min<<"x"<<w_min<<'\n';
                    for (int y = 2; y < h; ++y)
                    {
                        std::cout<<'\n';
                    }
                    std::cout<<std::flush;
                }
                else
                {
                    for (int y = 0; y < h; ++y)
                    {
                        std::cout<<'\n';
                    }

                }

                lock.unlock();
            }
            else
            {
                myTile->setSize(h,w);
                screen_too_small=false;

                //Temporarilly block executing commands from the commandline
                lock.lock();
                display();
                lock.unlock();
            }
        }

    }

    //Run in a separate thread, to auto-adjust to always fit the terminal
    void monitorSize()
    {
        while(running)
        {
            updateSizeAndDisplay();
        }
    }

    void start(std::shared_ptr<Tile> baseTile)
    {
        if (!running)
        {
            running=true;
            //null tiles are NOT permitted, from now on we ASSUME, it is NOT null
            myTile=(baseTile==nullptr? std::make_shared<Tile>(h_min,w_min,'?'): baseTile);

            //The screen before did not exist, so next monitor size should force update and display
            w=0;
            h=0;
            updateSizeAndDisplay();
            resizeThread=std::thread(monitorSize);

        }
    }

    void end()
    {
        if (running)
        {
            running=false;
            resizeThread.join();
            //No need to delete the tile, the smart-pointer does it for me
        }
    }


    std::vector<std::string> getCommands()
    {

        std::string input;
        lock.lock();
        display();
        lock.unlock();
        std::getline(std::cin,input);

        //If the screen is too small, refuse to accept inputs
        if (screen_too_small)
            return std::vector<std::string>();

        //We will not monitor resizing, until we have pushed everything to the screen
        lock.lock();
        std::vector<std::string> out;

        std::stringstream ss(input);
        std::string S;


        while(ss>>S){
            makeLowercase(S);
            out.push_back(S);
        }
        lock.unlock();

        //Wait for update, check if the update was a command
        return out;
    }
}
