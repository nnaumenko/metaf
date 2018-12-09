CC := emcc
CFLAGS := -O3 -s WASM=1 -pedantic -Wall -Wno-c++11-extensions -std=c++17
EMCCFLAGS := 

TEST_CFLAGS := 
TEST_EMCCFLAGS := -s DISABLE_EXCEPTION_CATCHING=0 -I googletest/googletest/include \
-I googletest/googletest/

SRCEXT := cpp
INCLUDEDIR := src

SRCDIR := src
BUILDDIR := build
TARGET := bin/main.html

TESTSRCDIR := test
TESTBUILDDIR := build/test
TESTTARGET := bin/test/main.html

GTESTSRCDIR = googletest/googletest/src
GTESTBUILDDIR := build/gtest

EXAMPLEDIR := examples
EXAMPLEBUILDDIR := build/examples
EXAMPLETARGETDIR := bin/examples

SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
INCLUDES := $(shell find $(INCLUDEDIR) -type f -name *.h)

OBJECTSSANSMAIN := $(subst $(BUILDDIR)/main.o,,$(OBJECTS))

TESTSOURCES := $(shell find $(TESTSRCDIR) -type f -name *.$(SRCEXT))
TESTOBJECTS := $(patsubst $(TESTSRCDIR)/%,$(TESTBUILDDIR)/%,$(TESTSOURCES:.$(SRCEXT)=.o))
TESTINCLUDES := $(shell find $(TESTSRCDIR) -type f -name *.h)

.PHONY: release
release: $(OBJECTS)
	$(CC) $(CFLAGS) $(EMCCFLAGS) $^ -I $(INCLUDEDIR) -o $(TARGET)

.PHONY: tests
tests: EMCCFLAGS += $(TEST_EMCCFLAGS)
tests: $(OBJECTSSANSMAIN) $(TESTOBJECTS) build/test/gtest_all.o
	$(CC) $(CFLAGS) $(EMCCFLAGS) $^ -I $(INCLUDEDIR) -o $(TESTTARGET)

.PHONY: examples
examples: $(OBJECTSSANSMAIN)
	$(CC) $(CFLAGS) $(EMCCFLAGS) -I $(INCLUDEDIR) $^ $(EXAMPLEDIR)/to_json.cpp \
	-s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']" \
	--shell-file $(EXAMPLEDIR)/shellfiles/to_json_shell.html \
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
	$(RM) $(EXAMPLEBUILDDIR)/*.*
