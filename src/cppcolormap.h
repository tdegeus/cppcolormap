/* =================================================================================================

(c - GPLv3) T.W.J. de Geus (Tom) | tom@geus.me | www.geus.me | github.com/tdegeus/cppcolormap

================================================================================================= */

#ifndef CPPCOLORMAP_H
#define CPPCOLORMAP_H

#define CPPCOLORMAP_WORLD_VERSION 0
#define CPPCOLORMAP_MAJOR_VERSION 1
#define CPPCOLORMAP_MINOR_VERSION 5

#define CPPCOLORMAP_VERSION_AT_LEAST(x,y,z) \
  (CPPCOLORMAP_WORLD_VERSION>x || (CPPCOLORMAP_WORLD_VERSION>=x && \
  (CPPCOLORMAP_MAJOR_VERSION>y || (CPPCOLORMAP_MAJOR_VERSION>=y && \
                                   CPPCOLORMAP_MINOR_VERSION>=z))))

#define CPPCOLORMAP_VERSION(x,y,z) \
  (CPPCOLORMAP_WORLD_VERSION==x && \
   CPPCOLORMAP_MAJOR_VERSION==y && \
   CPPCOLORMAP_MINOR_VERSION==z)

#include <vector>
#include <string>
#include <cfloat>
#include <math.h>
#include <cppmat/matrix.h>

namespace cppcolormap {

// =================================================================================================

template<typename T>
inline std::vector<float> linspace(T start_in, T end_in, size_t num_in)
{
  float start = static_cast<float>(start_in);
  float end   = static_cast<float>(end_in);
  float num   = static_cast<float>(num_in);
  float delta = (end-start)/(num-1);

  std::vector<float> out;

  for( size_t i = 0 ; i < num ; ++i )
    out.push_back(start+delta*i);

  out.push_back(end);

  return out;
}

// =================================================================================================

inline size_t findNearestNeighbourIndex(float value, const std::vector<float> &x)
{
  float  dist = FLT_MAX;
  size_t idx  = 0;

  for ( size_t i = 0 ; i<x.size() ; ++i )
  {
    float newDist = value-x[i];
    if ( newDist > 0 and newDist < dist )
    {
      dist = newDist;
      idx  = i;
    }
  }

  return idx;
}

// =================================================================================================

inline std::vector<float> interp1(const std::vector<float> &x, const std::vector<float> &y,
  const std::vector<float> &x_new)
{
  std::vector<float> y_new,dx,dy,slope,intercept;

  y_new    .reserve( x_new.size() );
  dx       .reserve( x    .size() );
  dy       .reserve( x    .size() );
  slope    .reserve( x    .size() );
  intercept.reserve( x    .size() );

  for ( size_t i = 0 ; i < x.size()-1 ; ++i )
  {
    dx       .push_back( x[i+1]-x[i]        );
    dy       .push_back( y[i+1]-y[i]        );
    slope    .push_back( dy[i]/dx[i]        );
    intercept.push_back( y[i]-x[i]*slope[i] );
  }

  dx       .push_back( dx[x.size()-2]        );
  dy       .push_back( dy[x.size()-2]        );
  slope    .push_back( slope[x.size()-2]     );
  intercept.push_back( intercept[x.size()-2] );

  for ( size_t i = 0 ; i < x_new.size() ; ++i )
  {
    size_t idx = findNearestNeighbourIndex(x_new[i],x);
    y_new.push_back( slope[idx]*x_new[i]+intercept[idx] );
  }

  y_new[0]              = y[0];
  y_new[x_new.size()-1] = y[y.size()-1];

  return y_new;
}

// =================================================================================================

inline cppmat::matrix<int> interp(const cppmat::matrix<int> &data , size_t N)
{
  size_t n = data.size()/3;

  cppmat::matrix<int> out({N,data.shape(1)});
  std::vector<float> x,xi,c(n),ci;

  x  = linspace(0.0,1.0,n);
  xi = linspace(0.0,1.0,N);

  for ( size_t j = 0 ; j < data.shape(1) ; j++ )
  {

    for ( size_t i = 0 ; i < n ; i++ ) c[i] = static_cast<float>(data(i,j));

    ci = interp1(x,c,xi);

    for ( size_t i = 0 ; i < N ; i++ ) out(i,j) = static_cast<int>(ci[i]);
  }

  return out;
}

// =================================================================================================

inline cppmat::matrix<int> reverse(const cppmat::matrix<int> &in)
{
  cppmat::matrix<int> out(in.shape());

  size_t n = in.shape(0);
  size_t m = in.shape(1);

  for ( size_t i = 0 ; i < n ; ++i )
    for ( size_t j = 0 ; j < m ; ++j )
      out(i,j) = in(n-1-i,j);

  return out;
}

// =================================================================================================
// qualitative colormaps
// =================================================================================================

inline cppmat::matrix<int> Accent ( size_t N=8 )
{
  cppmat::matrix<int> data({8,3});

  data( 0,0) = 127;   data( 0,1) = 201;   data( 0,2) = 127;
  data( 1,0) = 190;   data( 1,1) = 174;   data( 1,2) = 212;
  data( 2,0) = 253;   data( 2,1) = 192;   data( 2,2) = 134;
  data( 3,0) = 255;   data( 3,1) = 255;   data( 3,2) = 153;
  data( 4,0) =  56;   data( 4,1) = 108;   data( 4,2) = 176;
  data( 5,0) = 240;   data( 5,1) =   2;   data( 5,2) = 127;
  data( 6,0) = 191;   data( 6,1) =  91;   data( 6,2) =  23;
  data( 7,0) = 102;   data( 7,1) = 102;   data( 7,2) = 102;

  return interp(data,N);
}

inline cppmat::matrix<int> Dark2 ( size_t N=8 )
{
  cppmat::matrix<int> data({8,3});

  data( 0,0) =  27;   data( 0,1) = 158;   data( 0,2) = 119;
  data( 1,0) = 217;   data( 1,1) =  95;   data( 1,2) =   2;
  data( 2,0) = 117;   data( 2,1) = 112;   data( 2,2) = 179;
  data( 3,0) = 231;   data( 3,1) =  41;   data( 3,2) = 138;
  data( 4,0) = 102;   data( 4,1) = 166;   data( 4,2) =  30;
  data( 5,0) = 230;   data( 5,1) = 171;   data( 5,2) =   2;
  data( 6,0) = 166;   data( 6,1) = 118;   data( 6,2) =  29;
  data( 7,0) = 102;   data( 7,1) = 102;   data( 7,2) = 102;

  return interp(data,N);
}

inline cppmat::matrix<int> Paired ( size_t N=12 )
{
  cppmat::matrix<int> data({12,3});

  data( 0,0) = 166;   data( 0,1) = 206;   data( 0,2) = 227;
  data( 1,0) =  31;   data( 1,1) = 120;   data( 1,2) = 180;
  data( 2,0) = 178;   data( 2,1) = 223;   data( 2,2) = 138;
  data( 3,0) =  51;   data( 3,1) = 160;   data( 3,2) =  44;
  data( 4,0) = 251;   data( 4,1) = 154;   data( 4,2) = 153;
  data( 5,0) = 227;   data( 5,1) =  26;   data( 5,2) =  28;
  data( 6,0) = 253;   data( 6,1) = 191;   data( 6,2) = 111;
  data( 7,0) = 255;   data( 7,1) = 127;   data( 7,2) =   0;
  data( 8,0) = 202;   data( 8,1) = 178;   data( 8,2) = 214;
  data( 9,0) = 106;   data( 9,1) =  61;   data( 9,2) = 154;
  data(10,0) = 255;   data(10,1) = 255;   data(10,2) = 153;
  data(11,0) = 177;   data(11,1) =  89;   data(11,2) =  40;

  return interp(data,N);
}

inline cppmat::matrix<int> Spectral ( size_t N=11 )
{
  cppmat::matrix<int> data({11,3});

  data( 0,0) = 158;   data( 0,1) =   1;   data( 0,2) =  66;
  data( 1,0) = 213;   data( 1,1) =  62;   data( 1,2) =  79;
  data( 2,0) = 244;   data( 2,1) = 109;   data( 2,2) =  67;
  data( 3,0) = 253;   data( 3,1) = 174;   data( 3,2) =  97;
  data( 4,0) = 254;   data( 4,1) = 224;   data( 4,2) = 139;
  data( 5,0) = 255;   data( 5,1) = 255;   data( 5,2) = 191;
  data( 6,0) = 230;   data( 6,1) = 245;   data( 6,2) = 152;
  data( 7,0) = 171;   data( 7,1) = 221;   data( 7,2) = 164;
  data( 8,0) = 102;   data( 8,1) = 194;   data( 8,2) = 165;
  data( 9,0) =  50;   data( 9,1) = 136;   data( 9,2) = 189;
  data(10,0) =  94;   data(10,1) =  79;   data(10,2) = 162;

  return interp(data,N);
}

inline cppmat::matrix<int> Pastel1 ( size_t N=9 )
{
  cppmat::matrix<int> data({9,3});

  data( 0,0) = 251;   data( 0,1) = 180;   data( 0,2) = 174;
  data( 1,0) = 179;   data( 1,1) = 205;   data( 1,2) = 227;
  data( 2,0) = 204;   data( 2,1) = 235;   data( 2,2) = 197;
  data( 3,0) = 222;   data( 3,1) = 203;   data( 3,2) = 228;
  data( 4,0) = 254;   data( 4,1) = 217;   data( 4,2) = 166;
  data( 5,0) = 255;   data( 5,1) = 255;   data( 5,2) = 204;
  data( 6,0) = 229;   data( 6,1) = 216;   data( 6,2) = 189;
  data( 7,0) = 253;   data( 7,1) = 218;   data( 7,2) = 236;
  data( 8,0) = 242;   data( 8,1) = 242;   data( 8,2) = 242;

  return interp(data,N);
}

inline cppmat::matrix<int> Pastel2 ( size_t N=8 )
{
  cppmat::matrix<int> data({8,3});

  data( 0,0) = 179;   data( 0,1) = 226;   data( 0,2) = 205;
  data( 1,0) = 253;   data( 1,1) = 205;   data( 1,2) = 172;
  data( 2,0) = 203;   data( 2,1) = 213;   data( 2,2) = 232;
  data( 3,0) = 244;   data( 3,1) = 202;   data( 3,2) = 228;
  data( 4,0) = 230;   data( 4,1) = 245;   data( 4,2) = 201;
  data( 5,0) = 255;   data( 5,1) = 242;   data( 5,2) = 174;
  data( 6,0) = 241;   data( 6,1) = 226;   data( 6,2) = 204;
  data( 7,0) = 204;   data( 7,1) = 204;   data( 7,2) = 204;

  return interp(data,N);
}

inline cppmat::matrix<int> Set1 ( size_t N=9 )
{
  cppmat::matrix<int> data({9,3});

  data( 0,0) = 228;   data( 0,1) =  26;   data( 0,2) =  28;
  data( 1,0) =  55;   data( 1,1) = 126;   data( 1,2) = 184;
  data( 2,0) =  77;   data( 2,1) = 175;   data( 2,2) =  74;
  data( 3,0) = 152;   data( 3,1) =  78;   data( 3,2) = 163;
  data( 4,0) = 255;   data( 4,1) = 127;   data( 4,2) =   0;
  data( 5,0) = 255;   data( 5,1) = 255;   data( 5,2) =  51;
  data( 6,0) = 166;   data( 6,1) =  86;   data( 6,2) =  40;
  data( 7,0) = 247;   data( 7,1) = 129;   data( 7,2) = 191;
  data( 8,0) = 153;   data( 8,1) = 153;   data( 8,2) = 153;

  return interp(data,N);
}

inline cppmat::matrix<int> Set2 ( size_t N=8 )
{
  cppmat::matrix<int> data({8,3});

  data( 0,0) = 102;   data( 0,1) = 194;   data( 0,2) = 165;
  data( 1,0) = 252;   data( 1,1) = 141;   data( 1,2) =  98;
  data( 2,0) = 141;   data( 2,1) = 160;   data( 2,2) = 203;
  data( 3,0) = 231;   data( 3,1) = 138;   data( 3,2) = 195;
  data( 4,0) = 166;   data( 4,1) = 216;   data( 4,2) =  84;
  data( 5,0) = 255;   data( 5,1) = 217;   data( 5,2) =  47;
  data( 6,0) = 229;   data( 6,1) = 196;   data( 6,2) = 148;
  data( 7,0) = 179;   data( 7,1) = 179;   data( 7,2) = 179;

  return interp(data,N);
}

inline cppmat::matrix<int> Set3 ( size_t N=12 )
{
  cppmat::matrix<int> data({12,3});

  data( 0,0) = 141;   data( 0,1) = 211;   data( 0,2) = 199;
  data( 1,0) = 255;   data( 1,1) = 255;   data( 1,2) = 179;
  data( 2,0) = 190;   data( 2,1) = 186;   data( 2,2) = 218;
  data( 3,0) = 251;   data( 3,1) = 128;   data( 3,2) = 114;
  data( 4,0) = 128;   data( 4,1) = 177;   data( 4,2) = 211;
  data( 5,0) = 253;   data( 5,1) = 180;   data( 5,2) =  98;
  data( 6,0) = 179;   data( 6,1) = 222;   data( 6,2) = 105;
  data( 7,0) = 252;   data( 7,1) = 205;   data( 7,2) = 229;
  data( 8,0) = 217;   data( 8,1) = 217;   data( 8,2) = 217;
  data( 9,0) = 188;   data( 9,1) = 128;   data( 9,2) = 189;
  data(10,0) = 204;   data(10,1) = 235;   data(10,2) = 197;
  data(11,0) = 255;   data(11,1) = 237;   data(11,2) = 111;

  return interp(data,N);
}

// =================================================================================================
// sequential colormaps
// =================================================================================================

inline cppmat::matrix<int> Blues ( size_t N=9 )
{
  cppmat::matrix<int> data({9,3});

  data( 0,0) = 247;   data( 0,1) = 251;   data( 0,2) = 255;
  data( 1,0) = 222;   data( 1,1) = 235;   data( 1,2) = 247;
  data( 2,0) = 198;   data( 2,1) = 219;   data( 2,2) = 239;
  data( 3,0) = 158;   data( 3,1) = 202;   data( 3,2) = 225;
  data( 4,0) = 107;   data( 4,1) = 174;   data( 4,2) = 214;
  data( 5,0) =  66;   data( 5,1) = 146;   data( 5,2) = 198;
  data( 6,0) =  33;   data( 6,1) = 113;   data( 6,2) = 181;
  data( 7,0) =   8;   data( 7,1) =  81;   data( 7,2) = 156;
  data( 8,0) =   8;   data( 8,1) =  48;   data( 8,2) = 107;

  return interp(data,N);
}

inline cppmat::matrix<int> Greens ( size_t N=9 )
{
  cppmat::matrix<int> data({9,3});

  data( 0,0) = 247;   data( 0,1) = 252;   data( 0,2) = 245;
  data( 1,0) = 229;   data( 1,1) = 245;   data( 1,2) = 224;
  data( 2,0) = 199;   data( 2,1) = 233;   data( 2,2) = 192;
  data( 3,0) = 161;   data( 3,1) = 217;   data( 3,2) = 155;
  data( 4,0) = 116;   data( 4,1) = 196;   data( 4,2) = 118;
  data( 5,0) =  65;   data( 5,1) = 171;   data( 5,2) =  93;
  data( 6,0) =  35;   data( 6,1) = 139;   data( 6,2) =  69;
  data( 7,0) =   0;   data( 7,1) = 109;   data( 7,2) =  44;
  data( 8,0) =   0;   data( 8,1) =  68;   data( 8,2) =  27;

  return interp(data,N);
}

inline cppmat::matrix<int> Greys ( size_t N=2 )
{
  cppmat::matrix<int> data({2,3});

  data( 0,0) =   0;   data( 0,1) =   0;   data( 0,2) =   0;
  data( 1,0) = 255;   data( 1,1) = 255;   data( 1,2) = 255;

  return interp(data,N);
}

inline cppmat::matrix<int> Oranges ( size_t N=9 )
{
  cppmat::matrix<int> data({9,3});

  data( 0,0) = 255;   data( 0,1) = 245;   data( 0,2) = 235;
  data( 1,0) = 254;   data( 1,1) = 230;   data( 1,2) = 206;
  data( 2,0) = 253;   data( 2,1) = 208;   data( 2,2) = 162;
  data( 3,0) = 253;   data( 3,1) = 174;   data( 3,2) = 107;
  data( 4,0) = 253;   data( 4,1) = 141;   data( 4,2) =  60;
  data( 5,0) = 241;   data( 5,1) = 105;   data( 5,2) =  19;
  data( 6,0) = 217;   data( 6,1) =  72;   data( 6,2) =   1;
  data( 7,0) = 166;   data( 7,1) =  54;   data( 7,2) =   3;
  data( 8,0) = 127;   data( 8,1) =  39;   data( 8,2) =   4;

  return interp(data,N);
}

inline cppmat::matrix<int> Purples ( size_t N=9 )
{
  cppmat::matrix<int> data({9,3});

  data( 0,0) = 252;   data( 0,1) = 251;   data( 0,2) = 253;
  data( 1,0) = 239;   data( 1,1) = 237;   data( 1,2) = 245;
  data( 2,0) = 218;   data( 2,1) = 218;   data( 2,2) = 235;
  data( 3,0) = 188;   data( 3,1) = 189;   data( 3,2) = 220;
  data( 4,0) = 158;   data( 4,1) = 154;   data( 4,2) = 200;
  data( 5,0) = 128;   data( 5,1) = 125;   data( 5,2) = 186;
  data( 6,0) = 106;   data( 6,1) =  81;   data( 6,2) = 163;
  data( 7,0) =  84;   data( 7,1) =  39;   data( 7,2) = 143;
  data( 8,0) =  63;   data( 8,1) =   0;   data( 8,2) = 125;

  return interp(data,N);
}

inline cppmat::matrix<int> Reds ( size_t N=9 )
{
  cppmat::matrix<int> data({9,3});

  data( 0,0) = 255;   data( 0,1) = 245;   data( 0,2) = 240;
  data( 1,0) = 254;   data( 1,1) = 224;   data( 1,2) = 210;
  data( 2,0) = 252;   data( 2,1) = 187;   data( 2,2) = 161;
  data( 3,0) = 252;   data( 3,1) = 146;   data( 3,2) = 114;
  data( 4,0) = 251;   data( 4,1) = 106;   data( 4,2) =  74;
  data( 5,0) = 239;   data( 5,1) =  59;   data( 5,2) =  44;
  data( 6,0) = 203;   data( 6,1) =  24;   data( 6,2) =  29;
  data( 7,0) = 165;   data( 7,1) =  15;   data( 7,2) =  21;
  data( 8,0) = 103;   data( 8,1) =   0;   data( 8,2) =  13;

  return interp(data,N);
}

inline cppmat::matrix<int> BuPu ( size_t N=9 )
{
  cppmat::matrix<int> data({9,3});

  data( 0,0) = 247;   data( 0,1) = 252;   data( 0,2) = 253;
  data( 1,0) = 224;   data( 1,1) = 236;   data( 1,2) = 244;
  data( 2,0) = 191;   data( 2,1) = 211;   data( 2,2) = 230;
  data( 3,0) = 158;   data( 3,1) = 188;   data( 3,2) = 218;
  data( 4,0) = 140;   data( 4,1) = 150;   data( 4,2) = 198;
  data( 5,0) = 140;   data( 5,1) = 107;   data( 5,2) = 177;
  data( 6,0) = 136;   data( 6,1) =  65;   data( 6,2) = 157;
  data( 7,0) = 129;   data( 7,1) =  15;   data( 7,2) = 124;
  data( 8,0) =  77;   data( 8,1) =   0;   data( 8,2) =  75;

  return interp(data,N);
}

inline cppmat::matrix<int> GnBu ( size_t N=9 )
{
  cppmat::matrix<int> data({9,3});

  data( 0,0) = 247;   data( 0,1) = 252;   data( 0,2) = 240;
  data( 1,0) = 224;   data( 1,1) = 243;   data( 1,2) = 219;
  data( 2,0) = 204;   data( 2,1) = 235;   data( 2,2) = 197;
  data( 3,0) = 168;   data( 3,1) = 221;   data( 3,2) = 181;
  data( 4,0) = 123;   data( 4,1) = 204;   data( 4,2) = 196;
  data( 5,0) =  78;   data( 5,1) = 179;   data( 5,2) = 211;
  data( 6,0) =  43;   data( 6,1) = 140;   data( 6,2) = 190;
  data( 7,0) =   8;   data( 7,1) = 104;   data( 7,2) = 172;
  data( 8,0) =   8;   data( 8,1) =  64;   data( 8,2) = 129;

  return interp(data,N);
}

inline cppmat::matrix<int> PuBu ( size_t N=9 )
{
  cppmat::matrix<int> data({9,3});

  data( 0,0) = 255;   data( 0,1) = 247;   data( 0,2) = 251;
  data( 1,0) = 236;   data( 1,1) = 231;   data( 1,2) = 242;
  data( 2,0) = 208;   data( 2,1) = 209;   data( 2,2) = 230;
  data( 3,0) = 166;   data( 3,1) = 189;   data( 3,2) = 219;
  data( 4,0) = 116;   data( 4,1) = 169;   data( 4,2) = 207;
  data( 5,0) =  54;   data( 5,1) = 144;   data( 5,2) = 192;
  data( 6,0) =   5;   data( 6,1) = 112;   data( 6,2) = 176;
  data( 7,0) =   4;   data( 7,1) =  90;   data( 7,2) = 141;
  data( 8,0) =   2;   data( 8,1) =  56;   data( 8,2) =  88;

  return interp(data,N);
}

inline cppmat::matrix<int> PuBuGn ( size_t N=9 )
{
  cppmat::matrix<int> data({9,3});

  data( 0,0) = 255;   data( 0,1) = 247;   data( 0,2) = 251;
  data( 1,0) = 236;   data( 1,1) = 226;   data( 1,2) = 240;
  data( 2,0) = 208;   data( 2,1) = 209;   data( 2,2) = 230;
  data( 3,0) = 166;   data( 3,1) = 189;   data( 3,2) = 219;
  data( 4,0) = 103;   data( 4,1) = 169;   data( 4,2) = 207;
  data( 5,0) =  54;   data( 5,1) = 144;   data( 5,2) = 192;
  data( 6,0) =   2;   data( 6,1) = 129;   data( 6,2) = 138;
  data( 7,0) =   1;   data( 7,1) = 108;   data( 7,2) =  89;
  data( 8,0) =   1;   data( 8,1) =  70;   data( 8,2) =  54;

  return interp(data,N);
}

inline cppmat::matrix<int> PuRd ( size_t N=9 )
{
  cppmat::matrix<int> data({9,3});

  data( 0,0) = 247;   data( 0,1) = 244;   data( 0,2) = 249;
  data( 1,0) = 231;   data( 1,1) = 225;   data( 1,2) = 239;
  data( 2,0) = 212;   data( 2,1) = 185;   data( 2,2) = 218;
  data( 3,0) = 201;   data( 3,1) = 148;   data( 3,2) = 199;
  data( 4,0) = 223;   data( 4,1) = 101;   data( 4,2) = 176;
  data( 5,0) = 231;   data( 5,1) =  41;   data( 5,2) = 138;
  data( 6,0) = 206;   data( 6,1) =  18;   data( 6,2) =  86;
  data( 7,0) = 152;   data( 7,1) =   0;   data( 7,2) =  67;
  data( 8,0) = 103;   data( 8,1) =   0;   data( 8,2) =  31;

  return interp(data,N);
}

inline cppmat::matrix<int> RdPu ( size_t N=9 )
{
  cppmat::matrix<int> data({9,3});

  data( 0,0) = 255;   data( 0,1) = 247;   data( 0,2) = 243;
  data( 1,0) = 253;   data( 1,1) = 224;   data( 1,2) = 221;
  data( 2,0) = 252;   data( 2,1) = 197;   data( 2,2) = 192;
  data( 3,0) = 250;   data( 3,1) = 159;   data( 3,2) = 181;
  data( 4,0) = 247;   data( 4,1) = 104;   data( 4,2) = 161;
  data( 5,0) = 221;   data( 5,1) =  52;   data( 5,2) = 151;
  data( 6,0) = 174;   data( 6,1) =   1;   data( 6,2) = 126;
  data( 7,0) = 122;   data( 7,1) =   1;   data( 7,2) = 119;
  data( 8,0) =  73;   data( 8,1) =   0;   data( 8,2) = 106;

  return interp(data,N);
}

inline cppmat::matrix<int> OrRd ( size_t N=9 )
{
  cppmat::matrix<int> data({9,3});

  data( 0,0) = 255;   data( 0,1) = 247;   data( 0,2) = 236;
  data( 1,0) = 254;   data( 1,1) = 232;   data( 1,2) = 200;
  data( 2,0) = 253;   data( 2,1) = 212;   data( 2,2) = 158;
  data( 3,0) = 253;   data( 3,1) = 187;   data( 3,2) = 132;
  data( 4,0) = 252;   data( 4,1) = 141;   data( 4,2) =  89;
  data( 5,0) = 239;   data( 5,1) = 101;   data( 5,2) =  72;
  data( 6,0) = 215;   data( 6,1) =  48;   data( 6,2) =  31;
  data( 7,0) = 179;   data( 7,1) =   0;   data( 7,2) =   0;
  data( 8,0) = 127;   data( 8,1) =   0;   data( 8,2) =   0;

  return interp(data,N);
}

inline cppmat::matrix<int> RdOrYl ( size_t N=9 )
{
  cppmat::matrix<int> data({9,3});

  data( 0,0) = 128;   data( 0,1) = 0  ;   data( 0,2) = 38 ;
  data( 1,0) = 189;   data( 1,1) = 0  ;   data( 1,2) = 38 ;
  data( 2,0) = 227;   data( 2,1) = 26 ;   data( 2,2) = 28 ;
  data( 3,0) = 252;   data( 3,1) = 78 ;   data( 3,2) = 42 ;
  data( 4,0) = 253;   data( 4,1) = 141;   data( 4,2) = 60 ;
  data( 5,0) = 254;   data( 5,1) = 178;   data( 5,2) = 76 ;
  data( 6,0) = 254;   data( 6,1) = 217;   data( 6,2) = 118;
  data( 7,0) = 255;   data( 7,1) = 237;   data( 7,2) = 160;
  data( 8,0) = 255;   data( 8,1) = 255;   data( 8,2) = 204;

  return interp(data,N);
}

inline cppmat::matrix<int> YlGn ( size_t N=9 )
{
  cppmat::matrix<int> data({9,3});

  data( 0,0) = 255;   data( 0,1) = 255;   data( 0,2) = 229;
  data( 1,0) = 247;   data( 1,1) = 252;   data( 1,2) = 185;
  data( 2,0) = 217;   data( 2,1) = 240;   data( 2,2) = 163;
  data( 3,0) = 173;   data( 3,1) = 221;   data( 3,2) = 142;
  data( 4,0) = 120;   data( 4,1) = 198;   data( 4,2) = 121;
  data( 5,0) =  65;   data( 5,1) = 171;   data( 5,2) =  93;
  data( 6,0) =  35;   data( 6,1) = 132;   data( 6,2) =  67;
  data( 7,0) =   0;   data( 7,1) = 104;   data( 7,2) =  55;
  data( 8,0) =   0;   data( 8,1) =  69;   data( 8,2) =  41;

  return interp(data,N);
}

inline cppmat::matrix<int> YlGnBu ( size_t N=9 )
{
  cppmat::matrix<int> data({9,3});

  data( 0,0) = 255;   data( 0,1) = 255;   data( 0,2) = 217;
  data( 1,0) = 237;   data( 1,1) = 248;   data( 1,2) = 177;
  data( 2,0) = 199;   data( 2,1) = 233;   data( 2,2) = 180;
  data( 3,0) = 127;   data( 3,1) = 205;   data( 3,2) = 187;
  data( 4,0) =  65;   data( 4,1) = 182;   data( 4,2) = 196;
  data( 5,0) =  29;   data( 5,1) = 145;   data( 5,2) = 192;
  data( 6,0) =  34;   data( 6,1) =  94;   data( 6,2) = 168;
  data( 7,0) =  37;   data( 7,1) =  52;   data( 7,2) = 148;
  data( 8,0) =   8;   data( 8,1) =  29;   data( 8,2) =  88;

  return interp(data,N);
}

inline cppmat::matrix<int> YlOrRd ( size_t N=9 )
{
  cppmat::matrix<int> data({9,3});

  data( 0,0) = 255;   data( 0,1) = 255;   data( 0,2) = 204;
  data( 1,0) = 255;   data( 1,1) = 237;   data( 1,2) = 160;
  data( 2,0) = 254;   data( 2,1) = 217;   data( 2,2) = 118;
  data( 3,0) = 254;   data( 3,1) = 178;   data( 3,2) =  76;
  data( 4,0) = 253;   data( 4,1) = 141;   data( 4,2) =  60;
  data( 5,0) = 252;   data( 5,1) =  78;   data( 5,2) =  42;
  data( 6,0) = 227;   data( 6,1) =  26;   data( 6,2) =  28;
  data( 7,0) = 189;   data( 7,1) =   0;   data( 7,2) =  38;
  data( 8,0) = 128;   data( 8,1) =   0;   data( 8,2) =  38;

  return interp(data,N);
}

// =================================================================================================
// diverging colormaps
// =================================================================================================

inline cppmat::matrix<int> BrBG ( size_t N=11 )
{
  cppmat::matrix<int> data({11,3});

  data( 0,0) =  84;   data( 0,1) =  48;   data( 0,2) =   5;
  data( 1,0) = 140;   data( 1,1) =  81;   data( 1,2) =  10;
  data( 2,0) = 191;   data( 2,1) = 129;   data( 2,2) =  45;
  data( 3,0) = 223;   data( 3,1) = 194;   data( 3,2) = 125;
  data( 4,0) = 246;   data( 4,1) = 232;   data( 4,2) = 195;
  data( 5,0) = 245;   data( 5,1) = 245;   data( 5,2) = 245;
  data( 6,0) = 199;   data( 6,1) = 234;   data( 6,2) = 229;
  data( 7,0) = 128;   data( 7,1) = 205;   data( 7,2) = 193;
  data( 8,0) =  53;   data( 8,1) = 151;   data( 8,2) = 143;
  data( 9,0) =   1;   data( 9,1) = 102;   data( 9,2) =  94;
  data(10,0) =   0;   data(10,1) =  60;   data(10,2) =  48;

  return interp(data,N);
}

inline cppmat::matrix<int> PuOr ( size_t N=11 )
{
  cppmat::matrix<int> data({11,3});

  data( 0,0) = 127;   data( 0,1) =  59;   data( 0,2) =   8;
  data( 1,0) = 179;   data( 1,1) =  88;   data( 1,2) =   6;
  data( 2,0) = 224;   data( 2,1) = 130;   data( 2,2) =  20;
  data( 3,0) = 253;   data( 3,1) = 184;   data( 3,2) =  99;
  data( 4,0) = 254;   data( 4,1) = 224;   data( 4,2) = 182;
  data( 5,0) = 247;   data( 5,1) = 247;   data( 5,2) = 247;
  data( 6,0) = 216;   data( 6,1) = 218;   data( 6,2) = 235;
  data( 7,0) = 178;   data( 7,1) = 171;   data( 7,2) = 210;
  data( 8,0) = 128;   data( 8,1) = 115;   data( 8,2) = 172;
  data( 9,0) =  84;   data( 9,1) =  39;   data( 9,2) = 136;
  data(10,0) =  45;   data(10,1) =   0;   data(10,2) =  75;

  return interp(data,N);
}

inline cppmat::matrix<int> RdBu ( size_t N=11 )
{
  cppmat::matrix<int> data({11,3});

  data( 0,0) = 103;   data( 0,1) =   0;   data( 0,2) =  31;
  data( 1,0) = 178;   data( 1,1) =  24;   data( 1,2) =  43;
  data( 2,0) = 214;   data( 2,1) =  96;   data( 2,2) =  77;
  data( 3,0) = 244;   data( 3,1) = 165;   data( 3,2) = 130;
  data( 4,0) = 253;   data( 4,1) = 219;   data( 4,2) = 199;
  data( 5,0) = 247;   data( 5,1) = 247;   data( 5,2) = 247;
  data( 6,0) = 209;   data( 6,1) = 229;   data( 6,2) = 240;
  data( 7,0) = 146;   data( 7,1) = 197;   data( 7,2) = 222;
  data( 8,0) =  67;   data( 8,1) = 147;   data( 8,2) = 195;
  data( 9,0) =  33;   data( 9,1) = 102;   data( 9,2) = 172;
  data(10,0) =   5;   data(10,1) =  48;   data(10,2) =  97;

  return interp(data,N);
}

inline cppmat::matrix<int> RdGy ( size_t N=11 )
{
  cppmat::matrix<int> data({11,3});

  data( 0,0) = 103;   data( 0,1) =   0;   data( 0,2) =  31;
  data( 1,0) = 178;   data( 1,1) =  24;   data( 1,2) =  43;
  data( 2,0) = 214;   data( 2,1) =  96;   data( 2,2) =  77;
  data( 3,0) = 244;   data( 3,1) = 165;   data( 3,2) = 130;
  data( 4,0) = 253;   data( 4,1) = 219;   data( 4,2) = 199;
  data( 5,0) = 255;   data( 5,1) = 255;   data( 5,2) = 255;
  data( 6,0) = 224;   data( 6,1) = 224;   data( 6,2) = 224;
  data( 7,0) = 186;   data( 7,1) = 186;   data( 7,2) = 186;
  data( 8,0) = 135;   data( 8,1) = 135;   data( 8,2) = 135;
  data( 9,0) =  77;   data( 9,1) =  77;   data( 9,2) =  77;
  data(10,0) =  26;   data(10,1) =  26;   data(10,2) =  26;

  return interp(data,N);
}

inline cppmat::matrix<int> RdYlBu ( size_t N=11 )
{
  cppmat::matrix<int> data({11,3});

  data( 0,0) = 165;   data( 0,1) =   0;   data( 0,2) =  38;
  data( 1,0) = 215;   data( 1,1) =  48;   data( 1,2) =  39;
  data( 2,0) = 244;   data( 2,1) = 109;   data( 2,2) =  67;
  data( 3,0) = 253;   data( 3,1) = 174;   data( 3,2) =  97;
  data( 4,0) = 254;   data( 4,1) = 224;   data( 4,2) = 144;
  data( 5,0) = 255;   data( 5,1) = 255;   data( 5,2) = 191;
  data( 6,0) = 224;   data( 6,1) = 243;   data( 6,2) = 248;
  data( 7,0) = 171;   data( 7,1) = 217;   data( 7,2) = 233;
  data( 8,0) = 116;   data( 8,1) = 173;   data( 8,2) = 209;
  data( 9,0) =  69;   data( 9,1) = 117;   data( 9,2) = 180;
  data(10,0) =  49;   data(10,1) =  54;   data(10,2) = 149;

  return interp(data,N);
}

inline cppmat::matrix<int> RdYlGn ( size_t N=11 )
{
  cppmat::matrix<int> data({11,3});

  data( 0,0) = 165;   data( 0,1) =   0;   data( 0,2) =  38;
  data( 1,0) = 215;   data( 1,1) =  48;   data( 1,2) =  39;
  data( 2,0) = 244;   data( 2,1) = 109;   data( 2,2) =  67;
  data( 3,0) = 253;   data( 3,1) = 174;   data( 3,2) =  97;
  data( 4,0) = 254;   data( 4,1) = 224;   data( 4,2) = 139;
  data( 5,0) = 255;   data( 5,1) = 255;   data( 5,2) = 191;
  data( 6,0) = 217;   data( 6,1) = 239;   data( 6,2) = 139;
  data( 7,0) = 166;   data( 7,1) = 217;   data( 7,2) = 106;
  data( 8,0) = 102;   data( 8,1) = 189;   data( 8,2) =  99;
  data( 9,0) =  26;   data( 9,1) = 152;   data( 9,2) =  80;
  data(10,0) =   0;   data(10,1) = 104;   data(10,2) =  55;

  return interp(data,N);
}

inline cppmat::matrix<int> PiYG ( size_t N=11 )
{
  cppmat::matrix<int> data({11,3});

  data( 0,0) = 142;   data( 0,1) =   1;   data( 0,2) =  82;
  data( 1,0) = 197;   data( 1,1) =  27;   data( 1,2) = 125;
  data( 2,0) = 222;   data( 2,1) = 119;   data( 2,2) = 174;
  data( 3,0) = 241;   data( 3,1) = 182;   data( 3,2) = 218;
  data( 4,0) = 253;   data( 4,1) = 224;   data( 4,2) = 239;
  data( 5,0) = 247;   data( 5,1) = 247;   data( 5,2) = 247;
  data( 6,0) = 230;   data( 6,1) = 245;   data( 6,2) = 208;
  data( 7,0) = 184;   data( 7,1) = 225;   data( 7,2) = 134;
  data( 8,0) = 127;   data( 8,1) = 188;   data( 8,2) =  65;
  data( 9,0) =  77;   data( 9,1) = 146;   data( 9,2) =  33;
  data(10,0) =  39;   data(10,1) = 100;   data(10,2) =  25;

  return interp(data,N);
}

inline cppmat::matrix<int> PRGn ( size_t N=11 )
{
  cppmat::matrix<int> data({11,3});

  data( 0,0) =  64;   data( 0,1) =   0;   data( 0,2) =  75;
  data( 1,0) = 118;   data( 1,1) =  42;   data( 1,2) = 131;
  data( 2,0) = 153;   data( 2,1) = 112;   data( 2,2) = 171;
  data( 3,0) = 194;   data( 3,1) = 165;   data( 3,2) = 207;
  data( 4,0) = 231;   data( 4,1) = 212;   data( 4,2) = 232;
  data( 5,0) = 247;   data( 5,1) = 247;   data( 5,2) = 247;
  data( 6,0) = 217;   data( 6,1) = 240;   data( 6,2) = 211;
  data( 7,0) = 166;   data( 7,1) = 219;   data( 7,2) = 160;
  data( 8,0) =  90;   data( 8,1) = 174;   data( 8,2) =  97;
  data( 9,0) =  27;   data( 9,1) = 120;   data( 9,2) =  55;
  data(10,0) =   0;   data(10,1) =  68;   data(10,2) =  27;

  return interp(data,N);
}

// =================================================================================================
// monochromatic colormaps
// =================================================================================================

inline cppmat::matrix<int> White ( size_t N=1 )
{
  cppmat::matrix<int> data({1,3});

  data( 0,0) = 255;   data( 0,1) = 255;   data( 0,2) = 255;

  return interp(data,N);
}

inline cppmat::matrix<int> Grey ( size_t N=1 )
{
  cppmat::matrix<int> data({1,3});

  data( 0,0) = 127;   data( 0,1) = 127;   data( 0,2) = 127;

  return interp(data,N);
}

inline cppmat::matrix<int> Black ( size_t N=1 )
{
  cppmat::matrix<int> data({1,3});

  data( 0,0) =   0;   data( 0,1) =   0;   data( 0,2) =   0;

  return interp(data,N);
}

inline cppmat::matrix<int> Red ( size_t N=1 )
{
  cppmat::matrix<int> data({1,3});

  data( 0,0) = 255;   data( 0,1) =   0;   data( 0,2) =   0;

  return interp(data,N);
}

inline cppmat::matrix<int> Blue ( size_t N=1 )
{
  cppmat::matrix<int> data({1,3});

  data( 0,0) =   0;   data( 0,1) =   0;   data( 0,2) = 255;

  return interp(data,N);
}

// =================================================================================================
// inverse colormaps
// =================================================================================================

inline cppmat::matrix<int> Accent_r   ( size_t N= 8 ) { return reverse(Accent  (N)); }
inline cppmat::matrix<int> Dark2_r    ( size_t N= 8 ) { return reverse(Dark2   (N)); }
inline cppmat::matrix<int> Paired_r   ( size_t N=12 ) { return reverse(Paired  (N)); }
inline cppmat::matrix<int> Spectral_r ( size_t N=11 ) { return reverse(Spectral(N)); }
inline cppmat::matrix<int> Pastel1_r  ( size_t N= 9 ) { return reverse(Pastel1 (N)); }
inline cppmat::matrix<int> Pastel2_r  ( size_t N= 8 ) { return reverse(Pastel2 (N)); }
inline cppmat::matrix<int> Set1_r     ( size_t N= 9 ) { return reverse(Set1    (N)); }
inline cppmat::matrix<int> Set2_r     ( size_t N= 8 ) { return reverse(Set2    (N)); }
inline cppmat::matrix<int> Set3_r     ( size_t N=12 ) { return reverse(Set3    (N)); }
inline cppmat::matrix<int> Blues_r    ( size_t N= 9 ) { return reverse(Blues   (N)); }
inline cppmat::matrix<int> Greens_r   ( size_t N= 9 ) { return reverse(Greens  (N)); }
inline cppmat::matrix<int> Greys_r    ( size_t N= 2 ) { return reverse(Greys   (N)); }
inline cppmat::matrix<int> Oranges_r  ( size_t N= 9 ) { return reverse(Oranges (N)); }
inline cppmat::matrix<int> Purples_r  ( size_t N= 9 ) { return reverse(Purples (N)); }
inline cppmat::matrix<int> Reds_r     ( size_t N= 9 ) { return reverse(Reds    (N)); }
inline cppmat::matrix<int> BuPu_r     ( size_t N= 9 ) { return reverse(BuPu    (N)); }
inline cppmat::matrix<int> GnBu_r     ( size_t N= 9 ) { return reverse(GnBu    (N)); }
inline cppmat::matrix<int> PuBu_r     ( size_t N= 9 ) { return reverse(PuBu    (N)); }
inline cppmat::matrix<int> PuBuGn_r   ( size_t N= 9 ) { return reverse(PuBuGn  (N)); }
inline cppmat::matrix<int> PuRd_r     ( size_t N= 9 ) { return reverse(PuRd    (N)); }
inline cppmat::matrix<int> RdPu_r     ( size_t N= 9 ) { return reverse(RdPu    (N)); }
inline cppmat::matrix<int> OrRd_r     ( size_t N= 9 ) { return reverse(OrRd    (N)); }
inline cppmat::matrix<int> RdOrYl_r   ( size_t N= 9 ) { return reverse(RdOrYl  (N)); }
inline cppmat::matrix<int> YlGn_r     ( size_t N= 9 ) { return reverse(YlGn    (N)); }
inline cppmat::matrix<int> YlGnBu_r   ( size_t N= 9 ) { return reverse(YlGnBu  (N)); }
inline cppmat::matrix<int> YlOrRd_r   ( size_t N= 9 ) { return reverse(YlOrRd  (N)); }
inline cppmat::matrix<int> BrBG_r     ( size_t N=11 ) { return reverse(BrBG    (N)); }
inline cppmat::matrix<int> PuOr_r     ( size_t N=11 ) { return reverse(PuOr    (N)); }
inline cppmat::matrix<int> RdBu_r     ( size_t N=11 ) { return reverse(RdBu    (N)); }
inline cppmat::matrix<int> RdGy_r     ( size_t N=11 ) { return reverse(RdGy    (N)); }
inline cppmat::matrix<int> RdYlBu_r   ( size_t N=11 ) { return reverse(RdYlBu  (N)); }
inline cppmat::matrix<int> RdYlGn_r   ( size_t N=11 ) { return reverse(RdYlGn  (N)); }
inline cppmat::matrix<int> PiYG_r     ( size_t N=11 ) { return reverse(PiYG    (N)); }
inline cppmat::matrix<int> PRGn_r     ( size_t N=11 ) { return reverse(PRGn    (N)); }

// =================================================================================================
// read from string
// =================================================================================================

inline cppmat::matrix<int> colormap ( std::string cmap, size_t N=256 )
{
  if      ( cmap=="Accent"      ) { return Accent     (N); }
  else if ( cmap=="Dark2"       ) { return Dark2      (N); }
  else if ( cmap=="Paired"      ) { return Paired     (N); }
  else if ( cmap=="Spectral"    ) { return Spectral   (N); }
  else if ( cmap=="Pastel1"     ) { return Pastel1    (N); }
  else if ( cmap=="Pastel2"     ) { return Pastel2    (N); }
  else if ( cmap=="Set1"        ) { return Set1       (N); }
  else if ( cmap=="Set2"        ) { return Set2       (N); }
  else if ( cmap=="Set3"        ) { return Set3       (N); }
  else if ( cmap=="Blues"       ) { return Blues      (N); }
  else if ( cmap=="Greens"      ) { return Greens     (N); }
  else if ( cmap=="Greys"       ) { return Greys      (N); }
  else if ( cmap=="Oranges"     ) { return Oranges    (N); }
  else if ( cmap=="Purples"     ) { return Purples    (N); }
  else if ( cmap=="Reds"        ) { return Reds       (N); }
  else if ( cmap=="BuPu"        ) { return BuPu       (N); }
  else if ( cmap=="GnBu"        ) { return GnBu       (N); }
  else if ( cmap=="PuBu"        ) { return PuBu       (N); }
  else if ( cmap=="PuBuGn"      ) { return PuBuGn     (N); }
  else if ( cmap=="PuRd"        ) { return PuRd       (N); }
  else if ( cmap=="RdPu"        ) { return RdPu       (N); }
  else if ( cmap=="OrRd"        ) { return OrRd       (N); }
  else if ( cmap=="RdOrYl"      ) { return RdOrYl     (N); }
  else if ( cmap=="YlGn"        ) { return YlGn       (N); }
  else if ( cmap=="YlGnBu"      ) { return YlGnBu     (N); }
  else if ( cmap=="YlOrRd"      ) { return YlOrRd     (N); }
  else if ( cmap=="BrBG"        ) { return BrBG       (N); }
  else if ( cmap=="PuOr"        ) { return PuOr       (N); }
  else if ( cmap=="RdBu"        ) { return RdBu       (N); }
  else if ( cmap=="RdGy"        ) { return RdGy       (N); }
  else if ( cmap=="RdYlBu"      ) { return RdYlBu     (N); }
  else if ( cmap=="RdYlGn"      ) { return RdYlGn     (N); }
  else if ( cmap=="PiYG"        ) { return PiYG       (N); }
  else if ( cmap=="PRGn"        ) { return PRGn       (N); }
  else if ( cmap=="Accent_r"    ) { return Accent_r   (N); }
  else if ( cmap=="Dark2_r"     ) { return Dark2_r    (N); }
  else if ( cmap=="Paired_r"    ) { return Paired_r   (N); }
  else if ( cmap=="Spectral_r"  ) { return Spectral_r (N); }
  else if ( cmap=="Pastel1_r"   ) { return Pastel1_r  (N); }
  else if ( cmap=="Pastel2_r"   ) { return Pastel2_r  (N); }
  else if ( cmap=="Set1_r"      ) { return Set1_r     (N); }
  else if ( cmap=="Set2_r"      ) { return Set2_r     (N); }
  else if ( cmap=="Set3_r"      ) { return Set3_r     (N); }
  else if ( cmap=="Blues_r"     ) { return Blues_r    (N); }
  else if ( cmap=="Greens_r"    ) { return Greens_r   (N); }
  else if ( cmap=="Greys_r"     ) { return Greys_r    (N); }
  else if ( cmap=="Oranges_r"   ) { return Oranges_r  (N); }
  else if ( cmap=="Purples_r"   ) { return Purples_r  (N); }
  else if ( cmap=="Reds_r"      ) { return Reds_r     (N); }
  else if ( cmap=="BuPu_r"      ) { return BuPu_r     (N); }
  else if ( cmap=="GnBu_r"      ) { return GnBu_r     (N); }
  else if ( cmap=="PuBu_r"      ) { return PuBu_r     (N); }
  else if ( cmap=="PuBuGn_r"    ) { return PuBuGn_r   (N); }
  else if ( cmap=="PuRd_r"      ) { return PuRd_r     (N); }
  else if ( cmap=="RdPu_r"      ) { return RdPu_r     (N); }
  else if ( cmap=="OrRd_r"      ) { return OrRd_r     (N); }
  else if ( cmap=="RdOrYl_r"    ) { return RdOrYl_r   (N); }
  else if ( cmap=="YlGn_r"      ) { return YlGn_r     (N); }
  else if ( cmap=="YlGnBu_r"    ) { return YlGnBu_r   (N); }
  else if ( cmap=="YlOrRd_r"    ) { return YlOrRd_r   (N); }
  else if ( cmap=="BrBG_r"      ) { return BrBG_r     (N); }
  else if ( cmap=="PuOr_r"      ) { return PuOr_r     (N); }
  else if ( cmap=="RdBu_r"      ) { return RdBu_r     (N); }
  else if ( cmap=="RdGy_r"      ) { return RdGy_r     (N); }
  else if ( cmap=="RdYlBu_r"    ) { return RdYlBu_r   (N); }
  else if ( cmap=="RdYlGn_r"    ) { return RdYlGn_r   (N); }
  else if ( cmap=="PiYG_r"      ) { return PiYG_r     (N); }
  else if ( cmap=="PRGn_r"      ) { return PRGn_r     (N); }
  else if ( cmap=="White"       ) { return White      (N); }
  else if ( cmap=="Grey"        ) { return Grey       (N); }
  else if ( cmap=="Black"       ) { return Black      (N); }
  else if ( cmap=="Red"         ) { return Red        (N); }
  else if ( cmap=="Blue"        ) { return Blue       (N); }

  throw std::runtime_error("Colormap not recognized");
}

// =================================================================================================
// color cycles
// =================================================================================================

inline cppmat::matrix<int> tue(void)
{
  cppmat::matrix<int> data({13,3});

  data( 0,0) = 247;   data( 0,1) =  49;   data( 0,2) =  49; //  0: warm red
  data( 1,0) = 214;   data( 1,1) =   0;   data( 1,2) =  74; //  1: red
  data( 2,0) = 214;   data( 2,1) =   0;   data( 2,2) = 123; //  2: pink
  data( 3,0) = 173;   data( 3,1) =  32;   data( 3,2) = 173; //  3: dark pink
  data( 4,0) =  16;   data( 4,1) =  16;   data( 4,2) = 115; //  4: dark blue
  data( 5,0) =   0;   data( 5,1) = 102;   data( 5,2) = 204; //  5: blue
  data( 6,0) =   0;   data( 6,1) = 162;   data( 6,2) = 222; //  6: light blue
  data( 7,0) = 255;   data( 7,1) = 154;   data( 7,2) =   0; //  7: orange
  data( 8,0) = 255;   data( 8,1) = 221;   data( 8,2) =   0; //  8: yellow
  data( 9,0) = 206;   data( 9,1) = 223;   data( 9,2) =   0; //  9: lemon
  data(10,0) = 132;   data(10,1) = 210;   data(10,2) =   0; // 10: lime
  data(11,0) =   0;   data(11,1) = 172;   data(11,2) = 130; // 11: green
  data(12,0) =   0;   data(12,1) = 146;   data(12,2) = 181; // 12: cornflower blue

  return data;
}

inline cppmat::matrix<int> tuewarmred(void)
{
  cppmat::matrix<int> data({1,3});

  data( 0,0) = 247;   data( 0,1) =  49;   data( 0,2) =  49;

  return data;
}

inline cppmat::matrix<int> tuedarkblue(void)
{
  cppmat::matrix<int> data({1,3});

  data( 0,0) =  16;   data( 0,1) =  16;   data( 0,2) = 115;

  return data;
}

inline cppmat::matrix<int> tueblue(void)
{
  cppmat::matrix<int> data({1,3});

  data( 0,0) =   0;   data( 0,1) = 102;   data( 0,2) = 204;

  return data;
}

inline cppmat::matrix<int> tuelightblue(void)
{
  cppmat::matrix<int> data({1,3});

  data( 0,0) =   0;   data( 0,1) = 162;   data( 0,2) = 222;

  return data;
}

inline cppmat::matrix<int> colorcycle(std::string cmap)
{
  if      ( cmap=="tue"          ) { return tue         (); }
  else if ( cmap=="tuewarmred"   ) { return tuewarmred  (); }
  else if ( cmap=="tuedarkblue"  ) { return tuedarkblue (); }
  else if ( cmap=="tueblue"      ) { return tueblue     (); }
  else if ( cmap=="tuelightblue" ) { return tuelightblue(); }

  throw std::runtime_error("Color-cycle not recognized");
}

} // namespace cppcolormap

#endif

