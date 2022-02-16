import os
import subprocess

# Project information

project = "cppcolormap"
copyright = "2021, Tom de Geus"
author = "Tom de Geus"

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
