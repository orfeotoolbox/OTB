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

#include "itkMatrix.h"

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

	// Get the last gaussian for subsample and
	// repeat the process
	m_ShrinkFilter = ShrinkFilterType::New();
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
    
    for (lScale = 0; lScale != m_ScalesNumber+1; lScale++)
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
	// Compute max of DoG
	MinimumMaximumCalculatorPointerType lMaximumCalculator = MinimumMaximumCalculatorType::New();
	lMaximumCalculator->SetImage(lIterDoG.Get());
	lMaximumCalculator->Compute();
	
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
	    VectorPointType lTranslation;
	    
 	    // check local min/max
  	    if (IsLocalExtremum(lIterCurrent,
				lIterLowerAdjacent,
				lIterUpperAdjacent) &&
		
		RefineLocationKeyPoint(lIterCurrent,
				       lIterLowerAdjacent,
				       lIterUpperAdjacent,
				       lMaximumCalculator->GetMaximum(),
				       lTranslation) )
	      {
		// add key point
		OutputPointType keyPoint;
		lIterDoG.Get()->TransformIndexToPhysicalPoint(lIterCurrent.GetIndex(),
							      keyPoint);
		keyPoint[0] = keyPoint[0]+lIterDoG.Get()->GetSpacing()[0]*lTranslation[0];
		keyPoint[1] = keyPoint[1]+lIterDoG.Get()->GetSpacing()[1]*lTranslation[1];
		
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
   * Refine location key point
   */
  template <class TInputImage, class TOutputPointSet>
  bool
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::RefineLocationKeyPoint( const NeighborhoodIteratorType& currentScale,
			    const NeighborhoodIteratorType& previousScale,
			    const NeighborhoodIteratorType& nextScale,
			    const PixelType& maximumDoG,
			    VectorPointType& solution)
  {
    bool discard = false;
    
    OffsetType o1 = {{-1,0}};
    OffsetType o2 = {{1,0}};
    OffsetType o3 = {{0,-1}};
    OffsetType o4 = {{0,1}};
    OffsetType o5 = {{-1,-1}};
    OffsetType o6 = {{1,1}};
    OffsetType o7 = {{-1,1}};
    OffsetType o8 = {{1,-1}};
    
    PixelType dx = 0.5*(currentScale.GetPixel(o1)
			-currentScale.GetPixel(o2) );
    
    PixelType dy = 0.5*(currentScale.GetPixel(o3)
			-currentScale.GetPixel(o4) );
    
    PixelType ds = 0.5*(previousScale.GetCenterPixel()-
			nextScale.GetCenterPixel());
    
    PixelType dxx = currentScale.GetPixel(o1)
      -2*currentScale.GetCenterPixel()
      +currentScale.GetPixel(o2);
    
    PixelType dyy = currentScale.GetPixel(o3)
      -2*currentScale.GetCenterPixel()
      +currentScale.GetPixel(o4);
    
    PixelType dss = previousScale.GetCenterPixel()
      -2*currentScale.GetCenterPixel()
      +nextScale.GetCenterPixel();
    
    PixelType dxy = 0.25*(currentScale.GetPixel(o5)
			  +currentScale.GetPixel(o6)
			  -currentScale.GetPixel(o7)
			  -currentScale.GetPixel(o8) );
    
    PixelType dxs = 0.25*(previousScale.GetPixel(o1)
			  +nextScale.GetPixel(o2)
			  -previousScale.GetPixel(o2)
			  -nextScale.GetPixel(o1) );
    
    PixelType dys = 0.25*(previousScale.GetPixel(o3)
			  +nextScale.GetPixel(o4)
			  -previousScale.GetPixel(o4)
			  -nextScale.GetPixel(o3) );
    
    itk::Matrix<PixelType,3 , 3> lMatrixSecondOrder;
    lMatrixSecondOrder[0][0] = dxx;
    lMatrixSecondOrder[0][1] = dxy;
    lMatrixSecondOrder[0][2] = dxs;
    
    lMatrixSecondOrder[1][0] = dxy;
    lMatrixSecondOrder[1][1] = dyy;
    lMatrixSecondOrder[1][2] = dys;

    lMatrixSecondOrder[2][0] = dxs;
    lMatrixSecondOrder[2][1] = dys;
    lMatrixSecondOrder[2][2] = dss;
    
    itk::Vector<PixelType, 3> lVectorFirstOrder;
    lVectorFirstOrder[0] = dx;
    lVectorFirstOrder[1] = dy;
    lVectorFirstOrder[2] = ds;
    
    // Solve system
    itk::Matrix<PixelType,3,3> lMatrixInv(lMatrixSecondOrder.GetInverse());
    solution = lMatrixInv*lVectorFirstOrder;
    
    // if translation location is larger than 0.5 in any dimension
    // compute with anotherlocation
    OffsetType lOffset={{0,0}};
    OffsetType lOffsetZero={{0,0}};
    
    if (solution[0]>0.5)
      {
	lOffset[0]= 1;
      }
    else if (solution[0]<0.5)
      {
	lOffset[0]= -1;
      }

    if (solution[1]>0.5)
      {
	lOffset[1]= 1;
      }
    else if (solution[1]<0.5)
      {
	lOffset[1]= -1;
      }
    
    if (lOffset != lOffsetZero)
      {
	NeighborhoodIteratorType iterCurrent(currentScale);
	iterCurrent.SetLocation(currentScale.GetIndex()+lOffset);
	
	NeighborhoodIteratorType iterPrev(previousScale);
	iterPrev.SetLocation(previousScale.GetIndex()+lOffset);
	
	NeighborhoodIteratorType iterNext(nextScale);
	iterNext.SetLocation(nextScale.GetIndex()+lOffset);
	
	//lSolution = 
	// discard = RefineLocationKeyPoint(iterCurrent,
	// 					 iterPrev,
	// 					 iterNext, solution);
      }
    
    // Compute interpolated value DoG for lSolution
    PixelType lDoGInterpolated = currentScale.GetCenterPixel()+
      0.5*(dx*solution[0]+
	   dy*solution[1]+
	   ds*solution[2]);
    
    // DoG threshold
    discard = lDoGInterpolated < m_ThresholdDoG*maximumDoG;
    
    return discard;
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
