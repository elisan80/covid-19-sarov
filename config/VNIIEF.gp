set boxwidth 1.0 absolute  
set key outside center top vertical Right reverse noenhanced 
set key right top vertical 
set style increment default
set style data lines
set xlabel 'Порядковый номер дня эпидемии'
set ylabel 'Количество человек'
set grid
set title 'Расчет для ВНИИЭФ'
plot 'output.txt' using 1:2 title 'Количество не зараженных'lw 2 lt rgb 'blue', '' using 1:3 title 'Количество контактных' lw 2 lt rgb 'purple','' using 1:4 title 'Количество зараженных'lw 2 lt rgb 'red','' using  1:5 title 'Выздоровевшие' lw 2 lt rgb 'green','' using 1:6 title 'Количество умерших' lw 2 lt rgb 'black'