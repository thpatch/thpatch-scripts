#!/bin/bash

source ./api_helpers.sh

cat users.txt | cut -d':' -f2 | while read user; do
	echo "$user"
	thpatch_block_user "$user"
done
