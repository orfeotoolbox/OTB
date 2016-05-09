#! /bin/sh
convert $1 eps:-  | grep "%%BoundingBox"  > $1.bb

