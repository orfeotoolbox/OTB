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
  m_SensorModelFlip = 1;
  m_ScaleFactor = 1.0;
  m_VectorDataProjectionProj4 = "";
  m_VectorDataProjectionWKT = "";
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

  itk::MetaDataDictionary& dict = outputPtr->GetMetaDataDictionary();
  itk::EncapsulateMetaData<std::string> (dict, MetaDataKey::ProjectionRefKey,
                                           static_cast<std::string>(m_VectorDataProjectionWKT));

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

  //Font Handling
  if(!m_FontFileName.empty())
    mapnik::freetype_engine::register_font(m_FontFileName);

  //Handle the style type using helper class
  otb::VectorDataStyle::Pointer styleLoader = otb::VectorDataStyle::New();
  styleLoader->SetScaleFactor(m_ScaleFactor);

  //We assume that all the data are reprojected before using OTB.
  VectorDataConstPointer input = this->GetInput();
  //Converting the projection string to the proj.4 format
  itk::ExposeMetaData<std::string>(
    input->GetMetaDataDictionary(), MetaDataKey::ProjectionRefKey,  m_VectorDataProjectionWKT);
  otbMsgDebugMacro(<< "WKT -> " << m_VectorDataProjectionWKT);

  m_SensorModelFlip = 1;

  if (m_VectorDataProjectionWKT == "")
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
    OGRSpatialReferenceH oSRS = OSRNewSpatialReference(m_VectorDataProjectionWKT.c_str());
    char * pszProj4;
    OSRExportToProj4(oSRS, &pszProj4);
    m_VectorDataProjectionProj4 = pszProj4;
    CPLFree(pszProj4);
    OSRRelease(oSRS);
    m_SensorModelFlip =  1;
    otbMsgDevMacro(<< "The output map will be carto/geo geometry");
    }
  otbMsgDebugMacro(<< "Proj.4 -> " << m_VectorDataProjectionProj4);

  //Internal Tiling Support
  //Workaround to overcome mapnik maximum tile size limitation
  ImagePointer output = this->GetOutput();
  RegionType   requestedRegion = output->GetRequestedRegion();
  otbMsgDevMacro("requestedRegion: " << requestedRegion);

  m_NbTile = (vcl_pow(std::max(vcl_floor((double)requestedRegion.GetSize()[0] / 16000),
                          vcl_floor((double)requestedRegion.GetSize()[1] / 16000))+1, 2));

  //std::cout << "nbTile: " << m_NbTile << std::endl;
  //std::cout << "requestedRegion: " << requestedRegion << std::endl;

  m_TilingRegions.resize(m_NbTile);
  m_Maps.resize(m_NbTile);
  m_VectorDataExtractors.resize(m_NbTile);

  unsigned int tilingRegionsIdx = 0;
  unsigned int stdXOffset;
  unsigned int stdYOffset;

  stdXOffset = vcl_floor((double)requestedRegion.GetSize()[0]/ (m_NbTile/2))+1;
  stdYOffset = vcl_floor((double)requestedRegion.GetSize()[1]/ (m_NbTile/2))+1;

  for(unsigned int i=0; i < vcl_floor((double)(m_NbTile)/2 + 0.5); ++i)
    {
    for(unsigned int j=0; j < vcl_floor((double)(m_NbTile)/2 + 0.5); ++j)
      {
      //Set Regions
      SizeType size;
      IndexType index;
      if(m_NbTile == 1)
        {
        index = requestedRegion.GetIndex();
        size = requestedRegion.GetSize();
        }
      else
        {
        index[0] = requestedRegion.GetIndex()[0] + i * stdXOffset;
        index[1] = requestedRegion.GetIndex()[1] + j * stdYOffset;

        size[0] = std::min((unsigned int)(requestedRegion.GetSize()[0] - index[0]), stdXOffset);
        size[1] = std::min((unsigned int)(requestedRegion.GetSize()[1] - index[1]), stdYOffset);
        }
      m_TilingRegions[tilingRegionsIdx].SetIndex(index);
      m_TilingRegions[tilingRegionsIdx].SetSize(size);

      //std::cout << "tileRegions[" << tilingRegionsIdx << "] : "
      //          << m_TilingRegions[tilingRegionsIdx] << std::endl;

      //Set Maps
      m_Maps[tilingRegionsIdx] = mapnik::Map();

      ////Load the style type
      switch (m_RenderingStyleType)
        {
        case OSM:
        {
        styleLoader->LoadOSMStyle(m_Maps[tilingRegionsIdx]);
        if (m_UseAsOverlay)
          {
          //Set the default backgroup to transparent
          m_Maps[tilingRegionsIdx].set_background(mapnik::color(255, 255, 255, 0));
          }
        else
          {
          m_Maps[tilingRegionsIdx].set_background(mapnik::color("#b5d0d0"));
          }
        break;
        }
        case Binary:
        {
        styleLoader->LoadBinaryRasterizationStyle(m_Maps[tilingRegionsIdx]);
        //Set the backgroup to white
        m_Maps[tilingRegionsIdx].set_background(mapnik::color("#ffffff"));
        break;
        }
        default:
        {
        itkExceptionMacro(<< "Style Type Not Supported!");
        break;
        }
        }

      ////Set Mapnik projection information
      m_Maps[tilingRegionsIdx].set_srs(m_VectorDataProjectionProj4);

      //Set VectorData extracts
      m_VectorDataExtractors[tilingRegionsIdx].resize(this->GetNumberOfInputs());
      for (unsigned int idx = 0; idx < this->GetNumberOfInputs(); ++idx)
        {
        if (this->GetInput(idx))
          {
          RemoteSensingRegionType rsRegion;
          SizePhyType             sizePhy;
          sizePhy[0] = size[0] * m_Spacing[0];
          sizePhy[1] = size[1] * m_Spacing[1];
          rsRegion.SetSize(sizePhy);
          OriginType              origin;
          origin[0] = m_Origin[0] + (static_cast<double>(index[0]) - 0.5) * m_Spacing[0];
          origin[1] = m_Origin[1] + (static_cast<double>(index[1]) - 0.5) * m_Spacing[1];
          rsRegion.SetOrigin(origin);
          rsRegion.SetRegionProjection(m_VectorDataProjectionWKT);

          //std::cout << "m_SensorModelFlip: " << m_SensorModelFlip << std::endl;
          //std::cout << "rsTileRegions[" << tilingRegionsIdx << "] : "
          //          << rsRegion << std::endl;

          m_VectorDataExtractors[tilingRegionsIdx][idx] = VectorDataExtractROIType::New();
          m_VectorDataExtractors[tilingRegionsIdx][idx]->SetRegion(rsRegion);
          m_VectorDataExtractors[tilingRegionsIdx][idx]->SetInput(this->GetInput(idx));
          }
        }

      tilingRegionsIdx ++;
      }
    }
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

  if (m_StyleList.size() == 0)
    {
    switch (m_RenderingStyleType)
        {
        case OSM:
        {
        //Add default styles
        itkExceptionMacro(<< "No style is provided for the vector data");
        break;
        }
        case Binary:
        {
        //Add default styles
        this->AddStyle("binary-rasterization");
        break;
        }
        default:
        {
        itkExceptionMacro(<< "No style is provided for the vector data");
        break;
        }
        }
    }

  ImagePointer output = this->GetOutput();

  for (unsigned int tileIdx = 0; tileIdx < m_NbTile; ++tileIdx)
    {
    //Delete the previous layers from the map
    int numberLayer = mapnik_otb::get_num_layer(m_Maps[tileIdx]);
    for (int i = numberLayer - 1; i >= 0; i--) //yes, int.
      {
      m_Maps[tileIdx].removeLayer(i);
      }
    m_Maps[tileIdx].resize(m_TilingRegions[tileIdx].GetSize()[0], m_TilingRegions[tileIdx].GetSize()[1]);
  
    RemoteSensingRegionType rsRegion;

    for (unsigned int vdIdx = 0; vdIdx < this->GetNumberOfInputs(); ++vdIdx)
      {
      if (this->GetInput(vdIdx))
        {
        datasource_ptr mDatasource = datasource_ptr(new mapnik::memory_datasource);

        rsRegion = m_VectorDataExtractors[tileIdx][vdIdx]->GetRegion();

        m_VectorDataExtractors[tileIdx][vdIdx]->Update();
        VectorDataConstPointer input = m_VectorDataExtractors[tileIdx][vdIdx]->GetOutput();
        InternalTreeNodeType * inputRoot = const_cast<InternalTreeNodeType *>(input->GetDataTree()->GetRoot());

        ProcessNode(inputRoot, mDatasource);
        otbMsgDevMacro("Datasource size: " << mDatasource->size());

        std::stringstream layerName;
        layerName << "layer-" << tileIdx;
        mapnik::layer lyr(layerName.str());
        lyr.set_srs(m_VectorDataProjectionProj4);
        lyr.set_datasource(mDatasource);

        for (unsigned int i = 0; i < m_StyleList.size(); ++i)
          {
          lyr.add_style(m_StyleList[i]);
          }

        m_Maps[tileIdx].addLayer(lyr);
        }
      }
    assert((m_SensorModelFlip == 1) || (m_SensorModelFlip == -1));

    mapnik_otb::box2d envelope(
      rsRegion.GetOrigin(0),
      m_SensorModelFlip*(rsRegion.GetOrigin(1) + rsRegion.GetSize(1)),
      rsRegion.GetOrigin(0) + rsRegion.GetSize(0),
      m_SensorModelFlip*(rsRegion.GetOrigin(1))
      );

    mapnik_otb::zoom_to_box(&m_Maps[tileIdx], envelope);
    otbMsgDebugMacro(<< "Envelope: " << envelope);

    otbMsgDebugMacro(<< "Map scale: " << m_Maps[tileIdx].scale_denominator());
    mapnik::image_32 buf(mapnik_otb::get_width(m_Maps[tileIdx]),
                         mapnik_otb::get_height(m_Maps[tileIdx]));
    mapnik::agg_renderer<mapnik::image_32> ren(m_Maps[tileIdx], buf);
    ren.apply();

    const unsigned char * src = buf.raw_data();

    itk::ImageRegionIterator<ImageType> it(output, m_TilingRegions[tileIdx]);

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
    }
}

template <class TVectorData, class TImage>
void
VectorDataToImageFilter<TVectorData, TImage>
::ProcessNode(InternalTreeNodeType * source, datasource_ptr mDatasource)
{
  typedef typename VectorDataType::DataNodeType DataNodeType;
  typedef typename DataNodeType::Pointer        DataNodePointerType;

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
        mapnik_otb::geom* point = mapnik_otb::create_geom(mapnik::Point);

        point->move_to(dataNode->GetPoint()[0], m_SensorModelFlip * dataNode->GetPoint()[1]);
//           std::cout << dataNode->GetPoint()[0] << ", " << dataNode->GetPoint()[1] << std::endl;

        typedef boost::shared_ptr<mapnik::raster> raster_ptr;
        typedef mapnik::feature<mapnik_otb::geom, raster_ptr> Feature;
        typedef boost::shared_ptr<Feature>        feature_ptr;

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
        mapnik_otb::geom* line = mapnik_otb::create_geom(mapnik::LineString);

        typedef typename DataNodeType::LineType::VertexListConstIteratorType VertexIterator;
        VertexIterator itVertex = dataNode->GetLine()->GetVertexList()->Begin();
        while (itVertex != dataNode->GetLine()->GetVertexList()->End())
          {
//             std::cout << itVertex.Value()[0] << ", " << itVertex.Value()[1] << std::endl;
          line->line_to(itVertex.Value()[0], m_SensorModelFlip * itVertex.Value()[1]);
          ++itVertex;
          }

//           std::cout << "Num points: " << line->num_points() << std::endl;

        typedef boost::shared_ptr<mapnik::raster>               raster_ptr;
        typedef mapnik::feature<mapnik_otb::geom, raster_ptr> Feature;
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
        mapnik_otb::geom* polygon = mapnik_otb::create_geom(mapnik::Polygon);

        typedef typename DataNodeType::PolygonType::VertexListConstIteratorType VertexIterator;
        VertexIterator itVertex = dataNode->GetPolygonExteriorRing()->GetVertexList()->Begin();
        while (itVertex != dataNode->GetPolygonExteriorRing()->GetVertexList()->End())
          {
          polygon->line_to(itVertex.Value()[0], m_SensorModelFlip * itVertex.Value()[1]);
          ++itVertex;
          }

        typedef boost::shared_ptr<mapnik::raster> raster_ptr;
        typedef mapnik::feature<mapnik_otb::geom, raster_ptr> Feature;
        typedef boost::shared_ptr<Feature>        feature_ptr;

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
