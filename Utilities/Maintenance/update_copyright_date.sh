#!/bin/bash

tmp_file_name="_tmp_file_list.txt"
# get all files
find . -type f > $tmp_file_name
current_year=$(date -u | egrep -o "[0-9]{4}")

# for all files, update the year to current
while IFS= read -r f_path; do
  # except for this current script
  if [ "$f_path" != $0 ]; then
    sed -i "s/Copyright (C) 2005-20../Copyright (C) 2005-$current_year/g" "$line"
  fi
done < "$tmp_file_name"

rm $tmp_file_name
