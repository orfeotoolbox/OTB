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
#ifndef __otbGISTableToLabelMapFilter_txx
#define __otbGISTableToLabelMapFilter_txx

#include "otbGISTableToLabelMapFilter.h"
#include "itkNumericTraits.h"

namespace otb {

template<class TGISTable, class TLabelMap>
GISTableToLabelMapFilter<TGISTable, TLabelMap>
::GISTableToLabelMapFilter()
{
  m_BackgroundValue = itk::NumericTraits<OutputLabelMapPixelType>::max();
  m_GISTableToVectorDataFilter = GISTableToVectorDataFilterType::New();
  m_VectorDataToLabelMapFilter = VectorDataToLabelMapFilterType::New();

  m_VectorDataToLabelMapFilter->SetInput(m_GISTableToVectorDataFilter->GetOutput());
}

template<class TGISTable, class TLabelMap>
void
GISTableToLabelMapFilter<TGISTable, TLabelMap>
::SetInput(const InputGISTableType *input)
{
// Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<InputGISTableType *>(input));
}

template<class TGISTable, class TLabelMap>
void
GISTableToLabelMapFilter<TGISTable, TLabelMap>
::SetInput(unsigned int idx, const InputGISTableType *input)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(idx,
                                        const_cast<InputGISTableType *>(input));
}

template<class TGISTable, class TLabelMap>
const typename GISTableToLabelMapFilter<TGISTable, TLabelMap>::InputGISTableType *
GISTableToLabelMapFilter<TGISTable, TLabelMap>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const TGISTable *>
           (this->itk::ProcessObject::GetInput(0));
}

template<class TGISTable, class TLabelMap>
const typename GISTableToLabelMapFilter<TGISTable, TLabelMap>::InputGISTableType *
GISTableToLabelMapFilter<TGISTable, TLabelMap>
::GetInput(unsigned int idx)
{
  return static_cast<const TGISTable *>
           (this->itk::ProcessObject::GetInput(idx));
}

template<class TGISTable, class TLabelMap>
void
GISTableToLabelMapFilter<TGISTable, TLabelMap>
::GenerateInputRequestedRegion()
{ /*
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  if ( !input )
    { return; }
  input->SetRequestedRegion( input->GetLargestPossibleRegion() ); */
}

/*
template<class TGISTable, class TLabelMap >
void
GISTableToLabelMapFilter<TGISTable, TLabelMap>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}
*/

/**
 * overloaded because pb during copyinformation
 */
template <class TGISTable, class TLabelMap>
void GISTableToLabelMapFilter<TGISTable, TLabelMap>
::GenerateOutputInformation()
{
  // we can't call the superclass method here.

  // get pointers to the input and output
  const InputGISTableType * inputPtr  = this->GetInput();
  OutputLabelMapType *      outputPtr = this->GetOutput();

  if (!inputPtr || !outputPtr)
    {
    return;
    }

  // Set the size of the output region
  typename OutputLabelMapType::RegionType outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize(m_Size);
  outputLargestPossibleRegion.SetIndex(m_StartIndex);
  outputPtr->SetLargestPossibleRegion(outputLargestPossibleRegion);

  //Set spacing and origin
  outputPtr->SetSpacing(m_GISTableToVectorDataFilter->GetOutput()->GetSpacing());
  outputPtr->SetOrigin(m_GISTableToVectorDataFilter->GetOutput()->GetOrigin());

}

template<class TGISTable, class TLabelMap>
void
GISTableToLabelMapFilter<TGISTable, TLabelMap>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

//   std::cout << input->GetConnection()->GetDBName()<< std::endl;
//   std::cout << input->GetTableName() << std::endl;
  m_GISTableToVectorDataFilter->SetInput(this->GetInput());
  //With set background to numericTrait::max() //TODO use the value set by the user
  m_BackgroundValue = itk::NumericTraits<OutputLabelMapPixelType>::max();
  m_VectorDataToLabelMapFilter->SetBackgroundValue(this->GetBackgroundValue());

  //Set size of the final image
  m_VectorDataToLabelMapFilter->SetSize(m_Size);
  m_VectorDataToLabelMapFilter->SetStartIndex(m_StartIndex);

  OutputLabelMapType * output = this->GetOutput();

  output = m_VectorDataToLabelMapFilter->GetOutput();

  m_VectorDataToLabelMapFilter->Update();

}

template<class TGISTable, class TLabelMap>
void
GISTableToLabelMapFilter<TGISTable, TLabelMap>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "GISTableToVectorData:"  << this->m_GISTableToVectorDataFilter << std::endl;

}

} // end namespace otb
#endif
