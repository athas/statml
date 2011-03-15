CXX = gcc
CXX_ARGS = -Wall -g -std=c99 -fopenmp
DEPS=statml.c common_case.c
LIBRARIES =`gsl-config --libs` $(DEPS)
OUT_DIRS=fig_out
BASENAME=question
ALL_OUTS=$(BASENAME)31 $(BASENAME)32 

default: all


run_all: $(ALL_OUTS)
	for number in 31 32 ; do \
		./$(BASENAME)$$number; \
	done;\


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


out_dirs:
	mkdir -p $(OUT_DIRS)

clean:
	rm $(ALL_OUTS)

all: $(ALL_OUTS) $(DEPS)
