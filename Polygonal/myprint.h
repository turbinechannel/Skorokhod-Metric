/*** 
Author: Vinayak Prabhu 
***/

#ifndef GUARD_Printvec
#define GUARD_Printvec

#include  <vector>
#include <utility>
#include "free-space-boundary.h"
#include "cell.h"


void printvec(const std::vector<double> & myvec);

void printvec_nonend(const std::vector<double> & myvec);

void indexPrint(const int & i);

void printpair(const std::pair<double, double> & p);

void printconic(const ConicCoeff & conic);

void printquadratic(const QuadraticCoeff & q);

void printCell(const Cell & cell);

void   printCellArrays();


#endif
