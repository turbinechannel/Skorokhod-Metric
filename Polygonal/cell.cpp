/*** 
Author: Vinayak Prabhu 
***/


#include "cell.h"
#include "myprint.h"
#include "free-space-boundary.h"
#include <assert.h>
#include  <iostream>

#include <algorithm> //for max
#include <utility>



void Cell::setCorner(const double & xval, const double & yval)
  {
    x=xval;
    y=yval;
  }

std::pair<double, double> Cell::getCorner() const
  {
    return std::make_pair(x,y);
  }


void Cell::setEdgeMin(const double & val, const int & edge)
{
  if ( (0 <= edge) && (edge <4)) emin[edge] = val;
}


void Cell::setEdgeMax(const double & val, const int & edge)
{
  if ( (0 <= edge) && (edge <4)) emax[edge] = val;
}

void Cell::setEdge(const std::pair<double, double> & boundary, const int & edge)
{
  //std::cout<<"In setEdge ";
  //printpair(boundary);
  assert(boundary.first <= boundary.second);
  if ((0 <= edge) && (edge <4)) {
    setEdgeMin(boundary.first, edge);
    setEdgeMax(boundary.second, edge);
  }
}



double Cell:: getEdgeMin(const int & edge) const 
{
  if ( (0 <= edge) && (edge <4)) return emin[edge];
  else return -2; //invalid
}


double Cell:: getEdgeMax(const int & edge) const 
{
  if ( (0 <= edge) && (edge <4)) return emax[edge];
  else return -2; //invalid
}

void Cell::invalidateCell()
{
  for(int edge =0; edge<4; edge++){
     setEdgeMin(-1,edge);
     setEdgeMax(-1,edge);
  }
}

void Cell::invalidateEdge(const int & edge)
{
  setEdgeMin(-1,edge);
  setEdgeMax(-1,edge);
}


void Cell::reachUpdate(const Cell & left, const Cell & bot, const int & mode)
{
  //mode 0: normal. mode 1: for topmost triangular cell.  Mode -1: bottomost triagular cell  
  //Assume bottom and left cells will be more constrained
  //initialize edges 0 & 3

  if ((mode==0) or (mode == 1)){   
    setEdgeMin(bot.getEdgeMin(2), 0);
    setEdgeMax(bot.getEdgeMax(2), 0);
    }
  if( (mode == 0) or (mode == -1)){
    setEdgeMin(left.getEdgeMin(1), 3);
    setEdgeMax(left.getEdgeMax(1), 3);
  }

  if (mode == 1){
    invalidateEdge(3);
    invalidateEdge(2);
  }

  if (mode == -1){
    invalidateEdge(0);
    invalidateEdge(1);
  }
  
  //special case: left and bottom boundaries are unreachable
  if( (getEdgeMin(0) < 0) and  (getEdgeMin(3)< 0)){
    invalidateEdge(1);
    invalidateEdge(2);
    return; //whole cell is invalid
  }
    
  //edge 1: right edge
  if( (0<= getEdgeMin(0)) and (getEdgeMin(0)< 1)) ; //anything valid less than 1; noop (no change to right edge)
  else if ( (getEdgeMax(0) == 1) and (getEdgeMin(1)==0)); //special case
     //theoretically either both conjuncts are true, or neither is. Noop
  else if (getEdgeMin(0) < 0){//have to use left edge to cross to right side
    //By special case clause, the left edge cannot be empty
    if(getEdgeMin(3) >  getEdgeMax(1))   invalidateEdge(1);//unreachable
    else setEdgeMin( std::max(getEdgeMin(3), getEdgeMin(1)), 1); //max value is unchanged
  } 

  //edge 2; top edge
  if( (0<= getEdgeMin(3)) and  (getEdgeMin(3)< 1)) ; //anything valid less than 1; noop
  else if ( (getEdgeMax(3) == 1) and (getEdgeMin(2)==0)); //special case
     //theoretically either both conjuncts are true, or neither is. Noop
  else if (getEdgeMin(3) < 0){//have to use bottom edge to cross to top side
    //By special case clause, the bottom edge cannot be empty
    if(getEdgeMin(0) >  getEdgeMax(2)) invalidateEdge(2);//unreachable
    else setEdgeMin( std::max(getEdgeMin(0), getEdgeMin(2)), 2); //max value is unchanged
  } 

  return;

}


void Cell::reachUpdateBot(const Cell & bot_cell)
//must enter throught bot_cell
{
  if (0 == bot_cell.getEdgeMax(2) ) return;
  
  invalidateEdge(3);

  if (-1 == bot_cell.getEdgeMin(2)) {
    invalidateCell();
    return;
  }
 
  //edge 2 of bot_cell is valid; and so must also be edge 0 of current cell
  setEdgeMin( bot_cell.getEdgeMin(2), 0);
  setEdgeMax( bot_cell.getEdgeMax(2), 0);
  
  //compute top edge
  if (getEdgeMax(2) < bot_cell.getEdgeMin(2)) invalidateEdge(2);
  else {
    setEdgeMin( std::max( bot_cell.getEdgeMin(2), getEdgeMin(2)), 2); //max unchanged
  }

  //if edge 2 of bot_cell is valid, the right edge of current cell remains unchanged
  return;
}


void Cell::reachUpdateLeft(const Cell & left_cell)
//must enter throught left_cell
{
  if(0 == left_cell.getEdgeMin(1)) return;

  invalidateEdge(0);

  if (-1 == left_cell.getEdgeMin(1)) {
    invalidateCell();
    return;
  }

  //edge 1 of left cell is valid; and so must also be edge 3 of current cell
  setEdgeMin( left_cell.getEdgeMin(1), 3);
  setEdgeMax( left_cell.getEdgeMax(1), 3);

  //compute right edge
  if (getEdgeMax(1) < left_cell.getEdgeMin(1)) invalidateEdge(1);
  else {
    setEdgeMin( std::max( left_cell.getEdgeMin(1), getEdgeMin(1)), 1); //max unchanged
  }
				     
  //if edge 1 of left_cell is valid, the top edge of current cell remains unchanged
  return;
}





void Cell::intersectWith(const Cell & other)
{
  double l, h;

  for(int edge =0; edge < 4; edge++){

    if( (getEdgeMin(edge) <0) or  ( other.getEdgeMin(edge) <0)) {
      invalidateEdge(edge);
    }
    else{//both edges valid
      l = std::max(getEdgeMin(edge), other.getEdgeMin(edge));
      h = std::min(getEdgeMax(edge), other.getEdgeMax(edge));
      if (l> h) invalidateEdge(edge);
      else setEdge(std::make_pair(l, h), edge);
    }
    
  }

}



void Cell::freeSpaceFromVec(const std::vector<double> & px1, const std::vector<double> & px2, const std::vector<double> & py1,  const std::vector<double> & py2, const double & delta)
{
  //From points construct conic
  //From conic construct 4 quadratics
  //From quadratics get min max values
  ConicCoeff conic;
  std::pair<double, double> min_max;

  // std::cout <<"Cell::freeSpaceFromVec  vec size:"<<px1.size()<<std::endl;
  xpoint1 = px1;
  xpoint2 = px2;
  ypoint1 = py1;
  ypoint2 = py2;

  //std::cout <<"Cell::freeSpaceFromVec Post"<<std::endl;

  ComputeConicCoeff(px1, px2, py1, py2, delta, conic);
  // printconic(conic);
  // std::cout<<"Next calling setEdges" <<std::endl;
  for(int edge=0; edge<4; edge++){
    min_max = min_max_cell(conic, edge);
    setEdge(min_max, edge);
  }
    
}


