gcc -g -c -O0 -I/home/victor/Codes/levmar *.c
gcc -g -O0 -L/home/victor/Codes/levmar -L/home/victor/Codes/CLAPACK *.o -llevmar -llapack -lf2c -lblas -lm