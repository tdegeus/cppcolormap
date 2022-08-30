import matplotlib.colors
import numpy as np
from numpy.typing import ArrayLike

from ._cppcolormap import *  # noqa: F401, F403


def asLinearSegmentedColormap(
    cmap: ArrayLike, name: str = "mycolormap"
) -> matplotlib.colors.LinearSegmentedColormap:
    """
    Convert matrix of colors to a linear segmented colormap.
    See `this answer <https://stackoverflow.com/a/31499673/2646505>`__ for more details.

    :param cmap: Matrix of colors [n, 3].
    :return: The LinearSegmentedColormap.
    """
    assert cmap.ndim == 2
    assert cmap.shape[1] == 3
    return matplotlib.colors.LinearSegmentedColormap.from_list(
        name, np.c_[cmap, np.ones(cmap.shape[0])]
    )
