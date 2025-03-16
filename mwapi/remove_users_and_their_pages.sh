#!/bin/bash

source ./api_helpers.sh

rm -rf pageid.txt
touch pageid.txt

cat users.txt | while read user; do
	user=$(echo "$user" | sed -e 's/ /_/' \
        -e 's/^.*:\([^:]*\)$/\1/') # If the line is an URL, remove everything up to (and including) the last ':'.
                                   # Because the URL is supposed to end with "User:Something", it will keep the
                                   # "Something" part.
	echo $user
	curl_get "action=query&list=usercontribs&uclimit=500&ucnamespace=*&ucprop=ids&ucshow=new&ucuser=$user" | jq -r '.query.usercontribs[].pageid' >> pageid.txt
done

./batch_delete_pages.sh
./batch_remove_users.sh
