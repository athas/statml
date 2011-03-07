CXX = gcc
CXX_ARGS = -Wall -g -std=c99 -fopenmp
DEPS=plot.c statml/statml.c ppm.c common_case.c
LIBRARIES =`gsl-config --libs` gnuplot_i.c $(DEPS)
OUT_DIRS=tex_out img_out
BASENAME=mycase
ALL_OUTS=$(BASENAME)1 $(BASENAME)3 $(BASENAME)9 $(BASENAME)10


default: all

NUM=1

$(BASENAME)$(NUM): $(BASENAME)$(NUM).c $(DEPS) out_dirs
	$(CXX) $(CXX_ARGS) -o $@ $(LIBRARIES) $<

$(BASENAME)$(NUM)_run: $(BASENAME)$(NUM)
	./$(BASENAME)$(NUM);\

NUM=3
$(BASENAME)$(NUM): $(BASENAME)$(NUM).c $(DEPS) out_dirs
	$(CXX) $(CXX_ARGS) -o $@ $(LIBRARIES) $<

$(BASENAME)$(NUM)_run: $(BASENAME)$(NUM)
	./$(BASENAME)$(NUM);\

NUM=9
$(BASENAME)$(NUM): $(BASENAME)$(NUM).c $(DEPS) out_dirs
	$(CXX) $(CXX_ARGS) -o $@ $(LIBRARIES) $<

$(BASENAME)$(NUM)_run: $(BASENAME)$(NUM)
	./$(BASENAME)$(NUM);\
	cd img_out; bash ./foreach.sh pnm './conv.sh'; cd -

NUM=10
$(BASENAME)$(NUM): $(BASENAME)$(NUM).c $(DEPS) out_dirs
	$(CXX) $(CXX_ARGS) -o $@ $(LIBRARIES) $<

$(BASENAME)$(NUM)_run: $(BASENAME)$(NUM)
	./$(BASENAME)$(NUM);\
	cd img_out; bash ./foreach.sh pnm './conv.sh'; cd -


out_dirs: $(OUT_DIRS)
	mkdir -p $(OUT_DIRS)

clean:
	rm $(ALL_OUTS)

all: $(ALL_OUTS) $(DEPS)
