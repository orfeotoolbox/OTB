#ifndef otbSLICFilter_txx
#define otbSLICFilter_txx

#include "otbSLICFilter.h"

namespace otb{

  template <class TInputImage, class TOutputLabelImage>
  SLICFilter<TInputImage, TOutputLabelImage>
  ::SLICFilter() :
    m_SpatialWidth(100),
    m_SpatialDistanceWeight(1),
    m_MaxIterationNumber(10),
    m_Threshold(0.1),
    m_NbSPx(1),
    m_Margin(0)
  {
    this->SetNumberOfRequiredInputs(1);
    this->SetNumberOfRequiredOutputs(1);
    this->SetNthOutput(0,TOutputLabelImage::New());
  }

  template <class TInputImage, class TOutputLabelImage>
  SLICFilter<TInputImage, TOutputLabelImage>
  ::~SLICFilter()
  {}
  
  template <class TInputImage, class TOutputLabelImage>
  void
  SLICFilter<TInputImage, TOutputLabelImage>
  ::SetInput(const TInputImage* input)
  {
    this->ProcessObject::SetNthInput(0,const_cast<TInputImage*>(input));
  }
  template <class TInputImage, class TOutputLabelImage>
  void
  SLICFilter<TInputImage, TOutputLabelImage>
  ::AddInputMargin(typename TOutputLabelImage::Pointer InputSegPtr)
  {
    m_InputSeg.push_back(InputSegPtr);
  }
 
  template <class TInputImage, class TOutputLabelImage>
  void
  SLICFilter<TInputImage, TOutputLabelImage>
  ::SetTile(ProcessingTile const& _t)
  {
    m_Tile = _t;
  }
  template <class TInputImage, class TOutputLabelImage>
  void
  SLICFilter<TInputImage, TOutputLabelImage>
  ::ClearMargins()
  {
    m_InputSeg.clear();
  }
  template <class TInputImage, class TOutputLabelImage>
  TInputImage *
  SLICFilter<TInputImage, TOutputLabelImage>
  ::GetInput()
  {
    return static_cast<TInputImage *>(this->itk::ProcessObject::GetInput(0));
  }
  template <class TInputImage, class TOutputLabelImage>
  TOutputLabelImage *
  SLICFilter<TInputImage, TOutputLabelImage>
  ::GetOutput()
  {
    return static_cast<TOutputLabelImage *>(this->itk::ProcessObject::GetOutput(0));
  }
  template <class TInputImage, class TOutputLabelImage>
  const TOutputLabelImage *
  SLICFilter<TInputImage, TOutputLabelImage>
  ::GetOutput() const
  {
    return static_cast<TOutputLabelImage *>(this->itk::ProcessObject::GetOutput(0));
  }
  
  template <class TInputImage, class TOutputLabelImage>
  void
  SLICFilter<TInputImage, TOutputLabelImage>
  ::GenerateInputRequestedRegion(){
    this->GetOutput()->SetRegions(this->GetInput()->GetLargestPossibleRegion());
  }
  template <class TInputImage, class TOutputLabelImage>
  bool
  SLICFilter<TInputImage, TOutputLabelImage>
  ::inImageCorner(std::vector<RegionType> const& corners, typename TOutputLabelImage::IndexType index){
    for(RegionType cornerRegion : corners)
      if(cornerRegion.IsInside(index))
	return true;   
    return false;
  }


  template <class TInputImage, class TOutputLabelImage>
  void
  SLICFilter<TInputImage, TOutputLabelImage>
  ::removeFromCentroid(PixelType const& currentPixel, typename TInputImage::IndexType index, LabelType label, CountContainerType & count,  ListSampleType & means){          		
    const typename CountContainerType::iterator searchCount = count.find(label);
    //Update with running formulas
    //Update count
    searchCount->second--;	     
    //Update Means
    const typename ListSampleType::iterator searchM = means.find(label);	    
    const SampleType oldMeans = searchM->second;
    SampleType newMeans = oldMeans;
    const double N = searchCount->second;//Cast int to double
    const unsigned int nbComps = newMeans.size() - 2 ;
    for(unsigned int i = 0 ; i < nbComps ; ++i)
      {
	newMeans[i] += (oldMeans[i]-currentPixel[i])/N;
      }
    newMeans[nbComps] += (oldMeans[nbComps]-index[0])/N;
    newMeans[nbComps+1] += (oldMeans[nbComps+1]-index[1])/N;
    searchM->second = newMeans;
  }

  template <class TInputImage, class TOutputLabelImage>
  void
  SLICFilter<TInputImage, TOutputLabelImage>
  ::addToCentroid(PixelType const& currentPixel, typename TInputImage::IndexType index, LabelType label, CountContainerType & count,  ListSampleType & means){          		
    const typename CountContainerType::iterator searchCount = count.find(label);
    //Update with running formulas
    //Update count
    searchCount->second++;	     
    //Update Means
    const typename ListSampleType::iterator searchM = means.find(label);	    
    const SampleType oldMeans = searchM->second;
    SampleType newMeans = oldMeans;
    const double N = searchCount->second;//Cast int to double
    const unsigned int nbComps = newMeans.size() - 2 ;
    for(unsigned int i = 0 ; i < nbComps ; ++i)
      {
	newMeans[i] += (currentPixel[i]-oldMeans[i])/N;
      }
    newMeans[nbComps] += (index[0]-oldMeans[nbComps])/N;
    newMeans[nbComps+1] += (index[1]-oldMeans[nbComps+1])/N;
    searchM->second = newMeans;
  }
  
  template <class TInputImage, class TOutputLabelImage>
  void
  SLICFilter<TInputImage, TOutputLabelImage>
  ::GenerateData()
  {   
    this->GetOutput()->SetRegions(this->GetInput()->GetLargestPossibleRegion());
    this->GetOutput()->SetOrigin(this->GetInput()->GetOrigin());
    this->GetOutput()->SetSignedSpacing(this->GetInput()->GetSignedSpacing());
    this->GetOutput()->Allocate();
    this->GetOutput()->FillBuffer(0);

    //Initialize region without margins
    const typename RegionType::IndexType innerIndex({m_Tile.region.GetIndex()[0]+ m_Tile.margin[3],m_Tile.region.GetIndex()[1]+m_Tile.margin[0]}); 
    const typename RegionType::SizeType innerSize({m_Tile.region.GetSize()[0]-m_Tile.margin[3]-m_Tile.margin[1],m_Tile.region.GetSize()[1]-m_Tile.margin[0]-m_Tile.margin[2]});
    const RegionType innerRegion(innerIndex,innerSize);
    const unsigned int nbComps = this->GetInput()->GetNumberOfComponentsPerPixel();
    std::unordered_set<LabelType> stableLabels;
    //Calculate stable segments, depending on whether tile is touching image edge
    const unsigned int topMargin = m_Tile.tileNeighbors[NBH_TOP] >= 0 ? 1 : 0;
    const unsigned int rightMargin = m_Tile.tileNeighbors[NBH_RIGHT] >= 0 ? 1 : 0;
    const unsigned int bottomMargin = m_Tile.tileNeighbors[NBH_BOTTOM] >= 0 ? 1 : 0;
    const unsigned int leftMargin = m_Tile.tileNeighbors[NBH_LEFT] >= 0 ? 1 : 0;
    const bool stableSegments = (topMargin || rightMargin || bottomMargin || leftMargin) && (m_Margin > 0);
    //Calculate margin region (contains all pixels except for ones on the edge of the image)
    //TODO Improve this with faces calculator ?
    const typename RegionType::IndexType marginIndex({m_Tile.region.GetIndex()[0]+leftMargin,m_Tile.region.GetIndex()[1]+topMargin}); 
    const typename RegionType::SizeType marginSize({m_Tile.region.GetSize()[0]-leftMargin-rightMargin,m_Tile.region.GetSize()[1]-topMargin-bottomMargin});
    const RegionType marginRegion(marginIndex,marginSize);

    typename RegionType::IndexType cornerIndex;    
    std::vector<RegionType> corners;

    if(bottomMargin && leftMargin)
      {
	const long unsigned int m = m_Tile.margin[2];
	cornerIndex[0]=m_Tile.region.GetIndex()[0];
	cornerIndex[1]=m_Tile.region.GetIndex()[1]+m_Tile.region.GetSize()[1]-m; 
	const typename RegionType::SizeType cornerSize({m,m});
	corners.push_back(RegionType(cornerIndex,cornerSize));
      }
    if(bottomMargin && rightMargin)
      {
	const long unsigned int m = m_Tile.margin[2];
	cornerIndex[0]=m_Tile.region.GetIndex()[0]+m_Tile.region.GetSize()[0]-m;
	cornerIndex[1]=m_Tile.region.GetIndex()[1]+m_Tile.region.GetSize()[1]-m; 
	const typename RegionType::SizeType cornerSize({m,m});
	corners.push_back(RegionType(cornerIndex,cornerSize));
      }
    
    //Write margin values into output
    for(auto const& im : m_InputSeg)
      {
	itk::ImageRegionIterator<TOutputLabelImage> outputIt(this->GetOutput(),im->GetLargestPossibleRegion());
	itk::ImageRegionConstIterator<TOutputLabelImage> inputIt(im,im->GetLargestPossibleRegion());
	for(inputIt.GoToBegin(), outputIt.GoToBegin(); !inputIt.IsAtEnd() && !outputIt.IsAtEnd(); ++inputIt, ++outputIt)
	  {
	    const LabelType currentLabel = inputIt.Get();
	    outputIt.Set(currentLabel);
	  }  
      }
    ListSampleType initialSeedMeans;
    CountContainerType initialCount;
    // //To avoid rehashes
    initialSeedMeans.reserve(m_Tile.region.GetSize()[0]*m_Tile.region.GetSize()[1]/m_SpatialWidth/m_SpatialWidth);
    initialCount.reserve(m_Tile.region.GetSize()[0]*m_Tile.region.GetSize()[1]/m_SpatialWidth/m_SpatialWidth);
    //Update means and count in margin zones according to initial grid and stable elements
    itk::ImageRegionIterator<TOutputLabelImage> outputIt(this->GetOutput(),this->GetOutput()->GetLargestPossibleRegion());
    itk::ImageRegionConstIteratorWithIndex<TInputImage> vit(this->GetInput(), this->GetInput()->GetLargestPossibleRegion());
    for(vit.GoToBegin(), outputIt.GoToBegin(); !outputIt.IsAtEnd() && !vit.IsAtEnd() ; ++outputIt, ++vit)
      {
	if(!inImageCorner(corners,vit.GetIndex())){
	  LabelType currentLabel;	  
	  if(innerRegion.IsInside(vit.GetIndex())){
	    //Set grid in inner region
	    currentLabel = vcl_floor(vit.GetIndex()[0]/m_SpatialWidth)+m_NbSPx*vcl_floor(vit.GetIndex()[1]/m_SpatialWidth)+1;
	    outputIt.Set(currentLabel);
	  }
	  else {
	    //Read from margin values
	    currentLabel = outputIt.Get();
	  }
	  //Calculate initial means and count
	  if(initialSeedMeans.find(currentLabel) == initialSeedMeans.end()){
	    initialCount.insert({currentLabel,1});
	    
	    SampleType initialSeed(nbComps+2);
	    for (unsigned int i = 0; i < nbComps; i++) {
	      initialSeed[i] = vit.Get()[i];
	    }
	    initialSeed[nbComps] = vit.GetIndex()[0];
	    initialSeed[nbComps+1] = vit.GetIndex()[1];
	    
	    initialSeedMeans.insert({currentLabel,initialSeed});
	  }
	  else {
	    addToCentroid(vit.Get(),vit.GetIndex(),currentLabel,initialCount,initialSeedMeans);					     
	  }
	  //Calculate stable segments
	  if(stableSegments && !marginRegion.IsInside(vit.GetIndex()) && stableLabels.find(currentLabel) == stableLabels.end()){
	    stableLabels.insert(currentLabel);
	  }
	}
      }
    //Setup first iteration
    double residual = 0;
    bool firstLoop = (m_MaxIterationNumber > 0);
    unsigned int iteration = 0;
    //Copy initial values
    ListSampleType currentSeedMeans(initialSeedMeans);
    CountContainerType currentCount(initialCount);
    simplePointCalculator<NeighborhoodContainerIteratorType,LabelType> spc;
 
    //Take neighborhood iterator on labels
    const typename NeighborhoodLabelIteratorType::RadiusType radius = {{1,1}};
    NeighborhoodLabelIteratorType labIt(radius,this->GetOutput(),m_Tile.region);
    itk::ImageRegionConstIteratorWithIndex<VectorImageType> vitRegion(this->GetInput(),m_Tile.region);

    while(firstLoop || (residual > m_Threshold && iteration < m_MaxIterationNumber))
      {
	std::cout<<"Iteration "<<iteration<<", starting optimisation ..."<<std::endl;  
	for(vitRegion.GoToBegin(),labIt.GoToBegin();!vitRegion.IsAtEnd() && !labIt.IsAtEnd();++vitRegion,++labIt)
	  {
	    
	    if(!inImageCorner(corners,vitRegion.GetIndex()))
	      {
		const LabelType currentLabel = labIt.GetCenterPixel();
		if(currentCount[currentLabel] > 1)
		  {
		    if(!stableSegments || stableLabels.find(currentLabel) == stableLabels.end())
		      {
			//Copy 4 connected unique labels into std::vector
			NeighborhoodLabelsType neighborLabels;
			const NeighborhoodType n = labIt.GetNeighborhood();
			typename NeighborhoodType::ConstIterator ni;
			for(ni = n.Begin()+1 ;ni != n.End(); ni=ni+2)
			  {
			
			    if(*ni != 0 && (!stableSegments || stableLabels.find(*ni) == stableLabels.end()))
			      {
				if(std::find(neighborLabels.begin(), neighborLabels.end(), *ni) == neighborLabels.end())
				  {
				
				    neighborLabels.push_back(*ni);
				  }
			      }
			  }
			int iMin=-1;
			double distanceMin = std::numeric_limits<double>::max();
			if(neighborLabels.size() > 1)
			  {
			    bool topo_check = true;
			    //Point must be simple wrt all the 4 connected labeling neighbors
			    const unsigned int neighborLabelsSize  = neighborLabels.size();
			    for(unsigned int i = 0; i < neighborLabelsSize; i++)
			      {
				topo_check&=spc.isSimpleLabel(n.Begin(), n.End(), neighborLabels[i]);
			      }
			    if(topo_check)
			      {
				//Possible label change

				for(unsigned int i = 0; i < neighborLabelsSize; i++)
				  {
				    //Find closest label
				    double spectralDistance  = 0;
				    double spatialDistance   = 0;
				    const SampleType currentSeed = currentSeedMeans[neighborLabels[i]];
				    for(unsigned int comp = 0; comp<nbComps;++comp)
				      {
					spectralDistance+=(vitRegion.Get()[comp]-currentSeed[comp])*(vitRegion.Get()[comp]-currentSeed[comp]);
				      }
				    spatialDistance += (vitRegion.GetIndex()[0]-currentSeed[nbComps])*(vitRegion.GetIndex()[0]-currentSeed[nbComps]);
				    spatialDistance += (vitRegion.GetIndex()[1]-currentSeed[nbComps+1])*(vitRegion.GetIndex()[1]-currentSeed[nbComps+1]);
				    const double distance = vcl_sqrt(spectralDistance)/nbComps + m_SpatialDistanceWeight * vcl_sqrt(spatialDistance)/2;
				    if (distance < distanceMin)
				      {
					iMin=i;
					distanceMin=distance;
				      }
				  }
			      }
			  }
			if (iMin >= 0 && neighborLabels[iMin] != currentLabel)
			  {
			    //Change label
			    const LabelType newLabel = neighborLabels[iMin];
			    labIt.SetCenterPixel(newLabel);
			
			    //Update seeds
			    //Remove point at currentlabel from means
			    removeFromCentroid(vitRegion.Get(),vitRegion.GetIndex(),currentLabel,currentCount,currentSeedMeans);
			    //neighborLabel[iMin]
			    //Add point at newLabel in means
			    addToCentroid(vitRegion.Get(),vitRegion.GetIndex(),newLabel,currentCount,currentSeedMeans);
			  }
		      }
		  }
	      }
	  }
	//Compute residual
	residual = 0;
	//Update initialSeedMeans
	typename ListSampleType::const_iterator newSeed = currentSeedMeans.begin();
	typename ListSampleType::iterator oldSeed = initialSeedMeans.begin();
	for(;newSeed != currentSeedMeans.end() && oldSeed != initialSeedMeans.end();++newSeed,++oldSeed)
	  {
	    //spectral residual
	    for(unsigned int i = 0; i < nbComps ;++i)
	      {
		residual+=vcl_abs(newSeed->second[i] - oldSeed->second[i])/nbComps;
	      }
	    //spatial residual	    
	    for(unsigned int i = nbComps; i < nbComps+2 ;++i)
	      {
		residual+=vcl_abs(newSeed->second[i] - oldSeed->second[i])/2;
	      }
	    
	    //Update initialSeedMeans
	    oldSeed->second=newSeed->second;
	  }
	residual/=currentSeedMeans.size();
	std::cout<<"Done. Residual: "<<residual<<", max remaining iterations: "<<m_MaxIterationNumber-iteration<<std::endl;

	//Update count
	typename CountContainerType::const_iterator newSeedCount = currentCount.begin();
	typename CountContainerType::iterator oldSeedCount = initialCount.begin();
	for(;newSeedCount != currentCount.end() && oldSeedCount != initialCount.end(); ++newSeedCount,++oldSeedCount)
	  {
	    oldSeedCount->second = newSeedCount->second;
	  }
	firstLoop = false;
	++iteration;
	  
      }

    return;    
  }


} //end namespace otb
#endif

