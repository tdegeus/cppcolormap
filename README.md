# cppcolormap

Library with colormaps for C++. 

<!-- MarkdownTOC -->

- [Usage](#usage)
    - [From C++](#from-c)
- [Available colormaps](#available-colormaps)
    - [ColorBrewer](#colorbrewer)
- [Available color-cycles](#available-color-cycles)
    - [Eindhoven University of Technology](#eindhoven-university-of-technology)

<!-- /MarkdownTOC -->

# Usage

## From C++

The library is header-only. This means that one has to only include the header-file `cppcolormap.h`. Really, that's it! 

### Storage format

```cpp
#include "cppcolormap.h"

int main ( void )
{

  std::vector<int> cmap = cppcolormap::Reds();
  size_t           N    = cmap.size()/3;

  for ( size_t i=0 ; i<N ; ++i )
    printf("%3d , %3d , %3d\n",cmap[i*3+0],cmap[i*3+1],cmap[i*3+2]);

  return 0;
}
```

From the example we directly observe that the colormaps are stored are a sequential list of (R,G,B) colors, whereby each color has a range `[0..255]`. Take note of the following:

*   The colormaps are returns as ``std::vector<int>`. 
*   The number of colors (i.e. rows) can be backed-out from the length of the vector as: `size_t N = cmap.size()/3;`
*   To get the colors (R,G,B) from row `i`, use: `cmap[i*3+0]` for Red, `cmap[i*3+1]` for Blue, and `cmap[i*3+2]` for Green.

### String based selection

One can also select the colormap using a string:

```cpp
std::vector<int> cmap = cppcolormap::colormap("Reds",256);
```

To select one of the color-cycles:

```cpp
std::vector<int> cmap = cppcolormap::colorcycle("tue");
```

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
