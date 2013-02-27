What is this?
-------------

A Maya plug-in that creates a menger sponge.

For more information, see the Wikipedia article:
   http://en.wikipedia.org/wiki/Menger_sponge

What can it do?
===============

Currently create a sponge of any desired iteration, although for a 
fairly powerful 2013-era computer anything over 4 is painful for Maya.

What can it not do?
===================

Undo and redo is not working yet.

How do I build it?
------------------

- On Linux, run the make file.
- On Windows, use the MSVC projects (which might need custom edits.)

Well how do I use it?
---------------------

Load the plug-in through Maya's plug-in manager.

Run the command:

    createMenger -i $n -s $s 
    
...where $n is number of iterations and size is the length of one side of the final cube.
