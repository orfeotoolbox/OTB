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
  //std::cout << "before GetOutput" << std::endl;
  OutputGISTableType * output = this->GetOutput();
  //std::cout << "after GetOutput" << std::endl;
  const InputVectorDataType * input = this->GetInput();
  /*
  //Delete the previous GIS Table if exist
  if ( m_CreateGISTable )
  {
    ;  
  }
  //Copy DataNodes info into the GIS table
  //
  */
  
  
  for (unsigned int idx = 0; idx < this->GetNumberOfInputs(); ++idx)
  {
    if (this->GetInput(idx))
    {
      InputVectorDataConstPointer input = this->GetInput(idx);
      InternalTreeNodeType * inputRoot = const_cast<InternalTreeNodeType *>(input->GetDataTree()->GetRoot());
      //std::cout << "ProcessNode" << std::endl;
      ProcessNode(inputRoot);
      
      //std::stringstream layerName;
      //layerName << "layer-" << idx;
    }
  }
  
 }

template<class TVectorData , class TGISTable>
    void
        VectorDataToGISTableFilter< TVectorData, TGISTable >
  ::ProcessNode(InternalTreeNodeType * source)
{

  

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
      {
        //InsertBegin(); 
        this->GetOutput()->InsertPoint( static_cast<typename TGISTable::PointType> (dataNode->GetPoint()) );
        break;
      }
      case otb::FEATURE_LINE:
      {
        this->GetOutput()->InsertLineString( static_cast<typename TGISTable::LinePointerType> (dataNode->GetLine()) );
        break;
      }
      case FEATURE_POLYGON:
      {
        //std::cout << "before insert polygon" << std::endl;
        this->GetOutput()->InsertPolygons( static_cast<typename TGISTable::PolygonConstPointerType> (dataNode->GetPolygonExteriorRing()), static_cast<typename TGISTable::PolygonListConstPointerType> (dataNode->GetPolygonInteriorRings()) );
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
