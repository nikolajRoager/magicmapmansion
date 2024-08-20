#pragma once

#include<vector>
#include<string>
#include "Tile.hpp"

#define w_min 106
#define h_min 34

namespace skripler
{
    //Launch threads for actively monitoring screen resizing
    void start(std::shared_ptr<Tile> baseTile = std::make_shared<Tile>(h_min,w_min), int _h=h_min+1,int _w=w_min+1);
    //Join the above threads
    void end();

    void printWH();

    std::vector<std::string> getCommands();
}
