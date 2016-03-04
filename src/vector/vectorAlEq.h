

#include "halfsegment.h"
#include <cstdlib>
#include <iomanip>
// event queue contains a vector and is sorted in hseg order

#ifndef VECTORALEQ_H
#define VECTORALEQ_H

class eventQueue
{
    private:
        vector<halfsegment> eq;
    
    public:
        void insert( const halfsegment & h1 ){
            if( eq.empty() || eq[eq.size()-1] < h1 ) {
                eq.push_back( h1 );
                return;
            }
            else {
                for( vector<halfsegment>::iterator it = eq.begin(); ;it++ ){
                    if(  h1 < *it  ){
                        eq.insert( it, h1 );
                        break;
                    }
                }
                
            }
        }

        bool peek( halfsegment & h1 ){
            if( eq.empty() ) {
                return false;
            }
            h1 = eq[0];
            return true;
        }
        bool pop( ){
            if( eq.empty() ) {
                return false;
            }
            eq.erase( eq.begin() );
            return true;
        }
        int size(){
            return eq.size();
        }

        void print(){
            cerr << "eq:-----"<<endl;
            for( int i = 0; i < eq.size(); i++ )
                cerr << eq[i] << endl;
            cerr << "^^^^^^"<< endl;
        }


};


class activeListVec
{
    // need to insert, remove, find above, find below
    private:
        vector<halfsegment> al;
    public:
        double xVal;
        // always assume h1 is the new halfsegment (being entered into the list)
        // returns true if h1 < h2, false otherwise
        bool alHsegLT( const halfsegment &h1, const halfsegment &h2 )
        {
            // if equal, indicate
            if( h1 == h2 )
                return false;
            // if colinear, then 'first' is greater (due to hseg ordering) 
            // the only time we will have overlapping colinear is when inserting left hsegs
            // so this shouldn't ever come up when removing based on right segs
            if( h1.colinear( h2 ) )
                return false;

            double h1Y = h1.getYvalAtX( xVal );
            double h2Y = h2.getYvalAtX( xVal );
            // at this point, we simply construct hsegs using the sweep line intersect and xval as the dominating point
            // cerr << "xval, h1, h2 :" << xVal << ", " << h1 << ", " << h2 << endl;
            halfsegment newH1;
            halfsegment newH2;
            newH1.dx = newH2.dx = xVal;
            newH1.dy = h1Y;
            newH2.dy = h2Y;
            if( newH1.dx == h1.sx && newH1.dy == h1.sy ) {
                newH1.sx = h1.dx;
                newH1.sy = h1.dy;
            }
            else {
                newH1.sx = h1.sx;
                newH1.sy = h1.sy;
            }
            if( newH2.dx == h2.sx && newH2.dy == h2.sy ) {
                newH2.sx = h2.sx;
                newH2.sy = h2.dy;
            }
            else {
                newH2.sx = h2.sx;
                newH2.sy = h2.sy;
            }
            //cerr << "newh1, newh2: " << newH1 << ", " << newH2 << (newH1 < newH2)<<endl;
            // if Y vals are different
            if( h1Y < h2Y ) return true;
            else if( h1Y > h2Y ) return false;
            // otherwise, the dom point is the same
            // now we just use the halfsgment < operator 
            // for the active list, we want the seg that is above the other one.  
            // special case: when they are both right hsegs, they always share a dom point.  reverse <
            if( !newH1.isLeft() && !newH2.isLeft( ) ) {
                if(  newH1 < newH2 ) return false;
                return true;
            }
            else if( newH1.isLeft() && newH2.isLeft( ) ) { 
                if( newH1 < newH2 ) return true;
                return false;
            }
            else {

                // we have a left one and a right one.  the right hseg is less
                if( !newH1.isLeft() ) return true;
                return false;
                // we should never get 1 left and 1 right hseg

                cerr<< "AL comp.  got a left and right" << endl;
                cerr<< "xval: " << std::setprecision(100)<< xVal << endl;
                cerr << std::setprecision(100) << h1 << endl << h2 << endl << newH1 << endl << newH2 << endl;
                exit( -1 );
            }
            return false;
        }
        
        inline bool alHsegEQ( const halfsegment &h1, const halfsegment &h2 )
        {
            return (h1 == h2);
        }
        // returns a pointer to the halfseg in the list
        void insert( const halfsegment & h1, bool & duplicate, halfsegment & theDup, int & segIndex )
        {
            duplicate = false;
            if( al.empty() || !alHsegLT( h1, al[al.size()-1])  ) {
                al.push_back( h1 );
                theDup = h1;
                segIndex = al.size()-1;
                return;
            } 
            else {
                int i = 0;
                for( vector<halfsegment>::iterator  it = al.begin(); ; it++ ){
                    if( *it == h1 ){
                        duplicate = true;
                        theDup = *it;
                        segIndex = i;
                        return;
                    } 
                    else if( alHsegLT( h1, *it ) ){
                        al.insert( it, h1 );
                        theDup = h1;
                        segIndex = i;
                        return;
                    }
                    i++;
                }
            }
        }
        
        bool exists( const halfsegment& h1, halfsegment & theCopy, int & index ){
            index = find (h1 );
            if( index != -1 ){
                theCopy = al[index];
                return true;
            }
            return false;
        }
        int find( const halfsegment& h1 ){
            for( int i= 0; i < al.size(); i++ ){ 
                if( h1 == al[i] )
                    return i;
            }
            return -1;
        }
        /*halfsegment* findPtr( const halfsegment &h1 ){
            int i = 0;
            for(vector<halfsegment>::iterator  it = al.begin(); it != al.end(); it++ )
            {
                if( *it == h1 )
                {
                    return &(al[i]);
                }
                i++;
            }
            return NULL;
        }*/
        void replace( const halfsegment &h1, const halfsegment & newH1 ){
            int index = find( h1 );
            if( index <0 || index >= al.size() ){
                cerr << "replace: did not find halfsegment: " << h1 << endl;
                exit( -1 );
            }   
            al[index] = newH1;
        }
        void replace( const halfsegment &h1, const halfsegment & newH1, const int index ){
            if( index <0 || index >= al.size() ){
                cerr << "replace: did not find halfsegment: " << h1 << endl;
                exit( -1 );
            }
            if( al[index] == h1 ){
                al[index] = newH1;
                return;
            }
            else {
                cerr << "trying to replace a seg by index that does not match" <<endl;
                exit( -1 );
            }
        }

      /* halfsegment* getAbove( int index ){
            if( index < 0 !! index > al.size() ){
                cerr << "invalid size getAbove AL"<<endl;
                exit(-1);
            }
            if( index == al.size() ){
                return NULL;
            }
            theAboveHseg = al[index+1];
            return &(al[index+1]);
        }*/
       bool getAbove( const halfsegment& h1, halfsegment &theAbove, const int index ){
            // cerr << "foundabove index = " << index << endl;
            if( index < 0 || index > al.size() ){
                cerr << "invalid size getAbove AL"<<endl;
                exit(-1);
            }
            if( index >= al.size()-1 ){
                return false;
            }
            theAbove = al[index+1];
            return true;
        }
       bool getAbove( const halfsegment& h1, halfsegment &theAbove ){
            int index = this->find( h1 );
            // cerr << "foundabove index = " << index << endl;
            if( index < 0 || index > al.size() ){
                cerr << "invalid size getAbove AL"<<endl;
                exit(-1);
            }
            if( index >= al.size()-1 ){
                return false;
            }
            theAbove = al[index+1];
            return true;
        }
        /*halfsegment* getBelow( int index, halfsegment & theBelowHseg ){
            if( index < 0 !! index >= al.size() ){
                cerr << "invalid size getBelow AL"<<endl;
                exit(-1);
            }
            if( index == 0 ){
                return false;
            }
            theBelowHseg = al[index-1];
            return true;
        }*/
        bool getBelow(const halfsegment& h1, halfsegment &theBelow, const int index ){
            if( index < 0 || index >= al.size() ){
                cerr << "invalid size getBelow AL"<<endl;
                exit(-1);
            }
            if( index <= 0 ){
                return false;
            }
            theBelow = al[index-1];
            return true; 
        }
        bool getBelow(const halfsegment& h1, halfsegment &theBelow ){
            int index = this->find( h1 );
            if( index < 0 || index >= al.size() ){
                cerr << "invalid size getBelow AL"<<endl;
                exit(-1);
            }
            if( index <= 0 ){
                return false;
            }
            theBelow = al[index-1];
            return true; 
        }
                
        void erase( const halfsegment & h1, const int index )
        {
            // cerr << "erase1"<<endl;
            if( index < 0 || index >= al.size() ) 
                return; 
            
            vector<halfsegment>::iterator  it = al.begin() + index;
            if( *it == h1 ){
                al.erase( it );
                // cerr << "erase2"<<endl;
                return;
            } 
            else {
                cerr << "trying to erase a seg by index that does not match" <<endl;
                exit( -1 );
            }
	}
        void erase( const halfsegment & h1 )
        {
            // cerr << "erase1"<<endl;
            for(vector<halfsegment>::iterator  it = al.begin();it != al.end() ; it++ ){
                if( *it == h1 ){
                    al.erase( it );
                    // cerr << "erase2"<<endl;
                    return;
                } 
	    }
	}
        void print(){
            cerr << "al:-----"<<endl;
            for( int i = 0; i < al.size(); i++ )
                cerr << al[i] << endl;
            cerr << "^^^^^^" << endl;
        }
};
#endif

