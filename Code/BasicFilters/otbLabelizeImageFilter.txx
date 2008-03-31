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

    m_DeltaLowerThreshold = itk::NumericTraits<InputPixelType>::ZeroValue();
    m_DeltaUpperThreshold = itk::NumericTraits<InputPixelType>::ZeroValue();
  }
  
  /**
   * Generate data
   */
  template <class TInputImage, class TOutputImage>
  void
  LabelizeImageFilter<TInputImage,TOutputImage>
  ::GenerateData()
  { 
    ThresholdFilterPointerType filterThreshold = ThresholdFilterType::New();

    filterThreshold->SetInput(0, this->GetInput());
    
    filterThreshold->SetLowerThreshold( m_LowerThreshold );
    filterThreshold->SetUpperThreshold( m_UpperThreshold );
    
    m_PointSet=filterThreshold->GetOutput();
    
    // Compute points set
    filterThreshold->Update();

    // Create empty image to store results computing
    MultiplyFilterPointerType multiplyFilter = MultiplyFilterType::New();
    
    multiplyFilter->SetCoef(0.0);
    multiplyFilter->SetInput(this->GetInput());
    multiplyFilter->Update();
    typename OutputImageType::Pointer outputImage =  multiplyFilter->GetOutput();
    
    // Iterate Point set
    typedef typename PointSetType::PointsContainer ContainerType;
    ContainerType* pointsContainer = m_PointSet->GetPoints();
    typedef typename ContainerType::Iterator IteratorType;
    IteratorType itList = pointsContainer->Begin();
    
    typedef typename PointSetType::PointDataContainer DataContainerType;
    DataContainerType* dataContainer = m_PointSet->GetPointData();
    typedef typename DataContainerType::Iterator DataIteratorType;
    DataIteratorType itData = dataContainer->Begin();
    
    int replaceValue = 5;
    
    while( itList != pointsContainer->End()
	   && itData != dataContainer->End())
      {
	typename PointSetType::PointType pCoordinate = (itList.Value());
	typename InputImageType::IndexType index;
      
	index[0] = static_cast <int> (pCoordinate[0]);
	index[1] = static_cast <int> (pCoordinate[1]);
	
	if (outputImage->GetPixel(index) == 0 )
	  {
	    InputPixelType threshold = itData.Value();
	    ConnectedFilterPointerType connectedThreshold = ConnectedFilterType::New();
	    connectedThreshold->SetInput( this->GetInput() );
	    connectedThreshold->SetLower( threshold-m_DeltaLowerThreshold );
	    connectedThreshold->SetUpper( threshold+m_DeltaUpperThreshold );
	    
	    connectedThreshold->SetReplaceValue(replaceValue);
	    connectedThreshold->SetSeed(index);

	    AddImageFilterPointerType addImage = AddImageFilterType::New();
	    addImage->SetInput1(outputImage);
	    addImage->SetInput2(connectedThreshold->GetOutput());
	    addImage->Update();
	    outputImage = addImage->GetOutput();
	    
	    //++count;
	    
	    replaceValue++;
	  }
	++itList;
	++itData;
      }
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
