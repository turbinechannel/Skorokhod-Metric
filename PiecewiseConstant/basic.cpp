
/*
Basic algo (non-streaming)
1 indexing in arrays

NUM_POINTS: number of sampled data points
Assume the number of sampled points the same in both signals,
and also that the sampling occur at the same times.

std::pair<double, vector<double> >  arr_1[NUM_POINTS]
//First array of datapoints+times

std::pair<double, vector<double> >  arr_2[NUM_POINTS]
//First array of datapoints+times

double D[NUM_POINTS][NUM_POINTS]  //DP array

double comp(std::pair<double, vector<double> > , std::pair<double, vector<double> >)
Let comp be the function which gives the distance between two time-stamped
points, comp(p1, p2) = max( abs(p1.first-p2.first), norm(p1.second -p2.second))

Let us time-distort arr_1 and match the distortion to arr_2
*/

//Base case: mapping arr_1[1..i] to arr_2[1]
//Let us assume that the piecewise constant flows corressponding to
//an array arr is right continuous: so the k-th sample value is constant in
//the left-closed, right open interval [t_k, t_{k+1}) 

D[1,1] =  comp(arr_1[1], arr_2[1]);
for(i=2; i<= NUM_POINTS; i++)
    D[i,1] = max( comp(arr_1[i], arr_2[1]),  D[i-1,1] );
//we map the   flow corresponding to time [t_1, t_{i+1}) in arr_1 to the
//flow [t_1, t_1+epsilon) in arr_2 for a very small epsilon


for(j=2; j<= NUM_POINTS; j++)
  D[1,j] = max( comp(arr_1[1], arr_2[j]),  D[1,j-1] );
//other array


double v1=v2= v3=0;

for(i=2; i<= NUM_POINTS; i++)
  for(j=2; j<= NUM_POINTS; i++)
    {
      v1=max(D[i-1, j], comp(arr_1[i-1], arr_2[j]));
      //we map the   flow corresponding to time [t_1, t_i) in arr_1 to the
      //flow [t_1, t_j+epsilon) in arr_2 for a very small epsilon

      v2=max(D[i, j-1], comp(arr_1[i], arr_2[j-1]));
      //we map the   flow corresponding to time [t_1, t_{i+1}) in arr_1 to the
      //flow [t_1, t_{j-1}+epsilon) in arr_2 for a very small epsilon
      //This includes the possibility that time [t_1, t_{i+1}) in arr_1 may be
      //mapped to  [t_1, t_{k}+epsilon)  for k<<j-1. 
      //What we're doing works since we "pull" the sampled value arr_1[i] left through 
      //compression and have to match  arr_1[i]  to arr_2[k..j-1]
      
    v3 = max(D[i-1, j-1], comp(arr_1[i-1], arr_2[j-1]));
      
    D[i,j] = min (v1, v2, v3);
    }

return ( max(comp(arr_1[NUM_POINTS], arr_2[NUM_POINTS]), D[NUM_POINTS, NUM_POINTS]));

