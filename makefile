#Batch 66
#Akshit Johry 2014A7PS077P
#Arpit Panda 2014A7PS114P


output: driver.o
	gcc -g -o stage1exe driver.o -lm
driver.o: driver.c
	gcc -c -g driver.c -lm
clean:
	rm *.o
	rm driver
