/*--------------------------------------------------------------------------

  REVERSEM :

  UNIT     : HEAP CLASS

  COPYRIGHT (C) 1994 Erich P. Gatejen  ALL RIGHTS RESERVED
  This is Freeware.  You may distribute it as you wish.  However, you may not
  distribute a modified version without my consent.

  Feel free to cut and paste any algorthm.

  NOTE: XTILE is (C) 1992, 1994 by myself.  It is NOT freeware.  You may use
	it for personal projects, but otherwise, it is not to be distributed
	outside this REVERSEM package.  (Contact me if you wish to do
	otherwise)

---------------------------------------------------------------------------*/

#define  MAX_ZONES	5
#define  MAX_ZONESIZE   65530

class  Heap {

   unsigned int		ObjectSize;
   char*		Zones[MAX_ZONES];	// char must = 1 byte

   char*		NextSlot;

   unsigned int		SlotsLeft;

   unsigned int		CurrentZone;
   unsigned int		ZoneSlots;

 public:

   Heap( unsigned int  Size );
   ~Heap( void );

   void*	Allocate( void );
   void		Reset( void );

};