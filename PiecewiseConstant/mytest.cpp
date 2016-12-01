/***
Author: Vinayak Prabhu
***/

#include "myprint.h"
#include "point.h"
#include "pdist.h"
#include  <vector>
#include  <iostream>
#include  <iterator>
#include  <numeric>
#include <fstream>
#include <string>

using namespace std;


void testPDist(const timePoint & tp1, const timePoint & tp2)
{
  cout <<"Test PDist" <<endl;
  cout <<"Point1: (time) " <<tp1.time <<"  (val) ";
  printvec(tp1.point);

  cout <<"Point2: (time) " <<tp2.time <<"  (val) ";
  printvec(tp2.point);

  cout<<"Point distance: " <<pdist(tp1, tp2) <<endl;
}
    
int main(){

  vector<double> v1 = {1,5, 9};
  vector<double> v2 = {2,1, 6};
  timePoint tp1(1.0, v1);
  timePoint tp2(10.0, v2);
  testPDist(tp1,  tp2);

    return 0;
}


  
