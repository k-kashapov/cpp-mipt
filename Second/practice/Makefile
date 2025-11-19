SRCS=$(wildcard *.cpp)
ELFS=$(addprefix bin/,$(SRCS:.cpp=.elf))

bin: $(ELFS)

format:
	clang-format -i -style="{BasedOnStyle: LLVM, IndentWidth: 4, ColumnLimit: 100}" $(SRCS)

bin/%.elf: %.cpp
	clang++-19 --std=c++20 $< -o $@ -Wall -Wextra -Wpedantic -fsanitize=address,undefined -lgtest

clean:
	- rm -f $(abspath .)/bin/*

test: $(ELFS)
	for file in $^ ; do \
		echo $${file} ; \
	done
