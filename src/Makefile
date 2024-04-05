FILES=*.c stack/*.c calc/*.c calc/expr_validation_and_transformation/expr_validation_and_transformation.c dictionary/dictionary.c
TEST_FILES=stack/*.c calc/*.c calc/expr_validation_and_transformation/expr_validation_and_transformation.c dictionary/dictionary.c tests/tests.c
GCOV_FLAGS=-fprofile-arcs -ftest-coverage -lgcov
PKG_CONFIG=`pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`

all: install
	./../calculator/s21_smart_calc

install:
	mkdir ../calculator
	gcc ${PKG_CONFIG} ${FILES} -lm -Wall -Werror -Wextra -o ../calculator/s21_smart_calc

uninstall:
	-rm -r ../calculator

test:
	gcc ${PKG_CONFIG} ${TEST_FILES} -lm -lcheck -lrt -lpthread -o tests.out

dvi:
	latex documentation.tex

dist: clean
	tar -cvzf s21_smart_calc-1.0.tar.gz *

gcov_report:
	gcc ${PKG_CONFIG} ${TEST_FILES} -lm -lcheck -lrt -lpthread ${GCOV_FLAGS} -o gcov_report.out
	./gcov_report.out
	geninfo "." -b "." -o ./coverage.info
	genhtml coverage.info -o temp

clean: uninstall
	-rm *.o *.a *.gc* *.out *.info
	-rm -r temp
	-rm .clang-format
	-rm .gitkeep
	-rm -r .vscode
