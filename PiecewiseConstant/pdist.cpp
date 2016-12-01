/***
Author: Vinayak Prabhu
***/

#include "point.h"
#include "pdist.h"

#include <utility>
#include <cassert>
#include <algorithm>
#include<functional>
#include <cmath>        // std::abs
#include <math.h>       /* sqrt */


using namespace std;



double pdist(const  timePoint & tp1, const timePoint & tp2 )
{
  double v1 = abs( tp1.time - tp2.time);
  
  vector<double> temp;

  transform(tp1.point.begin(), tp1.point.end(), tp2.point.begin(),  back_inserter(temp),  minus<double>() );

  double v2 = inner_product(temp.begin(), temp.end(), temp.begin(), 0.0);

  v2=sqrt(v2);

  return max(v1, v2);
}
