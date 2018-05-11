/*--------------------------------------------------------------------------

  REVERSEM :

  UNIT     : Main unit

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
#include<conio.h>
#include<dos.h>
#include<alloc.h>
#include"heap.hpp"
#include"reversem.hpp"
#include"eval.hpp"
#include"spots.hpp"
#include"board.hpp"
#include"graphics.hpp"

// ------------------------------------------------------------------------
// --- DEFINITIONS
// ------------------------------------------------------------------------

// --- In game status
enum  STATUS { STATUS_CONT = 0, STATUS_END, STATUS_DONE, STATUS_NEW };

// --- Timing definitions  (all in miliseconds)
#define  BUTTON_TIME	500


// ------------------------------------------------------------------------
// --- CLASSES
// ------------------------------------------------------------------------

// --- This prototype is need here!
void *getnew( void );

// --- MMNode.  A minimax node --------------------------------------------
class  MMNode: public Board {

   MMNode	*Children[MAX_CHILDREN];

   int		NumberChildren;

   SpotStates   Owner;

   static       unsigned int	CurrentPly;
   static	Go		AGoTo;

 public:

   // Constructors
   MMNode( Board  *OldBoard, SpotStates  Who   	     ) : Board( OldBoard ) {
      Owner = Who;
   };

   MMNode( Spot   TheSpots[BOARDXSIZE][BOARDYSIZE],
	   SpotStates Who		             ) : Board( TheSpots ) {
      Owner = Who;
   };

   MMNode( Spot   TheSpots[BOARDXSIZE][BOARDYSIZE],
	   Go     GoTo,     SpotStates Who	     ) : Board( TheSpots ) {
      // This will make the move that creates this board
      Owner = Who;
      DoGo( GoTo, Owner );
   };
   ~MMNode( void ) {};

   void  SetPly( unsigned int Ply ) { CurrentPly = Ply; };

   // Enter the tree,  call for root only
   heuristic  TREE( SpotStates    ThePlayer,
		    Go	         &TheMove    );

   // Enter the tree,  call all non-root nodes
   heuristic  TREE( heuristic     UseThresh,  // For A/B pruning
		    heuristic     PassThresh,
		    BOOLEAN	  Pass  = FALSE  // So we can tell an end move
		  );

   // DAMN THING IS DRIVING ME NUTS!!!  Overload new.
   void * operator new( size_t size ) { return( getnew() ); };
   void operator delete( void *p  ) {};  // Don't delete anything.
					 // The heap will handle it
   // OK, lesson learned.  DO NOT recursively delete!!!
   // and don't use Borlands heap.  It blows up EVERY time!!!
};



// ------------------------------------------------------------------------
// --- DATA DECLARATIONS
// ------------------------------------------------------------------------

// --- Static members of MMNode
   unsigned int	MMNode::CurrentPly;
   Go		MMNode::AGoTo;

// --- Global variables

   // Plys for each Brain levels
   unsigned int  MaxPlys4Level[EXPERIMENTAL+1] = {

      2, // Simpleton
      3, // Dullard
      3, // Average
      4, // Swift
      5, // Genius           // <- 5 may be too high, I'm not sure.
      6, // Experimental

      // NOTE: With the heap limit set the way it is, any plys over
      // 5 may cause it to actually get stupider <sic>.

   };

   // Boards
   Board	MasterBoard;

   // Default the brain setting at the lowest level.
   BRAIN	BrainSetting = SIMPLETON;

// --- Define the graphics control item
   GraphicsControl		GCI;

// --- Give use a bigger stack!!!
   extern unsigned _stklen = ( 16 * 1024 );

// --- Define the heap for the minimax tree.
   Heap   MMHeap( sizeof( class MMNode ) );

// --- Running score
   int	  RunningScore = 0;

// --- Flag show heuristic
   BOOLEAN  ShowH = FALSE;


// ------------------------------------------------------------------------
// --- FUNCTIONS PROTOTYPES
// ------------------------------------------------------------------------
void  ComputerMoves( void );
void  HelpHuman( void );
heuristic  MINiMAX( SpotStates    ThePlayer,
		    Go	         &TheMove,
		    Board	  TheBoard     );


// -----------------------------------------------------------------------
// --- FUNCTIONS
// -----------------------------------------------------------------------

// -- Heap allocate fof the overloaded new -------------------------------
void *getnew( void ) {
      return ( MMHeap.Allocate() );
};

// -- Computer moves -----------------------------------------------------
void  ComputerMoves( void ) {

   Go		AGo;

   heuristic	H;

   char Buffer[40];

   // Say I'm thinking
   GCI.Me.ShowPicture( PIC_THINKING );
   GCI.Me.SayText( "OK.  GIVE ME A 'SEC.", NULL, NULL );
   delay( 200 );

   // Amove exists.  Generate the move we want
   MasterBoard.BrainIs( BrainSetting );
   H = MINiMAX(   SPOT_BLACK,  AGo, MasterBoard    );

   // OK.  Show where I wanna go
   GCI.Me.ShowPicture( PIC_LOOKRIGHT );
   GCI.Me.SayText( "I WILL GO HERE", NULL, NULL );
   GCI.Here( AGo );
   delay( 1000 );

   // We have a move.  Apply it and show it.
   MasterBoard.DoGo( AGo, SPOT_BLACK );
   GCI.ShowBoard( MasterBoard );


   // ---  OK.  Respond to the move.

   // If on, show H
   if ( ShowH ) {
      itoa( H, Buffer, 10);
      XSet_Box( 112, 235, 80, 5, 0 );
      XString4_C( 116, 235, 15, Buffer );
   };

   // Show my reaction
   if ( H > A_EXCELLENT ) {
      GCI.Me.ShowPicture( PIC_HALFSMILE );
      GCI.Me.SayText( "HE HE HE HE", "NOT LOOKING TO BAD", "FOR ME HERE" );
   } else
   if ( H > A_GOOD ) {
      GCI.Me.ShowPicture( PIC_HALFSMILE );
      GCI.Me.SayText( "WELL, NOT BAD.", NULL, NULL );
   } else
   if ( H > A_OK ) {
      GCI.Me.ShowPicture( PIC_LOOKRIGHT );
      GCI.Me.SayText( "HMMMMM...", "I'M NOT SURE WHAT TO", "THINK HERE." );
   } else
   if ( H > A_UNKNOWN ) {
      GCI.Me.ShowPicture( PIC_CALM );
      GCI.Me.SayText( "WELL.  THIS COULD GO", "EITHER WAY.", NULL );
   } else
   if ( H > A_UNGOOD ) {
      GCI.Me.ShowPicture( PIC_CURIOUSFROWN );
      GCI.Me.SayText( "UGG.", "THIS DOESN'T LOOK", "PROMISING" );
   } else
   if ( H > A_BAD ) {
      GCI.Me.ShowPicture( PIC_LOOKRIGHTC );
      GCI.Me.SayText( "OH.", "THIS IS NOT GOOD!", NULL );
   } else {
      GCI.Me.ShowPicture( PIC_FROWN );
      GCI.Me.SayText( "I'M DEAD.", NULL, NULL );
   };

};

// -- Help human with a move -----------------------------------------------
void  HelpHuman( void ) {

   Go		AGo;

   char Buffer[40];

   // Say I'm thinking
   GCI.Me.ShowPicture( PIC_THINKING );
   GCI.Me.SayText( "OK.  GIVE ME A 'SEC.", NULL, NULL );
   delay( 200 );

   // Amove exists.  Generate the move we want
   MasterBoard.BrainIs( BrainSetting );
   MINiMAX(   SPOT_WHITE,  AGo, MasterBoard    );

   // OK.  Show where I wanna go
   GCI.Me.ShowPicture( PIC_IDEA );
   GCI.Me.SayText( "I HAVE AN IDEA", NULL, NULL );
   delay( 1000 );

   // OK.  Show where I wanna go
   GCI.Me.ShowPicture( PIC_LOOKRIGHT );
   GCI.Me.SayText( "I WOULD GO HERE", NULL, NULL );
   GCI.Here( AGo );
   delay( 1000 );



};


// -- Handle the end of a game ---------------------------------------------
void  EndGame( void ) {

   int 	PlayerCount;

   // Count the players points and determine a win or loss
   PlayerCount = MasterBoard.Count( SPOT_WHITE );

   if ( PlayerCount == 0 ) {

      // A draw
      GCI.Me.ShowPicture( PIC_CALM );
      GCI.Me.SayText( "HEY, HOW DID THAT", "HAPPENED.", "WE TIED!" );

   } else if ( PlayerCount < 0 ) {

      // A loss
      GCI.Me.ShowPicture( PIC_VICTORY );
      GCI.Me.SayText( "HA HA HA HA HA", "I WIN!", NULL );


   } else {

      // A win
      GCI.Me.ShowPicture( PIC_WHATUPSET );
      GCI.Me.SayText( "HEY!!!", "HOW DID YOU DO THAT.", "YOU WIN" );

   }

   RunningScore += PlayerCount;
   GCI.Score( RunningScore );

};

// -- Minimax function ------------------------------------------------------
heuristic  MINiMAX( SpotStates    ThePlayer,
		    Go	         &TheMove,
		    Board	  TheBoard     ) {

   int  	X, Y;

   heuristic	H;

   register int  Step;

   heuristic	PassThresh = VERY_BAD_THING;
   heuristic    UseThresh  = VERY_GOOD_THING;

   Go		ChildGoTo[MAX_CHILDREN];
   heuristic    ChildValue[MAX_CHILDREN];
   MMNode	*Children[MAX_CHILDREN];
   unsigned int NumberChildren;


   // OK, do the drudgery.  Make the children..  For this player
   // Only here will we have to remeber the moves.
   NumberChildren = 0;
   for ( X = 0; X < BOARDXSIZE; X++ ) {
      for ( Y = 0; Y < BOARDYSIZE; Y++ ) {

	 ChildGoTo[NumberChildren].XIs( X );
	 ChildGoTo[NumberChildren].YIs( Y );

	 if ( TheBoard.ValidGo( ChildGoTo[NumberChildren], ThePlayer ) ) {

	    // ---- Found a child.  Build 'em ----

	    // Reset the Heap
	    MMHeap.Reset();

	    Children[NumberChildren] =
	       new MMNode( TheBoard.Spots, ChildGoTo[NumberChildren],
			   ThePlayer					 );

	    // If it is a NULL pointer, we are not getting anymore
	    if ( Children[NumberChildren] == NULL ) {
	       X = BOARDXSIZE;  // So it breaks out of both 'for's
	       break;
	    }

	    Children[NumberChildren]->SetPly( 1 );

	    // Oh No!  recursion!
	    ChildValue[NumberChildren] =
	       Children[NumberChildren]->TREE( -PassThresh, -UseThresh );
					       // WARNING!!!  ^^^^^^^
	    // Do A/B pruning
	    H = ChildValue[NumberChildren];	// Negate it

	    H = -H;
	    if ( H > PassThresh ) {

	       // A better value for the pass threshhold
	       PassThresh = H;

	    }

	    // Prepare for another iteration.
	    NumberChildren++;
	    if ( NumberChildren == MAX_CHILDREN ) {
	       X = BOARDXSIZE;
	       break;
	    }

	 }
      }
   }

   // Find the best move and value..  and return it
   H = ChildValue[0];
   X = 0;
   for ( Step = 1; Step < NumberChildren; Step++ ) {
      if ( ChildValue[Step] > H ) {
	 H = ChildValue[Step];
	 X = Step;
      }
   };
   TheMove = ChildGoTo[X];

   return( H );


};




// ------------------------------------------------------------------------
// --- MEMBERS
// ------------------------------------------------------------------------

// --- MMNode class ---------------------------------------------------------

// -- The recursive tree function
heuristic  MMNode::TREE( heuristic     UseThresh,  // For A/B pruning
			 heuristic     PassThresh,
			 BOOLEAN       Pass  	   // So we can tell an end move
		  ) {

   int  	X, Y;
   SpotStates   OtherGuy;
   heuristic	H;

   register int  Step;

   // Who's the other guy?  (Just flip the owner)
   if( Owner == SPOT_BLACK )
      OtherGuy = SPOT_WHITE;
   else
      OtherGuy = SPOT_BLACK;

   // OK, have I reached the end of my plys for the level?
   if ( CurrentPly == MaxPlys4Level[BrainLevel] ) {

      // Is is a win or loss situation.
      if (Pass) {

	 // If no more children can be created, then it is
	 for ( X = 0; X < BOARDXSIZE; X++ ) {
	    for ( Y = 0; Y < BOARDYSIZE; Y++ ) {

	       AGoTo.XIs( X );
	       AGoTo.YIs( Y );

	       if ( ValidGo( AGoTo, OtherGuy ) )
		  return ( Evaluate(Owner) );
	    }
	 }

	 // No valid moves.  It is the end of a game.
	 return( WinOrLose(Owner) );

      } // end if pass situation

      // Just evaluate self and unroll recusive call
      return( Evaluate(Owner) );

   };

   // OK, do the drudgery.  Make the children
   NumberChildren = 0;
   for ( X = 0; X < BOARDXSIZE; X++ ) {
      for ( Y = 0; Y < BOARDYSIZE; Y++ ) {

	 AGoTo.XIs( X );
	 AGoTo.YIs( Y );

	 if ( ValidGo( AGoTo, OtherGuy ) ) {

	    // Found a child.  Build 'em
	    Children[NumberChildren] = new MMNode( Spots, AGoTo, OtherGuy );

	    // If it is a NULL pointer, we are not getting anymore
	    if ( Children[NumberChildren] == NULL ) {
	       X = BOARDXSIZE;
	       break;
	    }

	    // Otherwise, tick up one more ply
	    CurrentPly++;

	    // Oh No!  recursion!
	    H = Children[NumberChildren]->TREE( -PassThresh, -UseThresh );

	    // Tack back the ply one
	    CurrentPly--;

	    // Do A/B pruning
	    H = -H;	// Negate it
	    if ( H > PassThresh ) {

	       // A better value for the pass threshhold
	       PassThresh = H;

	    } else
	    if ( PassThresh >= UseThresh ) {

	       // not a better value.  Stop pursuing this branch
	       return( PassThresh );

	    };

	    // Prepare for another iteration.
	    NumberChildren++;
	    if ( NumberChildren == MAX_CHILDREN ) {
	       X = BOARDXSIZE;
	       break;
	    }

	 }
      }
   }

   // Ok, if no children, it is a pass.  If this function was called with
   // a pass, then it is a game ender.  Evaluate as a winner or loser.
   // Else, make a single pass child, and recurse into it
   if ( NumberChildren == 0 ) {

      // Account for no children due to lack of memory
      if ( Children[NumberChildren] == NULL ) return( Evaluate(Owner) );

      if ( Pass ) {

	 return( WinOrLose(Owner) );

      } else {

	 // Build the child if we can.  Else return current board eval
	 Children[NumberChildren] = new MMNode( Spots, OtherGuy );

	 // Otherwise, tick up one more ply
	 CurrentPly++;

	 // Oh No!  recursion!
	 PassThresh =
	    Children[NumberChildren]->TREE( -PassThresh, -UseThresh, TRUE );

	 // Tack back the ply one
	 CurrentPly--;

	 NumberChildren = 1;

      } // end if pass

   }

   // This joker has explored all it's children.
   return( PassThresh );

};

// ------------------------------------------------------------------------
// --- PRIMARY GAME ROUND ROUTINE - StartGame
// ------------------------------------------------------------------------

STATUS  StartGame( SpotStates  WhoStarts ) {

   STATUS	TheStatus = STATUS_CONT;
   SpotStates   WhosTurn  = WhoStarts;
   USER_ACTION  Command;

   BOOLEAN	Pass = FALSE;
   Go		AGo;

   // Reset the master board and show it
   MasterBoard.InitBoard2Start();
   GCI.ShowBoard( MasterBoard );

   // Keep playing
   while( TheStatus == STATUS_CONT ) {

      if ( WhosTurn == SPOT_BLACK ) {

	 // Computer turn

	 // Is there a move?
	 if ( !MasterBoard.MoveExists( WhosTurn ) ) {

	    // No move exists.  If end game, end.  Else pass.
	    if ( Pass ) {

	       // End game
	       TheStatus = STATUS_END;
	       EndGame();

	    } else {

	       // Pass
	       GCI.Me.ShowPicture( PIC_CURIOUSFROWN );
	       GCI.Me.SayText( "HMMM!!!", "LOOKS LIKE I HAVE TO", "PASS" );
	       delay( 1000 );
	       Pass = TRUE;

	    } // end if game end

	 } else {

	    // Clear the pass flag and move
	    Pass = FALSE;
	    ComputerMoves();

	 }

      } else {

	 // Player turn

	 // Is there a move?
	 if ( !MasterBoard.MoveExists( WhosTurn ) ) {

	    // No move exists.  If end game, end.  Else pass.
	    if ( Pass ) {

	       // End game
	       TheStatus = STATUS_END;
	       EndGame();

	    } else {

	       // Pass
	       GCI.YouMustPass();
	       Pass = TRUE;

	    } // end if game end

	 } else {

	    // Show valid moves
	    GCI.ShowValidMoves( MasterBoard );

	    // Clear the pass flag
	    Pass = FALSE;

	    // Loop until the player does something that shifts turns or quits
	    do {

	       Command = GCI.WaitUser( AGo );

	       // Switch through actions
	       switch( Command ) {

		  // Set brain levels
		  case  CLICK_BRAIN1:
				      BrainSetting = SIMPLETON;
				      GCI.BrainSetting( SIMPLETON );
				      break;
		  case  CLICK_BRAIN2:
				      BrainSetting = DULLARD;
				      GCI.BrainSetting( DULLARD );
				      break;
		  case  CLICK_BRAIN3:
				      BrainSetting = AVERAGE;
				      GCI.BrainSetting( AVERAGE );
				      break;
		  case  CLICK_BRAIN4:
				      BrainSetting = SWIFT;
				      GCI.BrainSetting( SWIFT );
				      break;
		  case  CLICK_BRAIN5:
				      BrainSetting = GENIUS;
				      GCI.BrainSetting( GENIUS );
				      break;
		  case  CLICK_BRAIN6:
				      BrainSetting = EXPERIMENTAL;
				      GCI.BrainSetting( EXPERIMENTAL );
				      break;

		  case  CLICK_HELP:   GCI.PushMainButton( HELP_BUTTON );
				      HelpHuman();
				      GCI.PopMainButton( HELP_BUTTON );
				      break;

		  case  CLICK_DONE:   GCI.PushMainButton( DONE_BUTTON );
				      GCI.Me.ShowPicture( PIC_ARMSCROSS );
				      GCI.Me.SayText( "CHICKEN!", "ARE YA?", NULL );
				      delay( 1000 );
				      GCI.PopMainButton( DONE_BUTTON );

				      // End it all
				      if (GCI.YouSure()) {

					 TheStatus = STATUS_DONE;
					 Command   = CLICK_DONE;

				      }  else {

					 GCI.Me.ShowPicture( PIC_CALM);
					 GCI.Me.SayText( "GOOD", "THAT'S BETTER", NULL );
					 delay( 1000 );

					 // Dummy command so it will
					 // continue allowing user to click
					 Command = CLICK_HELP;

				      }
				      break;

		  case  CLICK_NEW :   GCI.PushMainButton( NEW_BUTTON );
				      GCI.Me.ShowPicture( PIC_WHAT );
				      GCI.Me.SayText( "NEW GAME?", "I'M GONNA HAVE TO", "DOCK YOU 100 POINTS" );
				      delay( 1000 );
				      GCI.PopMainButton( NEW_BUTTON );

				      // End it all
				      if (GCI.YouSure()) {

					 GCI.Me.ShowPicture( PIC_CALM);
					 GCI.Me.SayText( "OK", "YOU'RE THE BOSS", NULL );

					 RunningScore -= 100;
					 GCI.Score( RunningScore );

					 TheStatus = STATUS_NEW;
					 Command   = CLICK_DONE;

				      }  else {

					 GCI.Me.ShowPicture( PIC_CALM);
					 GCI.Me.SayText( "GOOD", "THAT'S BETTER", NULL );
					 delay( 1000 );

				      }
				      break;

		  case  CLICK_BOARD:
				      if ( MasterBoard.ValidGo(AGo, SPOT_WHITE) ) {

					 // A valid player move
					 MasterBoard.DoGo( AGo, SPOT_WHITE );
					 GCI.ShowBoard( MasterBoard );

					 // Break out
					 Command = CLICK_DONE;
				      }
				      break;


	       } //end case


	    } while( Command != CLICK_DONE );

	 } // end if move exists

      } // end if player turn

      // Flip turn
      if   ( WhosTurn == SPOT_WHITE ) WhosTurn = SPOT_BLACK;
      else WhosTurn = SPOT_WHITE;

   } // end while play

   return( TheStatus );


};



// ------------------------------------------------------------------------
// --- MAIN
// ------------------------------------------------------------------------
void  main( int  argn,  char  *argc[] ) {

   //                   SORRY!! ^^^ will cause a warning

   USER_ACTION	Command;
   BOOLEAN	YouFirst;

   Go		DummyGo;

   STATUS	ReturnStatus;

   randomize();

   // Should we show the heuristic?
   if ( argn > 1 ) ShowH = TRUE;

   // First time in, so go ahead and init main screen
   GCI.InitMainScreen();

   // Scrub it
   ReturnStatus = STATUS_CONT;

   // Loop through user actions
   do {

      // Check for a new game request during last game
      if ( ReturnStatus == STATUS_NEW ) {
	 Command = CLICK_NEW;

      } else
	 Command = GCI.WaitUser( DummyGo );


      // Switch through actions
      switch( Command ) {

	 // Set brain levels
	 case  CLICK_BRAIN1:
			     BrainSetting = SIMPLETON;
			     GCI.BrainSetting( SIMPLETON );
			     break;
	 case  CLICK_BRAIN2:
			     BrainSetting = DULLARD;
			     GCI.BrainSetting( DULLARD );
			     break;
	 case  CLICK_BRAIN3:
			     BrainSetting = AVERAGE;
			     GCI.BrainSetting( AVERAGE );
			     break;
	 case  CLICK_BRAIN4:
			     BrainSetting = SWIFT;
			     GCI.BrainSetting( SWIFT );
			     break;
	 case  CLICK_BRAIN5:
			     BrainSetting = GENIUS;
			     GCI.BrainSetting( GENIUS );
			     break;
	 case  CLICK_BRAIN6:
			     BrainSetting = EXPERIMENTAL;
			     GCI.BrainSetting( EXPERIMENTAL );
			     break;

	 case  CLICK_HELP:   GCI.PushMainButton( HELP_BUTTON );
			     delay( BUTTON_TIME );
			     GCI.PopMainButton( HELP_BUTTON );
			     break;

	 case  CLICK_NEW:    GCI.PushMainButton( NEW_BUTTON );
			     delay( BUTTON_TIME );

			     GCI.PopMainButton( NEW_BUTTON );
			     YouFirst = GCI.WhoFirst();
			     if( YouFirst )
				ReturnStatus = StartGame(SPOT_WHITE);
			     else
				ReturnStatus = StartGame(SPOT_BLACK);

			     // Check for quit
			     if ( ReturnStatus == STATUS_DONE )
				Command = CLICK_DONE;

			     break;

	 case  CLICK_DONE:   GCI.PushMainButton( DONE_BUTTON );
			     delay( BUTTON_TIME );
			     GCI.PopMainButton( DONE_BUTTON );
			     break;


      } //end case


   } while( Command != CLICK_DONE );

   // Done
   GCI.Me.ShowPicture( PIC_BYE );
   GCI.Me.SayText( "GOODBYE!", NULL, NULL );
   delay( 3000 );

};


