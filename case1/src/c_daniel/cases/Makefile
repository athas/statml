CXX = gcc
CXX_ARGS = -Wall -g -std=c99 -fopenmp
DEPS=plot.c statml/statml.c ppm.c common_case.c
LIBRARIES =`gsl-config --libs` gnuplot_i.c $(DEPS)
BASENAME=mycase
ALL_OUTS=$(BASENAME)1 $(BASENAME)3 $(BASENAME)19

default: all

NUM=1

$(BASENAME)$(NUM): $(BASENAME)$(NUM).c $(DEPS)
	$(CXX) $(CXX_ARGS) -o $@ $(LIBRARIES) $<;./$@

NUM=3
$(BASENAME)$(NUM): $(BASENAME)$(NUM).c $(DEPS)
	$(CXX) $(CXX_ARGS) -o $@ $(LIBRARIES) $<;./$@

NUM=19
$(BASENAME)$(NUM): $(BASENAME)$(NUM).c $(DEPS)
	$(CXX) $(CXX_ARGS) -o $@ $(LIBRARIES) $<;./$@


clean:
	rm $(ALL_OUTS)

all: $(ALL_OUTS) $(DEPS)
