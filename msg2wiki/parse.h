// Touhou Patch Parsechain
// -----------------------
// parse.h - general definitions
// -----------------------
// "©" Nmlgc, 2012

#include <windows.h>
#include <malloc.h>
#include <stdio.h>

#ifdef WIN32
#define snprintf _snprintf
#endif

#define MIN(a, b)				((a) < (b) ? (a) : (b))
#define MAX(a, b)				((a) > (b) ? (a) : (b))

const unsigned char utf8bom[3] = {0xEF, 0xBB, 0xBF};

// target language
extern char* target_lang;

// initial translation number
extern size_t trans_block_num;

// Stage number
extern size_t stage_num;

// process function prototype
int Process(char* fn, char* str, bool utf8);