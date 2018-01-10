#!/usr/bin/gnuplot
bin_width = 0.01;
bin_number(x) = floor(x/bin_width)
rounded(x) = bin_width * (bin_number(x) + 0.5)
plot 'gauss.dat' using (rounded($1)):(1) smooth frequency with boxes
exit
