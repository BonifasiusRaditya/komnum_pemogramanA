set title "Titik Interpolasi Linear pada Tahun yang Hilang"
set xlabel "Tahun"
set ylabel "Nilai"
set grid
set key outside

plot \
    "output_interpolasi.dat" using 1:2 with points pointtype 7 lc rgb "orange" title "Interpolasi Populasi", \
    "output_interpolasi.dat" using 1:3 with points pointtype 7 lc rgb "green" title "Interpolasi Internet"
