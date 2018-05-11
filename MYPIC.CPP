/*--------------------------------------------------------------------------

  REVERSE :

  UNIT    : My picture



---------------------------------------------------------------------------*/


// ------------------------------------------------------------------------
// --- INCLUDES
// ------------------------------------------------------------------------
#include<stdlib.h>
#include<stdio.h>
extern "C" {
   #include "xtile21!.h"
};
#include<string.h>
#include<alloc.h>
#include"reversem.hpp"
#include"mypic.hpp"

// ------------------------------------------------------------------------
// --- DEFINITIONS
// ------------------------------------------------------------------------

// --- Definitions ---------------------------------------------------------

#define  PICTUREX_SIZE	      100
#define  PICTUREY_SIZE	      100
#define  PICTURE_DATA_SIZE    (PICTUREX_SIZE * PICTUREY_SIZE)

// -- Screen location for picture
#define  PICTUREX_LOC		4
#define  PICTUREY_LOC	       31

// -- Text definitions
#define  TEXT_X			8
#define  TEXT_Y		      134
#define  TEXT_X_SIZE           92
#define  TEXT_Y_SIZE           22
#define  TEXT_COLOR	      103
#define  TEXT_BACKGROUND      123

// ------------------------------------------------------------------------
// --- PRIVATE MEMBERS
// ------------------------------------------------------------------------

// -- Members :  -----------------------------------------------------
void  MyPicture::LoadPicture( AVAILABLE_PICTURES Which,
			      unsigned int       FindPic ) {

   unsigned char  *PicLocation;
   long		   PicOffset;

   // Generate a pointer and a file offset.
   PicLocation = PicDump + (FindPic * PICTURE_DATA_SIZE);
   PicOffset   = (long)((long)(Which) * (long)(PICTURE_DATA_SIZE));

   fseek( PicFile, PicOffset, SEEK_SET );
   fread( PicLocation, PICTURE_DATA_SIZE, 1, PicFile );

};


void  MyPicture::DisplayPicture( unsigned int  FindPic ) {

   XWait_Retrace();
   XPut_Tile( PICTUREX_LOC, PICTUREY_LOC, PICTUREX_SIZE, PICTUREY_SIZE,
	      PicDump + (FindPic * PICTURE_DATA_SIZE) );
};


inline void  MyPicture::ClearText( void ) {

   XSet_Box( TEXT_X, TEXT_Y, TEXT_X_SIZE, TEXT_Y_SIZE, TEXT_BACKGROUND );

};


// ------------------------------------------------------------------------
// --- CONSTRUCTOR/DESTRUCTOR
// ------------------------------------------------------------------------

MyPicture::MyPicture( void ) {

   int  Step;

   // Open pic file, and allocate pic dump.  For now, any error is fatal.
   PicFile = fopen( "mypics.cmb", "rb" );
   if ( PicFile == NULL ) {
      puts( ".. FATAL ERROR ..  Unable to open 'mypics.cmb' file.\n\r\n\r");
      exit( 1 );
   }
   PicDump = (unsigned char *) farmalloc( (unsigned long) PICTURE_DATA_SIZE * MAX_LOADED_PICTURES );
   if ( PicDump == NULL ) {
      puts( ".. FATAL ERROR ..  Unable to allocate memory for my pictures.\n\r\n\r");
      exit( 1 );
   }

   // Clear the pictures loaded array.
   for (Step = 0; Step < MAX_LOADED_PICTURES; Step++ ) {
      LoadPics[Step] = NO_PICTURE;
   }

   UseTick = 0;

};


MyPicture::~MyPicture() {

   // Close file and return memory
   fclose(  PicFile );
   farfree( PicDump );

};


// ------------------------------------------------------------------------
// --- PUBLIC MEMBERS
// ------------------------------------------------------------------------
void  MyPicture::Reset( void ) {

   int  Step;

   // Reset the picture system.  Clear the loaded and clear the text window.
   for (Step = 0; Step < MAX_LOADED_PICTURES; Step++ ) {
      LoadPics[Step] = NO_PICTURE;
   }
   UseTick = 0;

   ClearText();

};


void  MyPicture::ShowPicture( AVAILABLE_PICTURES  Which ) {

   unsigned int  FindPic;
   unsigned int  Step;

   UseTick++;

   // If the picture requested is loaded, then show it.
   // If it is not loaded, then first load it.
   // If there is no room to load it, then replace the least recently used.

   for ( FindPic = 0; FindPic < MAX_LOADED_PICTURES; FindPic++ ) {

      if ( LoadPics[FindPic] == Which ) {
	 // It is loaded!!
	 DisplayPicture( FindPic );
	 TickOfLastUse[FindPic] = UseTick;
	 return;
      }
   }

   // Ok, not loaded.  Find a space to load
   for ( FindPic = 0; FindPic < MAX_LOADED_PICTURES; FindPic++ ) {

      if ( LoadPics[FindPic] == NO_PICTURE ) {

	 // It is an empty spot!!
	 LoadPicture( Which, FindPic );
	 LoadPics[FindPic] = Which;
	 DisplayPicture( FindPic );
	 TickOfLastUse[FindPic] = UseTick;
	 return;
      }
   }

   // Ok, boot the least recently used
   FindPic = 0;
   for ( Step = 1; Step < MAX_LOADED_PICTURES; Step++ ) {
      if ( TickOfLastUse[Step] < TickOfLastUse[FindPic] )
	 FindPic = Step;
   }
   LoadPicture( Which, FindPic );
   LoadPics[FindPic] = Which;
   DisplayPicture( FindPic );
   TickOfLastUse[FindPic] = UseTick;

};


void   MyPicture::SayText( char*   Line1,
			   char*   Line2,
			   char*   Line3	) {

   // Display the lines that are not NULL
   ClearText();

   if( Line1 != NULL )
      XString4_C( TEXT_X, TEXT_Y,    TEXT_COLOR, Line1 );
   if( Line2 != NULL )
      XString4_C( TEXT_X, TEXT_Y+6,  TEXT_COLOR, Line2 );
   if( Line3 != NULL )
      XString4_C( TEXT_X, TEXT_Y+12, TEXT_COLOR, Line3 );

};
