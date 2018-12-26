CC := emcc
CXXFLAGS := -O3 -s WASM=1 -pedantic -Wall -Wno-c++11-extensions -std=c++17
EMCCFLAGS := 

TEST_CXXFLAGS := -I googletest/googletest/include -I googletest/googletest/
TEST_EMCCFLAGS := -s DISABLE_EXCEPTION_CATCHING=0 --emrun

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
all: tests examples performance clean

.PHONY: tests
tests: CXXFLAGS += $(TEST_CXXFLAGS)
tests: EMCCFLAGS += $(TEST_EMCCFLAGS)
tests: $(OBJECTS) $(TESTOBJECTS) $(TESTBUILDDIR)/gtest_all.o
	$(CC) $(CXXFLAGS) $(EMCCFLAGS) $^ -I $(INCLUDEDIR) -o $(TESTTARGET)
	rm -f bin/test/test.xml
	#sh make_test_xml.sh to produce an xml report

.PHONY: examples
examples: $(OBJECTS)
	$(CC) $(CXXFLAGS) $(EMCCFLAGS) -I $(INCLUDEDIR) $^ examples/to_json.cpp \
	-s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']" \
	--shell-file examples/shellfiles/to_json_shell.html \
	-o $(EXAMPLETARGETDIR)/to_json.html
	$(CC) $(CXXFLAGS) $(EMCCFLAGS) -I $(INCLUDEDIR) $^ examples/explain.cpp \
	-s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']" \
	--shell-file examples/shellfiles/explain_shell.html \
	-o $(EXAMPLETARGETDIR)/explain.html

.PHONY: performance
performance: $(OBJECTS) $(TESTBUILDDIR)/testdata_real.o
	$(CC) $(CXXFLAGS) $(EMCCFLAGS) -I $(INCLUDEDIR) -I $(TESTSRCDIR) $^ \
	performance/main.cpp -o bin/performance/check.html

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(INCLUDES)
	mkdir -p $(BUILDDIR)
	$(CC) $(CXXFLAGS) $(EMCCFLAGS) -I $(INCLUDEDIR) -c $< -o $@

$(TESTBUILDDIR)/%.o: $(TESTSRCDIR)/%.cpp $(INCLUDES) $(TESTINCLUDES)
	mkdir -p $(TESTBUILDDIR)
	$(CC) $(CXXFLAGS) $(EMCCFLAGS) -I $(INCLUDEDIR) -c $< -o $@

$(TESTBUILDDIR)/gtest_all.o: googletest/googletest/src/gtest-all.cc
	mkdir -p $(TESTBUILDDIR)
	$(CC) $(CXXFLAGS) $(EMCCFLAGS) -I $(INCLUDEDIR) -c $< -o $@

.PHONY: clean
clean:
	rm --recursive --force $(TESTBUILDDIR)
	rm --recursive --force $(BUILDDIR)
