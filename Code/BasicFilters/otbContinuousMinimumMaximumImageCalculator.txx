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

#ifndef __otbContinuousMinimumMaximumImageCalculator_txx
#define __otbContinuousMinimumMaximumImageCalculator_txx

#include "otbContinuousMinimumMaximumImageCalculator.h"
#include "itkImageRegionConstIteratorWithIndex.h"

namespace otb
{

/*
  * Constructor
 */
template<class TInputImage>
ContinuousMinimumMaximumImageCalculator<TInputImage>
::ContinuousMinimumMaximumImageCalculator()
{
  m_Image = TInputImage::New();
  m_Maximum = itk::NumericTraits<PixelType>::NonpositiveMin();
  m_Minimum = itk::NumericTraits<PixelType>::max();
  m_IndexOfMinimum.Fill(0);
  m_IndexOfMaximum.Fill(0);
  m_RegionSetByUser = false;
}

/*
  * Compute Min and Max of m_Image
 */
template<class TInputImage>
void
ContinuousMinimumMaximumImageCalculator<TInputImage>
::Compute(void)
{
  if (!m_RegionSetByUser)
    {
    m_Region = m_Image->GetRequestedRegion();
    }

  itk::ImageRegionConstIteratorWithIndex<TInputImage>  it(m_Image, m_Region);
  m_Maximum = itk::NumericTraits<PixelType>::NonpositiveMin();
  m_Minimum = itk::NumericTraits<PixelType>::max();

  while (!it.IsAtEnd())
    {
    const RealPixelType value = it.Get();
    if (value > static_cast<RealPixelType>(m_Maximum))
      {
      m_Maximum = static_cast<PixelType>(value);
      m_IndexOfMaximum = it.GetIndex();
      }
    if (value < static_cast<RealPixelType>(m_Minimum))
      {
      m_Minimum = static_cast<PixelType>(value);
      m_IndexOfMinimum = it.GetIndex();
      }
    ++it;
    }

  IndexType indexNeighbor;

    { //Continuous Minimum calculation
      //Compute horizontal offset
    indexNeighbor[0] = m_IndexOfMinimum[0] - 1;
    indexNeighbor[1] = m_IndexOfMinimum[1];
    it.SetIndex(indexNeighbor);
    const RealPixelType leftValue = it.Get();
    indexNeighbor[0] = m_IndexOfMinimum[0] + 1;
    indexNeighbor[1] = m_IndexOfMinimum[1];
    it.SetIndex(indexNeighbor);
    const RealPixelType rightValue = it.Get();

    double hOffset = -(rightValue - leftValue) / (2 * (rightValue + leftValue - 2 * m_Minimum));

    //Compute vertical offset
    indexNeighbor[0] = m_IndexOfMinimum[0];
    indexNeighbor[1] = m_IndexOfMinimum[1] - 1;
    it.SetIndex(indexNeighbor);
    const RealPixelType topValue = it.Get();
    indexNeighbor[0] = m_IndexOfMinimum[0];
    indexNeighbor[1] = m_IndexOfMinimum[1] + 1;
    it.SetIndex(indexNeighbor);
    const RealPixelType bottomValue = it.Get();

    double vOffset = -(bottomValue - topValue) / (2 * (bottomValue + topValue - 2 * m_Minimum));

    m_ContinuousIndexOfMinimum[0] = m_IndexOfMinimum[0] + hOffset;
    m_ContinuousIndexOfMinimum[1] = m_IndexOfMinimum[1] + vOffset;
    }

    { //Continuous Maximum calculation
      //Compute horizontal offset
    indexNeighbor[0] = m_IndexOfMaximum[0] - 1;
    indexNeighbor[1] = m_IndexOfMaximum[1];
    it.SetIndex(indexNeighbor);
    const RealPixelType leftValue = it.Get();
    indexNeighbor[0] = m_IndexOfMaximum[0] + 1;
    indexNeighbor[1] = m_IndexOfMaximum[1];
    it.SetIndex(indexNeighbor);
    const RealPixelType rightValue = it.Get();

    double hOffset = -(rightValue - leftValue) / (2 * (rightValue + leftValue - 2 * m_Maximum));

    //Compute vertical offset
    indexNeighbor[0] = m_IndexOfMaximum[0];
    indexNeighbor[1] = m_IndexOfMaximum[1] - 1;
    it.SetIndex(indexNeighbor);
    const RealPixelType topValue = it.Get();
    indexNeighbor[0] = m_IndexOfMaximum[0];
    indexNeighbor[1] = m_IndexOfMaximum[1] + 1;
    it.SetIndex(indexNeighbor);
    const RealPixelType bottomValue = it.Get();

    double vOffset = -(bottomValue - topValue) / (2 * (bottomValue + topValue - 2 * m_Maximum));

    m_ContinuousIndexOfMaximum[0] = m_IndexOfMaximum[0] + hOffset;
    m_ContinuousIndexOfMaximum[1] = m_IndexOfMaximum[1] + vOffset;
    }

}

/*
  * Compute the minimum intensity value of the image
 */
template<class TInputImage>
void
ContinuousMinimumMaximumImageCalculator<TInputImage>
::ComputeMinimum(void)
{
  if (!m_RegionSetByUser)
    {
    m_Region = m_Image->GetRequestedRegion();
    }
  itk::ImageRegionConstIteratorWithIndex<TInputImage>  it(m_Image,  m_Region);
  m_Minimum = itk::NumericTraits<PixelType>::max();

  while (!it.IsAtEnd())
    {
    const RealPixelType value = it.Get();
    if (value < static_cast<RealPixelType>(m_Minimum))
      {
      m_Minimum = static_cast<PixelType>(value);
      m_IndexOfMinimum = it.GetIndex();
      }
    ++it;
    }

  IndexType indexNeighbor;

  //Compute horizontal offset
  indexNeighbor[0] = m_IndexOfMinimum[0] - 1;
  indexNeighbor[1] = m_IndexOfMinimum[1];
  it.SetIndex(indexNeighbor);
  const RealPixelType leftValue = it.Get();
  indexNeighbor[0] = m_IndexOfMinimum[0] + 1;
  indexNeighbor[1] = m_IndexOfMinimum[1];
  it.SetIndex(indexNeighbor);
  const RealPixelType rightValue = it.Get();

  double hOffset = -(rightValue - leftValue) / (2 * (rightValue + leftValue - 2 * m_Minimum));

  //Compute vertical offset
  indexNeighbor[0] = m_IndexOfMinimum[0];
  indexNeighbor[1] = m_IndexOfMinimum[1] - 1;
  it.SetIndex(indexNeighbor);
  const RealPixelType topValue = it.Get();
  indexNeighbor[0] = m_IndexOfMinimum[0];
  indexNeighbor[1] = m_IndexOfMinimum[1] + 1;
  it.SetIndex(indexNeighbor);
  const RealPixelType bottomValue = it.Get();

  double vOffset = -(bottomValue - topValue) / (2 * (bottomValue + topValue - 2 * m_Minimum));

  m_ContinuousIndexOfMinimum[0] = m_IndexOfMinimum[0] + hOffset;
  m_ContinuousIndexOfMinimum[1] = m_IndexOfMinimum[1] + vOffset;

}

/*
  * Compute the maximum intensity value of the image
 */
template<class TInputImage>
void
ContinuousMinimumMaximumImageCalculator<TInputImage>
::ComputeMaximum(void)
{
  if (!m_RegionSetByUser)
    {
    m_Region = m_Image->GetRequestedRegion();
    }
  itk::ImageRegionConstIteratorWithIndex<TInputImage>  it(m_Image,  m_Region);
  m_Maximum = itk::NumericTraits<PixelType>::NonpositiveMin();

  while (!it.IsAtEnd())
    {
    const RealPixelType value = it.Get();
    if (value > static_cast<RealPixelType>(m_Maximum))
      {
      m_Maximum = static_cast<PixelType>(value);
      m_IndexOfMaximum = it.GetIndex();
      }
    ++it;
    }

  //Solve equations a, b, c
//     y0 = a*x0^2 + b*x0 +c
//     y1 = a*x1^2 + b*x1 +c
//     y2 = a*x2^2 + b*x2 +c
//
//     y0 = a - b +c
//     y1 = c
//     y2 = a + b +c
//
//
// Max is at -b/2a
// -(y2-y0)/(2*(y0+y2-2y1))
  IndexType indexNeighbor;

  //Compute horizontal offset
  indexNeighbor[0] = m_IndexOfMaximum[0] - 1;
  indexNeighbor[1] = m_IndexOfMaximum[1];
  it.SetIndex(indexNeighbor);
  const RealPixelType leftValue = it.Get();
  indexNeighbor[0] = m_IndexOfMaximum[0] + 1;
  indexNeighbor[1] = m_IndexOfMaximum[1];
  it.SetIndex(indexNeighbor);
  const RealPixelType rightValue = it.Get();

  double hOffset = -(rightValue - leftValue) / (2 * (rightValue + leftValue - 2 * m_Maximum));

  //Compute vertical offset
  indexNeighbor[0] = m_IndexOfMaximum[0];
  indexNeighbor[1] = m_IndexOfMaximum[1] - 1;
  it.SetIndex(indexNeighbor);
  const RealPixelType topValue = it.Get();
  indexNeighbor[0] = m_IndexOfMaximum[0];
  indexNeighbor[1] = m_IndexOfMaximum[1] + 1;
  it.SetIndex(indexNeighbor);
  const RealPixelType bottomValue = it.Get();

  double vOffset = -(bottomValue - topValue) / (2 * (bottomValue + topValue - 2 * m_Maximum));

  m_ContinuousIndexOfMaximum[0] = m_IndexOfMaximum[0] + hOffset;
  m_ContinuousIndexOfMaximum[1] = m_IndexOfMaximum[1] + vOffset;

}

template<class TInputImage>
void
ContinuousMinimumMaximumImageCalculator<TInputImage>
::SetRegion(const RegionType& region)
{
  m_Region = region;
  m_RegionSetByUser = true;
}

template<class TInputImage>
void
ContinuousMinimumMaximumImageCalculator<TInputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Minimum: "
     << static_cast<typename itk::NumericTraits<PixelType>::PrintType>(m_Minimum)
     << std::endl;
  os << indent << "Maximum: "
     << static_cast<typename itk::NumericTraits<PixelType>::PrintType>(m_Maximum)
     << std::endl;
  os << indent << "Index of Minimum: " << m_IndexOfMinimum << std::endl;
  os << indent << "Index of Maximum: " << m_IndexOfMaximum << std::endl;
  os << indent << "Continuous Index of Minimum: " << m_ContinuousIndexOfMinimum << std::endl;
  os << indent << "Continuous Index of Maximum: " << m_ContinuousIndexOfMaximum << std::endl;

  os << indent << "Image: " << std::endl;
  m_Image->Print(os, indent.GetNextIndent());
  os << indent << "Region: " << std::endl;
  m_Region.Print(os, indent.GetNextIndent());
  os << indent << "Region set by User: " << m_RegionSetByUser << std::endl;
}

} // end namespace otb

#endif
