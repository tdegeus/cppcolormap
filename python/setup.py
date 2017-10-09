desc = '''
cppcolormap is a library that contains many colormaps from different sources:

* colorbrewer2.org
* tue.nl
'''

from setuptools import setup, Extension

import sys
import setuptools
import pybind11
import cppmat

__version__ = '0.1.5'

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
