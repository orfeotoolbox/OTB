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
#ifndef __otbBSplinesInterpolateDisplacementFieldGenerator_txx
#define __otbBSplinesInterpolateDisplacementFieldGenerator_txx

#include "otbBSplinesInterpolateDisplacementFieldGenerator.h"
// Disabling deprecation warning if on visual
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4996)
#endif
#include "itkLandmarkDisplacementFieldSource.h"
// Enabling remaining deprecation warning
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#include "otbMacro.h"
#include "otbImage.h"
#include "itkImageRegionIterator.h"

namespace otb
{
/** Main computation method */
template <class TPointSet, class TDisplacementField>
void
BSplinesInterpolateDisplacementFieldGenerator<TPointSet, TDisplacementField>
::GenerateData(void)
{
  DisplacementFieldPointerType outputPtr = this->GetOutput();
  typedef itk::Vector<ValueType, 2>                                      VectorType;
  typedef otb::Image<VectorType, DisplacementFieldType::ImageDimension>  ImageType;
  typedef itk::LandmarkDisplacementFieldSource<ImageType>                DisplacementFieldSourceType;
  typedef typename DisplacementFieldSourceType::LandmarkContainerPointer LandmarkContainerPointer;
  typedef typename DisplacementFieldSourceType::LandmarkContainer        LandmarkContainerType;
  typedef typename DisplacementFieldSourceType::LandmarkPointType        LandmarkPointType;
  typedef typename PointSetType::PointsContainer                         PointsContainer;
  typedef typename PointsContainer::ConstIterator                        PointsIterator;
  typedef typename PointSetType::PointDataContainer                      PointDataContainer;
  typedef typename PointDataContainer::ConstIterator                     PointDataIterator;

  typename LandmarkContainerType::Pointer sourceLandmarks = LandmarkContainerType::New();
  typename LandmarkContainerType::Pointer targetLandmarks = LandmarkContainerType::New();
  LandmarkPointType                       sourcePoint;
  LandmarkPointType                       targetPoint;

  PointsIterator pointIterator = this->GetPointSet()->GetPoints()->Begin();
  PointsIterator end = this->GetPointSet()->GetPoints()->End();
  unsigned int   pointId = 0;

  PointDataIterator pointDataIterator = this->GetPointSet()->GetPointData()->Begin();

  while (pointIterator != end)
    {
    typename PointDataContainer::Element valueAndDisplacements = pointDataIterator.Value();

    if (vcl_abs(valueAndDisplacements[0]) >= this->GetMetricThreshold())
      {
      typename PointSetType::PointType p = pointIterator.Value();   // access the point
      sourcePoint[0] = p[0];
      sourcePoint[1] = p[1];
      targetPoint[0] = p[0] - valueAndDisplacements[1];
      targetPoint[1] = p[1] - valueAndDisplacements[2];
      otbMsgDebugMacro(
        << "Adding landmark " << pointId << ", source point: " << sourcePoint << ", targetpoint: " << targetPoint);
      sourceLandmarks->InsertElement(pointId, sourcePoint);
      targetLandmarks->InsertElement(pointId, targetPoint);
      ++pointId;
      }
    ++pointIterator;
    ++pointDataIterator;
    }

  typename DisplacementFieldSourceType::Pointer deformer = DisplacementFieldSourceType::New();
  deformer->SetOutputSpacing(this->GetOutputSpacing());
  deformer->SetOutputOrigin(this->GetOutputOrigin());
  deformer->SetOutputRegion(outputPtr->GetRequestedRegion());
  deformer->SetSourceLandmarks(sourceLandmarks.GetPointer());
  deformer->SetTargetLandmarks(targetLandmarks.GetPointer());

  deformer->Update();
  outputPtr->Allocate();
  PixelType defaultPixel;
  defaultPixel.SetSize(2);
  defaultPixel.Fill(this->GetDefaultValue());
  outputPtr->FillBuffer(defaultPixel);
  typedef itk::ImageRegionIterator<ImageType>            ImageIteratorType;
  typedef itk::ImageRegionIterator<DisplacementFieldType> OutputIteratorType;

  ImageIteratorType inIt(deformer->GetOutput(), outputPtr->GetRequestedRegion());
  OutputIteratorType outIt(outputPtr, outputPtr->GetRequestedRegion());
  int i = 0;
  // Casting otb::Image<itt::Vector<ValueType, 2>, 2> to otb::VectorImage<ValueType, 2>
  for (inIt.GoToBegin(), outIt.GoToBegin(); (!inIt.IsAtEnd()) && (!outIt.IsAtEnd()); ++inIt, ++outIt, ++i)
    {
    typename ImageType::PixelType inPixel;
    inPixel = inIt.Get();
    PixelType outPixel;
    outPixel.SetSize(2);
    outPixel[0] = -inPixel[0];
    outPixel[1] = -inPixel[1];
    outIt.Set(outPixel);
    }
}
/**
 * PrintSelf Method
 */
template <class TPointSet, class TDisplacementField>
void
BSplinesInterpolateDisplacementFieldGenerator<TPointSet, TDisplacementField>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
