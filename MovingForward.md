Here you will find a listing of items that need to be completed soon. We will try to include as much information as possible about each task.

### Tasks ###

Blueprints:
  * Finalize members for each struct type
  * Which members will be sent to the server/clients?
  * Write function for packing all blueprint data
Blueprint Deltas:
  * Write function for packing blueprint delta information
  * Write function for unpacking/applying deltas
Networking:
  * Add a lobby screen while waiting for a game to begin
  * Implement a flow control algorithm
  * Packet sending needs to be buffered instead of many tiny packets
  * Network simulator needs to be finished
  * Handle partial receives
  * Test the networking protocol with the simulator to iron out bugs
Interface:
  * Handle mouse input
  * Reorganize views in the window
Fabricator:
  * Add lots of primitives
  * Improve interface for fabricator
  * Pack blueprint data as building
  * Switch between ship layers
  * Circuit building
Procedural Generation:
  * Implement perlin noise
  * Create RNG functions that will be needed
  * Try to create stunning textures for planets!
  * Deterministic generation of background stars [perlin?]
Blueprint File Format:
  * Wait till blueprints finalized a bit more
  * Needs to save a version number with it
  * Look at the blender file format for ideas

### TODO ###
Results of a large brainstorming session:

```
------------------------------------------------------------------------
full blueprint packing
------------------------------------------------------------------------

For each entity pack: id, primitive type, offset_x, offset_y, health, layer

All the blueprint data structures (linkedlist hasmap 2d-linkedlist) can
be computed easily on each host:

So lets say the incoming packets has a series of the above variables.
As it unpacks each one it creates a new entity and adds it to a linked
list. It then loops through the completed linked list and hashes all the
entities using the offset_x and offset_y. Then it loops through the list
again but this time running hashmap lookups to determine if pieces are
next to it. If there is a piece next to it then set the pointer. If not 
set null. That should completely rebuild the blueprint

------------------------------------------------------------------------
blueprint delta packing
------------------------------------------------------------------------

Three delta types:
  delete
  add
  update (many types)

Data to pack:

Delete:
Only the id is needed. Each client can do a lookup to find the correct
entitity to delete.

Add:
It would pack: [id, primitive_type, offset_x, offset_y, health, layer]
for each entity that was added. Hash lookups can be used to set all the
pointers for the entity.


Update:
It would pack: [id, update_type, delta1, delta2, ...] The update_type
will be a bitfield where bits are set to show which updates have
occured. You would check the bitfield to see how many updates there are
for that entitity and know how many to unpack. The series of delta*
would just reflect the change in a variable like -20 for health.

------------------------------------------------------------------------
buffering blueprint packet data in the fabricator
------------------------------------------------------------------------

Lets assume a new bluprint is 100*100 tiles so:
10,000 entities * 14 bytes/entity = 140,000 bytes = 140 KB

It would take about this long to send the blueprint all at once:
150,000 bytes / (600 bytes/packet * 30 packets/sec) = 8.3 seconds

We dont want to send 140 KB in one go unless we have to. Instead we
spread out the data transfer over time by constantly sending bp updates
while they are creating it in the fabricator. Each time they add/del
entities we begin to fill a fixedbuf with the delta packet data. Once
the buffer gets to a certain size we send it off to the networking
layer to be transmitted to the server. OR If they finalize the bp
before the buffer if full go ahead and send the data.

------------------------------------------------------------------------
buffering packets in the networking layer
------------------------------------------------------------------------
Need to have a fixedbuf_t that stores up to X bytes of data before
sending. It will be filled with packets fromt the packet_queue.

```