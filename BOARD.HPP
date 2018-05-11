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

// --- Playing board class ------------------------------------------------
class  Board {

   BOOLEAN  ValidNorth( void );
   void     FlipNorth(  void );
   BOOLEAN  ValidSouth( void );
   void     FlipSouth(  void );
   BOOLEAN  ValidEast ( void );
   void     FlipEast  ( void );
   BOOLEAN  ValidWest ( void );
   void     FlipWest  ( void );
   BOOLEAN  ValidNorthEast( void );
   void     FlipNorthEast(  void );
   BOOLEAN  ValidNorthWest( void );
   void     FlipNorthWest(  void );
   BOOLEAN  ValidSouthEast( void );
   void     FlipSouthEast(  void );
   BOOLEAN  ValidSouthWest( void );
   void     FlipSouthWest(  void );

   // Dump for the go.  Let's trade some space for time
   static   Go	    TheGo;

   // Which player 'owns' the board
   // This will provide a speed benifit, as well.
   static   SpotStates	Player;

   // Finally, there is an evaluation boards.  They contain the general
   // evaluation point values for both Player and Opponant
   static int	  PlayerVal[BOARDXSIZE][BOARDYSIZE];
   static int	  OpponantVal[BOARDXSIZE][BOARDYSIZE];


 protected:
   // Also, we will remeber the BRAIN level for the evaluations
   static   BRAIN	BrainLevel;

 public:

   // It has 64 spots, 8 by 8
   Spot	  Spots[BOARDXSIZE][BOARDYSIZE];

   // Constructors
   Board( void ) {};	      // Void constructor
   Board( Board *OldBoard );  // Copy
   Board( Spot   TheSpots[BOARDXSIZE][BOARDYSIZE] );  // Copy

   // Initialize board to starting pieces
   void	     InitBoard2Start( void );

   // Set the brain level
   void      BrainIs( BRAIN  TheLevel ) { BrainLevel = TheLevel; };

   // Actions
   void      DoGo   ( Go  GoTo, SpotStates  ThePlayer  );
   heuristic Evaluate(  SpotStates  ThePlayer    );
   heuristic WinOrLose( SpotStates  ThePlayer    );
   int       Count( SpotStates  ThePlayer    );

   // Reporters
   BOOLEAN   ValidGo( Go  GoTo, SpotStates  ThePlayer );
   BOOLEAN   MoveExists( SpotStates  ThePlayer );

};


