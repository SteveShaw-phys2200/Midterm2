GRAPH = gnuplot
CC = gcc
CFLAGS = -Wall -O0 -g -std=c99
LFLAGS = -O0 -g
LIBS = -lgsl -lm -lgslcblas

all: white-dwarf.png white-dwarf-loop.png

white-dwarf: white-dwarf.o white-dwarf-eqs.o
	${CC} $(LFLAGS) -o $@ $^ $(LIBS)
white-dwarf-loop: white-dwarf-loop.o white-dwarf-eqs.o
	${CC} $(LFLAGS) -o $@ $^ $(LIBS)

res1: white-dwarf
	./white-dwarf > res1
res2: white-dwarf-loop
	./white-dwarf-loop > res2

white-dwarf.png: white-dwarf.gpl res1
	$(GRAPH) white-dwarf.gpl
white-dwarf-loop.png: white-dwarf-loop.gpl res2
	$(GRAPH) white-dwarf-loop.gpl

clean : 
	rm -f *~
	rm -f *.o
	rm -f res1 white-dwarf
	rm -f res2 white-dwarf-loop
veryclean : clean
	rm -f  white-dwarf.png
	rm -f  white-dwarf-loop.png
