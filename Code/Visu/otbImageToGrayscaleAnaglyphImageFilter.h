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
#ifndef __otbImageToGrayscaleAnaglyphImageFilter_h
#define __otbImageToGrayscaleAnaglyphImageFilter_h

#include "itkBinaryFunctorImageFilter.h"
#include "itkNumericTraits.h"

namespace otb
{
namespace Functor
  {
    /** \class GrayscaleAnaglyphFunctor
     *  \brief This functor implements the pixel-wise grayscale anaglyph composition.
     *  The output pixel is a vector pixel of size 3, where the first component contains the
     *  value of the first input pixel, and the two last components contain the value of the
     *  second input pixel.
     */
    template <class TInputPixel1,class TInputPixel2,class TOutputPixel>
      class GrayscaleAnaglyphFunctor
      {
      public:
	/// Constructor
	GrayscaleAnaglyphFunctor(){};
	/// Destructor
	~GrayscaleAnaglyphFunctor(){};


	inline TOutputPixel operator()(const TInputPixel1& pixel1, const TInputPixel2& pixel2)
	  {
	    TOutputPixel result(3);
	    result[0]=static_cast<typename TOutputPixel::ValueType>(pixel1);
	    result[1]=static_cast<typename TOutputPixel::ValueType>(pixel2);
	    result[2]=static_cast<typename TOutputPixel::ValueType>(pixel2);
	    return result;
	  }
      };
  }
 /** \class ImageToGrayscaleAnaglyphImageFilter
  *  \brief This filter implements the synthesis of a grayscale anaglyph image from
  *  a pair of stereoscopic images.
  *  The output image is a VectorImage with 3 channels, where the first channel is the first
  *  input image, and the two last channels are the second input image.
  *  \note TOutputImage is supposed to be of type VectorImage.
  */
template <class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT ImageToGrayscaleAnaglyphImageFilter
  : public itk::BinaryFunctorImageFilter
  <  TInputImage1,TInputImage2,TOutputImage,
  Functor::GrayscaleAnaglyphFunctor<
  typename TInputImage1::PixelType,
  typename TInputImage2::PixelType,
  typename TOutputImage::PixelType
  > >
  {
 public:
  /** Standard typedefs */
  typedef ImageToGrayscaleAnaglyphImageFilter  Self;

  typedef itk::BinaryFunctorImageFilter
  <  TInputImage1,TInputImage2,TOutputImage,
    Functor::GrayscaleAnaglyphFunctor<
    typename TInputImage1::PixelType,
    typename TInputImage2::PixelType,
    typename TOutputImage::PixelType
    >  > Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImageToGrayscaleAnaglyphImageFilter, BinaryFunctorImageFilter);

  virtual void GenerateOutputInformation(void)
    {
      Superclass::GenerateOutputInformation();
      this->GetOutput()->SetNumberOfComponentsPerPixel(3);
    }
protected:
  /** Constructor */
  ImageToGrayscaleAnaglyphImageFilter(){};
  /** Destructor */
  virtual ~ImageToGrayscaleAnaglyphImageFilter() {};
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
    {
      Superclass::PrintSelf(os,indent);
    };

private:
  ImageToGrayscaleAnaglyphImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
}// End namespace otb
#endif
