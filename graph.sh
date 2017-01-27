#!/bin/bash


gnuplot <<-EOF
set xlabel "Time (ms)"
set ylabel "Acc"
set title "Acceleration en fonction du temps"
set term png
set output "acc.png"
plot "save" using 1:2 with line
EOF

gnuplot <<-EOF
set xlabel "Time (ms)"
set ylabel "Vel"
set title "Vélocité en fonction du temps"
set term png
set output "vel.png"
plot "save" using 1:3 with line
EOF

gnuplot <<-EOF
set xlabel "Time (ms)"
set ylabel "Pos"
set title "Position en fonction du temps"
set term png
set output "pos.png"
plot "save" using 1:4 with line
EOF

gpicview acc.png &
gpicview vel.png &
gpicview pos.png &
