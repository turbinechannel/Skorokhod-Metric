/*** 
Author: Vinayak Prabhu 
***/


#include "free-space-boundary.h"
#include "myprint.h"
#include  <iostream>

#include <vector>
#include <cassert>
#include<algorithm>
#include<functional>
#include<numeric>
#include <math.h> 
#include <utility>

using namespace std;

  
  void  ComputeConicCoeff(const vector<double> & f_first,  const vector<double> & f_second, 
				const vector<double> & g_first,  const vector<double> & g_second,  const double & delta, ConicCoeff & result)  
  //Quadratic equation characterizing free space given f=affine(f_first, f_second), g=affine(g_first, g_second) and delta
  //Prop 8 in techrep
  //return values in the variable result
  {

    vector<double> d_o, d_f, d_g;
    double val;

    
    assert( (f_first.size() ==  f_second.size() ) && (g_first.size() ==  g_second.size() ) && 
	    (f_first.size() ==  g_first.size() ));

    transform(f_first.begin(), f_first.end(),  g_first.begin(),   back_inserter(d_o),   minus<double>()  );
    
    
    transform(f_second.begin(), f_second.end(),  f_first.begin(),   back_inserter(d_f),   minus<double>()  );
    
    transform(g_first.begin(), g_first.end(),  g_second.begin(),   back_inserter(d_g),   minus<double>()  );
    
    /*  cout<<"d_o: ";
    printvec(d_o);
    cout<<"d_f: ";
    printvec(d_f);
    cout<<"d_g: ";
    printvec(d_g);*/

    val=0;
    result.A = inner_product(d_f.begin(), d_f.end(), d_f.begin(), val);

    val = 0;
    result.B = 2*  inner_product(d_f.begin(), d_f.end(), d_g.begin(), val);

    val = 0;
    result.C = inner_product(d_g.begin(), d_g.end(), d_g.begin(), val);

    val = 0;
    result.F = 2*  inner_product(d_o.begin(), d_o.end(), d_f.begin(), val);

    val = 0;
    result.G = 2*  inner_product(d_o.begin(), d_o.end(), d_g.begin(), val);

    val = -1 * delta * delta;
    result.H = inner_product(d_o.begin(), d_o.end(), d_o.begin(), val);

  }



pair<double, double> min_max_quadratic(const QuadraticCoeff  & q)
//min and max value of x satisfying A*x^2 + B*x + C \leq 0 in the interval [0,1]
//negative return values denote no solution to  A*x^2 + B*x + C \leq 0 in the interval [0,1]
//first element of return pair is min, the second is max
{
  double mymin, mymax;
  double root1, root2;
  const pair<double, double> invalid_pair(-1,-1);
  const pair<double, double> all_pair(0, 1);
  const double bac = (q.B*q.B) - (4* q.A *q.C);

  //handle q.A == 0
  if (q.A == 0){//Eqn now is B*x \leq -C
    if (q.B == 0){// Now 0 \leq -C
      if (q.C  <= 0) return all_pair;
      else return invalid_pair;
    }
    else{//Eqn now is B*x \leq -C
      root1 = -1*q.C/q.B;
      if (q.B >=0) {//Eqn now is x \leq -C/B
	if (root1 < 0) return invalid_pair;
	else{
	  if (root1 > 1) return all_pair;
	  else return make_pair(0, root1);
	}
      }
      else{ // q.B < 0; Eqn now is x \geq -C/B
	if (root1 < 0) return all_pair;
	else{
	  if (root1 > 1) return invalid_pair;
	  else return make_pair(root1,1);
	}
      }
    }
  }


  //Henceforth, Assume q.A \neq 0
  if (bac < 0) {
    if (q.C < 0) return all_pair;
    else return invalid_pair;
  }

  if (bac == 0){
    double quad_val;
    if (0==q.C) quad_val = q.A + q.B + q.C ; // 0 is a root, pick x= 1
    else quad_val = q.C; //0 is not a root
      
    if (quad_val<= 0) return all_pair; 
    else { // (quad_val  > 0) 
      root1 = -1*q.B / (2*q.A);
      if ( (0<= root1) && (root1 <= 1) ) return make_pair(root1, root1);
      else return invalid_pair;
    }
  }

  if (bac > 0){ 
    root1 =  (-1*q.B - sqrt(bac))/(2*q.A);
    root2 =  (-1*q.B + sqrt(bac))/(2*q.A);
    //make root1 < root2
    double tmp = root1;
    if (root1 > root2) {
      root1 = root2;
      root2 = tmp;
    }
    
    if(  ((2*q.A * root1) +q.B) > 0){//valid interval is [0,root1] union [root2,1]
      if (root1< 0){ //valid interval is [root2, 1]
	if (root2 > 1) return invalid_pair;
	else return make_pair(root2, 1);
      }
      else{// valid interval is [0,root1] union [root2,1]
	//min is 0
	if(root1 > 1) return all_pair;
	else{ // 0 <= root1 <= 1;   valid interval is [0,root1] union [root2,1]
	  if (root2 <=1) return all_pair;//this should not trigger in case root2 \neq root1
	                                                                      //as it would violate convexity
	  else return make_pair(0,root1);
	}
      }
    }
    else{//valid range is [0,1] \cap [root1, root2]
      if ( (root2 < 0) || (root1 > 1)) return invalid_pair;
      else{// root1<= 1 and  root2 >=0 ;  [0,1] \cap [root1, root2] is non-empty
	mymin = max(0.0, root1);
	mymax =  min(1.0, root2);
	return make_pair(mymin, mymax);
      }
    }
  }
  return invalid_pair; //unreachable

}


pair<double, double> min_max_cell(const ConicCoeff & conic, const int side)
{
  //Compute min max at cell boundaries.
  //  side denotes which boundary of the cell. Anticlockwise, bottom boundary = 0
  //Ax^2 + B xy + Cy^2 + Fx + Gy +H =0

  QuadraticCoeff q;
  pair<double, double> result;

  switch (side){

  case 0://y=0, conic is Ax^2 +  Fx +H =0
    q.A=conic.A;
    q.B =conic.F;
    q.C = conic.H;
    // cout<<"In min_max_cell. Edge:" <<side <<"     ";
    //printquadratic(q);
    result =  min_max_quadratic(q);
    break;

  case 2://y=1, conic is Ax^2 +  (F+B)x +(C+G+H)  =0
    q.A=conic.A;
    q.B =conic.F+conic.B;
    q.C = conic.C+conic.G+conic.H;
    //cout<<"In min_max_cell. Edge:"  <<side <<"     " ;
    //printquadratic(q);
    //    return min_max_quadratic(q);
    result =  min_max_quadratic(q);
    break;

  case 3://x=0, conic is  Cy^2  + Gy +H 
    q.A=conic.C;
    q.B =conic.G;
    q.C = conic.H;
    //cout<<"In min_max_cell. Edge:" <<side <<"     ";
    //printquadratic(q);
    result =  min_max_quadratic(q);
    //return min_max_quadratic(q);
    break;

  case 1: //x=1, conic is Cy^2 + (B+G) y+(A+F+H)
    q.A=conic.C;
    q.B =conic.B + conic.G;
    q.C = conic.A+ conic.F + conic.H;
    //cout<<"In min_max_cell. Edge:" <<side <<"     ";
    //printquadratic(q);
    result =  min_max_quadratic(q);
    //return min_max_quadratic(q);
    break;

  default:
     cout<<"Default";
    return make_pair(-1,-1);
    break;
  }
  //cout<<"      Output of min_max_quadratic:";
  //printpair(result);
  return result;

}

