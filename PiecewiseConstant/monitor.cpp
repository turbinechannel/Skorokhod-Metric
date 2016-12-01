/***
Author: Vinayak Prabhu
***/


#include "global_defs.h"
#include "myprint.h"
#include "pdist.h"
#include "monitor.h"
#include <utility>
#include <cassert>
#include <algorithm>  //std::max
#include  <iostream>


using namespace std;

void update_gprev_curr(pair<int, int> & prev_curr)
//toggle
{
  //cout <<"in update_gprev_curr" <<endl;
  if(prev_curr.first == 0){
    prev_curr.first = 1;
    prev_curr.second =0;
  }
  else{
    prev_curr.first = 0;
    prev_curr.second =1;
  }
}


int pIndex_fun(const int & vindex) //Guaranteed to return in range 0..W+1  if vindex \geq 0
//map virtual index to physical index
{
  return vindex % (WINDOW +2);
}


double monitor(const timePoint &  xval, const timePoint &  yval)
//Require gcurr_index \geq 0
//Increments gcurr_index by 1
//Keep window+1 row and column elements of DP store
//This entails keeping window+2 original data points
//D(i,j): i for xval; j for yval
//matrix notation
{
  double match_val = 0;
  //need to treat the first element as special
   const int curr_rowcol  =gprev_curr.second;
   const int prev_rowcol  =gprev_curr.first;

  if (gcurr_index == 0){
     gdata_x[0] = xval;
     gdata_y[0] = yval;
     gcurr_index++;
     match_val = pdist(xval, yval);
     grows[curr_rowcol][0] = match_val;
     gcols[curr_rowcol][0] = match_val;
     return match_val;
  }

  //now we have  gcurr_index \geq 1
   const int pindx = pIndex_fun(gcurr_index); //Guaranteed to return in range 0..W+1  if gcurr_index \geq 0
   //this will wrap-map the current point index to an index in  range 0..W+1


   gdata_x[pindx] = xval; 
   gdata_y[pindx] = yval;

  const int lowest_indx = max(0, gcurr_index -  WINDOW);
  //accessed datapoints from lowest_indx.. gcurr_index. Number of accessed points = gcurr_index - lowest_indx+1
  //Total number = WINDOW+1 in steady state.
  //number of array blocks filled (minus triangular block) = gcurr_index - lowest_indx.
  /*     4 3 2 1 0
       4 3 2 1 0 1
                    1 2
                    2 3
                    3 4
                    4
		    The 4 index corresponds to the datapoint index lowest_indx 
  */
  //filled blocks 0... A where A = gcurr_index -lowest_indx 
    //The last array cell has index WINDOW (in steady state)
  //need to update from A..0
  int carr_indx =gcurr_index -lowest_indx; 
  double left_dp = 0;
  double bottom_dp=0;
  double diag_dp =0;

  
  //current virtual row index in expanded DP store: gcurr_index
  //column virtual index: lowest_indx.. gcurr_index
  //column  virtual index lowest_indx corressponds to carr_indx
  //similarly for row and columns swapped
  assert( (1<=carr_indx) and (carr_indx <= WINDOW));
  
  if (gcurr_index < WINDOW){ //special transient case for last cells -- no diag element
    //for rows:
    match_val = pdist(xval, gdata_y[0]);
    bottom_dp = grows[prev_rowcol][carr_indx-1]; //carr_indx-1 \geq 0
    grows[curr_rowcol][carr_indx] = max(match_val, bottom_dp);
    //for cols:
    match_val = pdist(gdata_x[0], yval);
    left_dp = gcols[prev_rowcol][carr_indx-1];
    gcols[curr_rowcol][carr_indx] = max(match_val, left_dp);
  }
  else{//triangular last cell update
    //carr_indx = WINDOW
    //for rows:
    match_val = pdist(xval, gdata_y[ pIndex_fun(lowest_indx)]);
    bottom_dp = grows[prev_rowcol][WINDOW-1]; //carr_indx-1 \geq 0
    diag_dp = grows[prev_rowcol][WINDOW];
    grows[curr_rowcol][WINDOW] = max(match_val,  min(bottom_dp, diag_dp));
    //for cols:
    match_val = pdist(gdata_x[pIndex_fun(lowest_indx)], yval);
    left_dp = gcols[prev_rowcol][WINDOW-1]; //carr_indx-1 \geq 0
    diag_dp = gcols[prev_rowcol][WINDOW];
    gcols[curr_rowcol][WINDOW] = max(match_val,  min(left_dp, diag_dp));
  }

  //Now last cell is done
  //Do rest of cells
  carr_indx--;
  //carr_indx \geq 0
  assert( (0<=carr_indx) and (carr_indx < WINDOW));

  int curr_data_indx= lowest_indx;
  for(; carr_indx > 0; carr_indx--, curr_data_indx++){
    //for rows:
    match_val = pdist(gdata_y[ pIndex_fun(curr_data_indx)], xval);
    bottom_dp = grows[prev_rowcol][carr_indx-1]; //carr_indx-1 \geq 0
    diag_dp = grows[prev_rowcol][carr_indx];
    left_dp = grows[curr_rowcol][carr_indx+1]; //carr_indx+1 \leq WINDOW 
    grows[curr_rowcol][carr_indx] = max(match_val, min(min(bottom_dp, left_dp), diag_dp));
    //for cols:
    match_val = pdist(yval, gdata_x[pIndex_fun(curr_data_indx)]);
    left_dp = gcols[prev_rowcol][carr_indx-1]; //carr_indx-1 \geq 0
    diag_dp = gcols[prev_rowcol][carr_indx];
    bottom_dp = gcols[curr_rowcol][carr_indx+1]; //carr_indx+1 \leq WINDOW 
    gcols[curr_rowcol][carr_indx] = max(match_val, min(min(bottom_dp, left_dp), diag_dp));
  }
  
  //Process the 0-th element of the rows and colums. Requires special processing
  match_val = pdist(yval, xval);
  left_dp = grows[curr_rowcol][1];
  bottom_dp = gcols[curr_rowcol][1]; //carr_indx+1 \leq WINDOW 
  diag_dp = grows[prev_rowcol][0];
  grows[curr_rowcol][0] =  max(match_val, min(min(bottom_dp, left_dp), diag_dp));
  gcols[curr_rowcol][0] = grows[curr_rowcol][0];

  assert(gcurr_index > 0);
  //double time_penalty = abs(xval.time - gdata_x[pIndex_fun(gcurr_index-1)].time);


  gcurr_index++;  //latest element has been processed
  update_gprev_curr(gprev_curr);  //toggle
  return  grows[curr_rowcol][0];

}

