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

#include "otbLabelizeImageFilter.h"
#include "itkNumericTraits.h"

namespace otb
{
  
  /**
   * Constructor
   */
  template <class TInputImage, class TOutputImage>
  LabelizeImageFilter<TInputImage, TOutputImage>
  ::LabelizeImageFilter()
  {
    m_LowerThreshold = itk::NumericTraits<InputPixelType>::NonpositiveMin();
    m_UpperThreshold = itk::NumericTraits<InputPixelType>::max();

    m_LowerThresholdDelta = itk::NumericTraits<InputPixelType>::ZeroValue();
    m_UpperThresholdDelta = itk::NumericTraits<InputPixelType>::ZeroValue();

    m_MultiplyFilter = MultiplyFilterType::New();
    m_MultiplyFilter->SetCoef(0.0);
    
    m_ThresholdPointSetFilter = ThresholdFilterType::New();
  }
  
  /**
   * Generate data
   */
  template <class TInputImage, class TOutputImage>
  void
  LabelizeImageFilter<TInputImage,TOutputImage>
  ::GenerateData()
  {
    // create an empty image to store results computing
    m_MultiplyFilter->SetInput(this->GetInput());
    m_MultiplyFilter->Update();
    
    // Compute points set
    m_ThresholdPointSetFilter->SetInput(0, this->GetInput());
    m_ThresholdPointSetFilter->SetLowerThreshold( m_LowerThreshold );
    m_ThresholdPointSetFilter->SetUpperThreshold( m_UpperThreshold );
    m_ThresholdPointSetFilter->Update();
    m_PointSet = m_ThresholdPointSetFilter->GetOutput();
    
    // Iterate Point set
    typedef typename PointSetType::PointsContainer ContainerType;
    ContainerType* pointsContainer = m_PointSet->GetPoints();
    typedef typename ContainerType::Iterator IteratorType;
    IteratorType itList = pointsContainer->Begin();
    
    typename OutputImageType::Pointer outputImage = m_MultiplyFilter->GetOutput();
    int replaceValue = 5;
    
    while( itList != pointsContainer->End() )
      {
	typename PointSetType::PointType pCoordinate = (itList.Value());
	typename InputImageType::IndexType index;
      
 	index[0] = static_cast <int> (pCoordinate[0]);
 	index[1] = static_cast <int> (pCoordinate[1]);
 	if (outputImage->GetPixel(index) ==
	    itk::NumericTraits<InputPixelType>::ZeroValue() )
 	  {
	    InputPixelType threshold = this->GetInput()->GetPixel(index);
	    
	    ConnectedFilterPointerType connectedThreshold = ConnectedFilterType::New();
	    connectedThreshold->SetInput( this->GetInput() );
	    connectedThreshold->SetLower( threshold-m_LowerThresholdDelta );
	    connectedThreshold->SetUpper( threshold+m_UpperThresholdDelta );
	    
	    connectedThreshold->SetReplaceValue(replaceValue);
	    connectedThreshold->SetSeed(index);
	    
	    AddImageFilterPointerType addImage = AddImageFilterType::New();
	    addImage->SetInput1(outputImage);
	    addImage->SetInput2(connectedThreshold->GetOutput());
	    addImage->Update();
	    outputImage = addImage->GetOutput();
	    
	    replaceValue++;
 	  }
 	++itList;
      }
    
    this->GraftOutput(outputImage);

  }
  
  /**
   * PrintSelf Method
   */
  template <class TInputImage, class TOutputImage>
  void
  LabelizeImageFilter<TInputImage, TOutputImage>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }
  
} // End namespace otb
