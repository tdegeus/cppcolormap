# cppcolormap

[![Travis](https://travis-ci.org/tdegeus/cppcolormap.svg?branch=master)](https://travis-ci.org/tdegeus/cppcolormap)
[![Appveyor](https://ci.appveyor.com/api/projects/status/lmrkifr8q9vledv5?svg=true)](https://ci.appveyor.com/project/tdegeus/cppcolormap)

>   **Disclaimer**
>   
>   This library is free to use under the [GPLv3 license](https://github.com/tdegeus/cppcolormap/blob/master/LICENSE). Any additions are very much appreciated, in terms of suggested functionality, code, documentation, testimonials, word-of-mouth advertisement, etc. Bug reports or feature requests can be filed on [GitHub](https://github.com/tdegeus/cppcolormap). As always, the code comes with no guarantee. None of the developers can be held responsible for possible mistakes.
>   
>   Download: [.zip file](https://github.com/tdegeus/cppcolormap/zipball/master) | [.tar.gz file](https://github.com/tdegeus/cppcolormap/tarball/master).
>   
>   (c - [GPLv3](https://github.com/tdegeus/cppcolormap/blob/master/LICENSE)) T.W.J. de Geus (Tom) | tom@geus.me | www.geus.me | [github.com/tdegeus/cppcolormap](https://github.com/tdegeus/cppcolormap)
>   
>   **Acknowledgement**
>   
>   *   Wolf Vollprecht

# Contents

<!-- MarkdownTOC -->

- [Introduction](#introduction)
- [Usage from C++](#usage-from-c)
    - [Getting cppcolormap](#getting-cppcolormap)
        - [Using conda](#using-conda)
        - [From source](#from-source)
    - [Usage](#usage)
    - [Find match](#find-match)
    - [Compiling](#compiling)
        - [Using `CMakeLists.txt`](#using-cmakeliststxt)
- [Usage from Python](#usage-from-python)
    - [Getting cppcolormap](#getting-cppcolormap-1)
        - [Using conda](#using-conda-1)
        - [From source](#from-source-1)
    - [Usage](#usage-1)
    - [Find match](#find-match-1)
    - [Example](#example)
- [Available colormaps](#available-colormaps)
    - [ColorBrewer](#colorbrewer)
- [Matplotlib](#matplotlib)
- [Monochromatic colormaps](#monochromatic-colormaps)
- [Available color-cycles](#available-color-cycles)
    - [Xterm](#xterm)
    - [Eindhoven University of Technology](#eindhoven-university-of-technology)

<!-- /MarkdownTOC -->

# Introduction

Library with colormaps for C++/Python. 

# Usage from C++

## Getting cppcolormap

### Using conda

```bash
conda install -c conda-forge cppcolormap
```

### From source

```bash
# Download cppcolormap
git checkout https://github.com/tdegeus/cppcolormap.git
cd cppcolormap

# For CMake or pkg-config use
cmake .
make install
```

## Usage

The principle interface is with these two functions:

```cpp
#include <cppcolormap.h>

int main()
{
    std::cout << cppcolormap::colormap("Reds") << std::endl;
    std::cout << cppcolormap::colorcycle("tue") << std::endl;

    return 0;
}
```

Lists of [colormaps](#available-colormaps) and [color-cycles](#available-color-cycles) can be found below.

The colormaps are stored as a matrix whereby each row contains the (R,G,B) colors. Each color value has a range `[0..1]`. The number of colors varies from map to map, but can be interpolated by specifying the number of colors you want:

```cpp
#include <cppcolormap.h>

int main()
{
    std::cout << cppcolormap::colormap("Reds", 256) << std::endl;

    return 0;
}
```

Note that the colorcycles are not interpolatable. Consequently the functions do have a size option. Note also that the colormaps can also be called directly, e.g.

```cpp
#include <cppcolormap.h>

int main()
{
    std::cout << cppcolormap::Reds() << std::endl;
    std::cout << cppcolormap::Reds(256) << std::endl;
    std::cout << cppcolormap::tue() << std::endl;

    return 0;
}
```

## Find match

To find the closest match of each color of a colormap in another colormap you can use:

```cpp
xt::xtensor<size_t,1> idx = cppcolormap::match(cmap1, cmap2);
xt::xtensor<size_t,1> idx = cppcolormap::match(cmap1, cmap2, cppcolormap::metric::euclidean);
```

The following metrics can be used:

*   euclidean (default)
*   fast_perceptual
*   perceptual

## Compiling

### Using `CMakeLists.txt`

Using *cppcolormap* the `CMakeLists.txt` can be as follows

```cmake
cmake_minimum_required(VERSION 3.1)

project(example)

find_package(xtl REQUIRED)
find_package(xtensor REQUIRED)
find_package(cppcolormap REQUIRED)

add_executable(example example.cpp)

target_link_libraries(example
    PRIVATE
    xtensor
    cppcolormap)
```

[download "CMakeLists.txt"](./example/CMakeLists.txt)

Compilation can then proceed using 

```bash
cmake .
make
```

# Usage from Python

## Getting cppcolormap

### Using conda

```bash
conda install -c conda-forge cppcolormap
```

### From source

```bash
# Download cppcolormap
git checkout https://github.com/tdegeus/cppcolormap.git
cd cppcolormap

# Compile & build
python setup.py build
python setup.py install
```

## Usage

There are two principle functions, each returns a 2-d NumPy array:

```python
import cppcolormap as cm

# number of colors in the colormap (optional, may be omitted)
N = 256

# specify the colormap as string
cols = cm.colormap("Reds", N)
cols = cm.colorcycle("tue", N)

# or call the functions directly
cols = cm.Reds(N)
cols = cm.tue(N)
```

(see lists of [colormaps](#available-colormaps) and [color-cycles](#available-color-cycles) below).

## Find match

To find the closest match of each color of a colormap in another colormap you can use:

```cpp
idx = cm.match(cmap1, cmap2)
idx = cm.match(cmap1, cmap2, cm.metric.perceptual)
```

(See metrics above.)

## Example

```python
import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import cppcolormap as cm

x, y = np.meshgrid(
  np.linspace(0, 1, 100), 
  np.linspace(0, 1, 100))

d = np.sqrt(x**2 + y**2)

C = cm.Reds(256)
C = np.c_[C, np.ones(C.shape[0])]

cmap = matplotlib.colors.LinearSegmentedColormap.from_list('my_colormap', C)

fig, ax = plt.subplots()

cax = ax.imshow(d, cmap=cmap)

plt.show()
```

# Available colormaps

## ColorBrewer

| Name     | Inverse colormap |
|----------|------------------|
| Accent   | Accent_r         |
| Dark2    | Dark2_r          |
| Paired   | Paired_r         |
| Spectral | Spectral_r       |
| Pastel1  | Pastel1_r        |
| Pastel2  | Pastel2_r        |
| Set1     | Set1_r           |
| Set2     | Set2_r           |
| Set3     | Set3_r           |
| Blues    | Blues_r          |
| Greens   | Greens_r         |
| Greys    | Greys_r          |
| Oranges  | Oranges_r        |
| Purples  | Purples_r        |
| Reds     | Reds_r           |
| BuPu     | BuPu_r           |
| GnBu     | GnBu_r           |
| PuBu     | PuBu_r           |
| PuBuGn   | PuBuGn_r         |
| PuRd     | PuRd_r           |
| RdPu     | RdPu_r           |
| OrRd     | OrRd_r           |
| RdOrYl   | RdOrYl_r         |
| YlGn     | YlGn_r           |
| YlGnBu   | YlGnBu_r         |
| YlOrRd   | YlOrRd_r         |
| BrBG     | BrBG_r           |
| PuOr     | PuOr_r           |
| RdBu     | RdBu_r           |
| RdGy     | RdGy_r           |
| RdYlBu   | RdYlBu_r         |
| RdYlGn   | RdYlGn_r         |
| PiYG     | PiYG_r           |
| PRGn     | PRGn_r           |

>   Copyright (c) 2002 Cynthia Brewer, Mark Harrower, and The Pennsylvania State University.
>   
>   Licensed under the Apache License, Version 2.0
>   
>   [colorbrewer2.org](http://colorbrewer2.org)

# Matplotlib

| Name     | Inverse colormap |
|----------|------------------|
| magma    | magma_r          |
| inferno  | inferno_r        |
| plasma   | plasma_r         |
| viridis  | viridis_r        |
| jet      | jet_r            |

>   Copyright (c)  New matplotlib colormaps by Nathaniel J. Smith, Stefan van der Walt, and 
>   in the case of viridis) Eric Firing.
>   
>   Licensed under the under the CC0 license / public domain dedication.
>   
>   [GitHub](https://github.com/BIDS/colormap)

# Monochromatic colormaps

| Name     | Inverse colormap |
|----------|------------------|
| White    | -                |    
| Grey     | -                |   
| Black    | -                |    
| Red      | -                |  
| Blue     | -                |   

# Available color-cycles

## Xterm

| Name     | Inverse colormap |
|----------|------------------|
| xterm    | xterm_r          |

>   See [this site](https://jonasjacek.github.io/colors/)

## Eindhoven University of Technology

| Name         | Inverse colormap |
|--------------|------------------|
| tue          | tue_r            |
| tuedarkblue  | -                |           
| tueblue      | -                |       
| tuelightblue | -                |            
| tuewarmred   | -                |          

>   Based on the corporate color scheme of the 
>   [Eindhoven University of Technology](http://www.tue.nl).
