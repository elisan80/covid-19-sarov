set boxwidth 1.0 absolute  
#set key outside center top vertical Left reverse noenhanced 
set key left top vertical 
#autotitle columnhead nobox
set style increment default
set style data lines #with pointtype 3 lw 2
set xlabel '���������� ����� ��� ��������'
set ylabel '���������� �������'
set grid
set title '������ ��� ������'
plot 'output.txt' using 1:2 title '���������� �� ����������'lw 2 lt rgb 'blue', '' using 1:3 title '���������� ����������' lw 2 lt rgb 'purple','' using 1:4 title '���������� ����������'lw 2 lt rgb 'red','' using  1:5 title '�������������' lw 2 lt rgb 'green','' using 1:6 title '���������� �������' lw 2 lt rgb 'black'
#plot 'output.txt' using 2:xtic(1)title '���������� �� ����������' lt rgb 'blue', '' using 3 title '���������� ����������' lt rgb 'purple','' using 4 title '���������� ����������' lt rgb 'red','' using  5 title '�������������'lt rgb 'green','' using 6 title '���������� �������'lt rgb 'black'
