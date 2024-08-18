#include "Texture.hpp"
#include "textTools.hpp"
#include<iostream>

#include<fstream>
#include<filesystem>

namespace fs=std::filesystem;

namespace skripler
{
    Texture::Texture(const Texture& first, const Texture& second, bool leftright)//Append textures, for the sake of speed (of coding) no checks are done to see if the sizes match!!!
    {
        if (leftright)
        {
            h=first.h;
            w=first.w+second.w;
            for (int y = 0; y < h; ++y)
                content.push_back(first.content[y]+second.content[y]);

        }
        else
        {
            w=first.w;
            h=first.h+second.h;
            std::copy(first.content.begin(),first.content.end(),std::back_inserter(content));
            std::copy(second.content.begin(),second.content.end(),std::back_inserter(content));
        }

    }

    //A texture from a string
    Texture::Texture(const std::string& _name,bool border)
    {
        name=_name;
        w=truelength(name);
        if (border)
        {
            w+=2;
            h=3;
            content=std::vector<std::string>(h);
            content[0]="";
            content[1]="#"+name+"#";
            content[2]="";
            for (int i = 0; i < w; ++i)
            {
                content[0]+="#";
                content[2]+="#";
            }
        }
        else
        {
            h=1;
            content={name};

        }
    }

    void Texture::stamp(int y0, int x0,const Texture& that)
    {
        for(int i = 0; i < that.h; ++i)
        {
            int y=y0+i;
            if (y>=h)
                break;

            //Copy the before and the after around the string we insert
            content[y]=true_substr(content[y],0,x0)+overlay(that.content[i],true_substr(content[y],x0,x0+that.w))+true_substr(content[y],x0+that.w,w);


        }

    }
    void Texture::set(int y, int x, const std::string& c)
    {
        //Copy the before and the after around the character we insert
        content[y]=true_substr(content[y],0,x)+c+true_substr(content[y],x+1,w);
    }

    Texture::Texture(const std::string& _name)
    {
        name=_name;
        std::ifstream input(fs::path("textures")/name);
        h=0;
        if (input.is_open())
        {
            std::string s;
            w=0;
            while(std::getline(input,s))
            {
                ++h;
                content.push_back(s);
                if (w==0)
                    w=truelength(s);
            }
            input.close();
        }
        else
            throw std::runtime_error("Could not open textures/"+name+" (Be sure the textures folder is in the same folder, as you opened the terminal in!)");

    }

}
