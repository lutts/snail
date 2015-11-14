# (setenv "PROJECT_ROOT" "/home/lutts/devel2/Qt/Qt5/snail")

all: debug-build

release-build: FORCE
	make -C build
	cp build/compile_commands.json ./

debug-build: FORCE
	scan-build make -C build
	cp build/compile_commands.json ./

# TODO(lutts): should not use BUILD_SHARED_LIBS here, pls find a solution
release-prepare:
	-mkdir build
	cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DBUILD_SHARED_LIBS=true ..

debug-prepare:
	/bin/rm -rf build
	mkdir build
	cd build && scan-build cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DBUILD_SHARED_LIBS=true ..

#	cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..


cpplint: FORCE
	make -C build cpplint

cpplint-%:
	ENV_CPPLINT_ROOT=$* make -C build cpplint

test: FORCE
	cd build && ctest -E 'memcheck_*' --output-on-failure

memcheck: FORCE
	cd build && ctest -R 'memcheck_*' --output-on-failure

qtui_memcheck: FORCE
	cd build && ctest -R 'qtui_memcheck_*' --output-on-failure

test-%: FORCE
	cd build && ctest --output-on-failure -R $*

list-test: FORCE
	cd build && ctest -N

run: FORCE
	./build/src/main/snail

index: FORCE
	gindex-cpp.sh

Makefile:;

%:: FORCE
	scan-build make -C build $@
#	cp build/compile_commands.json ./

FORCE:

.PHONY: FORCE all cpplint
