#include <cppcolormap.h>

int main()
{
  if (!CPPCOLORMAP_VERSION(0,2,1))
    std::cout << "Problems!" << std::endl;

  auto cmap = cppcolormap::Reds();
  auto xterm = cppcolormap::xterm();
  auto idx = cppcolormap::match(cmap, xterm, cppcolormap::metric::perceptual);
  auto cmap_as_xterm = xt::view(xterm, xt::keep(idx), xt::all());

  return 0;
}
