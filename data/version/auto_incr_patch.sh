#!/bin/bash

path="data/version/VERSION"

old=$(cat $path)

old_version=$(echo $old | cut -d '.' -f 1)
old_patch=$(echo $old | cut -d '.' -f 2)

new_patch=$((old_patch + 1))
new="${old_version}.${new_patch}"

echo "$new" > $path
