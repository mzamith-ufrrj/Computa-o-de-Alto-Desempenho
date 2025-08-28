EXEFILE   = m_mult.exec
CPUCC     = gcc
CPPFLAGS  =  -O2 #-fopenmp
DEFS      =
INCLUDES  =
LIBDIR    =
LIBS     =  -lm
LINK     =  $(LIBDIR) $(LIBS)

CPU_COMPILE = $(CPUCC) $(DEFS) $(INCLUDES) $(CPPFLAGS)


all: fdm

	$(CPU_COMPILE)	main-multi.o $(LINK) -o $(EXEFILE) $(PAPILIBS)

fdm:
	$(CPU_COMPILE) -c main-multi.c

clean:
	rm *.o $(EXEFILE)
