# 2D Dungeon Generator
This repo contains a solution to the following problem:
```
Create a procedurally generated dungeon, as described here: 

https://en.wikipedia.org/wiki/Random_dungeon#:~:text=A%20random%20dungeon%20is%20a,there%20are%20innumerable%20possibilities%20for

The dungeon can be 2D or 3D. At the very least, it should have rooms and hallways.
```
The solution is for a 2D dungeon generated in a square grid.

## How it works
On a square grid, a start tile `S` is placed at random. From there, any adjacent tile can become a hallway `H` or a room `R`. When a hallway is placed, any adjacent tile that is empty `*` can again become a hallway or a room. When a room is placed, it will lead nowhere, so its adjacent spawning tile becomes the spawning tile again. The ending tile `E` becomes a potential tile to be placed once an amount of hallways equaling half the length of the grid have been placed. Once the ending tile has been placed generation is complete.

Before the minimum amount of hallways has been reached the chance for a room to spawn is %30 and the chance for a hall to spawn is %70. After this, the chance for the end to spawn is %10 with %30 for room and %60 for hall.

Example output with grid size 10:
```
* * * * * * * * * *
* * * * * * * * R *
* * * * * * * H H *
* * * * * * * H H H
* * * * * * * H R H
* * * * * R S H R H
* * * * * * * * * H
* * * * * * * * E H
* * * * * * * * * *
* * * * * * * * * *
```

## Things to improve
I'm pleased with my solution, but it did not turn out exactly as I had hoped. For instance, a glaring issue is that some dungeons get the ending tile trapped between rooms and/or against the border of the grid, which ends up being why the generator can't reliably generate large dungeons.

Example output with dungeon ending prematurely:
```
* * * R E R * * * *
* * * * S * * * * *
* * * * R * * * * *
* * * * * * * * * *
* * * * * * * * * *
* * * * * * * * * *
* * * * * * * * * *
* * * * * * * * * *
* * * * * * * * * *
* * * * * * * * * *
```
Here the start tile generates a room to its south, after which it generates a hallway to the north. After trapping itself with rooms to the east and west and the border to the north, there is no other option but to replace the hall with the end.

To rectify this I would need to refactor the behavior for checking for valid tile placement, as it is not as forward-thinking as it could be. I could keep track of how many rooms are being placed instead of blindly placing them around the current tile. The goal would be to always have at least ```dungeonSize / 2``` hallways.
