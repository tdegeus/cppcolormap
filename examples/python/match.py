import cppcolormap as cm
import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np

cmap = cm.Reds()
m_cmap = mpl.colors.ListedColormap(cmap, name='Reds', N=cmap.shape[0])

xterm = cm.xterm()
idx = cm.match(cmap, xterm, cm.metric.perceptual)
m_xterm = mpl.colors.ListedColormap(xterm[idx,:], name='xterm', N=cmap.shape[0])

fig, axes = plt.subplots(figsize=(16,8), ncols=2)

x, y = np.meshgrid(
    np.linspace(0, 99, 100),
    np.linspace(0, 99, 100))

z = (x - 50)**2.0 + (y - 50)**2.0

im = axes[0].imshow(z, cmap=m_cmap, clim=(0, 5000))
im = axes[1].imshow(z, cmap=m_xterm, clim=(0, 5000))

plt.savefig('match.pdf')
plt.close()

