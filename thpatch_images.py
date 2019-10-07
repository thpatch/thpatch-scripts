#!/usr/bin/env python3

# Touhou Patch Center
# Scripts
#
# ----
#
"""Builds an image page from thanm dumps."""

import argparse
import os
import pdb
import shutil

parser = argparse.ArgumentParser(
    description=__doc__
)
parser.add_argument(
    'f',
    help='Root path of the extracted data.'
)

parser.add_argument(
    '-t', '--to',
    help='Path that receives all files to be uploaded to Touhou Patch Center.',
    dest='t',
    default=None
)

parser.add_argument(
    '-g', '--game',
    help='Game ID.',
    required=True
)

if __name__ == '__main__':
    arg = parser.parse_args()
    # Header
    print("""<languages /><div style="float:right;">__TOC__</div>
{{#vardefine:game|%s}}
<translate><!--T:0-->
<!-- Optional message you want to include at the top of the page --></translate>"""
    % arg.game)
    for root, dirs, files in os.walk(arg.f):
        for fn in files:
            if (fn.endswith('.png') or fn.endswith('.jpg') or fn.endswith('.gif')) and not fn.startswith('bounds-'):
                f_fn = os.path.join(root, fn)
                game_fn = f_fn[len(arg.f) + 1:].replace('\\', '/')
                wiki_fn = arg.game + "-" + game_fn.replace('/', '-')
                if arg.t:
                    t_fn = os.path.join(arg.t, wiki_fn)
                    bounds_fn = 'bounds-' + wiki_fn
                    f_bounds_fn = os.path.join(arg.f, bounds_fn)
                    t_bounds_fn = os.path.join(arg.t, bounds_fn)
                    shutil.copy2(f_fn, t_fn)
                    try:
                        shutil.copy2(f_bounds_fn, t_bounds_fn)
                    except FileNotFoundError:
                        pass
                else:
                    t_fn = None
                print("""{{thcrap Image
|target=%s
|transcription=<translate><!--T:%s-->
Please add a transcription by editing the corresponding section on the root page ([[%s]])!
</translate>
}}""" % (game_fn, game_fn, arg.game + "/Images")
    )
        
    # Footer
    print("{{SubpageCategory}}\n(Don't forget to set the .anm data format in the main .js file!)")