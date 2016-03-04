#include <omp.h>
#include <vector>
#include <algorithm>
#include <limits> 
#include <iostream>
#include <cstdlib>
extern "C" {
#include "avl.h"
}

#ifndef PARSESWEEP_H
#define PARSESWEEP_H



/**
 * \file
 *
 * Header file containing the necessary function prototypes for the 
 * parallel plane sweep and the serial plane sweep algorithms.
 *
 * Also contains some helper functions.
 */

using namespace std;

//#define DEBUG_PRINT


/**
 * Left hand turn test
 *
 * returns true if when you start at p1, and walk towards p2, you have to make a left turn
 * at p2 in order to continue walking to p3.
 *
 * Takes 3 points.  The parameters are the x and y values for point p1, the x and y values for point p2, 
 * and the x and y values for point p3.
 */
inline bool leftHandturn( double p1x, double p1y, double p2x, double p2y, double p3x, double p3y )
{
	// returns true if when you start at p1, and walk towards p2, you have to make a left turn
	// at p2 in order to continue walking to p3
	return( ( ((p3y - p1y) * (p2x - p1x)) - ((p2y - p1y) * (p3x - p1x)) ) > 0 );
}


/** 
 * \class halfsegment
 *
 *  holds a halfsegment.  
 *
 *  Contains the x and y values for the dominating and submissive points.
 *
 *  Contains integer labels:  la = label above and lb = label below
 *
 *  Contains the ID of the strip that this halfsegment is assigned to.
 *
 *  Contains the region to which the halfsegment belongs (there are two input regions)
 *
 *  Contains the overlap lables.  Overlap labels indicate if the interior of the 
 *  opposing region lies above and/or below this halfsegment.
 */
struct halfsegment {
	// dominating and submissive points
	double dx, dy, sx, sy; /// dominating and submissive points
	int la, lb; ///label above, label below
	int stripID; /// strip id
	int regionID; /// the region this seg belongs to
	int ola, olb; /// overlap labels
	
        /**
         * Default constructor
         */
halfsegment():dx( 0 ), dy(0), sx(0), sy(0), la(-1), lb(-1),
		stripID(-1), regionID( -1 ), ola( -1 ), olb( -1 )
	{ }

    /**
     * Copy constructor
     */
halfsegment( const halfsegment &rhs ): dx(rhs.dx), dy(rhs.dy),
		sx(rhs.sx), sy(rhs.sy), 
		la(rhs.la), lb(rhs.lb), 
		stripID( rhs.stripID), regionID( rhs.regionID ),
		ola( rhs.ola ), olb( rhs.olb )
			{ }
	/**
         *  Returns true if this is a left halfsegment, right otherwise.
         */
        bool isLeft( ) const {
		return( dx < sx || (dx == sx && dy < sy ) );
	}

        /**
         * Compute the brother of this halfsegment. (switch the dominating and submissive points).
         */
	halfsegment getBrother( ) const {
		halfsegment tmp( *this );
		tmp.dx = sx;
		tmp.dy = sy;
		tmp.sx = dx;
		tmp.sy = dy;
		return tmp;
	}

        /**
         *  Overloaded equivalence test.
         *
         *  Computes equivalence based ONLY on the end points of the halfsegments.
         *
         *  Segments are equal if their dominating and submissive points match.
         */
	bool operator==( const halfsegment &rhs ) const {
		return dx == rhs.dx && dy == rhs.dy && sx == rhs.sx && sy == rhs.sy;
	}

        /**
         *  Overloaded less than operator.
         *
         *  Computes based ONLY on the end points of the halfsegment.
         */
	bool operator<( const halfsegment &rhs ) const {
		if( dx < rhs.dx || (dx == rhs.dx && dy < rhs.dy ) ) return true;
		else if( dx == rhs.dx && dy == rhs.dy ) {
			// dom points the same
			if( isLeft() != rhs.isLeft( ) ) return !isLeft();
			// both left or right hsegs
			else if( this->colinear( rhs ) && (sx < rhs.sx || (sx == rhs.sx && sy < rhs.sy)) ) return true;
			else if( leftHandturn( dx, dy, sx, sy, rhs.sx, rhs.sy ) ) return true;
		}
		return false;
	}
	
        /**
         *  Test if two halfsegments are colinear.
         *
         *  Tests for exact colnearity.  SENSITIVE TO ROUNDING ERRORS.
         */
	inline bool colinear( const halfsegment &rhs) const {
		return 0 ==	( ((rhs.dy - dy) * (sx - dx)) - ((sy - dy) * (rhs.dx - dx)) ) &&
			0 == ( ((rhs.sy - dy) * (sx - dx)) - ((sy - dy) * (rhs.sx - dx)) ) ;
	}
	
        /**
         *  Compute the y value on a linesegment (halfsegment in this case) at a given x value.
         */
	double getYvalAtX( const double x ) const 
	{
		if( x == dx )	return dy;
		else if( x == sx ) return sy;
		return( ( (sy*x - sy*dx - dy*x + dy*dx) / float((sx-dx))) + dy );
	}

        /**
         *  Overloaded ostream operator.
         */
	friend ostream & operator<<( ostream & out, const halfsegment & rhs ) {
		cerr <<"[(" << rhs.dx << "," << rhs.dy << ")(" << rhs.sx << "," << rhs.sy << ") " << rhs.la << ", " <<  rhs.lb << ", " << rhs.regionID << " <" << rhs.ola <<","<<rhs.olb<<">"<< "]";
		return out;
	}

};


/**
 *  Compute the overlay of two regions in parallel.  This is a wrapper function that divides a pair
 *  of input regions into strips, assigns halfsegments to the appropriate strips, then calls a plane
 *  sweep algorithm on each strip.  
 *
 *  \param r1 [in/out] input region 1
 *  \param r2 [in/out] input region 2
 *  \param numSplits how many strips should be created over the input. If no value is given, the number of strips defaults to the number of processor cores.
 * \param numWorkerThreads The number of worker threads for openMP to use.  If no value is given, openMP's default value is used.
 */
void parallelOverlay( vector<halfsegment> &r1, vector<halfsegment> &r2, vector<halfsegment> &result, 
        int numSplits=-1,  int numWorkerThreads = -1);

/**
 *  Compute the overlay of halfsegments given in two input vectors.  Uses the 
 *  plane sweep algorithm.  This is the serial plane sweep algorithm, it is called
 *  by parallelOverlay().  parallelOverlay() sets up the strips, then calls this over each strip.  The implementation of this function can be switched with any plane sweep style algorithm.
 *
 *  \param r1 a vector of halfsegments
 *  \param r2 a vector of halfsegments
 *  \param r1Size the length of the r1 vector
 *  \param r2Size the length of the r2 vector
 *  \param result [in/out] the result of overlaying r1 and r2
 */
void overlayPlaneSweep( const halfsegment r1[], int r1Size, 
        const halfsegment r2[], int r2Size, 
        vector<halfsegment>& result );
#endif


