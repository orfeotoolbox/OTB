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
            class TPrecision, Wavelet::Wavelet TMotherWaveletOperator >
SparseUnmixingImageFilter< TInputImage, TOutputImage, TPrecision, TMotherWaveletOperator >
::SparseUnmixingImageFilter ()
{
  this->SetNumberOfRequiredInputs(2);

  m_NumberOfComponentsRequired = 0;

  m_WvltFilter1 = WvltFilterType::New();
  m_WvltFilter1->SetNumberOfDecompositions(2);

  m_WvltFilter2 = WvltFilterType::New();
  m_WvltFilter2->SetNumberOfDecompositions( m_WvltFilter1->GetNumberOfDecompositions() );

  m_ListFilter = ListFilterType::New();
  m_ListFilter->GetFunctor().SetLowerThreshold(10.);

  /** 
   * The histogram is dedicated to angular value in [-PI,PI]
   * The histogram is smoothed with 100 bins
   */
  m_Histogram = HistogramType::New();
  typename HistogramType::SizeType size;
  size.Fill(100);
  m_Histogram->Initialize( size );

  m_Transformer = TransformFilterType::New();
}

template < class TInputImage, class TOutputImage, 
            class TPrecision, Wavelet::Wavelet TMotherWaveletOperator >
void
SparseUnmixingImageFilter< TInputImage, TOutputImage, TPrecision, TMotherWaveletOperator >
::SetInput1 ( const InputImageType * inputPtr )
{
  this->itk::ProcessObject::SetNthInput(0,
    const_cast< InputImageType * >( inputPtr ) );
}

template < class TInputImage, class TOutputImage, 
            class TPrecision, Wavelet::Wavelet TMotherWaveletOperator >
void
SparseUnmixingImageFilter< TInputImage, TOutputImage, TPrecision, TMotherWaveletOperator >
::SetInput2 ( const InputImageType * inputPtr )
{
  this->itk::ProcessObject::SetNthInput(1,
    const_cast< InputImageType * >( inputPtr ) );
}

template < class TInputImage, class TOutputImage, 
            class TPrecision, Wavelet::Wavelet TMotherWaveletOperator >
const TInputImage *
SparseUnmixingImageFilter< TInputImage, TOutputImage, TPrecision, TMotherWaveletOperator >
::GetInput1() const
{
  if ( this->GetNumberOfInputs() < 1 )
  {
    return 0;
  }

  return static_cast<const InputImageType * >
    (this->itk::ProcessObject::GetInput(0) );
}

template < class TInputImage, class TOutputImage, 
            class TPrecision, Wavelet::Wavelet TMotherWaveletOperator >
const TInputImage *
SparseUnmixingImageFilter< TInputImage, TOutputImage, TPrecision, TMotherWaveletOperator >
::GetInput2() const
{
  if ( this->GetNumberOfInputs() < 2 )
  {
    return 0;
  }

  return static_cast<const InputImageType * >
    (this->itk::ProcessObject::GetInput(1) );
}



template < class TInputImage, class TOutputImage, 
            class TPrecision, Wavelet::Wavelet TMotherWaveletOperator >
void
SparseUnmixingImageFilter< TInputImage, TOutputImage, TPrecision, TMotherWaveletOperator >
::GenerateData()
{
  itk::ProgressAccumulator::Pointer progress = itk::ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);


  m_WvltFilter1->SetInput( this->GetInput1() );
  progress->RegisterInternalFilter(m_WvltFilter1,0.25f);
  m_WvltFilter1->Update();

  m_WvltFilter2->SetInput( this->GetInput2() );
  progress->RegisterInternalFilter(m_WvltFilter2,0.25f);
  m_WvltFilter2->Update();

  m_ListFilter->SetInput1( m_WvltFilter1->GetOutput() );
  m_ListFilter->SetInput2( m_WvltFilter2->GetOutput() );
  progress->RegisterInternalFilter(m_ListFilter,0.25f);
  m_ListFilter->Update();

  typename InternalSampleListType::Iterator angleIter = m_ListFilter->GetOutputSampleList()->Begin();
  while ( angleIter != m_ListFilter->GetOutputSampleList()->End() )
  {
    m_Histogram->IncreaseFrequency( angleIter.GetMeasurementVector()[0], 1 );
    ++angleIter;
  }

  GenerateNumberOfComponentsRequired();
  otbMsgDebugMacro( << m_NumberOfComponentsRequired << " sources found\n" );

  m_Transformer->SetInput1( this->GetInput1() );
  m_Transformer->SetInput2( this->GetInput2() );
  m_Transformer->SetAngleSet( m_AngleValue );
  progress->RegisterInternalFilter(m_Transformer,0.25f);
  
  this->SetNumberOfOutputs( m_Transformer->GetNumberOfOutputs() );
  for ( unsigned int i = 0; i < this->GetNumberOfOutputs(); i++ )
  { 
    this->SetNthOutput(i, OutputImageType::New());
    m_Transformer->GraftNthOutput( i, this->GetOutput(i) );
  }

  m_Transformer->Update();

  for ( unsigned int i = 0; i < this->GetNumberOfOutputs(); i++ )
  {
    this->GraftNthOutput( i, m_Transformer->GetOutput(i) );
  }
}

template < class TInputImage, class TOutputImage, 
            class TPrecision, Wavelet::Wavelet TMotherWaveletOperator >
void
SparseUnmixingImageFilter< TInputImage, TOutputImage, TPrecision, TMotherWaveletOperator >
::GenerateNumberOfComponentsRequired () 
{
  std::vector<PrecisionType> angles;

  typename HistogramType::Iterator prevHist = m_Histogram->Begin();
  typename HistogramType::Iterator curHist = m_Histogram->Begin();
  /** Since operator-- does not exists in itk::Histgram, we have 
   * to reach the end step by step
   */
  ++curHist;
  while ( curHist != m_Histogram->End() )
  {
    ++curHist;
    ++prevHist;
  }
  curHist = m_Histogram->Begin();
  typename HistogramType::Iterator nextHist = m_Histogram->Begin();
  ++nextHist;

  if ( prevHist.GetFrequency() < curHist.GetFrequency()
      && curHist.GetFrequency() > nextHist.GetFrequency() )
  {
    angles.push_back( curHist.GetMeasurementVector()[0] );
  }

  prevHist = m_Histogram->Begin();
  ++curHist;
  ++nextHist;

  while ( curHist != m_Histogram->End() )
  {
    if ( nextHist == m_Histogram->End() )
      nextHist = m_Histogram->Begin();

    if ( prevHist.GetFrequency() < curHist.GetFrequency()
        && curHist.GetFrequency() > nextHist.GetFrequency() )
    {
      angles.push_back( curHist.GetMeasurementVector()[0] );
    }

    ++prevHist;
    ++curHist;
    ++nextHist;
  }
  
  m_NumberOfComponentsRequired = angles.size();
  m_AngleValue = angles;
}

} // end of namespace otb

#endif



