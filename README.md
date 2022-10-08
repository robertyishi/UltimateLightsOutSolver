# The Ultimate Lights Out Solver
Extremely efficient solver for the Lights Out puzzle.

Define **remoteness of a position** as the minimum number of moves that need to by made to reach the primitive position.

The solver takes advantage of two special properties of the Lights Out puzzle:

1. There is only one primitive position, namely the position where all lights are turned off.
2. The optimal solution for any starting position never involves toggling the same light twice as those two moves will cancel each other out.

Therefore, the remoteness of any position is the minimum number of moves that needs to be made from the primitive position to reach that position.
The primitive position has a remoteness of 0. Then, all positions reacheable by making exactly one move from the initial position have a remoteness of 1.
Next, all unvisited positions reacheable by making exactly two moves from the initial position have a remoteness of 2. So on and so forth until we fail
to assign a new remoteness for any unvisited position at some *k* value and we stop.

There exists a closed form solution for the puzzle at any starting position if the matrix corresponding to the system is invertible:
https://mathworld.wolfram.com/LightsOutPuzzle.html. Use matrix.py to check if the dimensions specified for the game result in an invertible
system matrix.
