
from setuptools import setup, Extension

import re
import pybind11
import pyxtensor

header = open('./include/cppcolormap.h', 'r').read()
major = re.split(r'(.*)(\#define CPPCOLORMAP_VERSION_MAJOR\ )([0-9]+)(.*)', header)[3]
minor = re.split(r'(.*)(\#define CPPCOLORMAP_VERSION_MINOR\ )([0-9]+)(.*)', header)[3]
patch = re.split(r'(.*)(\#define CPPCOLORMAP_VERSION_PATCH\ )([0-9]+)(.*)', header)[3]

__version__ = '.'.join([major, minor, patch])

ext_modules = [
  Extension(
    'cppcolormap',
    ['python/main.cpp'],
    include_dirs=[
      pybind11.get_include(False),
      pybind11.get_include(True),
      pyxtensor.get_include(False),
      pyxtensor.get_include(True),
    ],
    language='c++'
  ),
]

setup(
  name = 'cppcolormap',
  description = 'Library with colormaps',
  long_description = 'Library with colormaps',
  keywords = 'colormap, plot, matplotlib',
  version = __version__,
  license = 'MIT',
  author = 'Tom de Geus',
  author_email = 'tom@geus.me',
  url = 'https://github.com/tdegeus/cppcolormap',
  ext_modules = ext_modules,
  install_requires = ['pybind11>=2.2.0', 'pyxtensor>=0.0.1'],
  cmdclass = {'build_ext': pyxtensor.BuildExt},
  zip_safe = False,
)
