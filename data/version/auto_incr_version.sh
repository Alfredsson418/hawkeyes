#!/bin/bash

old=$(cat data/version/VERSION)

old_version=$(echo $old | cut -d '.' -f 1)

new_version=$((old_version + 1))
new="${new_version}.0"

echo "$new" > VERSION
