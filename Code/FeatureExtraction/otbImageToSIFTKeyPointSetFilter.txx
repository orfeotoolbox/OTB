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

  template <class TInputImage, class TOutputPointSet>
  const typename ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>::OffsetType
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>::m_Offsets[8] = {
    {{-1,-1}}, //0
    {{-1, 0}}, //1
    {{-1, 1}}, //2
    {{ 0,-1}}, //3
    {{ 0, 1}}, //4
    {{ 1,-1}}, //5
    {{ 1, 0}}, //6
    {{ 1, 1}}, //7
    };
  
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
    m_ChangeSamplePointsMax = 2;
    
    m_SigmaFactorOrientation = 3;
    m_SigmaFactorDescriptor = 1.5;
    
    m_GradientMagnitudeThreshold = 0.2;
    
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
    
    // for each octave, compute the difference of gaussian
    unsigned int lOctave = 0;
    m_Sigmak = vcl_pow(2, static_cast<double>(1/(double)(m_ScalesNumber+1)));
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
	m_ShrinkFilter->SetInput(m_LastGaussian);
	m_ShrinkFilter->SetShrinkFactors(m_ShrinkFactors);
	m_ShrinkFilter->Update();
	
	input = m_ShrinkFilter->GetOutput();
	
	typename InputImageType::PointType origin1;
	typename InputImageType::SpacingType spacing = input->GetSpacing();
	
	origin1[0] = origin0[0] + spacing[0]*0.25;
	origin1[1] = origin0[1] + spacing[1]*0.25;
	
	input->SetOrigin(origin1);
	
	otbGenericMsgDebugMacro( <<"ImageToSIFTKeyPointSetFilter:: Number total key points : " \
				 << m_ValidatedKeyPoints);
	otbGenericMsgDebugMacro( <<"ImageToSIFTKeyPointSetFilter:: Number different sample key points per octave : " \
				 << m_DifferentSamplePoints );
	otbGenericMsgDebugMacro( <<"ImageToSIFTKeyPointSetFilter:: Number discarded key points per octave : " \
				 << m_DiscardedKeyPoints );
	otbGenericMsgDebugMacro( <<"ImageToSIFTKeyPointSetFilter:: Resample image factor : " \
				 << m_ShrinkFactors);
	
      }
    
    otbGenericMsgDebugMacro( <<"ImageToSIFTKeyPointSetFilter:: Total number key points : " \
			     << this->GetOutput()->GetNumberOfPoints());
    
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
    InputImagePointerType previousGaussian;
    
    m_DoGList = ImageListType::New();
    
    m_MagnitudeList = ImageListType::New();
    m_OrientationList = ImageListType::New();
//     m_GaussianWeightOrientationList = ImageListType::New();
//     m_GaussianWeightDescriptorList = ImageListType::New();
    
    // itkRecursiveGaussian use spacing to compute
    // length sigma gaussian (in mm)
    // sigma = sigma/spacing
    //
    // with multiply by spacing before filtering, length sigma gaussian 
    // is compute in pixel
    double xsigman = input->GetSpacing()[0]*m_Sigma0;
    double ysigman = input->GetSpacing()[1]*m_Sigma0;
    
    for (lScale = 0; lScale != m_ScalesNumber+2; lScale++)
      {
	m_XGaussianFilter = GaussianFilterType::New();
	m_YGaussianFilter = GaussianFilterType::New();
	
	m_XGaussianFilter->SetSigma(xsigman);
	m_XGaussianFilter->SetDirection(0);
	m_XGaussianFilter->SetInput(input);
	
	m_YGaussianFilter->SetSigma(ysigman);
	m_YGaussianFilter->SetDirection(1);
	m_YGaussianFilter->SetInput(m_XGaussianFilter->GetOutput());
	
	m_YGaussianFilter->Update();
	
	m_GradientFilter = GradientFilterType::New();
	m_MagnitudeFilter = MagnitudeFilterType::New();
	m_OrientationFilter = OrientationFilterType::New();
// 	m_XOrientationGaussianFilter = GaussianFilterType::New();
// 	m_YOrientationGaussianFilter = GaussianFilterType::New();
// 	m_XDescriptorGaussianFilter = GaussianFilterType::New();
// 	m_YDescriptorGaussianFilter = GaussianFilterType::New();
	
	m_GradientFilter->SetInput(m_YGaussianFilter->GetOutput());
	m_MagnitudeFilter->SetInput(m_GradientFilter->GetOutput());
	m_OrientationFilter->SetInput(m_GradientFilter->GetOutput());
	
// 	m_XOrientationGaussianFilter->SetInput(m_OrientationFilter->GetOutput());
// 	m_XOrientationGaussianFilter->SetSigma(m_SigmaFactorOrientation*xsigman);
// 	m_XOrientationGaussianFilter->SetDirection(0);
	
// 	m_YOrientationGaussianFilter->SetInput(m_XOrientationGaussianFilter->GetOutput());
// 	m_YOrientationGaussianFilter->SetSigma(m_SigmaFactorOrientation*ysigman);
// 	m_YOrientationGaussianFilter->SetDirection(1);
	
// 	m_XDescriptorGaussianFilter->SetInput(m_OrientationFilter->GetOutput());
// 	m_XDescriptorGaussianFilter->SetSigma(m_SigmaFactorDescriptor*xsigman);
// 	m_XDescriptorGaussianFilter->SetDirection(0);
	
// 	m_YDescriptorGaussianFilter->SetInput(m_XDescriptorGaussianFilter->GetOutput());
// 	m_YDescriptorGaussianFilter->SetSigma(m_SigmaFactorDescriptor*ysigman);
// 	m_YDescriptorGaussianFilter->SetDirection(1);
	
	m_MagnitudeFilter->Update();
	m_OrientationFilter->Update();
	
// 	m_YOrientationGaussianFilter->Update();
// 	m_YDescriptorGaussianFilter->Update();
	
	
	m_MagnitudeList->PushBack(m_MagnitudeFilter->GetOutput());
	m_OrientationList->PushBack(m_OrientationFilter->GetOutput());
// 	m_GaussianWeightOrientationList->
// 	  PushBack(m_YOrientationGaussianFilter->GetOutput());
// 	m_GaussianWeightDescriptorList->
// 	  PushBack(m_YDescriptorGaussianFilter->GetOutput());
	
	if (lScale>0)
	  {
	    m_SubtractFilter = SubtractFilterType::New();
	    m_SubtractFilter->SetInput1(m_YGaussianFilter->GetOutput());
	    m_SubtractFilter->SetInput2(previousGaussian);
	    m_SubtractFilter->Update();
	    m_DoGList->PushBack(m_SubtractFilter->GetOutput());
	  }
	
	previousGaussian = m_YGaussianFilter->GetOutput();
	xsigman = xsigman*m_Sigmak;
	ysigman = ysigman*m_Sigmak;
      }
    m_LastGaussian = previousGaussian;
    otbGenericMsgDebugMacro( <<"ImageToSIFTKeyPointSetFilter:: Number of DoG "<<m_DoGList->Size() );
  }
  
  /**
   * Localize key point
   */
  template <class TInputImage, class TOutputPointSet>
  void
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::DetectKeyPoint( const unsigned int octave )
  {
    // need at least 3 DoG, ie 2 scales
    if (m_ScalesNumber > 1 )
      {
	typename ImageListType::Iterator lIterDoG = m_DoGList->Begin()+1;
	unsigned int lScale = 1;
	OutputPointSetPointerType  outputPointSet = this->GetOutput();
	typename InputImageType::SpacingType spacing = lIterDoG.Get()->GetSpacing();
	
	while ( (lIterDoG+1) != m_DoGList->End())
	  {
	    otbGenericMsgDebugMacro( <<"ImageToSIFTKeyPointSetFilter:: octave: " << octave << " scale: " << lScale);
	    // Compute max of DoG
	    MinimumMaximumCalculatorPointerType lMaximumCalculator = MinimumMaximumCalculatorType::New();
	    lMaximumCalculator->SetImage(lIterDoG.Get());
	    lMaximumCalculator->Compute();
	    
	    typename InputImageType::SizeType lRadius;
	    lRadius.Fill(1);
	    typename ImageListType::Iterator lIterNext = lIterDoG+1;
	    typename ImageListType::Iterator lIterPrev = lIterDoG-1;
	    
	    NeighborhoodIteratorType lIterCurrent(lRadius,
						  lIterDoG.Get(),
						  lIterDoG.Get()->GetLargestPossibleRegion());
	    NeighborhoodIteratorType lIterLowerAdjacent(lRadius,
							lIterPrev.Get(),
							lIterPrev.Get()->GetLargestPossibleRegion());
	    NeighborhoodIteratorType lIterUpperAdjacent(lRadius,
							lIterNext.Get(),
							lIterNext.Get()->GetLargestPossibleRegion());
	    
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
 							  lTranslation);
			
			OffsetType lTranslateOffset = {{0,0}};
			
			lTranslateOffset[0] += static_cast<int>(lTranslation[0]>0.5);
			lTranslateOffset[0] += -static_cast<int>(lTranslation[0]<-0.5);
			
			lTranslateOffset[1] += static_cast<int>(lTranslation[1]>0.5);
			lTranslateOffset[1] += -static_cast<int>(lTranslation[1]<-0.5);
			
			NeighborhoodIteratorType moveIterator = neighborCurrentScale+lTranslateOffset;
			
			if ( moveIterator.InBounds())
			  {
			    changed = lTranslateOffset != lOffsetZero;
			    
			    // move iterator
			    neighborCurrentScale+=lTranslateOffset;
			    neighborPreviousScale+=lTranslateOffset;
			    neighborNextScale+=lTranslateOffset;
			  }
			else
			  {
			    changed = false;
			  }						
			lChangeSamplePoints++;
		      }
		    if (changed)
		      {
			m_DifferentSamplePoints++;
		      }
		    
		    // add key point
		    if (accepted)
		      {
			PixelType lOrientation = 0;
			
			lOrientation = ComputeKeyPointOrientation(neighborCurrentScale,
								  lScale,
								  lTranslation[2]);
			
			std::vector<PixelType> lDescriptors = ComputeKeyPointDescriptor(neighborCurrentScale,
											lScale,
											lOrientation);
			
			OutputPointType keyPoint;

			lIterDoG.Get()->TransformIndexToPhysicalPoint(neighborCurrentScale.GetIndex(),
								      keyPoint);
			keyPoint[0] += spacing[0]*lTranslation[0];
			keyPoint[1] += spacing[1]*lTranslation[1];
			
			outputPointSet->SetPoint(m_ValidatedKeyPoints, keyPoint);
			
			OutputPixelType data;
			data.SetSize(2+128);
			// check this, compute scale
			// real scale = octave*scale
			data.SetElement(0,lScale+lTranslation[2]);
			data.SetElement(1,lOrientation);
			typename std::vector<PixelType>::const_iterator lIterDescriptor = 
			  lDescriptors.begin();
			
			unsigned int lIndDesc = 2;
			while (lIterDescriptor != lDescriptors.end())
			  {
			    data.SetElement(lIndDesc, *lIterDescriptor);
			    lIndDesc++;
			    lIterDescriptor++;
			  }
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
    bool isMin = currentScale.GetCenterPixel() < currentScale.GetPixel(m_Offsets[0]);
    bool isMax = currentScale.GetCenterPixel() > currentScale.GetPixel(m_Offsets[0]);
    bool isExtremum = isMin || isMax;
    unsigned int lIterOffset = 0;
    
    while (isExtremum && lIterOffset != 8)
      {
	OffsetType off = m_Offsets[lIterOffset];
	if (isMin)
	  {
	    isExtremum =
	      currentScale.GetCenterPixel() < currentScale.GetPixel(off) &&
 	      currentScale.GetCenterPixel() < previousScale.GetPixel(off) &&
	      currentScale.GetCenterPixel() < nextScale.GetPixel(off);
	  }
	else if (isMax)
	  {
	    isExtremum =
	      currentScale.GetCenterPixel() > currentScale.GetPixel(off) &&
 	      currentScale.GetCenterPixel() > previousScale.GetPixel(off) &&
 	      currentScale.GetCenterPixel() > nextScale.GetPixel(off);
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
			    VectorPointType& solution)
  {
    bool accepted = true;
    solution = VectorPointType(PixelType(0));
        
    PixelType dx = 0.5*(currentScale.GetPixel(m_Offsets[6])
			-currentScale.GetPixel(m_Offsets[1]) );
    
    PixelType dy = 0.5*(currentScale.GetPixel(m_Offsets[4])
			-currentScale.GetPixel(m_Offsets[3]) );
    
    PixelType ds = 0.5*(nextScale.GetCenterPixel()-
			previousScale.GetCenterPixel());
    
    PixelType dxx = currentScale.GetPixel(m_Offsets[6])
      -2*currentScale.GetCenterPixel()
      +currentScale.GetPixel(m_Offsets[1]);
    
    PixelType dyy = currentScale.GetPixel(m_Offsets[3])
      -2*currentScale.GetCenterPixel()
      +currentScale.GetPixel(m_Offsets[4]);
    
    PixelType dss = previousScale.GetCenterPixel()
      -2*currentScale.GetCenterPixel()
      +nextScale.GetCenterPixel();
   
    PixelType dxy = 0.25*(currentScale.GetPixel(m_Offsets[7])
			  +currentScale.GetPixel(m_Offsets[0])
			  -currentScale.GetPixel(m_Offsets[2])
			  -currentScale.GetPixel(m_Offsets[5]) );
    
    PixelType dxs = 0.25*(nextScale.GetPixel(m_Offsets[6])
			  +previousScale.GetPixel(m_Offsets[1])
			  -nextScale.GetPixel(m_Offsets[1])
			  -previousScale.GetPixel(m_Offsets[6]) );
    
    PixelType dys = 0.25*(nextScale.GetPixel(m_Offsets[4])
			  +previousScale.GetPixel(m_Offsets[3])
			  -nextScale.GetPixel(m_Offsets[3])
			  -previousScale.GetPixel(m_Offsets[4]) );
    
    // Compute matrice determinant
    double det = dxx*(dyy*dss-dys*dys) -dxy*(dxy*dss-dxs*dys)+dxs*(dxy*dys-dxs*dyy);

    // Solve system, compute key point offset
    solution[0] = -dx*(dyy*dss-dys*dys)-dy*(dxs*dys-dxy*dss)-ds*(dxy*dys-dyy*dxs);
    solution[1] = -dx*(dys*dxs-dss*dxy)-dy*(dxx*dss-dxs*dxs)-ds*(dxs*dxy-dxx*dys);
    solution[2] = -dx*(dxy*dys-dxs*dyy)-dy*(dxy*dxs-dxx*dys)-ds*(dxx*dyy-dxy*dxy);
    
    // Compute interpolated value DoG for lSolution (determinant factor)
    PixelType lDoGInterpolated = det*currentScale.GetCenterPixel() +
      0.5*(dx*solution[0]+
	   dy*solution[1]+
	   ds*solution[2]);
    
    PixelType lHessianTrace2 = (dxx+dyy)*(dxx+dyy);
    PixelType lHessianDet = dxx*dyy-dxy*dxy;
    // DoG threshold
    
    accepted = fabs(lDoGInterpolated) >= fabs(det*m_DoGThreshold);
    
    // Eliminating edge response

    accepted = accepted &&
      fabs(lHessianTrace2) < fabs(m_RatioEdgeThreshold*lHessianDet);
    
    if (!accepted)
      {
	m_DiscardedKeyPoints++;
      }
    if (det < 1e-10f)
      {
	solution.Fill(0);
      }
    else
      {
	// normalize offset with determinant of derivative matrix
	solution/=det;
      }
    return accepted;
  }
  
  /**
   * Compute key point orientation
   */
  template <class TInputImage, class TOutputPointSet>
  typename ImageToSIFTKeyPointSetFilter<TInputImage, TOutputPointSet>::PixelType
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::ComputeKeyPointOrientation( const NeighborhoodIteratorType& currentScale,
				const unsigned int scale,
				const PixelType translation)
  {    
    // compute histogram
    std::vector<double> lHistogram(36,0.);
    
    typename InputImageType::RegionType region;
    typename InputImageType::RegionType::SizeType regionSize;
    typename InputImageType::RegionType::IndexType indexStart;
    
    regionSize.Fill(3);
    region.SetSize(regionSize);
    indexStart[0] = currentScale.GetIndex()[0]-1;
    indexStart[1] = currentScale.GetIndex()[0]-1;
    region.SetIndex(indexStart);
    
    region.Crop(m_OrientationList->GetNthElement(scale)->GetLargestPossibleRegion());
    
    RegionIteratorType lIterOrientation(m_OrientationList->GetNthElement(scale),
					region);
    
    RegionIteratorType lIterMagn(m_MagnitudeList->GetNthElement(scale),
				 region);
    
    lIterOrientation.GoToBegin();
    lIterMagn.GoToBegin();
    double lSigma = scale*1.5;
    
    while (!lIterOrientation.IsAtEnd() &&
	   !lIterMagn.IsAtEnd())
      {
	PixelType lOrientation = lIterOrientation.Get();
	PixelType lMagnitude = lIterMagn.Get();
	
	double dist2 = (lIterOrientation.GetIndex()[0]-currentScale.GetIndex()[0])
	  *(lIterOrientation.GetIndex()[0]-currentScale.GetIndex()[0])
	  + (lIterOrientation.GetIndex()[1]-currentScale.GetIndex()[1])
	  *(lIterOrientation.GetIndex()[1]-currentScale.GetIndex()[1]);
	
	double lWeightMagnitude = vcl_exp(dist2/(2*lSigma*lSigma));
	
	unsigned int lHistoIndex = static_cast<unsigned int>
	  ( vcl_floor(36*lOrientation/(2*M_PI)) );
	lHistogram[lHistoIndex] += lMagnitude*lWeightMagnitude;
	++lIterOrientation;
	++lIterMagn;
      }
    
    // Computing smoothed histogram and looking for the maximum
    double max = 0;
    double sum = 0;
    unsigned int maxIndex = 0;
    int j = 0;
    int i = 0;
    
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
    
    return static_cast<PixelType>(maxIndex*10);
  }
  

  /**
   * Compute key point descriptor
   */
  template <class TInputImage, class TOutputPointSet>
  std::vector< typename ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>::PixelType >
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::ComputeKeyPointDescriptor( const NeighborhoodIteratorType& currentScale,
			       const unsigned int scale,
			       const PixelType& orientation)
  {
    std::vector<PixelType> lHistogram(128, 0.);
    
    typename InputImageType::RegionType  region;
    typename InputImageType::RegionType::SizeType regionSize;
    typename InputImageType::RegionType::IndexType inputStart;
    
    regionSize[0] = 4;
    regionSize[1] = 4;
    
    unsigned int lHDescriptors=0;
    unsigned int lVDescriptors=0;
    
    // sigma set to one half the width of descriptor window
    double lSigma = 4*0.5;
    
    while (lHDescriptors < 4)
      {
	lVDescriptors = 0;
	while(lVDescriptors < 4)
	  {
	    inputStart[0] = currentScale.GetIndex()[0] + lHDescriptors*4 - 8;
	    inputStart[1] = currentScale.GetIndex()[1] + lVDescriptors*4 - 8;
	    region.SetIndex(inputStart);
	    region.SetSize(regionSize);

	    if (region.Crop(m_OrientationList->GetNthElement(scale)->GetLargestPossibleRegion()))
	      {
		RegionIteratorType lIterMagnitude(m_MagnitudeList->GetNthElement(scale),
						  region);
		RegionIteratorType lIterOrientation(m_OrientationList->GetNthElement(scale),
						    region);
		RegionIteratorType lIterNMagnitude(m_MagnitudeList->GetNthElement(scale),
						   region);
		RegionIteratorType lIterNOrientation(m_OrientationList->GetNthElement(scale),
						     region);
					
		for ( lIterMagnitude.GoToBegin(), lIterOrientation.GoToBegin();
		      !lIterMagnitude.IsAtEnd();
		      ++lIterMagnitude, ++lIterOrientation)
		  { 
		    float dx = vcl_abs(lIterMagnitude.GetIndex()[0]-currentScale.GetIndex()[0]);
		    float dy = vcl_abs(lIterMagnitude.GetIndex()[1]-currentScale.GetIndex()[1]);
		    
		    float dist2 = dx*dx+dy*dy;
		    if (dist2 <= 8*8)
		      {
			float nx = vcl_cos(dx)+vcl_sin(dy);
			float ny = -vcl_sin(dx)+vcl_cos(dy);
			
			typename InputImageType::IndexType nIndex;
			nIndex[0] = static_cast<unsigned int>(vcl_floor(currentScale.GetIndex()[0]+nx+0.5));
			nIndex[1] = static_cast<unsigned int>(vcl_floor(currentScale.GetIndex()[1]+ny+0.5));
			
			lIterNMagnitude.SetIndex(nIndex);
			lIterNOrientation.SetIndex(nIndex);
			
			PixelType lMagnitude = lIterNMagnitude.Get();
			PixelType lOrientation = lIterNOrientation.Get();
			
			unsigned int lHistoIndex = 0;
			double diffAngle = lOrientation/(2*M_PI)*8 - orientation/45;
			if (diffAngle>=0)
			  {
			    lHistoIndex = static_cast<unsigned int>(diffAngle);
			  }
			else
			  {
			    lHistoIndex = static_cast<unsigned int>(diffAngle+8);
			  }
			
			double lWeightMagnitude = vcl_exp(dist2/(2*lSigma*lSigma));
			PixelType lHistoEntry = lMagnitude*lWeightMagnitude;
			
			lHistogram[lHDescriptors*4*8+lVDescriptors*8+lHistoIndex] += lHistoEntry;
		      }
		  }
	      }
	    lVDescriptors++;
	  }
	lHDescriptors++;
      }
    
    // normalize histogram to unit lenght
    typename std::vector<PixelType>::iterator lIterHisto = lHistogram.begin();
    float lNorm = 0.0;
    
    while (lIterHisto != lHistogram.end())
      {
	lNorm = lNorm + (*lIterHisto)*(*lIterHisto);
	++lIterHisto;
      }
    lNorm = vcl_sqrt(lNorm);
    
    lIterHisto = lHistogram.begin();
    while(lIterHisto != lHistogram.end())
      {
	if (lNorm>0)
	  {
	    *lIterHisto = (*lIterHisto)/lNorm;
	  }
	else
	  {
	    *lIterHisto = m_GradientMagnitudeThreshold;
	  }
	
	// threshold gradient magnitude
	if (*lIterHisto > m_GradientMagnitudeThreshold)
	  {
	    *lIterHisto = m_GradientMagnitudeThreshold;
	  }
	++lIterHisto;
      }
    
    // renormalize histogram to unit length
    lIterHisto = lHistogram.begin();
    lNorm = 0.0;
    
    while (lIterHisto != lHistogram.end())
      {
	lNorm = lNorm + (*lIterHisto)*(*lIterHisto);
	++lIterHisto;
      }
    lNorm = vcl_sqrt(lNorm);
    
    lIterHisto = lHistogram.begin();
    while(lIterHisto != lHistogram.end())
      {
	*lIterHisto = (*lIterHisto)/lNorm;
	++lIterHisto;
      }
    
    return lHistogram;
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
  }
  
} // End namespace otb
