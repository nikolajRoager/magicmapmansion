Magic Mansion Map: A Unicode Terminal game
===============
WALKTHROUGH AT THE BOTTOM OF THIS FILE.

PLEASE READ SETUP GUIDE ON WINDOWS OR LINUX OR MAC BEFORE RUNNING.

You are about to play a text-based puzzle game in your terminal, about a Borneo Orangutan in a late Baroque/Rococo mansion/palace. You are tasked with finding a hidden document in this mansion.

There is also a murder mystery; solving it is not required to win.

The rooms in the mansion can be re-arranged using your Magic Map. Use this to bypass locks, find clues, and find keys to unlock the mansion.

THE GAME SHOULD NEVER ASK FOR PERMISSION TO WRITE FILES

BEWARE OF BUGS.

How this fits the theme: Build to scale
------------
You are using the building plans, of a building to explore the building, and rearrange the rooms, the size and shape of the rooms is central to solving the puzzles.


How to play a terminal game
------------
If the folder `build` has been included in the download DO NOT RUN THE BINARIES IN THERE. It is a temporary debugging build made for Linux DO NOT RUN ON WINDOWS. the `build` folder should not have been part of the upload.

You have to run this game in your terminal (Windows, Mac and Linux instructions for how to turn on the game).

This is not an ASCII game: The game uses and supports Unicode characters. You need to make sure that your terminal is using a font, which at the very least support these characters: æ, ø, å, ß, ¤. You also need to make sure your terminal is using a MONOSPACED font: i.e. a font where all characters have the same width and height. Otherwise the display will be all messed up.

You also need to make sure that your terminal has the correct size for the game to work.

The Linux version IS the superior version (it supports automatic terminal resize detection), and Linux terminals are generally better than Windows terminals.



Starting the game on Windows 10
--------
I have not tested windows 11.

You have to run this game in a terminal, on windows 10 I recommend windows powershell (bundled with Windows 10). I recommend changing the background color to black, rather than the default blue if you haven't already.

THE WINDOWS VERSION DO NOT SUPPORT AUTOMATIC SIZE DETECTION! (The Linux version does, because Linux is better at terminals than windows)



The included executable `maptrial.exe` should work on Windows.

... TEMP

You can also recompile the C++ code.

I use only C++ standard libraries (plus windows or unix libraries to get Terminal size) so you only need a C++ compiler, I have used MinGW's Windows port of the Gnu G++ compiler. I do not know if Visual Studio compilers work.

I do not know if the included CMakelist.txt file allows CMake can compile it on windows, I have only used in on Linux.

Starting the game on Linux (or Mac)
--------
So you have realized that Windows is bad! Great! To show your superiority over Windows users, you will have to compile the C++ code, I use standard libraries (plus windows or unix libraries to get Terminal size) so you only need a compiler, I used g++. You should also have CMake installed, to build the project.

The game is made and tested on Arch Linux, I do not own a Mac, but Mac and Linux are very similar under the hood so the Linux code should compile and run on Mac.

g++ and CMake are available on Mac, I recommend using the Homebrew package manager (If you don't feel comfortable installing these things on Mac, you may want to skip this game, or use the windows version)

CMake makes compiling easy, just open your terminal in the projects folder, and run `cmake -Bbuild`, followed by `cmake --build build` this builds the executable in the file `build/bin/maptrial`

You will need to execute this file, while your terminal is still in the projects root folder, write: `build/bin/maptrial` in your terminal, the game should start.


Skripler engine DISCLAIMER
------------
Skripler is a terminal based ascii UI library for C++, developer for use in the GMTK Game Jam 2024.

Skripler is less capable than NCurses, but is hopefully faster to work in (important for a JAM), and supports Unicode better

Skripler renders tiles to the terminal, tiles can be text, split-screens, textures, or canvases.

Skripler reads lines terminated by enter, unlike NCurses, Skripler does not support "real-time" key presses.

Skripler has been developer PRIOR to the start of the JAM, this does not go against the rule, as it is no different from using a third party engine.

If I remember, I may link the Skripler github repository here.


Walkthrough, (spoilers)
----------------
In the entrance, a note tells you that key 2 is in the Pavilion. It will take a long time to get there.

Another note in the kitchen tells us that key 1 is in the Butler's room.

To get there from the starting setup, slide the ball-room south, then enter the banquet hall, then the kitchen, then the entrance, slide that south as well and enter the ball-room, then the kitchen. Slide the kitchen north, then west and enter the entrance. Your game should look something like this:

    ++........++........++........++........++........++........++........++
    ++--------++--------+          +--------+          +--------++--------++
    :| Library          |          | Pavilion          | Eastwing| Ågesen |:
    :|                  =          =        |          |        ==        |:
    :|                  |          |        |          |        ||        |:
    ++                  +          +--------+          +        ++--------++
    ++                  ++--------++--------+          +         +-------.++
    :|                  || Kitchen          |          |               .::|:
    :|                  |=                  |          |                  |:
    :|                  ||                  |          |                  |:
    ++---||---++---||---++--------+         +          +--------++---||---++
    ++---||---++---||---+          +        +                    +---||---++
    :| Study  || Corridor          |        1                    | Gallery|:
    :|        ==        =          =        &                    |        |:
    :|        ||        |          |        |                    |        |:
    ++        ++        +          +---||---+                    +        ++
    ++        ++        ++--------++---||AE.+                    +        ++
    :|        ||        |2 the entrance  .::|                    |        |:
    :|        ||        |& YOU              =                    =        |:
    :|        ||        |C                  D                    |        |:
    ++--------++        ++---||B--++--------+                    +---||---++
    ++-----.--++        ++---||---++--------++--------++---||---++---1&---++
    :| B.  ::.|3        2|           Grand ballroom   || Banquet hall     |:
    :|        =&        &|                            ==                  |:
    :|        ||        ||                            ||                  |:
    ++--------++--------++                            ++--------++--------++
    +                    +                            +                    +
    :                    |                            |                    :
    :                    =                            =                    :
    :                    |                            |                    :
    +                    +--------++--------++--------+                    +
    ++........++........++........++........++........++........++........++

No slide the entrance east twice, this allows you to enter the gallery, east-wing and the Butler's room in the north east.

Here you find the key to unlock key 1, now we can access the top floor! a letter suggest that we look in the chappel upstairs for a key to the basement.

Going upstairs we see that the chapel requires the key in the pavilion, we also find evidence that the Baroness' key has been stolen, and likely is at the escape route (likely the basement).

To get into the pavilion, we need to move the kitchen down one step, to do that we need to enter the kitchen, while the entrance and ball-room are out of the way.

To do this from the position above, move the gallery west twice, then enter the entrance and go to the banquet hall. Slide that north twice and enter the east-wing. Go upstairs, and back down in the entrance. To do that, you need to move the balcony down one step. After that, your game looks like this downstairs

    ++........++........++........++........++........++........++........++
    ++--------++--------+          +--------+          +--------++--------++
    :| Library          |          | Pavilion          | Eastwing| Ågesen |:
    :|                  =          =        |          |        ==        |:
    :|                  |          |        |          |        ||        |:
    ++                  +          +--------+          +        ++--------++
    ++                  ++--------++--------+          +         +-------.++
    :|                  || Kitchen          |          |               .::|:
    :|                  |=                  |          |                  |:
    :|                  ||                  |          |                  |:
    ++---||---++---||---++--------+         +          +--------++---||---++
    ++---||---++---||---+          +        ++---||---++---||---++---||---++
    :| Study  || Corridor          |        || Gallery|| Banquet hall     |:
    :|        ==        =          =        =|        |=                  |:
    :|        ||        |          |        ||        ||                  |:
    ++        ++        +          +---||---++        ++--------++--------++
    ++        ++        ++--------++---||AE.++        +                    +
    :|        ||        |2 the entrance  .::||        |                    :
    :|        ||        |& YOU              ==        |                    :
    :|        ||        |C                  D|        |                    :
    ++--------++        ++---||B--++--------++---||---+                    +
    ++-----.--++        ++---||---++--------++--------+                    +
    :| B.  ::.|3        2|           Grand ballroom   |                    :
    :|        =&        &|                            =                    :
    :|        ||        ||                            |                    :
    ++--------++--------++                            +                    +
    +                    +                            +                    +
    :                    |                            |                    :
    :                    =                            =                    :
    :                    |                            |                    :
    +                    +--------++--------++--------+                    +
    ++........++........++........++........++........++........++........++

Now go to the ballroom and move that two steps east, then enter the entrance through the gallery, and move the entrance two steps down. You should now be able to enter the gallery through the ballroom. Slide the gallery north once, enter the kitchen, and slide south once, now your lower floor looks like this:

    ++........++........++........++........++........++........++........++
    ++--------++--------+          +--------+          +--------++--------++
    :| Library          |          | Pavilion          | Eastwing| Ågesen |:
    :|                  =          =        |          |        ==        |:
    :|                  |          |        |          |        ||        |:
    ++                  +          +--------+          +        ++--------++
    ++                  +                    +---||---++         +-------.++
    :|                  |                    | Gallery||               .::|:
    :|                  |                    |        ||                  |:
    :|                  |                    |        ||                  |:
    ++---||---++---||---+                    +        ++--------++---||---++
    ++---||---++---||---++--------++--------++        ++---||---++---||---++
    :| Study  || Corridor| Kitchen          ||        || Banquet hall     |:
    :|        ==        == YOU              |=        |=                  |:
    :|        ||        ||                  ||        ||                  |:
    ++        ++        ++--------+         ++---||---++--------++--------++
    ++        ++        +          +        +                              +
    :|        ||        |          |        |                              :
    :|        ||        |          =        =                              :
    :|        ||        |          |        |                              :
    ++--------++        +          +---||---+                              +
    ++-----.--++        +                    +---||---++--------++--------++
    :| B.  ::.|3        2                    |           Grand ballroom   |:
    :|        =&        &                    |                            =:
    :|        ||        |                    |                            |:
    ++--------++--------+                    +                            ++
    +                    +--------++---||AE.++                            ++
    :                    2 the entrance  .::||                            |:
    :                    &                  ==                            =:
    :                    C                  D|                            |:
    +                    +---||B--++--------++--------++--------++--------++
    ++........++........++........++........++........++........++........++

From here the game is almost done, go to the library and slide it east, in the pavilion you get the key to unlock the chappel and most remaining doors.

From here you can enter the chappel, by essentially backtracing the steps you took to move the entrance back under the balcony. From there you can go up, into the chappel, get the basement key, and go back down and enter the basement. Go down in the basement, here you will find the key to the Baroness' room.

Going back up and entering that room finishes the game.
