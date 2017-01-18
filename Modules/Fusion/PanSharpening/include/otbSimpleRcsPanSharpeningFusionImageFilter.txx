/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbSimpleRcsPanSharpeningFusionImageFilter_txx
#define otbSimpleRcsPanSharpeningFusionImageFilter_txx

#include "otbSimpleRcsPanSharpeningFusionImageFilter.h"

namespace otb
{
template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
SimpleRcsPanSharpeningFusionImageFilter
<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>
::SimpleRcsPanSharpeningFusionImageFilter()
{
  // Fix number of required inputs
  this->SetNumberOfRequiredInputs(2);
  this->m_UseNoData = false;

  // Instantiate convolution filter
  m_ConvolutionFilter = ConvolutionFilterType::New();
  m_ConvolutionFilter->NormalizeFilterOn();

  // Set-up default parameters
  m_Radius.Fill(3);
  m_Filter.SetSize(7 * 7);
  m_Filter.Fill(1);

}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
void
SimpleRcsPanSharpeningFusionImageFilter
<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>
::SetPanInput(const TPanImageType *image)
{
  // We have 2 inputs:  an image and a vector image

  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(1,
                                        const_cast<TPanImageType*>(image));
  this->Modified();
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
const TPanImageType *
SimpleRcsPanSharpeningFusionImageFilter
<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>
::GetPanInput(void) const
{
  if (this->GetNumberOfInputs() < 2)
    {
    return ITK_NULLPTR;
    }

  return static_cast<const TPanImageType *>
           (this->itk::ProcessObject::GetInput(1));
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
void
SimpleRcsPanSharpeningFusionImageFilter
<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>
::SetXsInput(const TXsImageType *image)
{
  // We have 2 inputs:  an image and a vector image

  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<TXsImageType*>(image));
  this->Modified();
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
const TXsImageType *
SimpleRcsPanSharpeningFusionImageFilter
<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>
::GetXsInput(void) const
{
  if (this->GetNumberOfInputs() < 1)
    {
    return ITK_NULLPTR;
    }

  return static_cast<const TXsImageType *>
           (this->itk::ProcessObject::GetInput(0));
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
void
SimpleRcsPanSharpeningFusionImageFilter
<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>
::GenerateData()
{
  //Check if size is correct
  typename TPanImageType::SizeType       sizePan;
  typename TXsImageType::SizeType        sizeXs;
  sizePan = this->GetPanInput()->GetLargestPossibleRegion().GetSize();
  sizeXs = this->GetXsInput()->GetLargestPossibleRegion().GetSize();
  if ((sizePan[0] != sizeXs[0]) || (sizePan[1] != sizeXs[1]))
    {
    itkExceptionMacro(<< "SimpleRcsPanSharpeningFusionImageFilter: Wrong Pan/Xs size");
    }

  // Set-up progress reporting
  m_ProgressAccumulator = itk::ProgressAccumulator::New();
  m_ProgressAccumulator->SetMiniPipelineFilter(this);
  m_ProgressAccumulator->RegisterInternalFilter(m_ConvolutionFilter, 0.9);

  m_ConvolutionFilter->SetInput(this->GetPanInput());
  m_ConvolutionFilter->SetRadius(this->m_Radius);
  m_ConvolutionFilter->SetFilter(this->m_Filter);

  // Write no-data flags for Pan image
  std::vector<bool> noDataValuesPanAvailable;
  double tmpNoDataValuePan;
  typename TPanImageType::InternalPixelType noDataValuePan;
  bool retPan = itk::ExposeMetaData<std::vector<bool>>( this->GetPanInput()->GetMetaDataDictionary(), MetaDataKey::NoDataValueAvailable, noDataValuesPanAvailable );
  itk::ExposeMetaData<double>( this->GetPanInput()->GetMetaDataDictionary(), MetaDataKey::NoDataValue, tmpNoDataValuePan );
  noDataValuePan = static_cast<typename TPanImageType::InternalPixelType>( tmpNoDataValuePan );
  bool noDataValuePanAvailable = noDataValuesPanAvailable[0];
  noDataValuePanAvailable &= retPan;

  // Write no-data flags for Xs image
  std::vector<bool> noDataValuesXsAvailable;
  std::vector<double> tmpNoDataValuesXs;
  std::vector<typename TXsImageType::InternalPixelType> noDataValuesXs;
  bool retXs = itk::ExposeMetaData<std::vector<bool> >( this->GetXsInput()->GetMetaDataDictionary(), MetaDataKey::NoDataValueAvailable, noDataValuesXsAvailable );
  itk::ExposeMetaData<std::vector<double> >( this->GetXsInput()->GetMetaDataDictionary(), MetaDataKey::NoDataValue, tmpNoDataValuesXs );

  // Check if noData is needed and update noDataValuesAvailable with return function value
  if(retPan || retXs)
    {
    m_UseNoData = noDataValuePanAvailable;
    for ( unsigned int i = 0; i < noDataValuesXsAvailable.size(); ++i )
      {
      noDataValuesXs.push_back(static_cast<typename TXsImageType::InternalPixelType>(tmpNoDataValuesXs[i]));
      m_UseNoData |= (noDataValuesXsAvailable[i] = (noDataValuesXsAvailable[i] && retXs));
      }
    }


  // Instantiate fusion filter
  if ( m_UseNoData )
    {
    m_NoDataFusionFilter = NoDataFusionFilterType::New();
    m_ProgressAccumulator->RegisterInternalFilter( m_NoDataFusionFilter, 0.1 );

    m_NoDataFusionFilter->SetInput2( m_ConvolutionFilter->GetOutput() );
    m_NoDataFusionFilter->GetFunctor().SetNoDataValuesXsAvailable( noDataValuesXsAvailable );
    m_NoDataFusionFilter->GetFunctor().SetNoDataValuePanAvailable( noDataValuePanAvailable );
    m_NoDataFusionFilter->GetFunctor().SetNoDataValuePan( noDataValuePan );
    m_NoDataFusionFilter->GetFunctor().SetNoDataValuesXs( noDataValuesXs );

    m_NoDataFusionFilter->SetInput1( this->GetXsInput() );
    m_NoDataFusionFilter->SetInput2( m_ConvolutionFilter->GetOutput() );
    m_NoDataFusionFilter->SetInput3( this->GetPanInput() );

    // Wire composite filter
    m_NoDataFusionFilter->GraftOutput( this->GetOutput() );
    m_NoDataFusionFilter->Update();
    this->GraftOutput( m_NoDataFusionFilter->GetOutput() );
    }
  else
    {
    m_FusionFilter = FusionFilterType::New();
    m_ProgressAccumulator->RegisterInternalFilter( m_FusionFilter, 0.1 );

    m_FusionFilter->SetInput1( this->GetXsInput() );
    m_FusionFilter->SetInput2( m_ConvolutionFilter->GetOutput() );
    m_FusionFilter->SetInput3( this->GetPanInput() );

    // Wire composite filter
    m_FusionFilter->GraftOutput( this->GetOutput() );
    m_FusionFilter->Update();
    this->GraftOutput( m_FusionFilter->GetOutput() );
    }

}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
void
SimpleRcsPanSharpeningFusionImageFilter
<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os
  << indent << "Radius:" << this->m_Radius
  << std::endl;
}

} // end namespace otb

#endif
