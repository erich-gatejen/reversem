/*--------------------------------------------------------------------------

  REVERSEM :

  UNIT     : MY PICTURE DISPLAY CLASS

  COPYRIGHT (C) 1994 Erich P. Gatejen  ALL RIGHTS RESERVED
  This is Freeware.  You may distribute it as you wish.  However, you may not
  distribute a modified version without my consent.

  Feel free to cut and paste any algorthm.

  NOTE: XTILE is (C) 1992, 1994 by myself.  It is NOT freeware.  You may use
	it for personal projects, but otherwise, it is not to be distributed
	outside this REVERSEM package.  (Contact me if you wish to do
	otherwise)

---------------------------------------------------------------------------*/

// --- Definitions ---------------------------------------------------------
#define  MAX_LOADED_PICTURES	6  // Each picture is 10000 bytes.

// --- Define available pictures
// - Note:  The order of the enumeration directly relates to the order
// -        of pictures in the combine file
enum   AVAILABLE_PICTURES {
	NO_PICTURE = -1,
	PIC_HALFSMILE = 0,
	PIC_WHAT,
	PIC_IDEA,
	PIC_CALM,
	PIC_NOEXP,
	PIC_COVERED,
	PIC_WHATUPSET,
	PIC_FROWN,
	PIC_CURIOUSFROWN,
	PIC_ARMSCROSS,
	PIC_THINKING,
	PIC_LOOKRIGHT,
	PIC_VICTORY,
	PIC_LOOKRIGHTC,
	PIC_BYE

};


// --- My picture class -----------------------------------------------
class  MyPicture {

    // Private data
    unsigned char	*PicDump;
    AVAILABLE_PICTURES   LoadPics[MAX_LOADED_PICTURES];
    FILE		*PicFile;

    unsigned int	 UseTick;
    unsigned int	 TickOfLastUse[MAX_LOADED_PICTURES];

    // Private functions
    void  LoadPicture( AVAILABLE_PICTURES Which, unsigned int FindPic );
    void  DisplayPicture( unsigned int  FindPic );
    inline void  MyPicture::ClearText( void );

 public:

    // Constructor and destructor
    MyPicture( void );
   ~MyPicture();

    void   Reset( void );

    void   ShowPicture( AVAILABLE_PICTURES  Which );
    void   SayText( char*   Line1,
		    char*   Line2,
		    char*   Line3	);

};


