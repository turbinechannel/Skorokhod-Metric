/*** 
Author: Vinayak Prabhu 
***/


#include "myprint.h"
#include "global_defs.h"
#include  <vector>
#include  <iostream>
#include  <iterator>
#include <utility>


using namespace std;


void printvec(const vector<double> & myvec)

{
  copy(myvec.begin(), myvec.end(),  ostream_iterator<double>(cout, "  "));
  cout <<endl;
}


void printvec_nonend(const vector<double> & myvec)

{
  copy(myvec.begin(), myvec.end(),  ostream_iterator<double>(cout, "  "));
}


void indexPrint(const int & i)
{
  cout <<"Indexprint: " << i <<endl;
}

void printpair(const pair<double, double> & p)
{
  cout <<"Pair("<<p.first<<", "<<p.second<<") "<<endl;
}





