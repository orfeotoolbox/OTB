#ifndef __StreamingSimpleMosaicFilter_H
#define __StreamingSimpleMosaicFilter_H

#include "otbStreamingMosaicFilterBase.h"

namespace otb
{
/** \class StreamingSimpleMosaicFilter
 * \brief Computes the mosaic of an input images set.
 * The output pixel value is equal to the first input pixel of the stack
 * which is different from the Superclass::GetNoDataInputPixel()
 *
 * Support streaming
 *
 * The pixels must support the operator ==, +, /, etc.
 * The "no data value", output spacing, interpolator can be chosen.
 * The behavior of the filter is to put layers in the same order
 * as they are in input
 *
 * \ingroup OTBMosaic
 */
template <class TInputImage, class TOutputImage=TInputImage, class TInternalValueType=double>
class ITK_EXPORT StreamingSimpleMosaicFilter : public otb::StreamingMosaicFilterBase<TInputImage, TOutputImage,
                                                                                     TInternalValueType>
{
public:

  /** Standard Self typedef */
  typedef StreamingSimpleMosaicFilter                                                   Self;
  typedef otb::StreamingMosaicFilterBase<TInputImage, TOutputImage, TInternalValueType> Superclass;
  typedef itk::SmartPointer<Self>                                                       Pointer;
  typedef itk::SmartPointer<const Self>                                                 ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(StreamingSimpleMosaicFilter, StreamingMosaicFilterBase);

  /** Input image typedefs.  */
  typedef typename Superclass::InputImageType          InputImageType;
  typedef typename Superclass::InputImagePixelType     InputImagePixelType;
  typedef typename Superclass::IteratorType            IteratorType;
  typedef typename Superclass::InterpolatorPointerType InterpolatorPointerType;
  typedef typename Superclass::InputImageRegionType    InputImageRegionType;

  /** Output image typedefs.  */
  typedef typename Superclass::OutputImageType              OutputImageType;
  typedef typename Superclass::OutputImagePointType         OutputImagePointType;
  typedef typename Superclass::OutputImagePixelType         OutputImagePixelType;
  typedef typename Superclass::OutputImageInternalPixelType OutputImageInternalPixelType;
  typedef typename Superclass::OutputImageRegionType        OutputImageRegionType;

  /** Internal computing typedef support. */
  typedef typename Superclass::InternalValueType InternalValueType;
  typedef typename Superclass::InternalPixelType InternalPixelType;

protected:
  StreamingSimpleMosaicFilter() {
  }

  virtual ~StreamingSimpleMosaicFilter() {
  }

  /** Overrided methods */
  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId );

private:
  StreamingSimpleMosaicFilter(const Self&); //purposely not implemented
  void operator=(const Self&);              //purposely not implemented

}; // end of class

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingSimpleMosaicFilter.hxx"
#endif

#endif
