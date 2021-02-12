/**

\file cppcolormap.h Colormaps in C++

(c - GPLv3) T.W.J. de Geus (Tom) | tom@geus.me | www.geus.me | github.com/tdegeus/cppcolormap

*/

#ifndef CPPCOLORMAP_H
#define CPPCOLORMAP_H

#define CPPCOLORMAP_VERSION_MAJOR 1
#define CPPCOLORMAP_VERSION_MINOR 3
#define CPPCOLORMAP_VERSION_PATCH 0

#define CPPCOLORMAP_VERSION_AT_LEAST(x,y,z) \
    (CPPCOLORMAP_VERSION_MAJOR > x || (CPPCOLORMAP_VERSION_MAJOR >= x && \
    (CPPCOLORMAP_VERSION_MINOR > y || (CPPCOLORMAP_VERSION_MINOR >= y && \
                                       CPPCOLORMAP_VERSION_PATCH >= z))))

#define CPPCOLORMAP_VERSION(x,y,z) \
    (CPPCOLORMAP_VERSION_MAJOR == x && \
     CPPCOLORMAP_VERSION_MINOR == y && \
     CPPCOLORMAP_VERSION_PATCH == z)

// use "M_PI" from "math.h"
#define _USE_MATH_DEFINES

#include <vector>
#include <string>
#include <cfloat>
#include <iostream>
#include <sstream>
#include <math.h>
#include <xtensor/xtensor.hpp>
#include <xtensor/xarray.hpp>
#include <xtensor/xview.hpp>
#include <xtensor/xsort.hpp>
#include <xtensor/xmath.hpp>
#include <xtensor/xmanipulation.hpp>

namespace cppcolormap
{

namespace detail
{

    /**
    See: https://www.codespeedy.com/convert-rgb-to-hex-color-code-in-cpp

    \param r Red [0..255].
    \param g Green [0..255].
    \param b Blue [0..255].
    \return Hex string.
    */
    std::string rgb2hex(size_t r, size_t g, size_t b)
    {
        std::stringstream ss;
        ss << "#" << std::hex << (r << 16 | g << 8 | b);
        return ss.str();
    }

    /**
    See: https://stackoverflow.com/questions/28104559/arduino-strange-behavior-converting-hex-to-rgb

    \param hex Hex string.
    \return RGB data.
    */
    xt::xtensor<size_t, 1> hex2rgb(std::string hex)
    {
        if (hex.at(0) == '#') {
            hex.erase(0, 1);
        }

        while (hex.length() != 6) {
            hex += "0";
        }

        xt::xtensor<size_t, 1> rgb = xt::empty<size_t>({size_t(3)});

        size_t h = (size_t)std::stol(&hex[0], nullptr, 16);

        rgb(0) = h >> 16;
        rgb(1) = h >> 8 & 0xFF;
        rgb(2) = h & 0xFF;

        return rgb;
    }

} // namespace detail

/**
Convert RGB -> HEX.

\param data RGB data.
\returns Vector of strings.
*/
std::vector<std::string> rgb2hex(const xt::xtensor<double, 2>& data)
{
    std::vector<std::string> out;

    for (size_t i = 0; i < data.shape(0); ++i) {
        out.push_back(detail::rgb2hex(
            static_cast<size_t>(data(i, 0) * 255.0),
            static_cast<size_t>(data(i, 1) * 255.0),
            static_cast<size_t>(data(i, 2) * 255.0)));
    }

    return out;
}

/**
Convert RGB -> HEX.

\param data RGB data.
\returns String.
*/
std::string rgb2hex(const xt::xtensor<double, 1>& data)
{
    return detail::rgb2hex(
        static_cast<size_t>(data(0) * 255.0),
        static_cast<size_t>(data(1) * 255.0),
        static_cast<size_t>(data(2) * 255.0));
}

/**
Convert HEX -> RGB.

\param hex HEX data.
\returns RGB data.
*/
xt::xtensor<double, 2> hex2rgb(const std::vector<std::string>& hex)
{
    xt::xtensor<double, 2> out = xt::empty<double>({hex.size(), size_t(3)});

    for (size_t i = 0; i < hex.size(); ++i) {
        xt::view(out, i, xt::all()) = detail::hex2rgb(hex[i]);
    }

    return out / 255.0;
}

/**
Convert HEX -> RGB.

\param hex HEX data.
\returns RGB data.
*/
xt::xtensor<double, 1> hex2rgb(const std::string& hex)
{
    return detail::hex2rgb(hex) / 255.0;
}

/**
Interpolate the individual colours.

\param data RGB data.
\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> interp(const xt::xtensor<double, 2>& data, size_t N)
{
    if (N == data.shape(0)) {
        return data;
    }

    xt::xtensor<double, 2> out = xt::empty<double>({N, data.shape(1)});

    xt::xtensor<double, 1> x = xt::linspace(0.0, 1.0, data.shape(0));
    xt::xtensor<double, 1> xi = xt::linspace(0.0, 1.0, N);

    for (size_t j = 0; j < data.shape(1); j++) {
        auto c = xt::view(data, xt::all(), j);
        auto ci = xt::view(out, xt::all(), j);
        ci = xt::interp(xi, x, c);
    }

    return out;
}

namespace detail {

    template <class D, class C, typename V, class R>
    inline void as_colors_func(const D& data, const C& colors, V vmin, V vmax, R& ret)
    {
        assert(vmax > vmin);
        assert(colors.shape(0) > 0);

        auto d = xt::eval((data - vmin) / (vmax - vmin));
        d = xt::where(data < vmin, 0, d);
        d = xt::where(data > vmax, 1, d);
        auto index = xt::eval(xt::cast<size_t>(d * (colors.shape(0) - 1)));
        size_t stride = colors.shape(1);

        for (size_t i = 0; i < data.size(); ++i) {
            size_t j = index.data()[i];
            std::copy(
                &colors.data()[j * stride],
                &colors.data()[(j + 1) * stride],
                &ret.data()[i * stride]);
        }
    }

    template <class E, typename = void>
    struct as_colors_impl
    {
        template <typename T, typename S>
        static xt::xarray<T> run(const E& data, const xt::xtensor<T, 2> colors, S vmin, S vmax)
        {
            size_t N = data.dimension();
            std::vector<size_t> shape(N + 1);
            std::copy(data.shape().cbegin(), data.shape().cend(), shape.begin());
            shape[N] = colors.shape(1);
            xt::xarray<T> ret = xt::empty<T>(shape);
            as_colors_func(data, colors, vmin, vmax, ret);
            return ret;
        }
    };

    template <class E>
    struct as_colors_impl<E, typename xt::has_fixed_rank_t<E>>
    {
        using value_type = typename E::value_type;
        constexpr static size_t N = xt::get_rank<E>::value;

        template <typename T, typename S>
        static xt::xtensor<T, N + 1> run(
            const E& data,
            const xt::xtensor<T, 2> colors,
            S vmin,
            S vmax)
        {
            std::array<size_t, N + 1> shape;
            std::copy(data.shape().cbegin(), data.shape().cend(), shape.begin());
            shape[N] = colors.shape(1);
            xt::xtensor<T, N + 1> ret = xt::empty<T>(shape);
            detail::as_colors_func(data, colors, vmin, vmax, ret);
            return ret;
        }
    };
}

/**
Convert data to colors using a colormap.

\param data The data.
\param colors The colormap, e.g. ``cppcolormap::jet()``.
\param vmin The lower limit of the color-axis.
\param vmax The upper limit of the color-axis.
*/
template <class E, typename T, typename S>
inline auto as_colors(const E& data, const xt::xtensor<T, 2> colors, S vmin, S vmax)
{
    return detail::as_colors_impl<E>::run(data, colors, vmin, vmax);
}

/**
Convert data to colors using a colormap.

\param data The data.
\param colors The colormap, e.g. ``cppcolormap::jet()``.
*/
template <class E, typename T>
inline auto as_colors(const E& data, const xt::xtensor<T, 2> colors)
{
    return detail::as_colors_impl<E>::run(data, colors, xt::amin(data)(), xt::amax(data)());
}


/**
Qualitative colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Accent(size_t N = 8)
{
    xt::xtensor<double, 2> data = {
        {127, 201, 127},
        {190, 174, 212},
        {253, 192, 134},
        {255, 255, 153},
        { 56, 108, 176},
        {240,   2, 127},
        {191,  91,  23},
        {102, 102, 102},
    };

    return interp(data / 255.0, N);
}

/**
Qualitative colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Dark2(size_t N = 8)
{
    xt::xtensor<double, 2> data = {
        { 27, 158, 119},
        {217,  95,   2},
        {117, 112, 179},
        {231,  41, 138},
        {102, 166,  30},
        {230, 171,   2},
        {166, 118,  29},
        {102, 102, 102},
    };

    return interp(data / 255.0, N);
}

/**
Qualitative colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Paired(size_t N = 12)
{
    xt::xtensor<double, 2> data = {
        {166, 206, 227},
        { 31, 120, 180},
        {178, 223, 138},
        { 51, 160,  44},
        {251, 154, 153},
        {227,  26,  28},
        {253, 191, 111},
        {255, 127,   0},
        {202, 178, 214},
        {106,  61, 154},
        {255, 255, 153},
        {177,  89,  40},
    };

    return interp(data / 255.0, N);
}

/**
Qualitative colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Spectral(size_t N = 11)
{
    xt::xtensor<double, 2> data = {
        {158,   1,  66},
        {213,  62,  79},
        {244, 109,  67},
        {253, 174,  97},
        {254, 224, 139},
        {255, 255, 191},
        {230, 245, 152},
        {171, 221, 164},
        {102, 194, 165},
        { 50, 136, 189},
        { 94,  79, 162},
    };

    return interp(data / 255.0, N);
}

/**
Qualitative colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Pastel1(size_t N = 9)
{
    xt::xtensor<double, 2> data = {
        {251, 180, 174},
        {179, 205, 227},
        {204, 235, 197},
        {222, 203, 228},
        {254, 217, 166},
        {255, 255, 204},
        {229, 216, 189},
        {253, 218, 236},
        {242, 242, 242},
    };

    return interp(data / 255.0, N);
}

/**
Qualitative colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Pastel2(size_t N = 8)
{
    xt::xtensor<double, 2> data = {
        {179, 226, 205},
        {253, 205, 172},
        {203, 213, 232},
        {244, 202, 228},
        {230, 245, 201},
        {255, 242, 174},
        {241, 226, 204},
        {204, 204, 204},
    };

    return interp(data / 255.0, N);
}

/**
Qualitative colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Set1(size_t N = 9)
{
    xt::xtensor<double, 2> data = {
        {228,  26,  28},
        { 55, 126, 184},
        { 77, 175,  74},
        {152,  78, 163},
        {255, 127,   0},
        {255, 255,  51},
        {166,  86,  40},
        {247, 129, 191},
        {153, 153, 153},
    };

    return interp(data / 255.0, N);
}

/**
Qualitative colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Set2(size_t N = 8)
{
    xt::xtensor<double, 2> data = {
        {102, 194, 165},
        {252, 141,  98},
        {141, 160, 203},
        {231, 138, 195},
        {166, 216,  84},
        {255, 217,  47},
        {229, 196, 148},
        {179, 179, 179},
    };

    return interp(data / 255.0, N);
}

/**
Qualitative colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Set3(size_t N = 12)
{
    xt::xtensor<double, 2> data = {
        {141, 211, 199},
        {255, 255, 179},
        {190, 186, 218},
        {251, 128, 114},
        {128, 177, 211},
        {253, 180,  98},
        {179, 222, 105},
        {252, 205, 229},
        {217, 217, 217},
        {188, 128, 189},
        {204, 235, 197},
        {255, 237, 111},
    };

    return interp(data / 255.0, N);
}

/**
Sequential colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Blues(size_t N = 9)
{
    xt::xtensor<double, 2> data = {
        {247, 251, 255},
        {222, 235, 247},
        {198, 219, 239},
        {158, 202, 225},
        {107, 174, 214},
        { 66, 146, 198},
        { 33, 113, 181},
        {  8,  81, 156},
        {  8,  48, 107},
    };

    return interp(data / 255.0, N);
}

/**
Sequential colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Greens(size_t N = 9)
{
    xt::xtensor<double, 2> data = {
        {247, 252, 245},
        {229, 245, 224},
        {199, 233, 192},
        {161, 217, 155},
        {116, 196, 118},
        { 65, 171,  93},
        { 35, 139,  69},
        {  0, 109,  44},
        {  0,  68,  27},
    };

    return interp(data / 255.0, N);
}

/**
Sequential colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Greys(size_t N = 2)
{
    xt::xtensor<double, 2> data = {
        {255, 255, 255},
        {  0,   0,   0},
    };

    return interp(data / 255.0, N);
}

/**
Sequential colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Oranges(size_t N = 9)
{
    xt::xtensor<double, 2> data = {
        {255, 245, 235},
        {254, 230, 206},
        {253, 208, 162},
        {253, 174, 107},
        {253, 141,  60},
        {241, 105,  19},
        {217,  72,   1},
        {166,  54,   3},
        {127,  39,   4},
    };

    return interp(data / 255.0, N);
}

/**
Sequential colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Purples(size_t N = 9)
{
    xt::xtensor<double, 2> data = {
        {252, 251, 253},
        {239, 237, 245},
        {218, 218, 235},
        {188, 189, 220},
        {158, 154, 200},
        {128, 125, 186},
        {106,  81, 163},
        { 84,  39, 143},
        { 63,   0, 125},
    };

    return interp(data / 255.0, N);
}

/**
Sequential colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Reds(size_t N = 9)
{
    xt::xtensor<double, 2> data = {
        {255, 245, 240},
        {254, 224, 210},
        {252, 187, 161},
        {252, 146, 114},
        {251, 106,  74},
        {239,  59,  44},
        {203,  24,  29},
        {165,  15,  21},
        {103,   0,  13},
    };

    return interp(data / 255.0, N);
}

/**
Sequential colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> BuPu(size_t N = 9)
{
    xt::xtensor<double, 2> data = {
        {247, 252, 253},
        {224, 236, 244},
        {191, 211, 230},
        {158, 188, 218},
        {140, 150, 198},
        {140, 107, 177},
        {136,  65, 157},
        {129,  15, 124},
        { 77,   0,  75},
    };

    return interp(data / 255.0, N);
}

/**
Sequential colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> GnBu(size_t N = 9)
{
    xt::xtensor<double, 2> data = {
        {247, 252, 240},
        {224, 243, 219},
        {204, 235, 197},
        {168, 221, 181},
        {123, 204, 196},
        { 78, 179, 211},
        { 43, 140, 190},
        {  8, 104, 172},
        {  8,  64, 129},
    };

    return interp(data / 255.0, N);
}

/**
Sequential colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> PuBu(size_t N = 9)
{
    xt::xtensor<double, 2> data = {
        {255, 247, 251},
        {236, 231, 242},
        {208, 209, 230},
        {166, 189, 219},
        {116, 169, 207},
        { 54, 144, 192},
        {  5, 112, 176},
        {  4,  90, 141},
        {  2,  56,  88},
    };

    return interp(data / 255.0, N);
}

/**
Sequential colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> PuBuGn(size_t N = 9)
{
    xt::xtensor<double, 2> data = {
        {255, 247, 251},
        {236, 226, 240},
        {208, 209, 230},
        {166, 189, 219},
        {103, 169, 207},
        { 54, 144, 192},
        {  2, 129, 138},
        {  1, 108,  89},
        {  1,  70,  54},
    };

    return interp(data / 255.0, N);
}

/**
Sequential colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> PuRd(size_t N = 9)
{
    xt::xtensor<double, 2> data = {
        {247, 244, 249},
        {231, 225, 239},
        {212, 185, 218},
        {201, 148, 199},
        {223, 101, 176},
        {231,  41, 138},
        {206,  18,  86},
        {152,   0,  67},
        {103,   0,  31},
    };

    return interp(data / 255.0, N);
}

/**
Sequential colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> RdPu(size_t N = 9)
{
    xt::xtensor<double, 2> data = {
        {255, 247, 243},
        {253, 224, 221},
        {252, 197, 192},
        {250, 159, 181},
        {247, 104, 161},
        {221,  52, 151},
        {174,   1, 126},
        {122,   1, 119},
        { 73,   0, 106},
    };

    return interp(data / 255.0, N);
}

/**
Sequential colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> OrRd(size_t N = 9)
{
    xt::xtensor<double, 2> data = {
        {255, 247, 236},
        {254, 232, 200},
        {253, 212, 158},
        {253, 187, 132},
        {252, 141,  89},
        {239, 101,  72},
        {215,  48,  31},
        {179,   0,   0},
        {127,   0,   0},
    };

    return interp(data / 255.0, N);
}

/**
Sequential colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> RdOrYl(size_t N = 9)
{
    xt::xtensor<double, 2> data = {
        {128, 0  , 38 },
        {189, 0  , 38 },
        {227, 26 , 28 },
        {252, 78 , 42 },
        {253, 141, 60 },
        {254, 178, 76 },
        {254, 217, 118},
        {255, 237, 160},
        {255, 255, 204},
    };

    return interp(data / 255.0, N);
}

/**
Sequential colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> YlGn(size_t N = 9)
{
    xt::xtensor<double, 2> data = {
        {255, 255, 229},
        {247, 252, 185},
        {217, 240, 163},
        {173, 221, 142},
        {120, 198, 121},
        { 65, 171,  93},
        { 35, 132,  67},
        {  0, 104,  55},
        {  0,  69,  41},
    };

    return interp(data / 255.0, N);
}

/**
Sequential colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> YlGnBu(size_t N = 9)
{
    xt::xtensor<double, 2> data = {
        {255, 255, 217},
        {237, 248, 177},
        {199, 233, 180},
        {127, 205, 187},
        { 65, 182, 196},
        { 29, 145, 192},
        { 34,  94, 168},
        { 37,  52, 148},
        {  8,  29,  88},
    };

    return interp(data / 255.0, N);
}

/**
Sequential colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> YlOrRd(size_t N = 9)
{
    xt::xtensor<double, 2> data = {
        {255, 255, 204},
        {255, 237, 160},
        {254, 217, 118},
        {254, 178,  76},
        {253, 141,  60},
        {252,  78,  42},
        {227,  26,  28},
        {189,   0,  38},
        {128,   0,  38},
    };

    return interp(data / 255.0, N);
}

/**
Diverging colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> BrBG(size_t N = 11)
{
    xt::xtensor<double, 2> data = {
        { 84,  48,   5},
        {140,  81,  10},
        {191, 129,  45},
        {223, 194, 125},
        {246, 232, 195},
        {245, 245, 245},
        {199, 234, 229},
        {128, 205, 193},
        { 53, 151, 143},
        {  1, 102,  94},
        {  0,  60,  48},
    };

    return interp(data / 255.0, N);
}

/**
Diverging colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> PuOr(size_t N = 11)
{
    xt::xtensor<double, 2> data = {
        {127,  59,   8},
        {179,  88,   6},
        {224, 130,  20},
        {253, 184,  99},
        {254, 224, 182},
        {247, 247, 247},
        {216, 218, 235},
        {178, 171, 210},
        {128, 115, 172},
        { 84,  39, 136},
        { 45,   0,  75},
    };

    return interp(data / 255.0, N);
}

/**
Diverging colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> RdBu(size_t N = 11)
{
    xt::xtensor<double, 2> data = {
        {103,   0,  31},
        {178,  24,  43},
        {214,  96,  77},
        {244, 165, 130},
        {253, 219, 199},
        {247, 247, 247},
        {209, 229, 240},
        {146, 197, 222},
        { 67, 147, 195},
        { 33, 102, 172},
        {  5,  48,  97},
    };

    return interp(data / 255.0, N);
}

/**
Diverging colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> RdGy(size_t N = 11)
{
    xt::xtensor<double, 2> data = {
        {103,   0,  31},
        {178,  24,  43},
        {214,  96,  77},
        {244, 165, 130},
        {253, 219, 199},
        {255, 255, 255},
        {224, 224, 224},
        {186, 186, 186},
        {135, 135, 135},
        { 77,  77,  77},
        { 26,  26,  26},
    };

    return interp(data / 255.0, N);
}

/**
Diverging colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> RdYlBu(size_t N = 11)
{
    xt::xtensor<double, 2> data = {
        {165,   0,  38},
        {215,  48,  39},
        {244, 109,  67},
        {253, 174,  97},
        {254, 224, 144},
        {255, 255, 191},
        {224, 243, 248},
        {171, 217, 233},
        {116, 173, 209},
        { 69, 117, 180},
        { 49,  54, 149},
    };

    return interp(data / 255.0, N);
}

/**
Diverging colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> RdYlGn(size_t N = 11)
{
    xt::xtensor<double, 2> data = {
        {165,   0,  38},
        {215,  48,  39},
        {244, 109,  67},
        {253, 174,  97},
        {254, 224, 139},
        {255, 255, 191},
        {217, 239, 139},
        {166, 217, 106},
        {102, 189,  99},
        { 26, 152,  80},
        {  0, 104,  55},
    };

    return interp(data / 255.0, N);
}

/**
Diverging colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> PiYG(size_t N = 11)
{
    xt::xtensor<double, 2> data = {
        {142,   1,  82},
        {197,  27, 125},
        {222, 119, 174},
        {241, 182, 218},
        {253, 224, 239},
        {247, 247, 247},
        {230, 245, 208},
        {184, 225, 134},
        {127, 188,  65},
        { 77, 146,  33},
        { 39, 100,  25},
    };

    return interp(data / 255.0, N);
}

/**
Diverging colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> PRGn(size_t N = 11)
{
    xt::xtensor<double, 2> data = {
        { 64,   0,  75},
        {118,  42, 131},
        {153, 112, 171},
        {194, 165, 207},
        {231, 212, 232},
        {247, 247, 247},
        {217, 240, 211},
        {166, 219, 160},
        { 90, 174,  97},
        { 27, 120,  55},
        {  0,  68,  27},
    };

    return interp(data / 255.0, N);
}

namespace detail
{

    inline xt::xtensor<double, 1> from_anchor_color(size_t N, const xt::xtensor<double, 2>& x)
    {
        size_t n = x.shape(0);
        xt::xtensor<size_t, 1> idx = xt::view(x, xt::all(), 0) * (double)N;
        idx(0) = 0;
        idx(n - 1) = N;

        xt::xtensor<double, 1> ret = xt::empty<double>({N});

        for (size_t i = 0; i < n - 1; ++i) {
            xt::view(ret, xt::range(idx(i), idx(i + 1))) =
                x(i, 2) + (x(i + 1, 1) - x(i, 2)) * xt::linspace<double>(0.0, 1.0, idx(i + 1) - idx(i));
        }

        return ret;
    }

    inline xt::xtensor<double, 2> from_anchor(
        size_t N,
        const xt::xtensor<double, 2>& r,
        const xt::xtensor<double, 2>& g,
        const xt::xtensor<double, 2>& b)
    {
        std::array<size_t, 2> shape = {N, 3};
        xt::xtensor<double, 2> ret(shape);
        xt::view(ret, xt::all(), 0) = from_anchor_color(N, r);
        xt::view(ret, xt::all(), 1) = from_anchor_color(N, g);
        xt::view(ret, xt::all(), 2) = from_anchor_color(N, b);
        return ret;
    }

} // namespace detail

/**
matplotlib colormap, from anchor.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> spring(size_t N = 256)
{
    xt::xtensor<double, 2> r = {
        {0.0, 1.0, 1.0},
        {1.0, 1.0, 1.0}};

    xt::xtensor<double, 2> g = {
        {0.0, 0.0, 0.0},
        {1.0, 1.0, 1.0}};

    xt::xtensor<double, 2> b = {
        {0.0, 1.0, 1.0},
        {1.0, 0.0, 0.0}};

    return detail::from_anchor(N, r, g, b);
}

/**
matplotlib colormap, from anchor.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> summer(size_t N = 256)
{
    xt::xtensor<double, 2> r = {
        {0.0, 0.0, 0.0},
        {1.0, 1.0, 1.0}};

    xt::xtensor<double, 2> g = {
        {0.0, 0.5, 0.5},
        {1.0, 1.0, 1.0}};

    xt::xtensor<double, 2> b = {
        {0.0, 0.4, 0.4},
        {1.0, 0.4, 0.4}};

    return detail::from_anchor(N, r, g, b);
}

/**
matplotlib colormap, from anchor.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> autumn(size_t N = 256)
{
    xt::xtensor<double, 2> r = {
        {0.0, 1.0, 1.0},
        {1.0, 1.0, 1.0}};

    xt::xtensor<double, 2> g = {
        {0.0, 0.0, 0.0},
        {1.0, 1.0, 1.0}};

    xt::xtensor<double, 2> b = {
        {0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0}};

    return detail::from_anchor(N, r, g, b);
}

/**
matplotlib colormap, from anchor.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> winter(size_t N = 256)
{
    xt::xtensor<double, 2> r = {
        {0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0}};

    xt::xtensor<double, 2> g = {
        {0.0, 0.0, 0.0},
        {1.0, 1.0, 1.0}};

    xt::xtensor<double, 2> b = {
        {0.0, 1.0, 1.0},
        {1.0, 0.5, 0.5}};

    return detail::from_anchor(N, r, g, b);
}

/**
matplotlib colormap, from anchor.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> bone(size_t N = 256)
{
    xt::xtensor<double, 2> r = {
        {0.0, 0.0, 0.0},
        {0.746032, 0.652778, 0.652778},
        {1.0, 1.0, 1.0}};

    xt::xtensor<double, 2> g = {
        {0.0, 0.0, 0.0},
        {0.365079, 0.319444, 0.319444},
        {0.746032, 0.777778, 0.777778},
        {1.0, 1.0, 1.0}};

    xt::xtensor<double, 2> b = {
        {0.0, 0.0, 0.0},
        {0.365079, 0.444444, 0.444444},
        {1.0, 1.0, 1.0}};

    return detail::from_anchor(N, r, g, b);
}

/**
matplotlib colormap, from anchor.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> cool(size_t N = 256)
{
    xt::xtensor<double, 2> r = {
        {0.0, 0.0, 0.0},
        {1.0, 1.0, 1.0}};

    xt::xtensor<double, 2> g = {
        {0.0, 1.0, 1.0},
        {1.0, 0.0, 0.0}};

    xt::xtensor<double, 2> b = {
        {0.0, 1.0, 1.0},
        {1.0, 1.0, 1.0}};

    return detail::from_anchor(N, r, g, b);
}

/**
matplotlib colormap, from anchor.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> hot(size_t N = 256)
{
    xt::xtensor<double, 2> r = {
        {0.0, 0.0416, 0.0416},
        {0.365079, 1.000000, 1.000000},
        {1.0, 1.0, 1.0}};

    xt::xtensor<double, 2> g = {
        {0.0, 0.0, 0.0},
        {0.365079, 0.000000, 0.000000},
        {0.746032, 1.000000, 1.000000},
        {1.0, 1.0, 1.0}};

    xt::xtensor<double, 2> b = {
        {0.0, 0.0, 0.0},
        {0.746032, 0.000000, 0.000000},
        {1.0, 1.0, 1.0}};

    return detail::from_anchor(N, r, g, b);
}

/**
matplotlib colormap, from anchor.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> copper(size_t N = 256)
{
    xt::xtensor<double, 2> r = {
        {0.0, 0.0, 0.0},
        {0.809524, 1.000000, 1.000000},
        {1.0, 1.0, 1.0}};

    xt::xtensor<double, 2> g = {
        {0.0, 0.0, 0.0},
        {1.0, 0.7812, 0.7812}};

    xt::xtensor<double, 2> b = {
        {0.0, 0.0, 0.0},
        {1.0, 0.4975, 0.4975}};

    return detail::from_anchor(N, r, g, b);
}

/**
matplotlib colormap, from anchor.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> hsv(size_t N = 256)
{
    xt::xtensor<double, 2> r = {
        {0.0, 1.0, 1.0},
        {0.158730, 1.000000, 1.000000},
        {0.174603, 0.968750, 0.968750},
        {0.333333, 0.031250, 0.031250},
        {0.349206, 0.000000, 0.000000},
        {0.666667, 0.000000, 0.000000},
        {0.682540, 0.031250, 0.031250},
        {0.841270, 0.968750, 0.968750},
        {0.857143, 1.000000, 1.000000},
        {1.0, 1.0, 1.0}};

    xt::xtensor<double, 2> g = {
        {0.0, 0.0, 0.0},
        {0.158730, 0.937500, 0.937500},
        {0.174603, 1.000000, 1.000000},
        {0.507937, 1.000000, 1.000000},
        {0.666667, 0.062500, 0.062500},
        {0.682540, 0.000000, 0.000000},
        {1.0, 0.0, 0.0}};

    xt::xtensor<double, 2> b = {
        {0.0, 0.0, 0.0},
        {0.333333, 0.000000, 0.000000},
        {0.349206, 0.062500, 0.062500},
        {0.507937, 1.000000, 1.000000},
        {0.841270, 1.000000, 1.000000},
        {0.857143, 0.937500, 0.937500},
        {1.0, 0.09375, 0.09375}};

    return detail::from_anchor(N, r, g, b);
}

/**
matplotlib colormap, from anchor.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> nipy_spectral(size_t N = 256)
{
    xt::xtensor<double, 2> r = {
        {0.0, 0.0, 0.0},
        {0.05, 0.4667, 0.4667},
        {0.10, 0.5333, 0.5333},
        {0.15, 0.0, 0.0},
        {0.20, 0.0, 0.0},
        {0.25, 0.0, 0.0},
        {0.30, 0.0, 0.0},
        {0.35, 0.0, 0.0},
        {0.40, 0.0, 0.0},
        {0.45, 0.0, 0.0},
        {0.50, 0.0, 0.0},
        {0.55, 0.0, 0.0},
        {0.60, 0.0, 0.0},
        {0.65, 0.7333, 0.7333},
        {0.70, 0.9333, 0.9333},
        {0.75, 1.0, 1.0},
        {0.80, 1.0, 1.0},
        {0.85, 1.0, 1.0},
        {0.90, 0.8667, 0.8667},
        {0.95, 0.80, 0.80},
        {1.0, 0.80, 0.80}};

    xt::xtensor<double, 2> g = {
        {0.0, 0.0, 0.0},
        {0.05, 0.0, 0.0},
        {0.10, 0.0, 0.0},
        {0.15, 0.0, 0.0},
        {0.20, 0.0, 0.0},
        {0.25, 0.4667, 0.4667},
        {0.30, 0.6000, 0.6000},
        {0.35, 0.6667, 0.6667},
        {0.40, 0.6667, 0.6667},
        {0.45, 0.6000, 0.6000},
        {0.50, 0.7333, 0.7333},
        {0.55, 0.8667, 0.8667},
        {0.60, 1.0, 1.0},
        {0.65, 1.0, 1.0},
        {0.70, 0.9333, 0.9333},
        {0.75, 0.8000, 0.8000},
        {0.80, 0.6000, 0.6000},
        {0.85, 0.0, 0.0},
        {0.90, 0.0, 0.0},
        {0.95, 0.0, 0.0},
        {1.0, 0.80, 0.80}};

    xt::xtensor<double, 2> b = {
        {0.0, 0.0, 0.0},
        {0.05, 0.5333, 0.5333},
        {0.10, 0.6000, 0.6000},
        {0.15, 0.6667, 0.6667},
        {0.20, 0.8667, 0.8667},
        {0.25, 0.8667, 0.8667},
        {0.30, 0.8667, 0.8667},
        {0.35, 0.6667, 0.6667},
        {0.40, 0.5333, 0.5333},
        {0.45, 0.0, 0.0},
        {0.5, 0.0, 0.0},
        {0.55, 0.0, 0.0},
        {0.60, 0.0, 0.0},
        {0.65, 0.0, 0.0},
        {0.70, 0.0, 0.0},
        {0.75, 0.0, 0.0},
        {0.80, 0.0, 0.0},
        {0.85, 0.0, 0.0},
        {0.90, 0.0, 0.0},
        {0.95, 0.0, 0.0},
        {1.0, 0.80, 0.80}};

    return detail::from_anchor(N, r, g, b);
}

/**
matplotlib colormap, from anchor.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> jet(size_t N = 256)
{
    xt::xtensor<double, 2> r = {
        {0.00, 0.0, 0.0},
        {0.35, 0.0, 0.0},
        {0.66, 1.0, 1.0},
        {0.89, 1.0, 1.0},
        {1.00, 0.5, 0.5}};

    xt::xtensor<double, 2> g = {
        {0.000, 0.0, 0.0},
        {0.125, 0.0, 0.0},
        {0.375, 1.0, 1.0},
        {0.640, 1.0, 1.0},
        {0.910, 0.0, 0.0},
        {1.000, 0.0, 0.0}};

    xt::xtensor<double, 2> b = {
        {0.00, 0.5, 0.5},
        {0.11, 1.0, 1.0},
        {0.34, 1.0, 1.0},
        {0.65, 0.0, 0.0},
        {1.00, 0.0, 0.0}};

    return detail::from_anchor(N, r, g, b);
}

namespace detail
{

    inline xt::xtensor<double, 2> from_fraction(size_t N, const xt::xtensor<double, 2>& x)
    {
        size_t n = x.shape(0);
        xt::xtensor<size_t, 1> idx = xt::view(x, xt::all(), 0) * (double)N;
        idx(0) = 0;
        idx(n - 1) = N;

        std::array<size_t, 2> shape = {N, 3};
        xt::xtensor<double, 2> ret(shape);

        for (size_t i = 0; i < n - 1; ++i) {
            for (size_t j = 0; j < 3; ++j) {
                xt::view(ret, xt::range(idx(i), idx(i + 1)), j)
                    = x(i, j + 1)
                    + (x(i + 1, j + 1) - x(i, j + 1))
                    * xt::linspace<double>(0.0, 1.0, idx(i + 1) - idx(i));
            }
        }

        return ret;
    }

} // namespace detail

/**
matplotlib colormap, from fraction.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> terrain(size_t N = 6)
{
    xt::xtensor<double, 2> data = {
        {0.00, 0.2, 0.2, 0.6},
        {0.15, 0.0, 0.6, 1.0},
        {0.25, 0.0, 0.8, 0.4},
        {0.50, 1.0, 1.0, 0.6},
        {0.75, 0.5, 0.36, 0.33},
        {1.00, 1.0, 1.0, 1.0}};

    return detail::from_fraction(N, data);
}

namespace detail
{
    // Gnuplot palette functions
    inline xt::xtensor<double, 1> _g0(const xt::xtensor<double, 1>& x)
    {
        return xt::zeros_like(x);
    }

    inline xt::xtensor<double, 1> _g1(const xt::xtensor<double, 1>& x)
    {
        return 0.5 * xt::ones_like(x);
    }

    inline xt::xtensor<double, 1> _g2(const xt::xtensor<double, 1>& x)
    {
        return xt::ones_like(x);
    }

    inline xt::xtensor<double, 1> _g3(const xt::xtensor<double, 1>& x)
    {
        return x;
    }

    inline xt::xtensor<double, 1> _g4(const xt::xtensor<double, 1>& x)
    {
        return xt::pow(x, 2.0);
    }

    inline xt::xtensor<double, 1> _g5(const xt::xtensor<double, 1>& x)
    {
        return xt::pow(x, 3.0);
    }

    inline xt::xtensor<double, 1> _g6(const xt::xtensor<double, 1>& x)
    {
        return xt::pow(x, 4.0);
    }

    inline xt::xtensor<double, 1> _g7(const xt::xtensor<double, 1>& x)
    {
        return xt::sqrt(x);
    }

    inline xt::xtensor<double, 1> _g8(const xt::xtensor<double, 1>& x)
    {
        return xt::sqrt(xt::sqrt(x));
    }

    inline xt::xtensor<double, 1> _g9(const xt::xtensor<double, 1>& x)
    {
        return xt::sin(x * M_PI * 0.5);
    }

    inline xt::xtensor<double, 1> _g10(const xt::xtensor<double, 1>& x)
    {
        return xt::cos(x * M_PI * 0.5);
    }

    inline xt::xtensor<double, 1> _g11(const xt::xtensor<double, 1>& x)
    {
        return xt::abs(x - 0.5);
    }

    inline xt::xtensor<double, 1> _g12(const xt::xtensor<double, 1>& x)
    {
        return xt::pow(2.0 * x - 1.0, 2.0);
    }

    inline xt::xtensor<double, 1> _g13(const xt::xtensor<double, 1>& x)
    {
        return xt::sin(x * M_PI);
    }

    inline xt::xtensor<double, 1> _g14(const xt::xtensor<double, 1>& x)
    {
        return xt::abs(xt::cos(x * M_PI));
    }

    inline xt::xtensor<double, 1> _g15(const xt::xtensor<double, 1>& x)
    {
        return xt::sin(x * 2.0 * M_PI);
    }

    inline xt::xtensor<double, 1> _g16(const xt::xtensor<double, 1>& x)
    {
        return xt::cos(x * 2.0 * M_PI);
    }

    inline xt::xtensor<double, 1> _g17(const xt::xtensor<double, 1>& x)
    {
        return xt::abs(xt::sin(x * 2.0 * M_PI));
    }

    inline xt::xtensor<double, 1> _g18(const xt::xtensor<double, 1>& x)
    {
        return xt::abs(xt::cos(x * 2.0 * M_PI));
    }

    inline xt::xtensor<double, 1> _g19(const xt::xtensor<double, 1>& x)
    {
        return xt::abs(xt::sin(x * 4.0 * M_PI));
    }

    inline xt::xtensor<double, 1> _g20(const xt::xtensor<double, 1>& x)
    {
        return xt::abs(xt::cos(x * 4.0 * M_PI));
    }

    inline xt::xtensor<double, 1> _g21(const xt::xtensor<double, 1>& x)
    {
        return 3.0 * x;
    }

    inline xt::xtensor<double, 1> _g22(const xt::xtensor<double, 1>& x)
    {
        return 3.0 * x - 1.0;
    }

    inline xt::xtensor<double, 1> _g23(const xt::xtensor<double, 1>& x)
    {
        return 3.0 * x - 2.0;
    }

    inline xt::xtensor<double, 1> _g24(const xt::xtensor<double, 1>& x)
    {
        return xt::abs(3.0 * x - 1.0);
    }

    inline xt::xtensor<double, 1> _g25(const xt::xtensor<double, 1>& x)
    {
        return xt::abs(3.0 * x - 2.0);
    }

    inline xt::xtensor<double, 1> _g26(const xt::xtensor<double, 1>& x)
    {
        return (3.0 * x - 1.0) * 0.5;
    }

    inline xt::xtensor<double, 1> _g27(const xt::xtensor<double, 1>& x)
    {
        return (3.0 * x - 2.0) * 0.5;
    }

    inline xt::xtensor<double, 1> _g28(const xt::xtensor<double, 1>& x)
    {
        return xt::abs((3.0 * x - 1.0) * 0.5);
    }

    inline xt::xtensor<double, 1> _g29(const xt::xtensor<double, 1>& x)
    {
        return xt::abs((3.0 * x - 2.0) * 0.5);
    }

    inline xt::xtensor<double, 1> _g30(const xt::xtensor<double, 1>& x)
    {
        return x / 0.32 - 0.78125;
    }

    inline xt::xtensor<double, 1> _g31(const xt::xtensor<double, 1>& x)
    {
        return 2.0 * x - 0.84;
    }

    inline xt::xtensor<double, 1> _g32(const xt::xtensor<double, 1>& x)
    {
        auto ret = xt::zeros_like(x);
        ret = xt::where(xt::less(x, 0.25), 4.0 * x, ret);
        ret = xt::where(xt::greater_equal(x, 0.25) && xt::less(x, 0.92), -2.0 * x + 1.84, ret);
        ret = xt::where(xt::greater_equal(x, 0.92), x / 0.08 - 11.5, ret);
        return ret;
    }

    inline xt::xtensor<double, 1> _g33(const xt::xtensor<double, 1>& x)
    {
        return xt::abs(2.0 * x - 0.5);
    }

    inline xt::xtensor<double, 1> _g34(const xt::xtensor<double, 1>& x)
    {
        return 2.0 * x;
    }

    inline xt::xtensor<double, 1> _g35(const xt::xtensor<double, 1>& x)
    {
        return 2.0 * x - 0.5;
    }

    inline xt::xtensor<double, 1> _g36(const xt::xtensor<double, 1>& x)
    {
        return 2.0 * x - 1.0;
    }

    inline xt::xtensor<double, 1> gnu_palette(size_t i, const xt::xtensor<double, 1>& x)
    {
        xt::xtensor<double, 1> ret;

        if (i == 0) { ret = _g0(x); }
        else if (i == 1) { ret = _g1(x); }
        else if (i == 2) { ret = _g2(x); }
        else if (i == 3) { ret = _g3(x); }
        else if (i == 4) { ret = _g4(x); }
        else if (i == 5) { ret = _g5(x); }
        else if (i == 6) { ret = _g6(x); }
        else if (i == 7) { ret = _g7(x); }
        else if (i == 8) { ret = _g8(x); }
        else if (i == 9) { ret = _g9(x); }
        else if (i == 10) { ret = _g10(x); }
        else if (i == 11) { ret = _g11(x); }
        else if (i == 12) { ret = _g12(x); }
        else if (i == 13) { ret = _g13(x); }
        else if (i == 14) { ret = _g14(x); }
        else if (i == 15) { ret = _g15(x); }
        else if (i == 16) { ret = _g16(x); }
        else if (i == 17) { ret = _g17(x); }
        else if (i == 18) { ret = _g18(x); }
        else if (i == 19) { ret = _g19(x); }
        else if (i == 20) { ret = _g20(x); }
        else if (i == 21) { ret = _g21(x); }
        else if (i == 22) { ret = _g22(x); }
        else if (i == 23) { ret = _g23(x); }
        else if (i == 24) { ret = _g24(x); }
        else if (i == 25) { ret = _g25(x); }
        else if (i == 26) { ret = _g26(x); }
        else if (i == 27) { ret = _g27(x); }
        else if (i == 28) { ret = _g28(x); }
        else if (i == 29) { ret = _g29(x); }
        else if (i == 30) { ret = _g30(x); }
        else if (i == 31) { ret = _g31(x); }
        else if (i == 32) { ret = _g32(x); }
        else if (i == 33) { ret = _g33(x); }
        else if (i == 34) { ret = _g34(x); }
        else if (i == 35) { ret = _g35(x); }
        else if (i == 36) { ret = _g36(x); }
        else { throw std::runtime_error("gnu_palette out-of-bounds"); }

        ret = xt::where(xt::greater(ret, 1.0), 1.0, ret);
        ret = xt::where(xt::less(ret, 0.0), 0.0, ret);

        return ret;
    }

} // namespace detail

/**
GNU plot colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> afmhot(size_t N = 256)
{
    std::array<size_t, 2> shape = {N, 3};
    xt::xtensor<double, 2> data(shape);
    xt::xtensor<double, 1> x = xt::linspace<double>(0.0, 1.0, N);
    xt::view(data, xt::all(), 0) = detail::gnu_palette(34, x);
    xt::view(data, xt::all(), 1) = detail::gnu_palette(35, x);
    xt::view(data, xt::all(), 2) = detail::gnu_palette(36, x);
    return data;
}

/**
matplotlib colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> magma(size_t N = 256)
{
    xt::xtensor<double, 2> data = {
        {0.001462, 0.000466, 0.013866},
        {0.002258, 0.001295, 0.018331},
        {0.003279, 0.002305, 0.023708},
        {0.004512, 0.003490, 0.029965},
        {0.005950, 0.004843, 0.037130},
        {0.007588, 0.006356, 0.044973},
        {0.009426, 0.008022, 0.052844},
        {0.011465, 0.009828, 0.060750},
        {0.013708, 0.011771, 0.068667},
        {0.016156, 0.013840, 0.076603},
        {0.018815, 0.016026, 0.084584},
        {0.021692, 0.018320, 0.092610},
        {0.024792, 0.020715, 0.100676},
        {0.028123, 0.023201, 0.108787},
        {0.031696, 0.025765, 0.116965},
        {0.035520, 0.028397, 0.125209},
        {0.039608, 0.031090, 0.133515},
        {0.043830, 0.033830, 0.141886},
        {0.048062, 0.036607, 0.150327},
        {0.052320, 0.039407, 0.158841},
        {0.056615, 0.042160, 0.167446},
        {0.060949, 0.044794, 0.176129},
        {0.065330, 0.047318, 0.184892},
        {0.069764, 0.049726, 0.193735},
        {0.074257, 0.052017, 0.202660},
        {0.078815, 0.054184, 0.211667},
        {0.083446, 0.056225, 0.220755},
        {0.088155, 0.058133, 0.229922},
        {0.092949, 0.059904, 0.239164},
        {0.097833, 0.061531, 0.248477},
        {0.102815, 0.063010, 0.257854},
        {0.107899, 0.064335, 0.267289},
        {0.113094, 0.065492, 0.276784},
        {0.118405, 0.066479, 0.286321},
        {0.123833, 0.067295, 0.295879},
        {0.129380, 0.067935, 0.305443},
        {0.135053, 0.068391, 0.315000},
        {0.140858, 0.068654, 0.324538},
        {0.146785, 0.068738, 0.334011},
        {0.152839, 0.068637, 0.343404},
        {0.159018, 0.068354, 0.352688},
        {0.165308, 0.067911, 0.361816},
        {0.171713, 0.067305, 0.370771},
        {0.178212, 0.066576, 0.379497},
        {0.184801, 0.065732, 0.387973},
        {0.191460, 0.064818, 0.396152},
        {0.198177, 0.063862, 0.404009},
        {0.204935, 0.062907, 0.411514},
        {0.211718, 0.061992, 0.418647},
        {0.218512, 0.061158, 0.425392},
        {0.225302, 0.060445, 0.431742},
        {0.232077, 0.059889, 0.437695},
        {0.238826, 0.059517, 0.443256},
        {0.245543, 0.059352, 0.448436},
        {0.252220, 0.059415, 0.453248},
        {0.258857, 0.059706, 0.457710},
        {0.265447, 0.060237, 0.461840},
        {0.271994, 0.060994, 0.465660},
        {0.278493, 0.061978, 0.469190},
        {0.284951, 0.063168, 0.472451},
        {0.291366, 0.064553, 0.475462},
        {0.297740, 0.066117, 0.478243},
        {0.304081, 0.067835, 0.480812},
        {0.310382, 0.069702, 0.483186},
        {0.316654, 0.071690, 0.485380},
        {0.322899, 0.073782, 0.487408},
        {0.329114, 0.075972, 0.489287},
        {0.335308, 0.078236, 0.491024},
        {0.341482, 0.080564, 0.492631},
        {0.347636, 0.082946, 0.494121},
        {0.353773, 0.085373, 0.495501},
        {0.359898, 0.087831, 0.496778},
        {0.366012, 0.090314, 0.497960},
        {0.372116, 0.092816, 0.499053},
        {0.378211, 0.095332, 0.500067},
        {0.384299, 0.097855, 0.501002},
        {0.390384, 0.100379, 0.501864},
        {0.396467, 0.102902, 0.502658},
        {0.402548, 0.105420, 0.503386},
        {0.408629, 0.107930, 0.504052},
        {0.414709, 0.110431, 0.504662},
        {0.420791, 0.112920, 0.505215},
        {0.426877, 0.115395, 0.505714},
        {0.432967, 0.117855, 0.506160},
        {0.439062, 0.120298, 0.506555},
        {0.445163, 0.122724, 0.506901},
        {0.451271, 0.125132, 0.507198},
        {0.457386, 0.127522, 0.507448},
        {0.463508, 0.129893, 0.507652},
        {0.469640, 0.132245, 0.507809},
        {0.475780, 0.134577, 0.507921},
        {0.481929, 0.136891, 0.507989},
        {0.488088, 0.139186, 0.508011},
        {0.494258, 0.141462, 0.507988},
        {0.500438, 0.143719, 0.507920},
        {0.506629, 0.145958, 0.507806},
        {0.512831, 0.148179, 0.507648},
        {0.519045, 0.150383, 0.507443},
        {0.525270, 0.152569, 0.507192},
        {0.531507, 0.154739, 0.506895},
        {0.537755, 0.156894, 0.506551},
        {0.544015, 0.159033, 0.506159},
        {0.550287, 0.161158, 0.505719},
        {0.556571, 0.163269, 0.505230},
        {0.562866, 0.165368, 0.504692},
        {0.569172, 0.167454, 0.504105},
        {0.575490, 0.169530, 0.503466},
        {0.581819, 0.171596, 0.502777},
        {0.588158, 0.173652, 0.502035},
        {0.594508, 0.175701, 0.501241},
        {0.600868, 0.177743, 0.500394},
        {0.607238, 0.179779, 0.499492},
        {0.613617, 0.181811, 0.498536},
        {0.620005, 0.183840, 0.497524},
        {0.626401, 0.185867, 0.496456},
        {0.632805, 0.187893, 0.495332},
        {0.639216, 0.189921, 0.494150},
        {0.645633, 0.191952, 0.492910},
        {0.652056, 0.193986, 0.491611},
        {0.658483, 0.196027, 0.490253},
        {0.664915, 0.198075, 0.488836},
        {0.671349, 0.200133, 0.487358},
        {0.677786, 0.202203, 0.485819},
        {0.684224, 0.204286, 0.484219},
        {0.690661, 0.206384, 0.482558},
        {0.697098, 0.208501, 0.480835},
        {0.703532, 0.210638, 0.479049},
        {0.709962, 0.212797, 0.477201},
        {0.716387, 0.214982, 0.475290},
        {0.722805, 0.217194, 0.473316},
        {0.729216, 0.219437, 0.471279},
        {0.735616, 0.221713, 0.469180},
        {0.742004, 0.224025, 0.467018},
        {0.748378, 0.226377, 0.464794},
        {0.754737, 0.228772, 0.462509},
        {0.761077, 0.231214, 0.460162},
        {0.767398, 0.233705, 0.457755},
        {0.773695, 0.236249, 0.455289},
        {0.779968, 0.238851, 0.452765},
        {0.786212, 0.241514, 0.450184},
        {0.792427, 0.244242, 0.447543},
        {0.798608, 0.247040, 0.444848},
        {0.804752, 0.249911, 0.442102},
        {0.810855, 0.252861, 0.439305},
        {0.816914, 0.255895, 0.436461},
        {0.822926, 0.259016, 0.433573},
        {0.828886, 0.262229, 0.430644},
        {0.834791, 0.265540, 0.427671},
        {0.840636, 0.268953, 0.424666},
        {0.846416, 0.272473, 0.421631},
        {0.852126, 0.276106, 0.418573},
        {0.857763, 0.279857, 0.415496},
        {0.863320, 0.283729, 0.412403},
        {0.868793, 0.287728, 0.409303},
        {0.874176, 0.291859, 0.406205},
        {0.879464, 0.296125, 0.403118},
        {0.884651, 0.300530, 0.400047},
        {0.889731, 0.305079, 0.397002},
        {0.894700, 0.309773, 0.393995},
        {0.899552, 0.314616, 0.391037},
        {0.904281, 0.319610, 0.388137},
        {0.908884, 0.324755, 0.385308},
        {0.913354, 0.330052, 0.382563},
        {0.917689, 0.335500, 0.379915},
        {0.921884, 0.341098, 0.377376},
        {0.925937, 0.346844, 0.374959},
        {0.929845, 0.352734, 0.372677},
        {0.933606, 0.358764, 0.370541},
        {0.937221, 0.364929, 0.368567},
        {0.940687, 0.371224, 0.366762},
        {0.944006, 0.377643, 0.365136},
        {0.947180, 0.384178, 0.363701},
        {0.950210, 0.390820, 0.362468},
        {0.953099, 0.397563, 0.361438},
        {0.955849, 0.404400, 0.360619},
        {0.958464, 0.411324, 0.360014},
        {0.960949, 0.418323, 0.359630},
        {0.963310, 0.425390, 0.359469},
        {0.965549, 0.432519, 0.359529},
        {0.967671, 0.439703, 0.359810},
        {0.969680, 0.446936, 0.360311},
        {0.971582, 0.454210, 0.361030},
        {0.973381, 0.461520, 0.361965},
        {0.975082, 0.468861, 0.363111},
        {0.976690, 0.476226, 0.364466},
        {0.978210, 0.483612, 0.366025},
        {0.979645, 0.491014, 0.367783},
        {0.981000, 0.498428, 0.369734},
        {0.982279, 0.505851, 0.371874},
        {0.983485, 0.513280, 0.374198},
        {0.984622, 0.520713, 0.376698},
        {0.985693, 0.528148, 0.379371},
        {0.986700, 0.535582, 0.382210},
        {0.987646, 0.543015, 0.385210},
        {0.988533, 0.550446, 0.388365},
        {0.989363, 0.557873, 0.391671},
        {0.990138, 0.565296, 0.395122},
        {0.990871, 0.572706, 0.398714},
        {0.991558, 0.580107, 0.402441},
        {0.992196, 0.587502, 0.406299},
        {0.992785, 0.594891, 0.410283},
        {0.993326, 0.602275, 0.414390},
        {0.993834, 0.609644, 0.418613},
        {0.994309, 0.616999, 0.422950},
        {0.994738, 0.624350, 0.427397},
        {0.995122, 0.631696, 0.431951},
        {0.995480, 0.639027, 0.436607},
        {0.995810, 0.646344, 0.441361},
        {0.996096, 0.653659, 0.446213},
        {0.996341, 0.660969, 0.451160},
        {0.996580, 0.668256, 0.456192},
        {0.996775, 0.675541, 0.461314},
        {0.996925, 0.682828, 0.466526},
        {0.997077, 0.690088, 0.471811},
        {0.997186, 0.697349, 0.477182},
        {0.997254, 0.704611, 0.482635},
        {0.997325, 0.711848, 0.488154},
        {0.997351, 0.719089, 0.493755},
        {0.997351, 0.726324, 0.499428},
        {0.997341, 0.733545, 0.505167},
        {0.997285, 0.740772, 0.510983},
        {0.997228, 0.747981, 0.516859},
        {0.997138, 0.755190, 0.522806},
        {0.997019, 0.762398, 0.528821},
        {0.996898, 0.769591, 0.534892},
        {0.996727, 0.776795, 0.541039},
        {0.996571, 0.783977, 0.547233},
        {0.996369, 0.791167, 0.553499},
        {0.996162, 0.798348, 0.559820},
        {0.995932, 0.805527, 0.566202},
        {0.995680, 0.812706, 0.572645},
        {0.995424, 0.819875, 0.579140},
        {0.995131, 0.827052, 0.585701},
        {0.994851, 0.834213, 0.592307},
        {0.994524, 0.841387, 0.598983},
        {0.994222, 0.848540, 0.605696},
        {0.993866, 0.855711, 0.612482},
        {0.993545, 0.862859, 0.619299},
        {0.993170, 0.870024, 0.626189},
        {0.992831, 0.877168, 0.633109},
        {0.992440, 0.884330, 0.640099},
        {0.992089, 0.891470, 0.647116},
        {0.991688, 0.898627, 0.654202},
        {0.991332, 0.905763, 0.661309},
        {0.990930, 0.912915, 0.668481},
        {0.990570, 0.920049, 0.675675},
        {0.990175, 0.927196, 0.682926},
        {0.989815, 0.934329, 0.690198},
        {0.989434, 0.941470, 0.697519},
        {0.989077, 0.948604, 0.704863},
        {0.988717, 0.955742, 0.712242},
        {0.988367, 0.962878, 0.719649},
        {0.988033, 0.970012, 0.727077},
        {0.987691, 0.977154, 0.734536},
        {0.987387, 0.984288, 0.742002},
        {0.987053, 0.991438, 0.749504},
    };

    return interp(data, N);
}

/**
matplotlib colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> inferno(size_t N = 256)
{
    xt::xtensor<double, 2> data = {
        {0.001462, 0.000466, 0.013866},
        {0.002267, 0.001270, 0.018570},
        {0.003299, 0.002249, 0.024239},
        {0.004547, 0.003392, 0.030909},
        {0.006006, 0.004692, 0.038558},
        {0.007676, 0.006136, 0.046836},
        {0.009561, 0.007713, 0.055143},
        {0.011663, 0.009417, 0.063460},
        {0.013995, 0.011225, 0.071862},
        {0.016561, 0.013136, 0.080282},
        {0.019373, 0.015133, 0.088767},
        {0.022447, 0.017199, 0.097327},
        {0.025793, 0.019331, 0.105930},
        {0.029432, 0.021503, 0.114621},
        {0.033385, 0.023702, 0.123397},
        {0.037668, 0.025921, 0.132232},
        {0.042253, 0.028139, 0.141141},
        {0.046915, 0.030324, 0.150164},
        {0.051644, 0.032474, 0.159254},
        {0.056449, 0.034569, 0.168414},
        {0.061340, 0.036590, 0.177642},
        {0.066331, 0.038504, 0.186962},
        {0.071429, 0.040294, 0.196354},
        {0.076637, 0.041905, 0.205799},
        {0.081962, 0.043328, 0.215289},
        {0.087411, 0.044556, 0.224813},
        {0.092990, 0.045583, 0.234358},
        {0.098702, 0.046402, 0.243904},
        {0.104551, 0.047008, 0.253430},
        {0.110536, 0.047399, 0.262912},
        {0.116656, 0.047574, 0.272321},
        {0.122908, 0.047536, 0.281624},
        {0.129285, 0.047293, 0.290788},
        {0.135778, 0.046856, 0.299776},
        {0.142378, 0.046242, 0.308553},
        {0.149073, 0.045468, 0.317085},
        {0.155850, 0.044559, 0.325338},
        {0.162689, 0.043554, 0.333277},
        {0.169575, 0.042489, 0.340874},
        {0.176493, 0.041402, 0.348111},
        {0.183429, 0.040329, 0.354971},
        {0.190367, 0.039309, 0.361447},
        {0.197297, 0.038400, 0.367535},
        {0.204209, 0.037632, 0.373238},
        {0.211095, 0.037030, 0.378563},
        {0.217949, 0.036615, 0.383522},
        {0.224763, 0.036405, 0.388129},
        {0.231538, 0.036405, 0.392400},
        {0.238273, 0.036621, 0.396353},
        {0.244967, 0.037055, 0.400007},
        {0.251620, 0.037705, 0.403378},
        {0.258234, 0.038571, 0.406485},
        {0.264810, 0.039647, 0.409345},
        {0.271347, 0.040922, 0.411976},
        {0.277850, 0.042353, 0.414392},
        {0.284321, 0.043933, 0.416608},
        {0.290763, 0.045644, 0.418637},
        {0.297178, 0.047470, 0.420491},
        {0.303568, 0.049396, 0.422182},
        {0.309935, 0.051407, 0.423721},
        {0.316282, 0.053490, 0.425116},
        {0.322610, 0.055634, 0.426377},
        {0.328921, 0.057827, 0.427511},
        {0.335217, 0.060060, 0.428524},
        {0.341500, 0.062325, 0.429425},
        {0.347771, 0.064616, 0.430217},
        {0.354032, 0.066925, 0.430906},
        {0.360284, 0.069247, 0.431497},
        {0.366529, 0.071579, 0.431994},
        {0.372768, 0.073915, 0.432400},
        {0.379001, 0.076253, 0.432719},
        {0.385228, 0.078591, 0.432955},
        {0.391453, 0.080927, 0.433109},
        {0.397674, 0.083257, 0.433183},
        {0.403894, 0.085580, 0.433179},
        {0.410113, 0.087896, 0.433098},
        {0.416331, 0.090203, 0.432943},
        {0.422549, 0.092501, 0.432714},
        {0.428768, 0.094790, 0.432412},
        {0.434987, 0.097069, 0.432039},
        {0.441207, 0.099338, 0.431594},
        {0.447428, 0.101597, 0.431080},
        {0.453651, 0.103848, 0.430498},
        {0.459875, 0.106089, 0.429846},
        {0.466100, 0.108322, 0.429125},
        {0.472328, 0.110547, 0.428334},
        {0.478558, 0.112764, 0.427475},
        {0.484789, 0.114974, 0.426548},
        {0.491022, 0.117179, 0.425552},
        {0.497257, 0.119379, 0.424488},
        {0.503493, 0.121575, 0.423356},
        {0.509730, 0.123769, 0.422156},
        {0.515967, 0.125960, 0.420887},
        {0.522206, 0.128150, 0.419549},
        {0.528444, 0.130341, 0.418142},
        {0.534683, 0.132534, 0.416667},
        {0.540920, 0.134729, 0.415123},
        {0.547157, 0.136929, 0.413511},
        {0.553392, 0.139134, 0.411829},
        {0.559624, 0.141346, 0.410078},
        {0.565854, 0.143567, 0.408258},
        {0.572081, 0.145797, 0.406369},
        {0.578304, 0.148039, 0.404411},
        {0.584521, 0.150294, 0.402385},
        {0.590734, 0.152563, 0.400290},
        {0.596940, 0.154848, 0.398125},
        {0.603139, 0.157151, 0.395891},
        {0.609330, 0.159474, 0.393589},
        {0.615513, 0.161817, 0.391219},
        {0.621685, 0.164184, 0.388781},
        {0.627847, 0.166575, 0.386276},
        {0.633998, 0.168992, 0.383704},
        {0.640135, 0.171438, 0.381065},
        {0.646260, 0.173914, 0.378359},
        {0.652369, 0.176421, 0.375586},
        {0.658463, 0.178962, 0.372748},
        {0.664540, 0.181539, 0.369846},
        {0.670599, 0.184153, 0.366879},
        {0.676638, 0.186807, 0.363849},
        {0.682656, 0.189501, 0.360757},
        {0.688653, 0.192239, 0.357603},
        {0.694627, 0.195021, 0.354388},
        {0.700576, 0.197851, 0.351113},
        {0.706500, 0.200728, 0.347777},
        {0.712396, 0.203656, 0.344383},
        {0.718264, 0.206636, 0.340931},
        {0.724103, 0.209670, 0.337424},
        {0.729909, 0.212759, 0.333861},
        {0.735683, 0.215906, 0.330245},
        {0.741423, 0.219112, 0.326576},
        {0.747127, 0.222378, 0.322856},
        {0.752794, 0.225706, 0.319085},
        {0.758422, 0.229097, 0.315266},
        {0.764010, 0.232554, 0.311399},
        {0.769556, 0.236077, 0.307485},
        {0.775059, 0.239667, 0.303526},
        {0.780517, 0.243327, 0.299523},
        {0.785929, 0.247056, 0.295477},
        {0.791293, 0.250856, 0.291390},
        {0.796607, 0.254728, 0.287264},
        {0.801871, 0.258674, 0.283099},
        {0.807082, 0.262692, 0.278898},
        {0.812239, 0.266786, 0.274661},
        {0.817341, 0.270954, 0.270390},
        {0.822386, 0.275197, 0.266085},
        {0.827372, 0.279517, 0.261750},
        {0.832299, 0.283913, 0.257383},
        {0.837165, 0.288385, 0.252988},
        {0.841969, 0.292933, 0.248564},
        {0.846709, 0.297559, 0.244113},
        {0.851384, 0.302260, 0.239636},
        {0.855992, 0.307038, 0.235133},
        {0.860533, 0.311892, 0.230606},
        {0.865006, 0.316822, 0.226055},
        {0.869409, 0.321827, 0.221482},
        {0.873741, 0.326906, 0.216886},
        {0.878001, 0.332060, 0.212268},
        {0.882188, 0.337287, 0.207628},
        {0.886302, 0.342586, 0.202968},
        {0.890341, 0.347957, 0.198286},
        {0.894305, 0.353399, 0.193584},
        {0.898192, 0.358911, 0.188860},
        {0.902003, 0.364492, 0.184116},
        {0.905735, 0.370140, 0.179350},
        {0.909390, 0.375856, 0.174563},
        {0.912966, 0.381636, 0.169755},
        {0.916462, 0.387481, 0.164924},
        {0.919879, 0.393389, 0.160070},
        {0.923215, 0.399359, 0.155193},
        {0.926470, 0.405389, 0.150292},
        {0.929644, 0.411479, 0.145367},
        {0.932737, 0.417627, 0.140417},
        {0.935747, 0.423831, 0.135440},
        {0.938675, 0.430091, 0.130438},
        {0.941521, 0.436405, 0.125409},
        {0.944285, 0.442772, 0.120354},
        {0.946965, 0.449191, 0.115272},
        {0.949562, 0.455660, 0.110164},
        {0.952075, 0.462178, 0.105031},
        {0.954506, 0.468744, 0.099874},
        {0.956852, 0.475356, 0.094695},
        {0.959114, 0.482014, 0.089499},
        {0.961293, 0.488716, 0.084289},
        {0.963387, 0.495462, 0.079073},
        {0.965397, 0.502249, 0.073859},
        {0.967322, 0.509078, 0.068659},
        {0.969163, 0.515946, 0.063488},
        {0.970919, 0.522853, 0.058367},
        {0.972590, 0.529798, 0.053324},
        {0.974176, 0.536780, 0.048392},
        {0.975677, 0.543798, 0.043618},
        {0.977092, 0.550850, 0.039050},
        {0.978422, 0.557937, 0.034931},
        {0.979666, 0.565057, 0.031409},
        {0.980824, 0.572209, 0.028508},
        {0.981895, 0.579392, 0.026250},
        {0.982881, 0.586606, 0.024661},
        {0.983779, 0.593849, 0.023770},
        {0.984591, 0.601122, 0.023606},
        {0.985315, 0.608422, 0.024202},
        {0.985952, 0.615750, 0.025592},
        {0.986502, 0.623105, 0.027814},
        {0.986964, 0.630485, 0.030908},
        {0.987337, 0.637890, 0.034916},
        {0.987622, 0.645320, 0.039886},
        {0.987819, 0.652773, 0.045581},
        {0.987926, 0.660250, 0.051750},
        {0.987945, 0.667748, 0.058329},
        {0.987874, 0.675267, 0.065257},
        {0.987714, 0.682807, 0.072489},
        {0.987464, 0.690366, 0.079990},
        {0.987124, 0.697944, 0.087731},
        {0.986694, 0.705540, 0.095694},
        {0.986175, 0.713153, 0.103863},
        {0.985566, 0.720782, 0.112229},
        {0.984865, 0.728427, 0.120785},
        {0.984075, 0.736087, 0.129527},
        {0.983196, 0.743758, 0.138453},
        {0.982228, 0.751442, 0.147565},
        {0.981173, 0.759135, 0.156863},
        {0.980032, 0.766837, 0.166353},
        {0.978806, 0.774545, 0.176037},
        {0.977497, 0.782258, 0.185923},
        {0.976108, 0.789974, 0.196018},
        {0.974638, 0.797692, 0.206332},
        {0.973088, 0.805409, 0.216877},
        {0.971468, 0.813122, 0.227658},
        {0.969783, 0.820825, 0.238686},
        {0.968041, 0.828515, 0.249972},
        {0.966243, 0.836191, 0.261534},
        {0.964394, 0.843848, 0.273391},
        {0.962517, 0.851476, 0.285546},
        {0.960626, 0.859069, 0.298010},
        {0.958720, 0.866624, 0.310820},
        {0.956834, 0.874129, 0.323974},
        {0.954997, 0.881569, 0.337475},
        {0.953215, 0.888942, 0.351369},
        {0.951546, 0.896226, 0.365627},
        {0.950018, 0.903409, 0.380271},
        {0.948683, 0.910473, 0.395289},
        {0.947594, 0.917399, 0.410665},
        {0.946809, 0.924168, 0.426373},
        {0.946392, 0.930761, 0.442367},
        {0.946403, 0.937159, 0.458592},
        {0.946903, 0.943348, 0.474970},
        {0.947937, 0.949318, 0.491426},
        {0.949545, 0.955063, 0.507860},
        {0.951740, 0.960587, 0.524203},
        {0.954529, 0.965896, 0.540361},
        {0.957896, 0.971003, 0.556275},
        {0.961812, 0.975924, 0.571925},
        {0.966249, 0.980678, 0.587206},
        {0.971162, 0.985282, 0.602154},
        {0.976511, 0.989753, 0.616760},
        {0.982257, 0.994109, 0.631017},
        {0.988362, 0.998364, 0.644924},
    };

    return interp(data, N);
}

/**
matplotlib colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> plasma(size_t N = 256)
{
    xt::xtensor<double, 2> data = {
        {0.050383, 0.029803, 0.527975},
        {0.063536, 0.028426, 0.533124},
        {0.075353, 0.027206, 0.538007},
        {0.086222, 0.026125, 0.542658},
        {0.096379, 0.025165, 0.547103},
        {0.105980, 0.024309, 0.551368},
        {0.115124, 0.023556, 0.555468},
        {0.123903, 0.022878, 0.559423},
        {0.132381, 0.022258, 0.563250},
        {0.140603, 0.021687, 0.566959},
        {0.148607, 0.021154, 0.570562},
        {0.156421, 0.020651, 0.574065},
        {0.164070, 0.020171, 0.577478},
        {0.171574, 0.019706, 0.580806},
        {0.178950, 0.019252, 0.584054},
        {0.186213, 0.018803, 0.587228},
        {0.193374, 0.018354, 0.590330},
        {0.200445, 0.017902, 0.593364},
        {0.207435, 0.017442, 0.596333},
        {0.214350, 0.016973, 0.599239},
        {0.221197, 0.016497, 0.602083},
        {0.227983, 0.016007, 0.604867},
        {0.234715, 0.015502, 0.607592},
        {0.241396, 0.014979, 0.610259},
        {0.248032, 0.014439, 0.612868},
        {0.254627, 0.013882, 0.615419},
        {0.261183, 0.013308, 0.617911},
        {0.267703, 0.012716, 0.620346},
        {0.274191, 0.012109, 0.622722},
        {0.280648, 0.011488, 0.625038},
        {0.287076, 0.010855, 0.627295},
        {0.293478, 0.010213, 0.629490},
        {0.299855, 0.009561, 0.631624},
        {0.306210, 0.008902, 0.633694},
        {0.312543, 0.008239, 0.635700},
        {0.318856, 0.007576, 0.637640},
        {0.325150, 0.006915, 0.639512},
        {0.331426, 0.006261, 0.641316},
        {0.337683, 0.005618, 0.643049},
        {0.343925, 0.004991, 0.644710},
        {0.350150, 0.004382, 0.646298},
        {0.356359, 0.003798, 0.647810},
        {0.362553, 0.003243, 0.649245},
        {0.368733, 0.002724, 0.650601},
        {0.374897, 0.002245, 0.651876},
        {0.381047, 0.001814, 0.653068},
        {0.387183, 0.001434, 0.654177},
        {0.393304, 0.001114, 0.655199},
        {0.399411, 0.000859, 0.656133},
        {0.405503, 0.000678, 0.656977},
        {0.411580, 0.000577, 0.657730},
        {0.417642, 0.000564, 0.658390},
        {0.423689, 0.000646, 0.658956},
        {0.429719, 0.000831, 0.659425},
        {0.435734, 0.001127, 0.659797},
        {0.441732, 0.001540, 0.660069},
        {0.447714, 0.002080, 0.660240},
        {0.453677, 0.002755, 0.660310},
        {0.459623, 0.003574, 0.660277},
        {0.465550, 0.004545, 0.660139},
        {0.471457, 0.005678, 0.659897},
        {0.477344, 0.006980, 0.659549},
        {0.483210, 0.008460, 0.659095},
        {0.489055, 0.010127, 0.658534},
        {0.494877, 0.011990, 0.657865},
        {0.500678, 0.014055, 0.657088},
        {0.506454, 0.016333, 0.656202},
        {0.512206, 0.018833, 0.655209},
        {0.517933, 0.021563, 0.654109},
        {0.523633, 0.024532, 0.652901},
        {0.529306, 0.027747, 0.651586},
        {0.534952, 0.031217, 0.650165},
        {0.540570, 0.034950, 0.648640},
        {0.546157, 0.038954, 0.647010},
        {0.551715, 0.043136, 0.645277},
        {0.557243, 0.047331, 0.643443},
        {0.562738, 0.051545, 0.641509},
        {0.568201, 0.055778, 0.639477},
        {0.573632, 0.060028, 0.637349},
        {0.579029, 0.064296, 0.635126},
        {0.584391, 0.068579, 0.632812},
        {0.589719, 0.072878, 0.630408},
        {0.595011, 0.077190, 0.627917},
        {0.600266, 0.081516, 0.625342},
        {0.605485, 0.085854, 0.622686},
        {0.610667, 0.090204, 0.619951},
        {0.615812, 0.094564, 0.617140},
        {0.620919, 0.098934, 0.614257},
        {0.625987, 0.103312, 0.611305},
        {0.631017, 0.107699, 0.608287},
        {0.636008, 0.112092, 0.605205},
        {0.640959, 0.116492, 0.602065},
        {0.645872, 0.120898, 0.598867},
        {0.650746, 0.125309, 0.595617},
        {0.655580, 0.129725, 0.592317},
        {0.660374, 0.134144, 0.588971},
        {0.665129, 0.138566, 0.585582},
        {0.669845, 0.142992, 0.582154},
        {0.674522, 0.147419, 0.578688},
        {0.679160, 0.151848, 0.575189},
        {0.683758, 0.156278, 0.571660},
        {0.688318, 0.160709, 0.568103},
        {0.692840, 0.165141, 0.564522},
        {0.697324, 0.169573, 0.560919},
        {0.701769, 0.174005, 0.557296},
        {0.706178, 0.178437, 0.553657},
        {0.710549, 0.182868, 0.550004},
        {0.714883, 0.187299, 0.546338},
        {0.719181, 0.191729, 0.542663},
        {0.723444, 0.196158, 0.538981},
        {0.727670, 0.200586, 0.535293},
        {0.731862, 0.205013, 0.531601},
        {0.736019, 0.209439, 0.527908},
        {0.740143, 0.213864, 0.524216},
        {0.744232, 0.218288, 0.520524},
        {0.748289, 0.222711, 0.516834},
        {0.752312, 0.227133, 0.513149},
        {0.756304, 0.231555, 0.509468},
        {0.760264, 0.235976, 0.505794},
        {0.764193, 0.240396, 0.502126},
        {0.768090, 0.244817, 0.498465},
        {0.771958, 0.249237, 0.494813},
        {0.775796, 0.253658, 0.491171},
        {0.779604, 0.258078, 0.487539},
        {0.783383, 0.262500, 0.483918},
        {0.787133, 0.266922, 0.480307},
        {0.790855, 0.271345, 0.476706},
        {0.794549, 0.275770, 0.473117},
        {0.798216, 0.280197, 0.469538},
        {0.801855, 0.284626, 0.465971},
        {0.805467, 0.289057, 0.462415},
        {0.809052, 0.293491, 0.458870},
        {0.812612, 0.297928, 0.455338},
        {0.816144, 0.302368, 0.451816},
        {0.819651, 0.306812, 0.448306},
        {0.823132, 0.311261, 0.444806},
        {0.826588, 0.315714, 0.441316},
        {0.830018, 0.320172, 0.437836},
        {0.833422, 0.324635, 0.434366},
        {0.836801, 0.329105, 0.430905},
        {0.840155, 0.333580, 0.427455},
        {0.843484, 0.338062, 0.424013},
        {0.846788, 0.342551, 0.420579},
        {0.850066, 0.347048, 0.417153},
        {0.853319, 0.351553, 0.413734},
        {0.856547, 0.356066, 0.410322},
        {0.859750, 0.360588, 0.406917},
        {0.862927, 0.365119, 0.403519},
        {0.866078, 0.369660, 0.400126},
        {0.869203, 0.374212, 0.396738},
        {0.872303, 0.378774, 0.393355},
        {0.875376, 0.383347, 0.389976},
        {0.878423, 0.387932, 0.386600},
        {0.881443, 0.392529, 0.383229},
        {0.884436, 0.397139, 0.379860},
        {0.887402, 0.401762, 0.376494},
        {0.890340, 0.406398, 0.373130},
        {0.893250, 0.411048, 0.369768},
        {0.896131, 0.415712, 0.366407},
        {0.898984, 0.420392, 0.363047},
        {0.901807, 0.425087, 0.359688},
        {0.904601, 0.429797, 0.356329},
        {0.907365, 0.434524, 0.352970},
        {0.910098, 0.439268, 0.349610},
        {0.912800, 0.444029, 0.346251},
        {0.915471, 0.448807, 0.342890},
        {0.918109, 0.453603, 0.339529},
        {0.920714, 0.458417, 0.336166},
        {0.923287, 0.463251, 0.332801},
        {0.925825, 0.468103, 0.329435},
        {0.928329, 0.472975, 0.326067},
        {0.930798, 0.477867, 0.322697},
        {0.933232, 0.482780, 0.319325},
        {0.935630, 0.487712, 0.315952},
        {0.937990, 0.492667, 0.312575},
        {0.940313, 0.497642, 0.309197},
        {0.942598, 0.502639, 0.305816},
        {0.944844, 0.507658, 0.302433},
        {0.947051, 0.512699, 0.299049},
        {0.949217, 0.517763, 0.295662},
        {0.951344, 0.522850, 0.292275},
        {0.953428, 0.527960, 0.288883},
        {0.955470, 0.533093, 0.285490},
        {0.957469, 0.538250, 0.282096},
        {0.959424, 0.543431, 0.278701},
        {0.961336, 0.548636, 0.275305},
        {0.963203, 0.553865, 0.271909},
        {0.965024, 0.559118, 0.268513},
        {0.966798, 0.564396, 0.265118},
        {0.968526, 0.569700, 0.261721},
        {0.970205, 0.575028, 0.258325},
        {0.971835, 0.580382, 0.254931},
        {0.973416, 0.585761, 0.251540},
        {0.974947, 0.591165, 0.248151},
        {0.976428, 0.596595, 0.244767},
        {0.977856, 0.602051, 0.241387},
        {0.979233, 0.607532, 0.238013},
        {0.980556, 0.613039, 0.234646},
        {0.981826, 0.618572, 0.231287},
        {0.983041, 0.624131, 0.227937},
        {0.984199, 0.629718, 0.224595},
        {0.985301, 0.635330, 0.221265},
        {0.986345, 0.640969, 0.217948},
        {0.987332, 0.646633, 0.214648},
        {0.988260, 0.652325, 0.211364},
        {0.989128, 0.658043, 0.208100},
        {0.989935, 0.663787, 0.204859},
        {0.990681, 0.669558, 0.201642},
        {0.991365, 0.675355, 0.198453},
        {0.991985, 0.681179, 0.195295},
        {0.992541, 0.687030, 0.192170},
        {0.993032, 0.692907, 0.189084},
        {0.993456, 0.698810, 0.186041},
        {0.993814, 0.704741, 0.183043},
        {0.994103, 0.710698, 0.180097},
        {0.994324, 0.716681, 0.177208},
        {0.994474, 0.722691, 0.174381},
        {0.994553, 0.728728, 0.171622},
        {0.994561, 0.734791, 0.168938},
        {0.994495, 0.740880, 0.166335},
        {0.994355, 0.746995, 0.163821},
        {0.994141, 0.753137, 0.161404},
        {0.993851, 0.759304, 0.159092},
        {0.993482, 0.765499, 0.156891},
        {0.993033, 0.771720, 0.154808},
        {0.992505, 0.777967, 0.152855},
        {0.991897, 0.784239, 0.151042},
        {0.991209, 0.790537, 0.149377},
        {0.990439, 0.796859, 0.147870},
        {0.989587, 0.803205, 0.146529},
        {0.988648, 0.809579, 0.145357},
        {0.987621, 0.815978, 0.144363},
        {0.986509, 0.822401, 0.143557},
        {0.985314, 0.828846, 0.142945},
        {0.984031, 0.835315, 0.142528},
        {0.982653, 0.841812, 0.142303},
        {0.981190, 0.848329, 0.142279},
        {0.979644, 0.854866, 0.142453},
        {0.977995, 0.861432, 0.142808},
        {0.976265, 0.868016, 0.143351},
        {0.974443, 0.874622, 0.144061},
        {0.972530, 0.881250, 0.144923},
        {0.970533, 0.887896, 0.145919},
        {0.968443, 0.894564, 0.147014},
        {0.966271, 0.901249, 0.148180},
        {0.964021, 0.907950, 0.149370},
        {0.961681, 0.914672, 0.150520},
        {0.959276, 0.921407, 0.151566},
        {0.956808, 0.928152, 0.152409},
        {0.954287, 0.934908, 0.152921},
        {0.951726, 0.941671, 0.152925},
        {0.949151, 0.948435, 0.152178},
        {0.946602, 0.955190, 0.150328},
        {0.944152, 0.961916, 0.146861},
        {0.941896, 0.968590, 0.140956},
        {0.940015, 0.975158, 0.131326},
    };

    return interp(data, N);
}

/**
matplotlib colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> viridis(size_t N = 256)
{
    xt::xtensor<double, 2> data = {
        {0.267004, 0.004874, 0.329415},
        {0.268510, 0.009605, 0.335427},
        {0.269944, 0.014625, 0.341379},
        {0.271305, 0.019942, 0.347269},
        {0.272594, 0.025563, 0.353093},
        {0.273809, 0.031497, 0.358853},
        {0.274952, 0.037752, 0.364543},
        {0.276022, 0.044167, 0.370164},
        {0.277018, 0.050344, 0.375715},
        {0.277941, 0.056324, 0.381191},
        {0.278791, 0.062145, 0.386592},
        {0.279566, 0.067836, 0.391917},
        {0.280267, 0.073417, 0.397163},
        {0.280894, 0.078907, 0.402329},
        {0.281446, 0.084320, 0.407414},
        {0.281924, 0.089666, 0.412415},
        {0.282327, 0.094955, 0.417331},
        {0.282656, 0.100196, 0.422160},
        {0.282910, 0.105393, 0.426902},
        {0.283091, 0.110553, 0.431554},
        {0.283197, 0.115680, 0.436115},
        {0.283229, 0.120777, 0.440584},
        {0.283187, 0.125848, 0.444960},
        {0.283072, 0.130895, 0.449241},
        {0.282884, 0.135920, 0.453427},
        {0.282623, 0.140926, 0.457517},
        {0.282290, 0.145912, 0.461510},
        {0.281887, 0.150881, 0.465405},
        {0.281412, 0.155834, 0.469201},
        {0.280868, 0.160771, 0.472899},
        {0.280255, 0.165693, 0.476498},
        {0.279574, 0.170599, 0.479997},
        {0.278826, 0.175490, 0.483397},
        {0.278012, 0.180367, 0.486697},
        {0.277134, 0.185228, 0.489898},
        {0.276194, 0.190074, 0.493001},
        {0.275191, 0.194905, 0.496005},
        {0.274128, 0.199721, 0.498911},
        {0.273006, 0.204520, 0.501721},
        {0.271828, 0.209303, 0.504434},
        {0.270595, 0.214069, 0.507052},
        {0.269308, 0.218818, 0.509577},
        {0.267968, 0.223549, 0.512008},
        {0.266580, 0.228262, 0.514349},
        {0.265145, 0.232956, 0.516599},
        {0.263663, 0.237631, 0.518762},
        {0.262138, 0.242286, 0.520837},
        {0.260571, 0.246922, 0.522828},
        {0.258965, 0.251537, 0.524736},
        {0.257322, 0.256130, 0.526563},
        {0.255645, 0.260703, 0.528312},
        {0.253935, 0.265254, 0.529983},
        {0.252194, 0.269783, 0.531579},
        {0.250425, 0.274290, 0.533103},
        {0.248629, 0.278775, 0.534556},
        {0.246811, 0.283237, 0.535941},
        {0.244972, 0.287675, 0.537260},
        {0.243113, 0.292092, 0.538516},
        {0.241237, 0.296485, 0.539709},
        {0.239346, 0.300855, 0.540844},
        {0.237441, 0.305202, 0.541921},
        {0.235526, 0.309527, 0.542944},
        {0.233603, 0.313828, 0.543914},
        {0.231674, 0.318106, 0.544834},
        {0.229739, 0.322361, 0.545706},
        {0.227802, 0.326594, 0.546532},
        {0.225863, 0.330805, 0.547314},
        {0.223925, 0.334994, 0.548053},
        {0.221989, 0.339161, 0.548752},
        {0.220057, 0.343307, 0.549413},
        {0.218130, 0.347432, 0.550038},
        {0.216210, 0.351535, 0.550627},
        {0.214298, 0.355619, 0.551184},
        {0.212395, 0.359683, 0.551710},
        {0.210503, 0.363727, 0.552206},
        {0.208623, 0.367752, 0.552675},
        {0.206756, 0.371758, 0.553117},
        {0.204903, 0.375746, 0.553533},
        {0.203063, 0.379716, 0.553925},
        {0.201239, 0.383670, 0.554294},
        {0.199430, 0.387607, 0.554642},
        {0.197636, 0.391528, 0.554969},
        {0.195860, 0.395433, 0.555276},
        {0.194100, 0.399323, 0.555565},
        {0.192357, 0.403199, 0.555836},
        {0.190631, 0.407061, 0.556089},
        {0.188923, 0.410910, 0.556326},
        {0.187231, 0.414746, 0.556547},
        {0.185556, 0.418570, 0.556753},
        {0.183898, 0.422383, 0.556944},
        {0.182256, 0.426184, 0.557120},
        {0.180629, 0.429975, 0.557282},
        {0.179019, 0.433756, 0.557430},
        {0.177423, 0.437527, 0.557565},
        {0.175841, 0.441290, 0.557685},
        {0.174274, 0.445044, 0.557792},
        {0.172719, 0.448791, 0.557885},
        {0.171176, 0.452530, 0.557965},
        {0.169646, 0.456262, 0.558030},
        {0.168126, 0.459988, 0.558082},
        {0.166617, 0.463708, 0.558119},
        {0.165117, 0.467423, 0.558141},
        {0.163625, 0.471133, 0.558148},
        {0.162142, 0.474838, 0.558140},
        {0.160665, 0.478540, 0.558115},
        {0.159194, 0.482237, 0.558073},
        {0.157729, 0.485932, 0.558013},
        {0.156270, 0.489624, 0.557936},
        {0.154815, 0.493313, 0.557840},
        {0.153364, 0.497000, 0.557724},
        {0.151918, 0.500685, 0.557587},
        {0.150476, 0.504369, 0.557430},
        {0.149039, 0.508051, 0.557250},
        {0.147607, 0.511733, 0.557049},
        {0.146180, 0.515413, 0.556823},
        {0.144759, 0.519093, 0.556572},
        {0.143343, 0.522773, 0.556295},
        {0.141935, 0.526453, 0.555991},
        {0.140536, 0.530132, 0.555659},
        {0.139147, 0.533812, 0.555298},
        {0.137770, 0.537492, 0.554906},
        {0.136408, 0.541173, 0.554483},
        {0.135066, 0.544853, 0.554029},
        {0.133743, 0.548535, 0.553541},
        {0.132444, 0.552216, 0.553018},
        {0.131172, 0.555899, 0.552459},
        {0.129933, 0.559582, 0.551864},
        {0.128729, 0.563265, 0.551229},
        {0.127568, 0.566949, 0.550556},
        {0.126453, 0.570633, 0.549841},
        {0.125394, 0.574318, 0.549086},
        {0.124395, 0.578002, 0.548287},
        {0.123463, 0.581687, 0.547445},
        {0.122606, 0.585371, 0.546557},
        {0.121831, 0.589055, 0.545623},
        {0.121148, 0.592739, 0.544641},
        {0.120565, 0.596422, 0.543611},
        {0.120092, 0.600104, 0.542530},
        {0.119738, 0.603785, 0.541400},
        {0.119512, 0.607464, 0.540218},
        {0.119423, 0.611141, 0.538982},
        {0.119483, 0.614817, 0.537692},
        {0.119699, 0.618490, 0.536347},
        {0.120081, 0.622161, 0.534946},
        {0.120638, 0.625828, 0.533488},
        {0.121380, 0.629492, 0.531973},
        {0.122312, 0.633153, 0.530398},
        {0.123444, 0.636809, 0.528763},
        {0.124780, 0.640461, 0.527068},
        {0.126326, 0.644107, 0.525311},
        {0.128087, 0.647749, 0.523491},
        {0.130067, 0.651384, 0.521608},
        {0.132268, 0.655014, 0.519661},
        {0.134692, 0.658636, 0.517649},
        {0.137339, 0.662252, 0.515571},
        {0.140210, 0.665859, 0.513427},
        {0.143303, 0.669459, 0.511215},
        {0.146616, 0.673050, 0.508936},
        {0.150148, 0.676631, 0.506589},
        {0.153894, 0.680203, 0.504172},
        {0.157851, 0.683765, 0.501686},
        {0.162016, 0.687316, 0.499129},
        {0.166383, 0.690856, 0.496502},
        {0.170948, 0.694384, 0.493803},
        {0.175707, 0.697900, 0.491033},
        {0.180653, 0.701402, 0.488189},
        {0.185783, 0.704891, 0.485273},
        {0.191090, 0.708366, 0.482284},
        {0.196571, 0.711827, 0.479221},
        {0.202219, 0.715272, 0.476084},
        {0.208030, 0.718701, 0.472873},
        {0.214000, 0.722114, 0.469588},
        {0.220124, 0.725509, 0.466226},
        {0.226397, 0.728888, 0.462789},
        {0.232815, 0.732247, 0.459277},
        {0.239374, 0.735588, 0.455688},
        {0.246070, 0.738910, 0.452024},
        {0.252899, 0.742211, 0.448284},
        {0.259857, 0.745492, 0.444467},
        {0.266941, 0.748751, 0.440573},
        {0.274149, 0.751988, 0.436601},
        {0.281477, 0.755203, 0.432552},
        {0.288921, 0.758394, 0.428426},
        {0.296479, 0.761561, 0.424223},
        {0.304148, 0.764704, 0.419943},
        {0.311925, 0.767822, 0.415586},
        {0.319809, 0.770914, 0.411152},
        {0.327796, 0.773980, 0.406640},
        {0.335885, 0.777018, 0.402049},
        {0.344074, 0.780029, 0.397381},
        {0.352360, 0.783011, 0.392636},
        {0.360741, 0.785964, 0.387814},
        {0.369214, 0.788888, 0.382914},
        {0.377779, 0.791781, 0.377939},
        {0.386433, 0.794644, 0.372886},
        {0.395174, 0.797475, 0.367757},
        {0.404001, 0.800275, 0.362552},
        {0.412913, 0.803041, 0.357269},
        {0.421908, 0.805774, 0.351910},
        {0.430983, 0.808473, 0.346476},
        {0.440137, 0.811138, 0.340967},
        {0.449368, 0.813768, 0.335384},
        {0.458674, 0.816363, 0.329727},
        {0.468053, 0.818921, 0.323998},
        {0.477504, 0.821444, 0.318195},
        {0.487026, 0.823929, 0.312321},
        {0.496615, 0.826376, 0.306377},
        {0.506271, 0.828786, 0.300362},
        {0.515992, 0.831158, 0.294279},
        {0.525776, 0.833491, 0.288127},
        {0.535621, 0.835785, 0.281908},
        {0.545524, 0.838039, 0.275626},
        {0.555484, 0.840254, 0.269281},
        {0.565498, 0.842430, 0.262877},
        {0.575563, 0.844566, 0.256415},
        {0.585678, 0.846661, 0.249897},
        {0.595839, 0.848717, 0.243329},
        {0.606045, 0.850733, 0.236712},
        {0.616293, 0.852709, 0.230052},
        {0.626579, 0.854645, 0.223353},
        {0.636902, 0.856542, 0.216620},
        {0.647257, 0.858400, 0.209861},
        {0.657642, 0.860219, 0.203082},
        {0.668054, 0.861999, 0.196293},
        {0.678489, 0.863742, 0.189503},
        {0.688944, 0.865448, 0.182725},
        {0.699415, 0.867117, 0.175971},
        {0.709898, 0.868751, 0.169257},
        {0.720391, 0.870350, 0.162603},
        {0.730889, 0.871916, 0.156029},
        {0.741388, 0.873449, 0.149561},
        {0.751884, 0.874951, 0.143228},
        {0.762373, 0.876424, 0.137064},
        {0.772852, 0.877868, 0.131109},
        {0.783315, 0.879285, 0.125405},
        {0.793760, 0.880678, 0.120005},
        {0.804182, 0.882046, 0.114965},
        {0.814576, 0.883393, 0.110347},
        {0.824940, 0.884720, 0.106217},
        {0.835270, 0.886029, 0.102646},
        {0.845561, 0.887322, 0.099702},
        {0.855810, 0.888601, 0.097452},
        {0.866013, 0.889868, 0.095953},
        {0.876168, 0.891125, 0.095250},
        {0.886271, 0.892374, 0.095374},
        {0.896320, 0.893616, 0.096335},
        {0.906311, 0.894855, 0.098125},
        {0.916242, 0.896091, 0.100717},
        {0.926106, 0.897330, 0.104071},
        {0.935904, 0.898570, 0.108131},
        {0.945636, 0.899815, 0.112838},
        {0.955300, 0.901065, 0.118128},
        {0.964894, 0.902323, 0.123941},
        {0.974417, 0.903590, 0.130215},
        {0.983868, 0.904867, 0.136897},
        {0.993248, 0.906157, 0.143936},
    };

    return interp(data, N);
}

/**
matplotlib colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> seismic(size_t N = 5)
{
    xt::xtensor<double, 2> data = {
        {0.0, 0.0, 0.3},
        {0.0, 0.0, 1.0},
        {1.0, 1.0, 1.0},
        {1.0, 0.0, 0.0},
        {0.5, 0.0, 0.0}};

    return interp(data, N);
}

/**
Monotone colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> White(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{255, 255, 255}};
    return interp(data / 255.0, N);
}

/**
Monotone colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Grey(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{0.5, 0.5, 0.5}};
    return interp(data, N);
}

/**
Monotone colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Black(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{0, 0, 0}};
    return interp(data / 255.0, N);
}

/**
Monotone colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Red(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{255, 0, 0}};
    return interp(data / 255.0, N);
}

/**
Monotone colormap.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Blue(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{0, 0, 255}};
    return interp(data / 255.0, N);
}

/**
Eindhoven University of Technology.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> tuewarmred(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{247, 49, 49}};
    return interp(data / 255.0, N);
}

/**
Eindhoven University of Technology.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> tuedarkblue(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{16, 16, 115}};
    return interp(data / 255.0, N);
}

/**
Eindhoven University of Technology.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> tueblue(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{0, 102, 204}};
    return interp(data / 255.0, N);
}

/**
Eindhoven University of Technology.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> tuelightblue(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{0, 162, 222}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Apricot(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{251, 185, 130}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Aquamarine(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{0, 181, 190}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Bittersweet(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{192,  79,  23}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> BlueGreen(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{0, 179, 184}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> BlueViolet(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{71,  57, 146}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> BrickRed(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{182,  50,  28}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Brown(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{121,  37,   0}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> BurntOrange(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{247, 146,  29}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> CadetBlue(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{116, 114, 154}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> CarnationPink(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{242, 130, 180}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Cerulean(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{0, 162, 227}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> CornflowerBlue(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{65, 176, 228}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Cyan(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{0, 174, 239}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Dandelion(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{253, 188,  66}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> DarkOrchid(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{164,  83, 138}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Emerald(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{0, 169, 157}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> ForestGreen(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{0, 155,  85}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Fuchsia(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{140,  54, 140}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Goldenrod(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{255, 223,  66}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Gray(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{148, 150, 152}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Green(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{0, 166,  79}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> GreenYellow(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{223, 230, 116}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> JungleGreen(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{0, 169, 154}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Lavender(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{244, 158, 196}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> LimeGreen(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{141, 199,  62}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Magenta(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{236,   0, 140}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Mahogany(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{169,  52,  31}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Maroon(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{175,  50,  53}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Melon(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{248, 158, 123}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> MidnightBlue(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{0, 103, 149}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Mulberry(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{169,  60, 147}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> NavyBlue(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{0, 110, 184}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> OliveGreen(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{60, 128,  49}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Orange(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{245, 129,  55}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> OrangeRed(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{237,  19,  90}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Orchid(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{175, 114, 176}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Peach(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{247, 150,  90}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Periwinkle(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{121, 119, 184}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> PineGreen(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{0, 139, 114}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Plum(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{146,  38, 143}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> ProcessBlue(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{0, 176, 240}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Purple(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{153,  71, 155}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> RawSienna(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{151,  64,   6}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> RedOrange(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{242,  96,  53}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> RedViolet(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{161,  36, 107}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Rhodamine(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{239,  85, 159}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> RoyalBlue(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{0, 113, 188}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> RoyalPurple(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{97,  63, 153}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> RubineRed(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{237,   1, 125}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Salmon(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{246, 146, 137}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> SeaGreen(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{63, 188, 157}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Sepia(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{103,  24,   0}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> SkyBlue(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{70, 197, 221}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> SpringGreen(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{198, 220, 103}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Tan(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{218, 157, 118}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> TealBlue(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{0, 174, 179}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Thistle(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{216, 131, 183}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Turquoise(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{0, 180, 206}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Violet(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{88,  66, 155}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> VioletRed(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{239,  88, 160}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> WildStrawberry(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{238,  41, 103}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Yellow(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{255, 242,   0}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> YellowGreen(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{152, 204, 112}};
    return interp(data / 255.0, N);
}

/**
dvips color. See: https://en.wikibooks.org/wiki/LaTeX/Colors

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> YellowOrange(size_t N = 1)
{
    xt::xtensor<double, 2> data = {{250, 162,  26}};
    return interp(data / 255.0, N);
}

/**
Inverse of cppcolormap::Accent.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Accent_r(size_t N = 8)
{
    return xt::flip(Accent(N), 0);
}

/**
Inverse of cppcolormap::Dark2.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Dark2_r(size_t N = 8)
{
    return xt::flip(Dark2(N), 0);
}

/**
Inverse of cppcolormap::Paired.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Paired_r(size_t N = 12)
{
    return xt::flip(Paired(N), 0);
}

/**
Inverse of cppcolormap::Spectral.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Spectral_r(size_t N = 11)
{
    return xt::flip(Spectral(N), 0);
}

/**
Inverse of cppcolormap::Pastel1.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Pastel1_r(size_t N = 9)
{
    return xt::flip(Pastel1(N), 0);
}

/**
Inverse of cppcolormap::Pastel2.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Pastel2_r(size_t N = 8)
{
    return xt::flip(Pastel2(N), 0);
}

/**
Inverse of cppcolormap::Set1.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Set1_r(size_t N = 9)
{
    return xt::flip(Set1(N), 0);
}

/**
Inverse of cppcolormap::Set2.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Set2_r(size_t N = 8)
{
    return xt::flip(Set2(N), 0);
}

/**
Inverse of cppcolormap::Set3.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Set3_r(size_t N = 12)
{
    return xt::flip(Set3(N), 0);
}

/**
Inverse of cppcolormap::Blues.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Blues_r(size_t N = 9)
{
    return xt::flip(Blues(N), 0);
}

/**
Inverse of cppcolormap::Greens.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Greens_r(size_t N = 9)
{
    return xt::flip(Greens(N), 0);
}

/**
Inverse of cppcolormap::Greys.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Greys_r(size_t N = 2)
{
    return xt::flip(Greys(N), 0);
}

/**
Inverse of cppcolormap::Oranges.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Oranges_r(size_t N = 9)
{
    return xt::flip(Oranges(N), 0);
}

/**
Inverse of cppcolormap::Purples.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Purples_r(size_t N = 9)
{
    return xt::flip(Purples(N), 0);
}

/**
Inverse of cppcolormap::Reds.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> Reds_r(size_t N = 9)
{
    return xt::flip(Reds(N), 0);
}

/**
Inverse of cppcolormap::BuPu.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> BuPu_r(size_t N = 9)
{
    return xt::flip(BuPu(N), 0);
}

/**
Inverse of cppcolormap::GnBu.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> GnBu_r(size_t N = 9)
{
    return xt::flip(GnBu(N), 0);
}

/**
Inverse of cppcolormap::PuBu.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> PuBu_r(size_t N = 9)
{
    return xt::flip(PuBu(N), 0);
}

/**
Inverse of cppcolormap::PuBuGn.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> PuBuGn_r(size_t N = 9)
{
    return xt::flip(PuBuGn(N), 0);
}

/**
Inverse of cppcolormap::PuRd.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> PuRd_r(size_t N = 9)
{
    return xt::flip(PuRd(N), 0);
}

/**
Inverse of cppcolormap::RdPu.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> RdPu_r(size_t N = 9)
{
    return xt::flip(RdPu(N), 0);
}

/**
Inverse of cppcolormap::OrRd.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> OrRd_r(size_t N = 9)
{
    return xt::flip(OrRd(N), 0);
}

/**
Inverse of cppcolormap::RdOrYl.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> RdOrYl_r(size_t N = 9)
{
    return xt::flip(RdOrYl(N), 0);
}

/**
Inverse of cppcolormap::YlGn.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> YlGn_r(size_t N = 9)
{
    return xt::flip(YlGn(N), 0);
}

/**
Inverse of cppcolormap::YlGnBu.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> YlGnBu_r(size_t N = 9)
{
    return xt::flip(YlGnBu(N), 0);
}

/**
Inverse of cppcolormap::YlOrRd.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> YlOrRd_r(size_t N = 9)
{
    return xt::flip(YlOrRd(N), 0);
}

/**
Inverse of cppcolormap::BrBG.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> BrBG_r(size_t N = 11)
{
    return xt::flip(BrBG(N), 0);
}

/**
Inverse of cppcolormap::PuOr.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> PuOr_r(size_t N = 11)
{
    return xt::flip(PuOr(N), 0);
}

/**
Inverse of cppcolormap::RdBu.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> RdBu_r(size_t N = 11)
{
    return xt::flip(RdBu(N), 0);
}

/**
Inverse of cppcolormap::RdGy.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> RdGy_r(size_t N = 11)
{
    return xt::flip(RdGy(N), 0);
}

/**
Inverse of cppcolormap::RdYlBu.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> RdYlBu_r(size_t N = 11)
{
    return xt::flip(RdYlBu(N), 0);
}

/**
Inverse of cppcolormap::RdYlGn.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> RdYlGn_r(size_t N = 11)
{
    return xt::flip(RdYlGn(N), 0);
}

/**
Inverse of cppcolormap::PiYG.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> PiYG_r(size_t N = 11)
{
    return xt::flip(PiYG(N), 0);
}

/**
Inverse of cppcolormap::PRGn.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> PRGn_r(size_t N = 11)
{
    return xt::flip(PRGn(N), 0);
}

/**
Inverse of cppcolormap::spring.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> spring_r(size_t N = 256)
{
    return xt::flip(spring(N), 0);
}

/**
Inverse of cppcolormap::summer.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> summer_r(size_t N = 256)
{
    return xt::flip(summer(N), 0);
}

/**
Inverse of cppcolormap::autumn.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> autumn_r(size_t N = 256)
{
    return xt::flip(autumn(N), 0);
}

/**
Inverse of cppcolormap::winter.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> winter_r(size_t N = 256)
{
    return xt::flip(winter(N), 0);
}

/**
Inverse of cppcolormap::bone.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> bone_r(size_t N = 256)
{
    return xt::flip(bone(N), 0);
}

/**
Inverse of cppcolormap::cool.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> cool_r(size_t N = 256)
{
    return xt::flip(cool(N), 0);
}

/**
Inverse of cppcolormap::hot.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> hot_r(size_t N = 256)
{
    return xt::flip(hot(N), 0);
}

/**
Inverse of cppcolormap::copper.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> copper_r(size_t N = 256)
{
    return xt::flip(copper(N), 0);
}

/**
Inverse of cppcolormap::hsv.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> hsv_r(size_t N = 256)
{
    return xt::flip(hsv(N), 0);
}

/**
Inverse of cppcolormap::nipy_spectral.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> nipy_spectral_r(size_t N = 256)
{
    return xt::flip(nipy_spectral(N), 0);
}

/**
Inverse of cppcolormap::jet.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> jet_r(size_t N = 256)
{
    return xt::flip(jet(N), 0);
}

/**
Inverse of cppcolormap::terrain.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> terrain_r(size_t N = 5)
{
    return xt::flip(terrain(N), 0);
}

/**
Inverse of cppcolormap::seismic.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> seismic_r(size_t N = 6)
{
    return xt::flip(seismic(N), 0);
}

/**
Inverse of cppcolormap::afmhot.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> afmhot_r(size_t N = 256)
{
    return xt::flip(afmhot(N), 0);
}

/**
Inverse of cppcolormap::magma.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> magma_r(size_t N = 256)
{
    return xt::flip(magma(N), 0);
}

/**
Inverse of cppcolormap::inferno.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> inferno_r(size_t N = 256)
{
    return xt::flip(inferno(N), 0);
}

/**
Inverse of cppcolormap::plasma.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> plasma_r(size_t N = 256)
{
    return xt::flip(plasma(N), 0);
}

/**
Inverse of cppcolormap::viridis.

\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> viridis_r(size_t N = 256)
{
    return xt::flip(viridis(N), 0);
}

/**
Get colormap specified as string.

\param cmap Name of the colormap.
\param N Number of colors to output.
\returns RGB data.
*/
inline xt::xtensor<double, 2> colormap(const std::string& cmap, size_t N = 256)
{
    if (cmap == "Accent") { return Accent(N); }
    if (cmap == "Dark2") { return Dark2(N); }
    if (cmap == "Paired") { return Paired(N); }
    if (cmap == "Spectral") { return Spectral(N); }
    if (cmap == "Pastel1") { return Pastel1(N); }
    if (cmap == "Pastel2") { return Pastel2(N); }
    if (cmap == "Set1") { return Set1(N); }
    if (cmap == "Set2") { return Set2(N); }
    if (cmap == "Set3") { return Set3(N); }
    if (cmap == "Blues") { return Blues(N); }
    if (cmap == "Greens") { return Greens(N); }
    if (cmap == "Greys") { return Greys(N); }
    if (cmap == "Oranges") { return Oranges(N); }
    if (cmap == "Purples") { return Purples(N); }
    if (cmap == "Reds") { return Reds(N); }
    if (cmap == "BuPu") { return BuPu(N); }
    if (cmap == "GnBu") { return GnBu(N); }
    if (cmap == "PuBu") { return PuBu(N); }
    if (cmap == "PuBuGn") { return PuBuGn(N); }
    if (cmap == "PuRd") { return PuRd(N); }
    if (cmap == "RdPu") { return RdPu(N); }
    if (cmap == "OrRd") { return OrRd(N); }
    if (cmap == "RdOrYl") { return RdOrYl(N); }
    if (cmap == "YlGn") { return YlGn(N); }
    if (cmap == "YlGnBu") { return YlGnBu(N); }
    if (cmap == "YlOrRd") { return YlOrRd(N); }
    if (cmap == "BrBG") { return BrBG(N); }
    if (cmap == "PuOr") { return PuOr(N); }
    if (cmap == "RdBu") { return RdBu(N); }
    if (cmap == "RdGy") { return RdGy(N); }
    if (cmap == "RdYlBu") { return RdYlBu(N); }
    if (cmap == "RdYlGn") { return RdYlGn(N); }
    if (cmap == "PiYG") { return PiYG(N); }
    if (cmap == "PRGn") { return PRGn(N); }
    if (cmap == "spring") { return spring(N); }
    if (cmap == "summer") { return summer(N); }
    if (cmap == "autumn") { return autumn(N); }
    if (cmap == "winter") { return winter(N); }
    if (cmap == "bone") { return bone(N); }
    if (cmap == "cool") { return cool(N); }
    if (cmap == "hot") { return hot(N); }
    if (cmap == "copper") { return copper(N); }
    if (cmap == "hsv") { return hsv(N); }
    if (cmap == "nipy_spectral") { return nipy_spectral(N); }
    if (cmap == "jet") { return jet(N); }
    if (cmap == "terrain") { return terrain(N); }
    if (cmap == "seismic") { return seismic(N); }
    if (cmap == "afmhot") { return afmhot(N); }
    if (cmap == "magma") { return magma(N); }
    if (cmap == "inferno") { return inferno(N); }
    if (cmap == "plasma") { return plasma(N); }
    if (cmap == "viridis") { return viridis(N); }
    if (cmap == "Accent_r") { return Accent_r(N); }
    if (cmap == "Dark2_r") { return Dark2_r(N); }
    if (cmap == "Paired_r") { return Paired_r(N); }
    if (cmap == "Spectral_r") { return Spectral_r(N); }
    if (cmap == "Pastel1_r") { return Pastel1_r(N); }
    if (cmap == "Pastel2_r") { return Pastel2_r(N); }
    if (cmap == "Set1_r") { return Set1_r(N); }
    if (cmap == "Set2_r") { return Set2_r(N); }
    if (cmap == "Set3_r") { return Set3_r(N); }
    if (cmap == "Blues_r") { return Blues_r(N); }
    if (cmap == "Greens_r") { return Greens_r(N); }
    if (cmap == "Greys_r") { return Greys_r(N); }
    if (cmap == "Oranges_r") { return Oranges_r(N); }
    if (cmap == "Purples_r") { return Purples_r(N); }
    if (cmap == "Reds_r") { return Reds_r(N); }
    if (cmap == "BuPu_r") { return BuPu_r(N); }
    if (cmap == "GnBu_r") { return GnBu_r(N); }
    if (cmap == "PuBu_r") { return PuBu_r(N); }
    if (cmap == "PuBuGn_r") { return PuBuGn_r(N); }
    if (cmap == "PuRd_r") { return PuRd_r(N); }
    if (cmap == "RdPu_r") { return RdPu_r(N); }
    if (cmap == "OrRd_r") { return OrRd_r(N); }
    if (cmap == "RdOrYl_r") { return RdOrYl_r(N); }
    if (cmap == "YlGn_r") { return YlGn_r(N); }
    if (cmap == "YlGnBu_r") { return YlGnBu_r(N); }
    if (cmap == "YlOrRd_r") { return YlOrRd_r(N); }
    if (cmap == "BrBG_r") { return BrBG_r(N); }
    if (cmap == "PuOr_r") { return PuOr_r(N); }
    if (cmap == "RdBu_r") { return RdBu_r(N); }
    if (cmap == "RdGy_r") { return RdGy_r(N); }
    if (cmap == "RdYlBu_r") { return RdYlBu_r(N); }
    if (cmap == "RdYlGn_r") { return RdYlGn_r(N); }
    if (cmap == "PiYG_r") { return PiYG_r(N); }
    if (cmap == "PRGn_r") { return PRGn_r(N); }
    if (cmap == "spring_r") { return spring_r(N); }
    if (cmap == "summer_r") { return summer_r(N); }
    if (cmap == "autumn_r") { return autumn_r(N); }
    if (cmap == "winter_r") { return winter_r(N); }
    if (cmap == "bone_r") { return bone_r(N); }
    if (cmap == "cool_r") { return cool_r(N); }
    if (cmap == "hot_r") { return hot_r(N); }
    if (cmap == "copper_r") { return copper_r(N); }
    if (cmap == "hsv_r") { return hsv_r(N); }
    if (cmap == "nipy_spectral_r") { return nipy_spectral_r(N); }
    if (cmap == "jet_r") { return jet_r(N); }
    if (cmap == "terrain_r") { return terrain_r(N); }
    if (cmap == "seismic_r") { return seismic_r(N); }
    if (cmap == "afmhot_r") { return afmhot_r(N); }
    if (cmap == "magma_r") { return magma_r(N); }
    if (cmap == "inferno_r") { return inferno_r(N); }
    if (cmap == "plasma_r") { return plasma_r(N); }
    if (cmap == "viridis_r") { return viridis_r(N); }
    if (cmap == "White") { return White(N); }
    if (cmap == "Grey") { return Grey(N); }
    if (cmap == "Black") { return Black(N); }
    if (cmap == "Red") { return Red(N); }
    if (cmap == "Blue") { return Blue(N); }
    if (cmap == "tuewarmred") { return tuewarmred(N); }
    if (cmap == "tuedarkblue") { return tuedarkblue(N); }
    if (cmap == "tueblue") { return tueblue(N); }
    if (cmap == "tuelightblue") { return tuelightblue(N); }
    if (cmap == "Apricot") { return Apricot(N); }
    if (cmap == "Aquamarine") { return Aquamarine(N); }
    if (cmap == "Bittersweet") { return Bittersweet(N); }
    if (cmap == "BlueGreen") { return BlueGreen(N); }
    if (cmap == "BlueViolet") { return BlueViolet(N); }
    if (cmap == "BrickRed") { return BrickRed(N); }
    if (cmap == "Brown") { return Brown(N); }
    if (cmap == "BurntOrange") { return BurntOrange(N); }
    if (cmap == "CadetBlue") { return CadetBlue(N); }
    if (cmap == "CarnationPink") { return CarnationPink(N); }
    if (cmap == "Cerulean") { return Cerulean(N); }
    if (cmap == "CornflowerBlue") { return CornflowerBlue(N); }
    if (cmap == "Cyan") { return Cyan(N); }
    if (cmap == "Dandelion") { return Dandelion(N); }
    if (cmap == "DarkOrchid") { return DarkOrchid(N); }
    if (cmap == "Emerald") { return Emerald(N); }
    if (cmap == "ForestGreen") { return ForestGreen(N); }
    if (cmap == "Fuchsia") { return Fuchsia(N); }
    if (cmap == "Goldenrod") { return Goldenrod(N); }
    if (cmap == "Gray") { return Gray(N); }
    if (cmap == "Green") { return Green(N); }
    if (cmap == "GreenYellow") { return GreenYellow(N); }
    if (cmap == "JungleGreen") { return JungleGreen(N); }
    if (cmap == "Lavender") { return Lavender(N); }
    if (cmap == "LimeGreen") { return LimeGreen(N); }
    if (cmap == "Magenta") { return Magenta(N); }
    if (cmap == "Mahogany") { return Mahogany(N); }
    if (cmap == "Maroon") { return Maroon(N); }
    if (cmap == "Melon") { return Melon(N); }
    if (cmap == "MidnightBlue") { return MidnightBlue(N); }
    if (cmap == "Mulberry") { return Mulberry(N); }
    if (cmap == "NavyBlue") { return NavyBlue(N); }
    if (cmap == "OliveGreen") { return OliveGreen(N); }
    if (cmap == "Orange") { return Orange(N); }
    if (cmap == "OrangeRed") { return OrangeRed(N); }
    if (cmap == "Orchid") { return Orchid(N); }
    if (cmap == "Peach") { return Peach(N); }
    if (cmap == "Periwinkle") { return Periwinkle(N); }
    if (cmap == "PineGreen") { return PineGreen(N); }
    if (cmap == "Plum") { return Plum(N); }
    if (cmap == "ProcessBlue") { return ProcessBlue(N); }
    if (cmap == "Purple") { return Purple(N); }
    if (cmap == "RawSienna") { return RawSienna(N); }
    if (cmap == "RedOrange") { return RedOrange(N); }
    if (cmap == "RedViolet") { return RedViolet(N); }
    if (cmap == "Rhodamine") { return Rhodamine(N); }
    if (cmap == "RoyalBlue") { return RoyalBlue(N); }
    if (cmap == "RoyalPurple") { return RoyalPurple(N); }
    if (cmap == "RubineRed") { return RubineRed(N); }
    if (cmap == "Salmon") { return Salmon(N); }
    if (cmap == "SeaGreen") { return SeaGreen(N); }
    if (cmap == "Sepia") { return Sepia(N); }
    if (cmap == "SkyBlue") { return SkyBlue(N); }
    if (cmap == "SpringGreen") { return SpringGreen(N); }
    if (cmap == "Tan") { return Tan(N); }
    if (cmap == "TealBlue") { return TealBlue(N); }
    if (cmap == "Thistle") { return Thistle(N); }
    if (cmap == "Turquoise") { return Turquoise(N); }
    if (cmap == "Violet") { return Violet(N); }
    if (cmap == "VioletRed") { return VioletRed(N); }
    if (cmap == "WildStrawberry") { return WildStrawberry(N); }
    if (cmap == "Yellow") { return Yellow(N); }
    if (cmap == "YellowGreen") { return YellowGreen(N); }
    if (cmap == "YellowOrange") { return YellowOrange(N); }

    throw std::runtime_error("Colormap not recognized");
}

/**
xterm color-cyle.

\returns RGB data.
*/
inline xt::xtensor<double, 2> xterm()
{
    xt::xtensor<double, 2> data = {
        {  0,   0,   0}, // 0  , Black (SYSTEM) ,  #000000, hsl(0,0%,0%)
        {128,   0,   0}, // 1  , Maroon (SYSTEM),  #800000, hsl(0,100%,25%)
        {  0, 128,   0}, // 2  , Green (SYSTEM) ,  #008000, hsl(120,100%,25%)
        {128, 128,   0}, // 3  , Olive (SYSTEM) ,  #808000, hsl(60,100%,25%)
        {  0,   0, 128}, // 4  , Navy (SYSTEM)  ,  #000080, hsl(240,100%,25%)
        {128,   0, 128}, // 5  , Purple (SYSTEM),  #800080, hsl(300,100%,25%)
        {  0, 128, 128}, // 6  , Teal (SYSTEM)  ,  #008080, hsl(180,100%,25%)
        {192, 192, 192}, // 7  , Silver (SYSTEM),  #c0c0c0, hsl(0,0%,75%)
        {128, 128, 128}, // 8  , Grey (SYSTEM)  ,  #808080, hsl(0,0%,50%)
        {255,   0,   0}, // 9  , Red (SYSTEM)   ,  #ff0000, hsl(0,100%,50%)
        {  0, 255,   0}, // 10 , Lime (SYSTEM)  ,  #00ff00, hsl(120,100%,50%)
        {255, 255,   0}, // 11 , Yellow (SYSTEM),  #ffff00, hsl(60,100%,50%)
        {  0,   0, 255}, // 12 , Blue (SYSTEM)  ,  #0000ff, hsl(240,100%,50%)
        {255,   0, 255}, // 13 , Fuchsia (SYSTEM,) #ff00ff, hsl(300,100%,50%)
        {  0, 255, 255}, // 14 , Aqua (SYSTEM)  ,  #00ffff, hsl(180,100%,50%)
        {255, 255, 255}, // 15 , White (SYSTEM) ,  #ffffff, hsl(0,0%,100%)
        {  0,   0,   0}, // 16 , Grey0          ,  #000000, hsl(0,0%,0%)
        {  0,   0,  95}, // 17 , NavyBlue       ,  #00005f, hsl(240,100%,18%)
        {  0,   0, 135}, // 18 , DarkBlue       ,  #000087, hsl(240,100%,26%)
        {  0,   0, 175}, // 19 , Blue3          ,  #0000af, hsl(240,100%,34%)
        {  0,   0, 215}, // 20 , Blue3          ,  #0000d7, hsl(240,100%,42%)
        {  0,   0, 255}, // 21 , Blue1          ,  #0000ff, hsl(240,100%,50%)
        {  0,  95,   0}, // 22 , DarkGreen      ,  #005f00, hsl(120,100%,18%)
        {  0,  95,  95}, // 23 , DeepSkyBlue4   ,  #005f5f, hsl(180,100%,18%)
        {  0,  95, 135}, // 24 , DeepSkyBlue4   ,  #005f87, hsl(97,100%,26%)
        {  0,  95, 175}, // 25 , DeepSkyBlue4   ,  #005faf, hsl(07,100%,34%)
        {  0,  95, 215}, // 26 , DodgerBlue3    ,  #005fd7, hsl(13,100%,42%)
        {  0,  95, 255}, // 27 , DodgerBlue2    ,  #005fff, hsl(17,100%,50%)
        {  0, 135,   0}, // 28 , Green4         ,  #008700, hsl(120,100%,26%)
        {  0, 135,  95}, // 29 , SpringGreen4   ,  #00875f, hsl(62,100%,26%)
        {  0, 135, 135}, // 30 , Turquoise4     ,  #008787, hsl(180,100%,26%)
        {  0, 135, 175}, // 31 , DeepSkyBlue3   ,  #0087af, hsl(93,100%,34%)
        {  0, 135, 215}, // 32 , DeepSkyBlue3   ,  #0087d7, hsl(02,100%,42%)
        {  0, 135, 255}, // 33 , DodgerBlue1    ,  #0087ff, hsl(08,100%,50%)
        {  0, 175,   0}, // 34 , Green3         ,  #00af00, hsl(120,100%,34%)
        {  0, 175,  95}, // 35 , SpringGreen3   ,  #00af5f, hsl(52,100%,34%)
        {  0, 175, 135}, // 36 , DarkCyan       ,  #00af87, hsl(66,100%,34%)
        {  0, 175, 175}, // 37 , LightSeaGreen  ,  #00afaf, hsl(180,100%,34%)
        {  0, 175, 215}, // 38 , DeepSkyBlue2   ,  #00afd7, hsl(91,100%,42%)
        {  0, 175, 255}, // 39 , DeepSkyBlue1   ,  #00afff, hsl(98,100%,50%)
        {  0, 215,   0}, // 40 , Green3         ,  #00d700, hsl(120,100%,42%)
        {  0, 215,  95}, // 41 , SpringGreen3   ,  #00d75f, hsl(46,100%,42%)
        {  0, 215, 135}, // 42 , SpringGreen2   ,  #00d787, hsl(57,100%,42%)
        {  0, 215, 175}, // 43 , Cyan3          ,  #00d7af, hsl(68,100%,42%)
        {  0, 215, 215}, // 44 , DarkTurquoise  ,  #00d7d7, hsl(180,100%,42%)
        {  0, 215, 255}, // 45 , Turquoise2     ,  #00d7ff, hsl(89,100%,50%)
        {  0, 255,   0}, // 46 , Green1         ,  #00ff00, hsl(120,100%,50%)
        {  0, 255,  95}, // 47 , SpringGreen2   ,  #00ff5f, hsl(42,100%,50%)
        {  0, 255, 135}, // 48 , SpringGreen1   ,  #00ff87, hsl(51,100%,50%)
        {  0, 255, 175}, // 49 , MediumSpringGre,en#00ffaf, hsl(61,100%,50%)
        {  0, 255, 215}, // 50 , Cyan2          ,  #00ffd7, hsl(70,100%,50%)
        {  0, 255, 255}, // 51 , Cyan1          ,  #00ffff, hsl(180,100%,50%)
        { 95,   0,   0}, // 52 , DarkRed        ,  #5f0000, hsl(0,100%,18%)
        { 95,   0,  95}, // 53 , DeepPink4      ,  #5f005f, hsl(300,100%,18%)
        { 95,   0, 135}, // 54 , Purple4        ,  #5f0087, hsl(82,100%,26%)
        { 95,   0, 175}, // 55 , Purple4        ,  #5f00af, hsl(72,100%,34%)
        { 95,   0, 215}, // 56 , Purple3        ,  #5f00d7, hsl(66,100%,42%)
        { 95,   0, 255}, // 57 , BlueViolet     ,  #5f00ff, hsl(62,100%,50%)
        { 95,  95,   0}, // 58 , Orange4        ,  #5f5f00, hsl(60,100%,18%)
        { 95,  95,  95}, // 59 , Grey37         ,  #5f5f5f, hsl(0,0%,37%)
        { 95,  95, 135}, // 60 , MediumPurple4  ,  #5f5f87, hsl(240,17%,45%)
        { 95,  95, 175}, // 61 , SlateBlue3     ,  #5f5faf, hsl(240,33%,52%)
        { 95,  95, 215}, // 62 , SlateBlue3     ,  #5f5fd7, hsl(240,60%,60%)
        { 95,  95, 255}, // 63 , RoyalBlue1     ,  #5f5fff, hsl(240,100%,68%)
        { 95, 135,   0}, // 64 , Chartreuse4    ,  #5f8700, hsl(7,100%,26%)
        { 95, 135,  95}, // 65 , DarkSeaGreen4  ,  #5f875f, hsl(120,17%,45%)
        { 95, 135, 135}, // 66 , PaleTurquoise4 ,  #5f8787, hsl(180,17%,45%)
        { 95, 135, 175}, // 67 , SteelBlue      ,  #5f87af, hsl(210,33%,52%)
        { 95, 135, 215}, // 68 , SteelBlue3     ,  #5f87d7, hsl(220,60%,60%)
        { 95, 135, 255}, // 69 , CornflowerBlue ,  #5f87ff, hsl(225,100%,68%)
        { 95, 175,   0}, // 70 , Chartreuse3    ,  #5faf00, hsl(7,100%,34%)
        { 95, 175,  95}, // 71 , DarkSeaGreen4  ,  #5faf5f, hsl(120,33%,52%)
        { 95, 175, 135}, // 72 , CadetBlue      ,  #5faf87, hsl(150,33%,52%)
        { 95, 175, 175}, // 73 , CadetBlue      ,  #5fafaf, hsl(180,33%,52%)
        { 95, 175, 215}, // 74 , SkyBlue3       ,  #5fafd7, hsl(200,60%,60%)
        { 95, 175, 255}, // 75 , SteelBlue1     ,  #5fafff, hsl(210,100%,68%)
        { 95, 215,   0}, // 76 , Chartreuse3    ,  #5fd700, hsl(3,100%,42%)
        { 95, 215,  95}, // 77 , PaleGreen3     ,  #5fd75f, hsl(120,60%,60%)
        { 95, 215, 135}, // 78 , SeaGreen3      ,  #5fd787, hsl(140,60%,60%)
        { 95, 215, 175}, // 79 , Aquamarine3    ,  #5fd7af, hsl(160,60%,60%)
        { 95, 215, 215}, // 80 , MediumTurquoise,  #5fd7d7, hsl(180,60%,60%)
        { 95, 215, 255}, // 81 , SteelBlue1     ,  #5fd7ff, hsl(195,100%,68%)
        { 95, 255,   0}, // 82 , Chartreuse2    ,  #5fff00, hsl(7,100%,50%)
        { 95, 255,  95}, // 83 , SeaGreen2      ,  #5fff5f, hsl(120,100%,68%)
        { 95, 255, 135}, // 84 , SeaGreen1      ,  #5fff87, hsl(135,100%,68%)
        { 95, 255, 175}, // 85 , SeaGreen1      ,  #5fffaf, hsl(150,100%,68%)
        { 95, 255, 215}, // 86 , Aquamarine1    ,  #5fffd7, hsl(165,100%,68%)
        { 95, 255, 255}, // 87 , DarkSlateGray2 ,  #5fffff, hsl(180,100%,68%)
        {135,   0,   0}, // 88 , DarkRed        ,  #870000, hsl(0,100%,26%)
        {135,   0,  95}, // 89 , DeepPink4      ,  #87005f, hsl(17,100%,26%)
        {135,   0, 135}, // 90 , DarkMagenta    ,  #870087, hsl(300,100%,26%)
        {135,   0, 175}, // 91 , DarkMagenta    ,  #8700af, hsl(86,100%,34%)
        {135,   0, 215}, // 92 , DarkViolet     ,  #8700d7, hsl(77,100%,42%)
        {135,   0, 255}, // 93 , Purple         ,  #8700ff, hsl(71,100%,50%)
        {135,  95,   0}, // 94 , Orange4        ,  #875f00, hsl(2,100%,26%)
        {135,  95,  95}, // 95 , LightPink4     ,  #875f5f, hsl(0,17%,45%)
        {135,  95, 135}, // 96 , Plum4          ,  #875f87, hsl(300,17%,45%)
        {135,  95, 175}, // 97 , MediumPurple3  ,  #875faf, hsl(270,33%,52%)
        {135,  95, 215}, // 98 , MediumPurple3  ,  #875fd7, hsl(260,60%,60%)
        {135,  95, 255}, // 99 , SlateBlue1     ,  #875fff, hsl(255,100%,68%)
        {135, 135,   0}, // 100, Yellow4        ,  #878700, hsl(60,100%,26%)
        {135, 135,  95}, // 101, Wheat4         ,  #87875f, hsl(60,17%,45%)
        {135, 135, 135}, // 102, Grey53         ,  #878787, hsl(0,0%,52%)
        {135, 135, 175}, // 103, LightSlateGrey ,  #8787af, hsl(240,20%,60%)
        {135, 135, 215}, // 104, MediumPurple   ,  #8787d7, hsl(240,50%,68%)
        {135, 135, 255}, // 105, LightSlateBlue ,  #8787ff, hsl(240,100%,76%)
        {135, 175,   0}, // 106, Yellow4        ,  #87af00, hsl(3,100%,34%)
        {135, 175,  95}, // 107, DarkOliveGreen3,  #87af5f, hsl(90,33%,52%)
        {135, 175, 135}, // 108, DarkSeaGreen   ,  #87af87, hsl(120,20%,60%)
        {135, 175, 175}, // 109, LightSkyBlue3  ,  #87afaf, hsl(180,20%,60%)
        {135, 175, 215}, // 110, LightSkyBlue3  ,  #87afd7, hsl(210,50%,68%)
        {135, 175, 255}, // 111, SkyBlue2       ,  #87afff, hsl(220,100%,76%)
        {135, 215,   0}, // 112, Chartreuse2    ,  #87d700, hsl(2,100%,42%)
        {135, 215,  95}, // 113, DarkOliveGreen3,  #87d75f, hsl(100,60%,60%)
        {135, 215, 135}, // 114, PaleGreen3     ,  #87d787, hsl(120,50%,68%)
        {135, 215, 175}, // 115, DarkSeaGreen3  ,  #87d7af, hsl(150,50%,68%)
        {135, 215, 215}, // 116, DarkSlateGray3 ,  #87d7d7, hsl(180,50%,68%)
        {135, 215, 255}, // 117, SkyBlue1       ,  #87d7ff, hsl(200,100%,76%)
        {135, 255,   0}, // 118, Chartreuse1    ,  #87ff00, hsl(8,100%,50%)
        {135, 255,  95}, // 119, LightGreen     ,  #87ff5f, hsl(105,100%,68%)
        {135, 255, 135}, // 120, LightGreen     ,  #87ff87, hsl(120,100%,76%)
        {135, 255, 175}, // 121, PaleGreen1     ,  #87ffaf, hsl(140,100%,76%)
        {135, 255, 215}, // 122, Aquamarine1    ,  #87ffd7, hsl(160,100%,76%)
        {135, 255, 255}, // 123, DarkSlateGray1 ,  #87ffff, hsl(180,100%,76%)
        {175,   0,   0}, // 124, Red3           ,  #af0000, hsl(0,100%,34%)
        {175,   0,  95}, // 125, DeepPink4      ,  #af005f, hsl(27,100%,34%)
        {175,   0, 135}, // 126, MediumVioletRed,  #af0087, hsl(13,100%,34%)
        {175,   0, 175}, // 127, Magenta3       ,  #af00af, hsl(300,100%,34%)
        {175,   0, 215}, // 128, DarkViolet     ,  #af00d7, hsl(88,100%,42%)
        {175,   0, 255}, // 129, Purple         ,  #af00ff, hsl(81,100%,50%)
        {175,  95,   0}, // 130, DarkOrange3    ,  #af5f00, hsl(2,100%,34%)
        {175,  95,  95}, // 131, IndianRed      ,  #af5f5f, hsl(0,33%,52%)
        {175,  95, 135}, // 132, HotPink3       ,  #af5f87, hsl(330,33%,52%)
        {175,  95, 175}, // 133, MediumOrchid3  ,  #af5faf, hsl(300,33%,52%)
        {175,  95, 215}, // 134, MediumOrchid   ,  #af5fd7, hsl(280,60%,60%)
        {175,  95, 255}, // 135, MediumPurple2  ,  #af5fff, hsl(270,100%,68%)
        {175, 135,   0}, // 136, DarkGoldenrod  ,  #af8700, hsl(6,100%,34%)
        {175, 135,  95}, // 137, LightSalmon3   ,  #af875f, hsl(30,33%,52%)
        {175, 135, 135}, // 138, RosyBrown      ,  #af8787, hsl(0,20%,60%)
        {175, 135, 175}, // 139, Grey63         ,  #af87af, hsl(300,20%,60%)
        {175, 135, 215}, // 140, MediumPurple2  ,  #af87d7, hsl(270,50%,68%)
        {175, 135, 255}, // 141, MediumPurple1  ,  #af87ff, hsl(260,100%,76%)
        {175, 175,   0}, // 142, Gold3          ,  #afaf00, hsl(60,100%,34%)
        {175, 175,  95}, // 143, DarkKhaki      ,  #afaf5f, hsl(60,33%,52%)
        {175, 175, 135}, // 144, NavajoWhite3   ,  #afaf87, hsl(60,20%,60%)
        {175, 175, 175}, // 145, Grey69         ,  #afafaf, hsl(0,0%,68%)
        {175, 175, 215}, // 146, LightSteelBlue3,  #afafd7, hsl(240,33%,76%)
        {175, 175, 255}, // 147, LightSteelBlue ,  #afafff, hsl(240,100%,84%)
        {175, 215,   0}, // 148, Yellow3        ,  #afd700, hsl(1,100%,42%)
        {175, 215,  95}, // 149, DarkOliveGreen3,  #afd75f, hsl(80,60%,60%)
        {175, 215, 135}, // 150, DarkSeaGreen3  ,  #afd787, hsl(90,50%,68%)
        {175, 215, 175}, // 151, DarkSeaGreen2  ,  #afd7af, hsl(120,33%,76%)
        {175, 215, 215}, // 152, LightCyan3     ,  #afd7d7, hsl(180,33%,76%)
        {175, 215, 255}, // 153, LightSkyBlue1  ,  #afd7ff, hsl(210,100%,84%)
        {175, 255,   0}, // 154, GreenYellow    ,  #afff00, hsl(8,100%,50%)
        {175, 255,  95}, // 155, DarkOliveGreen2,  #afff5f, hsl(90,100%,68%)
        {175, 255, 135}, // 156, PaleGreen1     ,  #afff87, hsl(100,100%,76%)
        {175, 255, 175}, // 157, DarkSeaGreen2  ,  #afffaf, hsl(120,100%,84%)
        {175, 255, 215}, // 158, DarkSeaGreen1  ,  #afffd7, hsl(150,100%,84%)
        {175, 255, 255}, // 159, PaleTurquoise1 ,  #afffff, hsl(180,100%,84%)
        {215,   0,   0}, // 160, Red3           ,  #d70000, hsl(0,100%,42%)
        {215,   0,  95}, // 161, DeepPink3      ,  #d7005f, hsl(33,100%,42%)
        {215,   0, 135}, // 162, DeepPink3      ,  #d70087, hsl(22,100%,42%)
        {215,   0, 175}, // 163, Magenta3       ,  #d700af, hsl(11,100%,42%)
        {215,   0, 215}, // 164, Magenta3       ,  #d700d7, hsl(300,100%,42%)
        {215,   0, 255}, // 165, Magenta2       ,  #d700ff, hsl(90,100%,50%)
        {215,  95,   0}, // 166, DarkOrange3    ,  #d75f00, hsl(6,100%,42%)
        {215,  95,  95}, // 167, IndianRed      ,  #d75f5f, hsl(0,60%,60%)
        {215,  95, 135}, // 168, HotPink3       ,  #d75f87, hsl(340,60%,60%)
        {215,  95, 175}, // 169, HotPink2       ,  #d75faf, hsl(320,60%,60%)
        {215,  95, 215}, // 170, Orchid         ,  #d75fd7, hsl(300,60%,60%)
        {215,  95, 255}, // 171, MediumOrchid1  ,  #d75fff, hsl(285,100%,68%)
        {215, 135,   0}, // 172, Orange3        ,  #d78700, hsl(7,100%,42%)
        {215, 135,  95}, // 173, LightSalmon3   ,  #d7875f, hsl(20,60%,60%)
        {215, 135, 135}, // 174, LightPink3     ,  #d78787, hsl(0,50%,68%)
        {215, 135, 175}, // 175, Pink3          ,  #d787af, hsl(330,50%,68%)
        {215, 135, 215}, // 176, Plum3          ,  #d787d7, hsl(300,50%,68%)
        {215, 135, 255}, // 177, Violet         ,  #d787ff, hsl(280,100%,76%)
        {215, 175,   0}, // 178, Gold3          ,  #d7af00, hsl(8,100%,42%)
        {215, 175,  95}, // 179, LightGoldenrod3,  #d7af5f, hsl(40,60%,60%)
        {215, 175, 135}, // 180, Tan            ,  #d7af87, hsl(30,50%,68%)
        {215, 175, 175}, // 181, MistyRose3     ,  #d7afaf, hsl(0,33%,76%)
        {215, 175, 215}, // 182, Thistle3       ,  #d7afd7, hsl(300,33%,76%)
        {215, 175, 255}, // 183, Plum2          ,  #d7afff, hsl(270,100%,84%)
        {215, 215,   0}, // 184, Yellow3        ,  #d7d700, hsl(60,100%,42%)
        {215, 215,  95}, // 185, Khaki3         ,  #d7d75f, hsl(60,60%,60%)
        {215, 215, 135}, // 186, LightGoldenrod2,  #d7d787, hsl(60,50%,68%)
        {215, 215, 175}, // 187, LightYellow3   ,  #d7d7af, hsl(60,33%,76%)
        {215, 215, 215}, // 188, Grey84         ,  #d7d7d7, hsl(0,0%,84%)
        {215, 215, 255}, // 189, LightSteelBlue1,  #d7d7ff, hsl(240,100%,92%)
        {215, 255,   0}, // 190, Yellow2        ,  #d7ff00, hsl(9,100%,50%)
        {215, 255,  95}, // 191, DarkOliveGreen1,  #d7ff5f, hsl(75,100%,68%)
        {215, 255, 135}, // 192, DarkOliveGreen1,  #d7ff87, hsl(80,100%,76%)
        {215, 255, 175}, // 193, DarkSeaGreen1  ,  #d7ffaf, hsl(90,100%,84%)
        {215, 255, 215}, // 194, Honeydew2      ,  #d7ffd7, hsl(120,100%,92%)
        {215, 255, 255}, // 195, LightCyan1     ,  #d7ffff, hsl(180,100%,92%)
        {255,   0,   0}, // 196, Red1           ,  #ff0000, hsl(0,100%,50%)
        {255,   0,  95}, // 197, DeepPink2      ,  #ff005f, hsl(37,100%,50%)
        {255,   0, 135}, // 198, DeepPink1      ,  #ff0087, hsl(28,100%,50%)
        {255,   0, 175}, // 199, DeepPink1      ,  #ff00af, hsl(18,100%,50%)
        {255,   0, 215}, // 200, Magenta2       ,  #ff00d7, hsl(09,100%,50%)
        {255,   0, 255}, // 201, Magenta1       ,  #ff00ff, hsl(300,100%,50%)
        {255,  95,   0}, // 202, OrangeRed1     ,  #ff5f00, hsl(2,100%,50%)
        {255,  95,  95}, // 203, IndianRed1     ,  #ff5f5f, hsl(0,100%,68%)
        {255,  95, 135}, // 204, IndianRed1     ,  #ff5f87, hsl(345,100%,68%)
        {255,  95, 175}, // 205, HotPink        ,  #ff5faf, hsl(330,100%,68%)
        {255,  95, 215}, // 206, HotPink        ,  #ff5fd7, hsl(315,100%,68%)
        {255,  95, 255}, // 207, MediumOrchid1  ,  #ff5fff, hsl(300,100%,68%)
        {255, 135,   0}, // 208, DarkOrange     ,  #ff8700, hsl(1,100%,50%)
        {255, 135,  95}, // 209, Salmon1        ,  #ff875f, hsl(15,100%,68%)
        {255, 135, 135}, // 210, LightCoral     ,  #ff8787, hsl(0,100%,76%)
        {255, 135, 175}, // 211, PaleVioletRed1 ,  #ff87af, hsl(340,100%,76%)
        {255, 135, 215}, // 212, Orchid2        ,  #ff87d7, hsl(320,100%,76%)
        {255, 135, 255}, // 213, Orchid1        ,  #ff87ff, hsl(300,100%,76%)
        {255, 175,   0}, // 214, Orange1        ,  #ffaf00, hsl(1,100%,50%)
        {255, 175,  95}, // 215, SandyBrown     ,  #ffaf5f, hsl(30,100%,68%)
        {255, 175, 135}, // 216, LightSalmon1   ,  #ffaf87, hsl(20,100%,76%)
        {255, 175, 175}, // 217, LightPink1     ,  #ffafaf, hsl(0,100%,84%)
        {255, 175, 215}, // 218, Pink1          ,  #ffafd7, hsl(330,100%,84%)
        {255, 175, 255}, // 219, Plum1          ,  #ffafff, hsl(300,100%,84%)
        {255, 215,   0}, // 220, Gold1          ,  #ffd700, hsl(0,100%,50%)
        {255, 215,  95}, // 221, LightGoldenrod2,  #ffd75f, hsl(45,100%,68%)
        {255, 215, 135}, // 222, LightGoldenrod2,  #ffd787, hsl(40,100%,76%)
        {255, 215, 175}, // 223, NavajoWhite1   ,  #ffd7af, hsl(30,100%,84%)
        {255, 215, 215}, // 224, MistyRose1     ,  #ffd7d7, hsl(0,100%,92%)
        {255, 215, 255}, // 225, Thistle1       ,  #ffd7ff, hsl(300,100%,92%)
        {255, 255,   0}, // 226, Yellow1        ,  #ffff00, hsl(60,100%,50%)
        {255, 255,  95}, // 227, LightGoldenrod1,  #ffff5f, hsl(60,100%,68%)
        {255, 255, 135}, // 228, Khaki1         ,  #ffff87, hsl(60,100%,76%)
        {255, 255, 175}, // 229, Wheat1         ,  #ffffaf, hsl(60,100%,84%)
        {255, 255, 215}, // 230, Cornsilk1      ,  #ffffd7, hsl(60,100%,92%)
        {255, 255, 255}, // 231, Grey100        ,  #ffffff, hsl(0,0%,100%)
        {  8,   8,   8}, // 232, Grey3          ,  #080808, hsl(0,0%,3%)
        { 18,  18,  18}, // 233, Grey7          ,  #121212, hsl(0,0%,7%)
        { 28,  28,  28}, // 234, Grey11         ,  #1c1c1c, hsl(0,0%,10%)
        { 38,  38,  38}, // 235, Grey15         ,  #262626, hsl(0,0%,14%)
        { 48,  48,  48}, // 236, Grey19         ,  #303030, hsl(0,0%,18%)
        { 58,  58,  58}, // 237, Grey23         ,  #3a3a3a, hsl(0,0%,22%)
        { 68,  68,  68}, // 238, Grey27         ,  #444444, hsl(0,0%,26%)
        { 78,  78,  78}, // 239, Grey30         ,  #4e4e4e, hsl(0,0%,30%)
        { 88,  88,  88}, // 240, Grey35         ,  #585858, hsl(0,0%,34%)
        { 98,  98,  98}, // 241, Grey39         ,  #626262, hsl(0,0%,37%)
        {108, 108, 108}, // 242, Grey42         ,  #6c6c6c, hsl(0,0%,40%)
        {118, 118, 118}, // 243, Grey46         ,  #767676, hsl(0,0%,46%)
        {128, 128, 128}, // 244, Grey50         ,  #808080, hsl(0,0%,50%)
        {138, 138, 138}, // 245, Grey54         ,  #8a8a8a, hsl(0,0%,54%)
        {148, 148, 148}, // 246, Grey58         ,  #949494, hsl(0,0%,58%)
        {158, 158, 158}, // 247, Grey62         ,  #9e9e9e, hsl(0,0%,61%)
        {168, 168, 168}, // 248, Grey66         ,  #a8a8a8, hsl(0,0%,65%)
        {178, 178, 178}, // 249, Grey70         ,  #b2b2b2, hsl(0,0%,69%)
        {188, 188, 188}, // 250, Grey74         ,  #bcbcbc, hsl(0,0%,73%)
        {198, 198, 198}, // 251, Grey78         ,  #c6c6c6, hsl(0,0%,77%)
        {208, 208, 208}, // 252, Grey82         ,  #d0d0d0, hsl(0,0%,81%)
        {218, 218, 218}, // 253, Grey85         ,  #dadada, hsl(0,0%,85%)
        {228, 228, 228}, // 254, Grey89         ,  #e4e4e4, hsl(0,0%,89%)
        {238, 238, 238}, // 255, Grey93         ,  #eeeeee, hsl(0,0%,93%)
    };

    return data / 255.0;
}

/**
Eindhoven University of Technology color-cyle.

\returns RGB data.
*/
inline xt::xtensor<double, 2> tue()
{
    xt::xtensor<double, 2> data = {
        {247,  49,  49}, //  0: warm red
        {214,   0,  74}, //  1: red
        {214,   0, 123}, //  2: pink
        {173,  32, 173}, //  3: dark pink
        { 16,  16, 115}, //  4: dark blue
        {  0, 102, 204}, //  5: blue
        {  0, 162, 222}, //  6: light blue
        {255, 154,   0}, //  7: orange
        {255, 221,   0}, //  8: yellow
        {206, 223,   0}, //  9: lemon
        {132, 210,   0}, // 10: lime
        {  0, 172, 130}, // 11: green
        {  0, 146, 181}, // 12: cornflower blue
    };

    return data / 255.0;
}

/**
Inverse of cppcolormap::xterm.

\returns RGB data.
*/
inline xt::xtensor<double, 2> xterm_r()
{
    return xt::flip(xterm(), 0);
}

/**
Inverse of cppcolormap::tue.

\returns RGB data.
*/
inline xt::xtensor<double, 2> tue_r()
{
    return xt::flip(tue(), 0);
}

/**
Get color-cycle specified as string.

\param cmap Name of the colormap.
\returns RGB data.
*/
inline xt::xtensor<double, 2> colorcycle(const std::string& cmap)
{
    if (cmap == "xterm") {
        return xterm();
    }

    if (cmap == "tue") {
        return tue();
    }

    if (cmap == "xterm_r") {
        return xterm_r();
    }

    if (cmap == "tue_r") {
        return tue_r();
    }

    throw std::runtime_error("Color-cycle not recognized");
}

/**
Algorithm to use for color matching.
*/
enum metric
{
    euclidean, ///< Euclidean norm
    fast_perceptual, ///< Fast best perception algorithm. See: https://stackoverflow.com/a/1847112/2646505
    perceptual ///< Best perception algorithm. See: https://en.wikipedia.org/wiki/Color_difference
};

namespace detail
{

    double euclidean_metric(double R1, double G1, double B1, double R2, double G2, double B2)
    {
        return std::pow(R1 - R2, 2.0) +
               std::pow(G1 - G2, 2.0) +
               std::pow(B1 - B2, 2.0);
    }

    // https://stackoverflow.com/a/1847112/2646505
    double fast_perceptual_metric(double R1, double G1, double B1, double R2, double G2, double B2)
    {
        return 0.3  * std::pow(R1 - R2, 2.0) +
               0.59 * std::pow(G1 - G2, 2.0) +
               0.11 * std::pow(B1 - B2, 2.0);
    }

    // https://en.wikipedia.org/wiki/Color_difference
    double perceptual_metric(double R1, double G1, double B1, double R2, double G2, double B2)
    {
        double r_ = (R1 + R2) / 2.0;
        double DR = (R1 - R2);
        double DG = (G1 - G2);
        double DB = (B1 - B2);

        return 2 * DR * DR + 4 * DG * DG + 3 * DB * DB + ((r_ * (DR * DR - DB * DB)));
    }

}

/**
Match colors.

\param A List of colors.
\param B List of colors.
\param distance_metric Metric to use in color matching.
\return For each item in ``A``, the index of the closets corresponding color in ``B``.
*/
inline xt::xtensor<size_t, 1> match(
    const xt::xtensor<double, 2>& A,
    const xt::xtensor<double, 2>& B,
    metric distance_metric = euclidean)
{
    xt::xtensor<size_t, 1> idx = xt::empty<size_t>({A.shape(0)});
    xt::xtensor<double, 1> d = xt::empty<double>({B.shape(0)});

    auto fmetric = detail::euclidean_metric;

    if (distance_metric == metric::fast_perceptual) {
        fmetric = detail::fast_perceptual_metric;
    }

    if (distance_metric == metric::perceptual) {
        fmetric = detail::perceptual_metric;
    }

    for (size_t i = 0; i < A.shape(0); ++i) {
        for (size_t j = 0; j < B.shape(0); ++j) {
            d(j) = fmetric(A(i, 0), A(i, 1), A(i, 2), B(j, 0), B(j, 1), B(j, 2));
        }
        idx(i) = xt::argmin(d)(0);
    }

    return idx;
}

} // namespace cppcolormap

#endif
