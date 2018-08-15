desc = '''
GooseFEM is a C++ module, wrapped in Python, that provides several predefined finite element meshes.
The original C++ module also includes element definitions and several standard finite element
simulations.
'''

from setuptools import setup, Extension

import sys, re
import setuptools
import pybind11
import pyxtensor

header = open('./include/cppcolormap.h','r').read()
world  = re.split(r'(.*)(\#define CPPCOLORMAP_WORLD_VERSION\ )([0-9]+)(.*)',header)[3]
major  = re.split(r'(.*)(\#define CPPCOLORMAP_MAJOR_VERSION\ )([0-9]+)(.*)',header)[3]
minor  = re.split(r'(.*)(\#define CPPCOLORMAP_MINOR_VERSION\ )([0-9]+)(.*)',header)[3]

__version__ = '.'.join([world,major,minor])

ext_modules = [
  Extension(
    'cppcolormap',
    ['include/python.cpp'],
    include_dirs=[
      pybind11 .get_include(False),
      pybind11 .get_include(True ),
      pyxtensor.get_include(False),
      pyxtensor.get_include(True ),
    ],
    language='c++'
  ),
]

setup(
  name             = 'cppcolormap',
  description      = 'Library with colormaps',
  long_description = desc,
  keywords           = 'colormap, plot, matplotlib',
  version          = __version__,
  license          = 'MIT',
  author           = 'Tom de Geus',
  author_email     = 'tom@geus.me',
  url              = 'https://github.com/tdegeus/cppcolormap',
  ext_modules      = ext_modules,
  install_requires = ['pybind11>=2.2.0','pyxtensor>=0.0.1'],
  cmdclass         = {'build_ext': pyxtensor.BuildExt},
  zip_safe         = False,
)
