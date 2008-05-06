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

#ifndef _otbMarkovClassificationFilter_txx
#define _otbMarkovClassificationFilter_txx
#include "otbMarkovClassificationFilter.h"

namespace otb
{

template<class TInputImage, class TClassifiedImage>
MarkovClassificationFilter<TInputImage,TClassifiedImage>
::MarkovClassificationFilter(void):
  m_NumberOfClasses(0),
  m_MaximumNumberOfIterations(50),
  m_ErrorCounter(0),
  m_NeighborhoodRadius(1),
  m_TotalNumberOfValidPixelsInOutputImage(1),
  m_TotalNumberOfPixelsInInputImage(1),
  //m_ErrorTolerance(0.2),
  //m_SmoothingFactor(1),
  //m_NumberOfIterations(0),
  m_ExternalClassificationSet(false),
  m_StopCondition(MaximumNumberOfIterations)
{
  m_ExternalClassificationSet = false;
  m_SmoothingFactor = 1;
  m_NumberOfIterations = 0;
  m_ErrorTolerance = 0.2;
  this->SetNumberOfRequiredInputs(2);
  if( (int)InputImageDimension != (int)ClassifiedImageDimension )
    {
      itk::OStringStream msg;
      msg << "Input image dimension: " << InputImageDimension << " != output image dimension: " << ClassifiedImageDimension; 
      throw itk::ExceptionObject(__FILE__, __LINE__,msg.str().c_str(),ITK_LOCATION);
    }
  m_InputImageNeighborhoodRadius.Fill(m_NeighborhoodRadius);
  // m_MRFNeighborhoodWeight.resize(0);
  // m_NeighborInfluence.resize(0);
  // m_DummyVector.resize(0);
  // this->SetMRFNeighborhoodWeight( m_DummyVector );
  // this->SetDefaultMRFNeighborhoodWeight();

  EnergyRegularizationPointer m_EnergyRegularization = EnergyRegularizationType::New();;
  EnergyFidelityPointer       m_EnergyFidelity = EnergyFidelityType::New();;
  OptimizerPointer            m_Optimizer = OptimizerType::New();;    
  SamplerPointer              m_Sampler = SamplerType::New();   

  m_ValueInsteadRandom = itk::NumericTraits<int>::min();
}
  
template<class TInputImage, class TClassifiedImage>
MarkovClassificationFilter<TInputImage, TClassifiedImage>
::~MarkovClassificationFilter(void)
{
}
  

template<class TInputImage, class TClassifiedImage>
void
MarkovClassificationFilter<TInputImage, TClassifiedImage>
::SetTrainingInput(const TrainingImageType * trainingImage)
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(1, const_cast< TrainingImageType * >( trainingImage ) );
  this->Modified();
}


template <class TInputImage, class TClassifiedImage>
typename MarkovClassificationFilter<TInputImage, TClassifiedImage>::TrainingImageType*
MarkovClassificationFilter<TInputImage, TClassifiedImage>
::GetTrainingInput(void) 
{
  if (this->GetNumberOfInputs() < 2)
    {
      return 0;
    }
  return static_cast<TrainingImageType * >
    (this->itk::ProcessObject::GetInput(1) );
}


template<class TInputImage, class TClassifiedImage>
void
MarkovClassificationFilter<TInputImage, TClassifiedImage>
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf(os,indent);
  os << indent <<" MRF Image filter object " << std::endl;
  os << indent <<" Number of classes: " << m_NumberOfClasses << std::endl;
  os << indent <<" Maximum number of iterations: " << m_MaximumNumberOfIterations << std::endl;
  os << indent <<" Error tolerance for convergence: " << m_ErrorTolerance << std::endl;
  os << indent <<" Size of the MRF neighborhood radius:" << m_InputImageNeighborhoodRadius << std::endl;
  os << indent << "StopCondition: " << m_StopCondition << std::endl;
  os << indent <<" Number of iterations: " << m_NumberOfIterations << std::endl;
  
}// end PrintSelf



/*
 * GenerateInputRequestedRegion method.
 */
template <class TInputImage, class TClassifiedImage>
void
MarkovClassificationFilter<TInputImage, TClassifiedImage>
::GenerateInputRequestedRegion()
{  
  // this filter requires the all of the input images 
  // to be at the size of the output requested region
  InputImagePointer inputPtr = const_cast< InputImageType * >( this->GetInput() );
  OutputImagePointer outputPtr = this->GetOutput();
  inputPtr->SetRequestedRegion( outputPtr->GetRequestedRegion() );
}


/*
 * EnlargeOutputRequestedRegion method.
 */
template <class TInputImage, class TClassifiedImage>
void
MarkovClassificationFilter<TInputImage, TClassifiedImage>
::EnlargeOutputRequestedRegion( itk::DataObject *output )
{
  // this filter requires the all of the output image to be in
  // the buffer
  TClassifiedImage *imgData;
  imgData = dynamic_cast<TClassifiedImage*>( output );
  imgData->SetRequestedRegionToLargestPossibleRegion();
}



/*
 * GenerateOutputInformation method.
 */
template <class TInputImage, class TClassifiedImage>
void
MarkovClassificationFilter<TInputImage, TClassifiedImage>
::GenerateOutputInformation()
{
  typename TInputImage::ConstPointer input  = this->GetInput();
  typename TClassifiedImage::Pointer output = this->GetOutput();
  output->SetLargestPossibleRegion( input->GetLargestPossibleRegion() );
}




template<class TInputImage, class TClassifiedImage>
void
MarkovClassificationFilter<TInputImage, TClassifiedImage>
::GenerateData()
{
  //First run the Gaussian classifier calculator and
  //generate the Gaussian model for the different classes
  //and then generate the initial labelled dataset.
  
  InputImageConstPointer inputImage = this->GetInput();
  
  //Allocate memory for the labelled images
  this->Allocate();
  
  //Branch the pipeline
  this->Initialize();
  
  this->ApplyMarkovClassificationFilter();
  
}// end GenerateData



//-------------------------------------------------------
//Set the neighborhood radius
//-------------------------------------------------------
template<class TInputImage, class TClassifiedImage>
void
MarkovClassificationFilter<TInputImage, TClassifiedImage>
::SetNeighborhoodRadius( const unsigned long radiusValue)
{
  //Set up the neighbor hood 
  NeighborhoodRadiusType radius;
  for(unsigned int i=0;i < InputImageDimension; ++i)
    {
      radius[i] = radiusValue;
    }   
  this->SetNeighborhoodRadius( radius ); 
  
}// end SetNeighborhoodRadius



template<class TInputImage, class TClassifiedImage>
void
MarkovClassificationFilter<TInputImage, TClassifiedImage>
::SetNeighborhoodRadius( const unsigned long *radiusArray)
{
  NeighborhoodRadiusType  radius;
  for(unsigned int i=0;i < InputImageDimension; ++i)
    {
      radius[i] = radiusArray[i];
    }
  //Set up the neighbor hood 
  this->SetNeighborhoodRadius( radius ); 
  
}// end SetNeighborhoodRadius




//Set the neighborhood radius
template<class TInputImage, class TClassifiedImage>
void
MarkovClassificationFilter<TInputImage, TClassifiedImage>
::SetNeighborhoodRadius( const NeighborhoodRadiusType &radius)
{
  //Set up the neighbor hood 
  for(unsigned int i=0;i < InputImageDimension; ++i)
    {
      m_InputImageNeighborhoodRadius[ i ] = radius[ i ];
      m_LabelledImageNeighborhoodRadius[ i ] =radius[ i ];
    }
  
}// end SetNeighborhoodRadius
//-------------------------------------------------------



//-------------------------------------------------------
//-------------------------------------------------------
//Allocate algorithm specific resources
//-------------------------------------------------------
template<class TInputImage, class TClassifiedImage>
void
MarkovClassificationFilter<TInputImage, TClassifiedImage>
::Allocate()
{
  if( m_NumberOfClasses <= 0 )
    {
      throw itk::ExceptionObject(__FILE__, __LINE__,"NumberOfClasses <= 0.",ITK_LOCATION);
    }

  InputImageSizeType inputImageSize = this->GetInput()->GetBufferedRegion().GetSize();
  
  //---------------------------------------------------------------------
  //Get the number of valid pixels in the output MRF image
  //---------------------------------------------------------------------    
  int tmp;
  for( unsigned int i=0; i < InputImageDimension; i++ )
    {
      tmp = static_cast<int>(inputImageSize[i]);
      m_TotalNumberOfPixelsInInputImage *= tmp;
      m_TotalNumberOfValidPixelsInOutputImage *= ( tmp - 2*m_InputImageNeighborhoodRadius[i] ); 
    }  
  
  //Set the output labelled and allocate the memory
  LabelledImagePointer outputPtr = this->GetOutput();
  
  //Allocate the output buffer memory 
  outputPtr->SetBufferedRegion( outputPtr->GetRequestedRegion() );
  outputPtr->Allocate();
  
  //Copy input data in the output buffer memory or
  //initialize to random values if not set
  LabelledImageRegionIterator  outImageIt( outputPtr, outputPtr->GetRequestedRegion() );
  
  m_ExternalClassificationSet = true; //TODO switch to random if the ref image is not specified
  if (m_ExternalClassificationSet)
    {
      TrainingImagePointer  trainingImage = this->GetTrainingInput();
      LabelledImageRegionIterator trainingImageIt( trainingImage, outputPtr->GetRequestedRegion() );
      
      while ( !outImageIt.IsAtEnd() )
	{
	  LabelledImagePixelType labelvalue = ( LabelledImagePixelType ) trainingImageIt.Get();
	  
	  outImageIt.Set( labelvalue );
	  ++trainingImageIt;
	  ++outImageIt;
	}// end while
    }
  else //set to random value
    {
      // if it is a test, cancel the rand()
      if( m_ValueInsteadRandom == itk::NumericTraits<int>::min() )
	{
	  srand((unsigned)time(0));
	  
	  while ( !outImageIt.IsAtEnd() )
	    {
	      LabelledImagePixelType randomvalue = static_cast<LabelledImagePixelType>(rand() % static_cast<int>(m_NumberOfClasses));
	      outImageIt.Set( randomvalue );
	      ++outImageIt;
	    }// end while
	}
      else
	{
	  while ( !outImageIt.IsAtEnd() )
	    {
	      LabelledImagePixelType randomvalue = static_cast<LabelledImagePixelType>(m_ValueInsteadRandom % static_cast<int>(m_NumberOfClasses));
	      outImageIt.Set( randomvalue );
	      ++outImageIt;
	    }
	}
    }
  
  
}// Allocate


template<class TInputImage, class TClassifiedImage>
void
MarkovClassificationFilter<TInputImage, TClassifiedImage>
::Initialize() throw (itk::ExceptionObject)
{ 
  if( m_ValueInsteadRandom == itk::NumericTraits<int>::min() )
    {
      srand((unsigned)time(0));
    }
  
  if ( !m_EnergyRegularization )
    {
      itkExceptionMacro(<<"EnergyRegularization is not present" );
    }
  
  if ( !m_EnergyFidelity )
    {
      itkExceptionMacro(<<"EnergyFidelity is not present" );
    }
  
  if ( !m_Optimizer )
    {
      itkExceptionMacro(<<"Optimizer is not present" );
    }
  
    if( !m_Sampler )
      {
	itkExceptionMacro(<<"Sampler is not present");
      }
    
    m_Sampler->SetLambda(m_Lambda);
    m_Sampler->SetEnergyRegularization(m_EnergyRegularization);
    m_Sampler->SetEnergyFidelity(m_EnergyFidelity);
    m_Sampler->SetNumberOfClasses(m_NumberOfClasses);
    //     m_Optimizer->SetTemperature(m_Temperature);
}   



//-------------------------------------------------------
//-------------------------------------------------------
//Apply the MRF image filter
//-------------------------------------------------------
template<class TInputImage, class TClassifiedImage>
void 
MarkovClassificationFilter<TInputImage, TClassifiedImage>
::ApplyMarkovClassificationFilter()
{
  InputImageSizeType inputImageSize = this->GetInput()->GetBufferedRegion().GetSize();
  int totalNumberOfPixelsInInputImage = 1;
  for( unsigned int i = 0; i < InputImageDimension; i++ )  
    {
      totalNumberOfPixelsInInputImage *= static_cast<int>(inputImageSize[ i ]) ;
    }

  int maxNumPixelError = (int) ( vnl_math_rnd (m_ErrorTolerance * m_TotalNumberOfValidPixelsInOutputImage) );
  m_NumberOfIterations = 0;
  do
    {
      otbMsgDebugMacro(<< "Iteration No." << m_NumberOfIterations);   
      this->MinimizeOnce();
      otbMsgDebugMacro(<< "m_ErrorCounter/m_TotalNumberOfPixelsInInputImage: " 
		       << m_ErrorCounter/((double)(m_TotalNumberOfPixelsInInputImage)));
      
      m_NumberOfIterations += 1;
      //       m_ErrorCounter = m_TotalNumberOfValidPixelsInOutputImage - 
      //           totalNumberOfPixelsInInputImage;  
      
    } 
  while(( m_NumberOfIterations < m_MaximumNumberOfIterations ) && 
	( m_ErrorCounter > maxNumPixelError ) ); 
  
  otbMsgDebugMacro(<< "m_NumberOfIterations: " << m_NumberOfIterations);
  otbMsgDebugMacro(<< "m_MaximumNumberOfIterations: " << m_MaximumNumberOfIterations);
  otbMsgDebugMacro(<< "m_ErrorCounter: " << m_ErrorCounter);
  otbMsgDebugMacro(<< "maxNumPixelError: " << maxNumPixelError);
  
  
  
  //Determine stop condition
  if( m_NumberOfIterations >= m_MaximumNumberOfIterations )
    {
      m_StopCondition = MaximumNumberOfIterations;
    }
  else if( m_ErrorCounter <= maxNumPixelError ) 
    {
      m_StopCondition = ErrorTolerance;
    }
  
}// ApplyMarkovClassificationFilter



template<class TInputImage, class TClassifiedImage>
void 
MarkovClassificationFilter<TInputImage, TClassifiedImage>
::MinimizeOnce()
{
  LabelledImageNeighborhoodIterator labelledIterator(m_LabelledImageNeighborhoodRadius, this->GetOutput(), this->GetOutput()->GetLargestPossibleRegion() );
  InputImageNeighborhoodIterator dataIterator(m_InputImageNeighborhoodRadius, this->GetInput(), this->GetInput()->GetLargestPossibleRegion() );
  m_ErrorCounter = 0;
  //WARNING, is it the same size ??
  for (labelledIterator.GoToBegin(), dataIterator.GoToBegin(); !labelledIterator.IsAtEnd(); ++labelledIterator, ++dataIterator)
    {
      LabelledImagePixelType value;
      bool changeValue;
      m_Sampler->Compute(dataIterator,labelledIterator);
      value=m_Sampler->GetValue();
      changeValue= m_Optimizer->Compute(m_Sampler->GetDeltaEnergy());
      if (changeValue)
	{
	  labelledIterator.SetCenterPixel(value);
	  ++m_ErrorCounter;
	}
    }
}

} // namespace otb

#endif
