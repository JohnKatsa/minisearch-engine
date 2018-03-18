minisearch: minisearch.o map.o list.o trie.o query.o sort.o underline.o
	g++ -o minisearch minisearch.o map.o list.o trie.o query.o sort.o underline.o 

minisearch.o: minisearch.cpp structs.h
	g++ -c minisearch.cpp

map.o: map.cpp structs.h
	g++ -c map.cpp

list.o: list.cpp structs.h
	g++ -c list.cpp

trie.o: trie.cpp structs.h
	g++ -c trie.cpp -lm

query.o: query.cpp structs.h
	g++ -c query.cpp

sort.o:	sort.cpp structs.h
	g++ -c sort.cpp

underline.o: underline.cpp structs.h
	g++ -c underline.cpp

clean:
	rm minisearch minisearch.o map.o list.o trie.o query.o sort.o underline.o
