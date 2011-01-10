/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbVectorDataToImageFilter_txx
#define __otbVectorDataToImageFilter_txx

#include <sstream>
#include "otbVectorDataToImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageIteratorWithIndex.h"
#include "otbVectorDataStyle.h"
#include "itkRGBAPixel.h"

#include <mapnik/datasource_cache.hpp>
#include <mapnik/font_engine_freetype.hpp>
#include <mapnik/agg_renderer.hpp>
#include <mapnik/filter_factory.hpp>
#include <mapnik/color_factory.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/config_error.hpp>
#include <mapnik/memory_datasource.hpp>

#include <mapnik/value.hpp>

#include "ogr_spatialref.h"

namespace otb
{

/**
   * Constructor
 */
template <class TVectorData, class TImage>
VectorDataToImageFilter<TVectorData, TImage>
::VectorDataToImageFilter() :
  m_StyleList(),
  m_UseAsOverlay(true),
  m_RenderingStyleType(OSM)
{
  this->SetNumberOfRequiredInputs(1);
  m_Spacing.Fill(1.0);
  m_Origin.Fill(0.0);
  m_Direction.SetIdentity();
  m_Size.Fill(0);
  m_StartIndex.Fill(0);
  m_Map = mapnik::Map();
  m_SensorModelFlip = 1;
  m_ScaleFactor = 1.0;
  m_VectorDataProjectionProj4 = "";
}

template <class TVectorData, class TImage>
void
VectorDataToImageFilter<TVectorData, TImage>
::SetInput(const VectorDataType *input)
{
// Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<VectorDataType *>(input));
}

template <class TVectorData, class TImage>
void
VectorDataToImageFilter<TVectorData, TImage>
::SetInput(unsigned int idx, const VectorDataType *input)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(idx,
                                        const_cast<VectorDataType *>(input));
}

template <class TVectorData, class TImage>
const typename VectorDataToImageFilter<TVectorData, TImage>::VectorDataType *
VectorDataToImageFilter<TVectorData, TImage>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const TVectorData *>
           (this->itk::ProcessObject::GetInput(0));
}

template <class TVectorData, class TImage>
const typename VectorDataToImageFilter<TVectorData, TImage>::VectorDataType *
VectorDataToImageFilter<TVectorData, TImage>
::GetInput(unsigned int idx)
{
  return static_cast<const TVectorData *>
           (this->itk::ProcessObject::GetInput(idx));
}

//----------------------------------------------------------------------------
template <class TVectorData, class TImage>
void
VectorDataToImageFilter<TVectorData, TImage>
::SetSpacing(const SpacingType& spacing)
{
  if (this->m_Spacing != spacing)
    {
    this->m_Spacing = spacing;
    this->Modified();
    }
}

//----------------------------------------------------------------------------
template <class TVectorData, class TImage>
void
VectorDataToImageFilter<TVectorData, TImage>
::SetSpacing(const double spacing[2])
{
  SpacingType s(spacing);
  this->SetSpacing(s);
}

//----------------------------------------------------------------------------
template <class TVectorData, class TImage>
void
VectorDataToImageFilter<TVectorData, TImage>
::SetSpacing(const float spacing[2])
{
  itk::Vector<float, 2> sf(spacing);
  SpacingType s;
  s.CastFrom(sf);
  this->SetSpacing(s);
}

//----------------------------------------------------------------------------
template <class TVectorData, class TImage>
void
VectorDataToImageFilter<TVectorData, TImage>
::SetOrigin(const double origin[2])
{
  OriginType p(origin);
  this->SetOrigin(p);
}

//----------------------------------------------------------------------------
template <class TVectorData, class TImage>
void
VectorDataToImageFilter<TVectorData, TImage>
::SetOrigin(const float origin[2])
{
  itk::Point<float, 2> of(origin);
  OriginType p;
  p.CastFrom(of);
  this->SetOrigin(p);
}

/**
* Inform pipeline of required output region
*/
template <class TVectorData, class TImage>
void
VectorDataToImageFilter<TVectorData, TImage>
::GenerateOutputInformation()
{

  // we can't call the superclass method here.

  // get pointers to the input and output
  ImagePointer outputPtr = this->GetOutput();
  if (!outputPtr)
    {
    return;
    }

  // Set the size of the output region
  typename TImage::RegionType outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize(m_Size);
  outputLargestPossibleRegion.SetIndex(m_StartIndex);
  outputPtr->SetLargestPossibleRegion(outputLargestPossibleRegion);

  // Set spacing and origin
  outputPtr->SetSpacing(m_Spacing);
  outputPtr->SetOrigin(m_Origin);
  outputPtr->SetDirection(m_Direction);

  //TODO update or check the projection information

  return;
}

/**
* BeforeThreadedGenerateData
*/
template <class TVectorData, class TImage>
void
VectorDataToImageFilter<TVectorData, TImage>
::BeforeThreadedGenerateData(void)
{

  Superclass::BeforeThreadedGenerateData();

  if(!m_FontFileName.empty())
    mapnik::freetype_engine::register_font(m_FontFileName);
  
  //Load the OSM styles using helper class
  otb::VectorDataStyle::Pointer styleLoader = otb::VectorDataStyle::New();
  styleLoader->SetScaleFactor(m_ScaleFactor);
  switch (m_RenderingStyleType)
    {
    case OSM:
    {
    styleLoader->LoadOSMStyle(m_Map);
    if (m_UseAsOverlay)
      {
      //Set the default backgroup to transparent
      m_Map.set_background(mapnik::color(255, 255, 255, 0));
      }
    else
      {
      m_Map.set_background(mapnik::color("#b5d0d0"));
      }
    break;
    }
    case Binary:
    {
    styleLoader->LoadBinaryRasterizationStyle(m_Map);
    //Set the backgroup to white
    m_Map.set_background(mapnik::color("#ffffff"));
    break;
    }
    default:
    {
    itkExceptionMacro(<< "Style Type Not Supported!");
    break;
    }
    }

  //We assume that all the data are reprojected before using OTB.
  VectorDataConstPointer input = this->GetInput();
  //Converting the projection string to the proj.4 format
  std::string vectorDataProjectionWKT;
  itk::ExposeMetaData<std::string>(
    input->GetMetaDataDictionary(), MetaDataKey::ProjectionRefKey,  vectorDataProjectionWKT);
  std::cout << "WKT -> " << vectorDataProjectionWKT << std::endl;
  m_SensorModelFlip = 1;

  if (vectorDataProjectionWKT == "")
    {
    //We assume that it is an image in sensor model geometry
    //and tell mapnik that this is utm
    //(with a resolution of 1m per unit)
    m_VectorDataProjectionProj4 = "+proj=utm +zone=31 +ellps=WGS84";
    m_SensorModelFlip =  -1;
    otbMsgDevMacro(<< "The output map will be in sensor geometry");
    }
  else
    {
    OGRSpatialReferenceH oSRS = OSRNewSpatialReference(vectorDataProjectionWKT.c_str());
    char * pszProj4;
    OSRExportToProj4(oSRS, &pszProj4);
    m_VectorDataProjectionProj4 = pszProj4;
    CPLFree(pszProj4);
    OSRRelease(oSRS);
    m_SensorModelFlip =  1;
    otbMsgDevMacro(<< "The output map will be carto/geo geometry");
    }
  std::cout << "Proj.4 -> " << m_VectorDataProjectionProj4 << std::endl;
  m_Map.set_srs(m_VectorDataProjectionProj4);
}

/**
 * Generate Data
 */
template <class TVectorData, class TImage>
void
VectorDataToImageFilter<TVectorData, TImage>
::GenerateData(void)
{
  this->AllocateOutputs();

  this->BeforeThreadedGenerateData();

  ImagePointer output = this->GetOutput();
  RegionType   requestedRegion = output->GetRequestedRegion();
  otbMsgDevMacro("requestedRegion: " << requestedRegion);

  if ((requestedRegion.GetSize()[0] > (16LU << 10)) || (requestedRegion.GetSize()[1] > (16LU << 10)))
    {
    //Limitation in mapnik/map.hpp, where we have
    // static const unsigned MIN_MAPSIZE=16;
    // static const unsigned MAX_MAPSIZE=MIN_MAPSIZE<<11;
    itkExceptionMacro(<< "Mapnik does not support the generation of tiles bigger than 16 384");
    }

  //Delete the previous layers from the map
  int numberLayer = m_Map.layerCount();
  for (int i = numberLayer - 1; i >= 0; i--) //yes, int.
    {
    m_Map.removeLayer(i);
    }
  m_Map.resize(requestedRegion.GetSize()[0], requestedRegion.GetSize()[1]);

  for (unsigned int idx = 0; idx < this->GetNumberOfInputs(); ++idx)
    {
    if (this->GetInput(idx))
      {
      datasource_ptr mDatasource = datasource_ptr(new mapnik::memory_datasource);

      VectorDataConstPointer input = this->GetInput(idx);
      InternalTreeNodeType * inputRoot = const_cast<InternalTreeNodeType *>(input->GetDataTree()->GetRoot());

      ProcessNode(inputRoot, mDatasource);
      otbMsgDevMacro("Datasource size: " << mDatasource->size());

      std::stringstream layerName;
      layerName << "layer-" << idx;
      mapnik::Layer lyr(layerName.str());
      lyr.set_srs(m_VectorDataProjectionProj4);
      lyr.set_datasource(mDatasource);
      //     lyr.add_style("river");
      //     lyr.add_style("minor-roads-casing");
      //     lyr.add_style("minor-roads");
      //     lyr.add_style("roads");
      //     lyr.add_style("roads-text");
      //     lyr.add_style("world");

      if (m_StyleList.size() == 0)
        {
        itkExceptionMacro(<< "No style is provided for the vector data");
        }
      for (unsigned int i = 0; i < m_StyleList.size(); ++i)
        {
        lyr.add_style(m_StyleList[i]);
        }

      m_Map.addLayer(lyr);

      }
    }
  assert((m_SensorModelFlip == 1) || (m_SensorModelFlip == -1));

  mapnik::Envelope<double> envelope(
    m_Origin[0] + requestedRegion.GetIndex()[0]*m_Spacing[0],
    m_SensorModelFlip*(m_Origin[1] + requestedRegion.GetIndex()[1] * m_Spacing[1] + m_Spacing[1] *
                       requestedRegion.GetSize()[1]),
    m_Origin[0] + requestedRegion.GetIndex()[0]*m_Spacing[0] + m_Spacing[0]*requestedRegion.GetSize()[0],
    m_SensorModelFlip*(m_Origin[1] + requestedRegion.GetIndex()[1] * m_Spacing[1])
    );

  m_Map.zoomToBox(envelope);
//     std::cout << "Envelope: " << lyr.envelope() << std::endl;
  std::cout << "Envelope: " << envelope << std::endl;

  std::cout << "Map scale: " << m_Map.scale_denominator() << std::endl;
  mapnik::Image32 buf(m_Map.getWidth(), m_Map.getHeight());
  mapnik::agg_renderer<mapnik::Image32> ren(m_Map, buf);
  ren.apply();

  const unsigned char * src = buf.raw_data();

  itk::ImageRegionIterator<ImageType> it(output, output->GetRequestedRegion());

  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
    itk::RGBAPixel<unsigned char> pix;
    pix[0] = *src;
    pix[1] = *(src+1);
    pix[2] = *(src+2);
    pix[3] = *(src+3);
    src += 4;

    it.Set(m_RGBAConverter->Convert(pix));
    }
  
  this->AfterThreadedGenerateData();
}

template <class TVectorData, class TImage>
void
VectorDataToImageFilter<TVectorData, TImage>
::ProcessNode(InternalTreeNodeType * source, datasource_ptr mDatasource)
{

  typedef otb::DataNode<double, 2, double> DataNodeType; //FIXME check if it should be infered from the input type
  typedef typename DataNodeType::Pointer   DataNodePointerType;

  // Get the children list from the input node
  ChildrenListType children = source->GetChildrenList();

  // For each child
  for (typename ChildrenListType::iterator it = children.begin(); it != children.end(); ++it)
    {
    // Copy input DataNode info
    DataNodePointerType dataNode = (*it)->Get();

    switch (dataNode->GetNodeType())
      {
      case otb::ROOT:
        {
        ProcessNode((*it), mDatasource);
        break;
        }
      case otb::DOCUMENT:
        {
        ProcessNode((*it), mDatasource);
        break;
        }
      case otb::FOLDER:
        {
        ProcessNode((*it), mDatasource);
        break;
        }
      case FEATURE_POINT:
        {
//           itkExceptionMacro(<<"This type (FEATURE_POINT) is not handle (yet) by VectorDataToImageFilter(), please request for it");
//           std::cout << std::setprecision(15);
//           std::cout << " ** Inserting new point **" << std::endl;

        typedef mapnik::vertex<double, 2>  vertex2d;
        typedef mapnik::point<vertex2d>    point2d;
        typedef boost::shared_ptr<point2d> point_ptr;
        mapnik::geometry2d * point = new point2d;

        point->move_to(dataNode->GetPoint()[0], dataNode->GetPoint()[1]);
//           std::cout << dataNode->GetPoint()[0] << ", " << dataNode->GetPoint()[1] << std::endl;

        typedef boost::shared_ptr<mapnik::raster>               raster_ptr;
        typedef mapnik::feature<mapnik::geometry2d, raster_ptr> Feature;
        typedef boost::shared_ptr<Feature>                      feature_ptr;

        feature_ptr mfeature = feature_ptr(new Feature(1));
        mfeature->add_geometry(point);

        mapnik::transcoder tr("ISO-8859-15");

        if (dataNode->HasField("place_name"))
          boost::put(*mfeature, "name",
                     tr.transcode((dataNode->GetFieldAsString("place_name")).c_str()));

        boost::put(*mfeature, "place", tr.transcode("city"));
        boost::put(*mfeature, "capital", tr.transcode("yes"));  //FIXME more a question of style

        boost::put(*mfeature, "geometry", tr.transcode("point"));

        mDatasource->push(mfeature);

        break;
        }
      case otb::FEATURE_LINE:
        {
//           std::cout << std::setprecision(15);
//           std::cout << " ** Inserting new line **" << std::endl;
        typedef mapnik::vertex<double, 2>                             vertex2d;
        typedef mapnik::line_string<vertex2d, mapnik::vertex_vector2> line2d;
        typedef boost::shared_ptr<line2d>                             line_ptr;
        mapnik::geometry2d * line = new line2d;

        typedef DataNodeType::LineType::VertexListConstIteratorType VertexIterator;
        VertexIterator itVertex = dataNode->GetLine()->GetVertexList()->Begin();
        while (itVertex != dataNode->GetLine()->GetVertexList()->End())
          {
//             std::cout << itVertex.Value()[0] << ", " << itVertex.Value()[1] << std::endl;
          line->line_to(itVertex.Value()[0], m_SensorModelFlip * itVertex.Value()[1]);
          ++itVertex;
          }

//           std::cout << "Num points: " << line->num_points() << std::endl;

        typedef boost::shared_ptr<mapnik::raster>               raster_ptr;
        typedef mapnik::feature<mapnik::geometry2d, raster_ptr> Feature;
        typedef boost::shared_ptr<Feature>                      feature_ptr;

        feature_ptr mfeature = feature_ptr(new Feature(1));
        mfeature->add_geometry(line);

        mapnik::transcoder tr("ISO-8859-15");

        if (dataNode->HasField("name"))
          boost::put(*mfeature, "name",
                     tr.transcode((dataNode->GetFieldAsString("name")).c_str()));
        if (dataNode->HasField("NAME"))
          boost::put(*mfeature, "name",
                     tr.transcode((dataNode->GetFieldAsString("NAME")).c_str()));

//           std::cout << mfeature->props().size() << std::endl;
//           std::cout << " -> " << (*mfeature)["name"] << std::endl;

//           std::cout << "Name: " << dataNode->GetFieldAsString("NAME") << std::endl;
//           std::cout << "Type: " << dataNode->GetFieldAsString("TYPE") << std::endl;
//           std::cout << "OSM ID: " << dataNode->GetFieldAsString("osm_id") << std::endl;

        if (dataNode->HasField("type"))
          boost::put(*mfeature, "highway",
                     tr.transcode((dataNode->GetFieldAsString("type")).c_str()));
        if (dataNode->HasField("TYPE"))
          boost::put(*mfeature, "highway",
                     tr.transcode((dataNode->GetFieldAsString("TYPE")).c_str()));

        boost::put(*mfeature, "geometry", tr.transcode("line"));

        mDatasource->push(mfeature);

        break;
        }
      case FEATURE_POLYGON:
        {
//         itkExceptionMacro(<<"This type (FEATURE_POLYGON) is not handle (yet) by VectorDataToImageFilter(), please request for it");
        typedef mapnik::vertex<double, 2>                         vertex2d;
        typedef mapnik::polygon<vertex2d, mapnik::vertex_vector2> polygon2d;
        typedef boost::shared_ptr<polygon2d>                      polygon_ptr;
        mapnik::geometry2d * polygon = new polygon2d;

        typedef DataNodeType::PolygonType::VertexListConstIteratorType VertexIterator;
        VertexIterator itVertex = dataNode->GetPolygonExteriorRing()->GetVertexList()->Begin();
        while (itVertex != dataNode->GetPolygonExteriorRing()->GetVertexList()->End())
          {
          polygon->line_to(itVertex.Value()[0], m_SensorModelFlip * itVertex.Value()[1]);
          ++itVertex;
          }

        typedef boost::shared_ptr<mapnik::raster>               raster_ptr;
        typedef mapnik::feature<mapnik::geometry2d, raster_ptr> Feature;
        typedef boost::shared_ptr<Feature>                      feature_ptr;

        feature_ptr mfeature = feature_ptr(new Feature(1));
        mfeature->add_geometry(polygon);

        mapnik::transcoder tr("ISO-8859-15");

        boost::put(*mfeature, "geometry", tr.transcode("polygon"));

        mDatasource->push(mfeature);

        break;
        }
      case FEATURE_MULTIPOINT:
        {
        itkExceptionMacro(
          << "This type (FEATURE_MULTIPOINT) is not handle (yet) by VectorDataToImageFilter(), please request for it");
        break;
        }
      case FEATURE_MULTILINE:
        {
        itkExceptionMacro(
          << "This type (FEATURE_MULTILINE) is not handle (yet) by VectorDataToImageFilter(), please request for it");
        break;
        }
      case FEATURE_MULTIPOLYGON:
        {
        itkExceptionMacro(
          << "This type (FEATURE_MULTIPOLYGON) is not handle (yet) by VectorDataToImageFilter(), please request for it");
        break;
        }
      case FEATURE_COLLECTION:
        {
        itkExceptionMacro(
          << "This type (FEATURE_COLLECTION) is not handle (yet) by VectorDataToImageFilter(), please request for it");
        break;
        }
      }
    }
}

/**
 * PrintSelf Method
 */
template <class TVectorData, class TImage>
void
VectorDataToImageFilter<TVectorData, TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
}

#endif
