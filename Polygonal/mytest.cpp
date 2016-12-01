/*** 
Author: Vinayak Prabhu 
***/


#include "free-space-boundary.h"
#include "myprint.h"
#include "cell.h"
#include "point.h"
#include "monitor.h"
#include "global_defs.h"
#include "read_input_data.h"
#include  <vector>
#include  <iostream>
#include  <iterator>
#include  <numeric> 
#include <fstream>
#include <string>


using namespace std;


void testQuadraticMinMax(const double A, const double r1, const double r2)
  //min_max_quadratic
  //A*(x-r1)(x-r2) = A*(x^2 -(r1+r2)x +r1r2)
{
  
  pair<double, double> minmax;
  QuadraticCoeff q;
  
  q.A = A;
  q.B = A*(-1*(r1+r2));
  q.C=A*r1*r2;
  
  minmax = min_max_quadratic(q);
  cout<<"Quadratic minmax multfact: "<<A<<",  r1:= " << r1 <<", r2:= "<<r2 ;
  cout <<"      MinMax: ";
  printpair(minmax);
}

void testConicMinMax(const  ConicCoeff & conic, int side)
{
  pair<double, double> minmax;

  minmax= min_max_cell(conic, side);
  printconic(conic);
  cout<<"      Side: "<<side<<"  ";
  printpair(minmax);
  
}



int main(){
  /*
  vector<double> f_first(3), f_second(3), g_first(3), g_second(3);
  ConicCoeff conic_vals;

  iota(f_first.begin(), f_first.end(), 1);
  iota(f_second.begin(), f_second.end(), 4); 
  //  iota(g_first.begin(), g_first.end(), 7);
  g_first = {7.2, 7.9, 8.8};
  //  iota(g_second.begin(), g_second.end(), 10); 
  g_second = {10, 10.9, 11.7};
  
  cout << "f_first:  " ;
  printvec(f_first);

  cout << "f_second:  " ;
  printvec(f_second);

  cout << "g_first:  " ;
  printvec(g_first);

  cout << "g_second:  " ;
  printvec(g_second);

  ComputeConicCoeff(f_first, f_second, g_first, g_second, 1, conic_vals);

  printconic(conic_vals);
  */
    
  //min_max_quadratic
  //A*(x-r1)(x-r2) = A*(x^2 -(r1+r2)x +r1r2)
  /*Tested
  testQuadraticMinMax(1, 0.4, 0.5);
  testQuadraticMinMax(1, 1.2,1.3);  
  testQuadraticMinMax(1, 0,1);
  testQuadraticMinMax(1, -0.1, 0.3);

  testQuadraticMinMax(-1, 0.4, 0.5);

  testQuadraticMinMax(5, -0.1, 0.3);
  testQuadraticMinMax(-5, -0.1, 0.3); 

  testQuadraticMinMax(1, 0, 0);*/

  /* Tested
  //r1=0.4, r2=0.5
  conic_vals.A = 1;
  conic_vals.B = 0;
  conic_vals.C =0;
  conic_vals.F =-0.9;
  conic_vals.G =0;
  conic_vals.H =0.2;

  testConicMinMax(conic_vals, 0);
  testConicMinMax(conic_vals, 2);
  testConicMinMax(conic_vals, 1);
  */

  /*
  //Cell
  Cell cell, left,bottom;
  cell.setEdge(make_pair(0.1,0.9), 0);

  cell.setEdgeMin(0.2, 1);
  cell.setEdgeMax(0.8, 1);
  cell.setEdgeMin(0.3, 2);
  cell.setEdgeMax(0.7, 2);
  cell.setEdgeMin(0.4, 3);
  cell.setEdgeMax(0.6, 3);


  cout<<"Testing  make cell" <<endl;
  vector<double>  x_first = {2.6, 6.7};
  vector<double>  y_first = {2.6, 6.7};
  vector<double>  x_second  = {3.2, 5.4};
  vector<double>  y_second  = {3.2, 5.4};

  cell.freeSpaceFromVec(x_first, x_second, y_first,  y_second, 0);

  printCell(cell);   */

  /*
  //cout<<"cell: "<<endl;
  //printCell(cell);

  //  left.setEdge(make_pair(0.45, 0.55), 1);
  cout<<"Left cell: "<<endl;
  printCell(left);

  bottom.setEdge(make_pair(0.04, 0.98), 0);//for intersect
  bottom.setEdge(make_pair(0.44, 0.88), 2);
  cout<<"Bot cell: "<<endl;
  printCell(bottom);

  
  // cell.reachUpdate(left, bottom, 1);
  // cout<<"cell (after update): "<<endl;
  // printCell(cell);

  cell.intersectWith(bottom);
  cout<<"cell (after intersect with bot): "<<endl;
  printCell(cell);
  */

  
  cout<< "Testing monitor" <<endl <<endl;

  global_init();

  pair<bool, bool> monitor_result(true, true);
  //  const string input_file_x("skorokhod_input_x");
  //const string input_file_y("skorokhod_input_y");

  timePoint tp_x, tp_y;
  ifstream in_x( INPUT_FILE_X);
  ifstream in_y( INPUT_FILE_Y);
  int lcurr_index=0;

  if (in_x and in_y){
    while(read_timePoint(tp_x, in_x) and read_timePoint(tp_y, in_y)){
      cout<<"sample: " <<lcurr_index <<endl;
      cout<< "   X.time: " <<tp_x.time ;
      cout <<"     X.val:  ";
      printvec(tp_x.point);
      cout<< "   Y.time: " <<tp_y.time ;
      cout <<"     Y.val:  ";
      printvec(tp_y.point);
      //   gdata_x[pIndex_fun(lcurr_index)] = tp_x;
      //gdata_y[pIndex_fun(lcurr_index)]= tp_y;
      lcurr_index++;
      monitor_result = monitor(tp_x, tp_y);
      if (not monitor_result.first) break;
    }
  }
  else {
    cout <<"Error opening files" <<endl;
    return 0;
  }

  
  cout <<(gcurr_index)<<" datapoint pairs processed"<<endl;
  cout <<"Skorokhod bound: "<<DELTA<<endl;
  if (monitor_result.first)
    cout <<"Horizon is reachable (Skorokhod violation NOT detected)";
  else
    cout <<"Horizon is unreachable (Skorokhod violation detected)";
    
  cout <<endl;
  if(monitor_result.second){
    cout << "Trace x[0.." <<(gcurr_index-1)<<"] matches";
    cout << "Trace y[0.." <<(gcurr_index-1)<<"] for the given Skorokhod bound" <<endl;
  }
  else{
    cout << "Trace x[0.." <<(gcurr_index-1)<<"]  does NOT match";
    cout << "Trace y[0.." <<(gcurr_index-1)<<"] for the given Skorokhod bound" <<endl;
  }
  

  cout<<"Param read test" <<endl;
  /*
  const string param_file("skorokhod_parameters");
  ifstream in_param( param_file);

  int window;
  double delta;
  vector<double> scale_vector;
  if(in_param) read_global_params(window, delta, scale_vector, in_param);
  cout <<"window: " <<window <<"  Delta: " << delta <<"  Scale_vector: " ;
  printvec(scale_vector);
  */
  cout <<"WINDOW: " <<WINDOW <<"  Delta: " << DELTA <<"  Scale_vector: " ;
  printvec(SCALE_VECTOR);
  


  return 0;
}
