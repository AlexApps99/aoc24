DAYS := $(wildcard [0-2][0-9])
DAYS_SRCS := $(addsuffix .cpp,$(join $(addsuffix /,$(DAYS)),$(DAYS)))
SRCS := main.cpp $(DAYS_SRCS)

LIST_OF_IMPLS_PREFIX := ${addprefix X(,$(DAYS)}
LIST_OF_IMPLS := ${addsuffix ),$(LIST_OF_IMPLS_PREFIX)}

aoc24: main.cpp $(SRCS)
	$(CXX) -DLIST_OF_IMPLS="$(LIST_OF_IMPLS)" -O2 -g -o $@ $^

.PHONY: clean
clean:
	rm -f aoc24

