set terminal pngcairo size 800,600
set output 'grafico.png'
set title "Comparación de Tiempos de Ejecución"
set xlabel "Archivo"
set ylabel "Tiempo (ms)"
set grid
set key left top
set style data linespoints
set datafile separator ','

# Configura el formato del eje x para que use los nombres de los archivos
set xtics rotate by -45
set xtics border in
set format x "%s"  # Usa formato de texto simple para los nombres de archivos

# Graficar
plot 'resultados.txt' using 0:2 with linespoints title 'lcCycles', \
     '' using 0:3 with linespoints title 'evalJohnson'

