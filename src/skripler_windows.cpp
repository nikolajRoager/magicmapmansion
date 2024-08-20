#include "skripler.hpp"
#include "Tile.hpp"
#include <iostream>
#include <sstream>
#include <memory>

//Windows version NO RESIZE DETECTION, ASSUMES MINIMUM SIZE, OR SET AT STARTUP

namespace skripler
{

    bool running=false;
    bool screen_too_small=false;

    int h=34;
    int w=106;

    std::shared_ptr<Tile> myTile;


    //Does not require Skripler beeing started, test that get windows size works
    void printWH()
    {
        std::cout<<"Window height "<<h<<" width "<<w<<std::endl;
    }

    void display()
    {
        if (running && !screen_too_small)
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


    void start(std::shared_ptr<Tile> baseTile,int _h, int _w)
    {
        w=_w;
        h=_h;
        std::cout<<" "<<h<<" x "<<w<<std::endl;
        if (!running)
        {
            running=true;
            //null tiles are NOT permitted, from now on we ASSUME, it is NOT null
            myTile=(baseTile==nullptr? std::make_shared<Tile>(h_min,w_min,'?'): baseTile);



            myTile->setSize(h,w);
            screen_too_small=false;

            //Temporarilly block executing commands from the commandline
            display();
        }
    }

    void end()
    {
        if (running)
        {
            running=false;
            //No need to delete the tile, the smart-pointer does it for me
        }
    }


    std::vector<std::string> getCommands()
    {

        std::string input;
        display();
        std::getline(std::cin,input);

        //If the screen is too small, refuse to accept inputs
        if (screen_too_small)
            return std::vector<std::string>();

        //We will not monitor resizing, until we have pushed everything to the screen
        std::vector<std::string> out;

        std::stringstream ss(input);
        std::string S;


        while(ss>>S){
            makeLowercase(S);
            out.push_back(S);
        }

        //Wait for update, check if the update was a command
        return out;
    }
}
