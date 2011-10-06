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
#ifndef __otbWrapperOutputImageParameter_h
#define __otbWrapperOutputImageParameter_h

//#include "otbVectorImage.h"
#include "itkImageBase.h"
#include "otbWrapperParameter.h"
#include "otbStreamingImageFileWriter.h"

namespace otb
{
namespace Wrapper
{
/** \class OutputImageParameter
 *  \brief This class represents a OutputImage parameter
 */

class ITK_EXPORT OutputImageParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef OutputImageParameter          Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef itk::ImageBase<2> ImageBaseType;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(OutputImageParameter, Parameter);

  /** Set the value */
  itkSetObjectMacro(Image, ImageBaseType);

  /** Get the value */
  itkGetObjectMacro(Image, ImageBaseType);

  /** Return any value */
  void SetValue(ImageBaseType* image);

  /** Return any value */
  ImageBaseType* GetValue( void );

  /** Set/Get PixelType to be used when saving */
  itkSetMacro(PixelType, ImagePixelType);
  itkGetMacro(PixelType, ImagePixelType);

  /** Return true if a filename is set */
  bool HasValue() const;

  void SetFileName (const char* filename)
  {
    m_FileName = filename;
    SetActive(true);
  }
  void SetFileName (const std::string& filename)
  {
    this->SetFileName(filename.c_str());
  }

  itkGetStringMacro(FileName);

  void Write();

  itk::ProcessObject* GetWriter();

  void InitializeWriters();

protected:
  /** Constructor */
  OutputImageParameter();
  /** Destructor */
  virtual ~OutputImageParameter();

  template <class TInputImageType>
    void SwitchImageWrite();

  template <class TInputVectorImageType>
    void SwitchVectorImageWrite();

  template <class TInputVectorImageType>
    void SwitchRGBAImageWrite();

  //FloatVectorImageType::Pointer m_Image;
  ImageBaseType::Pointer m_Image;
  std::string            m_FileName;
  ImagePixelType         m_PixelType;

  typedef otb::StreamingImageFileWriter<Int8ImageType>   Int8WriterType;
  typedef otb::StreamingImageFileWriter<UInt8ImageType>  UInt8WriterType;
  typedef otb::StreamingImageFileWriter<Int16ImageType>  Int16WriterType;
  typedef otb::StreamingImageFileWriter<UInt16ImageType> UInt16WriterType;
  typedef otb::StreamingImageFileWriter<Int32ImageType>  Int32WriterType;
  typedef otb::StreamingImageFileWriter<UInt32ImageType> UInt32WriterType;
  typedef otb::StreamingImageFileWriter<FloatImageType>  FloatWriterType;
  typedef otb::StreamingImageFileWriter<DoubleImageType> DoubleWriterType;

  typedef otb::StreamingImageFileWriter<Int8VectorImageType>   VectorInt8WriterType;
  typedef otb::StreamingImageFileWriter<UInt8VectorImageType>  VectorUInt8WriterType;
  typedef otb::StreamingImageFileWriter<Int16VectorImageType>  VectorInt16WriterType;
  typedef otb::StreamingImageFileWriter<UInt16VectorImageType> VectorUInt16WriterType;
  typedef otb::StreamingImageFileWriter<Int32VectorImageType>  VectorInt32WriterType;
  typedef otb::StreamingImageFileWriter<UInt32VectorImageType> VectorUInt32WriterType;
  typedef otb::StreamingImageFileWriter<FloatVectorImageType>  VectorFloatWriterType;
  typedef otb::StreamingImageFileWriter<DoubleVectorImageType> VectorDoubleWriterType;

  typedef otb::StreamingImageFileWriter<Int8RGBAImageType>   RGBAInt8WriterType;
  typedef otb::StreamingImageFileWriter<UInt8RGBAImageType>  RGBAUInt8WriterType;
  typedef otb::StreamingImageFileWriter<Int16RGBAImageType>  RGBAInt16WriterType;
  typedef otb::StreamingImageFileWriter<UInt16RGBAImageType> RGBAUInt16WriterType;
  typedef otb::StreamingImageFileWriter<Int32RGBAImageType>  RGBAInt32WriterType;
  typedef otb::StreamingImageFileWriter<UInt32RGBAImageType> RGBAUInt32WriterType;
  typedef otb::StreamingImageFileWriter<FloatRGBAImageType>  RGBAFloatWriterType;
  typedef otb::StreamingImageFileWriter<DoubleRGBAImageType> RGBADoubleWriterType;

  Int8WriterType::Pointer   m_Int8Writer;
  UInt8WriterType::Pointer  m_UInt8Writer;
  Int16WriterType::Pointer  m_Int16Writer;
  UInt16WriterType::Pointer m_UInt16Writer;
  Int32WriterType::Pointer  m_Int32Writer;
  UInt32WriterType::Pointer m_UInt32Writer;
  FloatWriterType::Pointer  m_FloatWriter;
  DoubleWriterType::Pointer m_DoubleWriter;

  VectorInt8WriterType::Pointer   m_VectorInt8Writer;
  VectorUInt8WriterType::Pointer  m_VectorUInt8Writer;
  VectorInt16WriterType::Pointer  m_VectorInt16Writer;
  VectorUInt16WriterType::Pointer m_VectorUInt16Writer;
  VectorInt32WriterType::Pointer  m_VectorInt32Writer;
  VectorUInt32WriterType::Pointer m_VectorUInt32Writer;
  VectorFloatWriterType::Pointer  m_VectorFloatWriter;
  VectorDoubleWriterType::Pointer m_VectorDoubleWriter;

  RGBAInt8WriterType::Pointer   m_RGBAInt8Writer;
  RGBAUInt8WriterType::Pointer  m_RGBAUInt8Writer;
  RGBAInt16WriterType::Pointer  m_RGBAInt16Writer;
  RGBAUInt16WriterType::Pointer m_RGBAUInt16Writer;
  RGBAInt32WriterType::Pointer  m_RGBAInt32Writer;
  RGBAUInt32WriterType::Pointer m_RGBAUInt32Writer;
  RGBAFloatWriterType::Pointer  m_RGBAFloatWriter;
  RGBADoubleWriterType::Pointer m_RGBADoubleWriter;

private:
  OutputImageParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

}; // End class OutputImage Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
