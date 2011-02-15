CXX = gcc
CXX_ARGS = -Wall -g -std=c99 -fopenmp
LIBRARIES =`gsl-config --libs` gnuplot_i.o plot.c
DEPS=plot.c
BASENAME=mycase

NUM=1

$(BASENAME)$(NUM): $(BASENAME)$(NUM).c $(DEPS)
	$(CXX) $(CXX_ARGS) -o $@ $(LIBRARIES) $<;./$@

NUM=2
$(BASENAME)$(NUM): $(BASENAME)$(NUM).c
	$(CXX) $(CXX_ARGS) -o $@ $(LIBRARIES) $<;./$@
