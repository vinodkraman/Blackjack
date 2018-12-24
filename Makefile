
# How to use this Makefile...
###################
###################
##               ##
##  $ make help  ##
##               ##
###################
###################

# IMPORTANT NOTES:
#   1. Set EXECUTABLE to the command name from the project specification.
#   2. To enable automatic creation of unit test rules, your program logic
#      (where main() is) should be in a file named project*.cpp or
#      specified in the PROJECTFILE variable.
#   3. Files you want to include in your final submission cannot match the
#      test*.cpp pattern.

# enables c++17
PATH := /usr/um/gcc-6.2.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-6.2.0/lib64
LD_RUN_PATH := /usr/um/gcc-6.2.0/lib64

# TODO
# Change EXECUTABLE to match the command name given in the project spec.
EXECUTABLE  = blackjack
DEBUG       = $(EXECUTABLE)_debug

# designate which compiler to use
CXX         = g++

# list of test drivers (with main()) for development
TESTSOURCES = $(wildcard test*.cpp)
# names of test executables
TESTS       = $(TESTSOURCES:%.cpp=%)

# list of sources used in project
SOURCES     = $(wildcard *.cpp)
SOURCES     := $(filter-out $(TESTSOURCES), $(SOURCES))
# list of objects used in project
OBJECTS     = $(SOURCES:%.cpp=%.o)

# TODO
# If main() is in a file named project*.cpp, use the following line
PROJECTFILE = $(or $(wildcard project*.cpp), $(EXECUTABLE).cpp)
# TODO
# If main() is in another file delete line above, edit and uncomment below
#PROJECTFILE = main.cpp

# name of the tarball created for submission
#PARTIAL_SUBMITFILE = partialsubmit.tar.gz
#FULL_SUBMITFILE = fullsubmit.tar.gz

# name of the perf data file, only used by the clean target
PERF_FILE = perf.data*

#Default Flags (we prefer -std=c++17 but Mac/Xcode/Clang doesn't support)
CXXFLAGS = -std=c++1z -Wall -Werror -Wextra -pedantic 

# make release - will compile "all" with $(CXXFLAGS) and the -O3 flag
#                also defines NDEBUG so that asserts will not check
release: CXXFLAGS += -O3 -DNDEBUG
release: $(EXECUTABLE)

# make debug - will compile "all" with $(CXXFLAGS) and the -g flag
#              also defines DEBUG, so "#ifdef DEBUG /*...*/ #endif" works
debug: EXECUTABLE := $(DEBUG)
debug: CXXFLAGS += -g3 -DDEBUG
debug: clean $(EXECUTABLE)

# make profile - will compile "all" with $(CXXFLAGS) and the -pg flag
profile: CXXFLAGS += -pg
profile: clean $(EXECUTABLE)

# make static - will perform static analysis in the matter currently used
#               on the autograder
static:
	cppcheck --enable=all --suppress=missingIncludeSystem \
      $(SOURCES) *.h *.hpp

# Build both release and debug executables
all: clean
	$(MAKE) -Rr release
	$(MAKE) debug

$(EXECUTABLE): $(OBJECTS)
ifeq ($(EXECUTABLE), executable)
	@echo Edit EXECUTABLE variable in Makefile.
	@echo Using default a.out.
	$(CXX) $(CXXFLAGS) $(OBJECTS)
else
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXECUTABLE)
endif

# Automatically generate any build rules for test*.cpp files
define make_tests
    ifeq ($$(PROJECTFILE),)
	    @echo Edit PROJECTFILE variable to .cpp file with main\(\)
	    @exit 1
    endif
    SRCS = $$(filter-out $$(PROJECTFILE), $$(SOURCES))
    OBJS = $$(SRCS:%.cpp=%.o)
    HDRS = $$(wildcard *.h *.hpp)
    $(1): CXXFLAGS += -g3 -DDEBUG
    $(1): $$(OBJS) $$(HDRS) $(1).cpp
	$$(CXX) $$(CXXFLAGS) $$(OBJS) $(1).cpp -o $(1)
endef
$(foreach test, $(TESTS), $(eval $(call make_tests, $(test))))

alltests: clean $(TESTS)

# rule for creating objects
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp

# make clean - remove .o files, executables, tarball
clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(DEBUG) $(TESTS) \
      $(PARTIAL_SUBMITFILE) $(FULL_SUBMITFILE) $(PERF_FILE)
	rm -Rf *.dSYM

# these targets do not create any files
.PHONY: all release debug profile static clean alltests partialsubmit \
        fullsubmit sync2caen help
# disable built-in rules
.SUFFIXES:
