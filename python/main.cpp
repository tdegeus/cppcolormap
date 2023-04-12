
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#define FORCE_IMPORT_ARRAY
#include <xtensor-python/pyarray.hpp>
#include <xtensor-python/pytensor.hpp>

#define CPPCOLORMAP_USE_XTENSOR_PYTHON
#include <cppcolormap.h>

namespace py = pybind11;

#define DOC(function) \
    (std::string("See documentation of C++ API: :cpp:func:`cppcolormap::") + \
     std::string(function) + std::string("`")) \
        .c_str()

#define ENUM(function) \
    (std::string("See documentation of C++ API: :cpp:enum:`cppcolormap::") + \
     std::string(function) + std::string("`")) \
        .c_str()

/**
 * Overrides the `__name__` of a module.
 * Classes defined by pybind11 use the `__name__` of the module as of the time they are defined,
 * which affects the `__repr__` of the class type objects.
 */
class ScopedModuleNameOverride {
public:
    explicit ScopedModuleNameOverride(py::module m, std::string name) : module_(std::move(m))
    {
        original_name_ = module_.attr("__name__");
        module_.attr("__name__") = name;
    }
    ~ScopedModuleNameOverride()
    {
        module_.attr("__name__") = original_name_;
    }

private:
    py::module module_;
    py::object original_name_;
};

PYBIND11_MODULE(_cppcolormap, m)
{
    // Ensure members to display as `cppcolormap.X` rather than `cppcolormap._cppcolormap.X`
    ScopedModuleNameOverride name_override(m, "cppcolormap");

    xt::import_numpy();

    m.doc() = "Library with colormaps.";

    m.def("version", &cppcolormap::version, DOC("version"));
    m.def("version_dependencies", &cppcolormap::version_dependencies, DOC("version_dependencies"));

    m.def(
        "rgb2hex",
        static_cast<std::string (*)(const xt::pytensor<double, 1>&)>(&cppcolormap::rgb2hex),
        DOC("rgb2hex")
    );

    m.def(
        "rgb2hex",
        static_cast<std::vector<std::string> (*)(const xt::pytensor<double, 2>&)>(
            &cppcolormap::rgb2hex
        ),
        DOC("rgb2hex")
    );

    m.def(
        "hex2rgb",
        py::overload_cast<const std::vector<std::string>&>(&cppcolormap::hex2rgb),
        DOC("hex2rgb")
    );

    m.def("hex2rgb", py::overload_cast<const std::string&>(&cppcolormap::hex2rgb), DOC("hex2rgb"));

    m.def(
        "interp",
        &cppcolormap::interp<xt::pytensor<double, 2>, xt::pytensor<double, 2>>,
        DOC("interp"),
        py::arg("arg"),
        py::arg("N")
    );

    m.def(
        "as_colors",
        [](const xt::pyarray<double>& data,
           const xt::pytensor<double, 2>& colors,
           double vmin,
           double vmax) { return cppcolormap::as_colors(data, colors, vmin, vmax); },
        DOC("as_colors"),
        py::arg("data"),
        py::arg("colors"),
        py::arg("vmin"),
        py::arg("vmax")
    );

    m.def(
        "as_colors",
        [](const xt::pyarray<double>& data, const xt::pytensor<double, 2>& colors) {
            return cppcolormap::as_colors(data, colors);
        },
        DOC("as_colors"),
        py::arg("data"),
        py::arg("colors")
    );

    m.def("Accent", &cppcolormap::Accent, DOC("Accent"), py::arg("N") = 8);
    m.def("Dark2", &cppcolormap::Dark2, DOC("Dark2"), py::arg("N") = 8);
    m.def("Paired", &cppcolormap::Paired, DOC("Paired"), py::arg("N") = 12);
    m.def("Spectral", &cppcolormap::Spectral, DOC("Spectral"), py::arg("N") = 11);
    m.def("Pastel1", &cppcolormap::Pastel1, DOC("Pastel1"), py::arg("N") = 9);
    m.def("Pastel2", &cppcolormap::Pastel2, DOC("Pastel2"), py::arg("N") = 8);
    m.def("Set1", &cppcolormap::Set1, DOC("Set1"), py::arg("N") = 9);
    m.def("Set2", &cppcolormap::Set2, DOC("Set2"), py::arg("N") = 8);
    m.def("Set3", &cppcolormap::Set3, DOC("Set3"), py::arg("N") = 12);
    m.def("Blues", &cppcolormap::Blues, DOC("Blues"), py::arg("N") = 9);
    m.def("Greens", &cppcolormap::Greens, DOC("Greens"), py::arg("N") = 9);
    m.def("Greys", &cppcolormap::Greys, DOC("Greys"), py::arg("N") = 2);
    m.def("Oranges", &cppcolormap::Oranges, DOC("Oranges"), py::arg("N") = 9);
    m.def("Purples", &cppcolormap::Purples, DOC("Purples"), py::arg("N") = 9);
    m.def("Reds", &cppcolormap::Reds, DOC("Reds"), py::arg("N") = 9);
    m.def("BuPu", &cppcolormap::BuPu, DOC("BuPu"), py::arg("N") = 9);
    m.def("GnBu", &cppcolormap::GnBu, DOC("GnBu"), py::arg("N") = 9);
    m.def("PuBu", &cppcolormap::PuBu, DOC("PuBu"), py::arg("N") = 9);
    m.def("PuBuGn", &cppcolormap::PuBuGn, DOC("PuBuGn"), py::arg("N") = 9);
    m.def("PuRd", &cppcolormap::PuRd, DOC("PuRd"), py::arg("N") = 9);
    m.def("RdPu", &cppcolormap::RdPu, DOC("RdPu"), py::arg("N") = 9);
    m.def("OrRd", &cppcolormap::OrRd, DOC("OrRd"), py::arg("N") = 9);
    m.def("RdOrYl", &cppcolormap::RdOrYl, DOC("RdOrYl"), py::arg("N") = 9);
    m.def("YlGn", &cppcolormap::YlGn, DOC("YlGn"), py::arg("N") = 9);
    m.def("YlGnBu", &cppcolormap::YlGnBu, DOC("YlGnBu"), py::arg("N") = 9);
    m.def("YlOrRd", &cppcolormap::YlOrRd, DOC("YlOrRd"), py::arg("N") = 9);
    m.def("BrBG", &cppcolormap::BrBG, DOC("BrBG"), py::arg("N") = 11);
    m.def("PuOr", &cppcolormap::PuOr, DOC("PuOr"), py::arg("N") = 11);
    m.def("RdBu", &cppcolormap::RdBu, DOC("RdBu"), py::arg("N") = 11);
    m.def("RdGy", &cppcolormap::RdGy, DOC("RdGy"), py::arg("N") = 11);
    m.def("RdYlBu", &cppcolormap::RdYlBu, DOC("RdYlBu"), py::arg("N") = 11);
    m.def("RdYlGn", &cppcolormap::RdYlGn, DOC("RdYlGn"), py::arg("N") = 11);
    m.def("PiYG", &cppcolormap::PiYG, DOC("PiYG"), py::arg("N") = 11);
    m.def("PRGn", &cppcolormap::PRGn, DOC("PRGn"), py::arg("N") = 11);

    m.def("Accent_r", &cppcolormap::Accent_r, DOC("Accent_r"), py::arg("N") = 8);
    m.def("Dark2_r", &cppcolormap::Dark2_r, DOC("Dark2_r"), py::arg("N") = 8);
    m.def("Paired_r", &cppcolormap::Paired_r, DOC("Paired_r"), py::arg("N") = 12);
    m.def("Spectral_r", &cppcolormap::Spectral_r, DOC("Spectra_r"), py::arg("N") = 11);
    m.def("Pastel1_r", &cppcolormap::Pastel1_r, DOC("Pastel1_r"), py::arg("N") = 9);
    m.def("Pastel2_r", &cppcolormap::Pastel2_r, DOC("Pastel2_r"), py::arg("N") = 8);
    m.def("Set1_r", &cppcolormap::Set1_r, DOC("Set1_r"), py::arg("N") = 9);
    m.def("Set2_r", &cppcolormap::Set2_r, DOC("Set2_r"), py::arg("N") = 8);
    m.def("Set3_r", &cppcolormap::Set3_r, DOC("Set3_r"), py::arg("N") = 12);
    m.def("Blues_r", &cppcolormap::Blues_r, DOC("Blues_r"), py::arg("N") = 9);
    m.def("Greens_r", &cppcolormap::Greens_r, DOC("Greens_r"), py::arg("N") = 9);
    m.def("Greys_r", &cppcolormap::Greys_r, DOC("Greys_r"), py::arg("N") = 2);
    m.def("Oranges_r", &cppcolormap::Oranges_r, DOC("Oranges_r"), py::arg("N") = 9);
    m.def("Purples_r", &cppcolormap::Purples_r, DOC("Purples_r"), py::arg("N") = 9);
    m.def("Reds_r", &cppcolormap::Reds_r, DOC("Reds_r"), py::arg("N") = 9);
    m.def("BuPu_r", &cppcolormap::BuPu_r, DOC("BuPu_r"), py::arg("N") = 9);
    m.def("GnBu_r", &cppcolormap::GnBu_r, DOC("GnBu_r"), py::arg("N") = 9);
    m.def("PuBu_r", &cppcolormap::PuBu_r, DOC("PuBu_r"), py::arg("N") = 9);
    m.def("PuBuGn_r", &cppcolormap::PuBuGn_r, DOC("PuBuGn_r"), py::arg("N") = 9);
    m.def("PuRd_r", &cppcolormap::PuRd_r, DOC("PuRd_r"), py::arg("N") = 9);
    m.def("RdPu_r", &cppcolormap::RdPu_r, DOC("RdPu_r"), py::arg("N") = 9);
    m.def("OrRd_r", &cppcolormap::OrRd_r, DOC("OrRd_r"), py::arg("N") = 9);
    m.def("RdOrYl_r", &cppcolormap::RdOrYl_r, DOC("RdOrYl_r"), py::arg("N") = 9);
    m.def("YlGn_r", &cppcolormap::YlGn_r, DOC("YlGn_r"), py::arg("N") = 9);
    m.def("YlGnBu_r", &cppcolormap::YlGnBu_r, DOC("YlGnBu_r"), py::arg("N") = 9);
    m.def("YlOrRd_r", &cppcolormap::YlOrRd_r, DOC("YlOrRd_r"), py::arg("N") = 9);
    m.def("BrBG_r", &cppcolormap::BrBG_r, DOC("BrBG_r"), py::arg("N") = 11);
    m.def("PuOr_r", &cppcolormap::PuOr_r, DOC("PuOr_r"), py::arg("N") = 11);
    m.def("RdBu_r", &cppcolormap::RdBu_r, DOC("RdBu_r"), py::arg("N") = 11);
    m.def("RdGy_r", &cppcolormap::RdGy_r, DOC("RdGy_r"), py::arg("N") = 11);
    m.def("RdYlBu_r", &cppcolormap::RdYlBu_r, DOC("RdYlBu_r"), py::arg("N") = 11);
    m.def("RdYlGn_r", &cppcolormap::RdYlGn_r, DOC("RdYlGn_r"), py::arg("N") = 11);
    m.def("PiYG_r", &cppcolormap::PiYG_r, DOC("PiYG_r"), py::arg("N") = 11);
    m.def("PRGn_r", &cppcolormap::PRGn_r, DOC("PRGn_r"), py::arg("N") = 11);

    m.def("spring", &cppcolormap::spring, DOC("spring"), py::arg("N") = 256);
    m.def("summer", &cppcolormap::summer, DOC("summer"), py::arg("N") = 256);
    m.def("autumn", &cppcolormap::autumn, DOC("autumn"), py::arg("N") = 256);
    m.def("winter", &cppcolormap::winter, DOC("winter"), py::arg("N") = 256);
    m.def("bone", &cppcolormap::bone, DOC("bone"), py::arg("N") = 256);
    m.def("cool", &cppcolormap::cool, DOC("cool"), py::arg("N") = 256);
    m.def("hot", &cppcolormap::hot, DOC("hot"), py::arg("N") = 256);
    m.def("copper", &cppcolormap::copper, DOC("copper"), py::arg("N") = 256);
    m.def("hsv", &cppcolormap::hsv, DOC("hsv"), py::arg("N") = 256);
    m.def("nipy_spectral", &cppcolormap::nipy_spectral, DOC("nipy_spectral"), py::arg("N") = 256);
    m.def("terrain", &cppcolormap::terrain, DOC("terrain"), py::arg("N") = 6);
    m.def("seismic", &cppcolormap::seismic, DOC("seismic"), py::arg("N") = 5);
    m.def("afmhot", &cppcolormap::afmhot, DOC("afmhot"), py::arg("N") = 256);
    m.def("magma", &cppcolormap::magma, DOC("magma"), py::arg("N") = 256);
    m.def("inferno", &cppcolormap::inferno, DOC("inferno"), py::arg("N") = 256);
    m.def("plasma", &cppcolormap::plasma, DOC("plasma"), py::arg("N") = 256);
    m.def("viridis", &cppcolormap::viridis, DOC("viridis"), py::arg("N") = 256);
    m.def("jet", &cppcolormap::jet, DOC("jet"), py::arg("N") = 256);

    m.def("spring_r", &cppcolormap::spring_r, DOC("spring_r"), py::arg("N") = 256);
    m.def("summer_r", &cppcolormap::summer_r, DOC("summer_r"), py::arg("N") = 256);
    m.def("autumn_r", &cppcolormap::autumn_r, DOC("autumn_r"), py::arg("N") = 256);
    m.def("winter_r", &cppcolormap::winter_r, DOC("winter_r"), py::arg("N") = 256);
    m.def("bone_r", &cppcolormap::bone_r, DOC("bone_r"), py::arg("N") = 256);
    m.def("cool_r", &cppcolormap::cool_r, DOC("cool_r"), py::arg("N") = 256);
    m.def("hot_r", &cppcolormap::hot_r, DOC("hot_r"), py::arg("N") = 256);
    m.def("copper_r", &cppcolormap::copper_r, DOC("copper_r"), py::arg("N") = 256);
    m.def("hsv_r", &cppcolormap::hsv_r, DOC("hsv_r"), py::arg("N") = 256);
    m.def(
        "nipy_spectral_r", &cppcolormap::nipy_spectral_r, DOC("nipy_spectral_r"), py::arg("N") = 256
    );
    m.def("terrain_r", &cppcolormap::terrain_r, DOC("terrain_r"), py::arg("N") = 6);
    m.def("seismic_r", &cppcolormap::seismic_r, DOC("seismic_r"), py::arg("N") = 5);
    m.def("afmhot_r", &cppcolormap::afmhot_r, DOC("afmhot_r"), py::arg("N") = 256);
    m.def("magma_r", &cppcolormap::magma_r, DOC("magma_r"), py::arg("N") = 256);
    m.def("inferno_r", &cppcolormap::inferno_r, DOC("inferno_r"), py::arg("N") = 256);
    m.def("plasma_r", &cppcolormap::plasma_r, DOC("plasma_r"), py::arg("N") = 256);
    m.def("viridis_r", &cppcolormap::viridis_r, DOC("viridis_r"), py::arg("N") = 256);
    m.def("jet_r", &cppcolormap::jet_r, DOC("jet_r"), py::arg("N") = 256);

    m.def("Apricot", &cppcolormap::Apricot, DOC("Apricot"), py::arg("N") = 1);
    m.def("Aquamarine", &cppcolormap::Aquamarine, DOC("Aquamarine"), py::arg("N") = 1);
    m.def("Bittersweet", &cppcolormap::Bittersweet, DOC("Bittersweet"), py::arg("N") = 1);
    m.def("Black", &cppcolormap::Black, DOC("Black"), py::arg("N") = 1);
    m.def("Blue", &cppcolormap::Blue, DOC("Blue"), py::arg("N") = 1);
    m.def("BlueGreen", &cppcolormap::BlueGreen, DOC("BlueGreen"), py::arg("N") = 1);
    m.def("BlueViolet", &cppcolormap::BlueViolet, DOC("BlueViolet"), py::arg("N") = 1);
    m.def("BrickRed", &cppcolormap::BrickRed, DOC("BrickRed"), py::arg("N") = 1);
    m.def("Brown", &cppcolormap::Brown, DOC("Brown"), py::arg("N") = 1);
    m.def("BurntOrange", &cppcolormap::BurntOrange, DOC("BurntOrange"), py::arg("N") = 1);
    m.def("CadetBlue", &cppcolormap::CadetBlue, DOC("CadetBlue"), py::arg("N") = 1);
    m.def("CarnationPink", &cppcolormap::CarnationPink, DOC("CarnationPink"), py::arg("N") = 1);
    m.def("Cerulean", &cppcolormap::Cerulean, DOC("Cerulean"), py::arg("N") = 1);
    m.def("CornflowerBlue", &cppcolormap::CornflowerBlue, DOC("CornflowerBlue"), py::arg("N") = 1);
    m.def("Cyan", &cppcolormap::Cyan, DOC("Cyan"), py::arg("N") = 1);
    m.def("Dandelion", &cppcolormap::Dandelion, DOC("Dandelion"), py::arg("N") = 1);
    m.def("DarkOrchid", &cppcolormap::DarkOrchid, DOC("DarkOrchid"), py::arg("N") = 1);
    m.def("Emerald", &cppcolormap::Emerald, DOC("Emerald"), py::arg("N") = 1);
    m.def("ForestGreen", &cppcolormap::ForestGreen, DOC("ForestGreen"), py::arg("N") = 1);
    m.def("Fuchsia", &cppcolormap::Fuchsia, DOC("Fuchsia"), py::arg("N") = 1);
    m.def("Goldenrod", &cppcolormap::Goldenrod, DOC("Goldenrod"), py::arg("N") = 1);
    m.def("Gray", &cppcolormap::Gray, DOC("Gray"), py::arg("N") = 1);
    m.def("Green", &cppcolormap::Green, DOC("Green"), py::arg("N") = 1);
    m.def("GreenYellow", &cppcolormap::GreenYellow, DOC("GreenYellow"), py::arg("N") = 1);
    m.def("Grey", &cppcolormap::Grey, DOC("Grey"), py::arg("N") = 1);
    m.def("JungleGreen", &cppcolormap::JungleGreen, DOC("JungleGreen"), py::arg("N") = 1);
    m.def("Lavender", &cppcolormap::Lavender, DOC("Lavender"), py::arg("N") = 1);
    m.def("LimeGreen", &cppcolormap::LimeGreen, DOC("LimeGreen"), py::arg("N") = 1);
    m.def("Magenta", &cppcolormap::Magenta, DOC("Magenta"), py::arg("N") = 1);
    m.def("Mahogany", &cppcolormap::Mahogany, DOC("Mahogany"), py::arg("N") = 1);
    m.def("Maroon", &cppcolormap::Maroon, DOC("Maroon"), py::arg("N") = 1);
    m.def("Melon", &cppcolormap::Melon, DOC("Melon"), py::arg("N") = 1);
    m.def("MidnightBlue", &cppcolormap::MidnightBlue, DOC("MidnightBlue"), py::arg("N") = 1);
    m.def("Mulberry", &cppcolormap::Mulberry, DOC("Mulberry"), py::arg("N") = 1);
    m.def("NavyBlue", &cppcolormap::NavyBlue, DOC("NavyBlue"), py::arg("N") = 1);
    m.def("OliveGreen", &cppcolormap::OliveGreen, DOC("OliveGreen"), py::arg("N") = 1);
    m.def("Orange", &cppcolormap::Orange, DOC("Orange"), py::arg("N") = 1);
    m.def("OrangeRed", &cppcolormap::OrangeRed, DOC("OrangeRed"), py::arg("N") = 1);
    m.def("Orchid", &cppcolormap::Orchid, DOC("Orchid"), py::arg("N") = 1);
    m.def("Peach", &cppcolormap::Peach, DOC("Peach"), py::arg("N") = 1);
    m.def("Periwinkle", &cppcolormap::Periwinkle, DOC("Periwinkle"), py::arg("N") = 1);
    m.def("PineGreen", &cppcolormap::PineGreen, DOC("PineGreen"), py::arg("N") = 1);
    m.def("Plum", &cppcolormap::Plum, DOC("Plum"), py::arg("N") = 1);
    m.def("ProcessBlue", &cppcolormap::ProcessBlue, DOC("ProcessBlue"), py::arg("N") = 1);
    m.def("Purple", &cppcolormap::Purple, DOC("Purple"), py::arg("N") = 1);
    m.def("RawSienna", &cppcolormap::RawSienna, DOC("RawSienna"), py::arg("N") = 1);
    m.def("Red", &cppcolormap::Red, DOC("Red"), py::arg("N") = 1);
    m.def("RedOrange", &cppcolormap::RedOrange, DOC("RedOrange"), py::arg("N") = 1);
    m.def("RedViolet", &cppcolormap::RedViolet, DOC("RedViolet"), py::arg("N") = 1);
    m.def("Rhodamine", &cppcolormap::Rhodamine, DOC("Rhodamine"), py::arg("N") = 1);
    m.def("RoyalBlue", &cppcolormap::RoyalBlue, DOC("RoyalBlue"), py::arg("N") = 1);
    m.def("RoyalPurple", &cppcolormap::RoyalPurple, DOC("RoyalPurple"), py::arg("N") = 1);
    m.def("RubineRed", &cppcolormap::RubineRed, DOC("RubineRed"), py::arg("N") = 1);
    m.def("Salmon", &cppcolormap::Salmon, DOC("Salmon"), py::arg("N") = 1);
    m.def("SeaGreen", &cppcolormap::SeaGreen, DOC("SeaGreen"), py::arg("N") = 1);
    m.def("Sepia", &cppcolormap::Sepia, DOC("Sepia"), py::arg("N") = 1);
    m.def("SkyBlue", &cppcolormap::SkyBlue, DOC("SkyBlue"), py::arg("N") = 1);
    m.def("SpringGreen", &cppcolormap::SpringGreen, DOC("SpringGreen"), py::arg("N") = 1);
    m.def("Tan", &cppcolormap::Tan, DOC("Tan"), py::arg("N") = 1);
    m.def("TealBlue", &cppcolormap::TealBlue, DOC("TealBlue"), py::arg("N") = 1);
    m.def("Thistle", &cppcolormap::Thistle, DOC("Thistle"), py::arg("N") = 1);
    m.def("tueblue", &cppcolormap::tueblue, DOC("tueblue"), py::arg("N") = 1);
    m.def("tuedarkblue", &cppcolormap::tuedarkblue, DOC("tuedarkblue"), py::arg("N") = 1);
    m.def("tuelightblue", &cppcolormap::tuelightblue, DOC("tuelightblue"), py::arg("N") = 1);
    m.def("tuewarmred", &cppcolormap::tuewarmred, DOC("tuewarmred"), py::arg("N") = 1);
    m.def("Turquoise", &cppcolormap::Turquoise, DOC("Turquoise"), py::arg("N") = 1);
    m.def("Violet", &cppcolormap::Violet, DOC("Violet"), py::arg("N") = 1);
    m.def("VioletRed", &cppcolormap::VioletRed, DOC("VioletRed"), py::arg("N") = 1);
    m.def("White", &cppcolormap::White, DOC("White"), py::arg("N") = 1);
    m.def("WildStrawberry", &cppcolormap::WildStrawberry, DOC("WildStrawberry"), py::arg("N") = 1);
    m.def("Yellow", &cppcolormap::Yellow, DOC("Yellow"), py::arg("N") = 1);
    m.def("YellowGreen", &cppcolormap::YellowGreen, DOC("YellowGreen"), py::arg("N") = 1);
    m.def("YellowOrange", &cppcolormap::YellowOrange, DOC("YellowOrange"), py::arg("N") = 1);

    m.def("xterm", &cppcolormap::xterm, DOC("xterm"));
    m.def("tue", &cppcolormap::tue, DOC("tue"));

    m.def("xterm_r", &cppcolormap::xterm_r, DOC("xterm_r"));
    m.def("tue_r", &cppcolormap::tue_r, DOC("tue_r"));

    m.def("colormap", &cppcolormap::colormap, DOC("colormap"), py::arg("cmap"), py::arg("N") = 256);

    m.def("colorcycle", &cppcolormap::colorcycle, DOC("colorcycle"), py::arg("cmap"));

    py::enum_<cppcolormap::metric>(m, "metric", ENUM("metric"))
        .value("euclidean", cppcolormap::metric::euclidean)
        .value("fast_perceptual", cppcolormap::metric::fast_perceptual)
        .value("perceptual", cppcolormap::metric::perceptual)
        .export_values();

    m.def("match", &cppcolormap::match, DOC("match"));

} // PYBIND11_MODULE
