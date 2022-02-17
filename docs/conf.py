import os
import subprocess
import sys

# Project information

project = "cppcolormap"
copyright = "2021-2022, Tom de Geus"
author = "Tom de Geus"

# Build Python module

subprocess.call("cd ..; python setup.py build --build-type Release -vv", shell=True)
mybuild = os.listdir("../_skbuild")[0]
sys.path.insert(0, os.path.abspath(f"../_skbuild/{mybuild}/cmake-install/python"))

# Run Doxygen

doxydir = "_doxygen"

if not os.path.isdir(doxydir):
    os.mkdir(doxydir)

subprocess.call(f"cmake .. -B{doxydir:s} -DBUILD_DOCS=1", shell=True)
subprocess.call(f"cd {doxydir:s}; make html", shell=True)

# General configuration

extensions = [
    "breathe",
    "sphinx.ext.autodoc",
    "sphinx.ext.autosummary",
]

exclude_patterns = ["_build", "Thumbs.db", ".DS_Store"]

# Options for HTML output

html_theme = "sphinx_rtd_theme"

# Breathe configuration

breathe_projects = {
    project: f"{doxydir:s}/xml/",
}
