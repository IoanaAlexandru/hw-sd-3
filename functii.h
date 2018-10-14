//ALEXANDRU Ioana - 314CB

TTrie* aloca_trie(char c)
{
	TTrie *trie = calloc(1, sizeof(TTrie));
	if (trie) {
		trie->fii = NULL;
		trie->c = c;
		trie->w = 0;
	}
	return trie;
}

TLista aloca_celula(char c)
{
	TLista cel = calloc(1, sizeof(TCelula));
	if (cel) {
		cel->urm = NULL;
		cel->info = (TTrie*)aloca_trie(c);
		if (!cel->info)
			return NULL;
	}
	return cel;
}

void elib_trie(TTrie *trie)
{
	TLista aux;
	while (trie->fii) {
		if (trie->fii->info)
			elib_trie(trie->fii->info);
		aux = trie->fii;
		trie->fii = trie->fii->urm;
		free(aux);
	}
	free(trie);
	trie = NULL;
}

//inserare sortată în listă
TTrie* list_insert(ALista l, char c)
{
	TLista nou = aloca_celula(c), aux, p;
	if (!nou) 
		return NULL;
	if (!(*l)) {
		*l = nou;
		return nou->info;
	}
	if (((TTrie*)(*l)->info)->c > c) { //caracterul va fi primul în listă
		aux = *l;
		*l = nou;
		(*l)->urm = aux;
		return nou->info;
	}
	for (p = (*l); p != NULL && p->urm != NULL; p = p->urm) {
		if (((TTrie*)p->info)->c == c) { //caracterul există deja
			free(nou->info);
			free(nou);
			return p->info;
		}
		if (((TTrie*)p->urm->info)->c > c) {
			nou->urm = p->urm;
			p->urm = nou;
			return nou->info;
		}
	}
	if (((TTrie*)p->info)->c == c) { //caracterul există deja
		free(nou->info);
		free(nou);
		return p->info;
	}
	p->urm = nou; //caracterul de inserat va fi ultimul în listă
	return nou->info;
}

int tadd(TTrie *trie, char *word)
{
	char *c = word;
	int i;
	TTrie *t = trie;
	for (i = 0; i < strlen(word); i++) {
		t = list_insert(&t->fii, *c);
		if (!t)
			return 0;
		c++;
	}
	t->w = 1;
	return 1;
}

void tprint(TTrie *trie, char *word, int level, FILE *out) {
	TLista p;
	if (!trie)
		return;
	if (trie->w) {
		word[level] = 0;
		fprintf(out, "%s ", word);
	}
	for (p = trie->fii; p != NULL; p = p->urm) {
		word[level] = ((TTrie*)p->info)->c;
		tprint((TTrie*)p->info, word, level + 1, out);
	}
}

/*
 * Eiminare element din listă
 *  1 : eliminat cu succes
 *  0 : elementul are fii
 *      (se modifică doar indicatorul de cuvânt dacă w = 1)
 * -1 : lista este goală sau elementul nu a fost găsit
 */
int list_remove(ALista l, char c, char w)
{
	if (!(*l))
		return -1;
	TLista p, ant;
	for (p = *l, ant = NULL; p != NULL; ant = p, p = p->urm) {
		if (((TTrie*)p->info)->c == c)
			break;
		if (((TTrie*)p->info)->c > c)
			return -1;
	}
	if (!p)
		return -1;
	if (!ant) {
		if (((TTrie*)p->info)->fii != NULL) {
			if (w)
				((TTrie*)p->info)->w = 0;
			return 0;
		}
		*l = p->urm;
	}
	else {
		if (((TTrie*)p->info)->fii != NULL) {
			if (w)
				((TTrie*)p->info)->w = 0;
			return 0;
		}
		ant->urm = p->urm;
	}
	free(p->info);
	free(p);
	return 1;
}

//err: analog cu list_remove
int tremove(TTrie *trie, char *word, int level, char w)
{
	TLista p;
	int rez;
	if (!trie)
		return -1;
	for (p = trie->fii; p != NULL; p = p->urm) {
		if (word[level] == ((TTrie*)p->info)->c)
			break;
		if (word[level] < ((TTrie*)p->info)->c) //cuvântul nu există
			return -1;
	}
	if (!p)
		return -1;
	rez = tremove(p->info, word, level + 1, w);
	if (word[level + 1] == 0) {
		rez = list_remove(&trie->fii, word[level], w);
		word[level] = 0;
		w = 0;
		return rez;
	}
	return -1;
}

int tfind(TTrie *trie, char *word, int level)
{
	TLista p;
	if (!trie)
		return -1;
	if (level == strlen(word) && trie->w)
		return 1;
	for (p = trie->fii; p != NULL; p = p->urm) {
		if (word[level] == ((TTrie*)p->info)->c)
			return tfind(p->info, word, level + 1);
		if (word[level] < ((TTrie*)p->info)->c)
			return 0;
	}
	return 0;
}

int tfindprefix(TTrie *trie, char *word, int level, char *prefix)
{
	TLista p;
	if (!trie)
		return -1;
	if (level == strlen(word))
		return 1;
	for (p = trie->fii; p != NULL; p = p->urm) {
		if (word[level] == ((TTrie*)p->info)->c) {
			prefix[level] = word[level];
			prefix[level + 1] = 0;
			return tfindprefix(p->info, word, level + 1, prefix);
		}
		if (word[level] < ((TTrie*)p->info)->c)
			return 0;
	}
	return 0;
}

int tprefix(TTrie *trie, char *prefix, int level, FILE *out)
{
	TLista p;
	if (!trie)
		return -1;
	if (level >= strlen(prefix)) {
		tprint(trie, prefix, level, out);
		fprintf(out, "\n");
		return 1;
	}
	for (p = trie->fii; p != NULL; p = p->urm) {
		if (prefix[level] == ((TTrie*)p->info)->c)
			return tprefix(p->info, prefix, level + 1, out);
		if (prefix[level] < ((TTrie*)p->info)->c)
			return 0;
	}
	return 0;
}

//calculează suma lungimilor tuturor cuvintelor din (sub)arbore și numărul lor
void tlength(TTrie *trie, int level, int *length, int *wc)
{
	TLista p;
	if (!trie)
		return;
	if (trie->w) {
		(*length) += level + 1;
		(*wc)++;
	}
	for (p = trie->fii; p != NULL; p = p->urm)
		tlength((TTrie*)p->info, level + 1, length, wc);
}

float tfindlength(TTrie *trie, char *prefix, int level)
{
	TLista p;
	if (!trie)
		return 0;
	if (level >= strlen(prefix)) {
		int length = 0, wc = 0;
		tlength(trie, strlen(prefix) - 1, &length, &wc);
		if (wc == 0)
			return 0;
		return (float)length / wc;
	}
	for (p = trie->fii; p != NULL; p = p->urm) {
		if (prefix[level] == ((TTrie*)p->info)->c)
			return tfindlength(p->info, prefix, level + 1);
		if (prefix[level] < ((TTrie*)p->info)->c)
			return 0;
	}
	return 0;
}