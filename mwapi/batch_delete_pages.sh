#!/bin/bash

source ./api_helpers.sh

cat pageid.txt | while read page; do
	echo "$page"
	thpatch_delete "$page" --data-urlencode "reason=Spam account"
done
