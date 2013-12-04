#include "AggGradient.h"

std::ostream & operator<<(std::ostream & os , const AggPoint & p) {
    os << p.x << ";" << p.y;
    return os;
}
