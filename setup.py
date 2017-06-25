from setuptools import setup, Extension

import sys
import setuptools
import pybind11
import cppmat

__version__ = '0.1.3'

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
  name='cppcolormap',
  version=__version__,
  author='Tom de Geus',
  author_email='tom@geus.me',
  url='https://github.com/tdegeus/cppcolormap',
  description='Library with colormaps',
  long_description='',
  license='MIT',
  ext_modules=ext_modules,
  install_requires=['pybind11>=2.1.0','cppmat>=0.1.7'],
  cmdclass={'build_ext': cppmat.BuildExt},
  zip_safe=False,
)
