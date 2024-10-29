#!/bin/bash

source ./api_helpers.sh

rm -rf pageid.txt
touch pageid.txt

cat users.txt | while read page; do
	page=$(echo "$page" | sed -e 's/ /_/')
	echo $page
	thpatch_search "$page&srnamespace=2|3&srwhat=title" | jq -r '.query.search[].pageid' >> pageid.txt
done

./batch_delete_pages.sh
./batch_remove_users.sh
