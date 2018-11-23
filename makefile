CC := emcc
CFLAGS := -O3 -s WASM=1 -pedantic -Wall -Wno-c++11-extensions -std=c++17
EMCCFLAGS := 

TEST_CFLAGS := 
TEST_EMCCFLAGS := -s DISABLE_EXCEPTION_CATCHING=0

SRCEXT := cpp
INCLUDEDIR := src

SRCDIR := src
BUILDDIR := build
TARGET := bin/main.html

TESTSRCDIR := test
TESTBUILDDIR := build/test
TESTTARGET := bin/test/main.html

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
tests: $(OBJECTSSANSMAIN) $(TESTOBJECTS)
	$(CC) $(CFLAGS) $(EMCCFLAGS) $^ -I $(INCLUDEDIR) -o $(TESTTARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(INCLUDES)
	$(CC) $(CFLAGS) $(EMCCFLAGS) -I $(INCLUDEDIR) -c $< -o $@

$(TESTBUILDDIR)/%.o: $(TESTSRCDIR)/%.cpp $(INCLUDES) $(TESTINCLUDES)
	$(CC) $(CFLAGS) $(EMCCFLAGS) -I $(INCLUDEDIR) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(BUILDDIR)/*.*
	$(RM) $(TESTBUILDDIR)/*.*