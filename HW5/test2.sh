cp kernel2.cu kernel.cu
make
echo "====  kernel2 test  ====\n"
./mandelbrot -v 1 -i 1000 -g 1
./mandelbrot -v 2 -i 1000 -g 1
./mandelbrot -v 1 -i 10000 -g 1
./mandelbrot -v 2 -i 10000 -g 1
./mandelbrot -v 1 -i 100000 -g 1
./mandelbrot -v 2 -i 100000 -g 1
