set boxwidth 1.0 absolute  
set key left top vertical 
set style increment default
set style data lines
set xlabel '���������� ����� ��� ��������'
set ylabel '���������� �������'
set grid
set title '������ ��� ������'
plot 'output.txt' using 1:2 title '���������� �� ����������'lw 2 lt rgb 'blue', '' using 1:3 title '���������� ����������' lw 2 lt rgb 'purple','' using 1:4 title '���������� ����������'lw 2 lt rgb 'red','' using  1:5 title '�������������' lw 2 lt rgb 'green','' using 1:6 title '���������� �������' lw 2 lt rgb 'black'