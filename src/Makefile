CXX = g++-8
CXXFLAG = -std=c++17 -O2 -Wall -Wextra -fPIC
CXXFLAG_BONUS = -std=c++17 -O2 -Wall -Wextra -fPIC -Werror -Wextra -pedantic-errors
GMAEINCLUDE = ./
BUILDDIR = ./build

# -Werror          : regard warring as error
# -Wextra          : print extra warring hint
# -pedantic-errors : follow ANSI standard, otherwise, turn warring to error
# https://gcc.gnu.org/onlinedocs/gcc/Standards.html
# you can use "make all_bonus" to make file and get bonus :)

all: gamerunner AI

AI:
	$(CXX) $(CXXFLAG) -I$(GMAEINCLUDE) -shared AITemplate/Porting.cpp -o ${BUILDDIR}/AI.so
	cp ${BUILDDIR}/AI.so ${BUILDDIR}/a1.so
	cp ${BUILDDIR}/AI.so ${BUILDDIR}/a2.so

gamerunner:
	$(CXX) $(CXXFLAG) -I$(GMAEINCLUDE) -pthread main.cpp -o ${BUILDDIR}/gamerunner -ldl

#------------------------------------------------------------------------------------------
all_bonus: gamerunner_bonus AI_bonus

AI_bonus:
	$(CXX) $(CXXFLAG_BONUS) -I$(GMAEINCLUDE) -shared AITemplate/Porting.cpp -o ${BUILDDIR}/AI.so
	cp ${BUILDDIR}/AI.so ${BUILDDIR}/a1.so
	cp ${BUILDDIR}/AI.so ${BUILDDIR}/a2.so

gamerunner_bonus:
	$(CXX) $(CXXFLAG_BONUS) -I$(GMAEINCLUDE) -pthread main.cpp -o ${BUILDDIR}/gamerunner -ldl
#-------------------------------------------------------------------------------------------

.PHONY: clean
clean:
	-rm gamerunner a1.so
