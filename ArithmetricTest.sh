for M in 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32
do
	echo "----------------------"
	echo " M=$M"
	echo "----------------------"
	##############################
	# Run ArithmetricCount.c
	##############################
	rm golArith.out 2> /dev/null
	
	# calculating  b = ceiling( log2(M) )
	if b=$(python3 b-from-M.py $M); then
		echo "Using b=$b"
	else
		echo "There was error in calculation of b. Exiting..."
		exit  2
	fi
	
	gcc -Wall -I /home/MSTest/gsl2.4/include/ -c -O3 -D PASSED_M=$M -D PASSED_b=$b  ArithmetricCount.c -lm -mlzcnt -o golArith.o
	gcc -L /home/MSTest/gsl2.4/lib golArith.o -lgsl -lgslcblas -O3 -lm -mlzcnt -o golArith.out
	./golArith.out
	
done
