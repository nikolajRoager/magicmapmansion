#include"skripler.hpp"
#include"Tile.hpp"
#include"Room.hpp"
#include<iostream>
#include<fstream>


int main()
{
    //First thing first, print info splashscreen
    //TEMP DO IT


    //Then, set up the two windows we will be using:

    //The windows for the mape
    std::shared_ptr<skripler::BisectTile> map_base_tile = std::make_shared<skripler::BisectTile>(h_min, w_min, 32, 2, false,false,32);//Exactly 32 chars tall upper tile, the commands are at least 3 tall

    //The split between the textlog and map, this gives the map a fixed size 32x82
    std::shared_ptr<skripler::BisectTile> map_split_tile = std::make_shared<skripler::BisectTile>(32, w_min, 24, 82, true,false,-82);//Exactly 32 chars tall upper tile, the commands are at least 3 tall


    //The split of textlog and legend with a border
    std::shared_ptr<skripler::BisectTile> log_and_legend_split_tile = std::make_shared<skripler::BisectTile>(32, 24, 24, 24, false,true,-8);
    map_split_tile->set_first(log_and_legend_split_tile);

    //The main tile with the map, this has fixed size
    std::shared_ptr<skripler::Canvas> map_tile = std::make_shared<skripler::Canvas>(32, 82,' ');//Exactly 32 chars tall upper tile, the commands are at least 3 tall

    map_split_tile->set_second(map_tile);
    map_base_tile ->set_first(map_split_tile);

    skripler::Texture map_scale_background("map_scale.txt");
    map_tile->drawTexture(0,0,map_scale_background);

    //Add legend, this never changes
    std::shared_ptr<skripler::TextWall> legend_tile = std::make_shared<skripler::TextWall>(8,24,"","",false);//Exactly 32 chars tall upper tile, the commands are at least 3 tall
    log_and_legend_split_tile->set_second(legend_tile);
    legend_tile->printwords({" Legend","NEWLINE",
                                    "< ^ > v  :1m door","NEWLINE",
                                    "= ||     :2m door","NEWLINE",
                                    "   . .   :","NEWLINE",
                                    " .:: ::. :exit up/down","NEWLINE",
                                    "  -- |   :1m wall","NEWLINE",
                                    "Name     :Room name","NEWLINE",
                                    "YOU      :You"});

    std::shared_ptr<skripler::TextWall> map_log =std::make_shared<skripler::TextWall>(24,24,"","",false);
    log_and_legend_split_tile->set_first(map_log);


    std::shared_ptr<skripler::TextWall> command_log =std::make_shared<skripler::TextWall>(2,105,">","",false);
    command_log->printwords({
"Commands:", "\":close\" close map |", "\"floor up/down\" move map |","\"quit\" close game |","\":enter [exit letter]\"","NEWLINE",
"Move your room with:","\":slide north/south/east/west\""});

    log_and_legend_split_tile->set_first(map_log);
    map_base_tile->set_second(command_log);


    size_t yourRoom=2;

    std::vector<Room> rooms;


    //Load MANSION ... that should really be a class of its own, but I am in a hurry
    {
        std::ifstream mansion_file("mansion.txt");
        if (!mansion_file.is_open())
        {
            std::cout<<"could not open mansion.txt, check that you are running this from the file where it is"<<std::endl;
        }
        std::string input;
        while (mansion_file>>input)
        {
            if (input.compare("newroom")==0)
            {
                std::cout<<"Load room"<<std::endl;
                rooms.push_back(Room(mansion_file));
            }
        }
        mansion_file.close();

    }


    skripler::start(map_base_tile);




    std::vector<std::string> commands;

    bool map_mode =true;//TEMP

    while (true)
    {
        if (map_mode)
        {
            map_tile->clear();
            map_tile->drawTexture(0,0,map_scale_background);
            for (size_t i = 0; i < rooms.size(); ++i)
            {
                rooms[i].map_display(map_tile,yourRoom==i);
            }
            commands =skripler::getCommands();
            if (commands.size()>0)
            {
                map_log->printword("CLEAR");

                if (commands.size()==1 && (commands[0].compare("quit")==0))
                    break;
                else if (commands.size()>=2 && commands[0].compare("enter")==0)
                {
                    int target =commands[1][0]-'a';
                    if (commands[1].size()!=1 || (target<0 || target>=static_cast<int>(rooms[yourRoom].Nexits())))//Also captures non-ascii
                    {
                        map_log->printwords({"No","exit","named",commands[1]});
                    }
                    else
                    {

                        Room::Door D = rooms[yourRoom].getExit(target);
                        int floor = rooms[yourRoom].getfloor();
                        //Offset the door to find our match
                        switch(D.facing)
                        {
                            case WEST:
                            D.facing=EAST;
                            --D.x;
                            break;
                            case EAST:
                            D.facing=WEST;
                            ++D.x;
                            break;
                            case NORTH:
                            D.facing=SOUTH;
                            --D.y;
                            break;
                            case SOUTH:
                            D.facing=NORTH;
                            ++D.y;
                            break;
                            case DOWN:
                            D.facing=UP;
                            --floor;
                            break;
                            case UP:
                            D.facing=DOWN;
                            ++floor;
                            break;
                        }
                        bool fail=true;
                        bool Void=true;
                        for (size_t j = 0; j<rooms.size(); ++j)
                        {
                            if (rooms[j].hasBlock(D.y,D.x,floor))
                            {
                                Void=false;

                                if (rooms[j].hasDoor(floor,D))
                                {
                                    map_log->printwords({"You","take","the",D.Name,"to",rooms[j].getName()});
                                    yourRoom=j;
                                    fail = false;
                                    break;
                                }
                            }
                        }

                        if (fail)
                        {
                            if (Void)
                                map_log->printwords({"The",D.Name,"leads","to","an","endles","black","magical","void"});
                            else
                                map_log->printwords({"The",D.Name,"is","blocked","(by","a","wall)"});
                        }
                    }

                }
                else if (commands.size()>=2 && commands[0].compare("slide")==0)
                {
                    direction dir=UP;

                    if (commands[1].compare("north")==0)
                        dir=NORTH;
                    if (commands[1].compare("south")==0)
                        dir=SOUTH;
                    if (commands[1].compare("west")==0)
                        dir=WEST;
                    if (commands[1].compare("east")==0)
                        dir=EAST;

                    if (dir==UP)
                        map_log->printwords({"Invalid","direction",commands[1]});
                    else
                    {
                        //Check what it is we can slide against
                        std::vector<std::pair<int,int> > slide_targets;

                        if (dir==WEST || dir==EAST)
                        {
                            rooms[yourRoom].getNeighbours(NORTH,slide_targets);
                            rooms[yourRoom].getNeighbours(SOUTH,slide_targets);
                        }
                        else
                        {
                            rooms[yourRoom].getNeighbours(WEST,slide_targets);
                            rooms[yourRoom].getNeighbours(EAST,slide_targets);
                        }

                        bool has_slide_target=false;
                        //Check all other rooms for any of these neighbours we can slide against
                        for (std::pair<int,int>& trg : slide_targets)
                        {
                            std::cout<<trg.first<<" "<<trg.second<<std::endl;
                            for (size_t j = 0; j <rooms.size(); ++j)
                            {
                                if (j!=yourRoom)
                                    if (rooms[j].hasBlock(trg.first,trg.second,rooms[yourRoom].getfloor()))
                                    {
                                        std::cout<<"Found"<<std::endl;
                                        has_slide_target=true;
                                        break;
                                    }
                            }

                            if (has_slide_target)
                                break;
                        }




                        if (has_slide_target)
                        {
                            //Ok, try
                            Room newRoom = rooms[yourRoom];//copy constructor
                            newRoom.slide(dir);

                            if (newRoom.outBounds())
                            {
                                map_log->printwords({"That","is","out","of","bounds!"});
                            }
                            else
                            {


                                //Copy pasted code to check that we can slide somewhere (and can get back afterwards)
                                std::vector<std::pair<int,int> > slide_targets;

                                if (dir==WEST || dir==EAST)
                                {
                                    newRoom.getNeighbours(NORTH,slide_targets);
                                    newRoom.getNeighbours(SOUTH,slide_targets);
                                }
                                else
                                {
                                    newRoom.getNeighbours(WEST,slide_targets);
                                    newRoom.getNeighbours(EAST,slide_targets);
                                }

                                bool has_slide_target=false;
                                bool has_collision=false;
                                for (std::pair<int,int>& trg : slide_targets)
                                {
                                    std::cout<<trg.first<<" "<<trg.second<<std::endl;
                                    for (size_t j = 0; j <rooms.size(); ++j)
                                    {
                                        if (j!=yourRoom)
                                        {
                                            if (rooms[j].hasBlock(trg.first,trg.second,rooms[yourRoom].getfloor()))
                                            {
                                                has_slide_target=true;
                                                break;
                                            }
                                            if (rooms[j].collide(newRoom))
                                            {
                                                has_collision=true;
                                                break;

                                            }
                                        }
                                    }
                                    if (has_collision)
                                        break;
                                }


                                if (has_collision)
                                {
                                    map_log->printwords({"there", "is", "a", "room", "in", "the", "way"});

                                }
                                else if (has_slide_target)
                                {
                                    map_log->printwords({"You", "hear", "a", "faint", "magical", "humm,", "and", "feel", "the", "room", "rumble", "as", "it", "moves"});
                                    //Swap back
                                    rooms[yourRoom]=newRoom;
                                }
                                else
                                    map_log->printwords({"There","is","nothing","to","slide","to"});
                            }


                        }
                        else
                            map_log->printwords({"There","is","nothing","to","slide","against"});

                    }
                }
            }
        }
    }

    skripler::end();
}
