//ALEXANDRU Ioana - 314CB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPSIZE 21

#include "trie.h"
#include "functii.h"

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: %s in_file out_file\n", argv[0]);
		return -1;
	}
	FILE *in = fopen(argv[1], "r");
	if (!in) {
		printf("Could not open input file\n");
		return -1;
	}
	FILE *out = fopen(argv[2], "w");
	if (!out) {
		printf("Could not open output file\n");
		return -1;
	}

	TTrie *trie = aloca_trie('\0');
	if (!trie) {
		printf("Out of memory!\n");
		return -1;
	}
	int WSIZE = 100, N, i, l, rez, ok;
	float ml;
	char c;
	char *op = calloc(OPSIZE, sizeof(char)),
	     *w  = calloc(WSIZE, sizeof(char)),
	     *p  = calloc(WSIZE, sizeof(char));
	if (!op || !w || !p) {
		printf("Out of memory!\n");
		return -1;
	}

	fscanf(in, "%d\n", &N);

	for (i = 1; i <= N; i++) {
		l = 0, ok = 1; 
		//ok = 1 => citesc operația
		//ok = 0 => citesc w
		c = getc(in);
		while (c != '\n' && c != EOF) {
			if (ok) {
				op[l] = c;
			} else {
				if (l == WSIZE) {
					WSIZE *= 2;
					w = realloc(w, WSIZE);
					p = realloc(p, WSIZE);
				}
				w[l] = c;
			}
			c = getc(in);
			if (c == ' ') {
				op[l + 1] = 0;
				l = 0;
				ok = 0;
				c = getc(in);
				continue;
			}
			if (ok && (c == '\n' || c == EOF)) {
				op[l + 1] = 0;
				l = 0;
				break;
			}
			l++;
		}
		w[l] = 0;

		if (!strcmp(op, "add")) {
			rez = tadd(trie, w);
			if (!rez)
				printf("Adăugarea cuvântului %s nu a reușit!\n", w);
		} else if (!strcmp(op, "remove")) {
			rez = tremove(trie, w, 0, 1);
			//dacă se dorește afișarea unui mesaj, se va verifica rez
		} else if (!strcmp(op, "find")) {
			rez = tfind(trie, w, 0);
			if (rez == 1)
				fprintf(out, "True\n");
			else
				fprintf(out, "False\n");
		} else if (!strcmp(op, "find_longest_prefix")) {
			p[0] = 0;
			rez = tfindprefix(trie, w, 0, p);
			if (!strlen(p))
				fprintf(out, "None\n");
			else
				fprintf(out, "%s\n", p);
		} else if (!strcmp(op, "find_all_with_prefix")) {
			rez = tprefix(trie, w, 0, out);
			if (rez != 1)
				fprintf(out, "None\n");
		} else if (!strcmp(op, "mean_length")) {
			ml = tfindlength(trie, w, 0);
			if (!ml)
				fprintf(out, "0\n");
			else
				fprintf(out, "%.3f\n", ml);
		} else {
			printf("Invalid operation %d: %s!\n", i, op);
			return -1;
		}
	}

	free(op);
	free(w);
	free(p);
	elib_trie(trie);
	fclose(in);
	fclose(out);
	return 0;
}