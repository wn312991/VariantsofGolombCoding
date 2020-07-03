# Variants-of-Golomb-Coding
We presents a variant of Golomb coding that uses a fixed-length coding to code the remainder part.The simulations show that the proposed coding has up to 49.66% and 40.02% higher throughput than Golomb coding in encoding and decoding, respectively.  
The input sequence follows the geometric distribution provided by the GNU scientific library. These programs are tested on the platform equipped with Intel(R) Core(TM) i7-7700K CPU @ 3.60GHz on Ubuntu 16.04 operating system with gsl 2.4，Gcc 7.4.0 and -O3 option.  

You can refer to the installation of gsl2.4 on ubuntu at https://blog.csdn.net/u012248802/article/details/77920084  
Notably, before running, you need to modify the corresponding location of the GSL Library in your machine in .sh files.

To test the average number of arithmetic operations in both coding, run :  
chmod u+x ArithmetricTest.sh  
./ArithmetricTest.sh  

To test Golomb.c, run :   
chmod u+x TestGolomb.sh  
./TestGolomb.sh  

To test Ours.c, run :  
chmod u+x TestOurs.sh  
./TestOurs.sh  

Please refer to the Experimental results.pdf to see the results on my machine.
