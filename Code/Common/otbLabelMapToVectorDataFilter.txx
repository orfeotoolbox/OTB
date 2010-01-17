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
#ifndef __otbLabelMapToVectorDataFilter_txx
#define __otbLabelMapToVectorDataFilter_txx

#include "otbLabelMapToVectorDataFilter.h"


namespace otb {

template<class TLabelMap, class TVectorData >
LabelMapToVectorDataFilter<TLabelMap, TVectorData>
  ::LabelMapToVectorDataFilter()
{/*
  m_BackgroundValue = NumericTraits<OutputImagePixelType>::NonpositiveMin();*/
}


template<class TLabelMap, class TVectorData >
void
LabelMapToVectorDataFilter<TLabelMap, TVectorData>
  ::SetInput(const InputLabelMapType *input)
{
// Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast< InputLabelMapType * >( input ) );
}

template<class TLabelMap, class TVectorData >
void
LabelMapToVectorDataFilter<TLabelMap, TVectorData>
  ::SetInput(unsigned int idx, const InputLabelMapType *input)
{
    // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(idx,
                                        const_cast< InputLabelMapType * >( input ) );
}

template<class TLabelMap, class TVectorData >
    const typename LabelMapToVectorDataFilter<TLabelMap, TVectorData>::InputLabelMapType *
LabelMapToVectorDataFilter<TLabelMap, TVectorData>
  ::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
  {
    return 0;
  }

  return static_cast<const TLabelMap * >
      (this->itk::ProcessObject::GetInput(0) );
}

template<class TLabelMap, class TVectorData >
    const typename LabelMapToVectorDataFilter<TLabelMap, TVectorData>::InputLabelMapType *
LabelMapToVectorDataFilter<TLabelMap, TVectorData>
  ::GetInput(unsigned int idx)
{
  return static_cast<const TLabelMap * >
      (this->itk::ProcessObject::GetInput(idx) );
}

template<class TLabelMap, class TVectorData >
void
LabelMapToVectorDataFilter<TLabelMap, TVectorData>
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

template<class TLabelMap, class TVectorData >
void
    LabelMapToVectorDataFilter<TLabelMap, TVectorData>
::GenerateData()
{
//   std::cout << "ICI" << std::endl;
  /**Allocate the output*/
//   this->AllocateOutputs();
//   std::cout << "ICI" << std::endl;
  OutputVectorDataType * output = this->GetOutput();
  const InputLabelMapType * input = this->GetInput();
  /**create functors */
  FunctorType functor;
//   SimplifyFunctorType simplifyFunctor;
//   CloseFunctorType closeFunctor;
  CorrectFunctorType correctFunctor;
  /**Create usual root elements of the output vectordata*/
  DataNodePointerType document = DataNodeType::New();
  DataNodePointerType folder1 = DataNodeType::New();
  
  document->SetNodeType(DOCUMENT);
  folder1->SetNodeType(FOLDER);
  DataNodePointerType root = output->GetDataTree()->GetRoot()->Get();
  output->GetDataTree()->Add(document,root);
  output->GetDataTree()->Add(folder1,document);
  
  // Lets begin by declaring the iterator for the objects in the image.
  typename InputLabelMapType::LabelObjectContainerType::const_iterator it;
  // And get the object container to reuse it later
  const typename InputLabelMapType::LabelObjectContainerType & labelObjectContainer = input->GetLabelObjectContainer();
  for( it = labelObjectContainer.begin(); it != labelObjectContainer.end(); it++ )
  {
    // the label is there if we need it, but it can also be found at labelObject->GetLabel().
    // const PType & label = it->first;
    
    /**the label object*/
    LabelObjectType * labelObject = it->second;
    
    /**Get the polygon image of the labelobject using the functor*/
    typename PolygonType::Pointer polygon = functor(labelObject);
    
    /** Erase aligned points*/
//     PolygonPointerType simplifyPolygon = simplifyFunctor(polygon);
    
    
    /**Close polygon if necessary*/
//     PolygonPointerType closePolygon = closeFunctor(simplifyPolygon);
    
    /**correct polygon if necessary*/
    PolygonPointerType correctPolygon = correctFunctor(polygon);
//     std::cout << "correct polygon : " << correctPolygon << std::endl;
    DataNodePointerType node = DataNodeType::New();
    node->SetNodeType(otb::FEATURE_POLYGON);
    
    /**Store the label in the NodeId*/
    std::ostringstream oss;
    oss << labelObject->GetLabel();
    node->SetNodeId(oss.str());
    //TODO hole in the polygon are not handle yet by the functor
    node->SetPolygonExteriorRing(correctPolygon);
    
    /**Add the polygon to the VectorData*/
    output->GetDataTree()->Add(node,folder1);
  }
}
  
template<class TLabelMap, class TVectorData >
void
LabelMapToVectorDataFilter<TLabelMap, TVectorData>
::PrintSelf(std::ostream &os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

}// end namespace otb
#endif
