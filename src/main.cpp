#include"skripler.hpp"
#include"Tile.hpp"
#include"Room.hpp"
#include<iostream>
#include<deque>
#include<fstream>


int main(int argc, char* argv[])
{


    int start_h = argc>1 ? std::stoi(std::string(argv[1])) : h_min;
    int start_w = argc>2 ? std::stoi(std::string(argv[2])) : w_min;

    //Then, set up the two windows we will be using:

    //The windows for the mape
    std::shared_ptr<skripler::BisectTile> map_base_tile = std::make_shared<skripler::BisectTile>(h_min, w_min, 32, 2, false,false,32);//Exactly 32 chars tall upper tile, the commands are at least 3 tall

    //The split between the textlog and map, this gives the map a fixed size 32x72
    std::shared_ptr<skripler::BisectTile> map_split_tile = std::make_shared<skripler::BisectTile>(32, w_min, 34, 72, true,false,-72);//Exactly 32 chars tall upper tile, the commands are at least 3 tall


    //The split of textlog and legend with a border
    std::shared_ptr<skripler::BisectTile> log_and_legend_split_tile = std::make_shared<skripler::BisectTile>(32, 34, 22, 8, false,true,-8);
    map_split_tile->set_first(log_and_legend_split_tile);

    //The main tile with the map, this has fixed size
    std::shared_ptr<skripler::Canvas> map_tile = std::make_shared<skripler::Canvas>(32, 72,' ');//Exactly 32 chars tall upper tile, the commands are at least 3 tall

    map_split_tile->set_second(map_tile);
    map_base_tile ->set_first(map_split_tile);


    skripler::Texture win_message("win_message.txt");


    skripler::Texture map_scale_background("map_scale.txt");

    //Add legend, this never changes
    std::shared_ptr<skripler::TextWall> legend_tile = std::make_shared<skripler::TextWall>(8,24,"","",false);//Exactly 32 chars tall upper tile, the commands are at least 3 tall
    log_and_legend_split_tile->set_second(legend_tile);
    legend_tile->printwords({" Legend","NEWLINE",
                                    "=A ||A:door with id A","NEWLINE",
                                    "1&    :locked door,id 1","NEWLINE",
                                    "   .  :","NEWLINE",
                                    " .::  :exit up/down","NEWLINE",
                                    " -- | :1m wall","NEWLINE",
                                    "Name  :Room name","NEWLINE",
                                    "YOU   :You"});

    std::shared_ptr<skripler::TextWall> map_log =std::make_shared<skripler::TextWall>(24,24,"","",false);
    log_and_legend_split_tile->set_first(map_log);


    std::shared_ptr<skripler::TextWall> command_log =std::make_shared<skripler::TextWall>(2,105,">","",false);
    command_log->printwords({
"Commands:", "change map \"floor up/down\"|","end game \"quit\"|","use door \"enter [exit letter]\"|","get help \"hint\"","NEWLINE",
"Move room \"slide north/south/east/west\" |","\"undo\" |","Explore with \"examine room\"/\"examine [item]\""});

    log_and_legend_split_tile->set_first(map_log);
    map_base_tile->set_second(command_log);


    std::vector<Room> rooms;
    std::vector<std::string> locknames({"always open","east-wing key","west-wing key","basement key","Baroness' private key"});

    std::vector<std::vector<std::string> > hints;

    //Load MANSION ... that should really be a class of its own, but I am in a hurry
    {
        std::ifstream mansion_file("mansion.txt");
        if (!mansion_file.is_open())
        {
            std::cout<<"could not open mansion.txt, check that you are running this from the file where it is"<<std::endl;
            return 1;
        }
        std::string input;
        while (mansion_file>>input)
        {
            if (input.compare("newroom")==0)
           {
                std::cout<<"Loading room "<<std::endl;
                rooms.push_back(Room(mansion_file));
            }
            else if (input.compare("hintstart")==0)
            {
                hints.push_back(std::vector<std::string>());
                while (mansion_file>>input)
                {
                    if (input.compare("hintend")==0)
                        break;
                    else
                    {
                        hints[hints.size()-1].push_back(input);

                    }
                }

            }


        }
        mansion_file.close();

    }

    struct gamestate
    {
        int floor=0;
        size_t yourRoom=0;
        std::set<int> keys;
        std::vector<int> xs;
        std::vector<int> ys;
        int hintlvl = 0;

        gamestate(const std::vector<Room>& rooms)
        {
            xs=std::vector<int>(rooms.size());
            ys=std::vector<int>(rooms.size());
            for (size_t i = 0; i < rooms.size();++i)
            {
                xs[i]=rooms[i].getX();
                ys[i]=rooms[i].getY();
            }
            floor=0;
            yourRoom=0;
            keys.insert(0);
            hintlvl =0;
        }
        void setRooms(std::vector<Room>& rooms)
        {
            for (size_t i = 0; i < rooms.size();++i)
                rooms[i].setPos(xs[i],ys[i]);
        }
    } gameState(rooms);


    std::deque<gamestate> oldStates;
    oldStates.push_back(gameState);

    skripler::start(map_base_tile,start_h,start_w);


    map_log->printwords(rooms[gameState.yourRoom].getDescription());


    std::vector<std::string> commands;

    bool win=false;
    while (true)
    {
        //I meant for there to be multiple modes, but changed my mind
        {
            map_tile->clear();

            if (win)
                map_tile->drawTexture(4,4,win_message);
            map_tile->drawTexture(0,0,map_scale_background);
            for (size_t i = 0; i < rooms.size(); ++i)
            {
                rooms[i].map_display(map_tile,gameState.keys,gameState.floor,gameState.yourRoom==i);
            }
            commands =skripler::getCommands();
            if (commands.size()>0)
            {
                map_log->printword("CLEAR");

                if (commands.size()==1 && (commands[0].compare("quit")==0))
                    break;
                else if (commands.size()>=1 && commands[0].compare("undo")==0)
                {
                    if (oldStates.size()>1)
                    {
                        oldStates.pop_back();
                        gameState=oldStates.back();
                        gameState.setRooms(rooms);
                    }
                    else
                        map_log->printwords({"no","former","steps"});



                }
                else if (commands.size()>=2 && commands[0].compare("floor")==0)
                {
                    if (commands[1].compare("up")==0 && gameState.floor<=1)
                    {
                        ++gameState.floor;
                        oldStates.push_back(gameState);
                    }
                    else if (commands[1].compare("down")==0 && gameState.floor>=-1)
                    {
                        --gameState.floor;
                        oldStates.push_back(gameState);
                    }
                    else
                        map_log->printwords({"not","valid","should","be","\":floor up/down\""});



                }
                else if (commands.size()>=1 && commands[0].compare("hint")==0)//debug
                {
                    map_log->printwords(hints[gameState.hintlvl]);
                }
                else if (commands.size()>=2 && commands[0].compare("tp")==0)//debug
                {
                    gameState.yourRoom=std::stoi(commands[1]);
                    gameState.floor=rooms[gameState.yourRoom].getfloor();
                    oldStates.push_back(gameState);
                }
                else if (commands.size()>=2 && commands[0].compare("examine")==0)
                {
                    if (commands[1].compare("room")==0)
                    {
                        map_log->printwords(rooms[gameState.yourRoom].getDescription());
                    }
                    else
                    {
                        int status=0;
                        map_log->printwords(rooms[gameState.yourRoom].examine(commands,status,gameState.hintlvl));
                        if (status==1)
                        {
                            if (commands[1].compare("box")==0)//Happens to be the winning move
                            {
                                win=true;
                            }
                        }

                    }

                }
                else if (commands.size()>=2 && commands[0].compare("enter")==0)
                {
                    int target =commands[1][0]-'a';
                    if (commands[1].size()!=1 || (target<0 || target>=static_cast<int>(rooms[gameState.yourRoom].Nexits())))//Also captures non-ascii
                    {
                        map_log->printwords({"No","exit","named",commands[1]});
                    }
                    else
                    {
                        Room::Door D = rooms[gameState.yourRoom].getExit(target);

                        bool locked = gameState.keys.count(D.lock_ID)==0 && D.lock_ID!=0;
                        if (locked)
                        {
                            map_log->printwords({"The","door","is","locked","from","this","side","using","key",std::to_string(D.lock_ID)," ("+locknames[D.lock_ID]+")","NEWLINE","Hint:","replacing","this","door,","with","an","unlocked","door","might","work"});

                        }
                        else
                        {

                            int Floor = rooms[gameState.yourRoom].getfloor();
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
                                --Floor;
                                break;
                                case UP:
                                D.facing=DOWN;
                                ++Floor;
                                break;
                            }
                            bool fail=true;
                            bool Void=true;
                            for (size_t j = 0; j<rooms.size(); ++j)
                            {
                                int result =rooms[j].hasDoor(Floor,D,gameState.keys);

                                if (result!=3)
                                    Void=false;

                                if (result==1)//found but it is locked
                                {
                                    map_log->printwords({"The","door","is","locked","from","the","other"});
                                    fail=true;
                                    break;
                                }

                                else if (result==0)
                                {
                                    gameState.floor =Floor;
                                    gameState.yourRoom=j;
                                    int key =rooms[gameState.yourRoom].hasKey();
                                    if (gameState.keys.count(key)==0)
                                    {
                                        gameState.keys.insert(key);
                                    }
                                    oldStates.push_back(gameState);

                                    map_log->printwords(rooms[gameState.yourRoom].getDescription());


                                    fail = false;
                                    break;
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
                            rooms[gameState.yourRoom].getNeighbours(NORTH,slide_targets);
                            rooms[gameState.yourRoom].getNeighbours(SOUTH,slide_targets);
                        }
                        else
                        {
                            rooms[gameState.yourRoom].getNeighbours(WEST,slide_targets);
                            rooms[gameState.yourRoom].getNeighbours(EAST,slide_targets);
                        }

                        bool has_slide_target=false;
                        //Check all other rooms for any of these neighbours we can slide against
                        for (std::pair<int,int>& trg : slide_targets)
                        {
                            std::cout<<trg.first<<" "<<trg.second<<std::endl;
                            for (size_t j = 0; j <rooms.size(); ++j)
                            {
                                if (j!=gameState.yourRoom)
                                    if (rooms[j].hasBlock(trg.first,trg.second,rooms[gameState.yourRoom].getfloor()))
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
                            Room newRoom = rooms[gameState.yourRoom];//copy constructor
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
                                    for (size_t j = 0; j <rooms.size(); ++j)
                                    {
                                        if (j!=gameState.yourRoom)
                                        {
                                            if (rooms[j].hasBlock(trg.first,trg.second,rooms[gameState.yourRoom].getfloor()))
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
                                    rooms[gameState.yourRoom]=newRoom;
                                    gameState.xs[gameState.yourRoom]=newRoom.getX();
                                    gameState.ys[gameState.yourRoom]=newRoom.getY();


                                    oldStates.push_back(gameState);
                                }
                                else
                                {
                                    map_log->printwords({"To","be","able","to","slide"});

                                    if (dir==WEST || dir==EAST)
                                        map_log->printwords({"east","or","west,"});
                                    else
                                        map_log->printwords({"north","or","south,"});
                                    map_log->printwords({"your", "room" ,"need","to", "be", "touching", "another", "room", "to", "the"});

                                    if (dir==WEST || dir==EAST)
                                        map_log->printwords({"north","or","south,"});
                                    else
                                        map_log->printwords({"east","or","west,"});

                                    map_log->printwords({"both","before","and","after","the","slide","(Out","of","bound","does","not","count)"});

                                }
                            }


                        }
                        else
                        {
                            map_log->printwords({"To","be","able","to","slide"});

                            if (dir==WEST || dir==EAST)
                                map_log->printwords({"east","or","west,"});
                            else
                                map_log->printwords({"north","or","south,"});
                            map_log->printwords({"your", "room" ,"need","to", "be", "touching", "another", "room", "to", "the"});

                            if (dir==WEST || dir==EAST)
                                map_log->printwords({"north","or","south,"});
                            else
                                map_log->printwords({"east","or","west,"});

                            map_log->printwords({"both","before","and","after","the","slide","(Out","of","bound","does","not","count)"});
                        }

                    }
                }
                else
                    map_log->printwords({"not","valid","command"});
            }
        }
    }

    skripler::end();
}
