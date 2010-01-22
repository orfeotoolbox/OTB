#!/bin/sh
# Note: If you install Xcode 2.5 on OS X 2.4.x it destroys the libtool library.
# You MUST have Xcode 2.4.x if you're on Tiger.

rm -rf autom4te.cache

PLATFORM=`uname -s`
echo Platform is $PLATFORM

# Mac OS X doesn't know what libtoolize is. We have to call glibtoolize.
if [ $PLATFORM = Darwin ]
then
	LIBTOOLIZE=glibtoolize
else
	LIBTOOLIZE=libtoolize
fi
export LIBTOOLIZE=$LIBTOOLIZE

autoreconf --force --install

