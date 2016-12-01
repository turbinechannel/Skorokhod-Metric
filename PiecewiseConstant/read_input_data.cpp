/*** 
Author: Vinayak Prabhu 
***/


#include "point.h"
#include "myprint.h"
#include "global_defs.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <assert.h>
#include <algorithm>
#include<functional>
#include<numeric>




   






bool read_timePoint(timePoint & tp, std::ifstream & in)
{
  assert(SCALE_VECTOR.size() >1);
  if (in)
    {
      std::string line;
      std::vector<double>::const_iterator iter =SCALE_VECTOR.begin();

      tp.point.clear();
      if (std::getline(in, line)){
	std::istringstream split(line);
	double val;
	split >> val;
	tp.time = val ;
	if (split >> val){
	  tp.point.push_back(val);
	  while (split >> val) tp.point.push_back(val);

	  tp.time = tp.time* (*iter);
	  iter++;
	  transform(tp.point.begin(), tp.point.end(),  iter, tp.point.begin(), std::multiplies<double>());
	  return true; //success in reading  time & point
	}
	else return false;
      }
      else return false; //failure
    }
  else return false; //cannot open file
}



void end_message(const int & number_points, const double &  dist_val)
{
  std::cout <<number_points<<" datapoint pairs processed"<<std::endl;
  std::cout <<"WINDOW: " <<WINDOW <<"  Scale_vector: " ;
  printvec(SCALE_VECTOR);

    
  std::cout <<std::endl;
  std::cout << "Skorokhod distance between Trace x[0.." <<(number_points-1)<<"]  and";
  std::cout << "Trace y[0.." <<(number_points -1)<<"]  is " <<dist_val  <<std::endl;

}
  

