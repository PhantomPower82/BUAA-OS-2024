.PHONY: run clean

all:
	gcc hello.c -o hello

run: hello
	./hello

clean:
	rm hello

