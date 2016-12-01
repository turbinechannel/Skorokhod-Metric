#ifndef GUARD_Free_Space_Boundary
#define GUARD_Free_Space_Boundary

#include <vector>
#include <utility>


struct ConicCoeff{
  double A;
  double B;
  double C;
  double F;
  double G;
  double H;
};

struct QuadraticCoeff{
  double A;
  double B;
  double C;
};


void  ComputeConicCoeff(const std::vector<double> & f_first,  const std::vector<double> & f_second, 
			const std::vector<double> & g_first,  const std::vector<double> & g_second,  const double & delta, ConicCoeff & result) ;


std::pair<double, double> min_max_quadratic(const QuadraticCoeff  & q);

std::pair<double, double> min_max_cell(const ConicCoeff & conic, const int side);


#endif
