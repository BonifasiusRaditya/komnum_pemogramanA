set title "Populasi Indonesia (Data Asli + Regresi Polinomial)"
set xlabel "Tahun"
set ylabel "Populasi (juta)"
set grid
set key outside

plot \
    "output_populasi.dat" using 1:2 with points pointtype 7 lc rgb "purple" ps 1.3 title "Data Populasi Asli", \
    "output_populasi.dat" using 1:3 with lines lw 3 lc rgb "forest-green" title "Regresi Polinomial (deg3)"
