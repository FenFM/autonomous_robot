set xlabel "X"
set ylabel "Y"
set border
set key
set title "Path of Khepera Robot"
plot [0:1000][0:1000] 'STATS/path.dat'
