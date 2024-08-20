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
You have to run this game in your terminal (Windows, Mac and Linux instructions for how to turn on the game below).

The Code does support unicode, and the default version of the game (`magicmapmansion_WINDOWS.exe`) do use non-ascii characters: æ, ø, å, ß, ¤.

When you run the game, you can see if it works. The north-east room in the mansion belongs to mr Ågesen, If his name does not start with an A with an o above it, your terminal or font doesn't support these characters. In that case you should use `magicmapmansion_WINDOWS_english.exe` instead, this is identical, except it does not use non-ascii characters.

Almost all terminals (on Linux and Mac) support these characters, while most terminals on Windows don't. (If you want to install a third party terminal, git-bash does support it on windows)

You also need to make sure your terminal is using a MONOSPACED font: i.e. a font where all characters have the same width and height. Otherwise the display will be all messed up. But most terminals (even on Windows) do that by default.


Starting the game on Windows 10
--------
I have not tested windows 11.

The Windows version is inferior to the Linux version (Because Linux is better at terminal stuff), but it does still work.

First step, after downloading the folder with `magicmapmansion_WINDOWS.exe` is to open a terminal in that folder. Here is how to do that using the terminal PowerShell on Windows.

Method 1: Open the folder in a window in file-explore (or whatever it is called, I am not a windows person), press ctrl+l to highlight the path. Type `powershell` in the textfield (this deletes the path from the field, that is fine). Powershell should open automaticall in the folder. You can tell that if the text before the flashing cursor says: `PS C:\users\yourname\DownloadsOrWherever\magicmapmansion>`

Method 2: Open powershell, by default it opens in your root folder: The text before the flashing cursor should say:

`PS C:\users\yourname>`

now you can enter other folders by writing `cd .\foldername\`. If you have the folder in your downloadfolder, type `cd .\Downloads\` and then `cd .\magicmapmansion\` (Pressing TAB may autocomplete).

 By the way, if you have git-bash installed I do recommend that over PowerShell, but PowerShell should be installed on all Windows 10 computers.

Now you can start the game, the windows version does not support automatic size-detection of the terminal, so you will have to manually adjust it.

In POWERSHELL You can do this by right-clicking the top bar of the window and clicking properties, then click the tab layout. Look at the box labeled "window size", you can read the width and height here. For the best experience, I recommend setting it to width 108, and height 35.  FULLSCREEN DOES NOT WORK WELL

Now close the properties windows and type:`.\magicmapmansion_WINDOWS.exe`  or (`.\magicmapmansion_WINDOWS.exe`) This starts tge gane

you COULD also start the game you can doubleclick `magicmapmansion_WINDOWS.exe`, but I don't know if that always work

On Linux, the size would have been spotted automatically




You can also recompile the C++ code. But I don't know if that is possible on Windows. I have only compiled this on Linux (using mingw to compile a Windows executable on Linux).

If you know how CMake works, you could try to compile it.

Starting the game on Linux (or Mac)
--------
So you have realized that Windows is bad! Great! To show your superiority over Windows users, you will have to compile the C++ code, I use standard libraries (plus unix libraries to get Terminal size) so you only need a compiler, I used g++. You should also have CMake installed, to build the project.

The game is made and tested on Arch Linux, I do not own a Mac, but Mac and Linux are very similar under the hood so the Linux code should compile and run on Mac.

g++ and CMake are available on Mac, I recommend using the Homebrew package manager (If you don't feel comfortable installing these things on Mac, you may want to skip this game, or use the windows version)

CMake makes compiling easy, just open your terminal in the projects folder, and run `cmake -Bbuild`, followed by `cmake --build build` this builds the executable in the file `build/bin/maptrial`

You will need to execute this file, while your terminal is still in the projects root folder, write: `build/bin/maptrial` in your terminal, the game should start.

How to play (Once it is started)
---------------
The game window could look something like this:

    You are standing in the entrance  ++........++........++........++........++........++........++........++
    hall in a late baroque mansion,   ++--------++--------+          +--------+          +--------++--------++
    with dusty white-painted walls.   :| Library          |          | Pavilion          | Eastwing| Ågesen |:
                                      :|                  =          =        |          |        ==        |:
    Items and doors: there is a       :|                  |          |        |          |        ||        |:
    staircase ("exit E") and 4 doors  ++                  +          +--------+          +        ++--------++
    ("exit A","exit B","exit C","exit ++                  +                              +         +-------.++
    D") a "portrait" of the late      :|                  |                              |               .::|:
    baroness Josefine and a painting  :|                  |                              |                  |:
    with a "developer message", there :|                  |                              |                  |:
    is also a red "bloodstain" on the ++---||---++---||---+                              +--------++---||---++
    floor. There is a paper "note" on ++---||---++---||---++--------++---||AE.++--------++--------++---||---++
    the west door "exit C". HINT: try :| Study  || Corridor2 the entrance  .::|| Kitchen          || Gallery|:
    typing "examine developer         :|        ==        =& YOU              ==                  ||        |:
    message".                         :|        ||        |C                  D|                  ||        |:
                                      ++        ++        ++---||B--++--------++--------+         ++        ++
                                      ++        ++        ++---||---++--------++--------++        ++        ++
                                      :|        ||        ||           Grand ballroom   ||        1|        |:
                                      :|        ||        ||                            ==        &=        |:
                                      :|        ||        ||                            ||        ||        |:
                                      ++--------++        ++                            ++---||---++---||---++
                                      ++-----.--++        ++                            ++---||---++---1&---++
                                      :| B.  ::.|3        2|                            || Banquet hall     |:
    ##################################:|        =&        &=                            ==                  |:
     Legend                           :|        ||        ||                            ||                  |:
    =A ||A:door with id A             ++--------++--------++--------++--------++--------++--------++--------++
    1&    :locked door,id 1           +                                                                      +
       .  :                           :                                                                      :
     .::  :exit up/down               :                                                                      :
     -- | :1m wall                    :                                                                      :
    Name  :Room name                  +                                                                      +
    YOU   :You                        ++........++........++........++........++........++........++........++
    Commands: change map "floor up/down"| end game "quit"| use door "enter [exit letter]"| get help "hint"
    Move room "slide north/south/east/west" | "undo" | Explore with "examine room"/"examine [item]"
    :

There are 2 main windows in the game: A map to the right, a textlog to the left. The textlog prints the result of your commands, and the map shows you where you are.

Below this you see the valid commands, and below that you have a :, where you can start typing commands. (Finish with enter). Arrowkeys or backspace MAY OR MAY NOT be supported, it all depends on your terminal. Autocomplete is not.

One of the most important commands is "enter" which allows you to move between rooms:

To the left, you see the map legend, as you see | and -- are walls, and = and || are doors, the letter next to the door is the ID of that door. If the door has a & symbol, it means it is locked, the number next to it tells you what key number it is locked by.

You can move between rooms if they have doors facing each other, looking like == or :

   ||
   ||

If either one of the doors are locked, you can not pass through.


"examine" which allows you to get the item description for the room (write "examine room") or an item (write "examine itemname", if you write "examine room" or when you enter a room, you get the list of all Items and Doors, with all item names.

This is ESSENTIAL to find clues to where the KEYS are hidden.

To bypass locks, you can slide the room you are in using `slide north`, `slide south`, `slide west` and one more which shall not be named.

Rooms can not pass through each other, rooms can only slide if they are touch another room (must be touching both at the start and end of the move) and only if you physically are in them.

Some of the puzzles have multiple solutions ... and some of them are pretty tough.



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
