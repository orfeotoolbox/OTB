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

#include "otbImageToSIFTKeyPointSetFilter.h"

namespace otb
{

  /**
   * Constructor
   */
  template <class TInputImage, class TOutputPointSet>
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::ImageToSIFTKeyPointSetFilter()
  {
    m_OctavesNumber = 1;
    m_ScalesNumber = 4;
    m_ExpandFactors = 2;
    m_ShrinkFactors = 2;
    m_Sigma0 = 1.6;
    m_Sigmak = vcl_pow((double)2, (double)((double)1/(double)m_ScalesNumber));
    m_ValidatedKeyPoints = 0;
    m_ThresholdDoG = 0.03;
    
    m_ExpandFilter = ExpandFilterType::New();
  }
  
  template <class TInputImage, class TOutputPointSet>
  void
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::GenerateData()
  {
    // First, subsample the input image
    InitializeInputImage();

    InputImagePointerType input = m_ExpandFilter->GetOutput(); 
    
    // for each octave, compute the difference of gaussian
    unsigned int lOctave = 0;
    for (lOctave = 0; lOctave != m_OctavesNumber; lOctave++)
      {
	ComputeDifferenceOfGaussian(input);
	DetectKeyPoint(lOctave);
	
	m_ShrinkFilter = ShrinkFilterType::New();
	// temporary get the second gaussian for subsample and
	/// repeat the process
	m_ShrinkFilter->SetInput(m_GaussianList->Back());
	m_ShrinkFilter->SetShrinkFactors(m_ShrinkFactors);
	m_ShrinkFilter->Update();
	input = m_ShrinkFilter->GetOutput();
	
	std::cout << "Number key points per octave : " \
		  << m_ValidatedKeyPoints << std::endl;
	std::cout << "Resample image factor : " \
		  << m_ShrinkFactors << std::endl;
      }
  }
  
  /**
   * Initialize the input image
   */
  template <class TInputImage, class TOutputPointSet>
  void
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::InitializeInputImage()
  {
    m_ExpandFilter->SetInput( this->GetInput(0));
    m_ExpandFilter->SetExpandFactors(m_ExpandFactors);
    m_ExpandFilter->Update();
  }
  
  /**
   * Compute the difference of gaussian
   */
  template <class TInputImage, class TOutputPointSet>
  void
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::ComputeDifferenceOfGaussian(InputImagePointerType input)
  {
    unsigned int lScale = 0;
        
    m_GaussianList = ImageListType::New();
    m_DoGList = ImageListType::New();
    
    double sigman = m_Sigma0;
    
    for (lScale = 0; lScale != m_ScalesNumber; lScale++)
      {
	sigman = sigman*m_Sigmak;
	m_XGaussianFilter = GaussianFilterType::New();
	m_YGaussianFilter = GaussianFilterType::New();
	
	m_XGaussianFilter->SetSigma(sigman);
	m_XGaussianFilter->SetDirection(0);
	m_XGaussianFilter->SetInput(input);
	
	m_YGaussianFilter->SetSigma(sigman);
	m_YGaussianFilter->SetDirection(1);
	m_YGaussianFilter->SetInput(m_XGaussianFilter->GetOutput());
	
	m_YGaussianFilter->Update();
	
	m_GaussianList->PushBack(m_YGaussianFilter->GetOutput());
	
	std::cout << "Sigma scale " << sigman << std::endl;
      }
    
    typename ImageListType::Iterator lIterGaussianOutput = m_GaussianList->Begin();
    while (lIterGaussianOutput != m_GaussianList->End()-1)
      {	
  	m_SubtractFilter = SubtractFilterType::New();
  	m_SubtractFilter->SetInput1(lIterGaussianOutput.Get());
  	m_SubtractFilter->SetInput2((lIterGaussianOutput+1).Get());
 	m_SubtractFilter->Update();
	m_DoGList->PushBack(m_SubtractFilter->GetOutput());
	
  	++lIterGaussianOutput;
      }
    std::cout <<"Number of Gaussian images "<<m_GaussianList->Size() << std::endl;;
    std::cout <<"Number of DoG "<<m_DoGList->Size() << std::endl;
  }
  
  /**
   * Localize key point
   */
  template <class TInputImage, class TOutputPointSet>
  void
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::DetectKeyPoint( const unsigned int octave )
  {
    typename ImageListType::Iterator lIterDoG = m_DoGList->Begin()+1;
    
    // 3 min DoG
    while ( (lIterDoG+1) != m_DoGList->End())
      {
	typename InputImageType::SizeType lRadius;
	lRadius.Fill(1);
	typename ImageListType::Iterator lPrev = lIterDoG-1;
	typename ImageListType::Iterator lNext = lIterDoG+1;
	
 	NeighborhoodIteratorType lIterCurrent(lRadius,
 					      lIterDoG.Get(),
 					      lIterDoG.Get()->GetLargestPossibleRegion());
  	NeighborhoodIteratorType lIterLowerAdjacent(lRadius,
  						    lPrev.Get(),
  						    lPrev.Get()->GetLargestPossibleRegion());
  	NeighborhoodIteratorType lIterUpperAdjacent(lRadius,
  						    lNext.Get(),
 						    lNext.Get()->GetLargestPossibleRegion());

	OutputPointSetPointerType pointSet = this->GetOutput();
	
 	while ( !lIterCurrent.IsAtEnd() &&
		!lIterLowerAdjacent.IsAtEnd() &&
		!lIterUpperAdjacent.IsAtEnd() )
 	  {
 	    // check local min/max
  	    if (IsLocalExtremum(lIterCurrent,
				lIterLowerAdjacent,
				lIterUpperAdjacent))
	      {
		// add key point
		OutputPointType keyPoint;
		lIterDoG.Get()->TransformIndexToPhysicalPoint(lIterCurrent.GetIndex(),
							      keyPoint);
		pointSet->SetPoint(m_ValidatedKeyPoints, keyPoint);
		OutputPixelType data;
		data.SetSize(2);
		data.SetElement(0,octave);
		//! \todo compute scale
		unsigned int scale = 0;
		data.SetElement(1,scale);
		pointSet->SetPointData(m_ValidatedKeyPoints, data);
		++m_ValidatedKeyPoints;
	      }
	    
 	    ++lIterCurrent;
  	    ++lIterLowerAdjacent;
  	    ++lIterUpperAdjacent;
 	  }

	++lIterDoG;
      }
  }
  
  /**
   * Check local extremum for 26 neighbors
   */
  template <class TInputImage, class TOutputPointSet>
  bool
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::IsLocalExtremum( const NeighborhoodIteratorType& currentScale,
		     const NeighborhoodIteratorType& previousScale,
		     const NeighborhoodIteratorType& nextScale) const
  {
    OffsetType offset1 = {{-1,-1}};
    OffsetType offset2 = {{-1, 0}};
    OffsetType offset3 = {{-1, 1}};
    OffsetType offset4 = {{ 0,-1}};
    OffsetType offset6 = {{ 0, 1}};
    OffsetType offset7 = {{ 1,-1}};
    OffsetType offset8 = {{ 1, 0}};
    OffsetType offset9 = {{ 1, 1}};
    
    std::vector<OffsetType> lOffsets;
    lOffsets.push_back(offset1);
    lOffsets.push_back(offset2);
    lOffsets.push_back(offset3);
    lOffsets.push_back(offset4);
    lOffsets.push_back(offset6);
    lOffsets.push_back(offset7);
    lOffsets.push_back(offset8);
    lOffsets.push_back(offset9);
    
    typename std::vector<OffsetType>::iterator lIterOffset = lOffsets.begin();
    
    bool isMin = currentScale.GetCenterPixel() < currentScale.GetPixel(offset1);
    bool isMax = currentScale.GetCenterPixel() > currentScale.GetPixel(offset1);
    bool isExtremum = isMin || isMax;
    while (lIterOffset != lOffsets.end() && isExtremum)
      {
	if (isMin)
	  {
	    isExtremum =
	      currentScale.GetCenterPixel() < currentScale.GetPixel(*lIterOffset) &&
 	      currentScale.GetCenterPixel() < previousScale.GetPixel(*lIterOffset) &&
	      currentScale.GetCenterPixel() < nextScale.GetPixel(*lIterOffset);
	  }
	else if (isMax)
	  {
	    isExtremum =
	      currentScale.GetCenterPixel() > currentScale.GetPixel(*lIterOffset) &&
 	      currentScale.GetCenterPixel() > previousScale.GetPixel(*lIterOffset) &&
 	      currentScale.GetCenterPixel() > nextScale.GetPixel(*lIterOffset);
	  }
	else
	  {
	    isExtremum = false;
	  }
	lIterOffset++;
      }
    if (isExtremum && isMin)
      {
 	isExtremum =
 	  currentScale.GetCenterPixel() < previousScale.GetCenterPixel() &&
 	  currentScale.GetCenterPixel() < nextScale.GetCenterPixel();
      }
    else if (isExtremum && isMax)
      {
 	isExtremum =
 	  currentScale.GetCenterPixel() > previousScale.GetCenterPixel() &&
 	  currentScale.GetCenterPixel() > nextScale.GetCenterPixel();
      }
    return isExtremum;
  }
  
  /**
   * PrintSelf Method
   */
  template <class TInputImage, class TOutputPointSet>
  void
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
    os<<indent<<"Number of octaves: "<<m_OctavesNumber<<std::endl;
    os<<indent<<"Number of scales: "<<m_ScalesNumber<<std::endl;
  }
  
} // End namespace otb
