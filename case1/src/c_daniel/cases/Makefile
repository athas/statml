CXX = gcc
CXX_ARGS = -Wall -g -std=c99 -fopenmp
DEPS=plot.c statml/statml.c ppm.c common_case.c
LIBRARIES =`gsl-config --libs` gnuplot_i.c $(DEPS)
OUT_DIRS=tex_out img_out
BASENAME=mycase
ALL_OUTS=$(BASENAME)1 $(BASENAME)3 $(BASENAME)19


default: all

NUM=1

$(BASENAME)$(NUM): $(BASENAME)$(NUM).c $(DEPS) out_dirs
	$(CXX) $(CXX_ARGS) -o $@ $(LIBRARIES) $<;./$@

NUM=3
$(BASENAME)$(NUM): $(BASENAME)$(NUM).c $(DEPS) out_dirs
	$(CXX) $(CXX_ARGS) -o $@ $(LIBRARIES) $<;./$@

NUM=19
$(BASENAME)$(NUM): $(BASENAME)$(NUM).c $(DEPS) out_dirs
	$(CXX) $(CXX_ARGS) -o $@ $(LIBRARIES) $<;./$@; \
	cd img_out; ./foreach.sh pnm conv.sh

out_dirs: $(OUT_DIRS)
	mkdir -p $(OUT_DIRS)

clean:
	rm $(ALL_OUTS)

all: $(ALL_OUTS) $(DEPS)
