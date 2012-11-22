CXX = g++
GPUCC = nvcc
CXXFLAGS =  -w -Wall -ansi
LINKFLAGS = -lm

gsampling: main.o util.o mcmc_span.o reduce_func.o reduce.o
	$(GPUCC) -o gsmapling main.o util.o mcmc_span.o reduce.o reduce_func.o 
main.o:main.cpp mcmc_span.h util.h
	$(CXX) $(CXXFLAGS)  -c main.cpp
util.o:util.cpp util.h reduce_func.h
	$(CXX) $(CXXFLAGS) -c util.cpp
reduce.o:reduce.cu
	$(GPUCC) reduce.cu
mcmc_span.o: mcmc_span.cpp mcmc_span.h util.h reduce_func.h
	$(CXX) $(CXXFLAGS) -c mcmc_span.cpp
reduce_func.o:reduce_func.h reduce_func.cpp
	$(CXX) $(CXXFLAGS) -c reduce_func.cpp
clean:
	rm -f gsampling *.o *~

