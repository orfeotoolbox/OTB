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
#ifndef __otbGISTableToVectorDataFilter_txx
#define __otbGISTableToVectorDataFilter_txx

#include "otbGISTableToVectorDataFilter.h"
//#include "itkNumericTraits.h"
//#include "itkProgressReporter.h"
//#include "itkImageRegionConstIteratorWithIndex.h"
//#include "otbGISTable.h"

namespace otb {

template<class TGISTable, class TVectorData >
GISTableToVectorDataFilter<TGISTable, TVectorData>
  ::GISTableToVectorDataFilter()
{/*
  m_BackgroundValue = NumericTraits<OutputImagePixelType>::NonpositiveMin();*/
}


template<class TGISTable, class TVectorData >
void
GISTableToVectorDataFilter<TGISTable, TVectorData>
  ::SetInput(const InputGISTableType *input)
{
// Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast< InputGISTableType * >( input ) );
}

template<class TGISTable, class TVectorData >
void
GISTableToVectorDataFilter<TGISTable, TVectorData>
  ::SetInput(unsigned int idx, const InputGISTableType *input)
{
    // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(idx,
                                         const_cast< InputGISTableType * >( input ) );
}

template<class TGISTable, class TVectorData >
    const typename GISTableToVectorDataFilter<TGISTable, TVectorData>::InputGISTableType *
GISTableToVectorDataFilter<TGISTable, TVectorData>
  ::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
  {
    return 0;
  }

  return static_cast<const TGISTable * >
      (this->itk::ProcessObject::GetInput(0) );
}

template<class TGISTable, class TVectorData >
const typename GISTableToVectorDataFilter<TGISTable, TVectorData>::InputGISTableType *
GISTableToVectorDataFilter<TGISTable, TVectorData>
  ::GetInput(unsigned int idx)
{
  return static_cast<const TGISTable * >
      (this->itk::ProcessObject::GetInput(idx) );
}

template<class TGISTable, class TVectorData >
void 
GISTableToVectorDataFilter<TGISTable, TVectorData>
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

template<class TGISTable, class TVectorData >
void
GISTableToVectorDataFilter<TGISTable, TVectorData>
::GenerateData()
{
  
  // Allocate the output
  this->AllocateOutputs();
  
  OutputVectorDataType * output = this->GetOutput();
  const InputGISTableType * input = this->GetInput();
  
  //ProgressReporter progress( this, 0, output->GetRequestedRegion().GetNumberOfPixels() );

  //AttributeAccessorType accessor;

  //output->FillBuffer( m_BackgroundValue );
  //typedef TConnectionImplementation ConnectionType;
  //typedef typename ConnectionType::Pointer ConnectionPointerType;
  
  ConnectionPointerType conn = ConnectionType::New();
  
  //ConnectionType toto;
  input->GetConnection(); 
  
  //typename InputGISTableType::LabelObjectContainerType::const_iterator it;
  //const typename InputLabelMapType::LabelObjectContainerType & labelObjectContainer = //input->GetLabelObjectContainer();
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


template<class TGISTable, class TVectorData >
void
GISTableToVectorDataFilter<TGISTable, TVectorData>
::PrintSelf(std::ostream &os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
/*
  os << indent << "BackgroundValue: "  << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_BackgroundValue) << std::endl;
  */
}

}// end namespace otb
#endif
