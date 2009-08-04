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
//#include "itkNumericTraits.h"
//#include "itkProgressReporter.h"
//#include "itkImageRegionConstIteratorWithIndex.h"


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
  
  // Allocate the output
  this->AllocateOutputs();
  
  OutputVectorDataType * output = this->GetOutput();
  const InputLabelMapType * input = this->GetInput();
  
  //ProgressReporter progress( this, 0, output->GetRequestedRegion().GetNumberOfPixels() );

  //AttributeAccessorType accessor;

  //output->FillBuffer( m_BackgroundValue );

  typename InputLabelMapType::LabelObjectContainerType::const_iterator it;
  const typename InputLabelMapType::LabelObjectContainerType & labelObjectContainer = input->GetLabelObjectContainer();
  
  //typedef OutputVectorDataType::DataNodeType DataNodeType;
  DataNodePointerType document = DataNodeType::New();
  DataNodePointerType folder1 = DataNodeType::New();
  
  document->SetNodeType(DOCUMENT);
  folder1->SetNodeType(FOLDER);
  //typedef otb::Functor::LabelObjectToPolygonFunctor<LabelObjectType,PolygonType> FunctorType;
  
  /*
  for( it = labelObjectContainer.begin(); it != labelObjectContainer.end(); it++ )
    {
    const typename InputImageType::LabelObjectType * labeObject = it->second;
    const AttributeValueType & attribute = accessor( labeObject );

    typename InputImageType::LabelObjectType::LineContainerType::const_iterator lit;
    const typename InputImageType::LabelObjectType::LineContainerType & lineContainer = labeObject->GetLineContainer();

    for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
      {
      IndexType idx = lit->GetIndex();
      unsigned long length = lit->GetLength();
      for( unsigned int i=0; i<length; i++)
        {
        output->SetPixel( idx, static_cast<OutputImagePixelType>( attribute ) );
        idx[0]++;
        progress.CompletedPixel();
        }
      }
    }
  */
}


template<class TLabelMap, class TVectorData >
void
LabelMapToVectorDataFilter<TLabelMap, TVectorData>
::PrintSelf(std::ostream &os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
/*
  os << indent << "BackgroundValue: "  << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_BackgroundValue) << std::endl;
  */
}

}// end namespace otb
#endif
