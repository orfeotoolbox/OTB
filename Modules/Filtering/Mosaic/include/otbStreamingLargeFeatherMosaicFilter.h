#ifndef __StreamingLargeFeatherMosaicFilter_H
#define __StreamingLargeFeatherMosaicFilter_H

#include "otbStreamingMosaicFilterWithBlendingBase.h"
#include "itkLinearInterpolateImageFunction.h"

namespace otb
{
/** \class StreamingLargeFeatherMosaicFilter
 * \brief Computes the mosaic of an input images set using feathering technique.
 * The output is a nice seamless mosaic. Images are all added-weighted on all
 * overlapping areas. This could give nice results for perfectly aligned
 * images but can also cause blur on misaligned overlaps.
 *
 * Inputs are images and distance images. Both needs to be added with the PushBackInputs() method.
 *
 * Support streaming
 *
 * The pixels must support the operator ==, +, /, etc.
 * The "no data value", output spacing, interpolator can be chosen.
 *
 * TODO:
 * -maybe change containers from std::vector to itk::something?
 *
 * \ingroup OTBMosaic
 */
template <class TInputImage, class TOutputImage, class TDistanceImage, class TInternalValueType=double>
class ITK_EXPORT StreamingLargeFeatherMosaicFilter :
  public StreamingMosaicFilterWithBlendingBase<TInputImage, TOutputImage, TDistanceImage, TInternalValueType>
{
public:

  /** Standard Self typedef */
  typedef StreamingLargeFeatherMosaicFilter Self;
  typedef StreamingMosaicFilterWithBlendingBase<TInputImage, TOutputImage, TDistanceImage,
                                                TInternalValueType> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>
    ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(StreamingLargeFeatherMosaicFilter, StreamingMosaicFilterBlendingBase);

  /** Input image typedefs.  */
  typedef typename Superclass::InputImageType              InputImageType;
  typedef typename Superclass::InputImagePointer           InputImagePointer;
  typedef typename Superclass::InputImagePointType         InputImagePointType;
  typedef typename Superclass::InputImagePixelType         InputImagePixelType;
  typedef typename Superclass::InputImageIndexType         InputImageIndexType;
  typedef typename Superclass::InputImageSizeType          InputImageSizeType;
  typedef typename Superclass::InputImageSpacingType       InputImageSpacingType;
  typedef typename Superclass::InputImageInternalPixelType InputImageInternalPixelType;
  typedef typename Superclass::InputImageRegionType        InputImageRegionType;

  /** Output image typedefs.  */
  typedef typename Superclass::OutputImageType              OutputImageType;
  typedef typename Superclass::OutputImagePointer           OutputImagePointer;
  typedef typename Superclass::OutputImagePointType         OutputImagePointType;
  typedef typename Superclass::OutputImagePixelType         OutputImagePixelType;
  typedef typename Superclass::OutputImageIndexType         OutputImageIndexType;
  typedef typename Superclass::OutputImageSizeType          OutputImageSizeType;
  typedef typename Superclass::OutputImageSpacingType       OutputImageSpacingType;
  typedef typename Superclass::OutputImageInternalPixelType OutputImageInternalPixelType;
  typedef typename Superclass::OutputImageRegionType        OutputImageRegionType;

  /** Internal computing typedef support. */
  typedef typename Superclass::InternalValueType       InternalValueType;
  typedef typename Superclass::ContinuousIndexType     ContinuousIndexType;
  typedef typename Superclass::InterpolatorType        InterpolatorType;
  typedef typename Superclass::InterpolatorPointerType InterpolatorPointerType;
  typedef typename Superclass::DefaultInterpolatorType DefaultInterpolatorType;
  typedef typename Superclass::InternalImageType       InternalImageType;
  typedef typename Superclass::InternalPixelType       InternalPixelType;
  typedef typename Superclass::IteratorType            IteratorType;
  typedef typename Superclass::ConstIteratorType       ConstIteratorType;
  typedef typename Superclass::StreamingTraitsType     StreamingTraitsType;

  /** Distance image typedefs */
  typedef TDistanceImage                                            DistanceImageType;
  typedef typename Superclass::DistanceImagePointer                 DistanceImagePointer;
  typedef typename Superclass::DistanceImageConstPointer            DistanceImageConstPointer;
  typedef typename Superclass::DistanceImagePointType               DistanceImagePointType;
  typedef typename Superclass::DistanceImagePixelType               DistanceImagePixelType;
  typedef typename Superclass::DistanceImageInternalPixelType       DistanceImageInternalPixelType;
  typedef typename Superclass::DistanceImageInterpolatorType        DistanceImageInterpolatorType;
  typedef typename Superclass::DistanceImageDefaultInterpolatorType DistanceImageDefaultInterpolatorType;
  typedef typename Superclass::DistanceImageInterpolatorPointer     DistanceImageInterpolatorPointer;

protected:
  StreamingLargeFeatherMosaicFilter() {
  }

  virtual ~StreamingLargeFeatherMosaicFilter() {
  }

  /** Overrided method */
  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId );

}; // end of class

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingLargeFeatherMosaicFilter.txx"
#endif

#endif
