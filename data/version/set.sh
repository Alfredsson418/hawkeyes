#!/bin/bash

path="./data/version/version.json"

old=$(jq -r ".version" "$path")

new_release=$(echo "$old" | cut -d '.' -f 1)
new_version=$(echo "$old" | cut -d '.' -f 2)
new_patch=$(echo "$old" | cut -d '.' -f 3)

if [[ $1 != "-1" ]]; then
    new_release=$(($1))
fi

if [[ $2 != "-1" ]]; then
    new_version=$(($2))
fi

if [[ $3 != "-1" ]]; then
    new_patch=$(($3))
fi


new="{\"version\": \"${new_release}.${new_version}.${new_patch}\"}"

echo "$new" | jq > $path
