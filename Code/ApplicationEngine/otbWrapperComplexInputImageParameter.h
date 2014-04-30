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
#ifndef __otbWrapperComplexInputImageParameter_h
#define __otbWrapperComplexInputImageParameter_h

#include "otbImageFileReader.h"
#include "itkImageBase.h"
#include "otbWrapperParameter.h"

namespace otb
{
namespace Wrapper
{
/** \class ComplexInputImageParameter
 *  \brief This class represents a ComplexInputImage parameter
 */

class ITK_EXPORT ComplexInputImageParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef ComplexInputImageParameter    Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef itk::ImageBase<2> ImageBaseType;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(ComplexInputImageParameter, Parameter);

  /** Set value from filename */
  bool SetFromFileName(const std::string& filename);
  itkGetConstMacro(FileName, std::string);

  /** Get the input image as CFloatVectorImageType. */
  ComplexFloatVectorImageType* GetImage();

  /** Get the input image as XXXImageType */
  ComplexFloatImageType*  GetComplexFloatImage();
  ComplexDoubleImageType* GetComplexDoubleImage();

  ComplexFloatVectorImageType*  GetComplexFloatVectorImage();
  ComplexDoubleVectorImageType* GetComplexDoubleVectorImage();

  /** Get the input image as templated image type. */
  template <class TImageType>
    TImageType* GetImage();

  /** Set a FloatVectorImageType image.*/
  void SetImage(ComplexFloatVectorImageType* image);

  /** Set a templated image.*/
  template <class TImageType>
    void SetImage(TImageType* image);

  /** Generic cast method that will be specified for each image type. */
  template <class TComplexInputImage, class TOutputImage>
    TOutputImage* CastImage();

  /** Cast an image to an image of the same type
  * Image to Image, VectorImage to VectorImage, RGBAImage to RGBAImage. */
  template <class TComplexInputImage, class TOutputImage>
    TOutputImage* SimpleCastImage();

  /** Cast an image to a vector image. */
  template <class TComplexInputImage, class TOutputImage>
    TOutputImage* CastVectorImageFromImage();

  bool HasValue() const;

  void ClearValue();


protected:
  /** Constructor */
  ComplexInputImageParameter();

  /** Destructor */
  virtual ~ComplexInputImageParameter();

  ImageBaseType::Pointer m_Image;
  std::string m_FileName;


  /** Readers typedefs */
  typedef otb::ImageFileReader<ComplexFloatImageType> ComplexFloatReaderType;
  typedef otb::ImageFileReader<ComplexDoubleImageType> ComplexDoubleReaderType;

  typedef otb::ImageFileReader<ComplexFloatVectorImageType> ComplexFloatVectorReaderType;
  typedef otb::ImageFileReader<ComplexDoubleVectorImageType> ComplexDoubleVectorReaderType;

  itk::ProcessObject::Pointer m_Reader;
  itk::ProcessObject::Pointer m_Caster;

private:
  ComplexInputImageParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

  /** Store the loaded image filename */
  std::string m_PreviousFileName;

  /** flag : are we using a filename or an image pointer as an input */
  bool m_UseFilename;

}; // End class ComplexInputImage Parameter


// template specializations of CastImage<> should be declared in header
// so that the linker knows they exist when building OTB Applications

#define otbDefineCastImageMacro(ComplexInputImageType, OutputImageType)   \
  template<> OutputImageType *                                          \
  ComplexInputImageParameter::CastImage<ComplexInputImageType , OutputImageType>();   \

#define otbGenericDefineCastImageMacro(ComplexInputImageType, prefix)     \
  otbDefineCastImageMacro(ComplexInputImageType, ComplexFloat##prefix##ImageType) \
  otbDefineCastImageMacro(ComplexInputImageType, ComplexDouble##prefix##ImageType)


/*********************************************************************
********************** Image -> Image
**********************************************************************/

  otbGenericDefineCastImageMacro(ComplexFloatImageType, )
  otbGenericDefineCastImageMacro(ComplexDoubleImageType, )


/*********************************************************************
********************** VectorImage -> VectorImage
**********************************************************************/
  otbGenericDefineCastImageMacro(ComplexFloatVectorImageType, Vector)
  otbGenericDefineCastImageMacro(ComplexDoubleVectorImageType, Vector)

} // End namespace Wrapper
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbWrapperComplexInputImageParameter.txx"
#endif

#endif
