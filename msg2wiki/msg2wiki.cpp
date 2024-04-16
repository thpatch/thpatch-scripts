// Touhou Patch Parsechain
// -----------------------
// msg2wiki.cpp - converts thmsg dumps to wiki code
// -----------------------
// "©" Nmlgc, 2012-2017

#include <parse.h>
#include <math.h>
#include <Shlwapi.h>
#include "msg2wiki.h"

const wchar_t* side_desc[] = {L"$Player$", L"$Assist$", L"$Boss1$", L"$Boss2$"};
const wchar_t* dialog_template = L"dt";
const wchar_t* ruby_template = L"ruby-ja";
static const wchar_t OpenBrackets[5] = {0x300c, L'\'', 0xFF08, L'(', 0};
static const wchar_t CloseBrackets[5] = {0x300d, L'\'', 0xFF09, L')', 0};

// Boss name knowledge
int boss_known = 0;

// Boss presence counter
int boss_presence = 0;

// Detected game version
float version = -1.0f;

float FONT_WIDTH = 16.0f;

extern bool current_file_has_3_entries;
extern bool is_ending;

msg_state ms;

float SetVersion(const float new_ver)
{
	if(new_ver > version) version = new_ver;
	return version;
}

wchar_t* GetSideDesc(wchar_t* str, int entry, msg_side side)
{
	if(side >= SIDE_B1)
	{
		wsprintf(str, L"$Boss#%d_%d_%d$", stage_num, entry, side);
	}
	else
	{
		wcscpy(str, side_desc[side]);
	}
	return str;
}

// Base table structure
// --------------------
bool Table::SetLine_Base(int num, const wchar_t* str)
{
	if(!str)	return 1;
	if(line[num][0] != 0)	RenderAndClose();
	wcscpy(line[num], str);

	if(!num)	time = ms.time;
	return wcslen(line[num]) == 0;
}

bool Table::RenderAndClose_Base(const wchar_t* dialog_template, const wchar_t* add_params)
{
	int i = 0;
	char lineconv[1024];
	wchar_t prolog[1024];

	if(line[0][0] == 0 && line[1][0] == 0)	return 1;

	wsprintf(prolog, L"{{%s%s|code=#%d@%d|tl=", dialog_template, add_params, ms.entry, time);

	WideCharToMultiByte(CP_UTF8, 0, prolog, -1, lineconv, 1024, NULL, NULL);

	printf(lineconv);

	for(i = 0; i < 4; i++) {
		if(line[i][0]) {
			WideCharToMultiByte(CP_UTF8, 0, line[i], -1, lineconv, 1024, NULL, NULL);	
			if(i != 0) {
				printf("\n");
			}
			printf(lineconv);
			line[i][0] = 0;
		}
	}
	printf("</translate>}}\n");
	return (line[0][0] = line[1][0] = 0) != 0;
}

bool Table::SetLine(int linenum, const wchar_t* str)
{
	if(SetLine_Base(linenum, str))	return 1;
	// if(line[1][0] == 0)	return RenderAndClose(); // ?
	// if(line[1][0] != 0)	return RenderAndClose(); // !
	return 0;
}

bool Table::RenderAndClose()
{
	if(type[0] == 0)
	{
		return RenderAndClose_Base(dialog_template);
	}
	else
	{
		size_t str_len = (wcslen(dialog_template) + wcslen(type) + 2);
		wchar_t* str;
		str = (wchar_t*)alloca(str_len * sizeof(wchar_t));

		wsprintf(str, L"%s|%s", dialog_template, type);
		return RenderAndClose_Base(str);
	}
}
// --------------------

bool DialogTable::SetLine(int linenum, msg_side sideflag, const wchar_t* str)
{
	if(SetLine_Base(linenum, str))	return 1;
	// side flag
	side[linenum] = sideflag;
	if(linenum)	RenderAndClose();
	return 0;
}

bool DialogTable::SetLine(const wchar_t* str)
{
	bool ret;
	int linenum;
	wchar_t set_str[1024];
	if(!str)	return 1;

	set_str[0] = 0;

	for(linenum = 0; linenum < 4; linenum++) {
		if(line[linenum][0] == 0) {
			break;
		}
	}
	if(linenum == 4) {
		linenum = 0;
	}
	
	if(linenum == 0) {
		wchar_t side[128];
		GetSideDesc(side, ms.entry, this->side[linenum]);
		if(this->side[linenum] == SIDE_P2) {
			wcscpy(side, this->assist);
		}
		wsprintf(
			set_str, L"<translate><!--T:%d%s%s-->\n", trans_block_num++,
			side[0] != '\0' ? L"_" : L"", side
		);
	}
	wcscat(set_str, str);

	ret = SetLine_Base(linenum, set_str);
	// Merge furigana with text
	if(!ret)	return MergeFuri(linenum);
	else    	return ret;
}

bool DialogTable::SetSide(msg_side new_side)
{
	this->side[0] = this->side[1] = new_side;
	return 0;
}

bool DialogTable::MergeFuri(int num)
{
	float furi_utf8len;
	int str_furistart;
	int str_furispan;
	int c = 0;

	wchar_t temp_line[MAX_LINE];
	wchar_t str_base[MAX_LINE];
	
	if(!furi.str[0] || !line[num][0])	return 1;

	furi_utf8len = (float)wcslen(furi.str);
	
	// needs to be integer division to round off beginning char
	str_furistart = (int)(furi.text_x_offset / FONT_WIDTH);
	if(assist[0])
	{
		str_furistart -= wcslen(assist) + 1;
	}
	c = str_furistart;

	str_furispan = (int)ceilf( ((furi_utf8len * furi.char_spacing) - (FONT_WIDTH / 8)) / FONT_WIDTH);

	// Single out base string
	wcsncpy(str_base, line[num] + str_furistart, str_furispan);
	str_base[str_furispan] = 0;

	wcsncpy(temp_line, line[num], MAX_LINE);

	c += swprintf(line[num] + c, MAX_LINE, L"{{%s|%s|%s}}", ruby_template, str_base, furi.str);
	wcscpy(line[num] + c, &temp_line[str_furistart + str_furispan]);
	
	return (furi.str[0] = 0) != 0;
}

// FIXME: Convert to printf format and call from base function. It's too stupid this way.
//        Oh, and fix this damn dialog_template system to work with assist...
bool DialogTable::RenderAndClose()
{
	wchar_t str[MAX_PATH];
	int len = 0;

	if(side[1] == SIDE_UNKNOWN)
	{
		if(side[0] == SIDE_UNKNOWN)	return 0;
		side[1] = side[0];
	}

	if(side[0] == side[1])
	{
		if(assist[0] == 0)
		{
			if(is_ending) {
				len += wnsprintf(str, 256, L"|nochar");
			} else {
				len += wnsprintf(str, 256, L"|side=%d|char=", side[0] >= SIDE_B1);
				GetSideDesc(str + len, ms.entry, side[0]);
			}
		}
		else
		{
			if(is_ending) {
				wnsprintf(str, 256, L"|tabchar|char=%s", assist);
			} else {
				wnsprintf(str, 256, L"|assist|side=%d|char=%s",
					side[0] >= SIDE_B1, assist);
			}

			assist[0] = 0;
		}
	}
	else
	{
		len = wnsprintf(str, 256, L"|char=%s<br/>", side_desc[side[0]]);
		GetSideDesc(str + len, ms.entry, side[1]);
	}
	wcscat(str, add_param);
	return RenderAndClose_Base(dialog_template, str);
}

bool BossTitleDummy()
{
	boss_known = true;
	wprintf(L"{{%s|h1|tl=<Boss title>}}\n", dialog_template);
	return 0;
}

bool BossLongMsg(const wchar_t* msg)
{
	if(is_ending) {
		return 0;
	}
	wprintf(L"{{%s/status_iw|status=story_char_%s|$BossLong#%d_%d", dialog_template, msg, stage_num, ms.entry);
	if(version == 9)
	{
		printf(" #%02d", ms.char_id);
	}
	printf("$}}\n");
	return 0;
}

bool BossEnter()
{
	boss_presence++;
	return BossLongMsg(L"enter");
}

bool BossLeave()
{
	boss_presence--;
	return BossLongMsg(L"exit");
}

bool BossDefeat()
{
	if(boss_presence > 0)
	{
		bool ret = BossLongMsg(L"defeat");
		boss_presence--;
		return ret;
	}
	return boss_presence != 0;
}

bool BGMChange(int song_id = 0)
{
	if(is_ending) {
		return 0;
	}
	if(song_id == -1)	wprintf(L"{{%s/bgm|none}}\n", dialog_template);
	// else if(song_id==0)			wprintf(L"{{%s/bgm|th%02d_%02d}}\n", dialog_template, (int)version, song_id);
	else if(song_id==0)			wprintf(L"{{%s/bgm|th16_%02d}}\n", dialog_template, stage_num * 2 + 1);
	return 0;
}

bool SelectStage()
{
	wprintf(L"{{%s|st_desc\n|ja=<Stage selection>\n|tl=<Stage selection>\n}}\n", dialog_template);
	return 0;
}

bool CloseTable(bool table, int entry)
{
	if(table)
	{
		if(entry == 1 || is_ending) {
			wprintf(L"{{%s/Footer}}\n", dialog_template);
		}
		table = false;
	}
	return table;
}

char* trim_begin(char* str)
{
	char* r = str;
	while( (r[0] == ' ' || r[0] == '\t') && r[0] != 0)	r++;
	return r;
}

wchar_t* trim_begin(wchar_t* str)
{
	wchar_t* r = str;
	// 0x3000 = full-width space
	while( (r[0] == L' ' || r[0] == 0x3000 || r[0] == L'\t') && r[0] != 0)	r++;
	return r;
}

// Main loop
int Process(char* fn, char* str, bool utf8)
{
	// Dialog table w/ "hard" line access. Used by th06, th07, and with a few lines of th08.
	static DialogTable dt_hard;

	// Dialog table w/ automatic ("soft") line assignment. Used by most of th08, and the entirety of every later game.
	// opcodes #15 and #17 create a new text box
	static DialogTable dt_soft; 
	static Table st_desc;	  	// Character title table

	static bool table = false;	// wiki table open?

	// h1.type = L"h1";
	st_desc.type = L"st-desc";
	// ------

	wchar_t str_utf16[256];

	char* temp_str;
	wchar_t* temp_utf16;

	str = trim_begin(str);
	ms.fn = fn;

	// Parse entry number
	if(!strncmp("entry", str, 5))
	{
		int new_char = -1;
		char* str_entry = strtok(str, " ");
		if(str_entry)
		{
			ms.entry = atoi(strtok(NULL, str_entry));
			new_char = ms.entry / 10;
		}

		if(ms.char_id != new_char)	boss_known = false;

		if(9 == version && strstr(fn, "match"))
		{
			if(ms.char_id != new_char)
			{
				table = CloseTable(table, ms.entry);
				printf("==%s defeats $Char%02d$==\n{{%s/Header}}\n", side_desc[SIDE_P1], new_char, dialog_template);
			}
			printf("{{%s|status|status=Message #%d}}\n", dialog_template, (ms.entry % 10) + 1);
		}
		else if(ms.entry == 2)
		{
			printf("{{dt/Header}}\n");
			current_file_has_3_entries = true;
		}
		else if(ms.entry == 0 && !current_file_has_3_entries) {
			printf("{{dt/Header}}\n");
		}
		ms.char_id = new_char;
		table = true;
		ms.time = -1;
		dt_soft.add_param[0] = 0;
		return 0;
	}

	// Parse timecode
	if('@' == str[0])
	{
		ms.time = atoi(&str[1]);
		dt_soft.RenderAndClose();
		return 0;
	}

	const int token_count = 4;

	int op, i, c;
	msg_side side;

	char* param[token_count] = {NULL, NULL, NULL, NULL};

	param[0] = (char*)alloca(strlen(str) + 1);
	strcpy(param[0], str);

	strtok(param[0], ";");
	for(i = 1; i < token_count; i++)
	{
		param[i] = strtok(NULL, ";");
	}

	// Parse opcode
	op = atoi(param[0]);
	switch(op)
	{
	case 0:
		BossDefeat();
		dt_hard.RenderAndClose();
		dt_soft.RenderAndClose();
		if( !(9 == version && strstr(fn, "match")) ) table = CloseTable(table, ms.entry);
		break;

	// th128 Char Bank Switching
	case 2:
		if(param[1] && version > 12.8f)
		{
			ms.char_id = atoi(param[1]);
		}
		break;

	// th06 / th07:
	case 3:
		// No params? -> th10
		if(!param[1])	SetVersion(10);
		if(!param[2])	{
			// Ending text

			// we need to do this to catch *everything*, text may contain a parameter separator too.
			temp_str = strchr(str, ';') + 1;

			MultiByteToWideChar(utf8 ? CP_UTF8 : 932, 0, temp_str, strlen(temp_str) + 1, str_utf16, 256);

			dt_soft.SetSide(SIDE_P2);

			if(dt_soft.assist[0] == 0)
			{
				bool has_char_name = false;

				temp_utf16 = wcspbrk(str_utf16, OpenBrackets);

				if(temp_utf16) {
					auto text_dist = (temp_utf16 - str_utf16);
					has_char_name = text_dist > 0 && text_dist <= 5;
				}
				if(has_char_name)
				{
					c = temp_utf16 - str_utf16;
					wcsncpy(dt_soft.assist, str_utf16, c);
					
					dt_soft.assist[c] = 0;
				}
				else
				{
					temp_utf16 = str_utf16;
				}
			}
			else
			{
				temp_utf16 = trim_begin(str_utf16);
			}
			c = wcslen(temp_utf16) - 1;
			if(temp_utf16[c] == CloseBrackets[0] || temp_utf16[c] == CloseBrackets[1])
			{
				temp_utf16[c + 1] = 0;
			}

			dt_soft.SetLine(temp_utf16);

			SetVersion(100);
			return 0;
		} else {
			side = msg_side(atoi(param[1]));
			// This opcode is never used for assist lines anyway.
			if(SIDE_P2 == side)	side = SIDE_B1;

			temp_str = &str[param[3] - param[0]];
			MultiByteToWideChar(utf8 ? CP_UTF8 : 932, 0, temp_str, strlen(temp_str) + 1, str_utf16, 256);
			return dt_hard.SetLine(atoi(param[2]) != 0, side, str_utf16);
		}

	case 1:
		if( !(version == 9 && atoi(param[1]) == 1) )	break;

	case 6:
		// TH10+ endings: Next page
		if(is_ending && version >= 10) {
			wprintf(L"{{%s|h1}}\n", dialog_template);
		}
		if(version < 10)	return BossEnter();
		if(version = 100) {
			// Ending page wipe
			dt_soft.RenderAndClose();
			// h1.SetLine(0, L" ");
			// h1.RenderAndClose();
		}
		// fallthrough
	case 5:
		if(version == 100) {
			dt_soft.RenderAndClose();
		}
		break;

	case 7:
		if(!param[1]) 
		{
			SetVersion(10);
			return dt_soft.SetSide(SIDE_P1);
		}
		else			return BGMChange(atoi(param[1]));

	case 8:
		/*if(version >= 10)
		{*/
		if(param[1]) {
			auto char_on_side = atoi(param[1]);
			if(char_on_side == 2) {
				return dt_soft.SetSide(SIDE_B2);
			}
			else {
				return dt_soft.SetSide(SIDE_B1);
			}
		} else {
			return dt_soft.SetSide(SIDE_B1);
		}
		/*}
		else
		{
			if(!param[2])
			{
				// th09 doesn't pass parameters anymore.
				// Just throw out a dummy template
				return BossTitleDummy();
			}
			else
			{
				boss_known = true;
				temp_str = &str[param[3] - param[0]];
				MultiByteToWideChar(utf8 ? CP_UTF8 : 932, 0, temp_str, strlen(temp_str) + 1, str_utf16, 256);
				return h1.SetLine( atoi(param[2]) != 0, str_utf16);
			}
		}*/
	case 9:
		if(param[1])
		{
			// Ending text color
			size_t col = atoi(param[1]);
			col =
				((col & 0xff0000) >> 16) <<  0 |
				((col & 0x00ff00) >>  8) <<  8 |
				((col & 0x0000ff) >>  0) << 16
			;
			dt_hard.RenderAndClose();
			dt_soft.RenderAndClose();
			wprintf(L"{{dt/color|%x}}", col);
		}
		else
		{
			SetVersion(11);
			dt_soft.SetSide(SIDE_P2);
		}
		break;
	
	// th11
	case 11:
		if(param[1])
		{
			SetVersion(11);
			break;
		}
	case 12:
		if(version >= 10 && (!param[1]))
		{
			SetVersion(10);
			return BossEnter();
		}
		break;

	// th08
	case 15:
	case 17:
		if(version < 10)
		{
			dt_soft.RenderAndClose();
			// h1.RenderAndClose();

			side = msg_side(atoi(param[1]));
			if(version == 9)
			{
					 if(side == SIDE_P2)     	side = SIDE_B1;
				else if(side == SIDE_UNKNOWN)	side = SIDE_B2;
			}
			return dt_soft.SetSide(side);
		}
		// Fall through if th10<
	case 16:
		dt_hard.RenderAndClose();
		temp_str = strchr(str, ';') + 1;

		MultiByteToWideChar(utf8 ? CP_UTF8 : 932, 0, temp_str, strlen(temp_str) + 1, str_utf16, 256);
		// Furigana
		if(param[3])
		{
			temp_str = &str[param[3] - param[0]];

			dt_soft.furi.text_x_offset = atoi(strchr(param[1], '|') + 1);
			dt_soft.furi.char_spacing = atoi(param[2]);
			MultiByteToWideChar(utf8 ? CP_UTF8 : 932, 0, temp_str, strlen(temp_str) + 1, dt_soft.furi.str, 32);
			break;
		}
		if(version == 11 && dt_soft.side[0] == SIDE_P2)
		{
			if(dt_soft.assist[0] == 0)
			{
				if(temp_utf16 = wcspbrk(str_utf16, OpenBrackets))
				{
					c = temp_utf16 - str_utf16;
					wcsncpy(dt_soft.assist, str_utf16, c);
					
					dt_soft.assist[c] = 0;
					// Move text pointer behind bracket
					temp_utf16++;
				}
				else
				{
					temp_utf16 = str_utf16;
				}
			}
			else
			{
				temp_utf16 = trim_begin(str_utf16);
			}
			c = wcslen(temp_utf16) - 1;
			if(temp_utf16[c] == CloseBrackets[0] || temp_utf16[c] == CloseBrackets[1])
			{
				temp_utf16[c] = 0;
			}
		}
		else	temp_utf16 = str_utf16;

		dt_soft.SetLine(temp_utf16);
		break;

	case 18:
		if(!param[1] && version < 12.8f)
		{
			version = 10;
			return BGMChange();
		}
		break;

	// Decisions
	case 19:
		// This opcode is shared with th10<'s boss title opcode
		// If a parameter was given, it's th08, if not, it's th10< 
		if(!param[1])
		{
			if(version == 10)	return BossTitleDummy();
			else				return BGMChange();
		}
		// fall through if th08
	case 20:
		// th11 Boss Title
		if(version > 10)
		{
			return BossTitleDummy();
		}
		if(version < 9)
		{
			temp_str = strchr(str, ';') + 1;
			MultiByteToWideChar(utf8 ? CP_UTF8 : 932, 0, temp_str, strlen(temp_str) + 1, str_utf16, 256);
			if(param[1])	return st_desc.SetLine( (op - 19) != 0, str_utf16);
		}

	case 23:
		if(version == 10)	return BossLeave();
		break;

	case 24:
		SetVersion(9.0f);
		break;
	case 25:
		SetVersion(11.0f);
		break;
	case 28:
		SetVersion(12.8f);
		FONT_WIDTH = 18.0f;
		break;

	// th128 textbox frame style
	case 29:
		c = atoi(param[1]);
		if(c)	wcscpy(dt_soft.add_param, L"|shout=y");
		else	dt_soft.add_param[0] = 0;
		break;

	case 30:
		if(version == 12.8f)
		{
			SelectStage();
		}
		break;
	
	// th128 Char Bank Switching
	case 31:
		SetVersion(13.0f);
		ms.char_id = 1;
		break;
	}
	return 0;
}