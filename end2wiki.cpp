#include <Windows.h>
#include <stdio.h>
#include <string>

int wmain(int argc, wchar_t **argv) {
	if (argc < 4) {
		wprintf(L"Insufficient parameters.\nend2wiki.exe [end_file] [output] [title]\n");
	}

	HANDLE h_end_file = CreateFileW(
		argv[1],
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (!h_end_file) {
		wprintf(L"File %s not found\n", argv[1]);
		return 1;
	}
	size_t end_file_size = GetFileSize(h_end_file, NULL);
	char *end_file = (char*)malloc(end_file_size);
	DWORD byte_ret;
	ReadFile(h_end_file, end_file, end_file_size, &byte_ret, NULL);
	CloseHandle(h_end_file);

	size_t lc = 0;
	size_t advanced_bytes = 0;
	
	std::wstring w_wiki_file = L"<languages />\n<translate><!--T:-1-->\n<!--Optional message you want to include at the top of the page--></translate>\n{{thcrap Patch File|"; // I'm lazy
	w_wiki_file.append(argv[1]);
	w_wiki_file.append(L"}}\n<translate>");
	w_wiki_file.append(argv[3]);
	w_wiki_file.append(L"<!--T:0--></translate>\n{{dt/Header}}\n{{dt/color|404060}}");
	
	for (;;) {
		if (*end_file != '@') {
			std::wstring lc_w = std::to_wstring(lc);

			w_wiki_file.append(L"{{tt|nochar|code=");
			w_wiki_file.append(lc_w);
			w_wiki_file.append(L"|tl=<translate><!--T:");
			w_wiki_file.append(lc_w);
			w_wiki_file.append(L"-->");

			do {
				w_wiki_file.push_back('\n');
				size_t line_len_w = MultiByteToWideChar(
					932,
					MB_PRECOMPOSED,
					end_file,
					-1,
					NULL,
					0
				);
				wchar_t *line_w = (wchar_t*)malloc(line_len_w * sizeof(wchar_t));
				MultiByteToWideChar(
					932,
					MB_PRECOMPOSED,
					end_file,
					-1,
					line_w,
					line_len_w
				);

				w_wiki_file.append(line_w);

				register size_t end_line_advance = strlen(end_file) + 2;
				advanced_bytes += end_line_advance;
				end_file += end_line_advance;
				lc++;
				free(line_w);
			} while (end_file[0] != '@');
			w_wiki_file.append(L"</translate>}}\n");
		} else {
			if (end_file[1] == 'z' || end_file[1] == 'F') {
				break;
			}
			register char *end_file_advanced = (char*)memchr(end_file, '\n', end_file_size - advanced_bytes) + 1;
			advanced_bytes += end_file_advanced - end_file;
			end_file = end_file_advanced;
			lc++;
		}
	}
	w_wiki_file.append(L"{{{dt/Footer}}\n{{SubpageCategory}}\n{{LanguageCategory|endings}}");
	
	HANDLE h_output = CreateFileW(
		argv[2],
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	WriteFile(h_output, w_wiki_file.c_str(), w_wiki_file.length() * sizeof(wchar_t), &byte_ret, NULL);
	return 0;
}