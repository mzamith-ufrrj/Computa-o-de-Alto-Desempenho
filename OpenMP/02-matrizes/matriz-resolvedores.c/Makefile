CPUCC       = gcc
CPPFLAGS    = -O2 -fopenmp #-O3 #-fopenmp
DEFS        = -DALING=64
INCLUDES    = -I.
LIBDIR      =  #-L/opt/gnu-gcc/lib64
LIBS        = -lm


LINK        = $(LIBDIR) $(LIBS)
COMPILECPU  = $(CPUCC) $(DEFS) $(INCLUDES) $(CPPFLAGS)

all: solve-jacobi mul-matriz solve-gauss solve-lu

solve-jacobi:
	$(COMPILECPU) main-solver-jacobi.c $(LINK)  -o solver-jacobi.exec

solve-gauss:
	$(COMPILECPU) main-solver-gauss.c $(LINK)  -o solver-gauss.exec

solve-lu:
	$(COMPILECPU) main-solver-lu.c $(LINK)  -o solver-lu.exec

mul-matriz:
	$(COMPILECPU) main-mul-matriz.c $(LINK)  -o  multi-mat-cpu.exec



clean:
	rm ${OUT_DIR}/*.o; rm *.exec
