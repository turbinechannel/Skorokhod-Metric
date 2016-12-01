/*** 
Author: Vinayak Prabhu 
***/


#include "point.h"
#include "global_defs.h"
#include <utility>
#include <vector>
#include <string>
#include<algorithm>
#include <fstream>


//const int WINDOW = 3; //i can be matched to i-W, ...i, ...i+W
//Required: WINDOW \geq 1

//const double DELTA = 0.5;  //Required: DELTA \geq 0


double * (grows[2]); //DP store
double * (gcols[2]);
//double  grows[2][WINDOW +1];
//double  gcols[2][WINDOW +1];


//extern const std::vector<double> SCALE_VECTOR = {1, 1, 1};

//timePoint gdata_x[WINDOW +1];
//timePoint gdata_y[WINDOW +1];
timePoint *gdata_x;
timePoint *gdata_y;

int gcurr_index = 0;
//this count the number of point pairs seen. I assume no overflow

std::pair<int, int> gprev_curr(1,0); //First and second elements \in {0,1}.  prev_curr.first = prev



namespace
{
  int window;
  //  double delta;
  std::vector<double> scale_vector;
  
  
  bool read_global_params(int & window, 
			  std::vector<double> & scale_vector,  const std::string & name)
  {
    std::string dummy_string;
    bool success = true;
    
    std::ifstream in(name);

    scale_vector.clear();
    if (in)
      {
	in >> dummy_string;
	in >>   window; 
	//in >> dummy_string;
	//	in >> delta;
	double val;
	while (in >> dummy_string) {
	  if (in>>val) scale_vector.push_back(val);
	}
      }
    else success = false;
    if (0==scale_vector.size()) success = false;
    return success;
  }

  bool initialised = read_global_params(window, scale_vector,  PARAM_FILE);

}

const int WINDOW = window;
//const double DELTA = delta;
const std::vector<double> SCALE_VECTOR = scale_vector;


void global_init(){
  for (int i = 0; i<2; i++)  
    {
      grows[i] = new double[WINDOW+1];
      std::fill_n(grows[i], WINDOW+1, -1);  //initialize DP store to -1
    }

  for (int i = 0; i<2; i++)  
    {
      gcols[i] = new double[WINDOW+1];
      std::fill_n(gcols[i], WINDOW+1, -1);  //initialize DP store to -1
    }


  gdata_x = new timePoint[WINDOW+2];
  //We don't know the dimension of the value part, so we don't initialise
  //We need to access datapoints corrressponding to the last time-point

  gdata_y = new timePoint[WINDOW+2];


}


void global_restart(){
  gcurr_index = 0;
  gprev_curr.first = 1;
  gprev_curr.second = 0;
}
