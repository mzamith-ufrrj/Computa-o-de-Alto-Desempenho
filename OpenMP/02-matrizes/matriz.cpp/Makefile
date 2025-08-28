CPUCC       = g++
CPPFLAGS    = -O2 -fopenmp #-O3 #-fopenmp
DEFS        = -DALING=64
INCLUDES    = -I.
LIBDIR      =  #-L/opt/gnu-gcc/lib64
LIBS        = -lm
MKDIR_P     = mkdir -p
OUT_DIR     = obj

LINK        = $(LIBDIR) $(LIBS)  #-static #-static-libstdc++ -static-libgcc
COMPILECPU  = $(CPUCC) $(DEFS) $(INCLUDES) $(CPPFLAGS)

all: directories  diff-mat generate-mat-ABX multi-mat-cpu generate-mat show-mat trans-mat

solve-jacobi: rural-mat main-solver-jacobi
	$(COMPILECPU) ${OUT_DIR}/rural-mat.o ${OUT_DIR}/main-solver-jacobi.o $(LINK)  -o solver-jacobi.exec

trans-mat: rural-mat main-trans
	$(COMPILECPU) ${OUT_DIR}/rural-mat.o ${OUT_DIR}/main-trans.o $(LINK)  -o trans-mat.exec

diff-mat:  rural-mat main-diff-mat
	$(COMPILECPU) ${OUT_DIR}/rural-mat.o ${OUT_DIR}/main-diff-mat.o $(LINK)  -o diff-mat.exec

show-mat: rural-mat main-show
	$(COMPILECPU) ${OUT_DIR}/rural-mat.o ${OUT_DIR}/main-show.o $(LINK)  -o show-mat.exec

generate-mat-ABX:  rural-mat main-gener-A-B-X
	$(COMPILECPU) ${OUT_DIR}/rural-mat.o ${OUT_DIR}/main-gener-A-B-X.o $(LINK)  -o generate-mat-ABX.exec


generate-mat:  rural-mat main-gener
	$(COMPILECPU) ${OUT_DIR}/rural-mat.o ${OUT_DIR}/main-gener.o $(LINK)  -o generate-mat.exec

multi-mat-cpu:  rural-mat main-multi
	$(COMPILECPU) ${OUT_DIR}/rural-mat.o ${OUT_DIR}/main-multi.o $(LINK) -o multi-mat-cpu.exec

main-solver-jacobi:
	$(COMPILECPU) -c main-solver-jacobi.cpp -o ${OUT_DIR}/main-solver-jacobi.o

main-trans:
	$(COMPILECPU) -c main-trans.cpp -o ${OUT_DIR}/main-trans.o

main-diff-mat:
	$(COMPILECPU) -c main-diff-mat.cpp -o ${OUT_DIR}/main-diff-mat.o

main-show:
	$(COMPILECPU) -c main-show.cpp -o ${OUT_DIR}/main-show.o

main-gener:
	$(COMPILECPU) -c main-gener.cpp -o ${OUT_DIR}/main-gener.o

main-gener-A-B-X:
	$(COMPILECPU) -c main-gener-A-B-X.cpp -o ${OUT_DIR}/main-gener-A-B-X.o

main-multi:
	$(COMPILECPU) -c main-multi.cpp -o ${OUT_DIR}/main-multi.o

rural-mat:
	$(COMPILECPU) -c rural-mat.cpp -o ${OUT_DIR}/rural-mat.o

directories:
	${MKDIR_P} ${OUT_DIR}

clean:
	rm ${OUT_DIR}/*.o; rm *.exec
