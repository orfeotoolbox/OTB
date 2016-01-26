/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Mines-Telecom. All rights reserved.
  See IMTCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbWaveletTransform_h
#define __otbWaveletTransform_h

#include "otbImageToImageListFilter.h"
#include "otbImageListToImageFilter.h"
#include "otbObjectList.h"
#include "otbWaveletGenerator.h"
#include "otbWaveletOperatorBase.h"

namespace otb {

/** \class WaveletTransform
 * \brief Wavelet transformation framework
 *
 * This class defines the Wavelet transformation of an image
 * by using a (templated) elementary wavelet transform.
 *
 * Here the number of decomposition is given. The output is given
 * in the GetOutput() list of images ranked from the low-pass part to the
 * highest frequency part.
 *
 * Output image organization depends on the number of Dim on the image
 * In the OutputList, the first image is always the low passed one. Next,
 * follow the high Passed images of the lowest decomposition (ask
 * GetFilter(0)->GetNumberOfOutputs() to know the length) and so on until
 * the high passed images of the highest decomposition.
 *
 * \sa WaveletFilterBank
 * \sa WaveletPacketTransform
 *
 *
 * \ingroup OTBWavelet
 */
template <class TInputImage, class TOutputImage, class TFilter,
    Wavelet::WaveletDirection TDirectionOfTransformation>
class ITK_EXPORT WaveletTransform
  : public itk::ImageSource<TOutputImage>
{
public:
  /** Standard typedefs */
  typedef WaveletTransform               Self;
  typedef itk::ImageSource<TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(WaveletTransform, ImageSource);

protected:
  /** But this class cannot do any thing! No constructor is implemented */
  WaveletTransform();
  virtual ~WaveletTransform();

private:
  WaveletTransform (const Self &);
  void operator =(const Self&);
};

/** \class WaveletTransform
 * \brief Forward Wavelet transformation framework
 *
 * This class defines the Wavelet transformation of an image
 * by using a (templated) elementary wavelet transform.
 *
 * Here the number of decomposition is given. The output is given
 * in the GetOutput() list of images ranked from the low-pass part to the
 * highest frequency part.
 *
 * Output image organization depends on the number of Dim on the image
 * In the OutputList, the first image is always the low passed one. Next,
 * follow the high Passed images of the lowest decomposition (ask
 * GetFilter(0)->GetNumberOfOutputs() to know the length) and so on until
 * the high passed images of the highest decomposition.
 *
 * Here is the specialization of the Wavelet::FORWARD transformation.
 *
 * \sa WaveletFilterBank
 * \sa WaveletPacketTransform
 *
 *
 * \ingroup OTBWavelet
 */
template <class TInputImage, class TOutputImage, class TFilter>
class ITK_EXPORT WaveletTransform<TInputImage, TOutputImage, TFilter, Wavelet::FORWARD>
  : public ImageToImageListFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef WaveletTransform                                  Self;
  typedef ImageToImageListFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(WaveletTransform, ImageToImageListFilter);

  typedef          TInputImage                  InputImageType;
  typedef typename InputImageType::Pointer      InputImagePointerType;
  typedef typename InputImageType::ConstPointer InputImageConstPointerType;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename InputImageType::SizeType     SizeType;
  typedef typename InputImageType::ValueType    ValueType;

  typedef typename Superclass::OutputImageType        OutputImageType;
  typedef typename Superclass::OutputImagePointerType OutputImagePointerType;
  typedef typename Superclass::OutputImageListType    OutputImageListType;
  typedef typename OutputImageListType::Pointer       OutputImageListPointerType;
  typedef typename OutputImageListType::Iterator      OutputImageIterator;

  typedef TFilter                           FilterType;
  typedef typename FilterType::Pointer      FilterPointerType;
  typedef ObjectList<FilterType>            FilterListType;
  typedef typename FilterListType::Pointer  FilterListPointerType;
  typedef typename FilterListType::Iterator FilterListIterator;

  itkGetObjectMacro(FilterList, FilterListType);
  FilterType * GetNthFilter(unsigned int idx) const
  {
    if (idx >= m_NumberOfDecompositions)
      {
      itkExceptionMacro(<< "Impossible to GetNthFilter with the index element "
                        << idx << "; this element don't exist, the size of the filter list is "
                        << m_FilterList->Size() << ".");
      }
    return static_cast<FilterType*>(m_FilterList->GetNthElement(idx));
  }

  typedef Wavelet::WaveletDirection DirectionOfTransformationEnumType;
  itkStaticConstMacro(DirectionOfTransformation, DirectionOfTransformationEnumType, Wavelet::FORWARD);

  itkGetMacro(NumberOfDecompositions, unsigned int);
  itkSetMacro(NumberOfDecompositions, unsigned int);

  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);

  /**
   * Set/Get the level of down sampling of the image used in forward algorithm.
   * (or upsampling in the inverse case)
   *
   * In this implementation, we are not (yet?) dealing with M-band decomposition
   * then m_SubsampleImageFactor is most likely to be 1 or 2... but in any case integer
   * and not real...
   */
  itkGetMacro(SubsampleImageFactor, unsigned int);
  itkSetMacro(SubsampleImageFactor, unsigned int);

protected:
  WaveletTransform();
  virtual ~WaveletTransform() {}

  /** Generate data redefinition */
  virtual void GenerateData();

private:
  WaveletTransform (const Self &);
  void operator =(const Self&);

  /**
   * Number of decomposition of the multi-scale/-resolution decomposition.
   */
  unsigned int m_NumberOfDecompositions;

  /**
   * For multiscale decomposition, m_SubsampleImageFactor is set to 1.
   * It yields also a stationary decomposition.
   * For multiresolution decomposition, most of the time, m_SubsampleImageFactor is
   * to be set to 2. This is its default value.
   */
  unsigned int m_SubsampleImageFactor;

  /**
   * The list of filter is necessary to keep the pipeline updated.
   */
  FilterListPointerType m_FilterList;
}; // end of Forward class

/** \class WaveletTransform
 * \brief Inverse Wavelet transformation framework
 *
 * This class defines the Wavelet transformation of an image
 * by using a (templated) elementary wavelet transform.
 *
 * Here the number of decomposition is given. The output is given
 * in the GetOutput() list of images ranked from the low-pass part to the
 * highest frequency part.
 *
 * Output image organization depends on the number of Dim on the image
 * In the OutputList, the first image is always the low passed one. Next,
 * follow the high Passed images of the lowest decomposition (ask
 * GetFilter(0)->GetNumberOfOutputs() to know the length) and so on until
 * the high passed images of the highest decomposition.
 *
 * Here is the specialization of the Wavelet::INVERSE transformation.
 *
 * The filter should be implemented with TInputImage for input and output image
 * template. Transtyping to TOutputImage is performed by standard C++-style at pixel
 * level.
 *
 * \sa WaveletFilterBank
 * \sa WaveletPacketTransform
 *
 *
 * \ingroup OTBWavelet
 */
template <class TInputImage, class TOutputImage, class TFilter>
class ITK_EXPORT WaveletTransform<TInputImage, TOutputImage, TFilter, Wavelet::INVERSE>
  : public ImageListToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef WaveletTransform                                  Self;
  typedef ImageListToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(WaveletTransform, ImageListToImageFilter);

  typedef          TInputImage                  InputImageType;
  typedef typename InputImageType::Pointer      InputImagePointerType;
  typedef typename InputImageType::ConstPointer InputImageConstPointerType;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename InputImageType::SizeType     SizeType;
  typedef typename InputImageType::IndexType    IndexType;
  typedef typename InputImageType::ValueType    ValueType;

  typedef typename Superclass::InputImageListType InputImageListType;
  typedef typename InputImageListType::Pointer    InputImageListPointerType;
  typedef typename InputImageListType::Iterator   InputImageIterator;

  typedef typename Superclass::OutputImageType OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::SizeType   OutputImageSizeType;
  typedef typename OutputImageType::IndexType  OutputImageIndexType;
  typedef typename OutputImageType::ValueType  OutputValueType;

  typedef TFilter                           FilterType;
  typedef typename FilterType::Pointer      FilterPointerType;
  typedef ObjectList<FilterType>            FilterListType;
  typedef typename FilterListType::Pointer  FilterListPointerType;
  typedef typename FilterListType::Iterator FilterListIterator;

  itkGetObjectMacro(FilterList, FilterListType);
  FilterType * GetNthFilter(unsigned int idx) const
  {
    if (idx >= m_NumberOfDecompositions)
      {
      itkExceptionMacro(<< "Impossible to GetNthFilter with the index element "
                        << idx << "; this element don't exist, the size of the filter list is "
                        << m_FilterList->Size() << ".");
      }
    return static_cast<FilterType*>(m_FilterList->GetNthElement(idx));
  }

  typedef Wavelet::WaveletDirection DirectionOfTransformationEnumType;
  itkStaticConstMacro(DirectionOfTransformation, DirectionOfTransformationEnumType, Wavelet::INVERSE);

  itkGetMacro(NumberOfDecompositions, unsigned int);

  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);

  /**
   * Set/Get the level of down sampling of the image used in forward algorithm.
   * (or upsampling in the inverse case)
   *
   * In this implementation, we are not (yet?) dealing with M-band decomposition
   * then m_SubsampleImageFactor is most likely to be 1 or 2... but in any case integer
   * and not real...
   */
  itkGetMacro(SubsampleImageFactor, unsigned int);
  itkSetMacro(SubsampleImageFactor, unsigned int);

protected:
  WaveletTransform();
  virtual ~WaveletTransform() {}

  /** GenerateOutputInformation
    * Set the size of the output image depending on the decimation factor
    * Copy information from the input image if existing.
    **/
  virtual void GenerateOutputInformation();

  /** Generate data redefinition */
  virtual void GenerateData();

private:
  WaveletTransform (const Self &);
  void operator =(const Self&);

  /**
   * Number of decomposition of the multi-scale/-resolution decomposition.
   */
  unsigned int m_NumberOfDecompositions;

  /**
   * For multiscale decomposition, m_SubsampleImageFactor is set to 1 (default).
   * It yields also a stationnary decomposition.
   * For multiresolution decomposition, most of the time, m_SubsampleImageFactor is
   * to be set to 2.
   */
  unsigned int m_SubsampleImageFactor;

  /**
   * The list of filter is necessary to keep the papeline updated.
   */
  FilterListPointerType m_FilterList;
}; // end of inverse class

} // end of namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbWaveletTransform.txx"
#endif

#endif
