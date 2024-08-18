#include"Tile.hpp"
#include<unordered_map>
#include<unordered_set>
#include<string>
#include<iostream>

enum direction {NORTH,SOUTH,EAST,WEST,UP,DOWN};

class Room
{
public:
    //doors are in the walls,

    struct Door
    {

        //Controls where we end up, and the display
        int x=0;
        int y=0;
        direction facing=NORTH;
        bool two_meter=false;
        std::string Name="door";
        int lock_ID=0;//unlocked

    };

private:



    //Rooms are made up of blocks, 5 by 5 meter
    int y,x,floor;//Location of north-west block.

    //Width and height of enclosing rectangle
    int width,height;

    std::vector<bool> blocks;

    //These are hash-tables, for fast lookup
    std::vector<Door> doors;
  //  std::unordered_set<std::pair<int,int> > blocks;//The blocks taken up by this room

    std::vector<std::string> description;//Used for detailed description
    std::string name;//Unique name, used for identifying this thing, should


public:
    Room(std::ifstream& RoomFile);

    int getX() const {return x;}
    int getY() const {return y;}
    int getfloor() const {return floor;}
    //Display the room on the map
    const std::vector<std::string>& getDescription() const {return description;}
    void map_display(std::shared_ptr<skripler::Canvas>& map,bool youAreHere=false) const;

    void  getNeighbours(direction dir, std::vector<std::pair<int,int> >& out) const;

    const std::string& getName() const {return name;}

    bool hasBlock(int _y, int _x,int _floor) const
    {
        int ly = _y-y;
        int lx = _x-x;

        return ly >= 0 && ly <height && lx >=0 && lx <width  && floor==_floor && blocks[ly*height+lx];
    }

    void slide(direction dir)
    {
        switch(dir)
        {
            case NORTH:
            --y;
            break;
            case SOUTH:
            ++y;
            break;
            case WEST:
            --x;
            break;
            case EAST:
            ++x;
            break;
            default:
            break;
        }
    }

    bool outBounds() const
    {
        return y+height>6 || y<0 || x+width>8 || x<0;
    }

    bool collide(const Room& that) const;

    size_t Nexits() const {return doors.size();}

    //Assume target is already valid
    Door getExit(int target) const
    {
        Door D = doors[target];//copy constructor
        D.x+=x; D.y+=y;//make global
        return D;
    }

    bool hasDoor(int f, Door& D) const
    {
        if (floor==f)
            for (const Door& d : doors)
            {
                if (floor==f && D.x==d.x+x && D.y==d.y+y && D.two_meter==d.two_meter && D.facing==d.facing)
                {
                    return true;
                }
            }
        return false;
    }
};
