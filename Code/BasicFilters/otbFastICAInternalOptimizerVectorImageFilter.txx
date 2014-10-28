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
#ifndef __otbFastICAInternalOptimizerVectorImageFilter_txx
#define __otbFastICAInternalOptimizerVectorImageFilter_txx
#include "otbFastICAInternalOptimizerVectorImageFilter.h"

#include <itkMacro.h>
#include <itkImageRegionIterator.h>

#include <vnl/vnl_math.h>
#include <vnl/vnl_matrix.h>

namespace otb
{

template <class TInputImage, class TOutputImage>
FastICAInternalOptimizerVectorImageFilter< TInputImage, TOutputImage >
::FastICAInternalOptimizerVectorImageFilter()
{
  this->SetNumberOfRequiredInputs(2);

  m_CurrentBandForLoop = 0;
  m_Beta = 0.;
  m_Den = 0.;

  m_ContrastFunction = &vcl_tanh;

  m_TransformFilter = TransformFilterType::New();
}

template <class TInputImage, class TOutputImage>
void
FastICAInternalOptimizerVectorImageFilter< TInputImage, TOutputImage >
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  this->GetOutput()->SetNumberOfComponentsPerPixel(
    this->GetInput(0)->GetNumberOfComponentsPerPixel() );
}

template <class TInputImage, class TOutputImage>
void
FastICAInternalOptimizerVectorImageFilter< TInputImage, TOutputImage >
::BeforeThreadedGenerateData ()
{
  if ( m_W.empty() )
  {
    throw itk::ExceptionObject( __FILE__, __LINE__,
      "Give the initial W matrix", ITK_LOCATION );
  }

  m_BetaVector.resize( this->GetNumberOfThreads() );
  m_DenVector.resize( this->GetNumberOfThreads() );
  m_NbSamples.resize( this->GetNumberOfThreads() );
}

template <class TInputImage, class TOutputImage>
void
FastICAInternalOptimizerVectorImageFilter< TInputImage, TOutputImage >
::ThreadedGenerateData ( const OutputRegionType & outputRegionForThread, itk::ThreadIdType threadId )
{
  InputRegionType inputRegion;
  this->CallCopyOutputRegionToInputRegion( inputRegion, outputRegionForThread );

  itk::ImageRegionConstIterator< InputImageType > input0It
    ( this->GetInput(0), inputRegion );
  itk::ImageRegionConstIterator< InputImageType > input1It
    ( this->GetInput(1), inputRegion );
  itk::ImageRegionIterator< OutputImageType > outputIt
    ( this->GetOutput(), outputRegionForThread );

  unsigned int nbBands = this->GetInput(0)->GetNumberOfComponentsPerPixel();
  input0It.GoToBegin();
  input1It.GoToBegin();
  outputIt.GoToBegin();

  double beta = 0.;
  double den = 0.;
  double nbSample = 0.;

  while ( !input0It.IsAtEnd() && !input1It.IsAtEnd() && !outputIt.IsAtEnd() )
  {
    double x = static_cast<double>( input1It.Get()[GetCurrentBandForLoop()] );
    double g_x = (*m_ContrastFunction)(x);

    double x_g_x = x * g_x;
    beta += x_g_x;

    double gp = 1. - vcl_pow( g_x, 2. );
    den += gp;

    nbSample += 1.;

    typename OutputImageType::PixelType z ( nbBands );
    for ( unsigned int bd = 0; bd < nbBands; bd++ )
      z[bd] = g_x * input0It.Get()[bd];
    outputIt.Set(z);

    ++input0It;
    ++input1It;
    ++outputIt;
  } // end while loop

  m_BetaVector[threadId] = beta;
  m_DenVector[threadId] = den;
  m_NbSamples[threadId] = nbSample;
}

template <class TInputImage, class TOutputImage>
void
FastICAInternalOptimizerVectorImageFilter< TInputImage, TOutputImage >
::AfterThreadedGenerateData ()
{
  double beta = 0;
  double den = 0.;
  double nbSample = 0;

  for ( itk::ThreadIdType i = 0; i < this->GetNumberOfThreads(); ++i )
  {
    beta += m_BetaVector[i];
    den += m_DenVector[i];
    nbSample += m_NbSamples[i];
  }

  m_Beta = beta / nbSample;
  m_Den = den / nbSample - m_Beta;
}

} // end of namespace otb

#endif


