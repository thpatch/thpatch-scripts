// Touhou Patch Parsechain
// -----------------------
// main.cpp - OS/init handling & file loading
// -----------------------
// "©" Nmlgc, 2012-2017

#include <parse.h>
#include <shlwapi.h>

size_t trans_block_num = 1;
size_t stage_num = 1;
bool current_file_has_3_entries = false;
bool is_ending = false;

int main(int argc, char *argv[])
{
	FILE* in = NULL;
	bool utf8 = false;

	// check if an argument was given
	if(argc < 2)	return -1;

	fwrite(utf8bom, 3, 1, stdout);
	printf(
		"<languages />\n"
		"<translate><!--T:0-->\n"
		"<!--Optional message you want to include at the top of the page--></translate>\n"
	);

	for(stage_num = 1; stage_num < argc; stage_num++) {
		in = fopen(argv[stage_num], "r");
		if(in) {
			char* fn;
			char line[2048];
			char actual_fn[256];

			current_file_has_3_entries = false;

			fn = PathFindFileNameA(argv[stage_num]);
			strcpy(actual_fn, fn);
			PathRenameExtensionA(actual_fn, "");

			is_ending = (actual_fn[0] == 'e');

			// Skip byte order mark, if necessary
			fread(line, 3, 1, in);
			utf8 = memcmp(line, utf8bom, 3) == 0;
			if(!utf8)	fseek(in, 0, SEEK_SET);

			printf("{{thcrap Patch file|%s}}\n", actual_fn);
			printf("<translate>==Stage %d== <!--T:%d--></translate>\n", stage_num, trans_block_num++);

			while(fgets(line, 2048, in)) {
				int len = strlen(line) - 1;

				while(line[len] == 0x0D || line[len] == 0x0A) {
					line[len] = 0;
					len--;
				}

				if(Process(fn, line, utf8))	break;
			}
			fclose(in);
		}
	}

	printf(
		"{{SubpageCategory}}\n"
		"{{LanguageCategory|%s}}\n",
		is_ending ? "endings" : "story"
	);

	return 0;
}