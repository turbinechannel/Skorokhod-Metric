
ALGORITHM
------------------
Input: Two polygonal traces (piecewise linear and continuous) given as time-sampled sequences.

The algorithm checks whether the Skorokhod distance between two given timed traces X and Y is less than or equal to DELTA,  given a DELTA and a window size.
The main procedure reads in a pair of sample values of two given traces, and repeatedly calls the function monitor() for each such pair.
The function monitor updates some global data structures, and returns two bool values.

1. The first bool value denotes whether it is is possible to extend the sample traces 
(X_0, .., X_curr) and (Y_0, .., Y_curr) (where curr is the current sample point number) so that the Skorokhod bound can be
satisfied. This is done by checking whether a horizon in a global 2-D array is reachable or not.

2. The second bool value denotes whether  the distance between 
(X_0,..,X_curr) and (Y_0,..,Y_curr) is less then or equal to the
Skorokhod bound. Observe that if the second bool value is true, so must the first.

We terminate on end of input, or when the first bool value becomes false.
Both bool values are printed.

If we are given traces X[0..M] and Y[0..M], and if at the end of the execution the first
bool value is true, and the second false, then it means that X[0..M] does NOT
match  Y[0..M] according to the Skorokhod bound.

The algorithm runs in time O(m*W) where W is the window size, and m  the number of samples
(assuming a constant dimension);  in O(W) space.



PROGRAM
---------------

Compile by "make". The binary will be named as  "skorokhod_monitor".

The files "skorokhod_input_x" and "skorokhod_input_y" contain the two traces to be compared.
Each line of "skorokhod_input_x"  contains one timepoint per line -- the first number in a line 
is the timestamp, the following numbers denote the state vector. We call the whole line as the time-state vector.
If the state vector has dimension n, the time-state vector has dimension n+1.


GLOBAL PARAMETERS
-------------------
The global parameters for the Skorokhod monitoring algorithm reside in the file "skorokhod_parameters".
The global parameters are 
a. The sliding window. A window of W allows a point X[i] to be matched to the segment Y[i-W]..Y[i+W].
b. The Skorokhod distance bound to be monitored.
c. The scaling vector (of doubles) -- this vector is dot producted with the time-state vector to obtain "normalized" values
to be fed to the skorokhod monitoring algorithm.

The format of the file "skorokhod_parameters" is:
WINDOW 3
DELTA 0.29
1 1
2 1
3 1

This results in the sliding window being 3, the Skorokhod bound being 0.29, and the scaling vector of doubles being
[1, 1,1,].

The first string (WINDOW, 1,2,3 etc) of each line while mandatory,  is just for ease of reference.

The filenames "skorokhod_input_x" ,  "skorokhod_input_y" and  "skorokhod_parameters" are defined in
"global_defs.h".



INTERFACE
----------------

The main() function is defined in skorokhod_monitor.cpp.
The function reads in a pair of values, and calls monitor(Timepoint_X, Timepoint_Y).
The function monitor maintains a global data structure, and saves required data.
In particular, Timepoint_X and Timepoint_Y can be destroyed after the return of 
monitor(Timepoint_X, Timepoint_Y).
The  function monitor updates some global data structures, and returns two bool values
as described earlier.

REQUIREMENTS
-----------------------
1. WINDOW \geq 1
2. DELTA \geq 0
3. Dimension of scaling vector \geq 1+Dimension of the  state vector
     or equivalently, \geq Dimension of the  time-state vector
4. All time-state vectors must have the same dimension.

