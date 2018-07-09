desc = '''
cppcolormap is a library that contains many colormaps from different sources:

* colorbrewer2.org
* tue.nl
'''

from setuptools import setup, Extension

import sys,re
import setuptools
import pybind11
import cppmat

header = open('../src/cppcolormap.h','r').read()
world  = re.split('(.*)(\#define CPPCOLORMAP_WORLD_VERSION\ )([0-9]+)(.*)',header)[3]
major  = re.split('(.*)(\#define CPPCOLORMAP_MAJOR_VERSION\ )([0-9]+)(.*)',header)[3]
minor  = re.split('(.*)(\#define CPPCOLORMAP_MINOR_VERSION\ )([0-9]+)(.*)',header)[3]

__version__ = '.'.join([world,major,minor])

ext_modules = [
  Extension(
    'cppcolormap',
    ['python_interface.cpp'],
    include_dirs=[
      pybind11.get_include(False),
      pybind11.get_include(True ),
      cppmat  .get_include(False),
      cppmat  .get_include(True )
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
  install_requires = ['pybind11>=2.2.0','cppmat>=0.2.14'],
  cmdclass         = {'build_ext': cppmat.BuildExt},
  zip_safe         = False,
)
