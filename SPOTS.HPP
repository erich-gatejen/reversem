// --- Enumerate the states of a board spot -------------------------------
// --- I'm doing this instead of a 'enum' to save space
enum  SpotStates {
			SPOT_BLANK =   0,
			SPOT_BLACK,
			SPOT_WHITE
};


// --- Go.  Place a spot on a board ---------------------------------------
struct  Go {
   signed int  X;  // Negitive is significant.  It indicates a bounds break
   signed int  Y;

 public:
   void XIs( unsigned char   TheX ) { X = TheX; };
   void YIs( unsigned char   TheY ) { Y = TheY; };

   unsigned char  XIs( void ) { return X; };
   unsigned char  YIs( void ) { return Y; };

};


// --- Playing board spot class -------------------------------------------
struct  Spot  {

   SpotStates	State;

  public:

   // Reporters
   BOOLEAN  IsSpotBlank( void ) { if(State==SPOT_BLANK)return(TRUE);
				  else return( FALSE ); 		};
   BOOLEAN  IsSpotBlack( void ) { if(State==SPOT_BLACK)return(TRUE);
				  else return( FALSE ); 		};
   BOOLEAN  IsSpotWhite( void ) { if(State==SPOT_WHITE)return(TRUE);
				  else return( FALSE ); 		};
   BOOLEAN  IsSpot( SpotStates  TheState ) {
				  if(State==TheState)return(TRUE);
				  else return( FALSE ); 		};
   BOOLEAN  IsOther( SpotStates TheState ) {
				  if((State==SPOT_WHITE)&&(TheState==SPOT_BLACK))
				     return(TRUE);
				  if((State==SPOT_BLACK)&&(TheState==SPOT_WHITE))
				     return(TRUE);
				  return( FALSE );
									};
   void       Is( SpotStates TheState ) { State = TheState; };
   SpotStates Is( void ) { return State; };

   // Set
   void     SpotIsBlack( void ) { State = SPOT_BLACK; };
   void     SpotIsWhite( void ) { State = SPOT_WHITE; };
   void     SpotIsBlank( void ) { State = SPOT_BLANK; };
   void     SpotIs( SpotStates  TheState ) { State = TheState; };
   void     FlipSpot(    void ) {
				  if ( State == SPOT_BLACK ) State = SPOT_WHITE;
				  else State =  SPOT_BLACK;
				};


};

