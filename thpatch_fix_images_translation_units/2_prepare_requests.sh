#!/bin/bash

rm -f req_edit.txt req_move.txt

while read line; do
    page_name=$(echo "$line" | sed -e 's|^https://www\.thpatch\.net/wiki/\(.*\)$|\1|')
    page_file=$(echo "$page_name" | tr / _)
    page_name=$(echo "$page_name" | tr '_' ' ')

    languages=$(sed -e "s|^$page_name"'/\(.*\)$|\1|' ${page_file}_lang.txt)
    echo $page_name : $languages
    cp ${page_file}_content.txt ${page_file}_content_fixed.txt
    grep -o -e '<!--T:.*/.*-->' -e '<!--T:.*_.*-->' ${page_file}_content.txt | while read tlunit_whole; do
        tlunit_old_name=$(echo "$tlunit_whole" | sed -e 's/<!--T:\(.*\)-->/\1/')
        tlunit_new_name=$(echo "$tlunit_old_name" | tr '_/' '  ')
        echo "$tlunit_old_name => $tlunit_new_name"

        sed -i -e "s|$tlunit_whole|<!--T:$tlunit_new_name-->|" ${page_file}_content_fixed.txt

        while read language; do
            old_page_name=$page_name/$tlunit_old_name/$language
            new_page_name=$page_name/$tlunit_new_name/$language
            if grep "^Translations:$old_page_name\$" ${page_file}_translations.txt > /dev/null; then
                # echo "API: move $old_page_name to $new_page_name"
                echo "$old_page_name|$new_page_name" >> req_move.txt
            else
                # echo "$old_page_name: not in the server, ignoring."
                true
            fi
        done <<< "$languages"
    done

    if ! diff -q ${page_file}_content.txt ${page_file}_content_fixed.txt > /dev/null; then
        # echo "API: update $page_name"
        echo "$page_name" >> req_edit.txt
    else
        echo "$page_name: nothing to fix"
    fi
done < pages_list.txt
