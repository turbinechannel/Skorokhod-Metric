/*** 
Author: Vinayak Prabhu 
***/


//#include "free-space-boundary.h"
#include "myprint.h"
//#include "cell.h"
#include "point.h"
#include "monitor.h"
#include "global_defs.h"
#include "read_input_data.h"
#include  <vector>
#include  <iostream>
#include <fstream>



using namespace std;


int main(){

  cout<< "Monitoring the PWC Skorokhod distance" <<endl <<endl;

  global_init();  //allocate memory for the global data structures used by monitor()

  pair<bool, bool> monitor_result(true, true);

  timePoint tp_x, tp_y;
  ifstream in_x( INPUT_FILE_X);
  ifstream in_y( INPUT_FILE_Y);
  int lcurr_index=0;

  if (in_x and in_y){
    while(read_timePoint(tp_x, in_x) and read_timePoint(tp_y, in_y)){

      //comment out next 7 lines in case the read sample values should not be printed
      cout<<"sample: " <<lcurr_index   <<"   Normalised values:" <<endl;
      cout<< "   X.time: " <<tp_x.time ;
      cout <<"     X.val:  ";
      printvec(tp_x.point);
      cout<< "   Y.time: " <<tp_y.time ;
      cout <<"     Y.val:  ";
      printvec(tp_y.point);


      lcurr_index++;
      monitor_result = monitor(tp_x, tp_y);
      //      if (not monitor_result.first) break;
      //The function monitor() returns two bool values (see the README).
      //Once it returns false as the first bool value, further invocations have undefined behavior.
    }
  }
  else {
    cout <<"Error opening files" <<endl;
    return 0;
  }

  cout<<endl <<endl;

  end_message(lcurr_index, monitor_result.first, monitor_result.second); //print the result

  return 0;
}


