Strip packing using Scatter search
==================================


The 2D strip packing problem consist in find
the optimal place for `n` rectangles in an infinite strip
with a fixed width.

There are several heuristics to place the rectangles,
dropping the rectangles freely,
guillotining the strip and place a certain number of rectangles,
placing in a certain order, for example, left to right, etc.

In this case, the used heuristic is called Bottom-Left Fill,
which consist in place the rectangles from the bottom of the strip
from left to right, positioning the rectangles in the first spot
available which doesn't broke any constraints.

In the other hand,
the Scatter Search algorithm is an evolutionary approach,
which is based on the idea to merge solutions due
a certain number of constraints.


Compiling and using it
======================

To compile you only need to do a::

   $ make

And to use it, you need to give as a parameter the input data,
for example::

   $ ./main -i inputs/HT01.csv
