#include <cppcolormap.h>

int main()
{
    auto cmap = cppcolormap::Reds();
    auto xterm = cppcolormap::xterm();
    auto idx = cppcolormap::match(cmap, xterm, cppcolormap::metric::perceptual);
    auto cmap_as_xterm = xt::view(xterm, xt::keep(idx), xt::all());

    return 0;
}
