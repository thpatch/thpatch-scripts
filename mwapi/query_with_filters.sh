#!/bin/bash

source ./api_helpers.sh

# List all the user and user talk pages with "hair", and exclude "User:UnKnwn/en125" from the results.
# Surprisingly, this caught a lot of span accounts.
thpatch_search "hair&srnamespace=1|2" | jq -r 'del(.query.search[] | select(.title == "User:UnKnwn/en125")) | .query.search[].pageid'
