CC := emcc
CFLAGS := -O3 -s WASM=1 -pedantic -Wall -Wno-c++11-extensions -std=c++17
EMCCFLAGS := 

TEST_CFLAGS := -I googletest/googletest/include -I googletest/googletest/
TEST_EMCCFLAGS := -s DISABLE_EXCEPTION_CATCHING=0 

INCLUDEDIR := src
SRCDIR := src
BUILDDIR := build

TESTSRCDIR := test
TESTBUILDDIR := build/test
TESTTARGET := bin/test/main.html

EXAMPLETARGETDIR := bin/examples

SOURCES := $(shell find $(SRCDIR) -type f -name *.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.cpp=.o))
INCLUDES := $(shell find $(INCLUDEDIR) -type f -name *.h)

TESTSOURCES := $(shell find $(TESTSRCDIR) -type f -name *.cpp)
TESTOBJECTS := $(patsubst $(TESTSRCDIR)/%,$(TESTBUILDDIR)/%,$(TESTSOURCES:.cpp=.o))
TESTINCLUDES := $(shell find $(TESTSRCDIR) -type f -name *.h)

.PHONY: all
all: tests examples clean

.PHONY: tests
tests: CFLAGS += $(TEST_CFLAGS)
tests: EMCCFLAGS += $(TEST_EMCCFLAGS)
tests: $(OBJECTS) $(TESTOBJECTS) build/test/gtest_all.o
	$(CC) $(CFLAGS) $(EMCCFLAGS) $^ -I $(INCLUDEDIR) -o $(TESTTARGET)

.PHONY: examples
examples: $(OBJECTS)
	$(CC) $(CFLAGS) $(EMCCFLAGS) -I $(INCLUDEDIR) $^ examples/to_json.cpp \
	-s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']" \
	--shell-file examples/shellfiles/to_json_shell.html \
	-o $(EXAMPLETARGETDIR)/to_json.html

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(INCLUDES)
	$(CC) $(CFLAGS) $(EMCCFLAGS) -I $(INCLUDEDIR) -c $< -o $@

$(TESTBUILDDIR)/%.o: $(TESTSRCDIR)/%.cpp $(INCLUDES) $(TESTINCLUDES)
	$(CC) $(CFLAGS) $(EMCCFLAGS) -I $(INCLUDEDIR) -c $< -o $@

build/test/gtest_all.o: googletest/googletest/src/gtest-all.cc
	$(CC) $(CFLAGS) $(EMCCFLAGS) -I $(INCLUDEDIR) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(BUILDDIR)/*.*
	$(RM) $(TESTBUILDDIR)/*.*
