#ifndef __otbDifferenceImageFilter_h
#define __otbDifferenceImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkNumericTraits.h"
#include "itkArray.h"

namespace otb
{

/** \class DifferenceImageFilter
 * \brief Implements comparison between two images.
 *
 * This filter is used by the testing system to compute the difference between
 * a valid image and an image produced by the test. The comparison value is
 * computed by visiting all the pixels in the baseline images and comparing
 * their values with the pixel values in the neighborhood of the homologous
 * pixel in the other image.
 *
 * \ingroup IntensityImageFilters   Multithreaded
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT DifferenceImageFilter :
  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef DifferenceImageFilter                              Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DifferenceImageFilter, ImageToImageFilter);

  /** Some convenient typedefs. */
  typedef TInputImage                                            InputImageType;
  typedef TOutputImage                                           OutputImageType;
  typedef typename OutputImageType::PixelType                    OutputPixelType;
  typedef typename OutputImageType::RegionType                   OutputImageRegionType;
  typedef typename itk::NumericTraits<OutputPixelType>::RealType RealType;
  typedef typename itk::NumericTraits<RealType>::AccumulateType  AccumulateType;
  typedef typename RealType::RealValueType                       ScalarRealType;

  /** Set the valid image input.  This will be input 0.  */
  virtual void SetValidInput(const InputImageType* validImage);

  /** Set the test image input.  This will be input 1.  */
  virtual void SetTestInput(const InputImageType* testImage);

  /** Set/Get the maximum distance away to look for a matching pixel.
      Default is 0. */
  itkSetMacro(ToleranceRadius, int);
  itkGetMacro(ToleranceRadius, int);

  /** Set/Get the minimum threshold for pixels to be different (relative).
      Default is 0. */
  itkSetMacro(DifferenceThreshold, ScalarRealType);
  itkGetMacro(DifferenceThreshold, ScalarRealType);

  /** Get parameters of the difference image after execution.  */
  itkGetMacro(MeanDifference, RealType);
  itkGetMacro(TotalDifference, RealType);
  itkGetMacro(NumberOfPixelsWithDifferences, unsigned long);

protected:
  DifferenceImageFilter();
  virtual ~DifferenceImageFilter() {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** DifferenceImageFilter can be implemented as a multithreaded
   * filter.  Therefore, this implementation provides a
   * ThreadedGenerateData() routine which is called for each
   * processing thread. The output image data is allocated
   * automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to
   * the portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& threadRegion,
                            itk::ThreadIdType threadId);

  void BeforeThreadedGenerateData();
  void AfterThreadedGenerateData();
  virtual void GenerateOutputInformation();

  ScalarRealType m_DifferenceThreshold;
  RealType       m_MeanDifference;
  RealType       m_TotalDifference;
  unsigned long  m_NumberOfPixelsWithDifferences;
  int            m_ToleranceRadius;

  std::vector<RealType>     m_ThreadDifferenceSum;
  itk::Array<unsigned long> m_ThreadNumberOfPixels;

private:
  DifferenceImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDifferenceImageFilter.txx"
#endif

#endif
