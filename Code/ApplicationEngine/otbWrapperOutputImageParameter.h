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

#include "otbVectorImage.h"
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
  typedef OutputImageParameter           Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(OutputImageParameter, Parameter);

  /** Set the value */
  itkSetObjectMacro(Image, FloatVectorImageType);

  /** Get the value */
  itkGetObjectMacro(Image, FloatVectorImageType);

  /** Return any value */
  void SetValue(FloatVectorImageType* image);

  /** Return any value */
  FloatVectorImageType* GetValue( void );

  /** Set/Get PixelType to be used when saving */
  itkSetMacro(PixelType, ImagePixelType);
  itkGetMacro(PixelType, ImagePixelType);

  /** Return true if a filename is set */
  bool HasValue() const;

  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);

  void Write();

  itk::ProcessObject* GetWriter();

protected:
  /** Constructor */
  OutputImageParameter();
  /** Destructor */
  virtual ~OutputImageParameter();

  FloatVectorImageType::Pointer m_Image;
  std::string              m_FileName;
  ImagePixelType           m_PixelType;

  typedef otb::StreamingImageFileWriter<Int8VectorImageType>   Int8WriterType;
  typedef otb::StreamingImageFileWriter<UInt8VectorImageType>  UInt8WriterType;
  typedef otb::StreamingImageFileWriter<Int16VectorImageType>  Int16WriterType;
  typedef otb::StreamingImageFileWriter<UInt16VectorImageType> UInt16WriterType;
  typedef otb::StreamingImageFileWriter<Int32VectorImageType>  Int32WriterType;
  typedef otb::StreamingImageFileWriter<UInt32VectorImageType> UInt32WriterType;
  typedef otb::StreamingImageFileWriter<FloatVectorImageType>  FloatWriterType;
  typedef otb::StreamingImageFileWriter<DoubleVectorImageType> DoubleWriterType;

  Int8WriterType::Pointer   m_Int8Writer;
  UInt8WriterType::Pointer  m_UInt8Writer;
  Int16WriterType::Pointer  m_Int16Writer;
  UInt16WriterType::Pointer m_UInt16Writer;
  Int32WriterType::Pointer  m_Int32Writer;
  UInt32WriterType::Pointer m_UInt32Writer;
  FloatWriterType::Pointer  m_FloatWriter;
  DoubleWriterType::Pointer m_DoubleWriter;

private:
  OutputImageParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

}; // End class OutputImage Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
