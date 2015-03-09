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
#ifndef __otbDisparityMapEstimationMethod_txx
#define __otbDisparityMapEstimationMethod_txx

#include "otbDisparityMapEstimationMethod.h"
#include "itkImageRegistrationMethod.h"
#include "otbExtractROI.h"
#include "otbMacro.h"

// #include "otbImageFileWriter.h"
//

namespace otb
{
/*
 * Constructor.
 */
template <typename TFixedImage, typename TMovingImage, class TPointSet>
DisparityMapEstimationMethod<TFixedImage, TMovingImage, TPointSet>
::DisparityMapEstimationMethod()
{
  this->SetNumberOfRequiredInputs(3);
  //this->SetNumberOfRequiredOutputs(1);
  // this->SetReleaseDataFlag(false);
  this->SetReleaseDataBeforeUpdateFlag(false);
  m_Transform    = 0; // has to be provided by the user.
  m_Interpolator = 0; // has to be provided by the user.
  m_Metric       = 0; // has to be provided by the user.
  m_Optimizer    = 0; // has to be provided by the user.
  m_WinSize.Fill(15);
  m_ExploSize.Fill(10);
  m_InitialTransformParameters = ParametersType(1);
  m_InitialTransformParameters.Fill(0.0f);
}
/*
 * Destructor.
 */
template <class TFixedImage, class TMovingImage, class TPointSet>
DisparityMapEstimationMethod<TFixedImage, TMovingImage, TPointSet>
::~DisparityMapEstimationMethod()
{}
/**
 * Set the source pointset.
 * \param pointset The source pointset.
 */
template <class TFixedImage, class TMovingImage, class TPointSet>
void
DisparityMapEstimationMethod<TFixedImage, TMovingImage, TPointSet>
::SetPointSet(const TPointSet * pointset)
{
  this->itk::ProcessObject::SetNthInput(0, const_cast<PointSetType *>(pointset));
}
/**
 * Get the source pointset.
 * \return The source pointset.
 */
template <class TFixedImage, class TMovingImage, class TPointSet>
const TPointSet *
DisparityMapEstimationMethod<TFixedImage, TMovingImage, TPointSet>
::GetPointSet(void)
{
  return static_cast<const PointSetType *>(this->itk::ProcessObject::GetInput(0));
}
/**
 * Set the fixed image.
 * \param image The fixed image.
 **/
template <class TFixedImage, class TMovingImage, class TPointSet>
void
DisparityMapEstimationMethod<TFixedImage, TMovingImage, TPointSet>
::SetFixedImage(const TFixedImage * image)
{
  this->itk::ProcessObject::SetNthInput(1, const_cast<FixedImageType *>(image));
}
/**
 * Get the fixed image.
 * \return The fixed image.
 **/
template <class TFixedImage, class TMovingImage, class TPointSet>
const TFixedImage *
DisparityMapEstimationMethod<TFixedImage, TMovingImage, TPointSet>
::GetFixedImage(void)
{
  return static_cast<const FixedImageType *>(this->itk::ProcessObject::GetInput(1));
}
/**
 * Set the moving image.
 * \param image The mobing image.
 **/
template <class TFixedImage, class TMovingImage, class TPointSet>
void
DisparityMapEstimationMethod<TFixedImage, TMovingImage, TPointSet>
::SetMovingImage(const TMovingImage * image)
{
  this->itk::ProcessObject::SetNthInput(2, const_cast<MovingImageType *>(image));
}
/**
 * Get the fixed image.
 * \return The fixed image.
 **/
template <class TFixedImage, class TMovingImage, class TPointSet>
const TMovingImage *
DisparityMapEstimationMethod<TFixedImage, TMovingImage, TPointSet>
::GetMovingImage(void)
{
  return static_cast<const MovingImageType *>(this->itk::ProcessObject::GetInput(2));
}

/**
 * Main computation method.
 */
template <class TFixedImage, class TMovingImage, class TPointSet>
void
DisparityMapEstimationMethod<TFixedImage, TMovingImage, TPointSet>
::GenerateData(void)
{
  // inputs pointers
  const FixedImageType *  fixed = this->GetFixedImage();
  const MovingImageType * moving = this->GetMovingImage();
  const PointSetType *    pointSet = this->GetPointSet();
  PointSetType*           output = this->GetOutput();

  // Typedefs
  typedef typename PointSetType::PointsContainer                        PointsContainer;
  typedef typename PointsContainer::ConstIterator                       PointsIterator;
  typedef itk::ImageRegistrationMethod<FixedImageType, MovingImageType> RegistrationType;
  typedef otb::ExtractROI<FixedPixelType, FixedPixelType>               FixedExtractType;
  typedef otb::ExtractROI<MovingPixelType, MovingPixelType>             MovingExtractType;

  // points retrieving
  typename PointsContainer::ConstPointer points = pointSet->GetPoints();

  // Iterator set up
  PointsIterator pointIterator = points->Begin();
  PointsIterator end = points->End();
  unsigned int   dataId = 0;

  otbMsgDevMacro(<< "Starting registration");

  /// Iterate through the point set
  while (pointIterator != end)
    {
    typename PointSetType::PointType p = pointIterator.Value();   // access the point

    // Extract the needed sub-images
    typename FixedExtractType::Pointer  fixedExtractor = FixedExtractType::New();
    typename MovingExtractType::Pointer movingExtractor = MovingExtractType::New();
    fixedExtractor->SetInput(fixed);
    movingExtractor->SetInput(moving);

    // Fixed extractor setup
    fixedExtractor->SetStartX(static_cast<unsigned long>(p[0] - m_ExploSize[0]));
    fixedExtractor->SetStartY(static_cast<unsigned long>(p[1] - m_ExploSize[1]));
    fixedExtractor->SetSizeX(static_cast<unsigned long>(2 * m_ExploSize[0] + 1));
    fixedExtractor->SetSizeY(static_cast<unsigned long>(2 * m_ExploSize[1] + 1));
    otbMsgDevMacro(<< "Point id: " << dataId);
    otbMsgDevMacro(
      << "Fixed region: origin(" << p[0] - m_ExploSize[0] << ", " << p[1] - m_ExploSize[1] << ") size(" << 2 *
      m_ExploSize[0] + 1 << ", " << 2 * m_ExploSize[1] + 1 << ")");
    // Moving extractor setup
    movingExtractor->SetStartX(static_cast<unsigned long>(p[0] - m_WinSize[0]));
    movingExtractor->SetStartY(static_cast<unsigned long>(p[1] - m_WinSize[1]));
    movingExtractor->SetSizeX(static_cast<unsigned long>(2 * m_WinSize[0] + 1));
    movingExtractor->SetSizeY(static_cast<unsigned long>(2 * m_WinSize[1] + 1));
    otbMsgDevMacro(
      << "Moving region: origin(" << p[0] - m_WinSize[0] << ", " << p[1] - m_WinSize[1] << ") size(" << 2 *
      m_WinSize[0] + 1 << ", " << 2 * m_WinSize[1] + 1 << ")");
    // update the extractors
    fixedExtractor->Update();
    movingExtractor->Update();

//     std::cout<<"Fixed extract origin: "<<fixedExtractor->GetOutput()->GetOrigin()<<std::endl;
//     std::cout<<"Fixed extract spacing: "<<fixedExtractor->GetOutput()->GetSpacing()<<std::endl;
//     std::cout<<"Moving extract origin: "<<movingExtractor->GetOutput()->GetOrigin()<<std::endl;
//     std::cout<<"Moving extract spacing: "<<movingExtractor->GetOutput()->GetSpacing()<<std::endl;

//     typedef otb::ImageFileWriter<FixedImageType> FixedWriterType;
//      typedef otb::ImageFileWriter<MovingImageType> MovingWriterType;

//      typename FixedWriterType::Pointer fwriter = FixedWriterType::New();
//      typename MovingWriterType::Pointer mwriter = MovingWriterType::New();

//      std::ostringstream oss;
//      oss.str("");
//      oss<<"Fixed"<<dataId<<".tif";
//      fwriter->SetInput(fixedExtractor->GetOutput());
//      fwriter->SetFileName(oss.str());
//      fwriter->Update();

//      oss.str("");
//      oss<<"Moving"<<dataId<<".tif";
//      mwriter->SetInput(movingExtractor->GetOutput());
//      mwriter->SetFileName(oss.str());
//      mwriter->Update();

    // Registration filter definition
    typename RegistrationType::Pointer registration = RegistrationType::New();

    // Registration filter setup
    registration->SetOptimizer(m_Optimizer);
    registration->SetTransform(m_Transform);
    registration->SetInterpolator(m_Interpolator);
    registration->SetMetric(m_Metric);
    registration->SetFixedImage(fixedExtractor->GetOutput());
    registration->SetMovingImage(movingExtractor->GetOutput());

    // initial transform parameters setup
    registration->SetInitialTransformParameters(m_InitialTransformParameters);
    m_Interpolator->SetInputImage(movingExtractor->GetOutput());

    // Perform the registration
    registration->Update();

    // Retrieve the final parameters
    ParametersType finalParameters = registration->GetLastTransformParameters();
    double         value = m_Optimizer->GetValue(registration->GetLastTransformParameters());

    // Computing moving image point
    typename FixedImageType::PointType inputPoint, outputPoint;
    typename FixedImageType::IndexType inputIndex;

    // ensure that we have the right coord rep type
    inputIndex[0] = static_cast<unsigned int>(p[0]);
    inputIndex[1] = static_cast<unsigned int>(p[1]);

    fixed->TransformIndexToPhysicalPoint(inputIndex, inputPoint);

    m_Transform->SetParameters(finalParameters);
    outputPoint = m_Transform->TransformPoint(inputPoint);

    otbMsgDevMacro(<< "Metric value: " << value);
    otbMsgDevMacro(<< "Transform parameters: " << finalParameters);
    otbMsgDevMacro(
      << "Displacement: (" << outputPoint[0] - inputPoint[0] << ", " << outputPoint[1] - inputPoint[1] << ")");
    otbMsgDevMacro(<< "Final parameters: " << finalParameters);

    ParametersType data(finalParameters.GetSize() + 3);

    data[0] = value;
    data[1] = outputPoint[0] - inputPoint[0];
    data[2] = outputPoint[1] - inputPoint[1];

    for (unsigned int i = 0; i < finalParameters.GetSize(); ++i)
      {
      data[i + 3] = finalParameters[i];
      }

    // Set the parameters value in the point set data container.
    output->SetPoint(dataId, p);
    output->SetPointData(dataId, data);
    // otbMsgDevMacro(<<"Point "<<dataId<<": "<<finalParameters);
    ++pointIterator; // advance to next point
    ++dataId;
    }
}
template <class TFixedImage, class TMovingImage, class TPointSet>
void
DisparityMapEstimationMethod<TFixedImage, TMovingImage, TPointSet>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Window size: " << m_WinSize << std::endl;
  os << indent << "Exploration size: " << m_ExploSize << std::endl;
}
}
#endif
