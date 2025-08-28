CPUCC       = gcc
CPPFLAGS    = -g  -fopenmp #-O3 #-fopenmp
DEFS        = -DALING=64
INCLUDES    = -I.
LIBDIR      =  #-L/opt/gnu-gcc/lib64
LIBS        = -lm #-lgomp

LINK        = $(LIBDIR) $(LIBS)
COMPILECPU  = $(CPUCC) $(DEFS) $(INCLUDES) $(CPPFLAGS)

#all: 00-main-parallel-for 01-main-parallel-section 02-main-parallel-reduction-max

00-main-hello:
	$(COMPILECPU) 00-main-hello.c $(LINK)  -o hello-world-p.exec

00-main-diretiva:
	$(COMPILECPU) 00-main-diretiva.c $(LINK)  -o diretiva-00.exec

01-main-diretiva:
	$(COMPILECPU) 01-main-diretiva.c $(LINK)  -o diretiva-01.exec

00-main-parallel-for:
	$(COMPILECPU) 00-main-parallel-for.c $(LINK)  -o parallel-for-omp-00.exec

01-main-parallel-section:
	$(COMPILECPU) 01-main-parallel-section.c $(LINK)  -o parallel-section-omp.exec

02-main-parallel-reduction-max:
	$(COMPILECPU) 02-main-parallel-reduction-max.cpp $(LINK)  -o parallel-reduction-max-omp.exec

03-main-parallel-reduction-sum:
	$(COMPILECPU) 03-main-parallel-reduction-sum.cpp $(LINK)  -o parallel-reduction-sum-omp.exec


clean:
	rm *.exec
