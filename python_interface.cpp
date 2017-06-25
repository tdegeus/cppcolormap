
#include <cppmat/matrix.h>
#include <cppmat/pybind11_matrix.h>
#include "cppcolormap.h"


mat::matrix<int> colormap ( std::string cmap, size_t N=256 )
{
  std::vector<int> data = cppcolormap::colormap(cmap,N);
  mat::matrix<int> ret({N,3},data.data());
  return ret;
}

mat::matrix<int> colorcycle ( std::string cmap )
{
  std::vector<int> data = cppcolormap::colorcycle(cmap);
  size_t           N    = data.size()/3;
  mat::matrix<int> ret({N,3},data.data());
  return ret;
}


PYBIND11_PLUGIN(cppcolormap) {

py::module m("cppcolormap", "Library with colormaps");

m.def("colormap"  ,&colormap  ,"Return colormap by name"          ,py::arg("cmap"),py::arg("N")=256);
m.def("colorcycle",&colorcycle,"Return custom color-cycle by name",py::arg("cmap")                 );

return m.ptr();

} // PYBIND11_PLUGIN
