#include "halfsegment.h"
#include <vector>

#ifndef PARSESWEEP_H
#define PARSESWEEP_H

using namespace std;

/**
 * \file
 *
 * Header file containing the necessary function prototypes for the 
 * parallel plane sweep and the serial plane sweep algorithms.
 *
 * Also contains some helper functions.
 */


//#define DEBUG_PRINT




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


