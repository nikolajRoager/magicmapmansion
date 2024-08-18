#include "Room.hpp"
#include<fstream>
#include<iostream>


//Cheaky global variable
skripler::Texture exitUp("stairup.txt");
skripler::Texture exitDown("stairdown.txt");

Room::Room(std::ifstream& RoomFile)
{
    std::string input;

    x=0;
    y=0;
    floor=0;
    name="null";

    while (RoomFile>>input)
    {

        if (input.compare("endroom")==0)
            break;
        else if (input.compare("x")==0)
            RoomFile>>x;
        else if (input.compare("y")==0)
            RoomFile>>y;
        else if (input.compare("floor")==0)
            RoomFile>>floor;
        else if (input.compare("name")==0)
            std::getline(RoomFile,name);
        else if (input.compare("startdsc")==0)
        {
    //Read entire description, word for word
            while (RoomFile>>input)
            {
                if (input.compare("enddsc")==0)
                    break;
                else
                    description.push_back(input);
            }
        }
        else if (input.compare("blocks")==0)
        {
            //How many blocks wide and tall is this room

            RoomFile>>height;
            RoomFile>>width;

            blocks = std::vector<bool>(height*width);
            int occupied;
            for (int i = 0; i < height*width; ++i)
            {
                RoomFile>>occupied;
                blocks[i]=(occupied==1);
            }
        }
        else if (input.compare("exit")==0)
        {
            Door D;
            RoomFile>>D.y;
            RoomFile>>D.x;
            std::string dir;
            RoomFile>>dir;
            if (dir.compare("NORTH")==0)
                D.facing=NORTH;
            if (dir.compare("SOUTH")==0)
                D.facing=SOUTH;
            if (dir.compare("WEST")==0)
                D.facing=WEST;
            if (dir.compare("EAST")==0)
                D.facing=EAST;
            if (dir.compare("UP")==0)
                D.facing=UP;
            if (dir.compare("DOWN")==0)
                D.facing=DOWN;

            int size;
            RoomFile>>size;
            D.two_meter=size==2;
            RoomFile>>D.Name
            RoomFile>>D.lock_ID;
            if (D.Name.compare("name")==0)
                std::getline(RoomFile,D.Name);
            std::cout<<D.Name<<std::endl;
            doors.push_back(D);
        }
    }

}

#define char_per_blockwidth  10
#define char_per_blockheight 5

void Room::map_display(std::shared_ptr<skripler::Canvas>& map, bool youAreHere) const
{
    //Loop through all blocks local coordinate, if the tile east, west, north or south is unoccupied, draw the wall

    for (int ly = 0; ly<height; ++ly)
        for (int lx = 0; lx<width; ++lx)
            if (blocks[ly*width+lx])
            {
                //North not in this room
                if (ly==0 || !blocks[(ly-1)*width+lx])
                {
                    map->drawLine(1+(y+ly)*char_per_blockheight,1+(x+lx)*char_per_blockwidth,1+(y+ly)*char_per_blockheight,1+(x+1+lx)*char_per_blockwidth-1,'-');

                    map->drawDot(1+(y+ly)*char_per_blockheight,1+(x+lx)*char_per_blockwidth,"+");
                    map->drawDot(1+(y+ly)*char_per_blockheight,1+(x+1+lx)*char_per_blockwidth-1,"+");

                }
                if (ly+1==height || !blocks[(ly+1)*width+lx])//south
                {
                    map->drawLine(1+(1+y+ly)*char_per_blockheight-1,1+(x+lx)*char_per_blockwidth,1+(1+y+ly)*char_per_blockheight-1,1+(x+1+lx)*char_per_blockwidth-1,'-');

                    map->drawDot(1+(1+y+ly)*char_per_blockheight-1,1+(x+lx)*char_per_blockwidth,"+");
                    map->drawDot(1+(1+y+ly)*char_per_blockheight-1,1+(x+1+lx)*char_per_blockwidth-1,"+");
                }
                if (lx==0 || !blocks[(ly)*width+lx-1])//west
                {
                    map->drawLine(1+(y+ly)*char_per_blockheight,1+(x+lx)*char_per_blockwidth,1+(y+ly+1)*char_per_blockheight-1,1+(x+lx)*char_per_blockwidth,'|');
                    map->drawDot(1+(y+ly)*char_per_blockheight,1+(x+lx)*char_per_blockwidth,"+");
                    map->drawDot(1+(y+ly+1)*char_per_blockheight-1,1+(x+lx)*char_per_blockwidth,"+");
                }
                if (lx+1==width || !blocks[(ly)*width+lx+1])//East
                {
                    map->drawLine(1+(y+ly)*char_per_blockheight,1+(x+lx+1)*char_per_blockwidth-1,1+(y+ly+1)*char_per_blockheight-1,1+(x+lx+1)*char_per_blockwidth-1,'|');

                    map->drawDot(1+(y+ly)*char_per_blockheight,1+(x+lx+1)*char_per_blockwidth-1,"+");
                    map->drawDot(1+(y+ly+1)*char_per_blockheight-1,1+(x+lx+1)*char_per_blockwidth-1,"+");
                }



            }

    //Loop using chars, it is fine, there are not more than 128 doors, and this allows us to easilly print the ID
    for (char a = 0; a < static_cast<char>(doors.size()); ++a)
    {
        const Door& D = doors[a];
        //Place the door in the middle of the wall at this room

        if (D.facing==WEST)
        {
            map->drawDot(1+(y+D.y)*char_per_blockheight+char_per_blockheight/2,1+(x+D.x)*char_per_blockwidth,D.two_meter?"=":"<");
            //Label the doors from A and up, next to the door itself
            if (youAreHere)
                map->drawDot(1+(y+D.y)*char_per_blockheight+char_per_blockheight/2+1,1+(x+D.x)*char_per_blockwidth,std::string(1,static_cast<char>('A'+a)));
        }
        else if (D.facing==EAST)
        {
            map->drawDot(1+(y+D.y)*char_per_blockheight+char_per_blockheight/2,1+(x+1+D.x)*char_per_blockwidth-1,D.two_meter?"=":">");
            //Label the doors from A and up, next to the door itself
            if (youAreHere)
                map->drawDot(1+(y+D.y)*char_per_blockheight+char_per_blockheight/2+1,1+(x+1+D.x)*char_per_blockwidth-1,std::string(1,static_cast<char>('A'+a)));
        }
        else if (D.facing==NORTH)
        {
            //It looks better if the width it is rounded down
            map->drawDot(1+(y+D.y)*char_per_blockheight,1+(x+D.x)*char_per_blockwidth+(char_per_blockwidth-1)/2,D.two_meter?"|":"^");

            //Wider door if it is ... you know, wide
            if (D.two_meter)
                map->drawDot(1+(y+D.y)*char_per_blockheight,1+(x+D.x)*char_per_blockwidth+(char_per_blockwidth-1)/2+1,"|");

            //Label the doors from A and up, next to the door itself
            if (youAreHere)
                map->drawDot(1+(y+D.y)*char_per_blockheight,1+(x+D.x)*char_per_blockwidth+(char_per_blockwidth-1)/2+(D.two_meter?2:1),std::string(1,static_cast<char>('A'+a)));
        }
        else if (D.facing==SOUTH)
        {
            map->drawDot(1+(y+1+D.y)*char_per_blockheight-1,1+(x+D.x)*char_per_blockwidth+(char_per_blockwidth-1)/2,D.two_meter?"|":"v");

            //Wider door if it is ... you know, wide
            if (D.two_meter)
                map->drawDot(1+(y+1+D.y)*char_per_blockheight-1,1+(x+D.x)*char_per_blockwidth+(char_per_blockwidth-1)/2+1,"|");

            //Label the doors from A and up, next to the door itself
            if (youAreHere)
                map->drawDot(1+(1+y+D.y)*char_per_blockheight-1,1+(x+D.x)*char_per_blockwidth+(char_per_blockwidth-1)/2+(D.two_meter?2:1),std::string(1,static_cast<char>('A'+a)));
        }
        else//Up or down
        {
            //Draw in upper right corner of the room
            map->drawTexture(1+(y+D.y)*char_per_blockheight,1+(1+x+D.x)*char_per_blockwidth-1-3/*I totally know it is 3 wide, sorry for all the hardcoding*/,D.facing==UP?exitUp:exitDown);
            if (youAreHere)
                map->drawDot(1+(y+D.y)*char_per_blockheight+1-1,1+(1+x+D.x)*char_per_blockwidth-1-2,std::string(1,static_cast<char>('A'+a)));

        }


    }
        //Finally draw the name of the room name, and if you are here, draw you
        //This should be drawn at the middle height-wise, and at the start of the middle block (rounded down)

        map->drawTexture(1+(y)*char_per_blockheight+1,1+(x+(width-1)/2)*char_per_blockwidth+1,skripler::Texture(name,false));

        if (youAreHere)
            map->drawTexture(1+(y)*char_per_blockheight+2,1+(x+(width-1)/2)*char_per_blockwidth+2,skripler::Texture("YOU",false));
}

void  Room::getNeighbours(direction dir, std::vector<std::pair<int,int> >& out) const
{
    for (int ly = 0; ly<height; ++ly)
        for (int lx = 0; lx<width; ++lx)
        {
            if (blocks[ly*width+lx])
            {
                //For each occupied block, check that the appropriate neighbour is unoccupied or empty, if so, a neighbour there can be slided against
                switch(dir)
                {
                    case NORTH:
                    default:
                    if (ly==0 || !blocks[(ly-1)*width+lx])
                        out.push_back(std::make_pair(ly-1+y,lx+x));
                    break;
                    case SOUTH:
                    if (ly+1==height || !blocks[(ly+1)*width+lx])
                        out.push_back(std::make_pair(ly+1+y,lx+x));
                    break;
                    case WEST:
                    if (lx==0 || !blocks[(ly)*width+lx-1])
                        out.push_back(std::make_pair(ly+y,lx-1+x));
                    break;
                    case EAST:
                    if (lx+1==width || !blocks[(ly)*width+lx+1])
                        out.push_back(std::make_pair(ly+y,lx+1+x));
                    break;
                }

            }
        }
}

bool Room::collide(const Room& that) const
{
    //Simply loop through everything and check if it is the same

    for (int ly = 0; ly<height; ++ly)
        for (int lx = 0; lx<width; ++lx)
        {
            if (blocks[ly*width+lx])
            {
                int gy=ly+y;
                int gx=lx+x;

                if (that.hasBlock(gy,gx,floor))
                    return true;

            }
        }

    return false;
}
