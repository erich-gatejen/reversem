/*--------------------------------------------------------------------------

  REVERSEM :

  UNIT     : COMMON DEFINITIONS

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
// --- DEFINITIONS
// ------------------------------------------------------------------------

// --- General named numbers (constants) ----------------------------------
#define  BOARDXSIZE 	8
#define  BOARDYSIZE 	8
#define  TOPX		0
#define  BOTTOMX	7
#define  TOPY 		0
#define  BOTTOMY	7

#define  MAX_CHILDREN   12	// A SWAG

// --- Type casts ---------------------------------------------------------
typedef  int  heuristic;

// --- Enumerate BOOLEAN --------------------------------------------------
enum BOOLEAN { FALSE = 0, TRUE = 1 };

// --- Enumerations for MIN/MAX -------------------------------------------
enum STAGE { TREE_TOP = 0, MIN_PLY, MAX_PLY };

// --- Enumerations for how smart this computer will be -------------------
// --- and how it may otherwise behave
enum BRAIN { SIMPLETON = 0,        // Idiot
	     DULLARD,
	     AVERAGE,
	     SWIFT,
	     GENIUS,
	     EXPERIMENTAL

};


