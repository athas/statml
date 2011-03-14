CXX = gcc
CXX_ARGS = -Wall -g -std=c99 -fopenmp
DEPS=plot.c statml.c ppm.c common_case.c
LIBRARIES =`gsl-config --libs` gnuplot_i.c $(DEPS)
OUT_DIRS=tex_out
BASENAME=question
ALL_OUTS=$(BASENAME)1 $(BASENAME)3 $(BASENAME)9 $(BASENAME)10 $(BASENAME)11

default: all


run_all: $(ALL_OUTS)
	for number in 31 32 33 ; do \
		./$(BASENAME)$$number; \
	done;\
	gnuplot plot.gnu

NUM=31

$(BASENAME)$(NUM): $(BASENAME)$(NUM).c $(DEPS) out_dirs
	$(CXX) $(CXX_ARGS) -o $@ $(LIBRARIES) $<

$(BASENAME)$(NUM)_run: $(BASENAME)$(NUM)
	./$(BASENAME)$(NUM);\

NUM=32
$(BASENAME)$(NUM): $(BASENAME)$(NUM).c $(DEPS) out_dirs
	$(CXX) $(CXX_ARGS) -o $@ $(LIBRARIES) $<

$(BASENAME)$(NUM)_run: $(BASENAME)$(NUM)
	./$<;

NUM=33
$(BASENAME)$(NUM): $(BASENAME)$(NUM).c $(DEPS) out_dirs
	$(CXX) $(CXX_ARGS) -o $@ $(LIBRARIES) $<

$(BASENAME)$(NUM)_run: $(BASENAME)$(NUM)
	./$<;\

$(BASENAME)$(NUM)_run: $(BASENAME)$(NUM)
	./$<;\

out_dirs: $(OUT_DIRS)
	mkdir -p $(OUT_DIRS)

clean:
	rm $(ALL_OUTS)

all: $(ALL_OUTS) $(DEPS)
