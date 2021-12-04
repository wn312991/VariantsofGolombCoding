# Variants-of-Golomb-Coding
We present a variant of Golomb coding that uses a fixed-length coding to code the remainder part.The simulations show that compared with Golomb coding, the proposed coding has about 20% less addition, 40% less multiplication and 20% more bitwise operations in encoding, and 40% less addition, 10% less multiplication, 50% less bitwise and 20% more
branch operations in decoding.  
The input sequence follows the geometric distribution provided by the GNU scientific library. These programs are tested on the platform equipped with Intel(R) Core(TM) i7-7700K CPU @ 3.60GHz on Ubuntu 16.04 operating system with gsl 2.4，Gcc 7.4.0 and -O3 option.  

You can refer to the installation of gsl2.4 on ubuntu at https://blog.csdn.net/u012248802/article/details/77920084  
Notably, before running, you need to modify the corresponding location of the GSL Library in your machine in .sh files.

To test Golomb.c, run :   
chmod u+x TestGolomb.sh  
./TestGolomb.sh  

To test Ours.c, run :  
chmod u+x TestOurs.sh  
./TestOurs.sh  

Please refer to the Experimental results.pdf to see the results on my machine.
