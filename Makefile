CC = gcc
CXX = g++
CFLAGS := -Wall -O3 -pipe -m64 #-pg
CXXFLAGS := $(CFLAGS)
LOBJS = ssw.o
LCPPOBJS = ssw_cpp.o
PROG = ssw_test
LIB = libssw.so
EXAMPLE = example_c
EXAMPLE_CPP = example_cpp
HALLAM_TEST = hallam_SSW_test

.PHONY: all clean

all: $(PROG) $(EXAMPLE) $(EXAMPLE_CPP) $(HALLAM_TEST) $(LIB)

$(LIB): ssw.c ssw.h
	$(CC) $(CFLAGS) -fPIC -shared -rdynamic -o $@ $<

$(PROG): main.c kseq.h

$(EXAMPLE): example.c

$(HALLAM_TEST): hallamSSWTest.cpp $(LOBJS) $(LCPPOBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) -lm -lz

$(PROG) $(EXAMPLE): $(LOBJS)
	$(CC) -o $@ $(filter-out %.h,$^) $(CFLAGS) -lm -lz

$(EXAMPLE_CPP): example.cpp $(LOBJS) $(LCPPOBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) -lm -lz

ssw.o: ssw.c ssw.h
	$(CC) -c -o $@ $< $(CFLAGS)

ssw_cpp.o: ssw_cpp.cpp ssw_cpp.h ssw.h
	$(CXX) -c -o $@ $< $(CXXFLAGS)

clean:
	-rm -f $(LOBJS) $(LCPPOBJS) $(PROG) $(LIB) $(HALLAM_TEST) $(EXAMPLE) $(EXAMPLE_CPP) *~
