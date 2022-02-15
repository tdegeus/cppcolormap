from setuptools_scm import get_version
from skbuild import setup

project_name = "cppcolormap"

setup(
    name=project_name,
    description="Library with colormaps",
    long_description="Library with colormaps",
    keywords="colormap, plot, matplotlib",
    version=get_version(),
    license="MIT",
    author="Tom de Geus",
    author_email="tom@geus.me",
    url=f"https://github.com/tdegeus/{project_name}",
    packages=[f"{project_name}"],
    package_dir={"": "python"},
    cmake_install_dir=f"python/{project_name}",
    cmake_minimum_required_version="3.13...3.21",
)
