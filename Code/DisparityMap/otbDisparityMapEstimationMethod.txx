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
template < typename TFixedImage, typename TMovingImage, class TPointSet >
DisparityMapEstimationMethod<TFixedImage,TMovingImage,TPointSet>
::DisparityMapEstimationMethod()
{
  this->SetNumberOfRequiredInputs(0);
  this->SetNumberOfRequiredOutputs(1);
  m_FixedImage   = 0; // has to be provided by the user.
  m_MovingImage  = 0; // has to be provided by the user.
  m_Transform    = 0; // has to be provided by the user.
  m_Interpolator = 0; // has to be provided by the user.
  m_Metric       = 0; // has to be provided by the user.
  m_Optimizer    = 0; // has to be provided by the user.
  m_WinSize.Fill(15);
  m_ExploSize.Fill(10);
  m_InitialTransformParameters = ParametersType(1);
  m_InitialTransformParameters.Fill( 0.0f );
}
/*
 * Destructor.
 */
template < typename TFixedImage, typename TMovingImage, class TPointSet >
DisparityMapEstimationMethod<TFixedImage,TMovingImage,TPointSet>
::~DisparityMapEstimationMethod()
{}
/**
 * Main computation method.
 *
 */
template < typename TFixedImage, typename TMovingImage, class TPointSet >
void
DisparityMapEstimationMethod<TFixedImage,TMovingImage,TPointSet>
::GenerateData(void)
{
  // inputs pointers
  FixedImagePointerType fixed = this->GetFixedImage();
  MovingImagePointerType moving = this->GetMovingImage();
  PointSetPointerType pointSet = this->GetPointSet();
  PointSetType* output = this->GetOutput();
  
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
  
  /// Iterate through the point set
  while(pointIterator!= end) 
    {
    typename PointSetType::PointType p = pointIterator.Value();   // access the point    

    // Extract the needed sub-images
    typename FixedExtractType::Pointer fixedExtractor = FixedExtractType::New();
    typename MovingExtractType::Pointer movingExtractor = MovingExtractType::New();
    fixedExtractor->SetInput(fixed);
    movingExtractor->SetInput(moving);
    
    // Fixed extractor setup
    fixedExtractor->SetStartX(static_cast<unsigned int>(p[0]-m_ExploSize[0]));
    fixedExtractor->SetStartY(static_cast<unsigned int>(p[1]-m_ExploSize[1]));
    fixedExtractor->SetSizeX(2*m_ExploSize[0]+1);
    fixedExtractor->SetSizeY(2*m_ExploSize[1]+1);
    otbMsgDebugMacro(<<"Point id: "<<dataId);
    otbMsgDebugMacro(<<"Fixed region: origin("<<p[0]-m_ExploSize[0]<<", "<<p[1]-m_ExploSize[1]<<") size("<<2*m_ExploSize[0]+1<<", "<<2*m_ExploSize[1]+1<<")");
    // Moving extractor setup
    movingExtractor->SetStartX(static_cast<unsigned int>(p[0]-m_WinSize[0]));
    movingExtractor->SetStartY(static_cast<unsigned int>(p[1]-m_WinSize[1]));
    movingExtractor->SetSizeX(2*m_WinSize[0]+1);
    movingExtractor->SetSizeY(2*m_WinSize[1]+1);
    otbMsgDebugMacro(<<"Moving region: origin("<<p[0]-m_WinSize[0]<<", "<<p[1]-m_WinSize[1]<<") size("<<2*m_WinSize[0]+1<<", "<<2*m_WinSize[1]+1<<")");
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
    registration->SetInitialTransformParameters( m_InitialTransformParameters);
    m_Interpolator->SetInputImage(movingExtractor->GetOutput());
    
    // Perform the registration
    registration->StartRegistration(); 
    
    // Retrieve the final parameters
    ParametersType finalParameters = registration->GetLastTransformParameters();
    double value = m_Optimizer->GetValue(registration->GetLastTransformParameters());

    // Computing moving image point
    typename FixedImageType::PointType inputPoint,outputPoint;
    
    // ensure that we have the right coord rep type
    inputPoint[0] = static_cast<double>(p[0]);
    inputPoint[1] = static_cast<double>(p[1]);

    m_Transform->SetParameters(finalParameters);
    outputPoint = m_Transform->TransformPoint(inputPoint);

    otbMsgDebugMacro(<<"Metric value: "<<value);
    otbMsgDebugMacro(<<"Deformation: ("<<outputPoint[0]-inputPoint[0]<<", "<<outputPoint[1]-inputPoint[1]<<")");
    otbMsgDebugMacro(<<"Final parameters: "<<finalParameters);

    ParametersType data(finalParameters.GetSize()+3);

    data[0] = value;
    data[1] = outputPoint[0]-inputPoint[0];
    data[2] = outputPoint[1]-inputPoint[1];

    for(unsigned int i = 0;i<finalParameters.GetSize();++i)
      {
	data[i+3] = finalParameters[i];
      }

    // Set the parameters value in the point set data container.
    output->SetPoint(dataId,p);
    output->SetPointData(dataId,data);
    // otbMsgDevMacro(<<"Point "<<dataId<<": "<<finalParameters);
    ++pointIterator;// advance to next point
    ++dataId;
    }
}
template < typename TFixedImage, typename TMovingImage, class TPointSet >
void
DisparityMapEstimationMethod<TFixedImage,TMovingImage,TPointSet>
::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
    os << indent << "Window size: " << m_WinSize << std::endl;
    os << indent << "Exploration size: " << m_ExploSize << std::endl;
  }
}
#endif
