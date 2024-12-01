DAYS := $(wildcard [0-2][0-9])
DAYS_DIRS := $(addsuffix /,$(DAYS))
DAYS_SRCS := $(join $(DAYS_DIRS),$(addsuffix .cpp,$(DAYS)))
DAYS_EXAMPLES_IN := $(addsuffix example.in.txt,$(DAYS_DIRS))
DAYS_EXAMPLES_OUT := $(addsuffix example.out.txt,$(DAYS_DIRS))
SRCS := main.cpp $(DAYS_SRCS)

LIST_OF_IMPLS_PREFIX := ${addprefix X(,$(DAYS)}
LIST_OF_IMPLS := ${addsuffix ),$(LIST_OF_IMPLS_PREFIX)}

# flags
CXX := clang++
CXXFLAGS := -std=c++17 -O2 -g -Wall -Wextra -Wpedantic -DLIST_OF_IMPLS="$(LIST_OF_IMPLS)"

aoc24: main.cpp $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: tidy clean

tidy:
	clang-tidy $(SRCS) -- $(CXXFLAGS)

clean:
	rm -f aoc24
