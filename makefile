build/tree: build/intermediate/tree.o
	mkdir -p build
	gcc build/intermediate/tree.o -o build/tree -Wall

build/intermediate/tree.o: src/tree.c
	mkdir -p build/intermediate
	gcc src/tree.c -c -o build/intermediate/tree.o -Wall

clean:
	rm -rf build
