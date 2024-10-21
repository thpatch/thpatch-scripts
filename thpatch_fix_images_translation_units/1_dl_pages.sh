#!/bin/bash

# Example file:
# https://www.thpatch.net/wiki/Th135/Act_and_Nut_strings

url_prefix="https://www.thpatch.net/w/api.php?format=json"
while read line; do
    page_name=$(echo "$line" | sed -e 's|^https://www\.thpatch\.net/wiki/\(.*\)$|\1|')
    page_file=$(echo "$page_name" | tr / _)
    echo "$page_name"

    get_content_url="$url_prefix&action=query&prop=revisions&titles=$page_name&rvslots=*&rvprop=content&formatversion=2"
    get_lang_url="$url_prefix&action=query&list=prefixsearch&pslimit=500&pssearch=$page_name/"
    curl --no-progress-meter --retry 5 "$get_content_url" | jq -r '.query.pages[0].revisions[0].slots.main.content' > ${page_file}_content.txt
    curl --no-progress-meter --retry 5 "$get_lang_url"    | jq -r '.query.prefixsearch[].title' > ${page_file}_lang.txt

    rm -f ${page_file}_translations.txt
    psoffset=0
    while [ $psoffset != null ]; do
        echo "$page_name: translations[$psoffset]"
        get_translations_url="$url_prefix&action=query&list=prefixsearch&pslimit=500&psnamespace=1198&psoffset=$psoffset&pssearch=$page_name/"
        req=$(curl --no-progress-meter --retry 5 "$get_translations_url")
        echo "$req" | jq -r '.query.prefixsearch[].title' >> ${page_file}_translations.txt
        psoffset=$(echo "$req" | jq '.continue.psoffset')
    done
done < pages_list.txt
