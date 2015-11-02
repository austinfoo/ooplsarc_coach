INCDIRS:=/Users/dmielke/Documents/oopl/trees/googletest/googletest/include
LIBDIRS:=/Users/dmielke/Documents/oopl/trees/googletest/googletest/make

FILES :=                                \
    .travis.yml                         \
    dijkstra-tests/EID-RunCoach.in   \
    dijkstra-tests/EID-RunCoach.out  \
    dijkstra-tests/EID-TestCoach.c++ \
    dijkstra-tests/EID-TestCoach.out \
    Coach.c++                        \
    Coach.h                          \
    Coach.log                        \
    html                                \
    RunCoach.c++                     \
    RunCoach.in                      \
    RunCoach.out                     \
    TestCoach.c++                    \
    TestCoach.out                    \
    CoachBundle.c++

# Call gcc and gcov differently on Darwin
ifeq ($(shell uname), Darwin)
  CXX      := g++
  GCOV     := gcov
  VALGRIND := echo Valgrind not available on Darwin
else
  CXX      := g++-4.8
  GCOV     := gcov-4.8
  VALGRIND := valgrind
endif

CXXFLAGS   := -pedantic -std=c++11 -Wall -I$(INCDIRS)
LDFLAGS    := -lgtest -lgtest_main -pthread -L$(LIBDIRS)
GCOVFLAGS  := -fprofile-arcs -ftest-coverage
GPROF      := gprof
GPROFFLAGS := -pg

clean:
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
	rm -f RunCoach
	rm -f RunCoach.tmp
	rm -f TestCoach
	rm -f TestCoach.tmp
	rm -f CoachBundle

config:
	git config -l

bundle:
	cat Coach.h Coach.c++ RunCoach.c++ | sed -e "s/#include \"Coach.h\"//g" > CoachBundle.c++
	$(CXX) $(CXXFLAGS) $(GCOVFLAGS) CoachBundle.c++ -o CoachBundle

scrub:
	make  clean
	rm -f  Coach.log
	rm -rf dijkstra-tests
	rm -rf html
	rm -rf latex

status:
	make clean
	@echo
	git branch
	git remote -v
	git status

test: RunCoach.tmp TestCoach.tmp

RunCoach: Coach.h Coach.c++ RunCoach.c++
	$(CXX) $(CXXFLAGS) $(GCOVFLAGS) Coach.c++ RunCoach.c++ -o RunCoach

RunCoach.tmp: RunCoach
	./RunCoach < RunCoach.in > RunCoach.tmp
	diff RunCoach.tmp RunCoach.out

TestCoach: Coach.h Coach.c++ TestCoach.c++
	$(CXX) $(CXXFLAGS) $(GCOVFLAGS) Coach.c++ TestCoach.c++ -o TestCoach $(LDFLAGS)

TestCoach.tmp: TestCoach
	./TestCoach                                                     >  TestCoach.tmp 2>&1
	$(VALGRIND) ./TestCoach                                         >> TestCoach.tmp
	$(GCOV) -b Coach.c++     | grep -A 5 "File 'Coach.c++'"     >> TestCoach.tmp
	$(GCOV) -b TestCoach.c++ | grep -A 5 "File 'TestCoach.c++'" >> TestCoach.tmp
	cat TestCoach.tmp
