/*--------------------------------------------------------------------------

  REVERSEM :

  UNIT     : GRAPHICS CONTROL

  COPYRIGHT (C) 1994 Erich P. Gatejen  ALL RIGHTS RESERVED
  This is Freeware.  You may distribute it as you wish.  However, you may not
  distribute a modified version without my consent.

  Feel free to cut and paste any algorthm.

  NOTE: XTILE is (C) 1992, 1994 by myself.  It is NOT freeware.  You may use
	it for personal projects, but otherwise, it is not to be distributed
	outside this REVERSEM package.  (Contact me if you wish to do
	otherwise)

---------------------------------------------------------------------------*/

#include"mypic.hpp"


// --- Definitions ---------------------------------------------------------

// --- Define possible user actions
enum  USER_ACTION { NO_ACTION = 0,
		    CLICK_BRAIN1,
		    CLICK_BRAIN2,
		    CLICK_BRAIN3,
		    CLICK_BRAIN4,
		    CLICK_BRAIN5,
		    CLICK_BRAIN6,
		    CLICK_BOARD,
		    CLICK_HELP,
		    CLICK_NEW,
		    CLICK_DONE,
		    CLICK_PICTURE
};

// --- Main buttons
enum  MAIN_BUTTONS { HELP_BUTTON = 0,
		     NEW_BUTTON,
		     DONE_BUTTON
};


// --- Graphics control class -----------------------------------------------
class  GraphicsControl {

    BRAIN  BrainButtonState;

    // Private functions
    void   	 InitTGAHooks( void );
    int	   	 MouseView( void );
    USER_ACTION  ParseMouseCkick( Go&  AGo );

 public:

    // --- The my picture class
    MyPicture	 Me;

    // Constructor and Destructor
    GraphicsControl( void );
    ~GraphicsControl();

    // Primary routines
    void	 InitMainScreen( void );
    void 	 ShowBoard( Board     TheBoard );
    USER_ACTION  WaitUser( Go&  AGo );

    // Brain level buttons
    void	 BrainSetting( BRAIN  Setting );

    // Button control
    void 	 PushMainButton( MAIN_BUTTONS  TheButton );
    void 	 PopMainButton( MAIN_BUTTONS  TheButton );

    // Pop ups
    BOOLEAN      WhoFirst( void );  // Return TRUE if player first
    BOOLEAN      YouSure( void );   // Return TRUE if player is sure
    void	 YouMustPass( void );

    // Spot items
    void  	 ShowValidMoves( Board TheBoard );
    void	 Here( Go  TheGo );

    // Show the score
    void	 Score( int     TheScore );
};


