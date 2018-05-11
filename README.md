REVERSEM is a graphical Reversi game written in C++ using my XTile library.

I did it for an AI class. The project was to implement a minimax module in Pascal that the instructor would run in his harness. I asked if I could write it in C++ instead if I did the whole thing. He said OK. I got my assignment back (the printed source code) with "this is way too much work" written across the top, as well as an A+.

My minimax algorithm is sound, though my other methods weren't entirely scientific. I created my heuristics by running games on the Windows version of reversi. After I got Reversem working, I ran them side by side, so I could tweak things. I noticed that random perturbations to the heuristics did better against me than the plain version of Reversem or the Microsoft version. I added this as the "experimental" level.

After I released it, I got into conversations with a few experts,both in AI and the game. They noted that my biggest omission was book moves for the opening. I had moved on to other things, so I didn't make any changes.

Many years later, I found web pages that hosted Reversem. A couple noted I was from Finland. I'm not. When I wrote this I was living in Colorado. However, since then I have spent a lot of time in Finland and I find it a wonderful country.

---
Original Readme

REVERSEM  1.0

REVERSEM  Copyright (C) 1994  Erich P Gatejen
XTILE     Copyright (C) 1992, 1994  Erich P Gatejen  ALL RIGHTS RESERVED
All portions of reversem are freeware, except XTile.  Xtile may only be 
distributed with this package.  Feel free to play with reversem all you 
want.  However, do not distribute a modified version without my consent.
Cut and past the algorithms at your whim.

A few notes:  Borland's heap was constantly crashing the program.  So, I did
my own memory management.  My picture is there primarily to unnerve my
instructor.  It seemed to work;  he gave me an A.  Modifying the evaluation
functions will give a big payoff if you want to improve the brains.  The
values it has now are SWAGs.  

I can be reached at erichgatejen@yahoo.com

Later,
Erich


