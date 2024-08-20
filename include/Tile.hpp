#include<vector>
#pragma once

#include<iostream>
#include<string>
#include<cstdint>

//Smart C++ pointers, automatic memory management which works with polymorphism
#include<memory>

#include"Texture.hpp"

namespace skripler
{
    //Base class for all tiles, the base tile is a single resizable texture with a single char:
    class Tile
    {
    private:
        char c;
    protected:
        Texture myTex;
    public:
//Destructor is left empty, but explicitly declared virtual to silence warnings
        virtual ~Tile(){}
        Tile(uint32_t h, uint32_t w,char c='X');
        virtual void setSize(uint32_t h, uint32_t w);

        //Update the content of this Tile, and ALL sub-Tiles, most will leave this empty, but some might not
        virtual void update() {};//Default version only changes at set size, so do nothing here
        const Texture& getTexture() const {return myTex;}
    };

    //A tile, split left-right or up-down, the building block of everything else
    class BisectTile : public Tile
    {
    private:
        std::shared_ptr<Tile> first;
        std::shared_ptr<Tile> second;

        bool leftright=false;

        uint32_t first_min;//minimum width or height of the first window
        uint32_t second_min;//minimum width or height of the first window
        int max_size;//max size of the first tile if it is positive, and second if it is negative, if 0, there is no max_size

        uint32_t other_side;//The height or width not split

        uint32_t w;
        uint32_t h;

        bool border=false;

    public:
        ~BisectTile()
        {
        }

        //This does not allow null to be set
        void set_first(std::shared_ptr<Tile> newFirst);
        void set_second(std::shared_ptr<Tile> newSecond);

        //Because I don't have time to do propper checks, just make sure h_min and w_min are large enough to not allow below minimum size splits
        BisectTile(uint32_t h, uint32_t w, uint32_t _first_min, uint32_t _second_min, bool _leftright,bool border=false,int _max_size=0);

        //Update the content of this texture
        virtual void update();

        virtual void setSize(uint32_t h, uint32_t w);

    };

    //A non-scrollable word-wrapping wall of text, you can push text to it, NEWLINE forces a new line, words longer than the width is not displayed correctly write CLEAR to ... well.
    //Words MAY include spaces, (if you wish something to not be split), but WARNING, if a word is longer than the width, it WILL NOT be displayed
    class TextWall : public Tile
    {
    private:
        std::vector<std::string> content;

        std::string indent;
        std::string linestart;

        bool linenumbers;
        uint32_t h;
        uint32_t w;

    public:
        ~TextWall(){}
        TextWall(uint32_t _h, uint32_t _w,std::string _indent="   ",std::string _linestart=">",bool _linenumbers=true) : Tile(_h,_w,' '),indent(_indent),linestart(_linestart),linenumbers(_linenumbers), h(_h),w(_w){

                content=std::vector<std::string>();

        }
        virtual void setSize(uint32_t _h, uint32_t _w)
        {
            h=_h;
            w=_w;
            update();
        }

        void printword(const std::string& w);
        void printwords(const std::vector<std::string>& words);

        virtual void update();
    };

    //A tile which allows lines and textures to be drawn at select locations
    class Canvas : public Tile
    {
        public:

        struct pos
        {
            //Top left corner location, if positive it is relative to the left/top edge, if negative relative to right/bottom
            int x;
            int y;

            int getX(uint32_t w) const
            {
                return x>=0 ? x%w : (w-(-x)%w);
            }
            int getY(uint32_t h) const
            {
                return y>=0 ? y%h : (h-(-y)%h);
            }

            pos(int _y,int _x):x(_x),y(_y){}
        };

        private:

        uint32_t w;
        uint32_t h;

        char background=' ';


        //Textures are drawn ON TOP of each other, in the order they were added.
        std::vector<std::pair<pos,Texture> > Textures;
        struct line
        {
            pos p0;
            pos p1;
            std::string c;//Character (single character width, may be more than 1 byte)
            line(pos _p0, pos _p1,std::string _c): p0(_p0), p1(_p1), c(_c){}
        };
        std::vector<line> lines;

        public:

        void loadTxt(const std::string& input);

        Canvas(uint32_t h, uint32_t w,char bg=' ');

        void clear();//Clear EVERYTHING
        virtual void setSize(uint32_t h, uint32_t w);
        virtual void update();

        void drawDot(int y0, int x0,std::string c)
        {
            drawLine(y0,x0,y0,x0,c);
        }
        void drawLine(int y0, int x0, int y1, int x1,char c);

        void drawLine(int y0, int x0, int y1, int x1,std::string c);

        void drawTexture(int y, int x,const Texture& T);
    };
}
