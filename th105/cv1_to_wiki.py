#!/usr/bin/env python3

from enum import Enum
import sys

try:
	from th105_cards_list import ignore_cards_list
except ModuleNotFoundError:
	ignore_cards_list = []

class State(Enum):
	START = 1
	SKILL_CARD = 2
	SPELL_CARD = 3

def read_field(text, separator):
	is_in_quote = False
	field_start = 0
	for i in range(0, len(text)):
		if is_in_quote:
			if text[i] == '"':
				is_in_quote = False
			continue
		if text[i] == '"':
			is_in_quote = True
			continue
		if text[i] == separator:
			yield text[field_start:i]
			field_start = i + 1
	if field_start != len(text):
		yield text[field_start:len(text)]

# def split_comment(comment):
# 	out = ""
# 	i = 0
# 	while i < len(comment):
# 		out += comment[i:i + 15] + '\n'
# 		i += 15
# 	return out

# Lines can have either 15 or 16 characters, and are usually separated from
# each other by one or more ideographic space (U+3000) - but not always.
# Try to split on U+3000 first, and if we can't, split assuming 16 characters.
# (because I don't understand why it's sometimes 15 and sometimes 16).
def split_comment(comment):
	# return '\n'.join([x for x in comment.split('\u3000') if x])
	lines = [x for x in comment.split('\u3000') if x]
	lines2 = []
	for line in lines:
		while len(line) > 16:
			lines2.append(line[:16])
			line = line[16:]
		lines2.append(line)
	return '\n'.join(lines2)


state = State.START
char_name = sys.argv[2]
char_name_fix = {
	"udonge": "reisen",
	"chirno": "cirno",
	"meirin": "meiling",
}
if char_name in char_name_fix:
	char_name_mw = char_name_fix[char_name]
else:
	char_name_mw = char_name

char_name_full = {
	"common": "Common",
	"reimu": "Reimu Hakurei",
	"marisa": "Marisa Kirisame",
	"sakuya": "Sakuya Izayoi",
	"alice": "Alice Margatroid",
	"patchouli": "Patchouli Knowledge",
	"youmu": "Youmu Konpaku",
	"remilia": "Remilia Scarlet",
	"yuyuko": "Yuyuko Saigyouji",
	"yukari": "Yukari Yakumo",
	"suika": "Suika Ibuki",
	"reisen": "Reisen Udongein Inaba",
	"aya": "Aya Shameimaru",
	"komachi": "Komachi Onozuka",
	"iku": "Iku Nagae",
	"tenshi": "Tenshi Hinanawi",
	"sanae": "Sanae Kochiya",
	"cirno": "Cirno",
	"meiling": "Hong Meiling",
	"utsuho": "Utsuho Reiuji",
	"suwako": "Suwako Moriya",
	"namazu": "Giant Catfish",
}[char_name_mw]

with open(sys.argv[1], 'r', encoding="shift_jis") as fd:
	file = fd.read()
	for line in read_field(file, '\n'):
		line = line.strip('\r\n')
		line = [item.strip('"') for item in read_field(line, ',')]
		if line[0][0] == '#':
			continue
		id = line[0]
		name = line[1]
		comment = split_comment(line[4])
		id_full    = "{}-{}".format(char_name,    id)
		id_full_mw = "{}-{}".format(char_name_mw, id)

		if state == State.START:
			print("{{Spell Card/Section|{{int|Skill cards}}}}")
			state = State.SKILL_CARD
		if state == State.SKILL_CARD and int(id) >= 200:
			print("{{Spell Card/Section|{{int|Spell cards}}}}")
			state = State.SPELL_CARD

		if id_full    in ignore_cards_list and ignore_cards_list[id_full]    == name:
			continue
		if id_full_mw in ignore_cards_list and ignore_cards_list[id_full_mw] == name:
			continue

		print("{{{{Spell Card|id={}|name=<translate><!--T:{}-->".format(id_full, id_full_mw))
		print("{}</translate>|owner={}|comment_1=<translate><!--T:{}-comment-->".format(name, char_name_full, id_full_mw))
		print("{}</translate>}}}}".format(comment))
