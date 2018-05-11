/*--------------------------------------------------------------------------

  REVERSEM :

  UNIT     : HEAP CLASS.

  Since the Borland heap SUCKS, I had to add this.

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
#include<alloc.h>
#include"heap.hpp"

// ------------------------------------------------------------------------
// --- CONSTRUCTOR/DESTRUCTOR
// ------------------------------------------------------------------------

Heap::Heap( unsigned int  Size ) {

   unsigned int		Temp;

   // Clear the zone pointers
   for ( Temp = 0; Temp < MAX_ZONES; Temp++ ) {
      Zones[Temp] = NULL;
   }

   // Figure the zones needed.  And allocate them.
   // We are going to double word align these boys.
   Temp = Size & 0x0003;
   if ( Temp ) {
      Temp =  4 - Temp;
      ObjectSize = Size + Temp;
   } else
      ObjectSize = Size;


   SlotsLeft = MAX_ZONESIZE / ObjectSize;

   CurrentZone = 0;
   ZoneSlots   = SlotsLeft;
   Zones[CurrentZone] = (char *) farmalloc( ZoneSlots * ObjectSize );

   // Set next slot
   NextSlot = Zones[CurrentZone];

};


Heap::~Heap( void ) {

   unsigned int  Temp;

   // Kill of zones
   for( Temp = 0; Temp < MAX_ZONES; Temp++ ) {

      if ( Zones[Temp] != NULL )
	 farfree( Zones[Temp] );
   }

};

// ------------------------------------------------------------------------
// --- PUBLIC MEMBERS
// ------------------------------------------------------------------------

void*	Heap::Allocate( void ) {

   void*  ReturnPointer;

   void*  MemDump;

   // If a slot is readily available, allocate it.  Else, get a zone for it
   if ( SlotsLeft ) {

      ReturnPointer = (void *) NextSlot;
      SlotsLeft--;
      if ( SlotsLeft ) {
	 NextSlot = NextSlot + ObjectSize;
      };

   } else {

      // Can a new zone be had?
      CurrentZone++;
      if( CurrentZone == MAX_ZONES ) {

	 // No!!!  return a null pointer
	 CurrentZone--;
	 ReturnPointer = NULL;

      } else {

	 // Yes!!  Build it if we have to or can
	 if ( Zones[CurrentZone] == NULL ) {
	    if ( (long)(farcoreleft()+4) <= (long)(ZoneSlots * ObjectSize) ) {
	       // Couldn't get the memory, return a nul pointer
	       CurrentZone--;
	       ReturnPointer = NULL;
	       return ReturnPointer;

	    } else {

	       Zones[CurrentZone] =
		  (char *) farmalloc( ZoneSlots * ObjectSize );

	    }

	 }

	 // Allocate it
	 NextSlot   = Zones[CurrentZone] + ObjectSize; // remember, we are
	 SlotsLeft  = ZoneSlots - 1;                   // allocating one

	 ReturnPointer = (void *) Zones[CurrentZone];

      }
   } // end if slots left

   return  ReturnPointer;

};


void  Heap::Reset( void ) {

   // Reset back to first zone.  Don't dump any of the zones.
   CurrentZone = 0;
   NextSlot    = Zones[CurrentZone];
   SlotsLeft   = ZoneSlots;

};


