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
#include "itkProcessObject.h"

namespace otb
{
  
  template <class TInputImage, class TOutputPointSet>
  const double
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::m_HistogramGaussianWeights[73] = { 
    2.3771112282795414e-07, 3.8860734758633732e-07, 6.2655544995978937e-07, 9.9631120821413786e-07, 1.5624909838697011e-06, 2.4167238265599128e-06, 3.6865788528530121e-06, 
    5.5463469229192623e-06, 8.2295774080263437e-06, 1.2043009749602365e-05, 1.738119136656513e-05,  2.4740646513897326e-05, 3.4731980398846277e-05, 4.808781565748272e-05, 
    6.5664032975164266e-05, 8.8431512984476723e-05, 0.00011745555408931643, 0.00015386047198026335, 0.00019877765486783745, 0.00025327659834301937, 0.00031828015928190065, 
    0.00039446735551235698, 0.00048216931692246382, 0.00058126620279441276, 0.00069109471776775144, 0.00081037694122312908, 0.00093718121775182789, 0.0010689246133776746, 
    0.0012024238404411182,  0.0013339976954896103,  0.0014596192424447215,  0.0015751106965100009,  0.0016763688464699555,  0.0017596045720966803,  0.0018215772013714365, 
    0.0018598035923515156,  0.0018727231637146351,  0.0018598035923515156,  0.0018215772013714365,  0.0017596045720966803,  0.0016763688464699555,  0.0015751106965100009, 
    0.0014596192424447215,  0.0013339976954896103,  0.0012024238404411182,  0.0010689246133776746,  0.00093718121775182789, 0.00081037694122312908, 0.00069109471776775144, 
    0.00058126620279441276, 0.00048216931692246382, 0.00039446735551235698, 0.00031828015928190065, 0.00025327659834301937, 0.00019877765486783745, 0.00015386047198026335, 
    0.00011745555408931643, 8.8431512984476723e-05, 6.5664032975164266e-05, 4.808781565748272e-05,  3.4731980398846277e-05, 2.4740646513897326e-05, 1.738119136656513e-05, 
    1.2043009749602365e-05, 8.2295774080263437e-06, 5.5463469229192623e-06, 3.6865788528530121e-06, 2.4167238265599128e-06, 1.5624909838697011e-06, 9.9631120821413786e-07, 
    6.2655544995978937e-07, 3.8860734758633732e-07, 2.3771112282795414e-07};

  /**
   * Constructor
   */
  template <class TInputImage, class TOutputPointSet>
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::ImageToSIFTKeyPointSetFilter()
  {
    m_OctavesNumber = 1;
    m_ScalesNumber = 3;
    m_ExpandFactors = 2;
    m_ShrinkFactors = 2;
    m_Sigma0 = 1.6;
    m_Sigmak = 0;
    
    m_DoGThreshold = 0.03;
    m_EdgeThreshold = 10;
    m_RatioEdgeThreshold = 0;
    
    m_ValidatedKeyPoints = 0;
    m_DifferentSamplePoints = 0;
    m_DiscardedKeyPoints = 0;
    m_ChangeSamplePointsMax = 5;
    
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
    typename InputImageType::PointType point;
    typename InputImageType::IndexType  index;
    index[0] = 0;
    index[1] = 0;
    
    m_ExpandFilter->GetOutput()->TransformIndexToPhysicalPoint(index, point);
    
    std::cout << "Input Spacing: " << m_ExpandFilter->GetOutput()->GetSpacing() << std::endl;
    std::cout << "Input Origin: " << point << std::endl;
    
    // for each octave, compute the difference of gaussian
    unsigned int lOctave = 0;
    m_Sigmak = vcl_pow((double)2, (double)((double)1/(double)(m_ScalesNumber)));
    m_RatioEdgeThreshold = (m_EdgeThreshold+1)*(m_EdgeThreshold+1)/m_EdgeThreshold;
    
    for (lOctave = 0; lOctave != m_OctavesNumber; lOctave++)
      {
	m_DifferentSamplePoints = 0;
	m_DiscardedKeyPoints = 0;
	
	typename InputImageType::PointType origin0 = input->GetOrigin();
	  
	ComputeDifferenceOfGaussian(input);
	DetectKeyPoint(lOctave);
	
	// Get the last gaussian for subsample and
	// repeat the process
	m_ShrinkFilter = ShrinkFilterType::New();
	m_ShrinkFilter->SetInput(m_GaussianList->Back());
	m_ShrinkFilter->SetShrinkFactors(m_ShrinkFactors);
	m_ShrinkFilter->Update();
	
	input = m_ShrinkFilter->GetOutput();
	
	typename InputImageType::PointType origin1;
	typename InputImageType::SpacingType spacing = input->GetSpacing();
	
	origin1[0] = origin0[0] + spacing[0]*0.25;
	origin1[1] = origin0[1] + spacing[1]*0.25;
	
	input->SetOrigin(origin1);
	
	std::cout << "Number key points per octave : " \
		  << m_ValidatedKeyPoints << std::endl;
	std::cout << "Number different sample key points per octave : " \
		  << m_DifferentSamplePoints << std::endl;
	std::cout << "Number discarded key points per octave : " \
		  << m_DiscardedKeyPoints << std::endl;
	
	std::cout << "Resample image factor : " \
		  << m_ShrinkFactors << std::endl;
	
	typename InputImageType::PointType point;
	typename InputImageType::IndexType  index;
	index[0] = 0;
	index[1] = 0;
	
	m_ShrinkFilter->GetOutput()->TransformIndexToPhysicalPoint(index, point);
	
	std::cout << "Spacing: " << m_ShrinkFilter->GetOutput()->GetSpacing() << std::endl;
	std::cout << "Input Origin: " << point << std::endl;

	
      }
    std::cout << "Total number key points : " \
	      << this->GetOutput()->GetNumberOfPoints() << std::endl;
    
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

    typename InputImageType::PointType origin0 = this->GetInput(0)->GetOrigin();
    typename InputImageType::PointType origin1;
    typename InputImageType::SpacingType spacing = m_ExpandFilter->GetOutput()->GetSpacing();
    
    origin1[0] = origin0[0]-spacing[0]*0.5;
    origin1[1] = origin0[1]-spacing[1]*0.5;
    
    m_ExpandFilter->GetOutput()->SetOrigin(origin1);
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
    
    m_MagnitudeList = ImageListType::New();
    m_OrientationList = ImageListType::New();
    m_GaussianWeightOrientationList = ImageListType::New();
    
    double sigman = 1/input->GetSpacing()[0]*m_Sigma0*vcl_sqrt((double)(1.-1./(m_Sigmak*m_Sigmak)));
    
    for (lScale = 0; lScale != m_ScalesNumber+2; lScale++)
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
	
	m_GradientFilter = GradientFilterType::New();
	m_MagnitudeFilter = MagnitudeFilterType::New();
	m_OrientationFilter = OrientationFilterType::New();
	m_XGaussianFilter3 = GaussianFilterType::New();
	m_YGaussianFilter3 = GaussianFilterType::New();
	
	m_GradientFilter->SetInput(m_YGaussianFilter->GetOutput());
	m_MagnitudeFilter->SetInput(m_GradientFilter->GetOutput());
	m_OrientationFilter->SetInput(m_GradientFilter->GetOutput());

	m_XGaussianFilter3->SetInput(m_OrientationFilter->GetOutput());
	m_XGaussianFilter3->SetDirection(0);
	m_XGaussianFilter3->SetSigma(3*sigman);
	
	m_YGaussianFilter3->SetInput(m_XGaussianFilter3->GetOutput());
	m_YGaussianFilter3->SetDirection(1);
	m_YGaussianFilter3->SetSigma(3*sigman);
	
	m_MagnitudeFilter->Update();
	m_YGaussianFilter3->Update();
	
	m_MagnitudeList->PushBack(m_MagnitudeFilter->GetOutput());
	m_OrientationList->PushBack(m_OrientationFilter->GetOutput());
	m_GaussianWeightOrientationList->PushBack(m_YGaussianFilter3->GetOutput());
	
	if (lScale>0)
	  {
	    m_SubtractFilter = SubtractFilterType::New();
	    m_SubtractFilter->SetInput1((m_GaussianList->End()-1).Get());
	    m_SubtractFilter->SetInput2((m_GaussianList->End()-2).Get());
	    m_SubtractFilter->Update();
	    m_DoGList->PushBack(m_SubtractFilter->GetOutput());
	  }
	std::cout << "Scale " << sigman << std::endl;
      }
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
    if (m_DoGList->Size() >=3 )
      {
	typename ImageListType::Iterator lIterDoG = m_DoGList->Begin()+1;
	unsigned int lScale = 0;
	OutputPointSetPointerType  outputPointSet = this->GetOutput();
	typename InputImageType::SizeType size = lIterDoG.Get()->GetLargestPossibleRegion().GetSize();
	typename InputImageType::SpacingType spacing = lIterDoG.Get()->GetSpacing();
	
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
	    
	    while ( !lIterCurrent.IsAtEnd() &&
		    !lIterLowerAdjacent.IsAtEnd() &&
		    !lIterUpperAdjacent.IsAtEnd() )
	      { 
		// check local min/max
		if (IsLocalExtremum(lIterCurrent,
				    lIterLowerAdjacent,
				    lIterUpperAdjacent) )
		  {
		    VectorPointType lTranslation(PixelType(0));
		    OffsetType lOffsetZero = {{0,0}};
		    
		    unsigned int lChangeSamplePoints = 0;
		    NeighborhoodIteratorType neighborCurrentScale(lIterCurrent);
		    NeighborhoodIteratorType neighborPreviousScale(lIterLowerAdjacent);
		    NeighborhoodIteratorType neighborNextScale(lIterUpperAdjacent);
		    
		    bool accepted = false;
		    bool changed = true;
		    while (lChangeSamplePoints < m_ChangeSamplePointsMax &&
			   changed )
		      {
			accepted = RefineLocationKeyPoint(neighborCurrentScale,
							  neighborPreviousScale,
							  neighborNextScale,
							  lMaximumCalculator->GetMaximum(),
							  lTranslation);
			
			OffsetType lTranslateOffset = {{0,0}};
			
			lTranslateOffset[0] += static_cast<int>(lTranslation[0]>0.5);
			lTranslateOffset[0] += -static_cast<int>(lTranslation[0]<-0.5);
			
			lTranslateOffset[1] += static_cast<int>(lTranslation[1]>0.5);
			lTranslateOffset[1] += -static_cast<int>(lTranslation[1]<-0.5);

			neighborCurrentScale+=lTranslateOffset;
			neighborPreviousScale+=lTranslateOffset;
			neighborNextScale+=lTranslateOffset;
			
			changed = lTranslateOffset != lOffsetZero &&
			  neighborCurrentScale.InBounds();
			
			lChangeSamplePoints++;
		      }
		    if (changed)
		      {
			m_DifferentSamplePoints++;
		      }
		    
		    // add key point
		    if (accepted)
		      {
			PixelType lMagnitude = 0;
			PixelType lOrientation = 0;
			
			ComputeKeyPointDescriptor(neighborCurrentScale,
						  lScale,
						  lTranslation[2],
						  lMagnitude,
						  lOrientation);
			OutputPointType keyPoint;
			
			lIterDoG.Get()->TransformIndexToPhysicalPoint(neighborCurrentScale.GetIndex(),
								      keyPoint);
			keyPoint[0] += spacing[0]*lTranslation[0];
			keyPoint[1] += spacing[1]*lTranslation[1];
			
			outputPointSet->SetPoint(m_ValidatedKeyPoints, keyPoint);
			
			OutputPixelType data;
			data.SetSize(3);
			data.SetElement(0,lScale+lTranslation[2]);
			data.SetElement(1,lMagnitude);
			data.SetElement(2,lOrientation);
			outputPointSet->SetPointData(m_ValidatedKeyPoints, data);
			
			m_ValidatedKeyPoints++;
		      }
		  }
		
		++lIterCurrent;
		++lIterLowerAdjacent;
		++lIterUpperAdjacent;
	      }
	    
	    ++lIterDoG;
	    lScale++;
	  }
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
    bool accepted = true;
    solution = VectorPointType(PixelType(0));
    
    OffsetType o1 = {{-1,0}};
    OffsetType o2 = {{1,0}};
    OffsetType o3 = {{0,-1}};
    OffsetType o4 = {{0,1}};
    OffsetType o5 = {{-1,-1}};
    OffsetType o6 = {{1,1}};
    OffsetType o7 = {{-1,1}};
    OffsetType o8 = {{1,-1}};
    
    PixelType dx = 0.5*(currentScale.GetPixel(o2)
			-currentScale.GetPixel(o1) );
    
    PixelType dy = 0.5*(currentScale.GetPixel(o4)
			-currentScale.GetPixel(o3) );
    
    PixelType ds = 0.5*(nextScale.GetCenterPixel()-
			previousScale.GetCenterPixel());
    
    PixelType dxx = currentScale.GetPixel(o1)
      -2*currentScale.GetCenterPixel()
      +currentScale.GetPixel(o2);
    
    PixelType dyy = currentScale.GetPixel(o3)
      -2*currentScale.GetCenterPixel()
      +currentScale.GetPixel(o4);
    
    PixelType dss = previousScale.GetCenterPixel()
      -2*currentScale.GetCenterPixel()
      +nextScale.GetCenterPixel();
    
    PixelType dxy = 0.25*(currentScale.GetPixel(o6)
			  +currentScale.GetPixel(o5)
			  -currentScale.GetPixel(o7)
			  -currentScale.GetPixel(o8) );
    
    PixelType dxs = 0.25*(nextScale.GetPixel(o2)
			  +previousScale.GetPixel(o1)
			  -nextScale.GetPixel(o1)
			  -previousScale.GetPixel(o2) );
    
    PixelType dys = 0.25*(nextScale.GetPixel(o4)
			  +previousScale.GetPixel(o3)
			  -nextScale.GetPixel(o3)
			  -previousScale.GetPixel(o4) );
    
    itk::Matrix<double,3 , 3> lMatrixSecondOrder;
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
    
    PixelType lDoGInterpolated = currentScale.GetCenterPixel();
    
    // Compute matrice determinant
    double det = dxx*(dyy*dss-dys*dys) -dxy*(dxy*dss-dxs*dys)+dxs*(dxy*dys-dxs*dyy);
    if (det>=1e-10f)
      {
	double invdet = 1/det;
	double sx = -invdet*(dx*(dyy*dss-dys*dys)+dy*(dxs*dys-dxy*dss)+ds*(dxy*dys-dyy*dxs));
	double sy = -invdet*(dx*(dys*dxs-dss*dxy)+dy*(dxx*dss-dxs*dxs)+ds*(dxs*dxy-dxx*dys));
	double ss = -invdet*(dx*(dxy*dys-dxs*dyy)+dy*(dxy*dxs-dxx*dys)+ds*(dxx*dyy-dxy*dxy));

	// Solve system
	itk::Matrix<double,3,3> lMatrixInv(lMatrixSecondOrder.GetInverse());
	solution = lMatrixInv*lVectorFirstOrder;
	solution*=-1;
	
	// Compute interpolated value DoG for lSolution
	lDoGInterpolated += 0.5*(dx*solution[0]+
				 dy*solution[1]+
				 ds*solution[2]);
      }
    
    // DoG threshold
    // Eliminating edge response
    PixelType lRatioHessian = 0;
    if ((dxx*dyy -dxy*dxy)>0)
      {
	lRatioHessian = (dxx+dyy)*(dxx+dyy)/(dxx*dyy -dxy*dxy);
      }
    accepted =
      (lDoGInterpolated >= m_DoGThreshold*255 ||
       lDoGInterpolated <= -m_DoGThreshold*255)
      && lRatioHessian < m_RatioEdgeThreshold;
    
    if (!accepted)
      {
	m_DiscardedKeyPoints++;
      }
    
    return accepted;
  }
  
  /**
   * Compute key point orientation
   */
  template <class TInputImage, class TOutputPointSet>
  void
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::ComputeKeyPointDescriptor( const NeighborhoodIteratorType& currentScale,
			       const unsigned int scale,
			       const PixelType translation,
			       PixelType& magnitude,
			       PixelType& orientation)
  {
    // compute histogram
    std::vector<double> lHistogram(36,0.);
    //unsigned int scale = static_cast<unsigned int>(vcl_floor(closestScale+0.5));

    typename InputImageType::SizeType lRadius;
    lRadius.Fill(1);
    
    NeighborhoodIteratorType neighborsOrientation(lRadius,
						  m_OrientationList->GetNthElement(scale),
						  m_OrientationList->GetNthElement(scale)->GetLargestPossibleRegion());
    
    NeighborhoodIteratorType neighborsMagnitude(lRadius,
						m_MagnitudeList->GetNthElement(scale),
						m_MagnitudeList->GetNthElement(scale)->GetLargestPossibleRegion());
    
    NeighborhoodIteratorType neighborsGaussianWeight(lRadius,
						     m_GaussianWeightOrientationList->GetNthElement(scale),
						     m_GaussianWeightOrientationList->GetNthElement(scale)->GetLargestPossibleRegion());
    
    neighborsOrientation.SetLocation(currentScale.GetIndex());
    neighborsMagnitude.SetLocation(currentScale.GetIndex());
    neighborsGaussianWeight.SetLocation(currentScale.GetIndex());

    typename NeighborhoodIteratorType::NeighborhoodType lNeighborhoodOrientation = neighborsOrientation.GetNeighborhood();
    typename NeighborhoodIteratorType::NeighborhoodType lNeighborhoodGaussianWeight = neighborsGaussianWeight.GetNeighborhood();
    typename NeighborhoodIteratorType::NeighborhoodType lNeighborhoodMagnitude = neighborsMagnitude.GetNeighborhood();
    
    int i =0;
    
    for (i=0; i < static_cast<int>(lNeighborhoodOrientation.Size()); i++)
      {
 	double lOrientation =
	  static_cast<double>(lNeighborhoodOrientation[i]);
 	double lMagnitude =
	  static_cast<double>(lNeighborhoodMagnitude[i]);
 	double lWeightOri =
	  static_cast<double>(lNeighborhoodGaussianWeight[i]);
	
 	unsigned int histoIndex =
	  static_cast<unsigned int>(vcl_floor(36*lOrientation/(2*M_PI)));
	
 	lHistogram[histoIndex] += lMagnitude*lWeightOri;
      }
    
    // Computing smoothed histogram and looking for the maximum
    double max = 0;
    double sum = 0;
    unsigned int maxIndex = 0;
    int j = 0;
    
    for(i=0;i<36;++i)
      {
     	sum = 0;
     	for(j=i-36;j<i;++j)
     	  {
     	    sum+=lHistogram[i-j]*m_HistogramGaussianWeights[j+36];
     	  }
     	if(sum>max)
     	  {
     	    max=sum;
     	    maxIndex = i;
	  }
      }
    magnitude = neighborsMagnitude.GetCenterPixel();
    orientation = maxIndex;
  }
  
  /**
   * PrintSelf Method
   */
  template <class TInputImage, class TOutputPointSet>
  void
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    typedef typename TOutputPointSet::PointsContainerConstIterator PointsIteratorType;
    typedef typename TOutputPointSet::PointDataContainerIterator PointsDataIteratorType;
    typedef itk::ProcessObject ProcessObjectType;
    const OutputPointSetType* output = dynamic_cast<const OutputPointSetType*>(this->ProcessObjectType::GetOutput(0));
          
    Superclass::PrintSelf(os, indent);
    os<<indent<<"Number of octaves: "<<m_OctavesNumber<<std::endl;
    os<<indent<<"Number of scales: "<<m_ScalesNumber<<std::endl;
    
    os<<indent<<"Number of SIFT key points: " << output->GetNumberOfPoints() << std::endl;
    
    if (output->GetNumberOfPoints()>0)
      {
	PointsIteratorType lIterPoint = output->GetPoints()->Begin();
	PointsDataIteratorType lIterDataPoint = output->GetPointData()->Begin();
	
	while (lIterPoint != output->GetPoints()->End() &&
	       lIterDataPoint != output->GetPointData()->End())
	  {
	    os << indent << "Point: " << lIterPoint.Value()
	       << " Scale: " << lIterDataPoint.Value()[0]
	       << " Magnitude: " << lIterDataPoint.Value()[1]
	       << " Orientation: " << lIterDataPoint.Value()[2]
	       << std::endl;
	    
	    ++lIterPoint;
	    ++lIterDataPoint;
	  }
      }
  }
  
} // End namespace otb
