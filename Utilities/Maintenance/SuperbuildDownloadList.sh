#/bin/bash
grep -h -E "^[^#]*\"https?://.*(\.tar\.gz|\.tar\.bz2|\.tgz|\.tar\.xz|\.zip|export=download).*\"" ../../SuperBuild/CMake/*.cmake | grep -o -E "https?://[^\"]*" | sed "s/\/download$//" | sort | uniq
