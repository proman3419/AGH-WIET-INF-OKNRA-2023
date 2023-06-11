CC         := gcc
LINKER     := $(CC)
CFLAGS     := -O2 -Wall -msse3 -march=haswell
LDFLAGS    := -lm

run:
	$(CC) $(CFLAGS) $(FILE)
	./a.out

run_all:
	./run_all.sh

plot_all:
	./output_to_csv.sh
	./plot_all.py

do_all:
	make run_all
	make plot_all

clean:
	rm -f *.o *~ core *.x

cleanall:
	rm -f *.o *~ core *.x output*.m *.eps *.png
