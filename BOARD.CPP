/*--------------------------------------------------------------------------

  REVERSEM :

  UNIT     : PLAYING BOARD CLASS

  COPYRIGHT (C) 1994 Erich P. Gatejen  ALL RIGHTS RESERVED
  This is Freeware.  You may distribute it as you wish.  However, you may not
  distribute a modified version without my consent.

  Feel free to cut and paste any algorthm.

  NOTE: XTILE is (C) 1992, 1994 by myself.  It is NOT freeware.  You may use
	it for personal projects, but otherwise, it is not to be distributed
	outside this REVERSEM package.  (Contact me if you wish to do
	otherwise)

---------------------------------------------------------------------------*/


// ------------------------------------------------------------------------
// --- INCLUDES
// ------------------------------------------------------------------------
#include<stdlib.h>
#include<stdio.h>
extern "C" {
   #include "xtile21!.h"
};
#include"string.h"
#include"reversem.hpp"
#include"spots.hpp"
#include"board.hpp"
#include"eval.hpp"

// ------------------------------------------------------------------------
// --- PRIVATE MEMBERS
// ------------------------------------------------------------------------

// -- Members : Board -----------------------------------------------------

BOOLEAN  Board::ValidNorth( void ) {
// The valid checks are brute force.
   register signed int  X = TheGo.XIs();
   register signed int  Y = TheGo.YIs();

   // Run north to see if it is a valid move
   Y--;
   // Is is not off the board and is the other player
   if ( Y < TOPY ) return( FALSE );
   if ( !(Spots[X][Y].IsOther(Player)) ) return( FALSE );

   Y--;
   while( Y >= TOPY ) {
      if ( Spots[X][Y].IsSpot(Player) ) return( TRUE  );
      if ( Spots[X][Y].IsSpotBlank()  ) return( FALSE );
      Y--;
   };
   return( FALSE );
};

void     Board::FlipNorth( void ) {
// The valid checks are brute force.
   register signed int  X = TheGo.XIs();
   register signed int  Y = TheGo.YIs();

   // Run north and flip until it is the same player
   Y--;
   while( !Spots[X][Y].IsSpot(Player) ) {
      Spots[X][Y].FlipSpot();
      Y--;
   };
};

BOOLEAN  Board::ValidSouth( void ) {
// The valid checks are brute force.
   register signed int  X = TheGo.XIs();
   register signed int  Y = TheGo.YIs();

   // Run south to see if it is a valid move
   Y++;
   // Is is not off the board and is the other player
   if ( Y > BOTTOMY ) return( FALSE );
   if ( !(Spots[X][Y].IsOther(Player)) ) return( FALSE );

   Y++;
   while( Y <= BOTTOMY ) {
      if ( Spots[X][Y].IsSpot(Player) ) return( TRUE );
      if ( Spots[X][Y].IsSpotBlank()  ) return( FALSE );
      Y++;
   };
   return( FALSE );
};

void     Board::FlipSouth( void ) {
// The valid checks are brute force.
   register signed int  X = TheGo.XIs();
   register signed int  Y = TheGo.YIs();

   // Run south to see if it is a valid move
   Y++;
   while( !Spots[X][Y].IsSpot(Player) ) {
     Spots[X][Y].FlipSpot();
     Y++;
   };
};

BOOLEAN  Board::ValidWest( void ) {
// The valid checks are brute force.
   register signed int  X = TheGo.XIs();
   register signed int  Y = TheGo.YIs();

   // Run north to see if it is a valid move
   X--;
   // Is is not off the board and is the other player
   if ( X < TOPX ) return( FALSE );
   if ( !(Spots[X][Y].IsOther(Player)) ) return( FALSE );

   X--;
   while ( X >= TOPX ) {
      if ( Spots[X][Y].IsSpot(Player) ) return( TRUE  );
      if ( Spots[X][Y].IsSpotBlank()  ) return( FALSE );
      X--;
   };
   return( FALSE );
};

void     Board::FlipWest( void ) {
// The valid checks are brute force.
   register signed int  X = TheGo.XIs();
   register signed int  Y = TheGo.YIs();

   // Run north to see if it is a valid move
   X--;
   while ( !Spots[X][Y].IsSpot(Player) ) {
     Spots[X][Y].FlipSpot();
     X--;
   };
};

BOOLEAN  Board::ValidEast( void ) {
// The valid checks are brute force.
   register signed int  X = TheGo.XIs();
   register signed int  Y = TheGo.YIs();

   // Run south to see if it is a valid move
   X++;
   // Is is not off the board and is the other player
   if ( X > BOTTOMX ) return( FALSE );
   if ( !(Spots[X][Y].IsOther(Player)) ) return( FALSE );

   X++;
   while( X <= BOTTOMX ) {
      if ( Spots[X][Y].IsSpot(Player) ) return( TRUE );
      if ( Spots[X][Y].IsSpotBlank()  ) return( FALSE );
      X++;
   };
   return( FALSE );
};

void     Board::FlipEast( void ) {
// The valid checks are brute force.
   register signed int  X = TheGo.XIs();
   register signed int  Y = TheGo.YIs();

   // Run south to see if it is a valid move
   X++;
   while( !Spots[X][Y].IsSpot(Player) ) {
      Spots[X][Y].FlipSpot();
      X++;
   };
};

BOOLEAN  Board::ValidNorthEast( void ) {
// The valid checks are brute force.
   register signed int  X = TheGo.XIs();
   register signed int  Y = TheGo.YIs();

   // Run north to see if it is a valid move
   Y--;
   X++;
   // Is is not off the board and is the other player
   if ( X > BOTTOMX ) return( FALSE );
   if ( Y < TOPY )    return( FALSE );
   if ( !(Spots[X][Y].IsOther(Player)) ) return( FALSE );

   Y--;
   X++;
   while(( Y >= TOPY )&&( X <= BOTTOMX )) {
      if ( Spots[X][Y].IsSpot(Player) ) return( TRUE  );
      if ( Spots[X][Y].IsSpotBlank()  ) return( FALSE );
      Y--;
      X++;
   };
   return( FALSE );
};

void     Board::FlipNorthEast( void ) {
// The valid checks are brute force.
   register signed int  X = TheGo.XIs();
   register signed int  Y = TheGo.YIs();

   // Run north to see if it is a valid move
   Y--;
   X++;
   while( !Spots[X][Y].IsSpot(Player) ) {
      Spots[X][Y].FlipSpot();
      Y--;
      X++;
   };
};

BOOLEAN  Board::ValidNorthWest( void ) {
// The valid checks are brute force.
   register signed int  X = TheGo.XIs();
   register signed int  Y = TheGo.YIs();

   // Run north to see if it is a valid move
   X--;
   Y--;
   // Is is not off the board and is the other player
   if ( Y < TOPY ) return( FALSE );
   if ( X < TOPX ) return( FALSE );
   if ( !(Spots[X][Y].IsOther(Player)) ) return( FALSE );

   X--;
   Y--;
   while( (Y >= TOPY)&&( X >= TOPX ) ) {
      if ( Spots[X][Y].IsSpot(Player) ) return( TRUE  );
      if ( Spots[X][Y].IsSpotBlank()  ) return( FALSE );
      X--;
      Y--;
   };
   return( FALSE );
};

void     Board::FlipNorthWest( void ) {
// The valid checks are brute force.
   register signed int  X = TheGo.XIs();
   register signed int  Y = TheGo.YIs();

   // Run north to see if it is a valid move
   X--;
   Y--;
   while( !Spots[X][Y].IsSpot(Player) ) {
      Spots[X][Y].FlipSpot();
      X--;
      Y--;
   };
};

BOOLEAN  Board::ValidSouthEast( void ) {
// The valid checks are brute force.
   register signed int  X = TheGo.XIs();
   register signed int  Y = TheGo.YIs();

   // Run south to see if it is a valid move
   Y++;
   X++;
   // Is is not off the board and is the other player
   if ( X > BOTTOMX ) return( FALSE );
   if ( Y > BOTTOMY ) return( FALSE );
   if ( !(Spots[X][Y].IsOther(Player)) ) return( FALSE );

   Y++;
   X++;
   while( (Y <= BOTTOMY)&&( X <= BOTTOMX ) ) {
      if ( Spots[X][Y].IsSpot(Player) ) return( TRUE );
      if ( Spots[X][Y].IsSpotBlank()  ) return( FALSE );
      Y++;
      X++;
   };
   return( FALSE );
};

void     Board::FlipSouthEast( void ) {
// The valid checks are brute force.
   register signed int  X = TheGo.XIs();
   register signed int  Y = TheGo.YIs();

   // Run south to see if it is a valid move
   Y++;
   X++;
   while( !Spots[X][Y].IsSpot(Player) ) {
      Spots[X][Y].FlipSpot();
      Y++;
      X++;
   };
};

BOOLEAN  Board::ValidSouthWest( void ) {
// The valid checks are brute force.
   register signed int  X = TheGo.XIs();
   register signed int  Y = TheGo.YIs();

   // Run south to see if it is a valid move
   Y++;
   X--;
   // Is is not off the board and is the other player
   if ( Y > BOTTOMY ) return( FALSE );
   if ( X < TOPX ) return( FALSE );
   if ( !(Spots[X][Y].IsOther(Player)) ) return( FALSE );

   Y++;
   X--;
   while( (Y <= BOTTOMY)&&(X >= TOPX ) ) {
      if ( Spots[X][Y].IsSpot(Player) ) return( TRUE );
      if ( Spots[X][Y].IsSpotBlank()  ) return( FALSE );
      Y++;
      X--;
   };
   return( FALSE );
};

void    Board::FlipSouthWest( void ) {
// The valid checks are brute force.
   register signed int  X = TheGo.XIs();
   register signed int  Y = TheGo.YIs();

   // Run south to see if it is a valid move
   Y++;
   X--;
   while( !Spots[X][Y].IsSpot(Player) ) {
      Spots[X][Y].FlipSpot();
      Y++;
      X--;
   };
};

// ------------------------------------------------------------------------
// --- PUBLIC MEMBERS
// ------------------------------------------------------------------------

Board::Board( Board *OldBoard ) {

   memcpy( Spots, OldBoard->Spots, sizeof( Spots ) );

};

Board::Board( Spot  TheSpots[BOARDXSIZE][BOARDYSIZE] ) {

   memcpy( Spots, TheSpots, sizeof( Spots ) );

};

void  Board::InitBoard2Start( void ) {

   register int X, Y;

   for ( X = 0; X < BOARDXSIZE; X++ ) {
      for ( Y = 0; Y < BOARDYSIZE; Y++ ) {

	 Spots[X][Y].Is( SPOT_BLANK );
      }
   }

   Spots[3][3].Is( SPOT_WHITE );
   Spots[3][4].Is( SPOT_BLACK );
   Spots[4][3].Is( SPOT_BLACK );
   Spots[4][4].Is( SPOT_WHITE );


};


BOOLEAN  Board::ValidGo( Go  GoTo, SpotStates  ThePlayer ) {

   // If a piece is already there, it is not valid.
   if ( !Spots[GoTo.XIs()][GoTo.YIs()].IsSpotBlank() ) return FALSE;

   // Copy to local
   TheGo  = GoTo;
   Player = ThePlayer;

   // Walk in compass directions; if any are valid, the go is valid
   if (ValidNorth()) return (TRUE);
   if (ValidSouth()) return (TRUE);
   if (ValidEast ()) return (TRUE);
   if (ValidWest ()) return (TRUE);
   if (ValidNorthEast()) return (TRUE);
   if (ValidNorthWest()) return (TRUE);
   if (ValidSouthEast()) return (TRUE);
   if (ValidSouthWest()) return (TRUE);
   return( FALSE );

};


void  Board::DoGo ( Go  GoTo, SpotStates  ThePlayer  ) {

   // Copy the go local
   TheGo  = GoTo;
   Player = ThePlayer;

   // Do the go, flip as appropriate, for each compass direction
   Spots[TheGo.XIs()][TheGo.YIs()].Is( Player );

   if (ValidNorth()) 	FlipNorth();
   if (ValidSouth()) 	FlipSouth();
   if (ValidEast ()) 	FlipEast ();
   if (ValidWest ()) 	FlipWest ();
   if (ValidNorthEast())  FlipNorthEast ();
   if (ValidNorthWest())  FlipNorthWest ();
   if (ValidSouthEast())  FlipSouthEast ();
   if (ValidSouthWest())  FlipSouthWest ();

};


heuristic Board::Evaluate( SpotStates  ThePlayer ) {

   heuristic	Value = 0;
   int		X;
   int          Y;

   // Copy the go local
   Player = ThePlayer;

   // We need to evaluate the heuristic merit of this board for Player
   // This is where playing with this code will give you the biggest
   // pay off

   // OK, do the evals that require a visit to every spot
   for ( X = 0;  X < BOARDXSIZE; X++ ) {

      for ( Y = 0; Y < BOARDYSIZE; Y++ ) {

	 // Use a case to do some of the evals for some of the levels
	 switch( BrainLevel ) {

	    case EXPERIMENTAL:
	       // Through in a random modifier.
	       Value += (random( 5000 ) - 2500);

	    case GENIUS:
	    case SWIFT:
	       // Apply the opponate value
	       if ( Spots[X][Y].IsOther( Player ) )
		  Value += OpponantVal[X][Y];

	    case AVERAGE:
	       // Apply the player value
	       if ( Spots[X][Y].IsSpot( Player ) )
		  Value += PlayerVal[X][Y];

	    case DULLARD:

	       // Count bad guys spots
	       if ( Spots[X][Y].IsOther( Player ) )
		  Value -= SPOT_VALUE;

	    case SIMPLETON:

	       // Count good guys spots
	       if ( Spots[X][Y].IsSpot( Player ) )
		  Value += SPOT_VALUE;
	       break;


	 } // end case

      } // end inner for
   }

   // Do multipliers for brain level.  This will spread out the H
   switch( BrainLevel ) {

      case AVERAGE:  Value = Value * 2;
		     break;

      case DULLARD:  Value = Value * 3;
		     break;

      case SIMPLETON: Value = Value * 4;
		      break;

   }

   return Value;
};

heuristic Board::WinOrLose(  SpotStates  ThePlayer  ) {

   int		X;
   int          Y;
   int 		GoodGuys  = 0;
   int		BadGuys   = 0;

   // Copy the go local
   Player = ThePlayer;

   // Count up the spots.
   for ( X = 0;  X < BOARDXSIZE; X++ ) {
      for ( Y = 0; Y < BOARDYSIZE; Y++ ) {

	 if ( Spots[X][Y].IsSpot( Player ) )
	    GoodGuys++;

	 else  if ( Spots[X][Y].IsOther( Player ) )
	    BadGuys++;

      } // end inner for
   }

   // Is it a tie, win, or loss.
   if ( GoodGuys == BadGuys )
      return( 0 );

   else if ( GoodGuys > BadGuys )
      return( VERY_GOOD_THING );

   else
      return( VERY_BAD_THING  );

};

int  Board::Count( SpotStates  ThePlayer  ) {

   int		X;
   int          Y;
   int 		Count  = 0;

   // Copy the go local
   Player = ThePlayer;

   // Count up the spots.
   for ( X = 0;  X < BOARDXSIZE; X++ ) {
      for ( Y = 0; Y < BOARDYSIZE; Y++ ) {

	 if ( Spots[X][Y].IsSpot( Player ) )
	    Count++;

	 else  if ( Spots[X][Y].IsOther( Player ) )
	    Count--;

      } // end inner for
   }

   return( Count );

};


BOOLEAN   Board::MoveExists( SpotStates  ThePlayer ) {

   register int		X;
   register int         Y;
   Go			AGo;

   // OK, do the evals that require a visit to every spot
   for ( X = 0;  X < BOARDXSIZE; X++ ) {

      for ( Y = 0; Y < BOARDYSIZE; Y++ ) {

	 AGo.XIs(X);
	 AGo.YIs(Y);

	 if ( ValidGo( AGo, ThePlayer ) ) return TRUE;

      } // end inner for
   }

   return FALSE;

};


// OK!!  Board has a couple of static members.  Lets define them.
   SpotStates   Board::Player;
   BRAIN	Board::BrainLevel;
   Go		Board::TheGo;


   // The following are the evaluation boards.  They are col major (sorry)
   int	  Board::PlayerVal[BOARDXSIZE][BOARDYSIZE] = {

      { 900,  -220,  90, 70, 70,  90, -220, 900 },
      { -220, -200, -20, 0,  0,  -20, -200, -220 },
      {  90,  -30,   30, 40, 20,  40, -30,  90  },
      {  78,   0,    40, 0,  0,   20,  0,   78  },
      {  78,   0,    20, 0,  0,   40,  0,   78  },
      {  90,  -30,   40, 20, 40,  30, -30,  90  },
      { -220, -200, -20, 0,  0,  -20, -200, -220 },
      { 900,  -220,  90, 70, 70,  90, -220, 900 }
   };

   int    Board::OpponantVal[BOARDXSIZE][BOARDYSIZE] = {

      { -2000,  90, -20, -15, -15, -20, 90, -2000 },
      {   80, 200,  10,  30,  30,  10, 100,  90  },
      {  -20,  10,  0,   0,   0,   0,  10,  -20  },
      {  -15,  30,  0,   0,   0,   0,  30,  -15  },
      {  -15,  30,  0,   0,   0,   0,  30,  -15  },
      {  -20,  10,  0,   0,   0,   0,  10,  -20  },
      {   80, 200,  10,  30,  30,  10, 100,  90  },
      { -2000,  90, -20, -15, -15, -20, 90, -2000 },

   };
