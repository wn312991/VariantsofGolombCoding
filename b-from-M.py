import sys
import math
if  len(sys.argv) != 2:
    print('I need only one argument. The argument list provided:', sys.argv)
    print('Exiting ....')
    exit(1)
    
M= int(sys.argv[1])
b=math.ceil(math.log2(M))

print(b)
