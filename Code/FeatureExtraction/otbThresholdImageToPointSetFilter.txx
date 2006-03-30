/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   30 mars 2006
  Role      :   ThresholdImage To PointSet Filter  
  $Id$ 

=========================================================================*/
#ifndef __otbThresholdImageToPointSetFilter_txx
#define __otbThresholdImageToPointSetFilter_txx

#include "otbThresholdImageToPointSetFilter.h"
#include "itkImageRegionConstIterator.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputPointSet>
ThresholdImageToPointSetFilter<TInputImage, TOutputPointSet>
::ThresholdImageToPointSetFilter()
{
  m_Threshold = itk::NumericTraits<InputPixelType>::max();  
}

/**
 *
 */

template <class TInputImage, class TOutputPointSet>
void
ThresholdImageToPointSetFilter<TInputImage, TOutputPointSet>
::GenerateData()
{
  InputImagePointer     inputPtr  = this->GetInput();
  OutputPointSetPointer outputPtr = this->GetOutput(0);

  unsigned int pointId = 0;
  
  typedef itk::ImageRegionConstIterator<TInputImage> InputIterator;
  InputIterator  inIt(inputPtr, inputPtr->GetRequestedRegion() );

          
  // walk the regions, threshold each pixel
  while( !inIt.IsAtEnd() )
    {
    typedef typename TInputImage::PixelType PixelType;
    const PixelType value = inIt.Get();
    const IndexType index = inIt.GetIndex();
    if (value >= m_Threshold)
      {
       OutputPixelType outValue = itk::NumericTraits<OutputPixelType>(value);

       outputPtr->SetPoint(pointId,index);    
       outputPtr->SetPointData(pointId, outValue );

       pointId++;    
      }
    ++inIt;
    }
}

#if 0

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutputPointSet>
void 
ThreholdImageToPointSetFilter<TInputImage, TOutputPointSet>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Threshold : " << m_Threshold << std::endl;
}

#endif
} // end namespace otb


#endif
