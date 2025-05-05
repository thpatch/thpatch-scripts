#!/usr/bin/env python3
if __name__ != "__main__":
    quit()

from argparse import ArgumentParser
import json5
import pefile

parser = ArgumentParser()
parser.add_argument("exe_old", help="exe file where stringlocs are already known")
parser.add_argument("exe_new", help="new exe file for which you want to generate stringlocs")
parser.add_argument("stringlocs", help="stringlocs.js file")
args = parser.parse_args()

exe_new = pefile.PE(args.exe_new)
exe_old = pefile.PE(args.exe_old)

m_old = exe_old.get_memory_mapped_image()
m_new = exe_new.get_memory_mapped_image()

with open(args.stringlocs, encoding="UTF-8") as f:
    stringlocs = json5.load(f)

stringlocs_new = {}

for (key, val) in stringlocs.items():
    if key[:2] == "0x":
        rva_old = int(key[2:], 16) - exe_old.OPTIONAL_HEADER.ImageBase
    elif key[:2] == "Rx":
        rva_old = int(key[2:], 16)
    else:
        print("Error: stringlocs specifies address without 0x or Rx. Aborting")
        quit()    

    l = 0
    while m_old[rva_old + l] != 0:
        l += 1

    rva_new = m_new.find(m_old[rva_old:rva_old+l])

    stringlocs_new[f"Rx{rva_new:X}"] = val

print(json5.dumps(stringlocs_new, indent=4))
