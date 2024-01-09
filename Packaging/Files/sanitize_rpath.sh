#!/bin/bash
BINARY_FILES=$(find "$OTB_INSTALL_DIR/lib" "$OTB_INSTALL_DIR/bin" -type f -exec file {} \; | grep -i ': elf ' | cut -f1 -d':')
# run patchelf
for bin_file in $BINARY_FILES; do
    #echo "adding rpath to $bin_file"
    patchelf "--set-rpath" "$OTB_INSTALL_DIR/lib" $bin_file
done
