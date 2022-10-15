"# connect-4" 

## Team Members
Nathan Bunge: Lab 3
Tom Arad: Lab 7
Jared Hardwig: Lab 3
Jake Bortles: Lab 3

## Setting up the game:
To get 2 games running on one computer, you must find the connect4.exe file in the debug folder and open this twice. 
You can also run one window with the visual studio debugger and ther other by opening the conenct4.exe
To run the exe, you may have to create a shortcut. Set the path to where connect4.exe is stored, and then (IMPORTANT) set the "look first" box to where all the .dll files for SFML are stored.
If you do not set the "look first" box, you may get errors saying that the SFML libarary does not work.
Make sure you set the path to the connect4.exe that is in the "outermost" debug folder (the debug folder that is next to the .sln file).

Once you have two windows open, you will need to choose on of them to be the server. Take the address that the server gives and put into the other window.
This process is the same if you are running on one or two computers becuase when you are running on one computer you are simulating what it is like to run on two.
IMORTANT: Two computers can only connect if they are using the same wifi (or the same same router network).
IMORTANT: This code will NOT run on linux (sorry)
