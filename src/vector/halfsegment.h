
#include <iostream>
#ifndef HALFSEGMENT_H
#define HALFSEGMENT_H

//#define DEBUG_PRINT

using namespace std;

inline bool leftHandturn( double p1x, double p1y, double p2x, double p2y, double p3x, double p3y )
{
	// returns true if when you start at p1, and walk towards p2, you have to make a left turn
	// at p2 in order to continue walking to p3
	return( ( ((p3y - p1y) * (p2x - p1x)) - ((p2y - p1y) * (p3x - p1x)) ) > 0 );
}


struct halfsegment {
	// dominating and submissive points
	double dx, dy, sx, sy; 
	int la, lb; //label above, label below
	int stripID; // strip id
	int regionID; // the region this seg belongs to
	int ola, olb; // overlap labels
	
halfsegment():dx( 0 ), dy(0), sx(0), sy(0), la(-1), lb(-1),
		stripID(-1), regionID( -1 ), ola( -1 ), olb( -1 )
	{ }
halfsegment( const halfsegment &rhs ): dx(rhs.dx), dy(rhs.dy),
		sx(rhs.sx), sy(rhs.sy), 
		la(rhs.la), lb(rhs.lb), 
		stripID( rhs.stripID), regionID( rhs.regionID ),
		ola( rhs.ola ), olb( rhs.olb )
			{ }
	bool isLeft( ) const {
		return( dx < sx || (dx == sx && dy < sy ) );
	}
	halfsegment getBrother( ) const {
		halfsegment tmp( *this );
		tmp.dx = sx;
		tmp.dy = sy;
		tmp.sx = dx;
		tmp.sy = dy;
		return tmp;
	}
	bool operator==( const halfsegment &rhs ) const {
		return dx == rhs.dx && dy == rhs.dy && sx == rhs.sx && sy == rhs.sy;
	}
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
	
	inline bool colinear( const halfsegment &rhs) const {
		return 0 ==	( ((rhs.dy - dy) * (sx - dx)) - ((sy - dy) * (rhs.dx - dx)) ) &&
			0 == ( ((rhs.sy - dy) * (sx - dx)) - ((sy - dy) * (rhs.sx - dx)) ) ;
	}
	
	double getYvalAtX( const double x ) const 
	{
		if( x == dx )	return dy;
		else if( x == sx ) return sy;
		return( ( (sy*x - sy*dx - dy*x + dy*dx) / float((sx-dx))) + dy );
	}

	friend ostream & operator<<( ostream & out, const halfsegment & rhs ) {
		cerr <<"[(" << rhs.dx << "," << rhs.dy << ")(" << rhs.sx << "," << rhs.sy << ") " << rhs.la << ", " <<  rhs.lb << ", " << rhs.regionID << " <" << rhs.ola <<","<<rhs.olb<<">"<< "]";
		return out;
	}

};
#endif


