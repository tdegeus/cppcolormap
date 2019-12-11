convert -trim Colorcycles.png Colorcycles_trimmed.png
convert -trim Diverging.png Diverging_trimmed.png
convert -trim Qualitative.png Qualitative_trimmed.png
convert -trim Sequential.png Sequential_trimmed.png
convert -trim matplotlib.png matplotlib_trimmed.png
convert -trim monocolor.png monocolor_trimmed.png
convert -trim monocolor_dvips_1.png monocolor_dvips_1_trimmed.png
convert -trim monocolor_dvips_2.png monocolor_dvips_2_trimmed.png

mv Colorcycles_trimmed.png Colorcycles.png
mv Diverging_trimmed.png Diverging.png
mv Qualitative_trimmed.png Qualitative.png
mv Sequential_trimmed.png Sequential.png
mv matplotlib_trimmed.png matplotlib.png
mv monocolor_trimmed.png monocolor.png
mv monocolor_dvips_1_trimmed.png monocolor_dvips_1.png
mv monocolor_dvips_2_trimmed.png monocolor_dvips_2.png
