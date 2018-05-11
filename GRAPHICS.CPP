/*--------------------------------------------------------------------------

  REVERSEM :

  UNIT     : GRAPHICS CONTROL

  Most of the methods used here are not very 'C++' like.  They were used in
  development of the game Anarchy (ok, continuing development, it will be
  released in Jan 95).  Hey.  It's an AI class;  so, a bit of code reuse
  did me well.

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
#include<alloc.h>
#include<string.h>
#include"reversem.hpp"
#include"spots.hpp"
#include"board.hpp"
#include"eval.hpp"
#include"graphics.hpp"

#include"conio.h"

// ------------------------------------------------------------------------
// --- GRAPHICS DEFINEs
// ------------------------------------------------------------------------
#define  MAIN_PAGE	PAGE1

#define  MOUSE_EVENTS   (XLEFT_BUTTON | XRIGHT_BUTTON)

#define  FONT_4_SIZE	  192

#define  BRAIN_BUTTONSX       4
#define  BRAIN_BUTTONSY     177
#define  BRAIN_BUTTONXSIZE   52
#define  BRAIN_BUTTONYSIZE   20
#define  BRAIN_BUTTONXSEP    50

#define  MAIN_BUTTONSX	    152
#define  MAIN_BUTTONSY	      0
#define  MAIN_BUTTONSXSIZE   56
#define  MAIN_BUTTONSYSIZE   20

#define  POPUP_XSIZE	    104
#define  POPUP_YSIZE	     32

#define  BOARDX             116
#define  BOARDY		     32

#define  SPOT_XYSIZE	     24
#define  SPOT_TRIMXSIZE	     20
#define  SPOT_TRIMYSIZE	     17
#define  SPOT_TRIMX	      4
#define  SPOT_TRIMY	      5

#define  SCORE_X	    112
#define  SCORE_Y	     13
#define  SCORE_XSIZE	     32
#define  SCORE_YSIZE	      5
#define  SCORE_COLOR	     15


// ------------------------------------------------------------------------
// --- TGA offsets	( a hold over from Anarchy method )
// ------------------------------------------------------------------------

// -- TGA set for main load ----
#define	PREP_MOUSE_DATA_SIZE	256
#define	PREP_MOUSE_DATA_SITE	0

#define PREP_MOUSE_MASK_SIZE	64
#define PREP_MOUSE_MASK_SITE	(PREP_MOUSE_DATA_SITE + PREP_MOUSE_DATA_SIZE)

#define	WAIT_MOUSE_DATA_SIZE	256
#define	WAIT_MOUSE_DATA_SITE	(PREP_MOUSE_MASK_SITE + PREP_MOUSE_MASK_SIZE)

#define WAIT_MOUSE_MASK_SIZE	64
#define WAIT_MOUSE_MASK_SITE	(WAIT_MOUSE_DATA_SITE + WAIT_MOUSE_DATA_SIZE)

#define MAIN_PAL_SIZE		768
#define MAIN_PAL_SITE           (WAIT_MOUSE_MASK_SITE + WAIT_MOUSE_MASK_SIZE)

#define SPOT_SIZE		576
#define SPOT_SITE		(MAIN_PAL_SITE + MAIN_PAL_SIZE)

#define HERE_SIZE		576
#define HERE_SITE		(SPOT_SITE + SPOT_SIZE)

#define OK_SIZE			576
#define OK_SITE			(HERE_SITE + HERE_SIZE)

#define BLACK_SPOTS_EACH	340
#define BLACK_SPOTS_SIZE	(BLACK_SPOTS_EACH * 6)
#define BLACK_SPOTS_SITE	(OK_SITE + OK_SIZE)

#define WHITE_SPOTS_EACH	340
#define WHITE_SPOTS_SIZE	(WHITE_SPOTS_EACH * 6)
#define WHITE_SPOTS_SITE	(BLACK_SPOTS_SITE + BLACK_SPOTS_SIZE)

#define MUST_PASS_SIZE		3328
#define MUST_PASS_SITE          (WHITE_SPOTS_SITE + WHITE_SPOTS_SIZE)

#define WHO_1ST_SIZE		3328
#define WHO_1ST_SITE		(MUST_PASS_SITE + MUST_PASS_SIZE)

#define YOU_SURE_SIZE		3328
#define YOU_SURE_SITE		(WHO_1ST_SITE + WHO_1ST_SIZE)

#define BRAIN_BUTTON_EACH	1040
#define BRAIN_BUTTON_SIZE	(BRAIN_BUTTON_EACH * 6)
#define BRAIN_BUTTON_SITE       (YOU_SURE_SITE + YOU_SURE_SIZE)

#define BRAIN_BPUSHED_SITE	(BRAIN_BUTTON_SITE + BRAIN_BUTTON_SIZE)

#define MAIN_BUTTON_EACH	1120
#define MAIN_BUTTON_SIZE	(MAIN_BUTTON_EACH * 3)
#define MAIN_BUTTON_SITE	(BRAIN_BPUSHED_SITE + BRAIN_BUTTON_SIZE)

#define MAIN_BPUSHED_SITE	(MAIN_BUTTON_SITE + MAIN_BUTTON_SIZE)

#define LOAD1_SIZE		(MAIN_BPUSHED_SITE + MAIN_BUTTON_SIZE)

#define POPUP_RELOAD_SITE	LOAD1_SIZE

#define SCORE_RELOAD_SITE	(POPUP_RELOAD_SITE + WHO_1ST_SIZE)


#define MAIN_SCREEN_FILE_SITE   36400



// ------------------------------------------------------------------------
// --- DATA
// ------------------------------------------------------------------------

// --- The TGA
char	*TGA;

// --- TGA Hooks
char    *White_Spots;
char	*Black_Spots;

// --- The Main Graphics File
FILE	*MainGFile;

// --- Mouse data
unsigned int	Mouse_Data_Ready;
unsigned int    MouseX;
unsigned int    MouseY;
unsigned int    Mouse_Flags;


// --- Data for the 4-pix font
char    Font4[FONT_4_SIZE];



// ------------------------------------------------------------------------
// --- SPECIAL MOUSE HANDLER FUNCTIONS AND MEMBERS
// ------------------------------------------------------------------------

// The declaration of this function MUST NOT be changed for it to work
// with XTile.
void  GMouse_Handler( unsigned int X,
		      unsigned int Y,
		      unsigned int Event_Mask ) {

   Mouse_Data_Ready = TRUE;
   MouseX           = X;
   MouseY           = Y;
   Mouse_Flags      = Event_Mask;

};

int  GraphicsControl::MouseView ( void ) {
/* View a fleet screen and wait an event */

   unsigned int X, Y, Temp;  /* Last known mouse locations */

   Mouse_Data_Ready = FALSE;
   XWhere_Mouse( &X, &Y );
   XMouse_Active( MOUSE_EVENTS, X, Y, NORMALPAGE );


   /* Wait until the mouse button is pressed or a key */
   while ((Mouse_Data_Ready == FALSE)&&( kbhit() == 0)) {

   }

   /* Turn mouse off */
   XMouse_InActive();

   if ( kbhit() ) {
      Temp = getch();
      if (Temp == NULL) Temp = getch();  // Take care of function keys
      return Temp;
   } else
      return 0;

}


USER_ACTION  GraphicsControl::ParseMouseCkick( Go&  AGo ) {

   unsigned int  Temp;

   // This will be some pretty cryptic code.

   // Check top button region
   if ( (MouseX >= MAIN_BUTTONSX)&&(MouseY < MAIN_BUTTONSYSIZE) ) {

      // return the specific button
      Temp = MouseX - MAIN_BUTTONSX;
      Temp = Temp / MAIN_BUTTONSXSIZE;
      return( (USER_ACTION) (((unsigned int)CLICK_HELP) + Temp) );

   } else

   // Check if in brain button area
   if ( ( MouseX >= BRAIN_BUTTONSX                          )&&
	( MouseX <= (BRAIN_BUTTONSX+(2*BRAIN_BUTTONXSEP))   )&&
	( MouseY >= BRAIN_BUTTONSY			    )&&
	( MouseY <= (BRAIN_BUTTONSY+(3*BRAIN_BUTTONYSIZE))  )   ) {

      // return the specific button
      if ( MouseX < (BRAIN_BUTTONSX + BRAIN_BUTTONXSEP) ) {

	 // left half
	 if ( MouseY < (BRAIN_BUTTONSY + BRAIN_BUTTONYSIZE) )
	    return CLICK_BRAIN1;
	 if ( MouseY < (BRAIN_BUTTONSY + (2 * BRAIN_BUTTONYSIZE)) )
	    return CLICK_BRAIN3;
	 return CLICK_BRAIN5;

      } else {

	 // right half
	 if ( MouseY < (BRAIN_BUTTONSY + BRAIN_BUTTONYSIZE) )
	    return CLICK_BRAIN2;
	 if ( MouseY < (BRAIN_BUTTONSY + (2 * BRAIN_BUTTONYSIZE)) )
	    return CLICK_BRAIN4;
	 return CLICK_BRAIN6;

      }

   } else

   // Is it in the board
   if ((MouseX >= BOARDX)&&(MouseX < (BOARDX + (BOARDXSIZE *  SPOT_XYSIZE) ))&&
       (MouseY >= BOARDY)&&(MouseY < (BOARDY + (BOARDYSIZE *  SPOT_XYSIZE) ))  ) {

      Temp = MouseX - BOARDX;
      Temp = Temp / SPOT_XYSIZE;
      AGo.XIs( Temp );

      Temp = MouseY - BOARDY;
      Temp = Temp / SPOT_XYSIZE;
      AGo.YIs( Temp );

      return CLICK_BOARD;

   }

   return NO_ACTION;


};



// ------------------------------------------------------------------------
// --- PRIVATE MEMBERS
// ------------------------------------------------------------------------

// -- Member :  Initialize the TGA hooks -----------------------------------
void   GraphicsControl::InitTGAHooks( void ) {

   // Set the TGA hooks
   White_Spots  = TGA + WHITE_SPOTS_SITE;
   Black_Spots  = TGA + BLACK_SPOTS_SITE;

};

// ------------------------------------------------------------------------
// --- CONSTRUCTOR/DESTRUCTOR
// ------------------------------------------------------------------------
GraphicsControl::GraphicsControl( void ) {

   FILE	 *FontFile;

   // Allocate TGA and generate the hooks.  Any error is fatal.
   TGA = (char *) farmalloc( 65530 );  // I know, malloc.  It's safer here though
   if ( TGA == NULL ) {
      puts( ".. FATAL ERROR .. Could not allocate memory for graphics!\n\r\n\r");
      exit( 1 );
   }
   InitTGAHooks();

   // Open the main graphics file.  It will be closed by the destructor
   MainGFile = fopen( "reversem.cmb", "rb");
   if ( MainGFile == NULL ) {
      puts( ".. FATAL ERROR .. Could not open the 'reversem.cmb' graphics file.\n\r\n\r");
      exit(2);
   }

   // Load the TGA
   fread( TGA, LOAD1_SIZE, 1, MainGFile );

   // Init various items
   BrainButtonState = SIMPLETON;  // This is the default!

   // Load the font and register it
   FontFile = fopen("font4x6.msf","rb");
   if ( MainGFile == NULL ) {
      puts( ".. FATAL ERROR .. Could not open the 'font4x6.msf' font file.\n\r\n\r");
      exit(2);
   }
   fread(Font4, FONT_4_SIZE, 1, FontFile);
   fclose(FontFile);
   XRegister_Font4( 0, 65, ' ', DONTUPLOADFONT, Font4 ); /* Font4 is dummy */
   Register_Font_Masks4( Font4 );

};

GraphicsControl::~GraphicsControl() {

   fclose(MainGFile);
   farfree(TGA);

   // Put it back to text screen
   textmode ( LASTMODE );

}


// ------------------------------------------------------------------------
// --- PUBLIC MEMBERS
// ------------------------------------------------------------------------

// -- Member :  Initialize the main screen --------------------------------
void  GraphicsControl::InitMainScreen( void ) {

   char*  TMA;

   // We will temporarily need 64000 bytes
   TMA = (char *) farmalloc( 64000 );
   if ( TMA == NULL ) {
      puts( ".. FATAL ERROR .. Could not allocate memory for graphics screen load!\n\r\n\r");
      exit( 1 );
   }

   // Clear video memory and set the mode
   XWait_Retrace();
   XInit_Mode( 0 );
   XClear( 0 );
   XInit_Mode( 0 );

   // Load and put the main screen (in the main page)
   XSet_Write_Page( MAIN_PAGE, 320 );
   fseek( MainGFile, MAIN_SCREEN_FILE_SITE, SEEK_SET );
   fread( TMA,   1,  (320*200),  MainGFile);
   XPut_Tile( 0, 0, 320, 200, TMA  );
   fread( TMA,   1,  (320*40),   MainGFile );
   XPut_Tile( 0, 200, 320, 40, TMA  );


   // NOTE: The default brain button is IDIOT (SIMPLETON);
   //       the loaded screen has that button pushed

   // ALSO: The screen will show a completely blank board

   // Do any reloads
   XDownload_TileP  (  SCORE_X, SCORE_Y, SCORE_XSIZE, SCORE_YSIZE,
		       TGA +  SCORE_RELOAD_SITE				 );


   // Register mouse.  (default mouse is prep mouse)
   XRegister_Mouse( (TGA + PREP_MOUSE_DATA_SITE),
		    (TGA + PREP_MOUSE_MASK_SITE),
		    GMouse_Handler		  );

   // Show the welcome picture
   Me.ShowPicture( PIC_NOEXP );
   Me.SayText( "HI.  I AM ERICH.", "HIT 'NEW' IF YOU", "WANT TO PLAY" );

   // Show the screen.  Page flip to main page.
   XSet_Display_PageP( MAIN_PAGE, 320, 240, 0, 0, 0, 255,
		       TGA + MAIN_PAL_SITE );

   // Don't need the TMA anymore
   farfree( TMA );

};


// -- Member :  Show a board.  No finesse, just blast it out --------------
void  GraphicsControl::ShowBoard( Board     TheBoard ) {

   unsigned int  X, Y, XLoc, YLoc;

   // Lets try to avoid some flicker
   XWait_Retrace();

   // Put the tiles
   XLoc = BOARDX;
   for ( X = 0; X < BOARDXSIZE; X++ ) {
      YLoc = BOARDY;
      for ( Y = 0; Y < BOARDYSIZE; Y++ ) {

	 XPut_Tile( XLoc, YLoc, SPOT_XYSIZE, SPOT_XYSIZE, TGA + SPOT_SITE );
	 if ( TheBoard.Spots[X][Y].IsSpotBlack() )
	    XPut_Tile( XLoc + SPOT_TRIMX, YLoc + SPOT_TRIMY,
		       SPOT_TRIMXSIZE, SPOT_TRIMYSIZE,
		       TGA + BLACK_SPOTS_SITE		 );
	 if ( TheBoard.Spots[X][Y].IsSpotWhite() )
	    XPut_Tile( XLoc + SPOT_TRIMX, YLoc + SPOT_TRIMY,
		       SPOT_TRIMXSIZE, SPOT_TRIMYSIZE,
		       TGA + WHITE_SPOTS_SITE		 );

	 YLoc = YLoc + SPOT_XYSIZE;
      }
      XLoc = XLoc + SPOT_XYSIZE;
   }

};


// -- Member :  Show valid moves for the player (WHITE) ----------------
void   GraphicsControl::ShowValidMoves( Board TheBoard ) {

   register unsigned int  X, Y;
   unsigned int		  XLoc, YLoc;
   Go			  AGo;

   // Lets try to avoid some flicker
   XWait_Retrace();

   // Put the tiles
   XLoc = BOARDX;
   for ( X = 0; X < BOARDXSIZE; X++ ) {
      YLoc = BOARDY;
      for ( Y = 0; Y < BOARDYSIZE; Y++ ) {

	 AGo.XIs( X );
	 AGo.YIs( Y );
	 if ( TheBoard.ValidGo( AGo, SPOT_WHITE ) )
	    XPut_Tile( XLoc, YLoc, SPOT_XYSIZE, SPOT_XYSIZE,
		       TGA + OK_SITE				 );

	 YLoc = YLoc + SPOT_XYSIZE;
      }
      XLoc = XLoc + SPOT_XYSIZE;
   }

};



// -- Member :  Wait for a user event -------------------------------------
USER_ACTION  GraphicsControl::WaitUser( Go&  AGo ) {

   int	  	ViewReturn;
   USER_ACTION  ParseReturn = NO_ACTION;

   while ( ParseReturn == NO_ACTION ) {

      ViewReturn = MouseView();
      if ( ViewReturn == 0 )
	 ParseReturn = ParseMouseCkick( AGo );

   }

   return( ParseReturn );
};


// -- Member :  Set the appropriate button for the brain setting ----------
void	 GraphicsControl::BrainSetting( BRAIN  Setting ) {

   if ( Setting == BrainButtonState ) return;

   // Undo old button
   XWait_Retrace();
   switch( BrainButtonState ) {

      case  SIMPLETON:
	    XPut_TileA_M( BRAIN_BUTTONSX, BRAIN_BUTTONSY, BRAIN_BUTTONXSIZE,
			  BRAIN_BUTTONYSIZE,
			  TGA + BRAIN_BUTTON_SITE 	                    );
	    break;

      case  DULLARD:
	    XPut_TileA_M( BRAIN_BUTTONSX + BRAIN_BUTTONXSEP,
			  BRAIN_BUTTONSY, BRAIN_BUTTONXSIZE,
			  BRAIN_BUTTONYSIZE,
			  TGA + (BRAIN_BUTTON_SITE + BRAIN_BUTTON_EACH )    );
	    break;

      case  AVERAGE:
	    XPut_TileA_M( BRAIN_BUTTONSX,
			  BRAIN_BUTTONSY + BRAIN_BUTTONYSIZE,
			  BRAIN_BUTTONXSIZE, BRAIN_BUTTONYSIZE,
			  TGA + (BRAIN_BUTTON_SITE + (2*BRAIN_BUTTON_EACH) ) );
	    break;

      case  SWIFT:
	    XPut_TileA_M( BRAIN_BUTTONSX + BRAIN_BUTTONXSEP,
			  BRAIN_BUTTONSY + BRAIN_BUTTONYSIZE,
			  BRAIN_BUTTONXSIZE, BRAIN_BUTTONYSIZE,
			  TGA + (BRAIN_BUTTON_SITE + (3*BRAIN_BUTTON_EACH) ) );
	    break;

      case  GENIUS:
	    XPut_TileA_M( BRAIN_BUTTONSX,
			  BRAIN_BUTTONSY + (2*BRAIN_BUTTONYSIZE),
			  BRAIN_BUTTONXSIZE, BRAIN_BUTTONYSIZE,
			  TGA + (BRAIN_BUTTON_SITE + (4*BRAIN_BUTTON_EACH) ) );
	    break;

      case  EXPERIMENTAL:
	    XPut_TileA_M( BRAIN_BUTTONSX + BRAIN_BUTTONXSEP,
			  BRAIN_BUTTONSY + (2*BRAIN_BUTTONYSIZE),
			  BRAIN_BUTTONXSIZE, BRAIN_BUTTONYSIZE,
			  TGA + (BRAIN_BUTTON_SITE + (5*BRAIN_BUTTON_EACH) ) );
	    break;
   }

   // Put the new button
   BrainButtonState = Setting;
   switch( BrainButtonState ) {

      case  SIMPLETON:
	    XPut_TileA_M( BRAIN_BUTTONSX, BRAIN_BUTTONSY, BRAIN_BUTTONXSIZE,
			  BRAIN_BUTTONYSIZE,
			  TGA + BRAIN_BPUSHED_SITE 	                    );
	    break;

      case  DULLARD:
	    XPut_TileA_M( BRAIN_BUTTONSX + BRAIN_BUTTONXSEP,
			  BRAIN_BUTTONSY, BRAIN_BUTTONXSIZE,
			  BRAIN_BUTTONYSIZE,
			  TGA + (BRAIN_BPUSHED_SITE + BRAIN_BUTTON_EACH )    );
	    break;

      case  AVERAGE:
	    XPut_TileA_M( BRAIN_BUTTONSX,
			  BRAIN_BUTTONSY + BRAIN_BUTTONYSIZE,
			  BRAIN_BUTTONXSIZE, BRAIN_BUTTONYSIZE,
			  TGA + (BRAIN_BPUSHED_SITE + (2*BRAIN_BUTTON_EACH) ) );
	    break;

      case  SWIFT:
	    XPut_TileA_M( BRAIN_BUTTONSX + BRAIN_BUTTONXSEP,
			  BRAIN_BUTTONSY + BRAIN_BUTTONYSIZE,
			  BRAIN_BUTTONXSIZE, BRAIN_BUTTONYSIZE,
			  TGA + (BRAIN_BPUSHED_SITE + (3*BRAIN_BUTTON_EACH) ) );
	    break;

      case  GENIUS:
	    XPut_TileA_M( BRAIN_BUTTONSX,
			  BRAIN_BUTTONSY + (2*BRAIN_BUTTONYSIZE),
			  BRAIN_BUTTONXSIZE, BRAIN_BUTTONYSIZE,
			  TGA + (BRAIN_BPUSHED_SITE + (4*BRAIN_BUTTON_EACH) ) );
	    break;

      case  EXPERIMENTAL:
	    XPut_TileA_M( BRAIN_BUTTONSX + BRAIN_BUTTONXSEP,
			  BRAIN_BUTTONSY + (2*BRAIN_BUTTONYSIZE),
			  BRAIN_BUTTONXSIZE, BRAIN_BUTTONYSIZE,
			  TGA + (BRAIN_BPUSHED_SITE + (5*BRAIN_BUTTON_EACH) ) );
	    break;
   }

};


// -- Member :  Push the button specified -----------------------------
void  GraphicsControl::PushMainButton( MAIN_BUTTONS  TheButton ) {

   XPut_Tile( MAIN_BUTTONSX + (TheButton * MAIN_BUTTONSXSIZE),
	     MAIN_BUTTONSY, MAIN_BUTTONSXSIZE, MAIN_BUTTONSYSIZE,
	     TGA + (MAIN_BPUSHED_SITE + (TheButton * MAIN_BUTTON_EACH))	  );

};


// -- Member :  Pop the button specified -----------------------------
void  GraphicsControl::PopMainButton( MAIN_BUTTONS  TheButton ) {

   XPut_Tile( MAIN_BUTTONSX + (TheButton * MAIN_BUTTONSXSIZE),
	     MAIN_BUTTONSY, MAIN_BUTTONSXSIZE, MAIN_BUTTONSYSIZE,
	     TGA + (MAIN_BUTTON_SITE + (TheButton * MAIN_BUTTON_EACH))	  );

};


// -- Member :  Who is first -----------------------------------------
BOOLEAN  GraphicsControl::WhoFirst( void ) {

   BOOLEAN  ReturnValue;

   // Definitions unique to this popup
   #define  WHO_FIRSTX	 	160
   #define  WHO_FIRSTY	 	87
   #define  WHO_FIRST_YOUX      (WHO_FIRSTX + 12)
   #define  WHO_FIRST_YOUY      (WHO_FIRSTY + 16)
   #define  WHO_FIRST_MEX       (WHO_FIRSTX + 62)
   #define  WHO_FIRST_YMSIZEX   30
   #define  WHO_FIRST_YMSIZEY   11


   // Return TRUE if it is the player (YOU), FALSE is (ME).

   // Put the popup on the screen
   XDownload_TileP  (  WHO_FIRSTX, WHO_FIRSTY, POPUP_XSIZE, POPUP_YSIZE,
		       TGA + POPUP_RELOAD_SITE				 );
   XPut_Tile	    (  WHO_FIRSTX, WHO_FIRSTY, POPUP_XSIZE, POPUP_YSIZE,
		       TGA + WHO_1ST_SITE				 );

   // Loop until valid
   for(;;) {

      // Wait for a mouse event
      MouseView();

      // See if valid
      if ( (MouseY >= WHO_FIRST_YOUY)&&
	   (MouseY < (WHO_FIRST_YOUY + WHO_FIRST_YMSIZEY)) ) {

	 if ( (MouseX >= WHO_FIRST_YOUX) &&
	      (MouseX < (WHO_FIRST_YOUX + WHO_FIRST_YMSIZEX)) ) {
	    ReturnValue = TRUE;
	    break;
	 }
	 if ( (MouseX >= WHO_FIRST_MEX) &&
	      (MouseX < (WHO_FIRST_MEX + WHO_FIRST_YMSIZEX)) ) {
	    ReturnValue = FALSE;
	    break;
	 }

      }

   }

   // Restore screen
   XPut_Tile	    (  WHO_FIRSTX, WHO_FIRSTY, POPUP_XSIZE, POPUP_YSIZE,
		       TGA + POPUP_RELOAD_SITE				 );

   return  ReturnValue;

};

// -- Member :  You sure? -----------------------------------------
BOOLEAN  GraphicsControl::YouSure( void ) {

   // return true if sure

   BOOLEAN  ReturnValue;

   // Definitions
   #define  YOU_SUREX	 	160
   #define  YOU_SUREY	 	87
   #define  YOU_SURE_YOUX      (YOU_SUREX + 12)
   #define  YOU_SURE_YOUY      (YOU_SUREY + 16)
   #define  YOU_SURE_MEX       (YOU_SUREX + 62)
   #define  YOU_SURE_YMSIZEX   30
   #define  YOU_SURE_YMSIZEY   11


   // Return TRUE if it is the player (YOU), FALSE is (ME).

   // Put the popup on the screen
   XDownload_TileP  (  YOU_SUREX, YOU_SUREY, POPUP_XSIZE, POPUP_YSIZE,
		       TGA + POPUP_RELOAD_SITE				 );
   XPut_Tile	    (  YOU_SUREX, YOU_SUREY, POPUP_XSIZE, POPUP_YSIZE,
		       TGA + YOU_SURE_SITE				 );

   // Loop until valid
   for(;;) {

      // Wait for a mouse event
      MouseView();

      // See if valid
      if ( (MouseY >= YOU_SURE_YOUY)&&
	   (MouseY < (YOU_SURE_YOUY + YOU_SURE_YMSIZEY)) ) {

	 if ( (MouseX >= YOU_SURE_YOUX) &&
	      (MouseX < (YOU_SURE_YOUX + YOU_SURE_YMSIZEX)) ) {
	    ReturnValue = TRUE;
	    break;
	 }
	 if ( (MouseX >= YOU_SURE_MEX) &&
	      (MouseX < (YOU_SURE_MEX + YOU_SURE_YMSIZEX)) ) {
	    ReturnValue = FALSE;
	    break;
	 }

      }

   }

   // Restore screen
   XPut_Tile	    (  YOU_SUREX, YOU_SUREY, POPUP_XSIZE, POPUP_YSIZE,
		       TGA + POPUP_RELOAD_SITE				 );

   return  ReturnValue;

};

// -- Member :  The You Must Pass popup ---------------------------------
void  GraphicsControl::YouMustPass( void ) {

   // Definitions unique to this popup
   #define  MUST_PASSX	 	160
   #define  MUST_PASSY	 	87


   // Put the popup on the screen
   XDownload_TileP  (  MUST_PASSX, MUST_PASSY, POPUP_XSIZE, POPUP_YSIZE,
		       TGA + POPUP_RELOAD_SITE				 );
   XPut_Tile	    (  MUST_PASSX, MUST_PASSY, POPUP_XSIZE, POPUP_YSIZE,
		       TGA + MUST_PASS_SITE				 );

   // Wait for a mouse event
   MouseView();

   // Restore screen
   XPut_Tile	    (  MUST_PASSX, MUST_PASSY, POPUP_XSIZE, POPUP_YSIZE,
		       TGA + POPUP_RELOAD_SITE				 );

};


// -- Member :  Show the HERE tile --------------------------------------
void  GraphicsControl::Here( Go  TheGo ) {

   XWait_Retrace();

   XPut_Tile( BOARDX + (SPOT_XYSIZE * TheGo.XIs()),
	      BOARDY + (SPOT_XYSIZE * TheGo.YIs()),
	      SPOT_XYSIZE, SPOT_XYSIZE,
	      TGA + HERE_SITE				 );


};


// -- Member :  Show the HERE tile --------------------------------------
void  GraphicsControl::Score( int     TheScore ) {

   char		Buffer[10];

   XWait_Retrace();

   XPut_Tile(  SCORE_X, SCORE_Y, SCORE_XSIZE, SCORE_YSIZE,
	       TGA + SCORE_RELOAD_SITE );

   itoa( TheScore, Buffer, 10 );
   XString4_C( SCORE_X, SCORE_Y, SCORE_COLOR, Buffer );

};