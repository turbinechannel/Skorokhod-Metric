/*** 
Author: Vinayak Prabhu 
***/


#ifndef GUARD_Global_Defs
#define GUARD_Global_Defs

#include "point.h"
#include <utility>
#include <string>


const std::string INPUT_FILE_X("skorokhod_input_x");
const std::string INPUT_FILE_Y("skorokhod_input_y");

const std::string PARAM_FILE("skorokhod_parameters");


//const int WINDOW =3 ; //i can be matched to i-W, ...i, ...i+W
//Required: WINDOW \geq 1

//const double DELTA = 0.99 ;  //Required: DELTA \geq 0

//const std::vector<double> SCALE_VECTOR = {2, 2, 2};

extern const int WINDOW;
extern const std::vector<double> SCALE_VECTOR;


//extern Cell   grows[2][WINDOW+1];
extern double * (grows[2]);
extern double * (gcols[2]);

//extern Cell  gcols[2][WINDOW+1];


//extern timePoint gdata_x[WINDOW+1];
//extern timePoint gdata_y[WINDOW+1];

extern timePoint *gdata_x;
extern timePoint *gdata_y;


extern int gcurr_index ;

extern std::pair<int, int> gprev_curr; //First and second elements \in {0,1}.  prev_curr.first = prev


void global_init();

void global_restart();

#endif
