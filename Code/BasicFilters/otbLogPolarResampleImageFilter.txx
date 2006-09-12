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



#ifndef _otbLogPolarResampleImageFilter_txx
#define _otbLogPolarResampleImageFilter_txx

#include "otbLogPolarResampleImageFilter.h"
#include "itkObjectFactory.h"
#include "itkIdentityTransform.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkProgressReporter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageLinearIteratorWithIndex.h"
#include "itkSpecialCoordinatesImage.h"

namespace otb
{

/**
 * Initialize new instance
 */
template <class TInputImage,  class TInterpolator>
LogPolarResampleImageFilter<TInputImage, TInterpolator>
::LogPolarResampleImageFilter()
{
  m_RadialNumberOfSamples  = 128;
  m_AngularNumberOfSamples = 128;
  
  m_RadialStep  = 1.0;
  m_AngularStep = 1.0;    

  m_AngularStepIsConfigured = false;
  m_RadialStepIsConfigured  = false;


  m_OriginIsAtCenter  = true;
  m_DefaultPixelValue = 0;
  m_Sigma             = 0.5;
  
  m_Interpolator      = itk::LinearInterpolateImageFunction<InputImageType, CoordRepType>::New();
  
}

/**
 * Print out a description of self
 *
 */
template <class TInputImage, class TInterpolator>
void 
LogPolarResampleImageFilter<TInputImage, TInterpolator>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "m_RadialStep: " << m_RadialStep << std::endl;
  os << indent << "m_AngularStep: " << m_AngularStep << std::endl;
  os << indent << "m_RadialNumberOfSamples: " << m_RadialNumberOfSamples << std::endl;
  os << indent << "m_AngularNumberOfSamples: " << m_AngularNumberOfSamples << std::endl;
  os << indent << "m_OriginIsAtCenter: " << (m_OriginIsAtCenter ? "On" : "Off") << std::endl;
  os << indent << "m_DefaultPixelValue: " << m_DefaultPixelValue << std::endl;
  os << indent << "m_Interpolator: " << m_Interpolator.GetPointer() << std::endl;
 
  return;
}



/**
 * Set up state of filter before multi-threading.
 * InterpolatorType::SetInputImage is not thread-safe and hence
 * has to be set up before ThreadedGenerateData
 */
template <class TInputImage, class TInterpolator>
void 
LogPolarResampleImageFilter<TInputImage,TInterpolator>
::BeforeThreadedGenerateData()
{

  if( !m_Interpolator )
    {
    itkExceptionMacro(<< "Interpolator not set"<<std::endl);
    }

  // Connect input image to interpolator
  m_Interpolator->SetInputImage( this->GetInput() );

}


/**
 * Set up state of filter after multi-threading.
 */
template <class TInputImage, class TInterpolator>
void 
LogPolarResampleImageFilter<TInputImage,TInterpolator>
::AfterThreadedGenerateData()
{
  // Disconnect input image from the interpolator
  m_Interpolator->SetInputImage( NULL );

}

/**
 * ThreadedGenerateData
 */
template <class TInputImage, class TInterpolator>
void 
LogPolarResampleImageFilter<TInputImage,TInterpolator>
::ThreadedGenerateData(
  const OutputImageRegionType& outputRegionForThread,
  int threadId)
{
  // Get the output pointers
  OutputImagePointer      outputPtr = this->GetOutput();

  // Get ths input pointers
  InputImageConstPointer inputPtr=this->GetInput();

  // Create an iterator that will walk the output region for this thread.
  typedef itk::ImageRegionIteratorWithIndex<TInputImage> OutputIterator;

  OutputIterator outIt(outputPtr, outputRegionForThread);

  // Define a few indices that will be used to translate from an input pixel
  // to an output pixel
  PointType outputPoint;         // Coordinates of current output pixel
  PointType inputPoint;          // Coordinates of current input pixel

  typedef itk::ContinuousIndex<CoordRepType, OutputImageDimension> ContinuousIndexType;
  ContinuousIndexType inputIndex;

  // Support for progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
        
  typedef typename InterpolatorType::OutputType OutputType;

  // Min/max values of the output pixel type AND these values
  // represented as the output type of the interpolator
  const OutputPixelType minOutputValue =  itk::NumericTraits<OutputPixelType >::NonpositiveMin();
  const OutputPixelType maxOutputValue =  itk::NumericTraits<OutputPixelType >::max();

  // Walk the output region
  outIt.GoToBegin();

  while ( !outIt.IsAtEnd() )
    {
    // Determine the index of the current output pixel
    outputPtr->TransformIndexToPhysicalPoint( outIt.GetIndex(), outputPoint );

    // Compute corresponding input pixel position
    double Theta = outputPoint[0]*m_AngularStep*acos(-1.0)/180.0;
    double Rho   = outputPoint[1]*m_RadialStep;

    inputPoint[0] = exp(Rho) * cos(Theta);
    inputPoint[1] = exp(Rho) * sin(Theta);
  
    if(m_OriginIsAtCenter == true)
      {
      inputPoint[0] += inputPtr->GetLargestPossibleRegion().GetSize()[0]/2.;
      inputPoint[1] += inputPtr->GetLargestPossibleRegion().GetSize()[1]/2.;
      }
    
     
    inputPtr->TransformPhysicalPointToContinuousIndex(inputPoint, inputIndex);

    
    // Evaluate input at right position and copy to the output
    if( m_Interpolator->IsInsideBuffer(inputIndex) )
      {
      OutputPixelType pixval;
      double valueTemp = static_cast<double>(m_Interpolator->EvaluateAtContinuousIndex(inputIndex) );
      valueTemp *= exp(m_Sigma * Rho);
      valueTemp *= m_RadialStep; 
      OutputPixelType value = static_cast<OutputPixelType>(valueTemp);
      
      if( value < minOutputValue )
        {
        pixval = minOutputValue;
        }
      else if( value > maxOutputValue )
        {
        pixval = maxOutputValue;
        }
      else 
        {
        pixval = static_cast<OutputPixelType>( value );
        }
      outIt.Set( pixval );      
      }
    else
      {
      outIt.Set(m_DefaultPixelValue); // default background value
      }

    progress.CompletedPixel();
    ++outIt;
    }

  return;
  
}




/** 
 * Inform pipeline of necessary input image region
 *
 * Determining the actual input region is non-trivial, especially
 * when we cannot assume anything about the transform being used.
 * So we do the easy thing and request the entire input image.
 */
template <class TInputImage, class TInterpolator>
void 
LogPolarResampleImageFilter<TInputImage,TInterpolator>
::GenerateInputRequestedRegion()
{
  // call the superclass's implementation of this method
  Superclass::GenerateInputRequestedRegion();

  if ( !this->GetInput() )
    {
    return;
    }

  // get pointers to the input and output
  InputImagePointer  inputPtr  = 
    const_cast< TInputImage *>( this->GetInput() );

  // Request the entire input image
  InputImageRegionType inputRegion;
  inputRegion = inputPtr->GetLargestPossibleRegion();
  inputPtr->SetLargestPossibleRegion(inputRegion);
  inputPtr->SetRequestedRegion(inputRegion);

  return;
}


/** 
 * Inform pipeline of required output region
 */
template <class TInputImage, class TInterpolator>
void 
LogPolarResampleImageFilter<TInputImage,TInterpolator>
::GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointers to the input and output
  InputImagePointer  inputPtr  = const_cast< TInputImage *>( this->GetInput() );
  OutputImagePointer outputPtr = this->GetOutput();
  if ( !inputPtr )
    {
    return;
    }
    
  if(m_RadialStepIsConfigured == true)
    {
    std::cout << "1" << std::endl;
    CalculateRadialNumberOfSamples();
    }  
    else
    {
    std::cout << "2" << std::endl;
    CalculateRadialStep();
    }      

  if(m_AngularStepIsConfigured == true)
    {
    std::cout << "3" << std::endl;
    CalculateAngularNumberOfSamples();
    }  
    else
    {
    std::cout << "4" << std::endl;
    CalculateAngularStep();
    }      


  SizeType                Size;                // Size of the output image
  
  Size[0] = static_cast<SizeValueType>(m_AngularNumberOfSamples);
  Size[1] = static_cast<SizeValueType>(m_RadialNumberOfSamples);

  std::cout << "Size : " << Size << std::endl;
  
  OutputImageRegionType    outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize( Size );
  outputPtr->SetLargestPossibleRegion( outputLargestPossibleRegion );

  return;
}



/** 
 * Verify if any of the components has been modified.
 */
template <class TInputImage, class TInterpolator>
unsigned long 
LogPolarResampleImageFilter<TInputImage,TInterpolator>
::GetMTime( void ) const
{
  unsigned long latestTime = itk::Object::GetMTime(); 


  if( m_Interpolator )
    {
    if( latestTime < m_Interpolator->GetMTime() )
      {
      latestTime = m_Interpolator->GetMTime();
      }
    }

  return latestTime;
}


template <class TInputImage, class TInterpolator>
void
LogPolarResampleImageFilter<TInputImage,TInterpolator>
::CalculateRadialNumberOfSamples()
{

  // get pointers to the input and output
  InputImagePointer  inputPtr  = const_cast< TInputImage *>( this->GetInput() );

  if ( !inputPtr )
    {
    return;
    }
  
  SizeType                Size;                // Size of the output image
  Size = inputPtr->GetLargestPossibleRegion().GetSize();

  double Radial_max = sqrt(Size[0]*Size[0]+Size[1]*Size[1]);
  if(m_OriginIsAtCenter == true)
    {
    Radial_max /= 2.0;
    }    
  if( m_RadialStep <=0. )
    {
    itkExceptionMacro(<< "LogPolarResampleImageFilter::CalculateRadialNumberOfSamples() m_RadialStep must be greater than zero: " <<std::endl);
    }
  m_RadialNumberOfSamples = log(Radial_max) / m_RadialStep;

  double bx = log(m_RadialNumberOfSamples) / log(2.0);
  if(int(bx)!=bx)
    {
     m_RadialNumberOfSamples = pow(2,int(bx)+1);
    }  	
}

template <class TInputImage, class TInterpolator>
void
LogPolarResampleImageFilter<TInputImage,TInterpolator>
::CalculateAngularNumberOfSamples()
{
  double Angular_max = 90.0; 
  if(m_OriginIsAtCenter == true)
    {
    Angular_max = 360.0;
    }

  if( m_RadialStep <=0.0 )
    {
    itkExceptionMacro(<< "LogPolarResampleImageFilter::CalculateAngularNumberOfSamples() m_RadialStep must be greater than zero"<<std::endl);
    }

  m_AngularNumberOfSamples = Angular_max / m_AngularStep;

  double by = log(m_AngularNumberOfSamples) / log(2.0);
  if(int(by)!=by)
    {
     m_AngularNumberOfSamples = pow(2,int(by)+1);
    }
}

template <class TInputImage, class TInterpolator>
void
LogPolarResampleImageFilter<TInputImage,TInterpolator>
::CalculateRadialStep()
{
  // get pointers to the input and output
  InputImagePointer  inputPtr  = const_cast< TInputImage *>( this->GetInput() );

  SizeType                Size;                // Size of the output image
  Size = inputPtr->GetLargestPossibleRegion().GetSize();

  if( m_RadialNumberOfSamples <1 )
    {
    itkExceptionMacro(<< "LogPolarResampleImageFilter::CalculateRadialStep() m_RadialNumberOfSamples must be greater than one"<<std::endl);
    }

  double by = log(m_RadialNumberOfSamples) / log(2.0);
  if(int(by)!=by)
    {
     m_RadialNumberOfSamples = pow(2,int(by)+1);
    }

  double Radial_max = sqrt(Size[0]*Size[0]+Size[1]*Size[1]);
  if(m_OriginIsAtCenter == true)
    {
    Radial_max /= 2.0;
    }   
  m_RadialStep = log(Radial_max) / m_RadialNumberOfSamples ;   
}

template <class TInputImage, class TInterpolator>
void
LogPolarResampleImageFilter<TInputImage,TInterpolator>
::CalculateAngularStep()
{  
  if( m_RadialNumberOfSamples <1 )
    {
    itkExceptionMacro(<< "LogPolarResampleImageFilter::CalculateAngularStep() m_RadialNumberOfSamples must be greater than one"<<std::endl);
    }
  double by = log(m_AngularNumberOfSamples) / log(2.0);
  if(int(by)!=by)
    {
     m_AngularNumberOfSamples = pow(2,int(by)+1);
    }

  double Angular_max = 90.0; 
  if(m_OriginIsAtCenter == true)
    {
    Angular_max = 360.0;
    }

  m_AngularStep = Angular_max / m_AngularNumberOfSamples;

}

template <class TInputImage, class TInterpolator>
void
LogPolarResampleImageFilter<TInputImage,TInterpolator>
::SetAngularStep(double angularStep)
{
  m_AngularStep = angularStep;
  m_AngularStepIsConfigured = true;
}

template <class TInputImage, class TInterpolator>
void
LogPolarResampleImageFilter<TInputImage,TInterpolator>
::SetRadialStep(double radialStep)
{
  m_RadialStep = radialStep;
  m_RadialStepIsConfigured = true;
}

template <class TInputImage, class TInterpolator>
void
LogPolarResampleImageFilter<TInputImage,TInterpolator>
::SetAngularNumberOfSamples(double angularNumberOfSamples)
{
  m_AngularNumberOfSamples = angularNumberOfSamples;
  m_AngularStepIsConfigured = false;
}

template <class TInputImage, class TInterpolator>
void
LogPolarResampleImageFilter<TInputImage,TInterpolator>
::SetRadialNumberOfSamples(double radialNumberOfSamples)
{
  m_RadialNumberOfSamples = radialNumberOfSamples;
  m_RadialStepIsConfigured = false;
}



} // end namespace otb

#endif
