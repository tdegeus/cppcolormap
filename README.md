# cppcolormap

Library with colormaps for C++. Quick start: `#include <cppcolormap.h>`, that's about it. Note that the library depends on [xtensor](http://xtensor.readthedocs.io). Its installation and use are equally straightforward.

>   **Disclaimer**
>   
>   This library is free to use under the [GPLv3 license](https://github.com/tdegeus/cppcolormap/blob/master/LICENSE). Any additions are very much appreciated, in terms of suggested functionality, code, documentation, testimonials, word of mouth advertisement, .... Bug reports or feature requests can be filed on [GitHub](https://github.com/tdegeus/cppcolormap). As always, the code comes with no guarantee. None of the developers can be held responsible for possible mistakes.
>   
>   Download: [.zip file](https://github.com/tdegeus/cppcolormap/zipball/master) | [.tar.gz file](https://github.com/tdegeus/cppcolormap/tarball/master).
>   
>   (c - [GPLv3](https://github.com/tdegeus/cppcolormap/blob/master/LICENSE)) T.W.J. de Geus (Tom) | tom@geus.me | www.geus.me | [github.com/tdegeus/cppcolormap](https://github.com/tdegeus/cppcolormap)
>   
>   Acknowledgement:
>   
>   *   Wolf Vollprecht

# Contents

<!-- MarkdownTOC -->

- [Usage from C++](#usage-from-c)
    - [Installation](#installation)
    - [Usage](#usage)
- [Usage from Python](#usage-from-python)
    - [Installation](#installation-1)
    - [Usage](#usage-1)
    - [Find match](#find-match)
- [Available colormaps](#available-colormaps)
    - [ColorBrewer](#colorbrewer)
- [Matplotlib](#matplotlib)
- [Monochromatic colormaps](#monochromatic-colormaps)
- [Available color-cycles](#available-color-cycles)
    - [Xterm](#xterm)
    - [Eindhoven University of Technology](#eindhoven-university-of-technology)

<!-- /MarkdownTOC -->

# Usage from C++

## Installation

The library is header only. This means that one has to only include the header-file `cppcolormap.h`. Really, that's it! 

To be able to set the include path semi-automatic, one can choose to 'install' cppcolormap. To do this using CMake:

1.  Proceed to a (temporary) build directory. For example

    ```bash
    $ cd /path/to/cppcolormap/build
    ```

2.  'Build' cppcolormap

    ```bash
    $ cmake ..
    $ make install
    ```

## Usage

The main interface is with two functions:

```cpp
#include <cppcolormap.h>

int main ()
{
    std::cout << cppcolormap::colormap("Reds")  << std::endl;
    std::cout << cppcolormap::colorcycle("tue") << std::endl;

    return 0;
}
```

Lists of [colormaps](#available-colormaps) and [color-cycles](#available-color-cycles) can be found below.

The colormaps are stored as a matrix whereby each row contains the (R,G,B) colors. Each color value has a range `[0..1]`. The number of colors varies from map to map, but can be interpolated by specifying the number of colors you want:

```cpp
#include <cppcolormap.h>

int main ()
{
    std::cout << cppcolormap::colormap("Reds", 256)  << std::endl;

    return 0;
}
```

Note that the colorcycles are not interpolatable. Consequently the functions do have a size option. Note also that the colormaps can also be called directly, e.g.

```cpp
#include <cppcolormap.h>

int main ()
{
    std::cout << cppcolormap::Reds() << std::endl;
    std::cout << cppcolormap::tue()  << std::endl;

    return 0;
}
```

# Usage from Python

## Installation

Clone the repository and then run:

```bash
# if you are using Python 2.x
python setup.py build
python setup.py install

# if you are using Python 3.x
python3 setup.py build
python3 setup.py install
```

## Usage

There are two functions, each returns a 2-d NumPy array:

```python
import cppcolormap as cmap

# number of colors in the colormap (optional, may be omitted)
N = 256

# specify the colormap as string
cols = cmap.colormap("Reds",N)
cols = cmap.colorcycle("tue",N)

# or call the functions directly
cols = cmap.Reds(N)
cols = cmap.tue(N)
```

(see lists of [colormaps](#available-colormaps) and [color-cycles](#available-color-cycles) below).

## Find match

To find the closest match of each color of a colormap in another colormap you can use:

```cpp
xt::xtensor<size_t,1> idx = cppcolormap::match(cmap1, cmap2);
xt::xtensor<size_t,1> idx = cppcolormap::match(cmap1, cmap2, cppcolormap::metric::euclidean);
```

As option the following metrics can be used:

*   euclidean
*   fast_perceptual
*   perceptual

# Available colormaps

> Note that each colormap can be flipped by appending the name by "_r"

## ColorBrewer

*   Accent
*   Dark2
*   Paired
*   Spectral
*   Pastel1
*   Pastel2
*   Set1
*   Set2
*   Set3
*   Blues
*   Greens
*   Greys
*   Oranges
*   Purples
*   Reds
*   BuPu
*   GnBu
*   PuBu
*   PuBuGn
*   PuRd
*   RdPu
*   OrRd
*   RdOrYl
*   YlGn
*   YlGnBu
*   YlOrRd
*   BrBG
*   PuOr
*   RdBu
*   RdGy
*   RdYlBu
*   RdYlGn
*   PiYG
*   PRGn

>   Copyright (c) 2002 Cynthia Brewer, Mark Harrower, and The Pennsylvania State University.
>   
>   Licensed under the Apache License, Version 2.0
>   
>   [colorbrewer2.org](http://colorbrewer2.org)

# Matplotlib

*   magma
*   inferno
*   plasma
*   viridis
*   jet

>   Copyright (c)  New matplotlib colormaps by Nathaniel J. Smith, Stefan van der Walt, and 
>   in the case of viridis) Eric Firing.
>   
>   Licensed under the under the CC0 license / public domain dedication.
>   
>   [GitHub](https://github.com/BIDS/colormap)

# Monochromatic colormaps

*   White
*   Grey
*   Black
*   Red
*   Blue

# Available color-cycles

## Xterm

See [this site](https://jonasjacek.github.io/colors/)

*   xterm

## Eindhoven University of Technology

The following colors and color-cycles are based on the corporate color scheme of the [Eindhoven University of Technology](http://www.tue.nl).

*   tue
*   tuedarkblue
*   tueblue
*   tuelightblue
*   tuewarmred
