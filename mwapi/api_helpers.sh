source $(dirname "$0")/credentials.sh

url_api=https://www.thpatch.net/w/api.php
echo > token.txt

function curl_get
{
    curl --no-progress-meter --retry 5 -b cookies.txt -c cookies.txt "$url_api?format=json&$1"
}

function curl_post
{
    curl --no-progress-meter --retry 5 -b cookies.txt -c cookies.txt "$url_api" --data-urlencode format=json "$@"
}

function thpatch_login
{
    curl_get "action=query&meta=tokens&type=login" | jq .query.tokens.logintoken -r | tr -s '\\\\' > token.txt

    ret=$(curl_post --data-urlencode action=clientlogin --data-urlencode "username=$username" --data-urlencode "password=$password" --data-urlencode "logintoken=$(cat token.txt)" --data-urlencode 'loginreturnurl=http://example.org/')
    if [ "$(echo "$ret" | jq -r .clientlogin.status)" != "PASS" ]; then
        echo "FATAL ERROR: login failed" >&2
        echo "$ret" | jq --color-output . >&2
        exit 1
    fi

    curl_get "action=query&meta=tokens&type=csrf" | jq .query.tokens.csrftoken -r | tr -s '\\\\' > token.txt
}

function thpatch_csrf
{
    if [ "$(cat token.txt)" = "" ]; then
        thpatch_login
    fi

    ret=$(curl_post --data-urlencode "token=$(cat token.txt)" "$@")

    if [ "$(echo "$ret" | jq -r .error.code)" = "badtoken" ]; then
        thpatch_login
        ret=$(curl_post --data-urlencode "token=$(cat token.txt)" "$@")
    fi

    echo "$ret"
}

# Usage:
# thpatch_move source destination
function thpatch_move
{
    ret=$(thpatch_csrf --data-urlencode action=move --data-urlencode "from=$1" --data-urlencode "to=$2" --data-urlencode noredirect=true)
    if [ "$(echo "$ret" | jq -r .move.to)" != "$2" ]; then
        echo "ERROR moving $1 to $2"
        echo "$ret" | jq .
    fi
}

# Usage:
# thpatch_edit page_name new_content.txt
# Remove the `nocreate=true` if you want to create a page
function thpatch_edit
{
    ret=$(thpatch_csrf --data-urlencode action=edit --data-urlencode "title=$1" --data-urlencode "text@$2" --data-urlencode nocreate=true)
    if [ "$(echo "$ret" | jq -r .edit.result)" != "Success" ]; then
        echo "ERROR editing $1"
        echo "$ret" | jq .
    fi
}

# Usage:
# thpatch_delete page_id additional_parameters
function thpatch_delete
{
	page="$1"
	shift

    ret=$(thpatch_csrf --data-urlencode action=delete --data-urlencode "pageid=$page" "$@")
    if [ "$(echo "$ret" | jq -r .delete)" = "null" ]; then
        echo "ERROR removing page $page"
        echo "$ret" | jq .
    fi
}

# Usage:
# thpatch_search something
# You can add more arguments.
# For example:
# thpatch_search something&ns=1|2
# For the results, you can use this if you want the page names:
# thpatch_search something | jq -r '.query.search[]'.title
# Or if you want the page ID:
# thpatch_search something | jq -r '.query.search[]'.pageid
function thpatch_search
{
	curl_get "action=query&list=search&srwhat=text&srlimit=500&srsearch=$1"
}

# Usage:
# thpatch_block_user username
function thpatch_block_user
{
    ret=$(thpatch_csrf --data-urlencode action=block --data-urlencode "user=$1" --data-urlencode "reason=Spam account" --data-urlencode autoblock=true --data-urlencode noemail=true --data-urlencode allowusertalk=false)
    if [ "$(echo "$ret" | jq -r .block)" = "null" ]; then
        echo "ERROR blocking user $1"
        echo "$ret" | jq .
    fi
}
