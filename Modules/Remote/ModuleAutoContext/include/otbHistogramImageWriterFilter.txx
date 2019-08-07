#ifndef otbHistogramImageWriterFilter_txx
#define otbHistogramImageWriterFilter_txx

#include "otbHistogramImageWriterFilter.h"
namespace otb{

  template <class TInputLabel, class TOutputImage>
  HistogramImageWriterFilter<TInputLabel, TOutputImage>
  ::HistogramImageWriterFilter()
  {
    std::cout << "in const" << "\n";
    this->SetNumberOfRequiredInputs(1);
  }

  template <class TInputLabel, class TOutputImage>
  HistogramImageWriterFilter<TInputLabel, TOutputImage>
  ::~HistogramImageWriterFilter()
  {
    // std::free(m_Means);
    // std::free(m_Count);
  }
 

  template <class TInputLabel, class TOutputImage>
  TOutputImage*
  HistogramImageWriterFilter<TInputLabel, TOutputImage>
  ::GetOutput()
  {
    return dynamic_cast<TOutputImage *>(this->itk::ProcessObject::GetOutput(0));
  }
  template <class TInputLabel, class TOutputImage>
  const TOutputImage*
  HistogramImageWriterFilter<TInputLabel, TOutputImage>
  ::GetOutput() const
  {
    return dynamic_cast<TOutputImage *>(this->itk::ProcessObject::GetOutput(0));
  }

  template <class TInputLabel, class TOutputImage>
  void
  HistogramImageWriterFilter<TInputLabel, TOutputImage>
  ::SetMeans(LabeledSampleContainerType const& m)
  {
    m_Means = m;
  }

    /**
   * Generate the output information
   */
  template <class TInputLabel, class TOutputImage>
  void
  HistogramImageWriterFilter<TInputLabel, TOutputImage>
  ::GenerateOutputInformation()
  {
    std::cout << "GOI 1" << "\n";
    Superclass::GenerateOutputInformation();
    std::cout << "GOI 2" << "\n";
    m_NLabels=m_Means.begin()->second.size();
    std::cout << "GOI 3" << "\n";
    this->GetOutput()->SetNumberOfComponentsPerPixel(m_NLabels);
    std::cout << "GOI 4" << "\n";
  }
  
  template <class TInputLabel, class TOutputImage>
  void
  HistogramImageWriterFilter<TInputLabel, TOutputImage>
  ::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
  {
    // std::cout<< "Start thread " << threadId << " " <<outputRegionForThread<< std::endl;
    
    
    typename TOutputImage::Pointer outputPtr = this->GetOutput();
    
    itk::ImageRegionIterator<TOutputImage> outIt(outputPtr,outputRegionForThread);
    itk::ImageRegionConstIterator<TInputLabel> inIt(this->GetInput(),outputRegionForThread);

    for (outIt.GoToBegin(),inIt.GoToBegin();!outIt.IsAtEnd() && !inIt.IsAtEnd();++outIt,++inIt) {
      const LabelType l = inIt.Get();
      typename TOutputImage::PixelType p(m_NLabels);
      const std::vector<double> histo = m_Means.find(l)->second;
      for (unsigned i = 0; i < m_NLabels; i++) {
	p[i]=histo[i];
      }
      outIt.Set(p);
    }
    

  }

}
#endif
