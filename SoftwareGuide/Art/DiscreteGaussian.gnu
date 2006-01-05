set samples 30
set xrange  [-2:2]
set term fig color
set output "gaussian.fig"
plot exp(-x*x), exp(-x*x) with boxes
quit


