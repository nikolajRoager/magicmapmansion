#include"Tile.hpp"
#include<unordered_map>
#include<unordered_set>
#include<string>
#include<iostream>
#include<set>

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
        std::string Name="door";
        int lock_ID=0;//unlocked

        std::vector<std::string> description;//Used for detailed description
    };

    struct Item
    {
        std::vector<std::string> name;//name is made up of multiple words
        std::vector<std::string> description;
        int hint;
    };

private:

    int key =0;//Is there a key in this room

    //Rooms are made up of blocks, 5 by 5 meter
    int y,x,floor;//Location of north-west block.

    //Width and height of enclosing rectangle
    int width,height;

    std::vector<bool> blocks;

    std::vector<Item> items;

    //These are hash-tables, for fast lookup
    std::vector<Door> doors;
  //  std::unordered_set<std::pair<int,int> > blocks;//The blocks taken up by this room

    std::vector<std::string> description;//Used for detailed description
    std::string name;//Unique name, used for identifying this thing, should

    //bool discovered=false;
public:
    int hasKey() const {return key;}
    //void discover() {discovered=true}

    Room(std::ifstream& RoomFile);

    void setPos(int _x, int _y)
    {
        x=_x;
        y=_y;
    }

    int getX() const {return x;}
    int getY() const {return y;}
    int getfloor() const {return floor;}
    //Display the room on the map
    const std::vector<std::string>& getDescription() const {return description;}

    const std::vector<std::string>& examine(std::vector<std::string>& target,int& status,int& hint) const;

    void map_display(std::shared_ptr<skripler::Canvas>& map,const std::set<int>& keys,int floor, bool youAreHere=false) const;

    void  getNeighbours(direction dir, std::vector<std::pair<int,int> >& out) const;

    const std::string& getName() const {return name;}

    bool hasBlock(int _y, int _x,int _floor) const
    {
        int ly = _y-y;
        int lx = _x-x;

        return ly >= 0 && ly <height && lx >=0 && lx <width  && floor==_floor && blocks[ly*width+lx];
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
        return y+height>6 || y<0 || x+width>7 || x<0;
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

    //0 if open, 1 if locked, 2 if wall, 3 if void
    int hasDoor(int f, Door& D,const std::set<int>& keys) const
    {
        if (!hasBlock(D.y,D.x,floor))
            return 3;
        if (floor==f)
            for (const Door& d : doors)
            {
                if (floor==f && D.x==d.x+x && D.y==d.y+y && D.facing==d.facing)
                {
                    return (keys.count(d.lock_ID)==1 || d.lock_ID==0)? 0 : 1;
                }
            }
        return 2;
    }
};
