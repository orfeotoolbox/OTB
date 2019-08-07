#ifndef otbHistogramCalculationFilter_txx
#define otbHistogramCalculationFilter_txx

#include "otbHistogramCalculationFilter.h"
namespace otb{

  template <class TInputImage, class TInputLabel>
  HistogramCalculationFilter<TInputImage, TInputLabel>
  ::HistogramCalculationFilter()
  {
    this->SetNumberOfRequiredInputs(2);
    m_Count.reserve(250000);
    m_Means.reserve(250000);
  }

  template <class TInputImage, class TInputLabel>
  HistogramCalculationFilter<TInputImage, TInputLabel>
  ::~HistogramCalculationFilter()
  {
  }
  
  template <class TInputImage, class TInputLabel>
  void
  HistogramCalculationFilter<TInputImage, TInputLabel>
  ::SetImageSeg(LabelImagePointer im){
    this->SetNthInput(1,im);
  }
  template <class TInputImage, class TInputLabel>
  TInputLabel *
  HistogramCalculationFilter<TInputImage, TInputLabel>
  ::GetImageSeg() {
    return dynamic_cast<TInputLabel *>(this->itk::ProcessObject::GetInput(1));
  }
  template <class TInputImage, class TInputLabel>
  typename HistogramCalculationFilter<TInputImage, TInputLabel>::LabeledSampleContainerType
  HistogramCalculationFilter<TInputImage, TInputLabel>
  ::GetMeans() {
    return m_Means;
  }
  
  template <class TInputImage, class TInputLabel>
  typename HistogramCalculationFilter<TInputImage, TInputLabel>::LabeledIntContainerType
  HistogramCalculationFilter<TInputImage, TInputLabel>
  ::GetCount() {
    return m_Count;
  }

  template <class TInputImage, class TInputLabel>
  void
  HistogramCalculationFilter<TInputImage, TInputLabel>
  ::SetLabelPositions(LabeledIntContainerType const& lp){
    m_LabelPositions = lp;
  }
  
  template <class TInputImage, class TInputLabel>
  void
  HistogramCalculationFilter<TInputImage, TInputLabel>
  ::FuseMeansContainers(LabeledSampleContainerType & means1, LabeledSampleContainerType means2) const
  {
    typename LabeledSampleContainerType::const_iterator m2 = means2.begin();
    for(;m2 != means2.end();++m2)
      {
    	const LabelType currentLabel = m2->first;
    	LabeledSampleContainerIteratorType m1 = means1.find(currentLabel);
    	if(m1 == means1.end())
    	  {
    	    //new label
    	    means1.insert(*m2);
    	  }
    	else
    	  {
    	    // //Fuse old and new label contents
    	    for(unsigned int i = 0 ; i < m_NLabels ; ++i)
    	      {
    	    	m1->second[i]+=m2->second[i];
    	      }
    	  }
      }
  }
  template <class TInputImage, class TInputLabel>
  void
  HistogramCalculationFilter<TInputImage, TInputLabel>
  ::FuseCountContainers(LabeledIntContainerType & count1, LabeledIntContainerType count2) const
  {
    typename LabeledIntContainerType::const_iterator c2 = count2.begin();
    for(;c2 != count2.end();++c2)
      {
  	const LabelType currentLabel = c2->first;
  	LabeledIntContainerIteratorType c1 = count1.find(currentLabel);
  	if(c1 == count1.end())
  	  {
  	    //new label
  	    count1.insert(*c2);
  	  }
  	else
  	  {
  	    //Fuse old and new label contents
  	    c1->second+=c2->second;
  	  }
      }
  }
  
 
  

  template <class TInputImage, class TOutputLabelImage>
  void
  HistogramCalculationFilter<TInputImage, TOutputLabelImage>
  ::Reset()
  {
    m_NLabels = m_LabelPositions.size();
  }
  
  template <class TInputImage, class TInputLabel>
  void
  HistogramCalculationFilter<TInputImage, TInputLabel>
  ::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
  {
    m_NLabels = m_LabelPositions.size();
    std::cout<< "HistogramCalculationFilter Start thread " << threadId << " " <<outputRegionForThread<< std::endl;

    // typename ExtractImageFilterType::Pointer labelImageROI = ExtractImageFilterType::New();
    // // labelImageROI = ExtractROIFilterType::New();
    // labelImageROI->SetInput(this->GetInput());
    // labelImageROI->SetExtractionRegion(outputRegionForThread);
    // labelImageROI->Update();
    
    itk::ImageRegionConstIterator<InputImageType> vit(this->GetInput(),outputRegionForThread);

   
    itk::ImageRegionConstIterator<TInputLabel> lit(GetImageSeg(),outputRegionForThread);
   
    // LabeledIntContainerType count;

    // LabeledSampleContainerType means;

    for(vit.GoToBegin(),lit.GoToBegin();!vit.IsAtEnd() && !lit.IsAtEnd();++vit,++lit)
      {	  
	const LabelType label = lit.Get();	
	LabeledIntContainerIteratorType search = m_Count.find(label);
	if(search == m_Count.end())	  
	  {
	    // Label is not in list	    
	    // Create new element in m_Count
	    m_Count.insert(LabelIntPairType(label,1));

	    // Create new element in means	    
	    SampleType s(m_NLabels,0.0);
	    s[m_LabelPositions[vit.Get()]]+=1;
	    m_Means.insert(LabelSamplePairType(label,s));
	  }
	else
	  {
	    //Element is already in list
	    //Update with running formulas
	    //Update m_Count
	    search->second++;
	    
	    //Update Means
	    m_Means.find(label)->second[m_LabelPositions[vit.Get()]]+=1;
	  }
      }

    // FuseCountContainers(m_Count,count);
    // FuseMeansContainers(m_Means,means);
  }

  template <class TInputImage, class TOutputLabelImage>
  void
  HistogramCalculationFilter<TInputImage, TOutputLabelImage>
  ::Synthetize()
  {
    std::cout << "here" << "\n";
    for (auto & e : m_Means){
      const unsigned c = m_Count[e.first];
      for (unsigned i = 0; i < m_NLabels; i++) {
	if (c>0) {
	  e.second[i] /= c;
	}
	else{
	  e.second[i] = 0;
	}
      }
      
    }
    // for (auto & e : m_Means) {
    //   std::cout << e.first << " ";
    //   for (unsigned i = 0; i < m_NLabels; i++) {
    // 	std::cout << e.second[i] << " ";
    //   }
    //   std::cout << "\n";
    // // }
    // std::cout << "there" << "\n";
    // std::cout << m_Means.size() << "\n";
    // std::cout << m_Means.max_size() << "\n";
  }
  
}
#endif
