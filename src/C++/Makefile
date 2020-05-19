all: build
build:
	g++ mkindex.cpp -O3 -o mkindex
	g++ filter_sort.cpp -O3 -o filter_sort
preprocess: build
	ln -s /home/shared/uniprot/uniprot_trembl.fasta .
	./filter_sort uniprot_trembl.fasta
	./mkindex uniprot_trembl.fasta.sorted
