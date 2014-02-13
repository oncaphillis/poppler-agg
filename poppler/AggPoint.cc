#include "AggGradient.h"
#include "AggMatrix.h"

#include <iomanip>

std::ostream & operator<<(std::ostream & os , const AggPoint & po) {
    std::ios::fmtflags     f = os.flags();
    std::streamsize        p = os.precision();
    std::streamsize        w = os.width();
    char                   c = os.fill();

    os.fill(' ');

    os << std::setw(9) << std::fixed << std::setprecision(3) << po.x << ";"
       << std::setw(9) << std::fixed << std::setprecision(3) << po.y;

    os.fill(c);
    os.width(w);
    os.precision(p);
    os.flags(f);

    return os;
}

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
