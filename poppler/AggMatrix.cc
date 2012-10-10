#include "AggMatrix.h"

const AggMatrix AggMatrix::MirrorX = AggMatrix(1.0, 0.0, 0.0, 1.0,  0.0,  0.0);
const AggMatrix AggMatrix::MirrorY = AggMatrix(1.0, 0.0, 0.0,-1.0,  0.0,  0.0);

const AggMatrix AggMatrix::Scaling(double x,double y)
{
  agg::trans_affine t;
  return  t * agg::trans_affine_scaling( x , y );
}   
