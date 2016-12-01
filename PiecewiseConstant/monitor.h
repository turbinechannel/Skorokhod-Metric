/*** 
Author: Vinayak Prabhu 
***/

#ifndef GUARD_Monitor
#define GUARD_Monitor


#include "point.h"
#include "pdist.h"
#include "global_defs.h"

#include <vector>
#include <utility>



void update_gprev_curr(std::pair<int, int> & prev_curr);
//The monitor function toggles the 2 grow arrays, and similarly for gcol

int pIndex_fun(const int & vindex); //Guaranteed to return in range 0..W  if vindex \geq 0


double monitor(const timePoint &  xval, const timePoint &  yval);




#endif
