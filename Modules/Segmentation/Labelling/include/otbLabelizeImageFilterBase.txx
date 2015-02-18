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
#ifndef __otbLabelizeImageFilterBase_txx
#define __otbLabelizeImageFilterBase_txx

#include "otbLabelizeImageFilterBase.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFilter>
LabelizeImageFilterBase<TInputImage, TOutputImage, TFilter>
::LabelizeImageFilterBase()
{
  m_LowerThreshold = itk::NumericTraits<InputPixelType>::NonpositiveMin();
  m_UpperThreshold = itk::NumericTraits<InputPixelType>::max();

  m_MultiplyFilter = MultiplyFilterType::New();
  m_MultiplyFilter->SetCoef(0.0);

  m_ThresholdPointSetFilter = ThresholdFilterType::New();

  m_RegionGrowingFilter = RegionGrowingFilterType::New();
}

/** Generate data
 *
 */
template <class TInputImage, class TOutputImage, class TFilter>
void
LabelizeImageFilterBase<TInputImage, TOutputImage, TFilter>
::GenerateData()
{
  // set input for region growing filter
  m_RegionGrowingFilter->SetInput(this->GetInput());

  // create an empty image to store results computing
  m_MultiplyFilter->SetInput(this->GetInput());
  m_MultiplyFilter->Update();

  // Compute points set
  m_ThresholdPointSetFilter->SetInput(0, this->GetInput());
  m_ThresholdPointSetFilter->SetLowerThreshold(m_LowerThreshold);
  m_ThresholdPointSetFilter->SetUpperThreshold(m_UpperThreshold);
  m_ThresholdPointSetFilter->Update();
  m_PointSet = m_ThresholdPointSetFilter->GetOutput();
  m_ObjectCount = 0;

  // Iterate Point set
  typedef typename PointSetType::PointsContainer ContainerType;
  ContainerType* pointsContainer = m_PointSet->GetPoints();
  typedef typename ContainerType::Iterator IteratorType;
  IteratorType itList = pointsContainer->Begin();

  typename OutputImageType::Pointer outputImage = m_MultiplyFilter->GetOutput();

  while (itList != pointsContainer->End())
    {
    typename PointSetType::PointType   pCoordinate = (itList.Value());
    typename InputImageType::IndexType index;

    index[0] = static_cast <int> (pCoordinate[0]);
    index[1] = static_cast <int> (pCoordinate[1]);
    if (outputImage->GetPixel(index) ==
        itk::NumericTraits<OutputPixelType>::ZeroValue())
      {
      this->RegionGrowing(index);

      AddImageFilterPointerType addImage = AddImageFilterType::New();
      addImage->SetInput1(outputImage);
      addImage->SetInput2(m_RegionGrowingFilter->GetOutput());
      addImage->Update();
      outputImage = addImage->GetOutput();
      ++m_ObjectCount;
      }
    ++itList;
    }

  this->GraftOutput(outputImage);
}

/** PrintSelf Method
 *
 */
template <class TInputImage, class TOutputImage, class TFilter>
void
LabelizeImageFilterBase<TInputImage, TOutputImage, TFilter>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Seeds lower threshold: " << m_LowerThreshold << std::endl;
  os << indent << "Seeds upper threshold: " << m_UpperThreshold << std::endl;
  os << indent << "ObjectCount: "  << m_ObjectCount << std::endl;
  os << indent << m_RegionGrowingFilter << std::endl;
}
} // end namespace otb

#endif
