#pragma once

#include<vector>
#include<string>
#include "textTools.hpp"

namespace skripler
{
    struct Texture
    {
        std::string name;
        int w;
        int h;
        std::vector<std::string> content;//Each string is supposed to already have the correct true size (i.e. å and a both have size 1)

        //A texture from a string
        Texture(const std::string& name,bool border);

        //Read from a file in textures/
        Texture(const std::string& name);

        Texture(int _h, int _w,char c=' ')
        {
            name=std::to_string(_h)+"x"+std::to_string(_w)+std::string(1,c);
            w=_w;
            h=_h;

            content=std::vector<std::string>(h,std::string(w,c));
        }

        Texture(const Texture& first, const Texture& second, bool leftright);//Append textures, for the sake of speed (of coding) no checks are done to see if the sizes match!!!

        //Stamp this texture on top on this one at this location. The number 0 is transparant (Use Capital letter O instead of number 0)
        void stamp(int y, int x,const Texture& That);
        void set(int y, int x, const std::string& c);//It is assumed that C is one character (though it may be a multi-bit character)
    };

}
