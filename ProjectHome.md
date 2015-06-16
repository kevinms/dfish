# Moving to its new home http://debrislabs.com #
  * ## engine source will be open ##
  * ## game logic exposed through modding api ##

### Now supports both Linux and Windows (VS 2008 and soon 2010) ###

There is a DEVELOPING file to help developers get started!
  * http://code.google.com/p/dfish/source/browse/branches/engine-merge/DEVELOPING

To view the current code please go to:
  * Game Engine: http://code.google.com/p/dfish/source/browse/#svn/branches/engine-merge/pbwafer
  * Game Logic:  http://code.google.com/p/dfish/source/browse/#svn/branches/engine-merge


---


First you will need to download a few sdl libraries that are used:
```
sudo apt-get install libsdl-dev libsdl-ttf2.0-dev
```

Next checkout the code and compile, for sound support check [SoundSupport](http://code.google.com/p/dfish/wiki/SoundSupport):
```
svn checkout http://dfish.googlecode.com/svn/ dfish
cd dfish/branches/engine-merge/
make clean
make
```

Open a new terminal and run the server (default port 8888):
```
./server
```

Open a new terminal and run the client:
```
./client
```

You will see a red square highlighting what view is currently accepting input.  To change views use the (~) key.  This will let you go to the command line.  When in the main view you can use the menu.

For example, as soon as you start up the game.  Push (v) then (h) to connect to the server you have running on localhost.  Next push (~) to go the command line.  It is similar to what you would find in any quake based game.  Here is a list of commands:
```
say hello world [No (/)]
/info-bcast PORT - Get info from all servers on LAN
/info IP PORT - Request info from a server
/disconnect - Disconnect from a server
/connect IP PORT - Connect to a server
/name string name (no spaces) - Change player name
/quit - Quit the game
/cmdlist - List all the commands
/clear - Clear the console
/help - Tells how to use the console
```

There is much more to come :)


---


```

Active branches:
 - branches/engine-merge
 - trunk/src/

dfish   - Project name (game name will differ)
pbwafer - Engine name

pbwafer - debug macros     [          ]  0/10
pbwafer - window system    [#######   ]  7/10
pbwafer - input            [#######   ]  7/10
pbwafer - cmd              [#######   ]  7/10
pbwafer - menu             [#######   ]  7/10
pbwafer - buffer i/o       [########  ]  8/10
pbwafer - udp net library  [######    ]  6/10
pbwafer - net simulator    [#         ]  1/10
pbwafer - protocol         [#####     ]  5/10
pbwafer - huffman compress [          ]  0/10
pbwafer - blueprints       [##        ]  2/10
pbwafer - blueprint diff   [          ]  0/10
pbwafer - physics          [          ]  0/10
pbwafer - console          [#######   ]  7/10
pbwafer - random numbers   [          ]  0/10
pbwafer - path finding     [          ]  0/10

dfish - main view           [#         ]  1/10
dfish - fabricator view     [##        ]  2/10
dfish - procedural textures [          ]  0/10
dfish - procedural planets  [          ]  0/10
dfish - procedural stars    [          ]  0/10
dfish - procedural maps     [          ]  0/10
dfish - background gen.     [          ]  0/10
dfish - basic entities      [          ]  0/10
dfish - primitive BPs       [          ]  0/10
dfish - weapon BPs          [          ]  0/10

```