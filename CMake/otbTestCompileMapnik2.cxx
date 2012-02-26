#include <mapnik/feature.hpp>
#include <mapnik/geometry.hpp>
#include <mapnik/raster.hpp>
#include <mapnik/rule.hpp>

int main(int argc, char* argv[])
{
  mapnik::rule rule;

  typedef mapnik::vertex<double, 2>         vertex2d;
  typedef mapnik::geometry<vertex2d>        geom;
  typedef boost::shared_ptr<mapnik::raster> raster_ptr;
  typedef mapnik::feature<geom, raster_ptr> Feature;
  typedef boost::shared_ptr<Feature>        feature_ptr;
  feature_ptr mfeature = feature_ptr(new Feature(1));
}
