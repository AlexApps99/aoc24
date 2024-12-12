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
CXXFLAGS := -std=c++20 -O1 -fsanitize=address,undefined,integer,implicit-conversion,nullability -fno-omit-frame-pointer -fvisibility=hidden -g -Wall -Wextra -Wpedantic -DLIST_OF_IMPLS="$(LIST_OF_IMPLS)"

# object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# build from .o
aoc24: $(SRCS:.cpp=.o)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: tidy clean

tidy:
	clang-tidy $(SRCS) -- $(CXXFLAGS)

clean:
	rm -f aoc24 $(SRCS:.cpp=.o)
