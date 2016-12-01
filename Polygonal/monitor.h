/*** 
Author: Vinayak Prabhu 
***/

#ifndef GUARD_Monitor
#define GUARD_Monitor

#include "cell.h"
#include "point.h"
#include "global_defs.h"

#include <vector>
#include <utility>



void update_gprev_curr(std::pair<int, int> & prev_curr);

int pIndex_fun(const int & vindex); //Guaranteed to return in range 0..W  if vindex \geq 0




void fill_cell_arr(const int & gcurr_index, Cell cell_arr[],  const timePoint tpoints[], const timePoint & otherTP1, 
const timePoint & otherTP2, const   std::pair<double, double> & e,  const bool & isRow);


std::pair<bool, bool> monitor(const timePoint &  xval, const timePoint &  yval);
//returns <reachable_horizon, reachable_corner>; once <reachable_horizon== false, further invokations have 
//undefined behavior


bool edgeValid( const Cell & cell, const int & edge);

#endif
