Magic Mansion Map
===============

Devlog
========
Idea: A text-based puzzle/escape room game, about a Borneo Orangutan in a late Baroque/Rococo mansion/palace.

The rooms in the mansion can be re-arranged using the Magic Map.

This must be used to solve various puzzles.


To gate progress/learning: First the player can only translate rooms, then the player becomes able to rotate
Finally the player gains the ability to mirror rooms.

Rooms are only available, as they are added to the map.

Initial goals
-------
Saturday: Make map engine, allowing rooms to be loaded and displayed on the map, shifted, and described in the room view.

Sunday: Working inventory system, and interaction with items and exits

Monday:


Map visuals
--------
The map is a grid of 5 by 5 m blocks, Each Char represents one meter height and 2 m width!

The outer edge of the map looks like this:



    ++........++........++....++
    +                          +
    :                          :
    :                          :
    :                          :
    +                          +
    +                          +
    :                          :
    :                          :
    :                          :
    +                          +
    ++........++........++....++

A legend displayed, looking like this:


    /Legend~~~~~~~~~~~~~~~~~~
    \< ^ > v : Open 1m doors
    /@       : Closed  doors
    \ -- |   : 2m wall
    /"Chapel": Room name
    \"??????": unknown room
    / X      : You are here

A room on the map could look like this

    +---/\--.+
    |     .:C|
    <Entrance>
    |        |
    +---\/---+

The rooms are declared in blocks, each block-side can have 0,1,2 meter doors, which may be locked from this side.

Rooms can slide leftright or updown, but may not intersect, or be "loose"


Skripler engine
------------
Skripler is a terminal based ascii UI library for C++, developer for use in the GMTK Game Jam 2024.

Skripler is less capable than NCurses, but is hopefully faster to work in (important for a JAM), and is available on Windows.

Skripler renders windows to the terminal, windows can be text tables, textures textures, or canvases. Canvases can contain other windows.

Skripler reads lines terminated by enter, unlike NCurses, Skripler does not support "real-time" key presses.

Skripler has been developer PRIOR to the start of the JAM, this does not go against the rule, as it is no different from using a third party engine.

If I remember, I may link the Skripler github repository here.

WARNING
-------
Skripler development has been RUSHED, test driven practices have NOT been used. Bugs are bound to be as plentiful as some place which has a lot of bugs.
