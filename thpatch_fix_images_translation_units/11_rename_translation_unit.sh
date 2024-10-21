#!/bin/bash

# Example file:
# Hardcoded_strings/th07_Supernatural_Border|Hardcoded_ASCII_strings/th07_Supernatural_Border
# Hardcoded_strings/th07_CherryPoint_Max|Hardcoded_ASCII_strings/th07_CherryPoint_Max
# Hardcoded_strings/th07_Border_Bonus_Format|Hardcoded_ASCII_strings/th07_Border_Bonus_Format

rm -rf req_edit.txt
touch req_edit.txt
rm -rf req_move.txt

url_prefix="https://www.thpatch.net/w/api.php?format=json"
while read line; do
    param1=$(echo "$line" | cut -d'|' -f1)
    param2=$(echo "$line" | cut -d'|' -f2)
    page_name=$(echo "$param1" | sed -e 's|^https://www\.thpatch\.net/wiki/\(.*\)$|\1|')
    page_file=$(echo "$page_name" | tr / _)
    echo "$page_name"

    psoffset=0
    while [ $psoffset != null ]; do
        echo "$page_name: translations[$psoffset]"
        get_translations_url="$url_prefix&action=query&list=prefixsearch&pslimit=500&psnamespace=1198&psoffset=$psoffset&pssearch=$page_name/"
        req=$(curl --no-progress-meter --retry 5 "$get_translations_url")
        echo "$req" | jq -r '.query.prefixsearch[].title' | sed -e 's/^Translations://' | while read tl_page_name; do
            lang=$(echo "$tl_page_name" | sed -e 's|^.*/\([a-zA-Z0-9-]*\)$|\1|')
            new_page_name="$(echo "$param2" | tr '_' ' ')/$lang"
            echo "$tl_page_name|$new_page_name" >> req_move.txt
        done
        psoffset=$(echo "$req" | jq '.continue.psoffset')
    done
done < tlunits_list.txt
