#ifndef otbHistogramImageWriterFilter_h
#define otbHistogramImageWriterFilter_h

#include "otbVectorImage.h"
#include "otbImage.h"
#include "itkImageRegionConstIterator.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkConstantBoundaryCondition.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkImageToImageFilter.h"
#include "otbPersistentImageFilter.h"
#include "itkVariableSizeMatrix.h"
#include "itkProgressReporter.h"
#include "otbPersistentFilterStreamingDecorator.h"

#include <unordered_map>

namespace otb{
  template <class TInputLabel, class TOutputImage>
  class ITK_EXPORT HistogramImageWriterFilter : public itk::ImageToImageFilter<TInputLabel,TOutputImage>
  {
  public:
    typedef HistogramImageWriterFilter                                                    Self;
    typedef itk::ImageToImageFilter<TInputLabel,TOutputImage>                 Superclass;
    typedef itk::SmartPointer<Self>                                              Pointer;
    typedef itk::SmartPointer<const Self>                                        ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Runtime information support. */
    itkTypeMacro(HistogramImageWriterFilter, ImageToImageFilter);

    /** ImageDimension constants */
    itkStaticConstMacro(ImageDimension, unsigned int, TInputLabel::ImageDimension);
    
    typedef typename TInputLabel::PixelType LabelType;
    typedef TInputLabel LabelImageType;
    typedef itk::ImageRegionIterator<TInputLabel> LabelImageIteratorType;
    typedef typename TInputLabel::Pointer LabelImagePointer;
    
    typedef typename TInputLabel::RegionType RegionType;

    typedef std::vector<double> SampleType;
    typedef std::unordered_map<LabelType,SampleType > LabeledSampleContainerType;
    typedef typename LabeledSampleContainerType::iterator LabeledSampleContainerIteratorType;
    typedef std::pair<LabelType,SampleType> LabelSamplePairType;
    
    
    typedef std::unordered_map<LabelType, unsigned int> LabeledIntContainerType;
    typedef typename LabeledIntContainerType::iterator LabeledIntContainerIteratorType;
    typedef std::pair<LabelType,unsigned int> LabelIntPairType;
    
    typedef itk::ConstantBoundaryCondition<TInputLabel>                    BoundaryConditionType;
    // typedef itk::ConstNeighborhoodIterator<TInputLabel,BoundaryConditionType>   NeighborhoodLabelIteratorType;
    // typedef typename NeighborhoodLabelIteratorType::NeighborhoodType NeighborhoodType;

    TOutputImage* GetOutput();
    
    const TOutputImage* GetOutput() const;

    void SetMeans(LabeledSampleContainerType const& m);

  protected:
    HistogramImageWriterFilter();

    ~HistogramImageWriterFilter() ITK_OVERRIDE;
    
    void GenerateOutputInformation() ITK_OVERRIDE;
    
    virtual void ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId) ITK_OVERRIDE;

  private:
    HistogramImageWriterFilter(const Self&);//purposely not implemented
    
    void operator =(const Self&); //purposely not implemented
   
    LabeledSampleContainerType m_Means;

    unsigned m_NLabels;

  };

  
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHistogramImageWriterFilter.txx"
#endif

#endif

