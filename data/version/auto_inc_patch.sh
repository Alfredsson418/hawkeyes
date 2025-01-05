#!/bin/bash

old=$(cat VERSION)

old_version=$(echo $old | cut -d '.' -f 1)
old_patch=$(echo $old | cut -d '.' -f 2)

new_patch=$((old_patch + 1))
new="${old_version}.${new_patch}"

echo "$new" > VERSION
