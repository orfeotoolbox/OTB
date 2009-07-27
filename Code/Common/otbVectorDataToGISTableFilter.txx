/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelMapToAttributeImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbVectorDataToGISTableFilter_txx
#define __otbVectorDataToGISTableFilter_txx

#include "otbVectorDataToGISTableFilter.h"
//#include "itkNumericTraits.h"
//#include "itkProgressReporter.h"
//#include "itkImageRegionConstIteratorWithIndex.h"
//#include "otbGISTable.h"

namespace otb {

template<class TVectorData , class TGISTable>
VectorDataToGISTableFilter< TVectorData, TGISTable >
  ::VectorDataToGISTableFilter()
{/*
  m_BackgroundValue = NumericTraits<OutputImagePixelType>::NonpositiveMin();*/
}


template<class TVectorData , class TGISTable>
void
VectorDataToGISTableFilter< TVectorData, TGISTable >
  ::SetInput(const InputVectorDataType *input)
{
// Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast< InputVectorDataType * >( input ) );
}

template<class TVectorData , class TGISTable>
void
VectorDataToGISTableFilter< TVectorData, TGISTable >
  ::SetInput(unsigned int idx, const InputVectorDataType *input)
{
    // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(idx,
                                        const_cast< InputVectorDataType * >( input ) );
}

template<class TVectorData , class TGISTable>
    const typename VectorDataToGISTableFilter< TVectorData, TGISTable >::InputVectorDataType *
VectorDataToGISTableFilter< TVectorData, TGISTable >
  ::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
  {
    return 0;
  }

  return static_cast<const TVectorData * >
      (this->itk::ProcessObject::GetInput(0) );
}

template<class TVectorData , class TGISTable>
    const typename VectorDataToGISTableFilter< TVectorData, TGISTable >::InputVectorDataType *
VectorDataToGISTableFilter< TVectorData, TGISTable >
  ::GetInput(unsigned int idx)
{
  return static_cast<const TVectorData * >
      (this->itk::ProcessObject::GetInput(idx) );
}

template<class TVectorData , class TGISTable>
void 
VectorDataToGISTableFilter< TVectorData, TGISTable >
::GenerateInputRequestedRegion()
{/*
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  if ( !input )
    { return; }
  input->SetRequestedRegion( input->GetLargestPossibleRegion() );*/
}

/*
template<class TInputImage, class TVectorData >
void 
LabelMapToVectorDataFilter<TInputImage, TVectorData>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}
*/

template<class TVectorData , class TGISTable>
void
VectorDataToGISTableFilter< TVectorData, TGISTable >
::GenerateData()
{
  
  // Allocate the output
  this->AllocateOutputs();
  
  OutputGISTableType * output = this->GetOutput();
  const InputVectorDataType * input = this->GetInput();
  
  
  
  InternalTreeNodeType * inputRoot = const_cast<InternalTreeNodeType *>(input->GetDataTree()->GetRoot());
  
 }

template<class TVectorData , class TGISTable>
    void
        VectorDataToGISTableFilter< TVectorData, TGISTable >
  ::ProcessNode(InternalTreeNodeType * source)
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
        ProcessNode((*it));
        break;
      }
      case otb::DOCUMENT:
      {
        ProcessNode((*it));
        break;
      }
      case otb::FOLDER:
      {
        ProcessNode((*it));
        break;
      }
      case FEATURE_POINT:
      {/*
//           itkExceptionMacro(<<"This type (FEATURE_POINT) is not handle (yet) by VectorDataToImageFilter(), please request for it");
//           std::cout << std::setprecision(15);
//           std::cout << " ** Inserting new point **" << std::endl;

        typedef mapnik::vertex<double,2> vertex2d;
        typedef mapnik::point<vertex2d> point2d;
        typedef boost::shared_ptr<point2d> point_ptr;
        mapnik::geometry2d * point = new point2d;

        point->move_to(dataNode->GetPoint()[0],dataNode->GetPoint()[1]);
//           std::cout << dataNode->GetPoint()[0] << ", " << dataNode->GetPoint()[1] << std::endl;



        typedef boost::shared_ptr<mapnik::raster> raster_ptr;
        typedef mapnik::feature<mapnik::geometry2d,raster_ptr> Feature;
        typedef boost::shared_ptr<Feature> feature_ptr;

        feature_ptr mfeature = feature_ptr(new Feature(1));
        mfeature->add_geometry(point);

        mapnik::transcoder tr("ISO-8859-15");

        if (dataNode->HasField("place_name"))
          boost::put(*mfeature, "name", tr.transcode((dataNode->GetFieldAsString("place_name")).c_str()));

        boost::put(*mfeature, "place", tr.transcode("city"));
        boost::put(*mfeature, "capital", tr.transcode("yes"));//FIXME more a question of style

        mDatasource->push(mfeature);
*/
        break;
      }
      case otb::FEATURE_LINE:
      {
//           std::cout << std::setprecision(15);
//           std::cout << " ** Inserting new line **" << std::endl;
        /*typedef mapnik::vertex<double,2> vertex2d;
        typedef mapnik::line_string<vertex2d,mapnik::vertex_vector2> line2d;
        typedef boost::shared_ptr<line2d> line_ptr;
        mapnik::geometry2d * line = new line2d;

        typedef DataNodeType::LineType::VertexListConstIteratorType VertexIterator;
        VertexIterator itVertex = dataNode->GetLine()->GetVertexList()->Begin();
        while (itVertex != dataNode->GetLine()->GetVertexList()->End())
        {
//             std::cout << itVertex.Value()[0] << ", " << itVertex.Value()[1] << std::endl;
          line->line_to(itVertex.Value()[0],m_SensorModelFlip*itVertex.Value()[1]);
          ++itVertex;
        }

//           std::cout << "Num points: " << line->num_points() << std::endl;


        typedef boost::shared_ptr<mapnik::raster> raster_ptr;
        typedef mapnik::feature<mapnik::geometry2d,raster_ptr> Feature;
        typedef boost::shared_ptr<Feature> feature_ptr;

        feature_ptr mfeature = feature_ptr(new Feature(1));
        mfeature->add_geometry(line);

        mapnik::transcoder tr("ISO-8859-15");

        if (dataNode->HasField("name"))
          boost::put(*mfeature, "name", tr.transcode((dataNode->GetFieldAsString("name")).c_str()));
        if (dataNode->HasField("NAME"))
          boost::put(*mfeature, "name", tr.transcode((dataNode->GetFieldAsString("NAME")).c_str()));

//           std::cout << mfeature->props().size() << std::endl;
//           std::cout << " -> " << (*mfeature)["name"] << std::endl;

//           std::cout << "Name: " << dataNode->GetFieldAsString("NAME") << std::endl;
//           std::cout << "Type: " << dataNode->GetFieldAsString("TYPE") << std::endl;
//           std::cout << "OSM ID: " << dataNode->GetFieldAsString("osm_id") << std::endl;

        if (dataNode->HasField("type"))
          boost::put(*mfeature, "highway", tr.transcode((dataNode->GetFieldAsString("type")).c_str()));
        if (dataNode->HasField("TYPE"))
          boost::put(*mfeature, "highway", tr.transcode((dataNode->GetFieldAsString("TYPE")).c_str()));


        mDatasource->push(mfeature);
*/
        break;
      }
      case FEATURE_POLYGON:
      {
//         itkExceptionMacro(<<"This type (FEATURE_POLYGON) is not handle (yet) by VectorDataToImageFilter(), please request for it");
        /*typedef mapnik::vertex<double,2> vertex2d;
        typedef mapnik::polygon<vertex2d,mapnik::vertex_vector2> polygon2d;
        typedef boost::shared_ptr<polygon2d> polygon_ptr;
        mapnik::geometry2d * polygon = new polygon2d;

        typedef DataNodeType::PolygonType::VertexListConstIteratorType VertexIterator;
        VertexIterator itVertex = dataNode->GetPolygonExteriorRing()->GetVertexList()->Begin();
        while (itVertex != dataNode->GetPolygonExteriorRing()->GetVertexList()->End())
        {
          polygon->line_to(itVertex.Value()[0],m_SensorModelFlip*itVertex.Value()[1]);
          ++itVertex;
        }

        typedef boost::shared_ptr<mapnik::raster> raster_ptr;
        typedef mapnik::feature<mapnik::geometry2d,raster_ptr> Feature;
        typedef boost::shared_ptr<Feature> feature_ptr;

        feature_ptr mfeature = feature_ptr(new Feature(1));
        mfeature->add_geometry(polygon);
        mDatasource->push(mfeature);
*/
        break;
      }
      case FEATURE_MULTIPOINT:
      {
        itkExceptionMacro(<<"This type (FEATURE_MULTIPOINT) is not handle (yet) by VectorDataToImageFilter(), please request for it");
        break;
      }
      case FEATURE_MULTILINE:
      {
        itkExceptionMacro(<<"This type (FEATURE_MULTILINE) is not handle (yet) by VectorDataToImageFilter(), please request for it");
        break;
      }
      case FEATURE_MULTIPOLYGON:
      {
        itkExceptionMacro(<<"This type (FEATURE_MULTIPOLYGON) is not handle (yet) by VectorDataToImageFilter(), please request for it");
        break;
      }
      case FEATURE_COLLECTION:
      {
        itkExceptionMacro(<<"This type (FEATURE_COLLECTION) is not handle (yet) by VectorDataToImageFilter(), please request for it");
        break;
      }
    }
  }
}

template<class TVectorData , class TGISTable>
void
VectorDataToGISTableFilter< TVectorData, TGISTable >
::PrintSelf(std::ostream &os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
/*
  os << indent << "BackgroundValue: "  << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_BackgroundValue) << std::endl;
  */
}

}// end namespace otb
#endif
