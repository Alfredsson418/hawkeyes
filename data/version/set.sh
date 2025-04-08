#!/bin/bash

path="./data/version/version.json"

new_release=$(($1))
new_version=$(($2))
new_patch=$(($3))



new="{\"version\": \"${new_release}.${new_version}.${new_patch}\"}"

echo "$new" | jq > $path
