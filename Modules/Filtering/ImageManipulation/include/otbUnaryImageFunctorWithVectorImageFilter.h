/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbUnaryImageFunctorWithVectorImageFilter_h
#define __otbUnaryImageFunctorWithVectorImageFilter_h

#include "otbVectorImage.h"
#include "itkVariableLengthVector.h"
#include "itkUnaryFunctorImageFilter.h"

namespace otb
{
/** \class UnaryImageFunctorWithVectorImageFilter
 *  \brief Applies a functor to a VectorImage
 *
 *  The aim of the class is to work with vector images but with a functor
 *  that uses as input a component of the pixel.
 *
 *  For N components pixel, an array of functor is defined and each is applied to
 *  the corresponding component of the output pixel.
 *
 * \ingroup Functor
 * \ingroup VectorImage
 */
template <class TInputImage, class TOutputImage, class TFunction>
class ITK_EXPORT UnaryImageFunctorWithVectorImageFilter : public itk::InPlaceImageFilter<TInputImage, TOutputImage>

{
public:
  /** Standard class typedefs. */
  typedef UnaryImageFunctorWithVectorImageFilter             Self;
  typedef itk::InPlaceImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /**   Extract input and output images dimensions.*/
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** "typedef" to simplify the variables definition and the declaration. */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;
  typedef TFunction    FunctorType;

  /** Supported images definition. */
  typedef typename InputImageType::PixelType          InputPixelType;
  typedef typename InputImageType::InternalPixelType  InputInternalPixelType;
  typedef typename InputImageType::RegionType         InputImageRegionType;
  typedef typename OutputImageType::PixelType         OutputPixelType;
  typedef typename OutputImageType::InternalPixelType OutputInternalPixelType;
  typedef typename OutputImageType::RegionType        OutputImageRegionType;

  /** Image size "typedef" definition. */
  typedef typename InputImageType::SizeType SizeType;

  typedef std::vector<FunctorType> FunctorVectorType;

  /** return class name. */
  itkTypeMacro(UnaryImageFunctorWithVectorImageFilter, itk::InPlaceImageFilter);

  /** Get the functor list */
  FunctorVectorType& GetFunctorVector(void)
  {
    return m_FunctorVector;
  }

protected:
  UnaryImageFunctorWithVectorImageFilter();
  virtual ~UnaryImageFunctorWithVectorImageFilter() {}

  /** UnaryImageFunctorWithVectorImageFilter can produce an image which is a different
   * resolution than its input image.  As such, UnaryImageFunctorWithVectorImageFilter
   * needs to provide an implementation for
   * GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below.
   *
   * \sa ProcessObject::GenerateOutputInformaton()  */
  virtual void GenerateOutputInformation();

  /** UnaryImageFunctorWithVectorImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  /** Ponderation declaration*/
  FunctorVectorType m_FunctorVector;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbUnaryImageFunctorWithVectorImageFilter.txx"
#endif

#endif
