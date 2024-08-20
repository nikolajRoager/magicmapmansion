#include "Tile.hpp"
#include "textTools.hpp"

#include<iostream>
#include<fstream>
#include<sstream>

namespace skripler
{

    void display();//The tiles are able to call the display function

    Tile::Tile(uint32_t h, uint32_t w,char _c):c(_c),myTex(w,h,_c){}
    void Tile::setSize(uint32_t h, uint32_t w)
    {
        myTex=Texture(h,w,c);
    }

    BisectTile::BisectTile(uint32_t _h, uint32_t _w, uint32_t _first_min, uint32_t _second_min, bool _leftright,bool _border,int _max_size):Tile(_h,_w,'c')
    {
        border=_border;
        h=_h;
        w=_w;
        //Default to placeholders, writing 'A' and 'B', the +1 ensures that the first tile has its width or height rounded down, and the other rounded up
        first = std::make_shared<Tile>((leftright? h : h/2),(leftright? w/2 : w),'A');
        second= std::make_shared<Tile>((leftright? h : (h+1)/2),(leftright? (w+1)/2 : w),'B');

        leftright=_leftright;

        first_min = _first_min;
        second_min=_second_min;

        max_size=_max_size;

        update();
    }


    void BisectTile::set_first(std::shared_ptr<Tile> newFirst)
    {
        first = newFirst==nullptr ? std::make_shared<Tile>((leftright? h : h/2),(leftright? w/2 : w),'A'): newFirst;
        update();
    }

    void BisectTile::set_second(std::shared_ptr<Tile> newSecond)
    {
        second = newSecond==nullptr ? std::make_shared<Tile>((leftright? h : (h+1)/2),(leftright? (w+1)/2 : w),'B') : newSecond;
        update();
    }


    void BisectTile::setSize(uint32_t _h, uint32_t _w)
    {

        w=_w;
        h=_h;
        uint32_t split_size = leftright ? w : h;


        //Briefly ignore the border
        if (border)
            --split_size;

        //How much larger would either half be, it it was split evenly
        uint32_t first_extra_size = (split_size-(first_min+second_min))/2;
        uint32_t second_extra_size = (split_size-(first_min+second_min)+1)/2;//The +1 ensures that odd numbers get rounded different ways
        other_side = leftright ? h : w;

        if (max_size>0 && first_extra_size+first_min>static_cast<uint32_t>(max_size))//First is size constrained
        {
            //Add the excess size to the second

            second_extra_size += first_extra_size-max_size+first_min;
            first_extra_size = max_size-first_min;

        }
        else if(max_size<0 && second_extra_size+second_min>static_cast<uint32_t>(-max_size))//Second is size constrained
        {
            first_extra_size += second_extra_size+max_size+second_min;
            second_extra_size = -second_min-max_size;
        }


        {
            if (leftright)
            {
                first->setSize(other_side,first_min+first_extra_size);
                second->setSize(other_side,second_min+second_extra_size);
            }
            else
            {
                first->setSize(first_min+first_extra_size,other_side);
                second->setSize(second_min+second_extra_size,other_side);
            }
        }

        //Then add the border back
        if (border)
            ++split_size;
    }

    void BisectTile::update()
    {




        //Forcefully reset the size of the texture, after all, other people may use the pointer and we CAN NOT have that
        setSize(h,w);
        first -> update();
        second-> update();


        if (border)
        {
            Texture Border =leftright?Texture(h,1,'#'):Texture(1,w,'#');
            Texture First_half = Texture(first->getTexture(),Border,leftright);

            myTex = Texture(First_half,second->getTexture(),leftright);
        }
        else
            myTex = Texture(first->getTexture(),second->getTexture(),leftright);
    }


    void TextWall::printword(const std::string& w)
    {
        if (w.compare("CLEAR")==0)
            content=std::vector<std::string>();
        else
            content.push_back(w);

        update();
    }

    void TextWall::printwords(const std::vector<std::string>& words)
    {
        for (const std::string& W : words)
        {
            if (W.compare("CLEAR")==0)
                content=std::vector<std::string>();
            else
                content.push_back(W);
        }
        update();
    }

    void TextWall::update()
    {

        myTex.h=h;
        myTex.w=w;

        //Start making room for our text
        myTex.content = std::vector<std::string>(h);

        //We are just going to assume our content fits inside the wall
        size_t out_line = 0;
        size_t line_number=0;

        size_t indent_size    =truelength(indent);
        size_t linestart_size =truelength(linestart);

        if (content.size()>0)
        {

            //We can not have more lines than this
            size_t max_linenumber_size=truelength(std::to_string(h));


            std::string line_number_string = std::to_string(line_number);
            line_number_string = std::string(max_linenumber_size-truelength(line_number_string),' ')+line_number_string;


            myTex.content[out_line]+=(linenumbers?line_number_string:"")+linestart;

            bool line_start=true;//Did we just start a new line
            int line_pos = linestart_size+(linenumbers?max_linenumber_size:0);

            for (size_t i=0; i<content.size() && out_line <h; ++i)
            {
                size_t word_size = truelength(content[i]);
                if (content[i].compare("NEWLINE")==0 )//FORCE (and discard the word) WRAP
                {
                    ++line_number;


                    //End this line
                    myTex.content[out_line]+=std::string(w-line_pos+(line_start?0:1),' ');


                    if (out_line+1<h)
                    {
                        ++out_line;

                        std::string line_number_string = std::to_string(line_number);
                        line_number_string = std::string(max_linenumber_size-truelength(line_number_string),' ')+line_number_string;
                        myTex.content[out_line]+=(linenumbers?line_number_string:"")+linestart;


                        line_pos = linestart_size+(linenumbers?max_linenumber_size:0);
                        line_start=true;
                    }
                }
                else
                {
                    //WRAP AND PRINT IF TOO LONG
                    if (word_size+(line_start?0:1)+line_pos>w)
                    {
                        //IF this one word is too long to ever fit, skip it
                        if (word_size+indent_size>w)
                            continue;
                        myTex.content[out_line]+=std::string(w-line_pos+1,' ');
                        ++out_line;
                        line_pos = indent_size;
                        myTex.content[out_line]+=indent;
                        line_start=true;


                    }

                    myTex.content[out_line]+=(line_start?"": " ")+content[i];
                    line_pos+=word_size+1;

                    line_start=false;

                }
            }
            //End this line
            myTex.content[out_line]+=std::string(w-line_pos+ (line_start?0:1),' ');
            ++out_line;
        }
        for (; out_line<h; ++out_line)
        {
            myTex.content[out_line]=std::string(w,' ');


        }



    }

    Canvas::Canvas(uint32_t _h, uint32_t _w,char bg):Tile(_h,_w,bg),w(_w),h(_h),background(bg)
    {

    }

    void Canvas::setSize(uint32_t _h, uint32_t _w)
    {
        h=_h;
        w=_w;
        update();
    }

    void Canvas::update()
    {
        myTex=Texture(h,w,background);


        //Draw all lines behind the textures
        for (line& L : lines)
        {
            //Identify which side is longer
            int x0=L.p0.getX(w);
            int y0=L.p0.getY(h);
            int x1=L.p1.getX(w);
            int y1=L.p1.getY(h);

            if (x0==x1 && y0==y1)
            {
                myTex.set(y0,x0,L.c);
                continue;
            }


            bool longx=std::abs(x1-x0) > std::abs(y1-y0);

            int longD=longx? x1-x0 : y1-y0;

            //Every step, the long side gets one character
            for (int i = 0; i <= std::abs(longD); ++i)
            {
                int x,y;
                if (longx)
                {
                    x = x0+(longD>0?i:-i);
                    y = y0+(i*(y1-y0))/(x1-x0);//Pick whichever y fits here
                }
                else
                {
                    y = y0+(longD>0?i:-i);
                    x = x0+(i*(x1-x0))/(y1-y0);//Ditto when going the other way

                }
                myTex.set(y,x,L.c);
            }
        }

        //Draw all textures
        for (std::pair<pos,Texture>& tex : Textures)
        {
            myTex.stamp(tex.first.getY(h),tex.first.getX(w),tex.second);
        }
    };

    void Canvas::clear()
    {
        Textures=std::vector<std::pair<pos,Texture> >();
        lines   =std::vector<line>() ;
    }

    void Canvas::drawLine(int y0, int x0, int y1, int x1,char c)
    {
        lines.push_back(line(pos(y0,x0),pos(y1,x1),std::string(1,c)));
    }

    void Canvas::drawLine(int y0, int x0, int y1, int x1,std::string c)
    {
        if (truelength(c)>1)
            c="#";//Fallback if too wide
        lines.push_back(line(pos(y0,x0),pos(y1,x1),c));
    }

    void Canvas::drawTexture(int y, int x,const Texture& T)
    {
        Textures.push_back(std::make_pair(pos(y,x),T));
    }

    void Canvas::loadTxt(const std::string& input)
    {
        std::ifstream inputCommands(input);

        if (inputCommands.is_open())
        {
            std::string Command;
            while (std::getline(inputCommands,Command))
            {
                if(Command.size()>0)
                {
                    std::stringstream ss(Command);

                    std::string first_command;
                    ss>>first_command;

                    if (first_command.compare("clear")==0)
                    {
                        Textures=std::vector<std::pair<pos,Texture> >();
                        lines   =std::vector<line>() ;
                    }
                    else if (first_command.compare("line")==0)
                    {
                        int x0,y0,x1,y1;
                        std::string c;
                        ss>>y0;
                        ss>>x0;
                        ss>>y1;
                        ss>>x1;
                        ss>>c;
                        lines.push_back(line(pos(y0,x0),pos(y1,x1),c));
                    }
                    else if (first_command.compare("texfile")==0)
                    {
                        int x,y;
                        std::string filename;
                        ss>>y;
                        ss>>x;
                        ss>>filename;
                        Textures.push_back(std::make_pair(pos(y,x),filename));
                    }
                    else if (first_command.compare("texstring")==0)
                    {
                        int x,y;
                        std::string name;
                        ss>>y;
                        ss>>x;
                        std::getline(ss,name);
                        Textures.push_back(std::make_pair(pos(y,x),Texture(name,false)));
                    }
                    else if (first_command.compare("texboxstring")==0)
                    {
                        int x,y;
                        std::string name;
                        ss>>y;
                        ss>>x;
                        std::getline(ss,name);
                        Textures.push_back(std::make_pair(pos(y,x),Texture(name,true)));
                    }
                }

            }
            inputCommands.close();
        }
        update();
        display();

    }

}
