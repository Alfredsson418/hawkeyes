#!/bin/bash

path="data/version/version.json"

old=$(jq ".version" version.json -r)

old_release=$(echo $old | cut -d '.' -f 1)
old_version=$(echo $old | cut -d '.' -f 2)
old_patch=$(echo $old | cut -d '.' -f 3)


new_release=$((old_release + $1))
new_version=$((old_version + $2))
new_patch=$((old_patch + $2))



new="{\"version\": \"${new_release}.${new_version}.${new_patch}\"}"

echo "$new" | jq > $path
