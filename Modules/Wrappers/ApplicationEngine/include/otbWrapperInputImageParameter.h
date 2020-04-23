/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbWrapperInputImageParameter_h
#define otbWrapperInputImageParameter_h


#include "otbImageFileReader.h"
#include "otbClampImageFilter.h"
#include "otbWrapperParameter.h"

#include "itkImageBase.h"

#include <string>

namespace otb
{
namespace Wrapper
{
/** \class InputImageParameter
 *  \brief This class represents a InputImage parameter
 *
 * \ingroup OTBApplicationEngine
 */

class OTBApplicationEngine_EXPORT InputImageParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef InputImageParameter           Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(InputImageParameter, Parameter);

  typedef struct
  {
    itk::Object::Pointer app;
    std::string          key;
    bool                 isMem;
  } Connector;

  /** Set value from filename */
  bool SetFromFileName(const std::string& filename);
  itkGetConstReferenceMacro(FileName, std::string);

  void SetConnection(Connector c)
  {
    m_Connection = c;
  }

  const Connector& GetConnection() const
  {
    return m_Connection;
  }

  void SetConnectionMode(bool isMem)
  {
    m_Connection.isMem = isMem;
  }

  /** Get input-image as ImageBaseType. */
  ImageBaseType const* GetImage() const;
  ImageBaseType*       GetImage();

  /** Get the input image as XXXImageType */
  UInt8ImageType*  GetUInt8Image();
  UInt16ImageType* GetUInt16Image();
  Int16ImageType*  GetInt16Image();
  UInt32ImageType* GetUInt32Image();
  Int32ImageType*  GetInt32Image();
  FloatImageType*  GetFloatImage();
  DoubleImageType* GetDoubleImage();

  UInt8VectorImageType*  GetUInt8VectorImage();
  UInt16VectorImageType* GetUInt16VectorImage();
  Int16VectorImageType*  GetInt16VectorImage();
  UInt32VectorImageType* GetUInt32VectorImage();
  Int32VectorImageType*  GetInt32VectorImage();
  FloatVectorImageType*  GetFloatVectorImage();
  DoubleVectorImageType* GetDoubleVectorImage();

  UInt8RGBImageType*  GetUInt8RGBImage();
  UInt8RGBAImageType* GetUInt8RGBAImage();

  // Complex image
  ComplexInt16ImageType*  GetComplexInt16Image();
  ComplexInt32ImageType*  GetComplexInt32Image();
  ComplexFloatImageType*  GetComplexFloatImage();
  ComplexDoubleImageType* GetComplexDoubleImage();

  ComplexInt16VectorImageType*  GetComplexInt16VectorImage();
  ComplexInt32VectorImageType*  GetComplexInt32VectorImage();
  ComplexFloatVectorImageType*  GetComplexFloatVectorImage();
  ComplexDoubleVectorImageType* GetComplexDoubleVectorImage();

  /** Get the input image as templated image type. */
  template <class TImageType>
  TImageType* GetImage();

  /** Set a templated image.*/
  void SetImage(ImageBaseType* image);


  /** Generic cast method that will be specified for each image type. */
  template <class TInputImage, class TOutputImage>
  TOutputImage* CastImage();

  bool HasValue() const override;
  void ClearValue() override;

  ParameterType GetType() const override;
  std::string   ToString() const override;
  void FromString(const std::string& value) override;

protected:
  /** Constructor */
  InputImageParameter();

  /** Destructor */
  ~InputImageParameter() override;

private:
  InputImageParameter(const Parameter&) = delete;
  void operator=(const Parameter&) = delete;

  std::string                 m_FileName;
  itk::ProcessObject::Pointer m_Reader;

  ImageBaseType::Pointer m_Image;

  itk::ProcessObject::Pointer m_InputCaster;
  itk::ProcessObject::Pointer m_OutputCaster;

private:
  /** */
  template <typename T>
  using InputClampImageFilter = ClampImageFilter<T, otb::Wrapper::DoubleVectorImageType>;

  /** */
  template <typename T>
  using OutputClampImageFilter = ClampImageFilter<otb::Wrapper::DoubleVectorImageType, T>;

  /** */
  template <typename TOutputImage, typename TInputImage>
  TOutputImage* Cast(TInputImage*);

  /** Store the loaded image filename */
  std::string m_PreviousFileName;

  /** flag : are we using a filename or an image pointer as an input */
  bool m_UseFilename;

  Connector m_Connection;

}; // End class InputImage Parameter

} // End namespace Wrapper
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbWrapperInputImageParameter.hxx"
#endif

#endif
