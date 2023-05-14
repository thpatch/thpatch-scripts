// Touhou Patch Parsechain
// -----------------------
// msg2wiki.h - converts thmsg dumps to wiki code
// -----------------------
// "©" Nmlgc, 2012

static const unsigned int MAX_LINE = 256;

enum msg_side
{
	SIDE_UNKNOWN = -1,
	SIDE_P1 = 0,
	SIDE_P2 = 1,
	SIDE_B1 = 2,
	SIDE_B2 = 3
};

// parsing state
struct msg_state
{
	char* fn;	// message file name
	int entry;	// entry number
	int time;	// time_code;
	int char_id;
	// th19 stuff
	int player;
	int opponent;
	int message_no;
};
// furigana
struct msg_furi
{
	int	text_x_offset;
	int	char_spacing;
	wchar_t	str[32];
};

// Base table structure
struct Table
{
protected:
	bool SetLine_Base(int linenum, const wchar_t* str);
	bool RenderAndClose_Base(const wchar_t* template_name, const wchar_t* add_params = L"");
public:
	int time;	// timecode of first line
	const wchar_t* type;	// template subtype

	wchar_t line[4][MAX_LINE];
	
	virtual bool SetLine(int linenum, const wchar_t* str);
	virtual bool RenderAndClose();
};

struct DialogTable : public Table
{
protected:
	bool MergeFuri(int linenum);	// Merge furigana information w/ given line

public:
	msg_side side[2];	// Array only for those few instances where both characters speak in the same box -.-
	wchar_t assist[16];
	msg_furi furi;	// furigana information
	wchar_t add_param[32];	// Additional template parameters

	DialogTable()	{side[0] = side[1] = SIDE_UNKNOWN;	add_param[0] = 0;}
	
	bool SetLine(int linenum, msg_side sideflag, const wchar_t* str);
	bool SetLine(const wchar_t* str);	// sets the next line

	bool SetSide(msg_side new_side);
	bool RenderAndClose();
};
