#/bin/bash
grep "http" ../../SuperBuild/CMake/*.cmake | grep URL | cut -d "\"" -f 2 | sed "s/\/download$//" | sort | uniq
