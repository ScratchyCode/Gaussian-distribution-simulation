# Coded by ScratchyCode
#!/bin/bash
DIR=$(dirname "$0")
cd $DIR
gcc Gauss.c -lm -o gauss.out
./gauss.out
gnuplot script.gnuplot -p
sleep 1
rm gauss.dat
rm gauss.out
exit
