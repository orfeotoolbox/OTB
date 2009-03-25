#define BOOST_SPIRIT_THREADSAFE

#include <mapnik/map.hpp>
#include <mapnik/datasource_cache.hpp>
#include <mapnik/font_engine_freetype.hpp>
#include <mapnik/agg_renderer.hpp>
#include <mapnik/filter_factory.hpp>
#include <mapnik/color_factory.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/config_error.hpp>
#include <mapnik/memory_datasource.hpp>
#include <iostream>

int main ( int argc , char** argv)
{
  using namespace mapnik;
  datasource_cache::instance()->register_datasources( "/home/christop/opensource/mapnik/plugins/input/shape");
  freetype_engine::register_font("/usr/share/fonts/truetype/ttf-dejavu/DejaVuSans.ttf");


  Map m(800,600);
  m.set_background(color_factory::from_string("cadetblue"));

//   {
//   feature_type_style style;
//   rule_type rule;
//   rule.append(polygon_symbolizer(color(250, 190, 183)));
//   rule.append(line_symbolizer(color('rgb(50%,50%,50%)'),0.1));
//   style.add_rule(rule);
//   m.insert_style("world",style);
//   }

  {
    feature_type_style style;
    rule_type rule;
    rule.set_max_scale(250000000000LLU);
    rule.set_min_scale(600000);
    rule.append(polygon_symbolizer(color("#f2efe9")));
    style.add_rule(rule);
    m.insert_style("world",style);
  }
  {
    feature_type_style style;
    rule_type rule;
    rule.set_max_scale(600000);
    rule.append(polygon_symbolizer(color("#f2efe9")));
    style.add_rule(rule);
    m.insert_style("coast-poly",style);
  }

  typedef vertex<double,2> vertex2d;
  typedef line_string<vertex2d,vertex_vector2> line2d;
  typedef boost::shared_ptr<line2d> line_ptr;
  geometry2d * line = new line2d;
//   vertex2d.x = 0;
//   vertex2d.y = 0;
  std::cout << line->num_points() << std::endl;
  line->line_to(0,0);
  line->line_to(45,45);
  line->line_to(50,30);
  line->line_to(0,-30);
  std::cout << line->num_points() << std::endl;

  typedef boost::shared_ptr<raster> raster_ptr;
  typedef feature<geometry2d,raster_ptr> Feature;
  typedef boost::shared_ptr<Feature> feature_ptr;
  feature_ptr mfeature = feature_ptr(new Feature(1));
  mfeature->add_geometry(line);

  typedef boost::shared_ptr<memory_datasource> datasource_ptr;
  datasource_ptr mDatasource = datasource_ptr(new memory_datasource);
  mDatasource->push(mfeature);



  parameters p;
  p["type"]="shape";
  p["file"]="world_borders";

  Layer lyr("world");
//   lyr.set_datasource(datasource_cache::instance()->create(p));
  lyr.set_datasource(mDatasource);
  lyr.add_style("world");
  m.addLayer(lyr);


  m.zoomToBox(lyr.envelope());

  Image32 buf(m.getWidth(),m.getHeight());
  agg_renderer<Image32> ren(m,buf);
  ren.apply();

  save_to_file<ImageData32>(buf.data(),"myOwn.png","png");


  return EXIT_SUCCESS;
}