#include "textTools.hpp"
#include <algorithm>
#include <iostream>
namespace skripler
{

    using uint = uint32_t;
    size_t truelength(const std::string& s)
    {
        return (s.length() - std::count_if(s.begin(), s.end(), [](char c)->bool { return (c & 0xC0) == 0x80; }));
    }

    std::string true_substr(const std::string& s,size_t start_symbol, size_t end_symbol)
    {
        size_t min=std::string::npos, max=std::string::npos;
        uint8_t c;
        //Loop through the symbol j, and byte i
        for (uint j=0, i=0; i < s.length(); i++, j++)
        {
            if (j==start_symbol)
                min=i;//smallest so far
            if (j<=end_symbol || end_symbol==std::string::npos)
            max=i;//this byte is the largest so far (if within range)

            c = (uint8_t) s[i];
            if (c<=127) {}//regular ascii, just check to make sure the string is valid
            else if ((c & 0xE0) == 0xC0)  i+=1;
            else if ((c & 0xF0) == 0xE0)  i+=2;
            else if ((c & 0xF8) == 0xF0)  i+=3;//blimey
            else if ((c & 0xFC) == 0xF8) i+=4;//Oh god
            else if ((c & 0xFE) == 0xFC) i+=5;//Pleas stop
            else return "";//HAHA good luck
        }
        if (min==std::string::npos||max==std::string::npos) { return ""; }

        return s.substr(min,max);
    }


    //Place foreground above background, if the foreground is '0'
    std::string overlay(const std::string& foreground,const std::string& background)
    {
        std::string out="";

        //Byte i, symbol j of the foreground, and the corresponding background byte
        for (uint j=0, i=0, i_bg=0; i < foreground.length(); i++, j++,++i_bg)
        {
            uint8_t c = (uint8_t) foreground[i];
            if (c<=127)
            {//regular ascii, this might be 0

                if (c=='0')
                {
                    //Write in the background instead
                    c = (uint8_t) background[i_bg];
                    if (c<=127)
                    {//regular ascii, this might be 0

                        out+=background[i_bg];
                    }
                    else if ((c & 0xE0) == 0xC0)
                    {
                        out+=background[i_bg];
                        out+=background[i_bg+1];
                    }
                    else if ((c & 0xF0) == 0xE0)
                    {
                        out+=background[i_bg];
                        out+=background[i_bg+1];
                        out+=background[i_bg+2];
                    }
                    else if ((c & 0xF8) == 0xF0)
                    {
                        out+=background[i_bg];
                        out+=background[i_bg+1];
                        out+=background[i_bg+2];
                        out+=background[i_bg+3];
                    }
                    else if ((c & 0xFC) == 0xF8)
                    {
                        out+=background[i_bg];
                        out+=background[i_bg+1];
                        out+=background[i_bg+2];
                        out+=background[i_bg+3];
                        out+=background[i_bg+4];
                    }
                    else if ((c & 0xFE) == 0xFC)
                    {
                        out+=background[i_bg];
                        out+=background[i_bg+1];
                        out+=background[i_bg+2];
                        out+=background[i_bg+3];
                        out+=background[i_bg+4];
                        out+=background[i_bg+5];
                    }
                }
                else
                    out+=foreground[i];
            }
            else if ((c & 0xE0) == 0xC0)
            {
                out+=foreground[i];
                out+=foreground[i+1];
                i+=1;
            }
            else if ((c & 0xF0) == 0xE0)
            {
                out+=foreground[i];
                out+=foreground[i+1];
                out+=foreground[i+2];
                i+=2;
            }
            else if ((c & 0xF8) == 0xF0)
            {
                out+=foreground[i];
                out+=foreground[i+1];
                out+=foreground[i+2];
                out+=foreground[i+3];
                i+=3;//blimey
            }
            else if ((c & 0xFC) == 0xF8)
            {
                out+=foreground[i];
                out+=foreground[i+1];
                out+=foreground[i+2];
                out+=foreground[i+3];
                out+=foreground[i+4];
                i+=4;//Oh god
            }
            else if ((c & 0xFE) == 0xFC)
            {
                out+=foreground[i];
                out+=foreground[i+1];
                out+=foreground[i+2];
                out+=foreground[i+3];
                out+=foreground[i+4];
                out+=foreground[i+5];
                i+=5;//Pleas stop
            }

            //Keep advancing through the background
            c = (uint8_t) background[i_bg];
            if (c<=127) {}//regular ascii, just check to make sure the string is valid
            else if ((c & 0xE0) == 0xC0) i_bg+=1;
            else if ((c & 0xF0) == 0xE0) i_bg+=2;
            else if ((c & 0xF8) == 0xF0) i_bg+=3;//blimey
            else if ((c & 0xFC) == 0xF8) i_bg+=4;//Oh god
            else if ((c & 0xFE) == 0xFC) i_bg+=5;//Pleas stop
            else return "";//HAHA good luck



        }

        return out;
    }

    void makeLowercase(std::string& data)
    {
    std::transform(data.begin(), data.end(), data.begin(),
        [](unsigned char c){ return std::tolower(c); });
    }
}
