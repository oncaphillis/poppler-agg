#include "AggGradient.h"
#include "AggMatrix.h"

#include <iomanip>


AggPoint AggPoint::operator*(const AggMatrix & m) const {
    AggPoint p(*this);
    m->transform(&p.x,&p.y);
    return p;
}

AggPoint & AggPoint::operator*=(const AggMatrix & m) {
    return *this=*this*m;
}

AggPoint AggPoint::operator-(const AggPoint & p) const {
    return AggPoint(x-p.x,y-p.y);
}

AggPoint & AggPoint::operator-=(const AggPoint & p) {
    return *this=*this-p;
}

double AggPoint::getDistance(const AggPoint & p) const {
    AggPoint p0 = *this - p;
    return ::sqrt(p0.x*p0.x + p0.y*p0.y);
}

double AggPoint::getAngle(const AggPoint & p) const {
    return ::atan2(p.y - y,p.x-x) / agg::pi * 180.0;
}
