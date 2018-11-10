
#include <pyxtensor/pyxtensor.hpp>

#include "cppcolormap.h"

// -------------------------------------------------------------------------------------------------

PYBIND11_MODULE(cppcolormap, m)
{

m.doc() = "Library with colormaps";

// -------------------------------------------------------------------------------------------------

m.def("Accent"  , &cppcolormap::Accent  , "Accent  - colorbrewer2.org", py::arg("N")= 8);
m.def("Dark2"   , &cppcolormap::Dark2   , "Dark2   - colorbrewer2.org", py::arg("N")= 8);
m.def("Paired"  , &cppcolormap::Paired  , "Paired  - colorbrewer2.org", py::arg("N")=12);
m.def("Spectral", &cppcolormap::Spectral, "Spectra - colorbrewer2.org", py::arg("N")=11);
m.def("Pastel1" , &cppcolormap::Pastel1 , "Pastel1 - colorbrewer2.org", py::arg("N")= 9);
m.def("Pastel2" , &cppcolormap::Pastel2 , "Pastel2 - colorbrewer2.org", py::arg("N")= 8);
m.def("Set1"    , &cppcolormap::Set1    , "Set1    - colorbrewer2.org", py::arg("N")= 9);
m.def("Set2"    , &cppcolormap::Set2    , "Set2    - colorbrewer2.org", py::arg("N")= 8);
m.def("Set3"    , &cppcolormap::Set3    , "Set3    - colorbrewer2.org", py::arg("N")=12);
m.def("Blues"   , &cppcolormap::Blues   , "Blues   - colorbrewer2.org", py::arg("N")= 9);
m.def("Greens"  , &cppcolormap::Greens  , "Greens  - colorbrewer2.org", py::arg("N")= 9);
m.def("Greys"   , &cppcolormap::Greys   , "Greys   - colorbrewer2.org", py::arg("N")= 2);
m.def("Oranges" , &cppcolormap::Oranges , "Oranges - colorbrewer2.org", py::arg("N")= 9);
m.def("Purples" , &cppcolormap::Purples , "Purples - colorbrewer2.org", py::arg("N")= 9);
m.def("Reds"    , &cppcolormap::Reds    , "Reds    - colorbrewer2.org", py::arg("N")= 9);
m.def("BuPu"    , &cppcolormap::BuPu    , "BuPu    - colorbrewer2.org", py::arg("N")= 9);
m.def("GnBu"    , &cppcolormap::GnBu    , "GnBu    - colorbrewer2.org", py::arg("N")= 9);
m.def("PuBu"    , &cppcolormap::PuBu    , "PuBu    - colorbrewer2.org", py::arg("N")= 9);
m.def("PuBuGn"  , &cppcolormap::PuBuGn  , "PuBuGn  - colorbrewer2.org", py::arg("N")= 9);
m.def("PuRd"    , &cppcolormap::PuRd    , "PuRd    - colorbrewer2.org", py::arg("N")= 9);
m.def("RdPu"    , &cppcolormap::RdPu    , "RdPu    - colorbrewer2.org", py::arg("N")= 9);
m.def("OrRd"    , &cppcolormap::OrRd    , "OrRd    - colorbrewer2.org", py::arg("N")= 9);
m.def("RdOrYl"  , &cppcolormap::RdOrYl  , "RdOrYl  - colorbrewer2.org", py::arg("N")= 9);
m.def("YlGn"    , &cppcolormap::YlGn    , "YlGn    - colorbrewer2.org", py::arg("N")= 9);
m.def("YlGnBu"  , &cppcolormap::YlGnBu  , "YlGnBu  - colorbrewer2.org", py::arg("N")= 9);
m.def("YlOrRd"  , &cppcolormap::YlOrRd  , "YlOrRd  - colorbrewer2.org", py::arg("N")= 9);
m.def("BrBG"    , &cppcolormap::BrBG    , "BrBG    - colorbrewer2.org", py::arg("N")=11);
m.def("PuOr"    , &cppcolormap::PuOr    , "PuOr    - colorbrewer2.org", py::arg("N")=11);
m.def("RdBu"    , &cppcolormap::RdBu    , "RdBu    - colorbrewer2.org", py::arg("N")=11);
m.def("RdGy"    , &cppcolormap::RdGy    , "RdGy    - colorbrewer2.org", py::arg("N")=11);
m.def("RdYlBu"  , &cppcolormap::RdYlBu  , "RdYlBu  - colorbrewer2.org", py::arg("N")=11);
m.def("RdYlGn"  , &cppcolormap::RdYlGn  , "RdYlGn  - colorbrewer2.org", py::arg("N")=11);
m.def("PiYG"    , &cppcolormap::PiYG    , "PiYG    - colorbrewer2.org", py::arg("N")=11);
m.def("PRGn"    , &cppcolormap::PRGn    , "PRGn    - colorbrewer2.org", py::arg("N")=11);

// -------------------------------------------------------------------------------------------------

m.def("Accent_r"  , &cppcolormap::Accent_r  , "Accent_r  - colorbrewer2.org", py::arg("N")= 8);
m.def("Dark2_r"   , &cppcolormap::Dark2_r   , "Dark2_r   - colorbrewer2.org", py::arg("N")= 8);
m.def("Paired_r"  , &cppcolormap::Paired_r  , "Paired_r  - colorbrewer2.org", py::arg("N")=12);
m.def("Spectral_r", &cppcolormap::Spectral_r, "Spectra_r - colorbrewer2.org", py::arg("N")=11);
m.def("Pastel1_r" , &cppcolormap::Pastel1_r , "Pastel1_r - colorbrewer2.org", py::arg("N")= 9);
m.def("Pastel2_r" , &cppcolormap::Pastel2_r , "Pastel2_r - colorbrewer2.org", py::arg("N")= 8);
m.def("Set1_r"    , &cppcolormap::Set1_r    , "Set1_r    - colorbrewer2.org", py::arg("N")= 9);
m.def("Set2_r"    , &cppcolormap::Set2_r    , "Set2_r    - colorbrewer2.org", py::arg("N")= 8);
m.def("Set3_r"    , &cppcolormap::Set3_r    , "Set3_r    - colorbrewer2.org", py::arg("N")=12);
m.def("Blues_r"   , &cppcolormap::Blues_r   , "Blues_r   - colorbrewer2.org", py::arg("N")= 9);
m.def("Greens_r"  , &cppcolormap::Greens_r  , "Greens_r  - colorbrewer2.org", py::arg("N")= 9);
m.def("Greys_r"   , &cppcolormap::Greys_r   , "Greys_r   - colorbrewer2.org", py::arg("N")= 2);
m.def("Oranges_r" , &cppcolormap::Oranges_r , "Oranges_r - colorbrewer2.org", py::arg("N")= 9);
m.def("Purples_r" , &cppcolormap::Purples_r , "Purples_r - colorbrewer2.org", py::arg("N")= 9);
m.def("Reds_r"    , &cppcolormap::Reds_r    , "Reds_r    - colorbrewer2.org", py::arg("N")= 9);
m.def("BuPu_r"    , &cppcolormap::BuPu_r    , "BuPu_r    - colorbrewer2.org", py::arg("N")= 9);
m.def("GnBu_r"    , &cppcolormap::GnBu_r    , "GnBu_r    - colorbrewer2.org", py::arg("N")= 9);
m.def("PuBu_r"    , &cppcolormap::PuBu_r    , "PuBu_r    - colorbrewer2.org", py::arg("N")= 9);
m.def("PuBuGn_r"  , &cppcolormap::PuBuGn_r  , "PuBuGn_r  - colorbrewer2.org", py::arg("N")= 9);
m.def("PuRd_r"    , &cppcolormap::PuRd_r    , "PuRd_r    - colorbrewer2.org", py::arg("N")= 9);
m.def("RdPu_r"    , &cppcolormap::RdPu_r    , "RdPu_r    - colorbrewer2.org", py::arg("N")= 9);
m.def("OrRd_r"    , &cppcolormap::OrRd_r    , "OrRd_r    - colorbrewer2.org", py::arg("N")= 9);
m.def("RdOrYl_r"  , &cppcolormap::RdOrYl_r  , "RdOrYl_r  - colorbrewer2.org", py::arg("N")= 9);
m.def("YlGn_r"    , &cppcolormap::YlGn_r    , "YlGn_r    - colorbrewer2.org", py::arg("N")= 9);
m.def("YlGnBu_r"  , &cppcolormap::YlGnBu_r  , "YlGnBu_r  - colorbrewer2.org", py::arg("N")= 9);
m.def("YlOrRd_r"  , &cppcolormap::YlOrRd_r  , "YlOrRd_r  - colorbrewer2.org", py::arg("N")= 9);
m.def("BrBG_r"    , &cppcolormap::BrBG_r    , "BrBG_r    - colorbrewer2.org", py::arg("N")=11);
m.def("PuOr_r"    , &cppcolormap::PuOr_r    , "PuOr_r    - colorbrewer2.org", py::arg("N")=11);
m.def("RdBu_r"    , &cppcolormap::RdBu_r    , "RdBu_r    - colorbrewer2.org", py::arg("N")=11);
m.def("RdGy_r"    , &cppcolormap::RdGy_r    , "RdGy_r    - colorbrewer2.org", py::arg("N")=11);
m.def("RdYlBu_r"  , &cppcolormap::RdYlBu_r  , "RdYlBu_r  - colorbrewer2.org", py::arg("N")=11);
m.def("RdYlGn_r"  , &cppcolormap::RdYlGn_r  , "RdYlGn_r  - colorbrewer2.org", py::arg("N")=11);
m.def("PiYG_r"    , &cppcolormap::PiYG_r    , "PiYG_r    - colorbrewer2.org", py::arg("N")=11);
m.def("PRGn_r"    , &cppcolormap::PRGn_r    , "PRGn_r    - colorbrewer2.org", py::arg("N")=11);

// -------------------------------------------------------------------------------------------------

m.def("magma"  , &cppcolormap::magma  , "magma   - matplotlib", py::arg("N")=256);
m.def("inferno", &cppcolormap::inferno, "inferno - matplotlib", py::arg("N")=256);
m.def("plasma" , &cppcolormap::plasma , "plasma  - matplotlib", py::arg("N")=256);
m.def("viridis", &cppcolormap::viridis, "viridis - matplotlib", py::arg("N")=256);

// -------------------------------------------------------------------------------------------------

m.def("magma_r"  , &cppcolormap::magma_r  , "magma_r   - matplotlib", py::arg("N")=256);
m.def("inferno_r", &cppcolormap::inferno_r, "inferno_r - matplotlib", py::arg("N")=256);
m.def("plasma_r" , &cppcolormap::plasma_r , "plasma_r  - matplotlib", py::arg("N")=256);
m.def("viridis_r", &cppcolormap::viridis_r, "viridis_r - matplotlib", py::arg("N")=256);

// -------------------------------------------------------------------------------------------------

m.def("White", &cppcolormap::White, "White", py::arg("N")=1);
m.def("Grey" , &cppcolormap::Grey , "Grey" , py::arg("N")=1);
m.def("Black", &cppcolormap::Black, "Black", py::arg("N")=1);
m.def("Red"  , &cppcolormap::Red  , "Red"  , py::arg("N")=1);
m.def("Blue" , &cppcolormap::Blue , "Blue" , py::arg("N")=1);

// -------------------------------------------------------------------------------------------------

m.def("xterm"        ,&cppcolormap::xterm        ,"xterm"                );
m.def("tue"          ,&cppcolormap::tue          ,"tue          - tue.nl");
m.def("tuewarmred"   ,&cppcolormap::tuewarmred   ,"tuewarmred   - tue.nl");
m.def("tuedarkblue"  ,&cppcolormap::tuedarkblue  ,"tuedarkblue  - tue.nl");
m.def("tueblue"      ,&cppcolormap::tueblue      ,"tueblue      - tue.nl");
m.def("tuelightblue" ,&cppcolormap::tuelightblue ,"tuelightblue - tue.nl");

// -------------------------------------------------------------------------------------------------

m.def("colorcycle",&cppcolormap::colorcycle,"Color-cycle by name",py::arg("cmap")                 );
m.def("colormap"  ,&cppcolormap::colormap  ,"Colormap by name"   ,py::arg("cmap"),py::arg("N")=256);

// -------------------------------------------------------------------------------------------------

m.def("match"       , &cppcolormap::match       );
m.def("match_visual", &cppcolormap::match_visual);

// -------------------------------------------------------------------------------------------------

} // PYBIND11_MODULE
