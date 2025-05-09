set title "Persentase Pengguna Internet (Data Asli + Regresi Linear)"
set xlabel "Tahun"
set ylabel "Persentase (%)"
set grid
set key outside

plot \
    "output_internet.dat" using 1:2 with points pointtype 7 lc rgb "blue" title "Data Internet Asli", \
    "output_internet.dat" using 1:3 with lines lw 2 title "Regresi Linear"
