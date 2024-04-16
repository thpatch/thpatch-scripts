#!/usr/bin/env python3

# Touhou Community Reliant Automatic Patcher
# Scripts
#
# ----
#
"""Generates a Touhou Patch Center spell card page from the output of
xargs grep across the Touhou Toolkit ECL dumps of a game."""

import re
import argparse
import pdb

###
# Regex
###
sc_re = re.compile(r'(?P<fn>.*):.*\((?P<id>\d+),.*"(?P<name>.+)"');
sc_th08_re = re.compile(r'(?P<fn>.*):.*\(\d+, (?P<id>\d+), \d+, "(?P<name>.+)", "(?P<owner>.+)", "(?P<line1>.+)", "(?P<line2>.+)"');

###
# argparse
###

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('in_fn',
    help='xargs grep output',
    type=str)

###
# Entry point
###

if __name__ == '__main__':
    arg = parser.parse_args()

    in_file = open(arg.in_fn, encoding="utf-8");

    print("{{Spell Card/Header}}");

    prev_fn = ""
    
    for i in in_file:
        try:
            sc = sc_th08_re.match(i).groupdict()
        except:
            sc = sc_re.match(i).groupdict()
        sc_owner = sc.get('owner', None)
        try:
            sc_cmt = [ sc['line1'], sc['line2'] ]
        except:
            sc_cmt = None
        sc_id = int(sc['id']) + 1
        if sc['fn'] != prev_fn:
            print("\n{{Spell Card/Section|%s}}" % sc['fn'])
            prev_fn = sc['fn']
        
        print(
            "{{Spell Card|id=%d|name=<translate>%s <!--T:%d--></translate>|owner=%s" %
            (sc_id, sc['name'], sc_id, sc_owner if sc_owner else '$%s_long_name$' % sc['fn']),
            end=''
        )
        if sc_cmt:
            print("|comment1=<translate><!--T:%d comment-->\n%s\n%s</translate>" %
                  (sc_id, sc_cmt[0], sc_cmt[1]),
                  end=''
                  )
        print("}}")
    print("{{Spell Card/Footer}}")
