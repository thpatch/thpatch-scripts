#!/usr/bin/env python3

import sys

if len(sys.argv) != 3:
    print(f"Usage: {sys.argv[0]} game_id trophy.txt")
    print("Note: the file must be in utf-8")
    sys.exit(1)

game_id = sys.argv[1]
print("""\
<languages />
{{#vardefine:game|""" + game_id + """}}
<translate>
<!--T:optional message-->
<!-- Optional message you want to include at the top of the page --></translate>
{{thcrap Game File|trophy.js}}
{{Trophy/Header}}
""")

with open(sys.argv[2], "r") as file:
    lines = file.readlines()

i = 0
while True:
    while i < len(lines) and lines[i][0] != "@":
        i += 1
    if i >= len(lines):
        break
    id_ = lines[i][1:].strip()
    name = lines[i + 1].strip()
    locked = "".join(lines[i + 3:i + 6]).strip()
    unlocked = "".join(lines[i + 7:i + 10]).strip()

    print(f"""\
{{{{Trophy|id={id_}|title=<translate>
<!--T:{id_} name-->
{name}</translate>
|locked=<translate><!--T:{id_} locked--> {locked}</translate>
|unlocked=<translate>
<!--T:{id_} unlocked-->
{unlocked}</translate>}}}}
""")

    i += 10

print("""\
{{Trophy/Footer}}
{{SubpageCategory}}
{{LanguageCategory}}
""")
