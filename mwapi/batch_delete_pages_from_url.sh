#!/bin/bash

source ./api_helpers.sh

cat pages_list.txt | while read page; do
	echo "$page"
	thpatch_delete_from_url "$page" --data-urlencode "reason=Spam account"
done
