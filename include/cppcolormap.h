/* =================================================================================================

(c - GPLv3) T.W.J. de Geus (Tom) | tom@geus.me | www.geus.me | github.com/tdegeus/cppcolormap

================================================================================================= */

#ifndef CPPCOLORMAP_H
#define CPPCOLORMAP_H

#define CPPCOLORMAP_WORLD_VERSION 0
#define CPPCOLORMAP_MAJOR_VERSION 2
#define CPPCOLORMAP_MINOR_VERSION 0

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
#include <xtensor/xtensor.hpp>
#include <xtensor/xview.hpp>
#include <xtensor/xmath.hpp>
#include <xtensor/xio.hpp>

namespace cppcolormap {

// =================================================================================================
// support function: interpolate the individual colours
// =================================================================================================

inline xt::xtensor<int,2> interp(const xt::xtensor<int,2> &data , size_t N)
{
  // no interpolation needed
  if ( N == data.shape()[0] ) return data;

  // allocate output
  xt::xtensor<int,2> out = xt::empty<int>({N,data.shape()[1]});

  // set interpolation axes
  xt::xtensor<double,1> x  = xt::linspace(0.0, 1.0, data.shape()[0]);
  xt::xtensor<double,1> xi = xt::linspace(0.0, 1.0, N);

  // loop over colours
  for ( size_t j = 0 ; j < data.shape()[1] ; j++ )
  {
    // - 'view' of data
    auto c  = xt::view(data, xt::all(), j);
    auto ci = xt::view(out , xt::all(), j);
    // - interpolate
    ci = xt::interp(xi, x, c);
  }

  // return output
  return out;
}

// =================================================================================================
// qualitative colormaps
// =================================================================================================

inline xt::xtensor<int,2> Accent(size_t N=8)
{
  xt::xtensor<int,2> data = xt::empty<int>({8,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> Dark2(size_t N=8)
{
  xt::xtensor<int,2> data = xt::empty<int>({8,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> Paired(size_t N=12)
{
  xt::xtensor<int,2> data = xt::empty<int>({12,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> Spectral(size_t N=11)
{
  xt::xtensor<int,2> data = xt::empty<int>({11,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> Pastel1(size_t N=9)
{
  xt::xtensor<int,2> data = xt::empty<int>({9,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> Pastel2(size_t N=8)
{
  xt::xtensor<int,2> data = xt::empty<int>({8,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> Set1(size_t N=9)
{
  xt::xtensor<int,2> data = xt::empty<int>({9,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> Set2(size_t N=8)
{
  xt::xtensor<int,2> data = xt::empty<int>({8,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> Set3(size_t N=12)
{
  xt::xtensor<int,2> data = xt::empty<int>({12,3});

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

inline xt::xtensor<int,2> Blues(size_t N=9)
{
  xt::xtensor<int,2> data = xt::empty<int>({9,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> Greens(size_t N=9)
{
  xt::xtensor<int,2> data = xt::empty<int>({9,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> Greys(size_t N=2)
{
  xt::xtensor<int,2> data = xt::empty<int>({2,3});

  data( 0,0) =   0;   data( 0,1) =   0;   data( 0,2) =   0;
  data( 1,0) = 255;   data( 1,1) = 255;   data( 1,2) = 255;

  return interp(data,N);
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> Oranges(size_t N=9)
{
  xt::xtensor<int,2> data = xt::empty<int>({9,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> Purples(size_t N=9)
{
  xt::xtensor<int,2> data = xt::empty<int>({9,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> Reds(size_t N=9)
{
  xt::xtensor<int,2> data = xt::empty<int>({9,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> BuPu(size_t N=9)
{
  xt::xtensor<int,2> data = xt::empty<int>({9,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> GnBu(size_t N=9)
{
  xt::xtensor<int,2> data = xt::empty<int>({9,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> PuBu(size_t N=9)
{
  xt::xtensor<int,2> data = xt::empty<int>({9,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> PuBuGn(size_t N=9)
{
  xt::xtensor<int,2> data = xt::empty<int>({9,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> PuRd(size_t N=9)
{
  xt::xtensor<int,2> data = xt::empty<int>({9,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> RdPu(size_t N=9)
{
  xt::xtensor<int,2> data = xt::empty<int>({9,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> OrRd(size_t N=9)
{
  xt::xtensor<int,2> data = xt::empty<int>({9,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> RdOrYl(size_t N=9)
{
  xt::xtensor<int,2> data = xt::empty<int>({9,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> YlGn(size_t N=9)
{
  xt::xtensor<int,2> data = xt::empty<int>({9,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> YlGnBu(size_t N=9)
{
  xt::xtensor<int,2> data = xt::empty<int>({9,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> YlOrRd(size_t N=9)
{
  xt::xtensor<int,2> data = xt::empty<int>({9,3});

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

inline xt::xtensor<int,2> BrBG(size_t N=11)
{
  xt::xtensor<int,2> data = xt::empty<int>({11,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> PuOr(size_t N=11)
{
  xt::xtensor<int,2> data = xt::empty<int>({11,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> RdBu(size_t N=11)
{
  xt::xtensor<int,2> data = xt::empty<int>({11,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> RdGy(size_t N=11)
{
  xt::xtensor<int,2> data = xt::empty<int>({11,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> RdYlBu(size_t N=11)
{
  xt::xtensor<int,2> data = xt::empty<int>({11,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> RdYlGn(size_t N=11)
{
  xt::xtensor<int,2> data = xt::empty<int>({11,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> PiYG(size_t N=11)
{
  xt::xtensor<int,2> data = xt::empty<int>({11,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> PRGn(size_t N=11)
{
  xt::xtensor<int,2> data = xt::empty<int>({11,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> White(size_t N=1)
{
  xt::xtensor<int,2> data = xt::empty<int>({1,3});

  data( 0,0) = 255;   data( 0,1) = 255;   data( 0,2) = 255;

  return interp(data,N);
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> Grey(size_t N=1)
{
  xt::xtensor<int,2> data = xt::empty<int>({1,3});

  data( 0,0) = 127;   data( 0,1) = 127;   data( 0,2) = 127;

  return interp(data,N);
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> Black(size_t N=1)
{
  xt::xtensor<int,2> data = xt::empty<int>({1,3});

  data( 0,0) =   0;   data( 0,1) =   0;   data( 0,2) =   0;

  return interp(data,N);
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> Red(size_t N=1)
{
  xt::xtensor<int,2> data = xt::empty<int>({1,3});

  data( 0,0) = 255;   data( 0,1) =   0;   data( 0,2) =   0;

  return interp(data,N);
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> Blue(size_t N=1)
{
  xt::xtensor<int,2> data = xt::empty<int>({1,3});

  data( 0,0) =   0;   data( 0,1) =   0;   data( 0,2) = 255;

  return interp(data,N);
}

// =================================================================================================
// inverse colormaps
// =================================================================================================

inline xt::xtensor<int,2> Accent_r   (size_t N= 8) { return xt::flip(Accent  (N), 0); }
inline xt::xtensor<int,2> Dark2_r    (size_t N= 8) { return xt::flip(Dark2   (N), 0); }
inline xt::xtensor<int,2> Paired_r   (size_t N=12) { return xt::flip(Paired  (N), 0); }
inline xt::xtensor<int,2> Spectral_r (size_t N=11) { return xt::flip(Spectral(N), 0); }
inline xt::xtensor<int,2> Pastel1_r  (size_t N= 9) { return xt::flip(Pastel1 (N), 0); }
inline xt::xtensor<int,2> Pastel2_r  (size_t N= 8) { return xt::flip(Pastel2 (N), 0); }
inline xt::xtensor<int,2> Set1_r     (size_t N= 9) { return xt::flip(Set1    (N), 0); }
inline xt::xtensor<int,2> Set2_r     (size_t N= 8) { return xt::flip(Set2    (N), 0); }
inline xt::xtensor<int,2> Set3_r     (size_t N=12) { return xt::flip(Set3    (N), 0); }
inline xt::xtensor<int,2> Blues_r    (size_t N= 9) { return xt::flip(Blues   (N), 0); }
inline xt::xtensor<int,2> Greens_r   (size_t N= 9) { return xt::flip(Greens  (N), 0); }
inline xt::xtensor<int,2> Greys_r    (size_t N= 2) { return xt::flip(Greys   (N), 0); }
inline xt::xtensor<int,2> Oranges_r  (size_t N= 9) { return xt::flip(Oranges (N), 0); }
inline xt::xtensor<int,2> Purples_r  (size_t N= 9) { return xt::flip(Purples (N), 0); }
inline xt::xtensor<int,2> Reds_r     (size_t N= 9) { return xt::flip(Reds    (N), 0); }
inline xt::xtensor<int,2> BuPu_r     (size_t N= 9) { return xt::flip(BuPu    (N), 0); }
inline xt::xtensor<int,2> GnBu_r     (size_t N= 9) { return xt::flip(GnBu    (N), 0); }
inline xt::xtensor<int,2> PuBu_r     (size_t N= 9) { return xt::flip(PuBu    (N), 0); }
inline xt::xtensor<int,2> PuBuGn_r   (size_t N= 9) { return xt::flip(PuBuGn  (N), 0); }
inline xt::xtensor<int,2> PuRd_r     (size_t N= 9) { return xt::flip(PuRd    (N), 0); }
inline xt::xtensor<int,2> RdPu_r     (size_t N= 9) { return xt::flip(RdPu    (N), 0); }
inline xt::xtensor<int,2> OrRd_r     (size_t N= 9) { return xt::flip(OrRd    (N), 0); }
inline xt::xtensor<int,2> RdOrYl_r   (size_t N= 9) { return xt::flip(RdOrYl  (N), 0); }
inline xt::xtensor<int,2> YlGn_r     (size_t N= 9) { return xt::flip(YlGn    (N), 0); }
inline xt::xtensor<int,2> YlGnBu_r   (size_t N= 9) { return xt::flip(YlGnBu  (N), 0); }
inline xt::xtensor<int,2> YlOrRd_r   (size_t N= 9) { return xt::flip(YlOrRd  (N), 0); }
inline xt::xtensor<int,2> BrBG_r     (size_t N=11) { return xt::flip(BrBG    (N), 0); }
inline xt::xtensor<int,2> PuOr_r     (size_t N=11) { return xt::flip(PuOr    (N), 0); }
inline xt::xtensor<int,2> RdBu_r     (size_t N=11) { return xt::flip(RdBu    (N), 0); }
inline xt::xtensor<int,2> RdGy_r     (size_t N=11) { return xt::flip(RdGy    (N), 0); }
inline xt::xtensor<int,2> RdYlBu_r   (size_t N=11) { return xt::flip(RdYlBu  (N), 0); }
inline xt::xtensor<int,2> RdYlGn_r   (size_t N=11) { return xt::flip(RdYlGn  (N), 0); }
inline xt::xtensor<int,2> PiYG_r     (size_t N=11) { return xt::flip(PiYG    (N), 0); }
inline xt::xtensor<int,2> PRGn_r     (size_t N=11) { return xt::flip(PRGn    (N), 0); }

// =================================================================================================
// read from string
// =================================================================================================

inline xt::xtensor<int,2> colormap(const std::string &cmap, size_t N=256)
{
  if ( cmap == "Accent"      ) { return Accent     (N); }
  if ( cmap == "Dark2"       ) { return Dark2      (N); }
  if ( cmap == "Paired"      ) { return Paired     (N); }
  if ( cmap == "Spectral"    ) { return Spectral   (N); }
  if ( cmap == "Pastel1"     ) { return Pastel1    (N); }
  if ( cmap == "Pastel2"     ) { return Pastel2    (N); }
  if ( cmap == "Set1"        ) { return Set1       (N); }
  if ( cmap == "Set2"        ) { return Set2       (N); }
  if ( cmap == "Set3"        ) { return Set3       (N); }
  if ( cmap == "Blues"       ) { return Blues      (N); }
  if ( cmap == "Greens"      ) { return Greens     (N); }
  if ( cmap == "Greys"       ) { return Greys      (N); }
  if ( cmap == "Oranges"     ) { return Oranges    (N); }
  if ( cmap == "Purples"     ) { return Purples    (N); }
  if ( cmap == "Reds"        ) { return Reds       (N); }
  if ( cmap == "BuPu"        ) { return BuPu       (N); }
  if ( cmap == "GnBu"        ) { return GnBu       (N); }
  if ( cmap == "PuBu"        ) { return PuBu       (N); }
  if ( cmap == "PuBuGn"      ) { return PuBuGn     (N); }
  if ( cmap == "PuRd"        ) { return PuRd       (N); }
  if ( cmap == "RdPu"        ) { return RdPu       (N); }
  if ( cmap == "OrRd"        ) { return OrRd       (N); }
  if ( cmap == "RdOrYl"      ) { return RdOrYl     (N); }
  if ( cmap == "YlGn"        ) { return YlGn       (N); }
  if ( cmap == "YlGnBu"      ) { return YlGnBu     (N); }
  if ( cmap == "YlOrRd"      ) { return YlOrRd     (N); }
  if ( cmap == "BrBG"        ) { return BrBG       (N); }
  if ( cmap == "PuOr"        ) { return PuOr       (N); }
  if ( cmap == "RdBu"        ) { return RdBu       (N); }
  if ( cmap == "RdGy"        ) { return RdGy       (N); }
  if ( cmap == "RdYlBu"      ) { return RdYlBu     (N); }
  if ( cmap == "RdYlGn"      ) { return RdYlGn     (N); }
  if ( cmap == "PiYG"        ) { return PiYG       (N); }
  if ( cmap == "PRGn"        ) { return PRGn       (N); }
  if ( cmap == "Accent_r"    ) { return Accent_r   (N); }
  if ( cmap == "Dark2_r"     ) { return Dark2_r    (N); }
  if ( cmap == "Paired_r"    ) { return Paired_r   (N); }
  if ( cmap == "Spectral_r"  ) { return Spectral_r (N); }
  if ( cmap == "Pastel1_r"   ) { return Pastel1_r  (N); }
  if ( cmap == "Pastel2_r"   ) { return Pastel2_r  (N); }
  if ( cmap == "Set1_r"      ) { return Set1_r     (N); }
  if ( cmap == "Set2_r"      ) { return Set2_r     (N); }
  if ( cmap == "Set3_r"      ) { return Set3_r     (N); }
  if ( cmap == "Blues_r"     ) { return Blues_r    (N); }
  if ( cmap == "Greens_r"    ) { return Greens_r   (N); }
  if ( cmap == "Greys_r"     ) { return Greys_r    (N); }
  if ( cmap == "Oranges_r"   ) { return Oranges_r  (N); }
  if ( cmap == "Purples_r"   ) { return Purples_r  (N); }
  if ( cmap == "Reds_r"      ) { return Reds_r     (N); }
  if ( cmap == "BuPu_r"      ) { return BuPu_r     (N); }
  if ( cmap == "GnBu_r"      ) { return GnBu_r     (N); }
  if ( cmap == "PuBu_r"      ) { return PuBu_r     (N); }
  if ( cmap == "PuBuGn_r"    ) { return PuBuGn_r   (N); }
  if ( cmap == "PuRd_r"      ) { return PuRd_r     (N); }
  if ( cmap == "RdPu_r"      ) { return RdPu_r     (N); }
  if ( cmap == "OrRd_r"      ) { return OrRd_r     (N); }
  if ( cmap == "RdOrYl_r"    ) { return RdOrYl_r   (N); }
  if ( cmap == "YlGn_r"      ) { return YlGn_r     (N); }
  if ( cmap == "YlGnBu_r"    ) { return YlGnBu_r   (N); }
  if ( cmap == "YlOrRd_r"    ) { return YlOrRd_r   (N); }
  if ( cmap == "BrBG_r"      ) { return BrBG_r     (N); }
  if ( cmap == "PuOr_r"      ) { return PuOr_r     (N); }
  if ( cmap == "RdBu_r"      ) { return RdBu_r     (N); }
  if ( cmap == "RdGy_r"      ) { return RdGy_r     (N); }
  if ( cmap == "RdYlBu_r"    ) { return RdYlBu_r   (N); }
  if ( cmap == "RdYlGn_r"    ) { return RdYlGn_r   (N); }
  if ( cmap == "PiYG_r"      ) { return PiYG_r     (N); }
  if ( cmap == "PRGn_r"      ) { return PRGn_r     (N); }
  if ( cmap == "White"       ) { return White      (N); }
  if ( cmap == "Grey"        ) { return Grey       (N); }
  if ( cmap == "Black"       ) { return Black      (N); }
  if ( cmap == "Red"         ) { return Red        (N); }
  if ( cmap == "Blue"        ) { return Blue       (N); }

  throw std::runtime_error("Colormap not recognized");
}

// =================================================================================================
// color cycles
// =================================================================================================

inline xt::xtensor<int,2> tue()
{
  xt::xtensor<int,2> data = xt::empty<int>({13,3});

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

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> tuewarmred()
{
  xt::xtensor<int,2> data = xt::empty<int>({1,3});

  data( 0,0) = 247;   data( 0,1) =  49;   data( 0,2) =  49;

  return data;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> tuedarkblue()
{
  xt::xtensor<int,2> data = xt::empty<int>({1,3});

  data( 0,0) =  16;   data( 0,1) =  16;   data( 0,2) = 115;

  return data;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> tueblue()
{
  xt::xtensor<int,2> data = xt::empty<int>({1,3});

  data( 0,0) =   0;   data( 0,1) = 102;   data( 0,2) = 204;

  return data;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> tuelightblue()
{
  xt::xtensor<int,2> data = xt::empty<int>({1,3});

  data( 0,0) =   0;   data( 0,1) = 162;   data( 0,2) = 222;

  return data;
}

// -------------------------------------------------------------------------------------------------

inline xt::xtensor<int,2> colorcycle(const std::string &cmap)
{
  if ( cmap == "tue"          ) { return tue         (); }
  if ( cmap == "tuewarmred"   ) { return tuewarmred  (); }
  if ( cmap == "tuedarkblue"  ) { return tuedarkblue (); }
  if ( cmap == "tueblue"      ) { return tueblue     (); }
  if ( cmap == "tuelightblue" ) { return tuelightblue(); }

  throw std::runtime_error("Color-cycle not recognized");
}

// =================================================================================================

} // namespace cppcolormap

#endif

