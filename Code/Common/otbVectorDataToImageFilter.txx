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

#include "otbVectorDataToImageFilter.h"
#include "itkImageRegionIterator.h"
#include "otbVectorDataStyle.h"

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
  ::VectorDataToImageFilter()
  {
    this->SetNumberOfRequiredInputs( 1 );
    m_Spacing.Fill(1.0);
    m_Origin.Fill(0.0);
    m_Direction.SetIdentity();
    m_Size.Fill( 0 );
    m_StartIndex.Fill( 0 );
    m_Map = mapnik::Map();
  }


  template <class TVectorData, class TImage>
      void
          VectorDataToImageFilter<TVectorData, TImage>
  ::SetInput(const VectorDataType *input)
  {
// Process object is not const-correct so the const_cast is required here
    this->itk::ProcessObject::SetNthInput(0,
                                          const_cast< VectorDataType * >( input ) );
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

    return static_cast<const TVectorData * >
        (this->itk::ProcessObject::GetInput(0) );
  }



//----------------------------------------------------------------------------
  template <class TVectorData, class TImage>
      void
          VectorDataToImageFilter<TVectorData, TImage>
  ::SetSpacing(const SpacingType & spacing )
  {
    if ( this->m_Spacing != spacing )
    {
      this->m_Spacing = spacing;
      this->Modified();
    }
  }


//----------------------------------------------------------------------------
  template <class TVectorData, class TImage>
      void
          VectorDataToImageFilter<TVectorData, TImage>
  ::SetSpacing(const double spacing[2] )
  {
    SpacingType s(spacing);
    this->SetSpacing(s);
  }


//----------------------------------------------------------------------------
  template <class TVectorData, class TImage>
      void
          VectorDataToImageFilter<TVectorData, TImage>
  ::SetSpacing(const float spacing[2] )
  {
    itk::Vector<float, 2> sf(spacing);
    SpacingType s;
    s.CastFrom( sf );
    this->SetSpacing(s);
  }

//----------------------------------------------------------------------------
  template <class TVectorData, class TImage>
      void
          VectorDataToImageFilter<TVectorData, TImage>
  ::SetOrigin(const double origin[2] )
  {
    OriginType p(origin);
    this->SetOrigin( p );
  }


//----------------------------------------------------------------------------
  template <class TVectorData, class TImage>
      void
          VectorDataToImageFilter<TVectorData, TImage>
  ::SetOrigin(const float origin[2] )
  {
    itk::Point<float, 2> of(origin);
    OriginType p;
    p.CastFrom( of );
    this->SetOrigin( p );
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
    if ( !outputPtr )
    {
      return;
    }

    // Set the size of the output region
    typename TImage::RegionType outputLargestPossibleRegion;
    outputLargestPossibleRegion.SetSize( m_Size );
    outputLargestPossibleRegion.SetIndex( m_StartIndex );
    outputPtr->SetLargestPossibleRegion( outputLargestPossibleRegion );

    // Set spacing and origin
    outputPtr->SetSpacing( m_Spacing );
    outputPtr->SetOrigin( m_Origin );
    outputPtr->SetDirection( m_Direction );


    //TODO update or check the projection information

    return;
  }

  template <class TVectorData, class TImage>
      void
          VectorDataToImageFilter<TVectorData, TImage>
  ::BeforeThreadedGenerateData(void)
  {
    Superclass::BeforeThreadedGenerateData();
    mapnik::freetype_engine::register_font("/usr/share/fonts/truetype/ttf-dejavu/DejaVuSans.ttf");
    m_Map.resize(m_Size[0],m_Size[1]);

//     m_Map.set_background(mapnik::color("#f2efe9"));
    m_Map.set_background(mapnik::color(255,255,255,0));

    otb::VectorDataStyle::Pointer styleLoader = otb::VectorDataStyle::New();
    styleLoader->LoadOSMStyle(m_Map);


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

    datasource_ptr mDatasource = datasource_ptr(new mapnik::memory_datasource);

    VectorDataConstPointer input = this->GetInput();
    InternalTreeNodeType * inputRoot = const_cast<InternalTreeNodeType *>(input->GetDataTree()->GetRoot());


    //Converting the projection string to the porj.4 format
    std::string vectorDataProjectionWKT;
    itk::ExposeMetaData<std::string>(input->GetMetaDataDictionary(), MetaDataKey::ProjectionRefKey,  vectorDataProjectionWKT);
    std::cout << "WKT -> " << vectorDataProjectionWKT << std::endl;
    std::string vectorDataProjectionProj4;
    if (vectorDataProjectionWKT == "")
    {
      //We assume that it is an image in sensor model geometry
      //and tell mapnik that this is utm
      //(with a resolution of 1m per unit)
      vectorDataProjectionProj4 = "+proj=utm +zone=31 +ellps=WGS84";
    }
    else
    {
      OGRSpatialReference oSRS(vectorDataProjectionWKT.c_str());
      char * pszProj4;
      oSRS.exportToProj4(&pszProj4);
      vectorDataProjectionProj4 = pszProj4;
      CPLFree(pszProj4);
    }
    std::cout << "Proj.4 -> " << vectorDataProjectionProj4 << std::endl;

//      std::string   vectorDataProjectionProj4 = "+proj=utm +zone=31 +ellps=WGS84";
    m_Map.set_srs(vectorDataProjectionProj4);

    ProcessNode(inputRoot,mDatasource);


    std::cout << "Datasource size: " << mDatasource->size() << std::endl;

    mapnik::Layer lyr("world");
    lyr.set_srs(vectorDataProjectionProj4);
    lyr.set_datasource(mDatasource);
//     lyr.add_style("river");
    lyr.add_style("minor-roads-casing");
    lyr.add_style("minor-roads");
    lyr.add_style("roads");
    lyr.add_style("roads-text");

    m_Map.addLayer(lyr);

    mapnik::Envelope<double> envelope(m_Origin[0],
                                     m_Origin[1]+m_Spacing[1]*m_Size[1],
                                     m_Origin[0]+m_Spacing[0]*m_Size[0],
                                     m_Origin[1]);
//     m_Map.zoomToBox(lyr.envelope());//FIXME: use the Origin/Spacing to calculate this
    m_Map.zoomToBox(envelope);
//     std::cout << "Envelope: " << lyr.envelope() << std::endl;
    std::cout << "Envelope: " << envelope << std::endl;

    std::cout << "Map scale: " << m_Map.scale_denominator() << std::endl;
    mapnik::Image32 buf(m_Map.getWidth(),m_Map.getHeight());
    mapnik::agg_renderer<mapnik::Image32> ren(m_Map,buf);
    ren.apply();

    const unsigned char * src = buf.raw_data();

    ImagePointer output = this->GetOutput();
//     unsigned char * dst = output->GetBufferPointer();

//     unsigned int size = 4*m_Size[0]*m_Size[1];//FIXME: lot of assumption here: RGBA, 2 dimensions
//     memcpy(dst, src, size);

    itk::ImageRegionIterator<ImageType> it(output, output->GetLargestPossibleRegion());

    for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
      itk::RGBAPixel<unsigned char> pix;
      pix[0] = *src;
      ++src;
      pix[1] = *src;
      ++src;
      pix[2] = *src;
      ++src;
      pix[3] = *src;
      ++src;
      it.Set(pix);
    }


    this->AfterThreadedGenerateData();
  }



  template <class TVectorData, class TImage>
      void
          VectorDataToImageFilter<TVectorData, TImage>
  ::ProcessNode(InternalTreeNodeType * source, datasource_ptr mDatasource)
  {

    typedef otb::DataNode<double,2,double> DataNodeType; //FIXME check if it should be infered from the input type
    typedef typename DataNodeType::Pointer DataNodePointerType;

  // Get the children list from the input node
    ChildrenListType children = source->GetChildrenList();

  // For each child
    for(typename ChildrenListType::iterator it = children.begin(); it!=children.end();++it)
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
          std::cout << std::setprecision(15);
//           std::cout << " ** Inserting new line **" << std::endl;
          typedef mapnik::vertex<double,2> vertex2d;
          typedef mapnik::line_string<vertex2d,mapnik::vertex_vector2> line2d;
          typedef boost::shared_ptr<line2d> line_ptr;
          mapnik::geometry2d * line = new line2d;

          typedef DataNodeType::LineType::VertexListConstIteratorType VertexIterator;
          VertexIterator itVertex = dataNode->GetLine()->GetVertexList()->Begin();
          while (itVertex != dataNode->GetLine()->GetVertexList()->End())
          {
//             std::cout << itVertex.Value()[0] << ", " << itVertex.Value()[1] << std::endl;
            line->line_to(itVertex.Value()[0],itVertex.Value()[1]);
            ++itVertex;
          }

//           std::cout << "Num points: " << line->num_points() << std::endl;


          typedef boost::shared_ptr<mapnik::raster> raster_ptr;
          typedef mapnik::feature<mapnik::geometry2d,raster_ptr> Feature;
          typedef boost::shared_ptr<Feature> feature_ptr;

          feature_ptr mfeature = feature_ptr(new Feature(1));
           mfeature->add_geometry(line);

          mapnik::transcoder tr("ISO-8859-15");
          boost::put(*mfeature, "name", tr.transcode((dataNode->GetFieldAsString("name")).c_str()));

//           std::cout << mfeature->props().size() << std::endl;
//           std::cout << " -> " << (*mfeature)["name"] << std::endl;

//           std::cout << "Type: " << dataNode->GetFieldAsString("type") << std::endl;
//           std::cout << "OSM ID: " << dataNode->GetFieldAsString("osm_id") << std::endl;
          boost::put(*mfeature, "highway", tr.transcode((dataNode->GetFieldAsString("type")).c_str()));

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