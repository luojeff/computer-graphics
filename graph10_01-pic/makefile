all: gp
	./gp

gp: genpic.o
	gcc $< -o $@

genpic.o: genpic.c
	gcc -c $<

clean:
	rm *.o
