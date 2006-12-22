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
#ifndef _otbDisparityMapEstimationMethod_txx
#define _otbDisparityMapEstimationMethod_txx

#include "otbDisparityMapEstimationMethod.h"
#include "itkImageRegistrationMethod.h"
#include "otbExtractROI.h"
#include "otbMacro.h"

namespace otb
{
/*
 * Constructor.
 */
template < typename TFixedImage, typename TMovingImage >
DisparityMapEstimationMethod<TFixedImage,TMovingImage>
::DisparityMapEstimationMethod()
{
  this->SetNumberOfRequiredInputs(0);
  this->SetNumberOfRequiredOutputs(1);  // for the Transform
 //  m_FixedImage   = 0; // has to be provided by the user.
//   m_MovingImage  = 0; // has to be provided by the user.
//   m_Transform    = 0; // has to be provided by the user.
//   m_Interpolator = 0; // has to be provided by the user.
//   m_Metric       = 0; // has to be provided by the user.
//   m_Optimizer    = 0; // has to be provided by the user.
//   m_WinSize      = 15;
//   m_ExploSize    = 10;
//   m_InitialTransformParameters = ParametersType(1);
//   m_LastTransformParameters = ParametersType(1);
//   m_InitialTransformParameters.Fill( 0.0f );
//   m_LastTransformParameters.Fill( 0.0f );
//   m_FixedImageRegionDefined = false;
//   TransformOutputPointer transformDecorator=static_cast< TransformOutputType*>
//     (this->MakeOutput(0).GetPointer());
//   this->ProcessObject::SetNthOutput(0,transformDecorator.GetPointer());
}
/**
 * Main computation method.
 *
 */
template < typename TFixedImage, typename TMovingImage >
void
DisparityMapEstimationMethod<TFixedImage,TMovingImage>
::GenerateData(void)
{
  // inputs pointers
  FixedImagePointerType fixed = this->GetFixedImage();
  MovingImagePointerType moving = this->GetMovingImage();
  PointSetPointerType pointSet = this->GetPointSet();
  
  // Typedefs 
  typedef typename PointSetType::PointsContainer PointsContainer;
  typedef typename PointsContainer::Iterator PointsIterator;
  typedef itk::ImageRegistrationMethod<FixedImageType,MovingImageType> RegistrationType;
  typedef otb::ExtractROI<FixedPixelType,FixedPixelType> FixedExtractType;
  typedef otb::ExtractROI<MovingPixelType,MovingPixelType> MovingExtractType;

  // points retrieving
  typename PointsContainer::Pointer points = m_PointSet->GetPoints();

  // Iterator set up 
  PointsIterator  pointIterator = points->Begin();
  PointsIterator end = points->End();
  unsigned int dataId = 0;

  otbMsgDebugMacro(<<"Starting registration");
  while(pointIterator!= end) 
    {
    typename PointSetType::PointType p = pointIterator.Value();   // access the point    

    // Extract the needed sub-images
    typename FixedExtractType::Pointer fixedExtractor = FixedExtractType::New();
    typename MovingExtractType::Pointer movingExtractor = MovingExtractType::New();
    fixedExtractor->SetInput(fixed);
    movingExtractor->SetInput(moving);
    // Fixed extractor setup
    fixedExtractor->SetStartX( static_cast<unsigned int>(p[0]-m_ExploSize) );
    fixedExtractor->SetStartY( static_cast<unsigned int>(p[1]-m_ExploSize) );
    fixedExtractor->SetSizeX( 2*m_ExploSize+m_WinSize );
    fixedExtractor->SetSizeY( 2*m_ExploSize+m_WinSize );
    // Moving extractor setup
    movingExtractor->SetStartX( static_cast<unsigned int>(p[0]-m_WinSize/2) );
    movingExtractor->SetStartY( static_cast<unsigned int>(p[1]-m_WinSize/2) );
    movingExtractor->SetSizeX( m_WinSize );
    movingExtractor->SetSizeY( m_WinSize );
    // update the extractors
    fixedExtractor->Update();
    movingExtractor->Update();

    // Registration filter definition
    typename RegistrationType::Pointer   registration = RegistrationType::New();
    // Registration filter setup
    registration->SetOptimizer(m_Optimizer);
    registration->SetTransform(m_Transform);
    registration->SetInterpolator(m_Interpolator);
    registration->SetMetric(m_Metric);
    registration->SetFixedImage(fixedExtractor->GetOutput());
    registration->SetMovingImage(movingExtractor->GetOutput());
    
    // initial transform parameters setup
    ParametersType initialParameters(m_Transform->GetNumberOfParameters());
    for(unsigned int i =0; i<m_Transform->GetNumberOfParameters();++i)
      {
	initialParameters[i] = 0.0; 
      }
    registration->SetInitialTransformParameters( initialParameters);
    // Perform the registration
    registration->StartRegistration(); 
    // Retrieve the final parameters
    ParametersType finalParameters = registration->GetLastTransformParameters();
    // Set the parameters value in the point set data container.
    m_PointSet->SetPointData( dataId++, finalParameters );   
    ++pointIterator;// advance to next point
    ++dataId;
    }
}
}
#endif
