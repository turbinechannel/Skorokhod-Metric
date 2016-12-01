#ifndef GUARD_Point
#define GUARD_Point

#include  <vector>

class timePoint {
public:
    double time;
    std::vector<double> point;

    timePoint(double _time, std::vector<double> _pt) {
        time = _time;
        point = _pt;
    }

    timePoint()  = default ;
      

    timePoint(const timePoint& that) {
        copy(that);
    }

    timePoint & operator=(const timePoint& that) {
        if (this!=&that) {
            copy(that);
            return *this;
        }
	else return *this;
    }

    void copy(const timePoint& that) {
        time = that.time;
        point = that.point;
    }

    double getTime() { return time; }
    std::vector<double> getPoint() { return point; }
};




#endif
