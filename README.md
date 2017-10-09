# cppcolormap

Library with colormaps for C++. Quick-start: `#include <cppcolormap.h>`, that's about it. Note that the library depends on [cppmat](http://cppmat.geus.me). Its installation and use is equally straightforward.

>   **Disclaimer**
>   
>   This library is free to use under the [GPLv3 license](https://github.com/tdegeus/cppcolormap/blob/master/LICENSE). Any additions are very much appreciated, in terms of suggested functionality, code, documentation, testimonials, word of mouth advertisement, .... Bug reports or feature requests can be filed on [GitHub](https://github.com/tdegeus/cppcolormap). As always, the code comes with no guarantee. None of the developers can be held responsible for possible mistakes.
>   
>   Download: [.zip file](https://github.com/tdegeus/cppcolormap/zipball/master) | [.tar.gz file](https://github.com/tdegeus/cppcolormap/tarball/master).
>   
>   (c - [GPLv3](https://github.com/tdegeus/cppcolormap/blob/master/LICENSE)) T.W.J. de Geus (Tom) | tom@geus.me | www.geus.me | [github.com/tdegeus/cppcolormap](https://github.com/tdegeus/cppcolormap)

# Contents

<!-- MarkdownTOC -->

- [Usage](#usage)
    - [From C++](#from-c)
    - [From Python](#from-python)
- [Available colormaps](#available-colormaps)
    - [ColorBrewer](#colorbrewer)
- [Available color-cycles](#available-color-cycles)
    - [Eindhoven University of Technology](#eindhoven-university-of-technology)

<!-- /MarkdownTOC -->

# Usage

## From C++

### Installation

The library is header-only. This means that one has to only include the header-file `cppcolormap.h`. Really, that's it! 

To be able to set the include path semi-automatic, one can choose to 'install' cppcolormap. To do this using CMake:

1.  Proceed to a (temporary) build directory. For example

    ```bash
    $ cd /path/to/cppcolormap/src/build
    ```

2.  'Build' cppcolormap

    ```bash
    $ cmake ..
    $ make install
    ```

    (If you've used another build directory, change the first command to `$ cmake /path/to/cppcolormap/src`)

### Usage

The main interface is with two functions:

```cpp
#include <cppmat/matrix.h>
#include <cppcolormap.h>

int main ( void )
{
    cppmat::matrix<int> cmap;

    cmap = cppcolormap::colormap("Reds");
    cmap = cppcolormap::colorcycle("tue");

    cmap.printf("%4d");
}
```

(see lists of [colormaps](#available-colormaps) and [color-cycles](#available-color-cycles) below).

The colormaps are stored as a matrix whereby each row contains the (R,G,B) colors. Each color value has a range `[0..255]`.

## From Python

### Installation

Install using PyPI:

```bash
# if you are using Python 2.X
pip install ccpcolormap

# if you are using Python 3.X
pip3 install cppcolormap
```

Otherwise, clone the repository and then run:

```bash
# if you are using Python 2.X
python setup.py build
python setup.py install

# if you are using Python 3.X
python3 setup.py build
python3 setup.py install
```

### Usage

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

# Available colormaps

## ColorBrewer

The following colormaps are taken from "[ColorBrewer](http://colorbrewer2.org)":

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

# Available color-cycles

## Eindhoven University of Technology

The following colors and color-cycles are based on the corporate color scheme of the [Eindhoven University of Technology](http://www.tue.nl).

*   tue
*   tuedarkblue
*   tueblue
*   tuelightblue
*   tuewarmred
