/*** 
Author: Vinayak Prabhu 
***/


#include "myprint.h"
#include "cell.h"
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


void printconic(const ConicCoeff & conic_vals)
{
  cout <<"[ConicVals]  A: " <<conic_vals.A <<"  B: " <<  conic_vals.B <<"  C: "
       <<conic_vals.C <<"  F: " <<conic_vals.F << "  G: " <<conic_vals.G 
       <<"   H: " <<conic_vals.H << endl;
}


void printquadratic(const QuadraticCoeff & q)
{
  cout <<"[Quadratic] A: " <<q.A <<" B: "<<q.B << " C: "<<q.C<<endl;
}


void printCell(const Cell & cell)
{
  cout << "Cell Corner:  <" <<cell.getCorner().first  <<", " <<cell.getCorner().second <<"> "<<endl;
  cout<<"     Edge 0: ["<<cell.getEdgeMin(0)<<", "<<cell.getEdgeMax(0)<<"]    ";
  cout<<"Edge 1: ["<<cell.getEdgeMin(1)<<", "<<cell.getEdgeMax(1)<<"]    ";
  cout<<"Edge 2: ["<<cell.getEdgeMin(2)<<", "<<cell.getEdgeMax(2)<<"]    ";
  cout<<"Edge 3: ["<<cell.getEdgeMin(3)<<", "<<cell.getEdgeMax(3)<<"]    "<<endl;
  cout <<"Cell made from: xpoint1 (size= " <<cell.xpoint1.size()<<"): ";
  printvec_nonend(cell.xpoint1);
  cout <<" xpoint2: ";
  printvec_nonend(cell.xpoint2);
  cout <<endl;
  cout <<"                                   ypoint1: ";
  printvec_nonend(cell.ypoint1);
  cout <<" ypoint2: ";
  printvec_nonend(cell.ypoint2);
  cout <<endl;
  

}
      


void   printCellArrays()
{
  int max_arr;

  const int curr_rowcol  =gprev_curr.second;

  if ( (gcurr_index -  WINDOW) >= 0) max_arr = WINDOW;
  else max_arr = gcurr_index-1;
  
  cout <<"Printing arrays at gcurr_index =" <<gcurr_index<<endl;
  cout <<"Row" <<endl;

  for(int i=max_arr; i >=0; i--) printCell(grows[curr_rowcol][i]);

  cout <<"Cell" <<endl;

  for(int i=max_arr; i >=0; i--) printCell(gcols[curr_rowcol][i]);


}
