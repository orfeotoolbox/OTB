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
#ifndef __otbSparseUnmixingImageFilter_txx
#define __otbSparseUnmixingImageFilter_txx

#include "otbSparseUnmixingImageFilter.h"

#include "otbMath.h"
#include "itkProcessObject.h"

namespace otb {

template < class TInputImage, class TOutputImage,
            unsigned int VNbInputImage, class TPrecision,
            Wavelet::Wavelet TMotherWaveletOperator >
SparseUnmixingImageFilter< TInputImage, TOutputImage, VNbInputImage, TPrecision, TMotherWaveletOperator >
::SparseUnmixingImageFilter ()
{
  this->SetNumberOfRequiredInputs( NumberOfInputImages );
  this->SetNumberOfOutputs(1);
  this->SetNthOutput(0, OutputImageListType::New());

  m_NumberOfComponentsRequired = 0;
  m_NumberOfHistogramBins = 100;
  m_AngleList = AngleListType::New();

  m_WvltFilterList = WvltFilterListType::New();
  m_WvltFilterList->Resize( NumberOfInputImages );
  for ( unsigned int i = 0; i < NumberOfInputImages; ++i )
  {
    m_WvltFilterList->SetNthElement(i, WvltFilterType::New());
    m_WvltFilterList->GetNthElement(i)->SetNumberOfDecompositions(2);
    m_WvltFilterList->GetNthElement(i)->SetSubsampleImageFactor(1);
  }

  m_AngleListFilter = AngleListFilterType::New();
  m_AngleListFilter->SetThresholdValue( 10. );

    m_Histogram = HistogramType::New();
  m_Transformer = TransformFilterType::New();
}

template < class TInputImage, class TOutputImage,
            unsigned int VNbInputImage, class TPrecision,
            Wavelet::Wavelet TMotherWaveletOperator >
void
SparseUnmixingImageFilter< TInputImage, TOutputImage, VNbInputImage, TPrecision, TMotherWaveletOperator >
::SetInput ( unsigned int i, const InputImageType * img )
{
  this->itk::ProcessObject::SetNthInput(i,
    const_cast< InputImageType * >( img ) );
}

template < class TInputImage, class TOutputImage,
            unsigned int VNbInputImage, class TPrecision,
            Wavelet::Wavelet TMotherWaveletOperator >
const TInputImage *
SparseUnmixingImageFilter< TInputImage, TOutputImage, VNbInputImage, TPrecision, TMotherWaveletOperator >
::GetInput ( unsigned int i ) const
{
  if ( i >= this->GetNumberOfInputs() )
  {
    return 0;
  }

  return static_cast<const InputImageType * >
    (this->itk::ProcessObject::GetInput(i) );
}

template < class TInputImage, class TOutputImage,
            unsigned int VNbInputImage, class TPrecision,
            Wavelet::Wavelet TMotherWaveletOperator >
void
SparseUnmixingImageFilter< TInputImage, TOutputImage, VNbInputImage, TPrecision, TMotherWaveletOperator >
::GenerateData()
{
  itk::ProgressAccumulator::Pointer progress = itk::ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  for ( unsigned int i = 0; i < NumberOfInputImages; ++i )
  {
    m_WvltFilterList->GetNthElement(i)->SetInput( this->GetInput(i) );
    progress->RegisterInternalFilter( m_WvltFilterList->GetNthElement(i),
                                      0.5f/static_cast<float>(NumberOfInputImages) );
    m_WvltFilterList->GetNthElement(i)->Update();

    m_AngleListFilter->SetInput( i, m_WvltFilterList->GetNthElement(i)->GetOutput() );
  }

  progress->RegisterInternalFilter(m_AngleListFilter, 0.25f);
  m_AngleListFilter->Update();

  GenerateNumberOfComponentsRequired();
  otbMsgDebugMacro( << m_NumberOfComponentsRequired << " sources found\n" );

  for ( unsigned int i = 0; i < NumberOfInputImages; ++i )
    m_Transformer->SetInput( i, this->GetInput(i) );
  m_Transformer->SetAngleList( m_AngleList );
  progress->RegisterInternalFilter(m_Transformer, 0.25f);
  m_Transformer->Update();

  this->GetOutput()->Resize( m_Transformer->GetOutput()->Size() );
  for ( unsigned int i = 0; i < this->GetOutput()->Size(); ++i )
    this->GetOutput()->SetNthElement( i, m_Transformer->GetOutput()->GetNthElement(i) );
}

template < class TInputImage, class TOutputImage,
            unsigned int VNbInputImage, class TPrecision,
            Wavelet::Wavelet TMotherWaveletOperator >
void
SparseUnmixingImageFilter< TInputImage, TOutputImage, VNbInputImage, TPrecision, TMotherWaveletOperator >
::GenerateNumberOfComponentsRequired ()
{
  /**
   * The histogram is dedicated to angular value in [-PI, PI]
   * The histogram is smoothed with 100 bins (def. value)
   */

  HistogramSizeType size;
  size.Fill( m_NumberOfHistogramBins );
  MeasurementVectorType theMin (0.);
  theMin[NumberOfInputImages-2] = -M_PI;
  MeasurementVectorType theMax (M_PI);
  m_Histogram->Initialize( size, theMin, theMax );

  typename InternalSampleListType::Iterator angleIter
    = m_AngleListFilter->GetOutputSampleList()->Begin();

  if ( m_AngleListFilter->GetOutputSampleList()->Begin()
      == m_AngleListFilter->GetOutputSampleList()->End() )
  {
    throw itk::ExceptionObject( __FILE__, __LINE__,
      "The value of threshold is too high so that there is no angle value to consider for unmixing",
      ITK_LOCATION);
  }

  while ( angleIter != m_AngleListFilter->GetOutputSampleList()->End() )
  {
    if ( !m_Histogram->IncreaseFrequency( angleIter.GetMeasurementVector(), 1 ) )
      std::cerr << "Data out of bounds\n";

    ++angleIter;
  }
  
#if 0
  for ( unsigned int index = 0; index < m_Histogram->GetSize()[0]; ++index )
    std::cerr << index << "\t" << m_Histogram->GetMeasurementVector( index )[0]
      << "\t" << m_Histogram->GetFrequency(index) << "\n";
#endif

  AngleListPointerType angles = AngleListType::New();

  for ( unsigned int id = 0; id < m_Histogram->Size(); ++id )
  {
    HistogramIndexType curIdx = m_Histogram->GetIndex(id);

    // Is this curIdx a local max ?
    bool isLocalMax = true;
    for ( unsigned int k = 0; k < m_Histogram->GetSize().GetSizeDimension(); ++k )
    {
      HistogramIndexType prevIdx = curIdx;
      if ( prevIdx[k] == 0 )
        prevIdx[k] = m_Histogram->GetSize(k)-1;
      else
        prevIdx[k] = curIdx[k]-1;


      HistogramIndexType nextIdx = curIdx;
      if ( static_cast<int>( nextIdx[k] ) == m_Histogram->GetSize(k)-1 )
        nextIdx[k] = 0;
      else
        nextIdx[k] = curIdx[k]+1;

      double freq_prev = m_Histogram->GetFrequency( prevIdx );
      double freq_cur  = m_Histogram->GetFrequency( curIdx );
      double freq_next = m_Histogram->GetFrequency( nextIdx );

      if ( !( freq_prev < freq_cur && freq_cur > freq_next ) )
      {
        isLocalMax = false;
        break;
      }
    }

    if ( isLocalMax )
    {
      angles->PushBack( m_Histogram->GetMeasurementVector( curIdx ) );
    }
  }

  m_NumberOfComponentsRequired = angles->Size();
  m_AngleList = angles;

#if 1
  std::cout <<  m_NumberOfComponentsRequired << " sources found\n";
  for ( unsigned int i = 0; i < angles->Size(); i++ )
  {
    std::cerr << "Source " << i << ":";
    for ( unsigned int j = 0; j < m_Histogram->GetSize().GetSizeDimension(); ++j )
      std::cout << "\t" << angles->GetMeasurementVector(i)[j];
    std::cout << "\n";
  }
#endif
}

} // end of namespace otb

#endif


