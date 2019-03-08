/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperOutputImageParameter_h
#define otbWrapperOutputImageParameter_h

//#include "otbVectorImage.h"
#include "itkImageBase.h"
#include "otbWrapperParameter.h"
#include "otbImageFileWriter.h"
#include <string>

namespace otb
{
namespace Wrapper
{
/** \class OutputImageParameter
 *  \brief This class represents a OutputImage parameter
 *
 * \ingroup OTBApplicationEngine
 */

class OTBApplicationEngine_EXPORT OutputImageParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef OutputImageParameter          Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

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

  /** Set/Get DefaultPixelType  */
  itkSetMacro(DefaultPixelType, ImagePixelType);
  itkGetMacro(DefaultPixelType, ImagePixelType);

  /** Set/Get available RAM value */
  itkSetMacro(RAMValue, unsigned int);
  itkGetMacro(RAMValue, unsigned int);

  /** Implement the reset method (replace pixel type by default type) */
  void Reset() override
  {
    m_PixelType = m_DefaultPixelType;
  }

  /** Static method to convert pixel type into string */
  static std::string ConvertPixelTypeToString(ImagePixelType type);

  /** Converts a string into a pixel type (returns false if the conversion
   *  fails) */
  static bool ConvertStringToPixelType(const std::string &value, ImagePixelType &type);

  /** Return true if a filename is set */
  bool HasValue() const override;

  void SetFileName (const char* filename);
  void SetFileName (const std::string& filename);

  itkGetStringMacro(FileName);

  void Write();

  itk::ProcessObject* GetWriter();

  void InitializeWriters();

  std::string CheckFileName(bool fixMissingExtension = false);

  ParameterType GetType() const override
  {
    return ParameterType_OutputImage;
  }

  std::string ToString() const override
  {
    return GetFileName();
  }

  void FromString(const std::string& value) override
  {
    SetFileName(value);
  }

protected:
  /** Constructor */
  OutputImageParameter();
  /** Destructor */
  ~OutputImageParameter() override;

  template <class TInput>
    int SwitchInput(TInput *img);

  //FloatVectorImageType::Pointer m_Image;
  ImageBaseType::Pointer m_Image;
  std::string            m_FileName;
  ImagePixelType         m_PixelType;
  ImagePixelType         m_DefaultPixelType;

private:
  OutputImageParameter(const Parameter &) = delete;
  void operator =(const Parameter&) = delete;

  unsigned int                  m_RAMValue;

  itk::ProcessObject::Pointer m_Caster;

  itk::ProcessObject::Pointer m_Writer;

}; // End class OutputImage Parameter

// Declare specialisation for UInt8RGBAImageType
template <>
int
OutputImageParameter::SwitchInput(UInt8RGBAImageType *img);

// Declare specialisation for UInt8RGBImageType
template <>
int
OutputImageParameter::SwitchInput(UInt8RGBImageType *img);

} // End namespace Wrapper
} // End namespace otb

#endif
