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
#ifndef __otbSVMImageModelEstimator_txx
#define __otbSVMImageModelEstimator_txx

#include "otbSVMImageModelEstimator.h"
#include "otbMacro.h"

#include "itkCommand.h"
#include "itkImageRegionConstIterator.h"


namespace otb
{
/*
//Sous Visual, il n'aime pas l'appel à constructeur .. bizarre !!!
#if defined(WIN32) || defined(WIN32CE)
template<class TInputImage, class TTrainingImage>
SVMImageModelEstimator<TInputImage, TTrainingImage>
::SVMImageModelEstimator()
{
	this->m_NumberOfClasses = 0;
        this->m_Model = Superclass::SVMModelType::New();

        this->m_Done = 0;

        this->m_Model->SetSVMType(C_SVC);
	this->m_Model->SetKernelType(LINEAR);
	this->m_Model->SetPolynomialKernelDegree(3);
	this->m_Model->SetKernelGamma(0.);	// 1/k
	this->m_Model->SetKernelCoef0(0);
        this->m_Model->SetKernelFunctor(NULL);
	this->m_Model->SetNu(0.5);
	this->m_Model->SetCacheSize(40);
	this->m_Model->SetC(1);
	this->m_Model->SetEpsilon(1e-3);
	this->m_Model->SetP(0.1);
	this->m_Model->DoShrinking(1);
	this->m_Model->DoProbabilityEstimates(true);
}
#else
template<class TInputImage, class TTrainingImage>
SVMImageModelEstimator<TInputImage, TTrainingImage>
::SVMImageModelEstimator():  SVMModelEstimator<ITK_TYPENAME TInputImage::PixelType::ComponentType, ITK_TYPENAME TTrainingImage::PixelType >::SVMModelEstimator()
{
}
#endif
*/
template<class TInputImage, class TTrainingImage>
SVMImageModelEstimator<TInputImage, TTrainingImage>
::SVMImageModelEstimator()
{
}

template<class TInputImage,
         class TTrainingImage>
SVMImageModelEstimator<TInputImage, TTrainingImage>
::~SVMImageModelEstimator(void)
{
}

/*
 * PrintSelf
 */
template<class TInputImage,
         class TTrainingImage>
void
SVMImageModelEstimator<TInputImage, TTrainingImage>
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf(os,indent);
}// end PrintSelf


/**
 * Generate data (start the model building process)
 */


template<class TInputImage,
         class TTrainingImage>
void
SVMImageModelEstimator<TInputImage,  TTrainingImage>
::BuildProblem()
{

    //Do some error checking
  InputImagePointer  inputImage = this->GetInputImage();

  // Check if the training and input image dimensions are same
  if( (int)(TInputImage::ImageDimension) != (int)(TTrainingImage::ImageDimension) )
    {
    throw itk::ExceptionObject(__FILE__, __LINE__,"Training and input image dimensions are not the same.",ITK_LOCATION);
    }

  InputImageSizeType
    inputImageSize = inputImage->GetBufferedRegion().GetSize();

  typedef InputImageSizeType TrainingImageSizeType;

  TrainingImagePointer  trainingImage = this->GetTrainingImage();

  TrainingImageSizeType
    trainingImageSize = trainingImage->GetBufferedRegion().GetSize();

  // Check if size of the two inputs are same
  for( unsigned int i = 0; i < TInputImage::ImageDimension; i++)
    {
    if( inputImageSize[i] != trainingImageSize[i] ) throw itk::ExceptionObject(__FILE__, __LINE__,"Input image size is not the same as the training image size.",ITK_LOCATION);
    }


  // Declaration of the iterators on the input and training images

  typedef itk::ImageRegionConstIterator< TInputImage > InputIteratorType;
  typedef itk::ImageRegionConstIterator< TTrainingImage > TrainingIteratorType;

  InputIteratorType inIt(inputImage,inputImage->GetBufferedRegion());
  TrainingIteratorType trIt(trainingImage,trainingImage->GetBufferedRegion());

  inIt.GoToBegin();
  trIt.GoToBegin();

  // Erase the vector contents
  this->m_Measures.resize(0);
  this->m_Labels.resize(0);




  //This works with Image< itk::Vector > and with VectorImage< scalar >.
  unsigned int numberOfComponents = inIt.Get().Size();


 //  otbMsgDevMacro(  << " Before while " );
  while(!inIt.IsAtEnd() && !trIt.IsAtEnd())
    {

    if(trIt.Get()!=0)
      {
      this->m_Labels.push_back(trIt.Get());

      typename Superclass::MeasurementVectorType v;

      for(unsigned int k=0; k<numberOfComponents; k++)
	{
	v.push_back(inIt.Get()[k]);
	}

      this->m_Measures.push_back(v);

      }
    ++inIt;
    ++trIt;
    }

  this->PrepareData();
}



}//End namespace OTB
#endif
