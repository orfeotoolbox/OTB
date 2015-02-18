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
#ifndef __otbLineDetectorImageFilterBase_h
#define __otbLineDetectorImageFilterBase_h

#include "itkNearestNeighborInterpolateImageFunction.h"
#include "itkLinearInterpolateImageFunction.h"
#include "otbImageToModulusAndDirectionImageFilter.h"
#include "otbImage.h"
#include "itkNumericTraits.h"

namespace otb
{

/** \class LineDetectorImageFilterBase
 *
 * \brief Base class for line detector filters.
 *
 * This is the base class for the hierarchy of line detector
 * filters. Line detectors work by detecting 2 parallel edges for
 * a given set of directions. This class implements the
 * ThreadedGenerateData"()" method which, for every pixel and for
 * every direction, builds three sets of pixels (left, center and
 * right with respect to the reference direction). The
 * ThreadedGenerateData"()" method calls the virtual
 * ComputeMeasure"()" method which implements the detection. This
 * method should be overloaded by each specific line detector.
 *
 * The output is an image of intensity of detection and an image of
 * direction of the line fo each pixel.
 *
 */

template <class TInputImage,
    class TOutputImage,
    class TOutputImageDirection = TOutputImage,
    class TInterpolator = itk::LinearInterpolateImageFunction<TInputImage> >
class ITK_EXPORT LineDetectorImageFilterBase :  public ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage,
      TOutputImageDirection>
{
public:
  /**   Extract dimensions as well of the images of entry of exit. */
  itkStaticConstMacro(InputImageDimension,
                      unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension,
                      unsigned int,
                      TOutputImage::ImageDimension);

  /** typedef for the classes standards. */
  typedef LineDetectorImageFilterBase                                                             Self;
  typedef ImageToModulusAndDirectionImageFilter<TInputImage, TOutputImage, TOutputImageDirection> Superclass;
  typedef itk::SmartPointer<Self>                                                                 Pointer;
  typedef itk::SmartPointer<const Self>                                                           ConstPointer;

  /** Method for management of the object factory. */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(LineDetectorImageFilterBase, ImageToModulusAndDirectionImageFilter);

  typedef typename Superclass::InputImageType           InputImageType;
  typedef typename Superclass::OutputImageType          OutputImageType;
  typedef typename Superclass::OutputImageDirectionType OutputImageDirectionType;
  typedef TInterpolator                                 InterpolatorType;

  /** Typedefs to describe and access Interpolator */
  typedef typename InterpolatorType::Pointer      InterpolatorPointer;
  typedef typename InterpolatorType::CoordRepType CoordRepType;

  typedef typename InputImageType::PointType TPoint;

  /** Definition of the input and output images */
  typedef typename InputImageType::PixelType  InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;
  typedef typename OutputImageType::Pointer   OutputImagePointerType;

  typedef typename InputImageType::RegionType  InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  /** Definition of the size of the images. */
  typedef typename InputImageType::SizeType SizeType;

  /** Set the length of the linear feature. */
  itkSetMacro(LengthLine, unsigned int);

  /** Get the length of the linear feature. */
  itkGetConstReferenceMacro(LengthLine, unsigned int);

  /** Set the width of the linear feature. */
  itkSetMacro(WidthLine, unsigned int);

  /** Get the length of the linear feature. */
  itkGetConstReferenceMacro(WidthLine, unsigned int);

  /** Set the radius of one zone. */
  itkSetMacro(Radius, SizeType);

  /** Get the radius of one zone. */
  itkGetConstReferenceMacro(Radius, SizeType);

  /** Set the radius of one zone. */
  itkSetMacro(Threshold, OutputPixelType);

  /** Get the radius of one zone. */
  itkGetConstReferenceMacro(Threshold, OutputPixelType);

  /** Set the numbero of drections for line detection. */
  itkSetMacro(NumberOfDirections, unsigned int);

  /** Get the numbero of drections for line detection. */
  itkGetConstReferenceMacro(NumberOfDirections, unsigned int);

  virtual void GenerateInputRequestedRegion()
    throw(itk::InvalidRequestedRegionError);

protected:
  LineDetectorImageFilterBase();
  virtual ~LineDetectorImageFilterBase() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void BeforeThreadedGenerateData();

  /** LineDetectorImageFilterBase can be implemented for a treatment of filter multithreaded.
   * Thus, the ThreadedGenerateData() method is called for each thread process.
   * The data image are allocated automatically by the mother class by calling the
   * ThreadedGenerateData() method. ThreadedGenerateData can only write the portion
   * of the image specified by the parameter outputRegionForThread
   *
   * \sa ImageToImageFilter::ThreadedGenerateData()
   * \sa    ImageToImageFilter::GenerateData()
  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId);

  virtual double ComputeMeasure(std::vector<double>* m1, std::vector<double>* m2, std::vector<double>* m3);

  /** Length of the linear feature = 2*m_LengthLine+1 */
  unsigned int m_LengthLine;

  /** Width of the linear feature = 2*m_WidthLine+1 */
  unsigned int m_WidthLine;

  /** Radius of the region*/
  SizeType m_Radius;

  /** Size of the facelist*/
  SizeType m_FaceList;

  OutputPixelType m_Threshold;

  unsigned int m_NumberOfDirections;
private:
  LineDetectorImageFilterBase(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLineDetectorImageFilterBase.txx"
#endif

#endif
