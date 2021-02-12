
#include <pyxtensor/pyxtensor.hpp>
#include <cppcolormap.h>

// -------------------------------------------------------------------------------------------------

PYBIND11_MODULE(cppcolormap, m)
{

m.doc() ="Library with colormaps";

// -------------------------------------------------------------------------------------------------

m.def("Accent", &cppcolormap::Accent, "Accent - colorbrewer2.org", py::arg("N")=8);
m.def("Dark2", &cppcolormap::Dark2, "Dark2 - colorbrewer2.org", py::arg("N")=8);
m.def("Paired", &cppcolormap::Paired, "Paired - colorbrewer2.org", py::arg("N")=12);
m.def("Spectral", &cppcolormap::Spectral, "Spectra - colorbrewer2.org", py::arg("N")=11);
m.def("Pastel1", &cppcolormap::Pastel1, "Pastel1 - colorbrewer2.org", py::arg("N")=9);
m.def("Pastel2", &cppcolormap::Pastel2, "Pastel2 - colorbrewer2.org", py::arg("N")=8);
m.def("Set1", &cppcolormap::Set1, "Set1 - colorbrewer2.org", py::arg("N")=9);
m.def("Set2", &cppcolormap::Set2, "Set2 - colorbrewer2.org", py::arg("N")=8);
m.def("Set3", &cppcolormap::Set3, "Set3 - colorbrewer2.org", py::arg("N")=12);
m.def("Blues", &cppcolormap::Blues, "Blues - colorbrewer2.org", py::arg("N")=9);
m.def("Greens", &cppcolormap::Greens, "Greens - colorbrewer2.org", py::arg("N")=9);
m.def("Greys", &cppcolormap::Greys, "Greys - colorbrewer2.org", py::arg("N")=2);
m.def("Oranges", &cppcolormap::Oranges, "Oranges - colorbrewer2.org", py::arg("N")=9);
m.def("Purples", &cppcolormap::Purples, "Purples - colorbrewer2.org", py::arg("N")=9);
m.def("Reds", &cppcolormap::Reds, "Reds - colorbrewer2.org", py::arg("N")=9);
m.def("BuPu", &cppcolormap::BuPu, "BuPu - colorbrewer2.org", py::arg("N")=9);
m.def("GnBu", &cppcolormap::GnBu, "GnBu - colorbrewer2.org", py::arg("N")=9);
m.def("PuBu", &cppcolormap::PuBu, "PuBu - colorbrewer2.org", py::arg("N")=9);
m.def("PuBuGn", &cppcolormap::PuBuGn, "PuBuGn - colorbrewer2.org", py::arg("N")=9);
m.def("PuRd", &cppcolormap::PuRd, "PuRd - colorbrewer2.org", py::arg("N")=9);
m.def("RdPu", &cppcolormap::RdPu, "RdPu - colorbrewer2.org", py::arg("N")=9);
m.def("OrRd", &cppcolormap::OrRd, "OrRd - colorbrewer2.org", py::arg("N")=9);
m.def("RdOrYl", &cppcolormap::RdOrYl, "RdOrYl - colorbrewer2.org", py::arg("N")=9);
m.def("YlGn", &cppcolormap::YlGn, "YlGn - colorbrewer2.org", py::arg("N")=9);
m.def("YlGnBu", &cppcolormap::YlGnBu, "YlGnBu - colorbrewer2.org", py::arg("N")=9);
m.def("YlOrRd", &cppcolormap::YlOrRd, "YlOrRd - colorbrewer2.org", py::arg("N")=9);
m.def("BrBG", &cppcolormap::BrBG, "BrBG - colorbrewer2.org", py::arg("N")=11);
m.def("PuOr", &cppcolormap::PuOr, "PuOr - colorbrewer2.org", py::arg("N")=11);
m.def("RdBu", &cppcolormap::RdBu, "RdBu - colorbrewer2.org", py::arg("N")=11);
m.def("RdGy", &cppcolormap::RdGy, "RdGy - colorbrewer2.org", py::arg("N")=11);
m.def("RdYlBu", &cppcolormap::RdYlBu, "RdYlBu - colorbrewer2.org", py::arg("N")=11);
m.def("RdYlGn", &cppcolormap::RdYlGn, "RdYlGn - colorbrewer2.org", py::arg("N")=11);
m.def("PiYG", &cppcolormap::PiYG, "PiYG - colorbrewer2.org", py::arg("N")=11);
m.def("PRGn", &cppcolormap::PRGn, "PRGn - colorbrewer2.org", py::arg("N")=11);

// -------------------------------------------------------------------------------------------------

m.def("Accent_r", &cppcolormap::Accent_r, "Accent_r - colorbrewer2.org", py::arg("N")=8);
m.def("Dark2_r", &cppcolormap::Dark2_r, "Dark2_r - colorbrewer2.org", py::arg("N")=8);
m.def("Paired_r", &cppcolormap::Paired_r, "Paired_r - colorbrewer2.org", py::arg("N")=12);
m.def("Spectral_r", &cppcolormap::Spectral_r, "Spectra_r - colorbrewer2.org", py::arg("N")=11);
m.def("Pastel1_r", &cppcolormap::Pastel1_r, "Pastel1_r - colorbrewer2.org", py::arg("N")=9);
m.def("Pastel2_r", &cppcolormap::Pastel2_r, "Pastel2_r - colorbrewer2.org", py::arg("N")=8);
m.def("Set1_r", &cppcolormap::Set1_r, "Set1_r - colorbrewer2.org", py::arg("N")=9);
m.def("Set2_r", &cppcolormap::Set2_r, "Set2_r - colorbrewer2.org", py::arg("N")=8);
m.def("Set3_r", &cppcolormap::Set3_r, "Set3_r - colorbrewer2.org", py::arg("N")=12);
m.def("Blues_r", &cppcolormap::Blues_r, "Blues_r - colorbrewer2.org", py::arg("N")=9);
m.def("Greens_r", &cppcolormap::Greens_r, "Greens_r - colorbrewer2.org", py::arg("N")=9);
m.def("Greys_r", &cppcolormap::Greys_r, "Greys_r - colorbrewer2.org", py::arg("N")=2);
m.def("Oranges_r", &cppcolormap::Oranges_r, "Oranges_r - colorbrewer2.org", py::arg("N")=9);
m.def("Purples_r", &cppcolormap::Purples_r, "Purples_r - colorbrewer2.org", py::arg("N")=9);
m.def("Reds_r", &cppcolormap::Reds_r, "Reds_r - colorbrewer2.org", py::arg("N")=9);
m.def("BuPu_r", &cppcolormap::BuPu_r, "BuPu_r - colorbrewer2.org", py::arg("N")=9);
m.def("GnBu_r", &cppcolormap::GnBu_r, "GnBu_r - colorbrewer2.org", py::arg("N")=9);
m.def("PuBu_r", &cppcolormap::PuBu_r, "PuBu_r - colorbrewer2.org", py::arg("N")=9);
m.def("PuBuGn_r", &cppcolormap::PuBuGn_r, "PuBuGn_r - colorbrewer2.org", py::arg("N")=9);
m.def("PuRd_r", &cppcolormap::PuRd_r, "PuRd_r - colorbrewer2.org", py::arg("N")=9);
m.def("RdPu_r", &cppcolormap::RdPu_r, "RdPu_r - colorbrewer2.org", py::arg("N")=9);
m.def("OrRd_r", &cppcolormap::OrRd_r, "OrRd_r - colorbrewer2.org", py::arg("N")=9);
m.def("RdOrYl_r", &cppcolormap::RdOrYl_r, "RdOrYl_r - colorbrewer2.org", py::arg("N")=9);
m.def("YlGn_r", &cppcolormap::YlGn_r, "YlGn_r - colorbrewer2.org", py::arg("N")=9);
m.def("YlGnBu_r", &cppcolormap::YlGnBu_r, "YlGnBu_r - colorbrewer2.org", py::arg("N")=9);
m.def("YlOrRd_r", &cppcolormap::YlOrRd_r, "YlOrRd_r - colorbrewer2.org", py::arg("N")=9);
m.def("BrBG_r", &cppcolormap::BrBG_r, "BrBG_r - colorbrewer2.org", py::arg("N")=11);
m.def("PuOr_r", &cppcolormap::PuOr_r, "PuOr_r - colorbrewer2.org", py::arg("N")=11);
m.def("RdBu_r", &cppcolormap::RdBu_r, "RdBu_r - colorbrewer2.org", py::arg("N")=11);
m.def("RdGy_r", &cppcolormap::RdGy_r, "RdGy_r - colorbrewer2.org", py::arg("N")=11);
m.def("RdYlBu_r", &cppcolormap::RdYlBu_r, "RdYlBu_r - colorbrewer2.org", py::arg("N")=11);
m.def("RdYlGn_r", &cppcolormap::RdYlGn_r, "RdYlGn_r - colorbrewer2.org", py::arg("N")=11);
m.def("PiYG_r", &cppcolormap::PiYG_r, "PiYG_r - colorbrewer2.org", py::arg("N")=11);
m.def("PRGn_r", &cppcolormap::PRGn_r, "PRGn_r - colorbrewer2.org", py::arg("N")=11);

// -------------------------------------------------------------------------------------------------

m.def("spring", &cppcolormap::spring, "spring - matplotlib", py::arg("N")=256);
m.def("summer", &cppcolormap::summer, "summer - matplotlib", py::arg("N")=256);
m.def("autumn", &cppcolormap::autumn, "autumn - matplotlib", py::arg("N")=256);
m.def("winter", &cppcolormap::winter, "winter - matplotlib", py::arg("N")=256);
m.def("bone", &cppcolormap::bone, "bone - matplotlib", py::arg("N")=256);
m.def("cool", &cppcolormap::cool, "cool - matplotlib", py::arg("N")=256);
m.def("hot", &cppcolormap::hot, "hot - matplotlib", py::arg("N")=256);
m.def("copper", &cppcolormap::copper, "copper - matplotlib", py::arg("N")=256);
m.def("hsv", &cppcolormap::hsv, "hsv - matplotlib", py::arg("N")=256);
m.def("nipy_spectral", &cppcolormap::nipy_spectral, "nipy_spectral - matplotlib", py::arg("N")=256);
m.def("terrain", &cppcolormap::terrain, "terrain - matplotlib", py::arg("N")=6);
m.def("seismic", &cppcolormap::seismic, "seismic - matplotlib", py::arg("N")=5);
m.def("afmhot", &cppcolormap::afmhot, "afmhot - matplotlib", py::arg("N")=256);
m.def("magma", &cppcolormap::magma, "magma - matplotlib", py::arg("N")=256);
m.def("inferno", &cppcolormap::inferno, "inferno - matplotlib", py::arg("N")=256);
m.def("plasma", &cppcolormap::plasma, "plasma - matplotlib", py::arg("N")=256);
m.def("viridis", &cppcolormap::viridis, "viridis - matplotlib", py::arg("N")=256);
m.def("jet", &cppcolormap::jet, "jet - matplotlib", py::arg("N")=256);

// -------------------------------------------------------------------------------------------------

m.def("spring_r", &cppcolormap::spring_r, "spring_r - matplotlib", py::arg("N")=256);
m.def("summer_r", &cppcolormap::summer_r, "summer_r - matplotlib", py::arg("N")=256);
m.def("autumn_r", &cppcolormap::autumn_r, "autumn_r - matplotlib", py::arg("N")=256);
m.def("winter_r", &cppcolormap::winter_r, "winter_r - matplotlib", py::arg("N")=256);
m.def("bone_r", &cppcolormap::bone_r, "bone_r - matplotlib", py::arg("N")=256);
m.def("cool_r", &cppcolormap::cool_r, "cool_r - matplotlib", py::arg("N")=256);
m.def("hot_r", &cppcolormap::hot_r, "hot_r - matplotlib", py::arg("N")=256);
m.def("copper_r", &cppcolormap::copper_r, "copper_r - matplotlib", py::arg("N")=256);
m.def("hsv_r", &cppcolormap::hsv_r, "hsv_r - matplotlib", py::arg("N")=256);
m.def("nipy_spectral_r", &cppcolormap::nipy_spectral_r, "nipy_spectral_r - matplotlib", py::arg("N")=256);
m.def("terrain_r", &cppcolormap::terrain_r, "terrain_r - matplotlib", py::arg("N")=6);
m.def("seismic_r", &cppcolormap::seismic_r, "seismic_r - matplotlib", py::arg("N")=5);
m.def("afmhot_r", &cppcolormap::afmhot_r, "afmhot_r - matplotlib", py::arg("N")=256);
m.def("magma_r", &cppcolormap::magma_r, "magma_r - matplotlib", py::arg("N")=256);
m.def("inferno_r", &cppcolormap::inferno_r, "inferno_r - matplotlib", py::arg("N")=256);
m.def("plasma_r", &cppcolormap::plasma_r, "plasma_r - matplotlib", py::arg("N")=256);
m.def("viridis_r", &cppcolormap::viridis_r, "viridis_r - matplotlib", py::arg("N")=256);
m.def("jet_r", &cppcolormap::jet_r, "jet_r - matplotlib", py::arg("N")=256);

// -------------------------------------------------------------------------------------------------

m.def("White", &cppcolormap::White, "White", py::arg("N")=1);
m.def("Grey", &cppcolormap::Grey, "Grey", py::arg("N")=1);
m.def("Black", &cppcolormap::Black, "Black", py::arg("N")=1);
m.def("Red", &cppcolormap::Red, "Red", py::arg("N")=1);
m.def("Blue", &cppcolormap::Blue, "Blue", py::arg("N")=1);
m.def("tuewarmred", &cppcolormap::tuewarmred, "tuewarmred", py::arg("N")=1);
m.def("tuedarkblue", &cppcolormap::tuedarkblue, "tuedarkblue", py::arg("N")=1);
m.def("tueblue", &cppcolormap::tueblue, "tueblue", py::arg("N")=1);
m.def("tuelightblue", &cppcolormap::tuelightblue, "tuelightblue", py::arg("N")=1);
m.def("Apricot", &cppcolormap::Apricot, "Apricot", py::arg("N")=1);
m.def("Aquamarine", &cppcolormap::Aquamarine, "Aquamarine", py::arg("N")=1);
m.def("Bittersweet", &cppcolormap::Bittersweet, "Bittersweet", py::arg("N")=1);
m.def("Black", &cppcolormap::Black, "Black", py::arg("N")=1);
m.def("Blue", &cppcolormap::Blue, "Blue", py::arg("N")=1);
m.def("BlueGreen", &cppcolormap::BlueGreen, "BlueGreen", py::arg("N")=1);
m.def("BlueViolet", &cppcolormap::BlueViolet, "BlueViolet", py::arg("N")=1);
m.def("BrickRed", &cppcolormap::BrickRed, "BrickRed", py::arg("N")=1);
m.def("Brown", &cppcolormap::Brown, "Brown", py::arg("N")=1);
m.def("BurntOrange", &cppcolormap::BurntOrange, "BurntOrange", py::arg("N")=1);
m.def("CadetBlue", &cppcolormap::CadetBlue, "CadetBlue", py::arg("N")=1);
m.def("CarnationPink", &cppcolormap::CarnationPink, "CarnationPink", py::arg("N")=1);
m.def("Cerulean", &cppcolormap::Cerulean, "Cerulean", py::arg("N")=1);
m.def("CornflowerBlue", &cppcolormap::CornflowerBlue, "CornflowerBlue", py::arg("N")=1);
m.def("Cyan", &cppcolormap::Cyan, "Cyan", py::arg("N")=1);
m.def("Dandelion", &cppcolormap::Dandelion, "Dandelion", py::arg("N")=1);
m.def("DarkOrchid", &cppcolormap::DarkOrchid, "DarkOrchid", py::arg("N")=1);
m.def("Emerald", &cppcolormap::Emerald, "Emerald", py::arg("N")=1);
m.def("ForestGreen", &cppcolormap::ForestGreen, "ForestGreen", py::arg("N")=1);
m.def("Fuchsia", &cppcolormap::Fuchsia, "Fuchsia", py::arg("N")=1);
m.def("Goldenrod", &cppcolormap::Goldenrod, "Goldenrod", py::arg("N")=1);
m.def("Gray", &cppcolormap::Gray, "Gray", py::arg("N")=1);
m.def("Green", &cppcolormap::Green, "Green", py::arg("N")=1);
m.def("GreenYellow", &cppcolormap::GreenYellow, "GreenYellow", py::arg("N")=1);
m.def("JungleGreen", &cppcolormap::JungleGreen, "JungleGreen", py::arg("N")=1);
m.def("Lavender", &cppcolormap::Lavender, "Lavender", py::arg("N")=1);
m.def("LimeGreen", &cppcolormap::LimeGreen, "LimeGreen", py::arg("N")=1);
m.def("Magenta", &cppcolormap::Magenta, "Magenta", py::arg("N")=1);
m.def("Mahogany", &cppcolormap::Mahogany, "Mahogany", py::arg("N")=1);
m.def("Maroon", &cppcolormap::Maroon, "Maroon", py::arg("N")=1);
m.def("Melon", &cppcolormap::Melon, "Melon", py::arg("N")=1);
m.def("MidnightBlue", &cppcolormap::MidnightBlue, "MidnightBlue", py::arg("N")=1);
m.def("Mulberry", &cppcolormap::Mulberry, "Mulberry", py::arg("N")=1);
m.def("NavyBlue", &cppcolormap::NavyBlue, "NavyBlue", py::arg("N")=1);
m.def("OliveGreen", &cppcolormap::OliveGreen, "OliveGreen", py::arg("N")=1);
m.def("Orange", &cppcolormap::Orange, "Orange", py::arg("N")=1);
m.def("OrangeRed", &cppcolormap::OrangeRed, "OrangeRed", py::arg("N")=1);
m.def("Orchid", &cppcolormap::Orchid, "Orchid", py::arg("N")=1);
m.def("Peach", &cppcolormap::Peach, "Peach", py::arg("N")=1);
m.def("Periwinkle", &cppcolormap::Periwinkle, "Periwinkle", py::arg("N")=1);
m.def("PineGreen", &cppcolormap::PineGreen, "PineGreen", py::arg("N")=1);
m.def("Plum", &cppcolormap::Plum, "Plum", py::arg("N")=1);
m.def("ProcessBlue", &cppcolormap::ProcessBlue, "ProcessBlue", py::arg("N")=1);
m.def("Purple", &cppcolormap::Purple, "Purple", py::arg("N")=1);
m.def("RawSienna", &cppcolormap::RawSienna, "RawSienna", py::arg("N")=1);
m.def("Red", &cppcolormap::Red, "Red", py::arg("N")=1);
m.def("RedOrange", &cppcolormap::RedOrange, "RedOrange", py::arg("N")=1);
m.def("RedViolet", &cppcolormap::RedViolet, "RedViolet", py::arg("N")=1);
m.def("Rhodamine", &cppcolormap::Rhodamine, "Rhodamine", py::arg("N")=1);
m.def("RoyalBlue", &cppcolormap::RoyalBlue, "RoyalBlue", py::arg("N")=1);
m.def("RoyalPurple", &cppcolormap::RoyalPurple, "RoyalPurple", py::arg("N")=1);
m.def("RubineRed", &cppcolormap::RubineRed, "RubineRed", py::arg("N")=1);
m.def("Salmon", &cppcolormap::Salmon, "Salmon", py::arg("N")=1);
m.def("SeaGreen", &cppcolormap::SeaGreen, "SeaGreen", py::arg("N")=1);
m.def("Sepia", &cppcolormap::Sepia, "Sepia", py::arg("N")=1);
m.def("SkyBlue", &cppcolormap::SkyBlue, "SkyBlue", py::arg("N")=1);
m.def("SpringGreen", &cppcolormap::SpringGreen, "SpringGreen", py::arg("N")=1);
m.def("Tan", &cppcolormap::Tan, "Tan", py::arg("N")=1);
m.def("TealBlue", &cppcolormap::TealBlue, "TealBlue", py::arg("N")=1);
m.def("Thistle", &cppcolormap::Thistle, "Thistle", py::arg("N")=1);
m.def("Turquoise", &cppcolormap::Turquoise, "Turquoise", py::arg("N")=1);
m.def("Violet", &cppcolormap::Violet, "Violet", py::arg("N")=1);
m.def("VioletRed", &cppcolormap::VioletRed, "VioletRed", py::arg("N")=1);
m.def("WildStrawberry", &cppcolormap::WildStrawberry, "WildStrawberry", py::arg("N")=1);
m.def("Yellow", &cppcolormap::Yellow, "Yellow", py::arg("N")=1);
m.def("YellowGreen", &cppcolormap::YellowGreen, "YellowGreen", py::arg("N")=1);
m.def("YellowOrange", &cppcolormap::YellowOrange, "YellowOrange", py::arg("N")=1);

// -------------------------------------------------------------------------------------------------

m.def("xterm", &cppcolormap::xterm, "xterm");
m.def("tue", &cppcolormap::tue, "tue - tue.nl");

// -------------------------------------------------------------------------------------------------

m.def("xterm_r", &cppcolormap::xterm_r, "xterm_r");
m.def("tue_r", &cppcolormap::tue_r, "tue_r - tue.nl");

// -------------------------------------------------------------------------------------------------

m.def("colormap", &cppcolormap::colormap,
    "Colormap by name",
    py::arg("cmap"),
    py::arg("N")=256);

m.def("colorcycle",
    &cppcolormap::colorcycle,
    "Color-cycle by name",
    py::arg("cmap"));

m.def("hex2rgb",
    py::overload_cast<const std::vector<std::string>&>(&cppcolormap::hex2rgb),
    "Convert hex -> rgb");

m.def("hex2rgb",
    py::overload_cast<const std::string&>(&cppcolormap::hex2rgb),
    "Convert hex -> rgb");

m.def("rgb2hex",
    py::overload_cast<const xt::xtensor<double,2>&>(&cppcolormap::rgb2hex),
    "Convert rgb -> hex");

m.def("rgb2hex",
    py::overload_cast<const xt::xtensor<double,1>&>(&cppcolormap::rgb2hex),
    "Convert rgb -> hex");

m.def("as_colors", [](
        const xt::xarray<double>& data,
        const xt::xtensor<double, 2>& colors,
        double vmin,
        double vmax) {
            return cppcolormap::as_colors(data, colors, vmin, vmax);
    },
    "Convert data to colors using a colormap."
    "See :cpp:func:`cppcolormap::as_colors`.",
    py::arg("data"),
    py::arg("colors"),
    py::arg("vmin"),
    py::arg("vmax"));

m.def("as_colors", [](
        const xt::xarray<double>& data,
        const xt::xtensor<double, 2>& colors) {
            return cppcolormap::as_colors(data, colors);
    },
    "Convert data to colors using a colormap."
    "See :cpp:func:`cppcolormap::as_colors`.",
    py::arg("data"),
    py::arg("colors"));

// -------------------------------------------------------------------------------------------------

py::enum_<cppcolormap::metric>(m, "metric", "Distance metric for color matching")
    .value("euclidean", cppcolormap::metric::euclidean)
    .value("fast_perceptual", cppcolormap::metric::fast_perceptual)
    .value("perceptual", cppcolormap::metric::perceptual)
    .export_values();

m.def("match", &cppcolormap::match);

// -------------------------------------------------------------------------------------------------

} // PYBIND11_MODULE
