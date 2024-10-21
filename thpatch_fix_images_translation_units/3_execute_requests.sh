#!/bin/bash

browser=/mnt/c/Program\ Files/Mozilla\ Firefox/firefox.exe

source ../mwapi/api_helpers.sh

while read line; do
    echo "Updating $line"
    thpatch_edit "$line" "$(echo "$line" | tr '/ ' '__')_content_fixed.txt"
done < req_edit.txt

browser_args=
while read line; do
    browser_args="$browser_args https://www.thpatch.net/w/index.php?title=Special:PageTranslation&target=$(echo "$line" | sed -e 's|/|%2F|g' | tr ' ' '+')&do=mark"
done < req_edit.txt
if [ "$browser_args" != "" ]; then
    "$browser" $browser_args
    read -p "Please mark these pages for translation then press Enter..." -s
fi

while read line; do
    src=$(echo "$line" | cut -d'|' -f1)
    dst=$(echo "$line" | cut -d'|' -f2)
    echo "$src => $dst"
    thpatch_move "Translations:$src" "Translations:$dst"
done < req_move.txt
