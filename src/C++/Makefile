all: build
build:
	g++ mkindex.cpp -Wall -Wextra -pedantic -O3 -std=c++17 -g -o mkindex
	g++ mkindex_nosort.cpp -Wall -Wextra -pedantic -O3 -std=c++17 -g -o mkindex_nosort
preprocess: build
	ln -s /home/shared/uniprot/uniprot_trembl.fasta .
	./filter_sort uniprot_trembl.fasta
	./mkindex uniprot_trembl.fasta.sorted
