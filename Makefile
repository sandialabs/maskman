# -*- mode: makefile-gmake ; compile-command: "gmake clean genmake-warning" -*-

CFLAGS ?= -O2 -g
NAME := maskman
CMAKE_GENERATOR := "Unix Makefiles"
BUILD_COMMAND ?= gmake
APP_GCOV ?= gcov
APP_GCOVR ?= ~/.local/bin/gcovr
APP_LLVM_PROFDATA ?= llvm-profdata
APP_LLVM_COV ?= llvm-cov
APP_CLANG_FORMAT ?= clang-format
APP_DIFF ?= diff

# this makes it require GNU Make as opposed to, say, BSD Make
#   THIS DOESN'T WORK IF YOUR PATH HAS SPACES... WILL NEED TO MANUALLY SPECIFY THESE VARS IN THAT CASE
mkfile_path ?= $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir ?= $(dir $(mkfile_path))


all: app
app: genmake-app
test: test-app
install: install-app
package: package-app

genmake-app: CMAKE_GENERATOR = "Unix Makefiles"
genmake-app: BUILD_COMMAND = make
genmake-app: build-app
genninja-app: CMAKE_GENERATOR = "Ninja"
genninja-app: BUILD_COMMAND = ninja
genninja-app: build-app
genmake-install-app: CMAKE_GENERATOR = "Unix Makefiles"
genmake-install-app: BUILD_COMMAND = make
genmake-install-app: install-app
genninja-install-app: CMAKE_GENERATOR = "Ninja"
genninja-install-app: BUILD_COMMAND = ninja
genninja-install-app: install-app

genmake-bundle: CMAKE_GENERATOR = "Unix Makefiles"
genmake-bundle: BUILD_COMMAND = make
genmake-bundle: CC = musl-gcc
genmake-bundle: CFLAGS = -Os -static
genmake-bundle: build-bundle

genmake-cosmo: CMAKE_GENERATOR = "Unix Makefiles"
genmake-cosmo: BUILD_COMMAND = make
genmake-cosmo: build-cosmo

genmake-configure: CMAKE_GENERATOR = "Unix Makefiles"
genmake-configure: BUILD_COMMAND = make
genmake-configure: configure

genmake-simple: CMAKE_GENERATOR = "Unix Makefiles"
genmake-simple: BUILD_COMMAND = make
genmake-simple: build-simple

genmake-debug: CMAKE_GENERATOR = "Unix Makefiles"
genmake-debug: BUILD_COMMAND = make VERBOSE=1
genmake-debug: build-debug
genninja-debug: CMAKE_GENERATOR = "Ninja"
genninja-debug: BUILD_COMMAND = ninja -v
genninja-debug: build-debug

genmake-warning: CMAKE_GENERATOR = "Unix Makefiles"
genmake-warning: BUILD_COMMAND = make VERBOSE=1
genmake-warning: build-warning
genninja-warning: CMAKE_GENERATOR = "Ninja"
genninja-warning: BUILD_COMMAND = ninja -v
genninja-warning: build-warning

genmake-clang-tidy: CMAKE_GENERATOR = "Unix Makefiles"
genmake-clang-tidy: BUILD_COMMAND = make VERBOSE=1
genmake-clang-tidy: build-clang-tidy
genninja-clang-tidy: CMAKE_GENERATOR = "Ninja"
genninja-clang-tidy: BUILD_COMMAND = ninja -v
genninja-clang-tidy: build-clang-tidy

genmake-clang-coverage: CMAKE_GENERATOR = "Unix Makefiles"
genmake-clang-coverage: BUILD_COMMAND = make VERBOSE=1
genmake-clang-coverage: build-clang-coverage
genninja-clang-coverage: CMAKE_GENERATOR = "Ninja"
genninja-clang-coverage: BUILD_COMMAND = ninja -v
genninja-clang-coverage: build-clang-coverage

genmake-gnu-coverage: CMAKE_GENERATOR = "Unix Makefiles"
genmake-gnu-coverage: BUILD_COMMAND = make VERBOSE=1
genmake-gnu-coverage: build-gnu-coverage
genninja-gnu-coverage: CMAKE_GENERATOR = "Ninja"
genninja-gnu-coverage: BUILD_COMMAND = ninja -v
genninja-gnu-coverage: build-gnu-coverage

genmake-gnu-address-sanitizer: CMAKE_GENERATOR = "Unix Makefiles"
genmake-gnu-address-sanitizer: BUILD_COMMAND = make VERBOSE=1
genmake-gnu-address-sanitizer: build-gnu-address-sanitizer
genninja-gnu-address-sanitizer: CMAKE_GENERATOR = "Ninja"
genninja-gnu-address-sanitizer: BUILD_COMMAND = ninja -v
genninja-gnu-address-sanitizer: build-gnu-address-sanitizer

genmake-clang-address-sanitizer: CMAKE_GENERATOR = "Unix Makefiles"
genmake-clang-address-sanitizer: BUILD_COMMAND = make VERBOSE=1
genmake-clang-address-sanitizer: build-clang-address-sanitizer
genninja-clang-address-sanitizer: CMAKE_GENERATOR = "Ninja"
genninja-clang-address-sanitizer: BUILD_COMMAND = ninja -v
genninja-clang-address-sanitizer: build-clang-address-sanitizer

genmake-valgrind: CMAKE_GENERATOR = "Unix Makefiles"
genmake-valgrind: BUILD_COMMAND = make VERBOSE=1
genmake-valgrind: build-valgrind
genninja-valgrind: CMAKE_GENERATOR = "Ninja"
genninja-valgrind: BUILD_COMMAND = ninja -v
genninja-valgrind: build-valgrind

configure:
	mkdir -p _build_configure _install
	cd _build_configure && cmake -DCMAKE_C_COMPILER=$(CC) -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_INSTALL_PREFIX:path=../_install -G $(CMAKE_GENERATOR) "$(mkfile_dir)"
	cp _build_configure/compile_commands.json "$(mkfile_dir)"

build-app:
	mkdir -p _build_app _install
	cd _build_app && cmake -DCMAKE_C_COMPILER=$(CC) -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_INSTALL_PREFIX:path=../_install -G $(CMAKE_GENERATOR) "$(mkfile_dir)"
	cp _build_app/compile_commands.json "$(mkfile_dir)"
	cd _build_app && $(BUILD_COMMAND) -j 4

build-bundle:
	mkdir -p _build_bundle _install
	cd _build_bundle && cmake -DCMAKE_C_COMPILER=$(CC) -DCMAKE_C_FLAGS="$(CFLAGS)" -DMaskman_Application:bool=ON -DMaskman_Documentation:bool=ON -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_INSTALL_PREFIX:path=../_install -G $(CMAKE_GENERATOR) "$(mkfile_dir)"
	cp _build_bundle/compile_commands.json "$(mkfile_dir)"
	cd _build_bundle && $(BUILD_COMMAND) -j 4

build-cosmo:
	mkdir -p _build_cosmo _install
	cd _build_cosmo && cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/cosmo.cmake -DMaskman_Application:bool=ON -DMaskman_Documentation:bool=ON -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_INSTALL_PREFIX:path=../_install -G $(CMAKE_GENERATOR) "$(mkfile_dir)"
	cp _build_cosmo/compile_commands.json "$(mkfile_dir)"
	cd _build_cosmo && $(BUILD_COMMAND) -j 4

build-simple:
	mkdir -p _build_simple _install
	cd _build_simple && cmake -DCMAKE_C_COMPILER=$(CC) -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX:path=../_install -G $(CMAKE_GENERATOR) "$(mkfile_dir)"
	cp _build_simple/compile_commands.json "$(mkfile_dir)"
	cd _build_simple && $(BUILD_COMMAND) -j 4

build-debug:
	mkdir -p _build_debug _install
	cd _build_debug && cmake -DCMAKE_C_COMPILER=$(CC) -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX:path=../_install -G $(CMAKE_GENERATOR) "$(mkfile_dir)"
	cd _build_debug && $(BUILD_COMMAND) -j 4

build-warning:
#	clang-tidy affimask.c 1>>$(NAME)-tidy.log 2>>$(NAME)-tidy.log
	mkdir -p _build_warning _install
#cd _build_warning && cmake -DCMAKE_C_COMPILER=$(CC) -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX:path=../_install -G $(CMAKE_GENERATOR) -DWARNING:bool=ON $(mkfile_dir)
	cd _build_warning && cmake -DCMAKE_C_COMPILER=$(CC) -DCMAKE_INSTALL_PREFIX:path=../_install -G $(CMAKE_GENERATOR) -DWARNING:bool=ON "$(mkfile_dir)"
	cd _build_warning && $(BUILD_COMMAND) -j 1

build-clang-tidy:
#	clang-tidy affimask.c 1>>$(NAME)-tidy.log 2>>$(NAME)-tidy.log
	echo "OMPI_CC=${OMPI_CC}"
	mkdir -p _build_clang_tidy _install
	cd _build_clang_tidy && cmake -DCMAKE_C_COMPILER=$(CC) -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX:path=../_install -G $(CMAKE_GENERATOR) -DLLVM_CLANG_TIDY:bool=ON "$(mkfile_dir)"
	cd _build_clang_tidy && $(BUILD_COMMAND) -j 1

build-clang-coverage:
	mkdir -p _build_clang_coverage _install
	cd _build_clang_coverage && cmake -DCMAKE_C_COMPILER=$(CC) -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX:path=../_install -G $(CMAKE_GENERATOR) -DLLVM_COVERAGE:bool=ON "$(mkfile_dir)"
	cd _build_clang_coverage && $(BUILD_COMMAND) -j 4

build-gnu-coverage:
	mkdir -p _build_gnu_coverage _install
	cd _build_gnu_coverage && cmake -DCMAKE_C_COMPILER=$(CC) -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX:path=../_install -G $(CMAKE_GENERATOR) -DGNU_COVERAGE:bool=ON "$(mkfile_dir)"
	cd _build_gnu_coverage && $(BUILD_COMMAND) -j 4
#cd _build_gnu_coverage && cmake -DCMAKE_C_COMPILER=$(CC) -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX:path=../_install -G $(CMAKE_GENERATOR) -DCOVERAGE_COMMAND=${APP_GCOV} -DGNU_COVERAGE:bool=ON $(mkfile_dir)

build-gnu-address-sanitizer:
	mkdir -p _build_gnu_address_sanitizer _install
	cd _build_gnu_address_sanitizer && cmake -DCMAKE_C_COMPILER=$(CC) -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX:path=../_install -G $(CMAKE_GENERATOR) -DGNU_ASAN:bool=ON "$(mkfile_dir)"
	cd _build_gnu_address_sanitizer && $(BUILD_COMMAND) -j 4

build-clang-address-sanitizer:
#	scan-build $(CC) $(CFLAGS) -o $(NAME)-static affimask.c 1>>$(NAME)-static.log 2>>$(NAME)-static.log
#	-fsanitize=address -O1 -fno-omit-frame-pointer -fno-optimize-sibling-calls
#	cd _build_clang_address_sanitizer && cmake -DCMAKE_C_COMPILER=$(CC) -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX:path=../_install -DCMAKE_COMPILE_WARNING_AS_ERROR=ON -DCMAKE_C_FLAGS:string="-O1 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls" ..
	mkdir -p _build_clang_address_sanitizer _install
	cd _build_clang_address_sanitizer && cmake -DCMAKE_C_COMPILER=$(CC) -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX:path=../_install -G $(CMAKE_GENERATOR) -DLLVM_ASAN:bool=ON "$(mkfile_dir)"
	cd _build_clang_address_sanitizer && $(BUILD_COMMAND) -j 4

# Valgrind seems to be more sensitive when Shim is linked dynamically than the other 2...
build-valgrind:
	mkdir -p _build_valgrind _install
	cd _build_valgrind && cmake -DCMAKE_C_COMPILER=$(CC) -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_INSTALL_PREFIX:path=../_install -DMEMORYCHECK_COMMAND_OPTIONS="-q --trace-children=yes --tool=memcheck  --leak-check=yes --num-callers=50 --gen-suppressions=all --error-exitcode=1 --suppressions=$(mkfile_dir)/test/valgrind/valgrind-suppress.txt" -G $(CMAKE_GENERATOR) "$(mkfile_dir)"
	cp _build_valgrind/compile_commands.json "$(mkfile_dir)"
	cd _build_valgrind && $(BUILD_COMMAND) -j 4

test-app:
	cd _build_app && ctest -j 2

test-bundle:
	cd _build_bundle && ctest -j 2

test-cosmo:
	cd _build_cosmo && ctest -j 2

test-simple:
	cd _build_simple && ctest -j 2

test-debug:
	cd _build_debug && ctest -j 2

test-warning:
	cd _build_warning && ctest -j 2

test-clang-tidy:
	cd _build_clang_tidy && ctest -j 2

test-clang-coverage:
	cd _build_clang_coverage && LLVM_PROFILE_FILE=../../$(NAME)-%p.profraw ctest -j 2
	${APP_LLVM_PROFDATA} merge -sparse _build_clang_coverage/$(NAME)-*.profraw -o _build_clang_coverage/$(NAME).profdata
	${APP_LLVM_COV} show ./_build_clang_coverage/appMultishim/app-multishim -instr-profile=_build_clang_coverage/$(NAME).profdata --show-line-counts
	${APP_LLVM_COV} report ./_build_clang_coverage/appMultishim/app-multishim -instr-profile=_build_clang_coverage/$(NAME).profdata
#${APP_LLVM_COV} show ./_build_clang_coverage/appMultishim/app-multishim -instr-profile=_build_clang_coverage/$(NAME).profdata --show-line-counts-or-regions --show-branches=count --show-expansions

test-gnu-coverage:
	cd _build_gnu_coverage && ctest -j 2 -T Test -T Coverage
#	cd _build_gnu_coverage && ${APP_GCOV} -b -x -m `find . -name "*.gcda" | sed 's/gcda//g'`

report-gnu-coverage:
	-rm -f report-gnu-coverage.txt
	${APP_GCOVR} -r ./ --txt report-gcc-coverage.txt
	${APP_GCOVR} -r ./ --xml-pretty -o report-gcc-coverage.xml
	cat report-gcc-coverage.txt

test-valgrind:
#	valgrind --leak-check=full --show-leak-kinds=all --show-error-list=yes --track-origins=yes -s -v ./$(NAME) --verbose 1>>$(NAME)-valgrind.log 2>>$(NAME)-valgrind.log
	# cd _build_warning && ctest --overwrite MemoryCheckCommandOptions="--leak-check=full --default-suppressions=yes --gen-supressions=yes" -T memcheck -VV -j 2
	cd _build_valgrind && ctest -T memcheck -VV -j 2

test-gnu-address-sanitizer: export ASAN_OPTIONS = detect_leaks=1
test-gnu-address-sanitizer: export LSAN_OPTIONS = verbosity=1:log_threads=1:suppressions="$(mkfile_dir)/test/asan/default.supp"
test-gnu-address-sanitizer:
	@echo "ASAN_OPTIONS=$(ASAN_OPTIONS)"
	@echo "LSAN_OPTIONS=$(LSAN_OPTIONS)"
	cd _build_gnu_address_sanitizer && ctest -j 1 --output-on-failure

test-clang-address-sanitizer: export ASAN_OPTIONS = detect_leaks=1
test-clang-address-sanitizer: export LSAN_OPTIONS = verbosity=1:log_threads=1:suppressions="$(mkfile_dir)/test/asan/default.supp"
test-clang-address-sanitizer:
	@echo "ASAN_OPTIONS=$(ASAN_OPTIONS)"
	@echo "LSAN_OPTIONS=$(LSAN_OPTIONS)"
	cd _build_clang_address_sanitizer && ctest -j 1 --output-on-failure

analyze: test-app test-warning test-clang-tidy test-clang-address-sanitizer test-valgrind

clang-format-alter:
	$(APP_CLANG_FORMAT) -style=file -i "$(mkfile_dir)"/lib*/*.[ch] "$(mkfile_dir)"/method*/*.[ch] "$(mkfile_dir)"/app*/*.[ch]

clang-format-check:
	$(APP_CLANG_FORMAT) -style=file --dry-run -Werror "$(mkfile_dir)"/lib*/*.[ch] "$(mkfile_dir)"/method*/*.[ch] "$(mkfile_dir)"/app*/*.[ch]

.PHONY: doc-makefile
doc-makefile:
	make -C doc all

.PHONY: doc-makefile-clean
doc-makefile-clean:
	make -C doc initpurge

.PHONY: doc
doc:
	mkdir -p _build_doc _install
	cd _build_doc && cmake -DCMAKE_C_COMPILER=$(CC) -DMaskman_Application:bool=OFF -DMaskman_Documentation:bool=ON -DCMAKE_INSTALL_PREFIX:path=../_install -G $(CMAKE_GENERATOR) "$(mkfile_dir)"
	cd _build_doc && $(BUILD_COMMAND) -j 4

install-app:
	cd _build_app && $(BUILD_COMMAND) install

install-doc:
	cd _build_doc && $(BUILD_COMMAND) install

install-bundle:
	cd _build_bundle && $(BUILD_COMMAND) install

install-cosmo:
	cd _build_cosmo && $(BUILD_COMMAND) install

package-bundle:
	cd _build_bundle && cpack

package-cosmo:
	cd _build_cosmo && cpack

cloc:
	cloc "$(mkfile_dir)"

clean:
	-rm -f $(NAME) $(NAME)-*
	-rm -rf _build* _install* _package* *coverage.txt
