
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import cppcolormap as cm

# Colormap

cmaps = {
    'Qualitative' : (
        "Accent",
        "Dark2",
        "Paired",
        "Spectral",
        "Pastel1",
        "Pastel2",
        "Set1",
        "Set2",
        "Set3"),
    'Sequential' : (
        "Blues",
        "Greens",
        "Greys",
        "Oranges",
        "Purples",
        "Reds",
        "BuPu",
        "GnBu",
        "PuBu",
        "PuBuGn",
        "PuRd",
        "RdPu",
        "OrRd",
        "RdOrYl",
        "YlGn",
        "YlGnBu",
        "YlOrRd"),
    'Diverging' : (
        "BrBG",
        "PuOr",
        "RdBu",
        "RdGy",
        "RdYlBu",
        "RdYlGn",
        "PiYG",
        "PRGn"),
    'matplotlib' : (
        "magma",
        "inferno",
        "plasma",
        "viridis",
        "jet"),
    'monocolor' : (
        "White",
        "Grey",
        "Black",
        "Red",
        "Blue",
        "tuewarmred",
        "tuedarkblue",
        "tueblue",
        "tuelightblue")}

nrows = max(len(cmap_list) for cmap_category, cmap_list in cmaps.items())
gradient = np.linspace(0, 1, 256)
gradient = np.vstack((gradient, gradient))

def plot_color_gradients(cmap_category, cmap_list, nrows):

    fig, axes = plt.subplots(nrows=nrows)
    fig.subplots_adjust(top=0.95, bottom=0.01, left=0.2, right=0.99)
    axes[0].set_title(cmap_category + ' colormaps', fontsize=14)

    for ax, name in zip(axes, cmap_list):
        c = cm.colormap(name)
        c = mpl.colors.ListedColormap(c, name=name, N=c.shape[0])
        ax.imshow(gradient, aspect='auto', cmap=c)
        pos = list(ax.get_position().bounds)
        x_text = pos[0] - 0.01
        y_text = pos[1] + pos[3]/2.
        fig.text(x_text, y_text, name, va='center', ha='right', fontsize=10)

    for ax in axes:
        ax.set_axis_off()

    plt.savefig(cmap_category + '.png')
    plt.close()

for cmap_category, cmap_list in cmaps.items():
    plot_color_gradients(cmap_category, cmap_list, nrows)

# Colorcyles

cmaps = (
    "xterm",
    "tue")

def plot_color_gradients(cmap_category, cmap_list, nrows):

    fig, axes = plt.subplots(nrows=nrows)
    fig.subplots_adjust(top=0.95, bottom=0.01, left=0.2, right=0.99)
    axes[0].set_title(cmap_category + ' colormaps', fontsize=14)

    for ax, name in zip(axes, cmap_list):
        c = cm.colorcycle(name)
        N = c.shape[0]
        c = mpl.colors.ListedColormap(c, name=name, N=N)
        gradient = np.linspace(0, 1, N)
        gradient = np.vstack((gradient, gradient))
        ax.imshow(gradient, aspect='auto', cmap=c)
        pos = list(ax.get_position().bounds)
        x_text = pos[0] - 0.01
        y_text = pos[1] + pos[3]/2.
        fig.text(x_text, y_text, name, va='center', ha='right', fontsize=10)

    for ax in axes:
        ax.set_axis_off()

    plt.savefig(cmap_category + '.png')
    plt.close()

plot_color_gradients('Colorcycles', cmaps, nrows)

