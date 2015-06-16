### Table of Contents ###



# Scope #
Just think: Dwarf Fortress + Space + Real-time + Online + Legos + Combat + Destructability!

Imagine being plopped into a beautiful galaxy next to a remote planet. Having only a small ship with a few crew members for company, the struggle for existence begins here. Your ship is set into orbit around the planet you soon call home. With harvested materials from deep within the core, modifications are made to your ship. More rooms. Airlock doors. Missiles. Lasers. Mines. The population growth pushes for a full space station to begin construction. Suddenly, light is warped into blackness. Out of the abyss a mammoth ship rolls out. All eyes are on this new object. What seems like waves of colorful light shine from the ship. The light literally rips apart your station piece by piece. Defense systems are assimilated before they even active. Your ships are being eaten away. The crew is sucked into the vacuum of space. Chaos threatens your existence. The only hope of survival calls for drastic measures. As the orders are made you shoot off into space on the same small ship you arrived in. You watch as your planet, your home, implodes taking your unknown enemy with it.


---

# Map Generation #
Maps will be created using procedural generation. Using a set of random seeds + a PRNG + a procedural function it should be possible to generate an entire map. An overly simple example would be:
  1. Server receives requested map size from clients
  1. Server generates a set of seed numbers
  1. Server generates N random planetoid locations
  1. Server creates a blueprint for each planetoid
  1. Server applies procedural function to generates properties and textures for each planetoid
  1. Server generates random space debri blueprints around planetoids
  1. Server populates its spacial indexing data structure with blueprints
  1. Server sends all blueprints and map data to clients
  1. Clients populate their spatial indexing data structure
Planetoids will be represented as blueprints and will be given a location somewhere in space. Using some noise generating functions like perlin noise it should be possible to create interesting textures for the planetoids.

A few things need to be discussed:
  * What the map struct will look like
  * The protocol used for transferring the map data
  * Will planets be mobile?

I also think blueprints will need to have a class identifier to tell whether it is a planet/asteroid/nebula/star/space-debris/black hole/worm hole/etc.

References:
  * [Random Number Generators](http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_rand.aspx)
  * [Procedural Generation](http://notch.tumblr.com/post/3746989361/terrain-generation-part-1)
  * [Perlin Noise](http://www.cs.clemson.edu/~kevin4/perlin.html)

---

# Blueprints #
All information in the game is stored in blueprints. This includes entire ships, guns, floor tiles, wires, spacemen, etc. I will go into a bit more depth on how I think BPs (blueprints) should be stored and accessed a bit later. Whether or not maps will be a blueprint is undecided. Blueprints make up an entire hierarchy of objects. At the most basic level you have primitives.
```
typedef struct prim_s {
	int ch;
	char r,g,b;
	char fr,fg,fb;
} prim_t;
```
Primitives are a single character which can represent one type of tile. There could be hundreds/thousands of primitives. One might be a floor tile; another a wall tile; another a piece of glass; another a gold wire. Next up in the hierarchy is an entity.
```
typedef struct entity_s
{
	unsigned short id;
	unsigned int mass;
	unsigned int health;
	prim_t *p;

	// 2D Doubly-Linked List
	struct entity_s *n;
	struct entity_s *s;
	struct entity_s *e;
	struct entity_s *w;

	// Hashmap pieces
	int offset_x; // These could hashed and stored in a table
	int offset_y; // It would enable a much faster search

	// Linked List, will speed up looping through
	struct entity_s *next;
} entity_t;
```

Each entity is just an instance of a primitive. There could be a hundred instances of the wall primitive so each entity would just have a reference to the same wall tile. Now there are some variables that each instance needs to keep track of separately like id, mass, health, etc. which are also contained within the entity. Finally, each entity is a piece of a larger data structure. Actually, multiple data structures. Each one has its own benefits.
```
2D Doubly-Linked List:
 faster collision detection
 faster path finding

Hashmap:
 faster lookup

Linked List:
 faster iterating
```
Whether all these data structures will actually be used in the end is yet to be determined. Some data structures may use the principle of shifting computation in time to improve performance.

Entity positions will be stored relative to a base piece, e.g. (+20x -20y). The base piece can be either the visual center or center of mass which can be calculated.

An unsigned short or two bytes for entity IDs gives us 2<sup>16 or 65536 ids. That means players can build blueprints with 2</sup>16 tiles in them!

The final level is the blueprint itself:
```
typedef struct bp_s {
	entity_t *head;

	entity_t *center[3];

	entity_t *hull; // Just a pointer to a hull piece, this will hopefully speed
	                // up collision detection

	unsigned int mass;

	int x;
	int y;
} bp_t;
```

Information about an object as a whole is stored here. Such as mass, speed, position, etc. The entity pointers are again for quicker access and computation. The head pointer is for the first item in the linked list. The center entity will point to the 'center' of the blueprint object (which will be calculated on the fly). The hull entity will point to any hull tile (outside edge piece). This should quicken collision detection.

The next step is remembering dfish is an online multiplayer game. Each player will need to get some info on each ship out in space as other players build them. Also, since ships are completely  destructible they will need some way to tell which parts have been damaged/destroyed. It will be possible for blueprints to be split as well.

First, a limited blueprint will be sent to each player when it is built in the fabricator. This blueprint will not contain ALL the information about the blueprint only what is needed by the other clients.

Second, deltas will be sent over the network to each player to update each other of events that are happening to their blueprints. The name implies correctly that only the changes in the blueprint are sent. Much brain-thunderstorming needs to be done in this area before anything becomes close final in the code.
```
typedef struct delta_s
{
	//hmmm
} delta_t;
```

Nested Blueprints?

Should maps be BPs?

More Info:

  * Blueprints that are intended to move, such as ships, must have all pieces connecting. The fabricator can give errors telling the requirements.
  * We can calculate the final velocity of the object and have it display in a stats window

  * Blueprints will list entities pieces along with id (e.g. door = 42, id = 1), ID will help link entities
OR
  * Blueprints will list entities pieces (e.g. door = 42), entity position will help link entities
  * If an entity is moved or destroyed you search to destroy the link

```
// Archie says :)
We might need to check if a wall has been damanged enough for space to come in 
```


---

# Hash Table for Blueprints #
```

Check eternally confuzzled!

Our hash table will use separate chaining for handling collisions. The
major reasons are:
  + it can handle deletion of keys easily - destroying entities will be
    a common operation this is needed
  + it can handle duplicate keys easily - the x,y offsets will be used
    as the key in the hash table, entities may have the same offsets
    since there are multiple layers of entities

When the game is in a playable state comparison of hash functions will
have to be done on the blueprint data sets to minimize collisions.

Again, eternally confuzzled is a good resource to find many common
hashing algorithms that are very quick and can achieve avalanche!
```

---

# Average Packet Size/Rate #

```
Estimated average packet statistics:
30 packets/sec at ~512 B/packet = ~15 kB/s

Client upload speeds are the limiting factor ranging from 10 kB/s
to 50 kB/s. We should not flood the clients with data from the server
even though download speeds are much higher. The ack bitfield can only
support a certain number of acks per second.

It is suggested by Glenn Feidler to have several states you can drop
into depending on your current network conditions. We might use 3
states being 10, 30, 60 packets/sec. On LANs this rate can be increased
even more.

Resources:
http://gafferongames.com/networking-for-game-programmers/reliability-and-flow-control/
Google search, wireshark capturing, etc.
```

---


# Layers of Ship Design #

There are multiple layers that can be viewed for a ship:
  * Exo-Layer
  * Endo-Layer
    * Floor Plan
    * Wiring

The exo-layer will be seen by your enemies. Enemies should not be able to see any of the endo-layer unless they steal a ship or board the ship. The hull of the ship will define how the exo-layer appears.

The endo-layer will be seen by the owner of the ship. The floor plan will be the default layer but you can also switch to the wiring layer to connect pieces of machinery. Route power to different destinations. And more.

Blueprints are built in what is called the fabricator. If you want the object to be movable, e.g. ships, all tiles must be connected. There can not be any loose parts floating in space. This helps define a definite hull for ships. There will probably be some sort of ship core that will be given upon starting the game. Objects can only move and be considered ships if they have this ship core, intact.

dfish should push for good ship design to be an integral part of game play. A few key factors could be:
  * where the ship core is placed
  * where the wiring is run throughout the ship
  * how rooms are organized
  * how airlock doors are utilized

Some elements of circuit design could be interesting to incorporate as well.


---


# Interface #


---


# Management of Units #
<a href='Hidden comment:  Potentially inapplicable '></a>

---


# Cosmetic Effects #
  * When a ship is destroyed, little space men should be visible floating out into space.

---


# Scaling over the course of the game #


---


# Entities #
## Ship Parts ##
## Raw Materials ##
## Larger-Scale Tiles ##