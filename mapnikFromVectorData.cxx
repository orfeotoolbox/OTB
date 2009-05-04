#include "otbVectorDataFileReader.h"
#include "otbVectorData.h"
#include "otbVectorDataProjectionFilter.h"
#include <fstream>
#include <iostream>

#include "itkTreeContainer.h"
#include "itkDataObject.h"
#include "otbDataNode.h"

#include <mapnik/map.hpp>
#include <mapnik/datasource_cache.hpp>
#include <mapnik/font_engine_freetype.hpp>
#include <mapnik/agg_renderer.hpp>
#include <mapnik/filter_factory.hpp>
#include <mapnik/color_factory.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/config_error.hpp>
#include <mapnik/memory_datasource.hpp>

#include <mapnik/value.hpp>

// ./mapnikFromVectorData  ~/OTB/trunk/OTB-Data/Input/ToulouseRoad-examples.shp

  typedef otb::VectorData<> VectorDataType;
  typedef otb::DataNode<double,2,double> DataNodeType;
  typedef DataNodeType::Pointer DataNodePointerType;
  typedef itk::TreeContainer<DataNodePointerType> DataTreeType;
  typedef VectorDataType::DataTreeType::TreeNodeType    InternalTreeNodeType;
  typedef InternalTreeNodeType::ChildrenListType        ChildrenListType;

  typedef boost::shared_ptr<mapnik::memory_datasource> datasource_ptr;

  void ProcessNode(InternalTreeNodeType * source, datasource_ptr mDatasource)
{
  // Get the children list from the input node
  ChildrenListType children = source->GetChildrenList();

  // For each child
  for(ChildrenListType::iterator it = children.begin(); it!=children.end();++it)
  {
    // Copy input DataNode info
    DataNodePointerType dataNode = (*it)->Get();

    switch(dataNode->GetNodeType())
    {
      case otb::ROOT:
      {
        ProcessNode((*it),mDatasource);
        break;
      }
      case otb::DOCUMENT:
      {
        ProcessNode((*it),mDatasource);
        break;
      }
      case otb::FOLDER:
      {
        ProcessNode((*it),mDatasource);
        break;
      }
//       case FEATURE_POINT:
//       {
//         newDataNode->SetPoint(this->ReprojectPoint(dataNode->GetPoint()));
//         newContainer = OutputInternalTreeNodeType::New();
//         newContainer->Set(newDataNode);
//         destination->AddChild(newContainer);
//         break;
//       }
      case otb::FEATURE_LINE:
      {
        std::cout << std::setprecision(15)  << " ** Inserting new line **" << std::endl;
        typedef mapnik::vertex<double,2> vertex2d;
        typedef mapnik::line_string<vertex2d,mapnik::vertex_vector2> line2d;
        typedef boost::shared_ptr<line2d> line_ptr;
        mapnik::geometry2d * line = new line2d;

        typedef DataNodeType::LineType::VertexListConstIteratorType VertexIterator;
        VertexIterator itVertex = dataNode->GetLine()->GetVertexList()->Begin();
        while (itVertex != dataNode->GetLine()->GetVertexList()->End())
        {
          std::cout << itVertex.Value()[0] << ", " << itVertex.Value()[1] << std::endl;
          line->line_to(itVertex.Value()[0],itVertex.Value()[1]);
          ++itVertex;
        }

        std::cout << "Num points: " << line->num_points() << std::endl;


        typedef boost::shared_ptr<mapnik::raster> raster_ptr;
        typedef mapnik::feature<mapnik::geometry2d,raster_ptr> Feature;
        typedef boost::shared_ptr<Feature> feature_ptr;

//         typedef std::map<std::string,mapnik::value> mapType;
//         mapType myMap;
//         myMap["name"] = mapnik::value("test");
//         std::cout << myMap.size() << std::endl;

        feature_ptr mfeature = feature_ptr(new Feature(1));
//         feature_ptr mfeature = feature_ptr(new Feature(myMap));
        mfeature->add_geometry(line);
//         mfeature->properties().insert(make_pair(std::string("name"),
//                                         std::string("test")));

//         boost::put(*mfeature, "name", mapnik::value("test"));
        mapnik::transcoder tr("ISO-8859");
        boost::put(*mfeature, "name", tr.transcode("test"));
        boost::put(*mfeature, "name2", 10);
        std::cout << mfeature->props().size() << std::endl;
        std::cout << " -> " << (*mfeature)["name"] << std::endl;
        std::cout << " -> " << (*mfeature)["name2"] << std::endl;

        mDatasource->push(mfeature);


        break;
      }
//       case FEATURE_POLYGON:
//       {
//         newDataNode->SetPolygonExteriorRing(this->ReprojectPolygon(dataNode->GetPolygonExteriorRing()));
//         newDataNode->SetPolygonInteriorRings(this->ReprojectPolygonList(dataNode->GetPolygonInteriorRings()));
//         newContainer = OutputInternalTreeNodeType::New();
//         newContainer->Set(newDataNode);
//         destination->AddChild(newContainer);
//         break;
//       }
//       case FEATURE_MULTIPOINT:
//       {
//         newContainer = OutputInternalTreeNodeType::New();
//         newContainer->Set(newDataNode);
//         destination->AddChild(newContainer);
//         ProcessNode((*it),newContainer);
//         break;
//       }
//       case FEATURE_MULTILINE:
//       {
//         newContainer = OutputInternalTreeNodeType::New();
//         newContainer->Set(newDataNode);
//         destination->AddChild(newContainer);
//         ProcessNode((*it),newContainer);
//         break;
//       }
//       case FEATURE_MULTIPOLYGON:
//       {
//         newContainer = OutputInternalTreeNodeType::New();
//         newContainer->Set(newDataNode);
//         destination->AddChild(newContainer);
//         ProcessNode((*it),newContainer);
//         break;
//       }
//       case FEATURE_COLLECTION:
//       {
//         newContainer = OutputInternalTreeNodeType::New();
//         newContainer->Set(newDataNode);
//         destination->AddChild(newContainer);
//         ProcessNode((*it),newContainer);
//         break;
//       }
    }
  }
}


int main(int argc, char * argv[])
{


  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
  VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();

  reader->SetFileName(argv[1]);
//   reader->Update();

  typedef otb::VectorDataProjectionFilter<VectorDataType, VectorDataType> ProjectionFilterType;
  ProjectionFilterType::Pointer projection = ProjectionFilterType::New();
  projection->SetInput(reader->GetOutput());
  projection->Update();

//   mapnik::datasource_cache::instance()->register_datasources( "/home/christop/opensource/mapnik/plugins/input/shape");
//   mapnik::datasource_cache::instance()->register_datasources( "/usr/lib/mapnik/0.5/input");
  mapnik::freetype_engine::register_font("/usr/share/fonts/truetype/ttf-dejavu/DejaVuSans.ttf");
  mapnik::Map m(800,600);
//   m.set_background(mapnik::color_factory::from_string("cadetblue"));
  m.set_background(mapnik::color("#f2efe9"));
  {
    mapnik::feature_type_style style;
    mapnik::rule_type rule;
    rule.set_max_scale(250000000000LLU);
    rule.set_min_scale(600000);
    rule.append(mapnik::line_symbolizer(mapnik::color("#f2efe9")));
//     rule.append(mapnik::polygon_symbolizer(mapnik::Color(242,239,233)));
    style.add_rule(rule);
    m.insert_style("world",style);
  }
  {
    mapnik::feature_type_style style;
    mapnik::rule_type rule;
    rule.set_max_scale(600000);
    rule.append(mapnik::line_symbolizer(mapnik::color("#f2efe9")));
//     rule.append(mapnik::polygon_symbolizer(mapnik::Color(242,239,233)));
    style.add_rule(rule);
    m.insert_style("coast-poly",style);
  }
  {
    mapnik::feature_type_style style;
    mapnik::rule_type rule;
    rule.set_max_scale(600000);
    rule.append(mapnik::line_symbolizer(mapnik::color("#e46d71")));
//     rule.append(mapnik::polygon_symbolizer(mapnik::Color(242,239,233)));
    style.add_rule(rule);
    m.insert_style("coast-poly",style);
  }
  {
    mapnik::feature_type_style style;
    mapnik::rule_type rule;
    rule.set_max_scale(1000000);
//     rule.set_min_scale(500000);
    rule.append(mapnik::line_symbolizer(mapnik::color("#809bc0"),8.0));
//     TextSymbolizer name="name" face_name="DejaVu Sans Book" size="9" fill="#000" halo_radius="1"  placement="line"
//     text_symbolizer (std::string const &name, std::string const &face_name, unsigned size, color const &fill)
//     text_symbolizer (std::string const &name, unsigned size, color const &fill)
    mapnik::text_symbolizer textSymb("name", "DejaVu Sans Book", 9, mapnik::color("#000"));
    textSymb.set_label_placement(mapnik::LINE_PLACEMENT);
    rule.append(textSymb);
    style.add_rule(rule);
    m.insert_style("roads",style);
  }
//   <Filter>[highway] = 'motorway' or [highway] = 'motorway_link'</Filter>
//       <MaxScaleDenominator>1000000</MaxScaleDenominator>
//       <MinScaleDenominator>500000</MinScaleDenominator>
//       <LineSymbolizer>
//       <CssParameter name="stroke">#809bc0</CssParameter>
//       <CssParameter name="stroke-width">3</CssParameter>
//       </LineSymbolizer>

  datasource_ptr mDatasource = datasource_ptr(new mapnik::memory_datasource);


  InternalTreeNodeType * inputRoot = const_cast<InternalTreeNodeType *>(projection->GetOutput()->GetDataTree()->GetRoot());

  ProcessNode(inputRoot,mDatasource);


  std::cout << "Datasource size: " << mDatasource->size() << std::endl;


//   mapnik::parameters p;
//   p["type"]="shape";
//   p["file"]="world_borders";

  mapnik::Layer lyr("world");
//   lyr.set_datasource(mapnik::datasource_cache::instance()->create(p));
  lyr.set_datasource(mDatasource);
  lyr.add_style("roads");
  m.addLayer(lyr);


  m.zoomToBox(lyr.envelope());
  std::cout << "Envelope: " << lyr.envelope() << std::endl;

  mapnik::Image32 buf(m.getWidth(),m.getHeight());
  mapnik::agg_renderer<mapnik::Image32> ren(m,buf);
  ren.apply();

  mapnik::save_to_file<mapnik::ImageData32>(buf.data(),"myOwn.png","png");



  return EXIT_SUCCESS;
}

