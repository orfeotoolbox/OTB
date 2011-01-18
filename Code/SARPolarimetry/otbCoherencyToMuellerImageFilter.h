/*=========================================================================

  Program:   OTB
  Module:    $otbStokesToMCLImageFilter.h$
  Language:  C++
  Date:      $Date: 24/10/2006 $
  Version:   $Revision: 1.0 $

 

=========================================================================*/
#ifndef __CoherencyToMuellerImageFilter_h
#define __CoherencyToMuellerImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkVector.h"

#include "otbImage.h"

namespace otb
 {
/** \class otbCoherencyToMuellerImageFilter
 * \brief Compute the Coherency image (9 channels) from the MLC image (9 channels)
 */
template <class TPixel>
class  CoherencyToMuellerImageFilter :
   public itk::ImageToImageFilter< otb::Image<itk::Vector<TPixel,9>,2>, otb::Image<itk::Vector<TPixel,10>,2> >
{
public:
  /** Convenient typedefs for simplifying declarations. */
  typedef TPixel InputPixelType;
  typedef otb::Image<itk::Vector<TPixel,9>,2> InputImageType;
  typedef otb::Image<itk::Vector<TPixel,10>,2> OutputImageType;

  /** Extract dimension from input and output image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      OutputImageType::ImageDimension);

  
  /** Standard class typedefs. */
  typedef CoherencyToMuellerImageFilter Self;
  typedef itk::ImageToImageFilter< InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(CoherencyToMuellerImageFilter, ImageToImageFilter);
  
  /** Image typedef support. */
  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;
  
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  typedef typename InputImageType::SizeType InputSizeType;
  typedef typename InputImageType::IndexType IndexType ;

  /** Coherency type **/
  typedef itk::Vector<float,9> CoherencyType;
  /** Mueller Type **/
  typedef itk::Vector<float,10> MuellerType;
  
  
  /** CoherencyToMuellerImageFilter needs a larger input requested region than
   * the output requested region.  As such, CoherencyToMuellerImageFilter needs
   * to provide an implementation for GenerateInputRequestedRegion()
   * in order to inform the pipeline execution model.
   *
   * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion() throw(itk::InvalidRequestedRegionError);

protected:
  CoherencyToMuellerImageFilter();
  virtual ~CoherencyToMuellerImageFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** CoherencyToMuellerImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData()
   * routine which is called for each processing thread. The output
   * image data is allocated automatically by the superclass prior to
   * calling ThreadedGenerateData().  ThreadedGenerateData can only
   * write to the portion of the output image specified by the
   * parameter "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

private:
  CoherencyToMuellerImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

 
 

};
  
} // end namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbCoherencyToMuellerImageFilter.txx"
#endif

#endif
