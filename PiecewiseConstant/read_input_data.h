/*** 
Author: Vinayak Prabhu 
***/


#ifndef GUARD_Read_input_data
#define GUARD_Read_input_data



#include "point.h"
#include <fstream>
#include <vector>

//bool read_global_params(int & window, double & delta,
//			std::vector<double> & scale_vector,  std::ifstream & in);


bool read_timePoint(timePoint & tp, std::ifstream & in);


void end_message(const int & number_points, const double & dist_val);


#endif
