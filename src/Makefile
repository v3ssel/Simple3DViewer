CC = gcc
CFLAGS = -Wall -Werror -Wextra
TEST_LIBS = -lcheck -lm -lpthread
GCOVFLAGS = -fprofile-arcs -ftest-coverage
ARCHIVE = archive-3dviewer
BUILD = build
INSTALL_DIR = ~/Desktop/
OS = $(shell uname)

ifeq (${OS}, Linux)
	TEST_LIBS += -lsubunit -lrt
endif

all: install dist dvi tests check

install:
	mkdir -p ${BUILD}
	cd ${BUILD} && cmake ../3DViewer/CMakeLists.txt && ${MAKE} && rm -rf ${INSTALL_DIR}/3DViewer.app && mv -f 3DViewer.app ${INSTALL_DIR}
	rm -rf ${BUILD}

open:
	open ~/Desktop/3DViewer.app

uninstall:
	rm -rf ~/Desktop/3DViewer.app

clean:
	rm -rf ${ARCHIVE}*
	rm -rf ./report test.out

dist:
	mkdir -p ${ARCHIVE}
	cp -R 3DViewer/ ${ARCHIVE}/
	tar -cf ${ARCHIVE}.tar ${ARCHIVE}
	gzip -9f ${ARCHIVE}.tar
	rm -rf ${ARCHIVE}

tests:
	$(CC) $(CFLAGS) 3DViewer/CPart/parser.c 3DViewer/Unit-tests/test.c $(TEST_LIBS) -o test.out
	./test.out

gcov_report:
	$(CC) $(CFLAGS) --coverage $(TEST_LIBS) 3DViewer/CPart/parser.c 3DViewer/Unit-tests/test.c -o gcov_test
	chmod +x *
	./gcov_test
	lcov -t "gcov_test" -o gcov_test.info --no-external -c -d .
	genhtml -o report/ gcov_test.info
	open ./report/index.html
	rm -rf ./*.gcno ./*.gcda ./gcov*

check:
	clang-format -style=Google -dump-config > .clang-format
	cd 3DViewer/ && clang-format -n CPart/parser.c 				\
									CPart/parser.h 				\
									GLWidget/scene.cpp 			\
									GLWidget/scene.h 			\
									Viewer/viewer.cpp 			\
									Viewer/viewer.h 			\
									Unit-tests/test.c			\
									GIFCreation/gifImage/*.cpp  \
									GIFCreation/gifImage/*.h 	\
									GIFCreation/gifLib/*.c 		\
									GIFCreation/gifLib/*.h		\
									main.cpp
	rm -f .clang-format