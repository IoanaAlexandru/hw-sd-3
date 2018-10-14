//ALEXANDRU Ioana - 314CB

typedef struct celula {
	void *info; //este defapt un TTrie
	struct celula *urm;
}TCelula, *TLista, **ALista;

typedef struct trie {
	char c, w; //w este 1 pentru sfârșit de cuvânt, 0 altfel
	TLista fii;
}TTrie;