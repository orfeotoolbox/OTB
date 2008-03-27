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
#ifndef _otbImageToSIFTKeyPointSetFilter_txx
#define _otbImageToSIFTKeyPointSetFilter_txx

#include "otbImageToSIFTKeyPointSetFilter.h"


#include "otbImageFileWriter.h"

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
    m_NumberOfIterations = 1; 
  }

  template <class TInputImage, class TOutputPointSet>
  void
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::SetInput(const InputImageType * image)
  {
    this->Superclass::SetInput(0,image);
  }

  template <class TInputImage, class TOutputPointSet>
  void
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::ResetFilters()
  {
    // filters instantiation
    //     m_XGaussianFilter1 = GaussianFilterType::New();
    //     m_YGaussianFilter1 = GaussianFilterType::New();
    
    //     m_XGaussianFilter2 = GaussianFilterType::New();
    //     m_YGaussianFilter2 = GaussianFilterType::New();
    
    m_DiscreteGaussian1 = DiscreteGaussianFilterType::New();
    m_DiscreteGaussian2 = DiscreteGaussianFilterType::New();
    
    m_XGaussianFilter3 = GaussianFilterType::New();
    m_YGaussianFilter3 = GaussianFilterType::New();

    //     m_ExpandFilter = ExpandFilterType::New();
    //     m_ExpandFilter->SetExpandFactors(2);
    
    m_SubtractFilter = SubtractFilterType::New();
    m_ResampleFilter = ResampleFilterType::New();
    m_GradientFilter = GradientFilterType::New();
    m_MagnitudeFilter = MagnitudeFilterType::New();
    m_OrientationFilter = OrientationFilterType::New();
    m_RescaleFilter = RescaleFilterType::New();
    m_MinimumMaximumCalculator = MinimumMaximumCalculatorType::New();
    m_MultiplyFilter = MultiplyFilterType::New();
    
    //     m_ResampleFilter->SetNumeratorFactors(2);
    //     m_ResampleFilter->SetDenominatorFactors(3);
    
    // Pipeline connections
    //     m_XGaussianFilter1->SetInput(m_ResampleFilter->GetOutput());
    //     m_XGaussianFilter2->SetInput(m_ResampleFilter->GetOutput());
    //     m_YGaussianFilter1->SetInput(m_XGaussianFilter1->GetOutput());
    //     m_YGaussianFilter2->SetInput(m_XGaussianFilter2->GetOutput());
    m_DiscreteGaussian1->SetInput(m_ResampleFilter->GetOutput());
    m_DiscreteGaussian2->SetInput(m_ResampleFilter->GetOutput());
    
    //m_SubtractFilter->SetInput1(m_YGaussianFilter1->GetOutput());
    //m_SubtractFilter->SetInput2(m_YGaussianFilter2->GetOutput());
    
    m_SubtractFilter->SetInput2(m_DiscreteGaussian1->GetOutput());
    m_SubtractFilter->SetInput1(m_DiscreteGaussian2->GetOutput());
    
    //m_GradientFilter->SetInput(m_YGaussianFilter1->GetOutput());
    m_GradientFilter->SetInput(m_DiscreteGaussian1->GetOutput());
    
    m_MagnitudeFilter->SetInput(m_GradientFilter->GetOutput());
    m_OrientationFilter->SetInput(m_GradientFilter->GetOutput());
    m_MinimumMaximumCalculator->SetImage(m_MagnitudeFilter->GetOutput());
    m_RescaleFilter->SetInput(m_MagnitudeFilter->GetOutput());
    m_XGaussianFilter3->SetInput(m_OrientationFilter->GetOutput());
    m_YGaussianFilter3->SetInput(m_OrientationFilter->GetOutput());
    m_MultiplyFilter->SetInput1(m_RescaleFilter->GetOutput());
    m_MultiplyFilter->SetInput2(m_YGaussianFilter3->GetOutput());
  
    // Set the sigma value for each gaussian filter
    //m_XGaussianFilter1->SetSigma(vcl_sqrt(2.));
    //m_XGaussianFilter1->SetDirection(0);
    //m_XGaussianFilter1->SetNormalizeAcrossScale(true);
    
    
    //     m_YGaussianFilter1->SetSigma(vcl_sqrt(2.));
    //     m_YGaussianFilter1->SetDirection(1);
    //m_YGaussianFilter1->SetNormalizeAcrossScale(true);

    // m_XGaussianFilter2->SetSigma(2.);
    // m_XGaussianFilter2->SetDirection(0);
    //m_XGaussianFilter2->SetNormalizeAcrossScale(true);
    
    // m_YGaussianFilter2->SetSigma(2.);
    //     m_YGaussianFilter2->SetDirection(1);
    //m_YGaussianFilter2->SetNormalizeAcrossScale(true);

    m_DiscreteGaussian1->SetVariance(vcl_sqrt(2.));
    m_DiscreteGaussian2->SetVariance(2.);
    
    m_XGaussianFilter3->SetDirection(0);
    m_YGaussianFilter3->SetDirection(1);
  }

  template <class TInputImage, class TOutputPointSet>
  void
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::KeyFeaturesFilterSetup(unsigned int iteration)
  {
    m_MagnitudeFilter->Update();
    m_MinimumMaximumCalculator->Compute();
    m_RescaleFilter->SetOutputMinimum(m_MinimumMaximumCalculator->GetMinimum());
    m_RescaleFilter->SetOutputMaximum(0.1*m_MinimumMaximumCalculator->GetMaximum());

    //! \TODO: check this
    m_XGaussianFilter3->SetSigma(3.*(iteration+1));
    m_YGaussianFilter3->SetSigma(3.*(iteration+1));
  }


  template <class TInputImage, class TOutputPointSet>
  void
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::GenerateData()
  {
    // Warning: TO REMOVE later !
    itk::OStringStream oss;
    typedef otb::ImageFileWriter<InputImageType> WriterType;
    typename WriterType::Pointer writer = WriterType::New();
  
    ResetFilters();

    // Get the input pointer
    InputImageConstPointerType inputPtr  = this->GetInput(0);
    OutputPointSetPointerType  outputPtr = this->GetOutput();

    RegionType largestRegion = inputPtr->GetLargestPossibleRegion();

    // Set up the initial resampling (factor = 2)
    
    m_ResampleFilter->SetInput(inputPtr);

    // Compute the new spacing
    SpacingType inputSpacing = inputPtr->GetSpacing();
    SpacingType outputSpacing;
    outputSpacing[0] = inputSpacing[0]*0.5;
    outputSpacing[1] = inputSpacing[1]*0.5;
    m_ResampleFilter->SetOutputSpacing(outputSpacing);
    
    // Compute new origin
    PointType inputOrigin = inputPtr->GetOrigin();
    PointType outputOrigin;
    outputOrigin[0] = inputOrigin[0]+(outputSpacing[0] - inputSpacing[0])*0.5;
    outputOrigin[1] = inputOrigin[1]+(outputSpacing[1] - inputSpacing[1])*0.5;
    m_ResampleFilter->SetOutputOrigin(outputOrigin);
    
    // Compute the new size 
    SizeType inputSize = inputPtr->GetLargestPossibleRegion().GetSize();
    SizeType outputSize;
    outputSize[0] = inputSize[0]*2;
    outputSize[1] = inputSize[1]*2;
    m_ResampleFilter->SetSize(outputSize);
    
    // Compute new start index ??
    //m_ResampleFilter->SetOutputStartIndex(index);
    
    //     IndexType index = inputPtr->GetLargestPossibleRegion().GetIndex();
    //     for(unsigned int dimension=0; dimension<InputImageType::ImageDimension;++dimension)
    //       {
    // 	size[dimension]=2*size[dimension];
    // 	index[dimension]=2*index[dimension];
    //       }
    
    //     writer->SetFileName("ResamplerOutput0.tif");
    //     writer->SetInput(m_ResampleFilter->GetOutput());
    //     writer->Update();
    
    // Trigger pipeline execution
    m_SubtractFilter->Update();
    
    // Temporary
    writer->SetFileName("SubtractOutput0.tif");
    writer->SetInput(m_SubtractFilter->GetOutput());
    writer->Update();
    
    // Set up the key filters
    KeyFeaturesFilterSetup(0);

    // Loop on the first level to spot potential keys
    SizeType radius;
    radius.Fill(1);
    NeighborhoodIteratorType it(radius,m_SubtractFilter->GetOutput(),
				m_SubtractFilter->GetOutput()->GetLargestPossibleRegion());
    // Temporary point
    it.GoToBegin();
    std::cout << "==============================" << std::endl;
    std::cout << "Whole Detection octave = " << 0 << std::endl;
    while(!it.IsAtEnd())
      {
	if(IsLocalExtremum(it.GetNeighborhood()))
	  {
	    OutputPointType point;
	    //m_SubtractFilter->GetOutput()->
	    //  TransformIndexToPhysicalPoint(it.GetIndex(),point);
	    point[0] = vcl_floor(it.GetIndex()[0]*m_SubtractFilter->GetOutput()->GetSpacing()[0]+0.5);
	    point[1] = vcl_floor(it.GetIndex()[1]*m_SubtractFilter->GetOutput()->GetSpacing()[1]+0.5);
	    
	    std::cout << "Key point : " << point << " index : " << it.GetIndex() << std::endl;
	    AddKey(point);
	    UpdateKey(m_ResultMap.find(point),0);
	  }
	++it;
      }

    InputImagePointerType current;
    // for each iteration step
    for(unsigned int iteration = 1;iteration<m_NumberOfIterations;++iteration)
      {
	// Store current image for recursion
	//current = m_YGaussianFilter2->GetOutput();
	current = m_DiscreteGaussian2->GetOutput();
	
	ResetFilters();
	
	// Set up the resampling (factor = 2/3)
	m_ResampleFilter->SetInput(current);
	
	// Compute the new spacing
	inputSpacing =current->GetSpacing();
	outputSpacing[0] = inputSpacing[0]*1.5;
	outputSpacing[1] = inputSpacing[1]*1.5;
	m_ResampleFilter->SetOutputSpacing(outputSpacing);
	
	// Compute the new origin
	inputOrigin = current->GetOrigin();
	outputOrigin[0] = inputOrigin[0]+ (outputSpacing[0]-inputSpacing[0])*0.5;
	outputOrigin[1] = inputOrigin[1]+ (outputSpacing[1]-inputSpacing[1])*0.5;
	m_ResampleFilter->SetOutputOrigin(outputOrigin);
	
	// Compute the new size 
 	inputSize = current->GetLargestPossibleRegion().GetSize();
	outputSize[0] = static_cast<unsigned int>(vcl_floor(static_cast<double>(inputSize[0]*2/3)));
	outputSize[1] = static_cast<unsigned int>(vcl_floor(static_cast<double>(inputSize[1]*2/3)));
	m_ResampleFilter->SetSize(outputSize);
	
	// 	SizeType size;
	// 	for(unsigned int dimension=0; dimension<InputImageType::ImageDimension;++dimension)
	// 	  {
	// 	    size[dimension]=static_cast<unsigned int>(vcl_floor(static_cast<double>(2*oldSize[dimension])/3));
	// 	  }
	
      	oss<<"ResamplerOutput"<<iteration<<".tif";
	writer->SetFileName(oss.str());
	writer->SetInput(m_ResampleFilter->GetOutput());
	writer->Update();
	oss.str("");
	oss<<"SubtractOutput"<<iteration<<".tif";
	writer->SetFileName(oss.str());//.c_str());
	writer->SetInput(m_SubtractFilter->GetOutput());
	writer->Update();
	oss.str("");

	// Set up key features calculation
	KeyFeaturesFilterSetup(iteration);

	// This list will contain the key to suppress
	std::list<ResultMapIteratorType> supressionList;
	
	// Walk the previousmy detected key
	ResultMapIteratorType itMap = m_ResultMap.begin();
	while(itMap != m_ResultMap.end())
	  {
	    // If the key is not valid at the scale
	    if(!CheckKey(itMap))
	      {
		// Mark it as supressed
		supressionList.push_back(itMap);
	      }
	    else
	      {
		// Update the gradient and orientation field for the current scale and key
		UpdateKey(itMap, iteration);
	      }
	    ++itMap;
	  }

	// Actually suppress the key marked as suppressed
	typename std::list<ResultMapIteratorType>::iterator suprIt = supressionList.begin();
	while(suprIt!=supressionList.end())
	  {
	    m_ResultMap.erase(*suprIt);
	    ++suprIt;
	  }

	// Loop on the first level to spot potential keys
	NeighborhoodIteratorType iter(radius,
				      m_SubtractFilter->GetOutput(),
				      m_SubtractFilter->GetOutput()->GetLargestPossibleRegion());
	// Temporary point
	std::cout << "==============================" << std::endl;
	std::cout << "Whole Detection octave = " << iteration << std::endl;
	iter.GoToBegin();
	while(!iter.IsAtEnd())
	  {
	    if(IsLocalExtremum(iter.GetNeighborhood()))
	      {
		OutputPointType point;
		m_SubtractFilter->GetOutput()->
		  TransformIndexToPhysicalPoint(iter.GetIndex(),point);
		std::cout << "Key point : " << point << " index : " << iter.GetIndex() << std::endl;
	      }
	    ++iter;
	  }
      }
    
    // Finally export the sift keys
    ExportSolution();
  }

  template <class TInputImage, class TOutputPointSet>
  void
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::AddKey(const OutputPointType& point)
  {
    OutputPixelType values;
    values.SetSize(2*m_NumberOfIterations);
    values.Fill(0.);
    m_ResultMap[point]=values;

  }

  template <class TInputImage, class TOutputPointSet>
  typename ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::RegionType
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::GetLocalRegion(const IndexType & index) const
  {
    // Constructing a small region containing the index to check
    RegionType requestedRegion;
    IndexType regionIndex = index;
    for(unsigned int dimension=0; dimension<InputImageType::ImageDimension;++dimension)
      {
	regionIndex[dimension]--;
      }
  
    SizeType size;
    size.Fill(3);
    requestedRegion.SetSize(size);
    requestedRegion.SetIndex(regionIndex);
    return requestedRegion;
  }

  template <class TInputImage, class TOutputPointSet>
  bool
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::CheckKey(ResultMapIteratorType it)
  {
    // Go to the key location
    m_SubtractFilter->GetOutput()->UpdateOutputInformation();
    IndexType index;
    //m_SubtractFilter->GetOutput()->TransformPhysicalPointToIndex(it->first,index);
    
    unsigned int i;
    for(i = 0;i<InputImageType::ImageDimension;++i)
      {
     	index[i]=static_cast<int>(vcl_floor((it->first[i]-m_SubtractFilter->GetOutput()->GetOrigin()[i])/m_SubtractFilter->GetOutput()->GetSpacing()[i]+0.5));
      }
    std::cout << "Check key point map = " << it->first << " Index = " << index << std::endl;
    
    RegionType requestedRegion = GetLocalRegion(index);
 
    requestedRegion.Crop(m_SubtractFilter->GetOutput()->GetLargestPossibleRegion());

    m_SubtractFilter->GetOutput()->SetRequestedRegion(requestedRegion);
    m_SubtractFilter->GetOutput()->PropagateRequestedRegion();
    m_SubtractFilter->GetOutput()->UpdateOutputData();

    // Building the neighborhood iterator
    SizeType radius;
    radius.Fill(1);
    NeighborhoodIteratorType neighIt(radius,m_SubtractFilter->GetOutput(),
				     requestedRegion);

  

    neighIt.SetLocation(index);
    
    // Check if local extremum
    return IsLocalExtremum(neighIt.GetNeighborhood());
  }

  template <class TInputImage, class TOutputPointSet>
  bool
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::IsLocalExtremum(const NeighborhoodType& neigh) const
  {
    PixelType center = neigh.GetCenterValue();
    unsigned int centerIndex = neigh.GetCenterNeighborhoodIndex();
    unsigned int i = 0;
    bool isMax = center > neigh[0];
    bool isMin = center < neigh[0]; 

    while(i!=neigh.Size() && (isMax || isMin) )
      {
	if(i != centerIndex)
	  {
	    isMax = isMax && (center > neigh[i]);
	    isMin = isMin && (center < neigh[i]);
	  }
	++i;
      }
    return isMax || isMin;
  }

  template <class TInputImage, class TOutputPointSet>
  void
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::UpdateKey(ResultMapIteratorType it, const unsigned int iteration)
  {
    // Computing the magnitude key value
    m_RescaleFilter->GetOutput()->UpdateOutputInformation();
    IndexType index;
    m_RescaleFilter->GetOutput()->TransformPhysicalPointToIndex(it->first,index);
    RegionType requestedRegion = GetLocalRegion(index);
    requestedRegion.Crop(m_RescaleFilter->GetOutput()->GetLargestPossibleRegion());
    m_RescaleFilter->GetOutput()->SetRequestedRegion(requestedRegion);
    m_RescaleFilter->GetOutput()->PropagateRequestedRegion();
    m_RescaleFilter->GetOutput()->UpdateOutputData();
    SizeType radius;
    radius.Fill(1);
    NeighborhoodIteratorType neighIt(radius,m_RescaleFilter->GetOutput(),
				     requestedRegion);  
    neighIt.SetLocation(index);
    it->second.SetElement(2*iteration,neighIt.GetCenterPixel());

    // Computing the orientation
    m_MultiplyFilter->GetOutput()->UpdateOutputInformation();
    m_MultiplyFilter->GetOutput()->SetRequestedRegion(requestedRegion);
    m_MultiplyFilter->GetOutput()->PropagateRequestedRegion();
    m_MultiplyFilter->GetOutput()->UpdateOutputData();

    std::vector<double> histogram(36,0.);
  
    // Get the neighborhoods
    typename NeighborhoodIteratorType::NeighborhoodType orientationNeighborhood, histoWeightNeighborhood;

    NeighborhoodIteratorType orientationNeighIt(radius,m_OrientationFilter->GetOutput(),
						requestedRegion);  
    orientationNeighIt.SetLocation(index);
    orientationNeighborhood = orientationNeighIt.GetNeighborhood();

    NeighborhoodIteratorType histoWeightNeighIt(radius,m_MultiplyFilter->GetOutput(),
						requestedRegion);  
    histoWeightNeighIt.SetLocation(index);
    histoWeightNeighborhood = histoWeightNeighIt.GetNeighborhood();

    int i;
    int j,histoIndex;
    double orientationValue;
    double sum;

    // For each pixel in the neighborhood
    for(i = 0;i<static_cast<int>(orientationNeighborhood.Size());++i)
      {
	orientationValue = static_cast<double>(orientationNeighborhood[i]);
	histoIndex = static_cast<unsigned int>(vcl_floor(36*orientationValue/(2*M_PI)));
	histogram[histoIndex]+= static_cast<double>(histoWeightNeighborhood[i]);
      }

    // Computing smoothed histogram and looking for the maximum
    double max = 0;
    int maxIndex = 0;
    for(i=0;i<36;++i)
      {
	sum = 0;
	for(j=i-36;j<i;++j)
	  {
	    sum+=histogram[i-j]*m_HistogramGaussianWeights[j+36];
	  }
	if(sum>max)
	  {
	    max=sum;
	    maxIndex = i;
	  }
      }
    it->second.SetElement(2*iteration+1,10.*maxIndex);
  }

  template <class TInputImage, class TOutputPointSet>
  void
  ImageToSIFTKeyPointSetFilter<TInputImage,TOutputPointSet>
  ::ExportSolution()
  {
    // Output pointer
    OutputPointSetPointerType  outputPtr = this->GetOutput();
    InputImageConstPointerType inputPtr  = this->GetInput(0);

    typename OutputPointSetType::PointIdentifier id = 0;
    typename OutputPointSetType::PointType point;

    // Walk the previousmy detected key
    ResultMapIteratorType itMap = m_ResultMap.begin();
    while(itMap != m_ResultMap.end())
      {
	outputPtr->SetPoint(id,itMap->first);
	outputPtr->SetPointData(id,itMap->second);
	++itMap;
	++id;
      }
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
    os<<indent<<"Number of iterations: "<<m_NumberOfIterations<<std::endl;
  }
} // End namespace otb
#endif
