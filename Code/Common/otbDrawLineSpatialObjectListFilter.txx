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
#ifndef __otbDrawLineSpatialObjectListFilter_txx
#define __otbDrawLineSpatialObjectListFilter_txx

#include "otbDrawLineSpatialObjectListFilter.h"

#include "itkDataObject.h"
#include "itkExceptionObject.h"
#include "itkImageRegionIterator.h"

#include <math.h>


namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
DrawLineSpatialObjectListFilter<TInputImage, TOutputImage>::DrawLineSpatialObjectListFilter()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);

  m_DrawLineFilter = DrawLineType::New();
  m_RescaleFilter = RescalerType::New();

}


template <class TInputImage, class TOutputImage>
void
DrawLineSpatialObjectListFilter<TInputImage, TOutputImage>
::SetInputLineSpatialObjectList(const LinesListType * list)
{
  this->ProcessObjectType::SetNthInput(1,
                                       const_cast< LinesListType * >( list ) );

}


template <class TInputImage, class TOutputImage>
typename DrawLineSpatialObjectListFilter<TInputImage, TOutputImage>::LinesListType *
DrawLineSpatialObjectListFilter<TInputImage, TOutputImage>
::GetInputLineSpatialObjectList(void)
{
  //ROMAIN
  return static_cast</*const */LinesListType *>
         (this->ProcessObjectType::GetInput(1) );
}

template <class TInputImage, class TOutputImage>
void
DrawLineSpatialObjectListFilter<TInputImage, TOutputImage>
::GenerateData(void)
{

  typename InputImageType::ConstPointer input  = this->GetInput();
  typename OutputImageType::Pointer output  = this->GetOutput();
  typename LinesListType::Pointer    list = this->GetInputLineSpatialObjectList();


  typename OutputImageType::RegionType region;
  region.SetSize(input->GetLargestPossibleRegion().GetSize());
  region.SetIndex(input->GetLargestPossibleRegion().GetIndex());
  output->SetRegions( region );
  output->SetOrigin(input->GetOrigin());
  output->SetSpacing(input->GetSpacing());
  output->Allocate();
  output->FillBuffer(0);

  m_RescaleFilter->SetOutputMinimum( itk::NumericTraits< OutputPixelType >::min());
  m_RescaleFilter->SetOutputMaximum( itk::NumericTraits< OutputPixelType >::max());

  m_RescaleFilter->SetInput( input );

  LineListIterator  itList = list->begin();
  m_DrawLineFilter->SetInputImage( m_RescaleFilter->GetOutput() );
  m_DrawLineFilter->SetInputLine( *itList );
  m_DrawLineFilter->GraftOutput( this->GetOutput() );
  m_DrawLineFilter->Update();
  ++itList;


  // Draw each line of the list
  while ( itList != list->end() )
  {

    m_DrawLineFilter->SetInputImage( this->GetOutput() );
    m_DrawLineFilter->SetInputLine( *itList );
    m_DrawLineFilter->Update();

    ++itList;

  }

  this->GraftOutput( m_DrawLineFilter->GetOutput() );


}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void
DrawLineSpatialObjectListFilter<TInputImage, TOutput>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


} // end namespace otb


#endif
