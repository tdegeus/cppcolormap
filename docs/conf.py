import os
import subprocess
import sys

project = "cppcolormap"
copyright = "2021-2023, Tom de Geus"
author = "Tom de Geus"

subprocess.call("cd ..; python setup.py build --build-type Release -vv", shell=True)
mybuild = os.listdir("../_skbuild")[0]
sys.path.insert(0, os.path.abspath(f"../_skbuild/{mybuild}/cmake-install/python"))

doxydir = "_doxygen"

if not os.path.isdir(doxydir):
    os.mkdir(doxydir)

subprocess.call(f"cmake .. -B{doxydir:s} -DBUILD_DOCS=1", shell=True)
subprocess.call(f"cd {doxydir:s}; make html", shell=True)

extensions = [
    "breathe",
    "sphinx.ext.autodoc",
    "sphinx.ext.autosummary",
    "sphinx_mdinclude",
]

exclude_patterns = ["_build", "Thumbs.db", ".DS_Store"]

html_theme = "furo"

breathe_projects = {
    project: f"{doxydir:s}/xml/",
}
