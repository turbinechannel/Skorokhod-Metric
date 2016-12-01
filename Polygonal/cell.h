/*** 
Author: Vinayak Prabhu 
***/

#ifndef GUARD_Cell
#define GUARD_Cell

#include  <utility>
#include <vector>

class Cell {
   double x=0;
   double y =0;
  double emin[4] = {-1,-1,-1,-1};
  double emax[4] = {-1,-1,-1,-1};
 public:
   std::vector<double> xpoint1, xpoint2, ypoint1, ypoint2;
  void setCorner(const double & xval, const double & yval);
  std::pair<double, double> getCorner() const; //(x,y)
  void setEdgeMin(const double & val, const int & edge);
  void setEdgeMax(const double & val, const int  & edge);
  void setEdge(const std::pair<double, double> & boundary, const int & edge);
  double getEdgeMin(const int & edge) const;
  double getEdgeMax(const int & edge) const;
  void reachUpdate(const Cell & left_cell, const Cell & bot_cell, const int & mode); 
  void reachUpdateBot(const Cell & bot_cell);
  void reachUpdateLeft(const Cell & left_cell); 
  void intersectWith(const Cell & other);
  void freeSpaceFromVec(const std::vector<double> & px1, const std::vector<double> & px2, const std::vector<double> & py1,  const std::vector<double> & py2, const double & delta);
  void invalidateCell();
  void invalidateEdge(const int & edge);
};



#endif

