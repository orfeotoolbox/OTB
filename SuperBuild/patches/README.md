## Synopsis

Illustrates how to include a patch to the superbuild sources. This patch will be apply before the compilation of the external project.

## Example

Here is how to make a patch for superbuild. The example is here used libgeos but it can be done for any external projects in the OTB Superbuild. 

- cd /tmp
- wget http://download.osgeo.org/geos/geos-3.6.1.tar.bz2
- tar xjf geos-3.6.1.tar.bz2
- cp -r geos-3.6.1 geos-3.6.1.orig

(make changes to geos-3.6.1)

And prepare diff between two directories

- diff -burN geos-3.6.1.orig geos-3.6.1 > geos-1-fixes-all.diff

diff file naming is <project>-<number for applying in correct order>-<all/win/linux/macx>.diff
