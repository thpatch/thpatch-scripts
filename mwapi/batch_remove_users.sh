#!/bin/bash

source ./api_helpers.sh

cat users.txt | sed -e 's/^.*:\([^:]*\)$/\1/' | while read user; do
	echo "$user"
	thpatch_block_user "$user"
done
