#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <stdint.h>
#include <io.h>
char *readline(FILE *f) {
	static char buf[512];
	int c;
	int i = 0;
	while (i < 512 && (c = fgetc(f), c != '\n' && c != -1)) {
		buf[i++] = c;
	}
	buf[i] = 0;
	if (i == 512) return NULL;
	return buf;
}
typedef struct _tag_entry{
	char *name;
	uint32_t off;
	uint32_t len;
	struct _tag_entry *next;
} Entry;

#define READ_FOURCC() do { \
	if(4 != fread(fourcc, 1, 4, f)) { \
		fprintf(stderr, "Error in file %s: read error at 0x%X\n", ent->name, ftell(f)); \
		goto err; \
	} \
} while(0)
#define CHECK_FOURCC(cc) do { \
	READ_FOURCC(); \
	if(memcmp(fourcc, cc, 4)) { \
		fprintf(stderr, "Error in file %s: fourcc mismatch at 0x%X, expected '%s'\n", ent->name, ftell(f), cc); \
		goto err; \
	} \
} while(0)
#define READ_INT() do { \
	if(1 != fread(&num, 4, 1, f)) { \
		fprintf(stderr, "Error in file %s: read error at 0x%X\n", ent->name, ftell(f)); \
		goto err; \
	} \
} while(0)
#define CHECK_INTEGER(chk) do { \
	READ_INT(); \
	if(num != chk) { \
		fprintf(stderr, "Error in file %s: integer mismatch at 0x%X, expected 0x%08X\n", ent->name, ftell(f), chk); \
		goto err; \
	} \
} while(0)
#define WRITE_INT_AT(wher, what) do { \
	fseek(f, wher, SEEK_SET); \
	if(1 != fwrite(&what, 4, 1, f)) { \
		fprintf(stderr, "Error in file %s: write error at 0x%X\n", ent->name, ftell(f)); \
		goto err; \
	} \
} while(0)
void fixFile(Entry *ent, FILE* readback) {
	FILE *f = fopen(ent->name, readback?"rb":"rb+");
	if (!f) {
		fprintf(stderr, "Error in file %s: couldn't open\n", ent->name);
		return;
	}
	char fourcc[4];
	uint32_t num;

	CHECK_FOURCC("RIFF");
	READ_INT();
	CHECK_FOURCC("SFPL");
	CHECK_FOURCC("cue ");
	READ_INT();
	int cuecount = num;
	READ_INT();
	int elements = num;
	if (cuecount != 4 + 24 * elements) {
		fprintf(stderr, "Error in file %s: object count doesn't match up with chunk size\n", ent->name);
		goto err;
	}
	if (elements != 1) {
		fprintf(stderr, "Error in file %s: multiple ranges\n", ent->name);
		goto err;
	}
	READ_INT();
	int id = num;
	long firstpos = ftell(f);
	READ_INT();
	uint32_t oldoff = num;
	CHECK_FOURCC("data");
	CHECK_INTEGER(0);
	CHECK_INTEGER(0);
	long secondpos = ftell(f);
	READ_INT();
	CHECK_FOURCC("LIST");
	READ_INT();
	CHECK_FOURCC("adtl");
	CHECK_FOURCC("ltxt");
	CHECK_INTEGER(20);
	CHECK_INTEGER(id);
	long len = ftell(f);
	READ_INT();
	uint32_t oldlen = num;
	CHECK_FOURCC("rgn ");
	CHECK_INTEGER(0);
	CHECK_INTEGER(0);
	if (readback) {
		fprintf(readback,"%s,%d,%d\n",ent->name, oldoff, oldoff + oldlen);
	}
	else {
		WRITE_INT_AT(firstpos, ent->off);
		WRITE_INT_AT(secondpos, ent->off);
		WRITE_INT_AT(len, ent->len);
	}
	fprintf(stderr, "Success! %s\n", ent->name);
err:
	fclose(f);
}
int edit() {
	int linecount = 0;
	Entry *first = NULL, **pnew = &first;

	FILE *info = fopen("info.csv", "r");
	if (!info) {
		fprintf(stderr, "Couldn't find info.csv\n");
		goto fatal;
	}
	while (!feof(info)) {
		++linecount;
		char *line = readline(info);
		if (!line) {
			fprintf(stderr, "Error at line %d: line too long.", linecount);
			fclose(info);
			goto fatal;
		}
		
		if (!line[0]) continue;
		char *off = strchr(line, ',');
		if (!off) {
			fprintf(stderr, "Warning at line %d: expected comma", linecount);
			continue;
		}
		*off++ = '\0';
		char *len = strchr(off, ',');
		if (!off) {
			fprintf(stderr, "Warning at line %d: expected comma", linecount);
			continue;
		}
		*len++ = '\0';

		// make entry
		Entry *cur = malloc(sizeof(Entry));
		if (!cur) {
			fprintf(stderr, "Error at line %d: out of memory", linecount);
			goto fatal;
		}
		cur->name = _strdup(line);
		if (!cur->name) {
			free(cur);
			fprintf(stderr, "Error at line %d: out of memory", linecount);
			goto fatal;
		}
		cur->off = strtoul(off, NULL, 10);
		cur->len = strtoul(len, NULL, 10) - cur->off;
		cur->next = NULL;

		*pnew = cur;
		pnew = &cur->next;
	}
	fclose(info);
	for (Entry *next=first; next; next = next->next) {
		fixFile(next, NULL);
	}
fatal:
	while (first) {
		Entry *next = first->next;
		free(first->name);
		first = next;
	}
	fprintf(stderr,"Press any key to continue...\n");
	_getch();
	return 0;
}
int readback() {
	FILE *f = fopen("out.csv", "w");
	if (!f) {
		fprintf(stderr, "Couldn't open info.csv for writing\n");
		goto fatal;
	}
	Entry e = { 0 };
	struct _finddata_t fd;
	intptr_t find = _findfirst("*.sfl", &fd);
	if (find) {
		do {
			e.name = fd.name;
			fixFile(&e, f);
		} while (!_findnext(find, &fd));
		_findclose(find);
	}
	fclose(f);
fatal:
	fprintf(stderr, "Press any key to continue...\n");
	_getch();
	return 0;
}
int main(int argc, char **argv) {
	if (argc == 2 && !strcmp(argv[1], "extract")) {
		return readback();
	}
	else if (argc == 1) {
		return edit();
	}
	else {
		fprintf(stderr, "Invalid arguments\nPress any key to continue...\n");
		_getch();
		return 1;
	}
}