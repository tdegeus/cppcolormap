
#ifndef CPPCOLORMAP_H
#define CPPCOLORMAP_H

#include <vector>
#include <string>
#include <cfloat>
#include <math.h>

namespace cppcolormap {

template<typename T>
inline std::vector<float> linspace(T start_in, T end_in, size_t num_in)
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

inline size_t findNearestNeighbourIndex( float value, std::vector<float> &x )
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

inline std::vector<float> interp1( std::vector<float> &x, std::vector<float> &y, std::vector<float> &x_new )
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

inline std::vector<int> interp ( std::vector<int> data , size_t N )
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

inline std::vector<int> reverse ( std::vector<int> in )
{
  std::vector<int> out(in.size());
  size_t N = in.size()/3;

  for ( size_t i=0 ; i<N ; i++ )
    for ( size_t j=0 ; j<3 ; j++ )
      out[i*3+j] = in[(N-i-1)*3+j];

  return out;
};

// =============================================================================
// qualitative colormaps
// =============================================================================

inline std::vector<int> Accent ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(127); data.push_back(201); data.push_back(127);
  data.push_back(190); data.push_back(174); data.push_back(212);
  data.push_back(253); data.push_back(192); data.push_back(134);
  data.push_back(255); data.push_back(255); data.push_back(153);
  data.push_back( 56); data.push_back(108); data.push_back(176);
  data.push_back(240); data.push_back(  2); data.push_back(127);
  data.push_back(191); data.push_back( 91); data.push_back( 23);
  data.push_back(102); data.push_back(102); data.push_back(102);
  return interp(data,N);
};

inline std::vector<int> Dark2 ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back( 27); data.push_back(158); data.push_back(119);
  data.push_back(217); data.push_back( 95); data.push_back(  2);
  data.push_back(117); data.push_back(112); data.push_back(179);
  data.push_back(231); data.push_back( 41); data.push_back(138);
  data.push_back(102); data.push_back(166); data.push_back( 30);
  data.push_back(230); data.push_back(171); data.push_back(  2);
  data.push_back(166); data.push_back(118); data.push_back( 29);
  data.push_back(102); data.push_back(102); data.push_back(102);
  return interp(data,N);
};

inline std::vector<int> Paired ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(166); data.push_back(206); data.push_back(227);
  data.push_back( 31); data.push_back(120); data.push_back(180);
  data.push_back(178); data.push_back(223); data.push_back(138);
  data.push_back( 51); data.push_back(160); data.push_back( 44);
  data.push_back(251); data.push_back(154); data.push_back(153);
  data.push_back(227); data.push_back( 26); data.push_back( 28);
  data.push_back(253); data.push_back(191); data.push_back(111);
  data.push_back(255); data.push_back(127); data.push_back(  0);
  data.push_back(202); data.push_back(178); data.push_back(214);
  data.push_back(106); data.push_back( 61); data.push_back(154);
  data.push_back(255); data.push_back(255); data.push_back(153);
  data.push_back(177); data.push_back( 89); data.push_back( 40);
  return interp(data,N);
};

inline std::vector<int> Spectral ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(158); data.push_back(  1); data.push_back( 66);
  data.push_back(213); data.push_back( 62); data.push_back( 79);
  data.push_back(244); data.push_back(109); data.push_back( 67);
  data.push_back(253); data.push_back(174); data.push_back( 97);
  data.push_back(254); data.push_back(224); data.push_back(139);
  data.push_back(255); data.push_back(255); data.push_back(191);
  data.push_back(230); data.push_back(245); data.push_back(152);
  data.push_back(171); data.push_back(221); data.push_back(164);
  data.push_back(102); data.push_back(194); data.push_back(165);
  data.push_back( 50); data.push_back(136); data.push_back(189);
  data.push_back( 94); data.push_back( 79); data.push_back(162);
  return interp(data,N);
};

inline std::vector<int> Pastel1 ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(251); data.push_back(180); data.push_back(174);
  data.push_back(179); data.push_back(205); data.push_back(227);
  data.push_back(204); data.push_back(235); data.push_back(197);
  data.push_back(222); data.push_back(203); data.push_back(228);
  data.push_back(254); data.push_back(217); data.push_back(166);
  data.push_back(255); data.push_back(255); data.push_back(204);
  data.push_back(229); data.push_back(216); data.push_back(189);
  data.push_back(253); data.push_back(218); data.push_back(236);
  data.push_back(242); data.push_back(242); data.push_back(242);
  return interp(data,N);
};

inline std::vector<int> Pastel2 ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(179); data.push_back(226); data.push_back(205);
  data.push_back(253); data.push_back(205); data.push_back(172);
  data.push_back(203); data.push_back(213); data.push_back(232);
  data.push_back(244); data.push_back(202); data.push_back(228);
  data.push_back(230); data.push_back(245); data.push_back(201);
  data.push_back(255); data.push_back(242); data.push_back(174);
  data.push_back(241); data.push_back(226); data.push_back(204);
  data.push_back(204); data.push_back(204); data.push_back(204);
  return interp(data,N);
};

inline std::vector<int> Set1 ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(228); data.push_back( 26); data.push_back( 28);
  data.push_back( 55); data.push_back(126); data.push_back(184);
  data.push_back( 77); data.push_back(175); data.push_back( 74);
  data.push_back(152); data.push_back( 78); data.push_back(163);
  data.push_back(255); data.push_back(127); data.push_back(  0);
  data.push_back(255); data.push_back(255); data.push_back( 51);
  data.push_back(166); data.push_back( 86); data.push_back( 40);
  data.push_back(247); data.push_back(129); data.push_back(191);
  data.push_back(153); data.push_back(153); data.push_back(153);
  return interp(data,N);
};

inline std::vector<int> Set2 ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(102); data.push_back(194); data.push_back(165);
  data.push_back(252); data.push_back(141); data.push_back( 98);
  data.push_back(141); data.push_back(160); data.push_back(203);
  data.push_back(231); data.push_back(138); data.push_back(195);
  data.push_back(166); data.push_back(216); data.push_back( 84);
  data.push_back(255); data.push_back(217); data.push_back( 47);
  data.push_back(229); data.push_back(196); data.push_back(148);
  data.push_back(179); data.push_back(179); data.push_back(179);
  return interp(data,N);
};

inline std::vector<int> Set3 ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(141); data.push_back(211); data.push_back(199);
  data.push_back(255); data.push_back(255); data.push_back(179);
  data.push_back(190); data.push_back(186); data.push_back(218);
  data.push_back(251); data.push_back(128); data.push_back(114);
  data.push_back(128); data.push_back(177); data.push_back(211);
  data.push_back(253); data.push_back(180); data.push_back( 98);
  data.push_back(179); data.push_back(222); data.push_back(105);
  data.push_back(252); data.push_back(205); data.push_back(229);
  data.push_back(217); data.push_back(217); data.push_back(217);
  data.push_back(188); data.push_back(128); data.push_back(189);
  data.push_back(204); data.push_back(235); data.push_back(197);
  data.push_back(255); data.push_back(237); data.push_back(111);
  return interp(data,N);
};

// =============================================================================
// sequential colormaps
// =============================================================================

inline std::vector<int> Blues ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(247); data.push_back(251); data.push_back(255);
  data.push_back(222); data.push_back(235); data.push_back(247);
  data.push_back(198); data.push_back(219); data.push_back(239);
  data.push_back(158); data.push_back(202); data.push_back(225);
  data.push_back(107); data.push_back(174); data.push_back(214);
  data.push_back( 66); data.push_back(146); data.push_back(198);
  data.push_back( 33); data.push_back(113); data.push_back(181);
  data.push_back(  8); data.push_back( 81); data.push_back(156);
  data.push_back(  8); data.push_back( 48); data.push_back(107);
  return interp(data,N);
};

inline std::vector<int> Greens ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(247); data.push_back(252); data.push_back(245);
  data.push_back(229); data.push_back(245); data.push_back(224);
  data.push_back(199); data.push_back(233); data.push_back(192);
  data.push_back(161); data.push_back(217); data.push_back(155);
  data.push_back(116); data.push_back(196); data.push_back(118);
  data.push_back( 65); data.push_back(171); data.push_back( 93);
  data.push_back( 35); data.push_back(139); data.push_back( 69);
  data.push_back(  0); data.push_back(109); data.push_back( 44);
  data.push_back(  0); data.push_back( 68); data.push_back( 27);
  return interp(data,N);
};

inline std::vector<int> Greys ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(  0); data.push_back(  0); data.push_back(  0);
  data.push_back(255); data.push_back(255); data.push_back(255);
  return interp(data,N);
};

inline std::vector<int> Oranges ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(255); data.push_back(245); data.push_back(235);
  data.push_back(254); data.push_back(230); data.push_back(206);
  data.push_back(253); data.push_back(208); data.push_back(162);
  data.push_back(253); data.push_back(174); data.push_back(107);
  data.push_back(253); data.push_back(141); data.push_back( 60);
  data.push_back(241); data.push_back(105); data.push_back( 19);
  data.push_back(217); data.push_back( 72); data.push_back(  1);
  data.push_back(166); data.push_back( 54); data.push_back(  3);
  data.push_back(127); data.push_back( 39); data.push_back(  4);
  return interp(data,N);
};

inline std::vector<int> Purples ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(252); data.push_back(251); data.push_back(253);
  data.push_back(239); data.push_back(237); data.push_back(245);
  data.push_back(218); data.push_back(218); data.push_back(235);
  data.push_back(188); data.push_back(189); data.push_back(220);
  data.push_back(158); data.push_back(154); data.push_back(200);
  data.push_back(128); data.push_back(125); data.push_back(186);
  data.push_back(106); data.push_back( 81); data.push_back(163);
  data.push_back( 84); data.push_back( 39); data.push_back(143);
  data.push_back( 63); data.push_back(  0); data.push_back(125);
  return interp(data,N);
};

inline std::vector<int> Reds ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(255); data.push_back(245); data.push_back(240);
  data.push_back(254); data.push_back(224); data.push_back(210);
  data.push_back(252); data.push_back(187); data.push_back(161);
  data.push_back(252); data.push_back(146); data.push_back(114);
  data.push_back(251); data.push_back(106); data.push_back( 74);
  data.push_back(239); data.push_back( 59); data.push_back( 44);
  data.push_back(203); data.push_back( 24); data.push_back( 29);
  data.push_back(165); data.push_back( 15); data.push_back( 21);
  data.push_back(103); data.push_back(  0); data.push_back( 13);
  return interp(data,N);
};

inline std::vector<int> BuPu ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(247); data.push_back(252); data.push_back(253);
  data.push_back(224); data.push_back(236); data.push_back(244);
  data.push_back(191); data.push_back(211); data.push_back(230);
  data.push_back(158); data.push_back(188); data.push_back(218);
  data.push_back(140); data.push_back(150); data.push_back(198);
  data.push_back(140); data.push_back(107); data.push_back(177);
  data.push_back(136); data.push_back( 65); data.push_back(157);
  data.push_back(129); data.push_back( 15); data.push_back(124);
  data.push_back( 77); data.push_back(  0); data.push_back( 75);
  return interp(data,N);
};

inline std::vector<int> GnBu ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(247); data.push_back(252); data.push_back(240);
  data.push_back(224); data.push_back(243); data.push_back(219);
  data.push_back(204); data.push_back(235); data.push_back(197);
  data.push_back(168); data.push_back(221); data.push_back(181);
  data.push_back(123); data.push_back(204); data.push_back(196);
  data.push_back( 78); data.push_back(179); data.push_back(211);
  data.push_back( 43); data.push_back(140); data.push_back(190);
  data.push_back(  8); data.push_back(104); data.push_back(172);
  data.push_back(  8); data.push_back( 64); data.push_back(129);
  return interp(data,N);
};

inline std::vector<int> PuBu ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(255); data.push_back(247); data.push_back(251);
  data.push_back(236); data.push_back(231); data.push_back(242);
  data.push_back(208); data.push_back(209); data.push_back(230);
  data.push_back(166); data.push_back(189); data.push_back(219);
  data.push_back(116); data.push_back(169); data.push_back(207);
  data.push_back( 54); data.push_back(144); data.push_back(192);
  data.push_back(  5); data.push_back(112); data.push_back(176);
  data.push_back(  4); data.push_back( 90); data.push_back(141);
  data.push_back(  2); data.push_back( 56); data.push_back( 88);
  return interp(data,N);
};

inline std::vector<int> PuBuGn ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(255); data.push_back(247); data.push_back(251);
  data.push_back(236); data.push_back(226); data.push_back(240);
  data.push_back(208); data.push_back(209); data.push_back(230);
  data.push_back(166); data.push_back(189); data.push_back(219);
  data.push_back(103); data.push_back(169); data.push_back(207);
  data.push_back( 54); data.push_back(144); data.push_back(192);
  data.push_back(  2); data.push_back(129); data.push_back(138);
  data.push_back(  1); data.push_back(108); data.push_back( 89);
  data.push_back(  1); data.push_back( 70); data.push_back( 54);
  return interp(data,N);
};

inline std::vector<int> PuRd ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(247); data.push_back(244); data.push_back(249);
  data.push_back(231); data.push_back(225); data.push_back(239);
  data.push_back(212); data.push_back(185); data.push_back(218);
  data.push_back(201); data.push_back(148); data.push_back(199);
  data.push_back(223); data.push_back(101); data.push_back(176);
  data.push_back(231); data.push_back( 41); data.push_back(138);
  data.push_back(206); data.push_back( 18); data.push_back( 86);
  data.push_back(152); data.push_back(  0); data.push_back( 67);
  data.push_back(103); data.push_back(  0); data.push_back( 31);
  return interp(data,N);
};

inline std::vector<int> RdPu ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(255); data.push_back(247); data.push_back(243);
  data.push_back(253); data.push_back(224); data.push_back(221);
  data.push_back(252); data.push_back(197); data.push_back(192);
  data.push_back(250); data.push_back(159); data.push_back(181);
  data.push_back(247); data.push_back(104); data.push_back(161);
  data.push_back(221); data.push_back( 52); data.push_back(151);
  data.push_back(174); data.push_back(  1); data.push_back(126);
  data.push_back(122); data.push_back(  1); data.push_back(119);
  data.push_back( 73); data.push_back(  0); data.push_back(106);
  return interp(data,N);
};

inline std::vector<int> OrRd ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(255); data.push_back(247); data.push_back(236);
  data.push_back(254); data.push_back(232); data.push_back(200);
  data.push_back(253); data.push_back(212); data.push_back(158);
  data.push_back(253); data.push_back(187); data.push_back(132);
  data.push_back(252); data.push_back(141); data.push_back( 89);
  data.push_back(239); data.push_back(101); data.push_back( 72);
  data.push_back(215); data.push_back( 48); data.push_back( 31);
  data.push_back(179); data.push_back(  0); data.push_back(  0);
  data.push_back(127); data.push_back(  0); data.push_back(  0);
  return interp(data,N);
};

inline std::vector<int> RdOrYl ( size_t N=256 )
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

inline std::vector<int> YlGn ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(255); data.push_back(255); data.push_back(229);
  data.push_back(247); data.push_back(252); data.push_back(185);
  data.push_back(217); data.push_back(240); data.push_back(163);
  data.push_back(173); data.push_back(221); data.push_back(142);
  data.push_back(120); data.push_back(198); data.push_back(121);
  data.push_back( 65); data.push_back(171); data.push_back( 93);
  data.push_back( 35); data.push_back(132); data.push_back( 67);
  data.push_back(  0); data.push_back(104); data.push_back( 55);
  data.push_back(  0); data.push_back( 69); data.push_back( 41);
  return interp(data,N);
};

inline std::vector<int> YlGnBu ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(255); data.push_back(255); data.push_back(217);
  data.push_back(237); data.push_back(248); data.push_back(177);
  data.push_back(199); data.push_back(233); data.push_back(180);
  data.push_back(127); data.push_back(205); data.push_back(187);
  data.push_back( 65); data.push_back(182); data.push_back(196);
  data.push_back( 29); data.push_back(145); data.push_back(192);
  data.push_back( 34); data.push_back( 94); data.push_back(168);
  data.push_back( 37); data.push_back( 52); data.push_back(148);
  data.push_back(  8); data.push_back( 29); data.push_back( 88);
  return interp(data,N);
};

inline std::vector<int> YlOrRd ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(255); data.push_back(255); data.push_back(204);
  data.push_back(255); data.push_back(237); data.push_back(160);
  data.push_back(254); data.push_back(217); data.push_back(118);
  data.push_back(254); data.push_back(178); data.push_back( 76);
  data.push_back(253); data.push_back(141); data.push_back( 60);
  data.push_back(252); data.push_back( 78); data.push_back( 42);
  data.push_back(227); data.push_back( 26); data.push_back( 28);
  data.push_back(189); data.push_back(  0); data.push_back( 38);
  data.push_back(128); data.push_back(  0); data.push_back( 38);
  return interp(data,N);
};

// =============================================================================
// diverging colormaps
// =============================================================================

inline std::vector<int> BrBG ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back( 84); data.push_back( 48); data.push_back(  5);
  data.push_back(140); data.push_back( 81); data.push_back( 10);
  data.push_back(191); data.push_back(129); data.push_back( 45);
  data.push_back(223); data.push_back(194); data.push_back(125);
  data.push_back(246); data.push_back(232); data.push_back(195);
  data.push_back(245); data.push_back(245); data.push_back(245);
  data.push_back(199); data.push_back(234); data.push_back(229);
  data.push_back(128); data.push_back(205); data.push_back(193);
  data.push_back( 53); data.push_back(151); data.push_back(143);
  data.push_back(  1); data.push_back(102); data.push_back( 94);
  data.push_back(  0); data.push_back( 60); data.push_back( 48);
  return interp(data,N);
};

inline std::vector<int> PuOr ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(127); data.push_back( 59); data.push_back(  8);
  data.push_back(179); data.push_back( 88); data.push_back(  6);
  data.push_back(224); data.push_back(130); data.push_back( 20);
  data.push_back(253); data.push_back(184); data.push_back( 99);
  data.push_back(254); data.push_back(224); data.push_back(182);
  data.push_back(247); data.push_back(247); data.push_back(247);
  data.push_back(216); data.push_back(218); data.push_back(235);
  data.push_back(178); data.push_back(171); data.push_back(210);
  data.push_back(128); data.push_back(115); data.push_back(172);
  data.push_back( 84); data.push_back( 39); data.push_back(136);
  data.push_back( 45); data.push_back(  0); data.push_back( 75);
  return interp(data,N);
};

inline std::vector<int> RdBu ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(103); data.push_back(  0); data.push_back( 31);
  data.push_back(178); data.push_back( 24); data.push_back( 43);
  data.push_back(214); data.push_back( 96); data.push_back( 77);
  data.push_back(244); data.push_back(165); data.push_back(130);
  data.push_back(253); data.push_back(219); data.push_back(199);
  data.push_back(247); data.push_back(247); data.push_back(247);
  data.push_back(209); data.push_back(229); data.push_back(240);
  data.push_back(146); data.push_back(197); data.push_back(222);
  data.push_back( 67); data.push_back(147); data.push_back(195);
  data.push_back( 33); data.push_back(102); data.push_back(172);
  data.push_back(  5); data.push_back( 48); data.push_back( 97);
  return interp(data,N);
};

inline std::vector<int> RdGy ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(103); data.push_back(  0); data.push_back( 31);
  data.push_back(178); data.push_back( 24); data.push_back( 43);
  data.push_back(214); data.push_back( 96); data.push_back( 77);
  data.push_back(244); data.push_back(165); data.push_back(130);
  data.push_back(253); data.push_back(219); data.push_back(199);
  data.push_back(255); data.push_back(255); data.push_back(255);
  data.push_back(224); data.push_back(224); data.push_back(224);
  data.push_back(186); data.push_back(186); data.push_back(186);
  data.push_back(135); data.push_back(135); data.push_back(135);
  data.push_back( 77); data.push_back( 77); data.push_back( 77);
  data.push_back( 26); data.push_back( 26); data.push_back( 26);
  return interp(data,N);
};

inline std::vector<int> RdYlBu ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(165); data.push_back(  0); data.push_back( 38);
  data.push_back(215); data.push_back( 48); data.push_back( 39);
  data.push_back(244); data.push_back(109); data.push_back( 67);
  data.push_back(253); data.push_back(174); data.push_back( 97);
  data.push_back(254); data.push_back(224); data.push_back(144);
  data.push_back(255); data.push_back(255); data.push_back(191);
  data.push_back(224); data.push_back(243); data.push_back(248);
  data.push_back(171); data.push_back(217); data.push_back(233);
  data.push_back(116); data.push_back(173); data.push_back(209);
  data.push_back( 69); data.push_back(117); data.push_back(180);
  data.push_back( 49); data.push_back( 54); data.push_back(149);
  return interp(data,N);
};

inline std::vector<int> RdYlGn ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(165); data.push_back(  0); data.push_back( 38);
  data.push_back(215); data.push_back( 48); data.push_back( 39);
  data.push_back(244); data.push_back(109); data.push_back( 67);
  data.push_back(253); data.push_back(174); data.push_back( 97);
  data.push_back(254); data.push_back(224); data.push_back(139);
  data.push_back(255); data.push_back(255); data.push_back(191);
  data.push_back(217); data.push_back(239); data.push_back(139);
  data.push_back(166); data.push_back(217); data.push_back(106);
  data.push_back(102); data.push_back(189); data.push_back( 99);
  data.push_back( 26); data.push_back(152); data.push_back( 80);
  data.push_back(  0); data.push_back(104); data.push_back( 55);
  return interp(data,N);
};

inline std::vector<int> PiYG ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back(142); data.push_back(  1); data.push_back( 82);
  data.push_back(197); data.push_back( 27); data.push_back(125);
  data.push_back(222); data.push_back(119); data.push_back(174);
  data.push_back(241); data.push_back(182); data.push_back(218);
  data.push_back(253); data.push_back(224); data.push_back(239);
  data.push_back(247); data.push_back(247); data.push_back(247);
  data.push_back(230); data.push_back(245); data.push_back(208);
  data.push_back(184); data.push_back(225); data.push_back(134);
  data.push_back(127); data.push_back(188); data.push_back( 65);
  data.push_back( 77); data.push_back(146); data.push_back( 33);
  data.push_back( 39); data.push_back(100); data.push_back( 25);
  return interp(data,N);
};

inline std::vector<int> PRGn ( size_t N=256 )
{
  std::vector<int> data;
  data.push_back( 64); data.push_back(  0); data.push_back( 75);
  data.push_back(118); data.push_back( 42); data.push_back(131);
  data.push_back(153); data.push_back(112); data.push_back(171);
  data.push_back(194); data.push_back(165); data.push_back(207);
  data.push_back(231); data.push_back(212); data.push_back(232);
  data.push_back(247); data.push_back(247); data.push_back(247);
  data.push_back(217); data.push_back(240); data.push_back(211);
  data.push_back(166); data.push_back(219); data.push_back(160);
  data.push_back( 90); data.push_back(174); data.push_back( 97);
  data.push_back( 27); data.push_back(120); data.push_back( 55);
  data.push_back(  0); data.push_back( 68); data.push_back( 27);
  return interp(data,N);
};

// =============================================================================
// monochromatic colormaps
// =============================================================================

inline std::vector<int> White ( size_t N=1 )
{
  std::vector<int> data;
  data.push_back(255); data.push_back(255); data.push_back(255);
  return interp(data,N);
};

inline std::vector<int> Grey ( size_t N=1 )
{
  std::vector<int> data;
  data.push_back(127); data.push_back(127); data.push_back(127);
  return interp(data,N);
};

inline std::vector<int> Black ( size_t N=1 )
{
  std::vector<int> data;
  data.push_back(  0); data.push_back(  0); data.push_back(  0);
  return interp(data,N);
};

inline std::vector<int> Red ( size_t N=1 )
{
  std::vector<int> data;
  data.push_back(255); data.push_back(  0); data.push_back(  0);
  return interp(data,N);
};

inline std::vector<int> Blue ( size_t N=1 )
{
  std::vector<int> data;
  data.push_back(  0); data.push_back(  0); data.push_back(255);
  return interp(data,N);
};

// =============================================================================
// inverse colormaps
// =============================================================================

inline std::vector<int> Accent_r   ( size_t N=256 ) { return reverse(Accent  (N)); };
inline std::vector<int> Dark2_r    ( size_t N=256 ) { return reverse(Dark2   (N)); };
inline std::vector<int> Paired_r   ( size_t N=256 ) { return reverse(Paired  (N)); };
inline std::vector<int> Spectral_r ( size_t N=256 ) { return reverse(Spectral(N)); };
inline std::vector<int> Pastel1_r  ( size_t N=256 ) { return reverse(Pastel1 (N)); };
inline std::vector<int> Pastel2_r  ( size_t N=256 ) { return reverse(Pastel2 (N)); };
inline std::vector<int> Set1_r     ( size_t N=256 ) { return reverse(Set1    (N)); };
inline std::vector<int> Set2_r     ( size_t N=256 ) { return reverse(Set2    (N)); };
inline std::vector<int> Set3_r     ( size_t N=256 ) { return reverse(Set3    (N)); };
inline std::vector<int> Blues_r    ( size_t N=256 ) { return reverse(Blues   (N)); };
inline std::vector<int> Greens_r   ( size_t N=256 ) { return reverse(Greens  (N)); };
inline std::vector<int> Greys_r    ( size_t N=256 ) { return reverse(Greys   (N)); };
inline std::vector<int> Oranges_r  ( size_t N=256 ) { return reverse(Oranges (N)); };
inline std::vector<int> Purples_r  ( size_t N=256 ) { return reverse(Purples (N)); };
inline std::vector<int> Reds_r     ( size_t N=256 ) { return reverse(Reds    (N)); };
inline std::vector<int> BuPu_r     ( size_t N=256 ) { return reverse(BuPu    (N)); };
inline std::vector<int> GnBu_r     ( size_t N=256 ) { return reverse(GnBu    (N)); };
inline std::vector<int> PuBu_r     ( size_t N=256 ) { return reverse(PuBu    (N)); };
inline std::vector<int> PuBuGn_r   ( size_t N=256 ) { return reverse(PuBuGn  (N)); };
inline std::vector<int> PuRd_r     ( size_t N=256 ) { return reverse(PuRd    (N)); };
inline std::vector<int> RdPu_r     ( size_t N=256 ) { return reverse(RdPu    (N)); };
inline std::vector<int> OrRd_r     ( size_t N=256 ) { return reverse(OrRd    (N)); };
inline std::vector<int> RdOrYl_r   ( size_t N=256 ) { return reverse(RdOrYl  (N)); };
inline std::vector<int> YlGn_r     ( size_t N=256 ) { return reverse(YlGn    (N)); };
inline std::vector<int> YlGnBu_r   ( size_t N=256 ) { return reverse(YlGnBu  (N)); };
inline std::vector<int> YlOrRd_r   ( size_t N=256 ) { return reverse(YlOrRd  (N)); };
inline std::vector<int> BrBG_r     ( size_t N=256 ) { return reverse(BrBG    (N)); };
inline std::vector<int> PuOr_r     ( size_t N=256 ) { return reverse(PuOr    (N)); };
inline std::vector<int> RdBu_r     ( size_t N=256 ) { return reverse(RdBu    (N)); };
inline std::vector<int> RdGy_r     ( size_t N=256 ) { return reverse(RdGy    (N)); };
inline std::vector<int> RdYlBu_r   ( size_t N=256 ) { return reverse(RdYlBu  (N)); };
inline std::vector<int> RdYlGn_r   ( size_t N=256 ) { return reverse(RdYlGn  (N)); };
inline std::vector<int> PiYG_r     ( size_t N=256 ) { return reverse(PiYG    (N)); };
inline std::vector<int> PRGn_r     ( size_t N=256 ) { return reverse(PRGn    (N)); };

// =============================================================================
// read from string
// =============================================================================

inline std::vector<int> colormap ( std::string cmap, size_t N=256 )
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
};

}; // namespace cppcolormap

#endif

