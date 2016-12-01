
ALGORITHM
-----------------
-Input: Two piecewise constant traces  given as time-sampled sequences.

The algorithm computes the Skorokhod distance between two given piecewise constant
timed-sampled traces X and Y   for given a window size.
The procedure reads in a pair of sample values of two given traces, and repeatedly calls the function monitor() for each such pair.
The function monitor updates some global data structures, and returns the current Skorokhod distance value.

The algorithm runs in time O(m*W) where W is the window size, and m  the number of samples
(assuming a constant dimension);  in O(W) space.

Assumption: traces X and Y are sampled at the same time-points (and hence also contain the same number of sample points).
If this is not the case, then a preprocessing oversampling step needs to be performed.
The intersample duration can be variable -- that is, X and Y can be sampled at timepoints
0.0, 1.0, 2.0, 10.0, 12.0, 25.0, 25.5; this is not a problem.


PROGRAM
---------------

Compile by "make" (tested on Linux Debian). The binary will be named as  "skorokhod_monitor".

The files "skorokhod_input_x" and "skorokhod_input_y" contain the two traces to be compared.
Each line of "skorokhod_input_x"  contains one timesampled point per line -- the first number in a line 
is the timestamp, the following numbers (separated by spaces) denote the state vector. We call the whole line as the time-state vector.
If the state vector has dimension n, the time-state vector has dimension n+1.


GLOBAL PARAMETERS
-------------------
The global parameters for the Skorokhod monitoring algorithm reside in the file "skorokhod_parameters".
The global parameters are 
(a) The sliding window. A window of W allows a point X[i] to be matched to the segment Y[i-W]..Y[i+W].
(b) The scaling vector (of doubles) -- this vector is elementwise multiplied with the time-state vector to obtain "normalized" time-state values to be fed to the skorokhod monitoring algorithm.


A sample  "skorokhod_parameters" file is:
WINDOW 3
1 1
2 1.5
3 1.8

This results in the sliding window being 3, and the scaling vector of doubles being
[1.0, 1.5, 1.8] --- this will result in each time stamp being multiplied by 1.0, the first state dimension being scaled by 1.5, and the second state dimension being scaled by 1.8.
The Skorokhod distance value is between the normalized traces obtained as above.

The first string (WINDOW, 1,2,3 etc) of each line (mandatory) is  for ease of reference, and can be any string (the string values are ignored).

The filenames "skorokhod_input_x" ,  "skorokhod_input_y" and  "skorokhod_parameters" are defined in
"global_defs.h" and can be changed to other strings there.

INTERFACE
-----------------
The main() function is defined in skorokhod_monitor.cpp.
The function reads in a pair of values, and calls monitor(Timepoint_X, Timepoint_Y) repeatedly.
The function monitor maintains a global data structure, and saves required data.
In particular, Timepoint_X and Timepoint_Y can be destroyed after the return of 
monitor(Timepoint_X, Timepoint_Y).
The value returned by monitor(Timepoint_X, Timepoint_Y) is the current "running" Skorokhod distance.

REQUIREMENTS
-----------------------
1. WINDOW \geq 1
2. Dimension of scaling vector \geq 1+Dimension of the  state vector
     or equivalently, \geq Dimension of the  time-state vector
3. All time-state vectors (in the files "skorokhod_input_x" ,  "skorokhod_input_y")
 must have the same dimension.



OVER-APPROXIMATION
---------------------------------
Given two traces X, Y the procedure returns a value in the range [alpha, alpha+Delta]
where 
alpha = Skorokhod distance between the normalized traces X,Y
Delta = the maximal normalized time-gap between two successive sample points
I chose to implement the overapproximation scheme rather than implement the exact algorithm for the sake of running time -- the exact implementation requires twice the running time as it must replicate each time-sample value (as described in the journal paper in Formal Methods in System Design).


