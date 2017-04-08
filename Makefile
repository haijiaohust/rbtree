objects = rbtree.o rbtest.o

rbtest:$(objects)
	cc -o rbtest $(objects)

.PHONY:clean
clean:
	rm -rf *.o rbtest