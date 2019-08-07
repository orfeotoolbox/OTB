#ifndef otbHistogramCalculationFilter_h
#define otbHistogramCalculationFilter_h

#include "otbVectorImage.h"
#include "otbImage.h"
#include "itkImageRegionConstIterator.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkConstantBoundaryCondition.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "otbPersistentImageFilter.h"
#include "itkVariableSizeMatrix.h"
#include "itkProgressReporter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "otbExtractROI.h"
#include <unordered_map>

namespace otb{
  template <class TInputImage, class TInputLabel>
  class ITK_EXPORT HistogramCalculationFilter : public PersistentImageFilter<TInputImage,TInputImage>
  {
  public:
    typedef HistogramCalculationFilter                                           Self;
    typedef PersistentImageFilter<TInputImage, TInputImage>                      Superclass;
    typedef itk::SmartPointer<Self>                                              Pointer;
    typedef itk::SmartPointer<const Self>                                        ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Runtime information support. */
    itkTypeMacro(HistogramCalculationFilter, PersistentImageFilter);

    /** ImageDimension constants */
    itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension);
    
    typedef typename TInputLabel::PixelType LabelType;
    typedef TInputLabel LabelImageType;
    typedef itk::ImageRegionIterator<TInputLabel> LabelImageIteratorType;
    typedef typename TInputLabel::Pointer LabelImagePointer;
 
    typedef TInputImage InputImageType;
    typedef typename TInputImage::PixelType InputImagePixelType;
    typedef typename std::vector<double> SampleType;
    typedef itk::ImageRegionIterator<TInputImage> InputImageIteratorType;
    typedef typename TInputImage::Pointer InputImagePointer;
    
    typedef typename TInputImage::RegionType RegionType;
 
    typedef std::unordered_map<LabelType,SampleType> LabeledSampleContainerType;
    typedef typename LabeledSampleContainerType::iterator LabeledSampleContainerIteratorType;
    typedef std::pair<LabelType,SampleType> LabelSamplePairType;
    
    
    typedef std::unordered_map<LabelType, unsigned int> LabeledIntContainerType;
    typedef typename LabeledIntContainerType::iterator LabeledIntContainerIteratorType;
    typedef std::pair<LabelType,unsigned int> LabelIntPairType;
    
    typedef itk::ConstantBoundaryCondition<TInputLabel>                    BoundaryConditionType;

    typedef itk::ExtractImageFilter<InputImageType,InputImageType> ExtractImageFilterType;

    virtual void Reset() ITK_OVERRIDE;
    
    virtual void Synthetize() ITK_OVERRIDE;

    void SetImageSeg(LabelImagePointer im);

    TInputLabel* GetImageSeg();

    LabeledSampleContainerType GetMeans();

    LabeledIntContainerType GetCount();
    
    void SetLabelPositions(LabeledIntContainerType const& lp);
    
    
  protected:

    HistogramCalculationFilter();

    ~HistogramCalculationFilter() ITK_OVERRIDE;
    
    // virtual void BeforeThreadedGenerateData() ITK_OVERRIDE;
    
    virtual void ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId) ITK_OVERRIDE;
    // virtual void AfterThreadedGenerateData() ITK_OVERRIDE;

    // virtual void GenerateInputRequestedRegion() ITK_OVERRIDE;
  
  private:
    HistogramCalculationFilter(const Self&);//purposely not implemented
    void operator =(const Self&); //purposely not implemented

    
    void FuseMeansContainers(LabeledSampleContainerType & means1, LabeledSampleContainerType means2) const;
    
    void FuseCountContainers(LabeledIntContainerType & count1, LabeledIntContainerType count2) const ;    
   
    // std::vector<LabeledSampleContainerType> m_TemporaryMeans;
    LabeledSampleContainerType m_Means;

    // std::vector<LabeledIntContainerType> m_TemporaryCount;
    LabeledIntContainerType m_Count;

    unsigned m_NLabels;

    LabeledIntContainerType m_LabelPositions;

  };
  
  template <class TInputImage, class TInputLabel>
  class ITK_EXPORT StreamingHistogramCalculationFilter : public PersistentFilterStreamingDecorator<HistogramCalculationFilter<TInputImage,TInputLabel> >
  {
  public:
    typedef StreamingHistogramCalculationFilter                                                                     Self;
    typedef PersistentFilterStreamingDecorator<HistogramCalculationFilter<TInputImage,TInputImage> >                Superclass;
    typedef itk::SmartPointer<Self>                                              Pointer;
    typedef itk::SmartPointer<const Self>                                        ConstPointer;

    typedef typename std::vector<double> SampleType;
    typedef typename TInputLabel::PixelType LabelType;
    typedef std::unordered_map<LabelType,SampleType> LabeledSampleContainerType;
    typedef typename LabeledSampleContainerType::iterator LabeledSampleContainerIteratorType;
    typedef std::pair<LabelType,SampleType> LabelSamplePairType;

    typedef std::unordered_map<LabelType, unsigned int> LabeledIntContainerType;
    typedef typename LabeledIntContainerType::iterator LabeledIntContainerIteratorType;
    typedef std::pair<LabelType,unsigned int> LabelIntPairType;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Runtime information support. */
    itkTypeMacro(StreamingHistogramCalculationFilter, PersistentFilterStreamingDecorator);

    /** ImageDimension constants */
    itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension);


    /** Set the input image. */
    using Superclass::SetInput;
    void SetInput(TInputImage * input)
    {
      this->GetFilter()->SetInput(input);
    }
    const TInputImage * GetInput()
    {
      return this->GetFilter()->GetInput();
    }

    void SetImageSeg(TInputLabel * input)
    {
      this->GetFilter()->SetImageSeg(input);
    }

    void SetLabelPositions(LabeledIntContainerType const& input)
    {
      this->GetFilter()->SetLabelPositions(input);
    }

    LabeledSampleContainerType GetMeans()
    {
      std::cout << "befGet" << "\n";
      return this->GetFilter()->GetMeans();
    }
    LabeledIntContainerType GetCount()
    {
      std::cout << "befGetc" << "\n";
      return this->GetFilter()->GetCount();
    }

  protected:
    StreamingHistogramCalculationFilter() {};

    ~StreamingHistogramCalculationFilter() ITK_OVERRIDE {};
        
  private:
    StreamingHistogramCalculationFilter(const Self&);//purposely not implemented
    void operator =(const Self&); //purposely not implemented

    
  };

  
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHistogramCalculationFilter.txx"
#endif

#endif
