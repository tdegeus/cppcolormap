
#ifndef CPPCOLORMAP_H
#define CPPCOLORMAP_H

#include <vector>
#include <cfloat>
#include <math.h>

namespace cppcolormap {

template<typename T>
std::vector<float> linspace(T start_in, T end_in, size_t num_in)
{
  float start = static_cast<float>(start_in);
  float end   = static_cast<float>(end_in);
  float num   = static_cast<float>(num_in);
  float delta = (end-start)/(num-1);

  std::vector<float> out;

  for( size_t i=0 ; i<num ; ++i )
    out.push_back(start+delta*i);

  out.push_back(end);

  return out;
};

size_t findNearestNeighbourIndex( float value, std::vector<float> &x )
{
  float  dist = FLT_MAX;
  size_t idx  = 0;

  for ( size_t i=0 ; i<x.size() ; ++i ) {
    float newDist = value-x[i];
    if ( newDist>0 && newDist<dist ) {
      dist = newDist;
      idx  = i;
    }
  }

  return idx;
};

std::vector<float> interp1( std::vector<float> &x, std::vector<float> &y, std::vector<float> &x_new )
{
  std::vector<float> y_new,dx,dy,slope,intercept;

  y_new    .reserve( x_new.size() );
  dx       .reserve( x    .size() );
  dy       .reserve( x    .size() );
  slope    .reserve( x    .size() );
  intercept.reserve( x    .size() );

  for ( size_t i=0 ; i<x.size()-1 ; ++i ) {
    dx       .push_back( x[i+1]-x[i]        );
    dy       .push_back( y[i+1]-y[i]        );
    slope    .push_back( dy[i]/dx[i]        );
    intercept.push_back( y[i]-x[i]*slope[i] );
  }

  dx       .push_back( dx[x.size()-2]        );
  dy       .push_back( dy[x.size()-2]        );
  slope    .push_back( slope[x.size()-2]     );
  intercept.push_back( intercept[x.size()-2] );

  for ( size_t i = 0; i < x_new.size(); ++i )
  {
    size_t idx = findNearestNeighbourIndex(x_new[i],x);
    y_new.push_back( slope[idx]*x_new[i]+intercept[idx] );
  }

  y_new[0]              = y[0];
  y_new[x_new.size()-1] = y[y.size()-1];

  return y_new;
};

std::vector<int> interp ( std::vector<int> data , size_t N )
{
  size_t n = data.size()/3;

  std::vector<int>   out(N*3);
  std::vector<float> x,xi,c(n),ci;

  x  = linspace(0.0,1.0,n);
  xi = linspace(0.0,1.0,N);

  for ( size_t j=0 ; j<3 ; j++ ) {

    for ( size_t i=0 ; i<n ; i++ )
      c[i] = static_cast<float>(data[i*3+j]);

    ci = interp1(x,c,xi);

    for ( size_t i=0 ; i<N ; i++ )
      out[i*3+j] = static_cast<int>(ci[i]);
  }

  return out;
};

std::vector<int> reverse ( std::vector<int> in )
{
  std::vector<int> out(in.size());
  size_t N = in.size()/3;

  for ( size_t i=0 ; i<N ; i++ )
    for ( size_t j=0 ; j<3 ; j++ )
      out[i*3+j] = in[(N-i-1)*3+j];

  return out;
};

std::vector<int> RdOrYl ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(128); data.push_back(0  ); data.push_back(38 );
  data.push_back(189); data.push_back(0  ); data.push_back(38 );
  data.push_back(227); data.push_back(26 ); data.push_back(28 );
  data.push_back(252); data.push_back(78 ); data.push_back(42 );
  data.push_back(253); data.push_back(141); data.push_back(60 );
  data.push_back(254); data.push_back(178); data.push_back(76 );
  data.push_back(254); data.push_back(217); data.push_back(118);
  data.push_back(255); data.push_back(237); data.push_back(160);
  data.push_back(255); data.push_back(255); data.push_back(204);

  return interp(data,N);
};

std::vector<int> RdOrYl_r ( size_t N=256 ) { return reverse(RdOrYl(N)); };

}; // namespace cppcolormap

#endif

