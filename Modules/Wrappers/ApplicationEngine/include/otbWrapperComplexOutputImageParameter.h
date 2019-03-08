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

#ifndef otbWrapperComplexOutputImageParameter_h
#define otbWrapperComplexOutputImageParameter_h

#include "itkImageBase.h"
#include "otbWrapperParameter.h"
#include "otbImageFileWriter.h"
#include <string>

namespace otb
{
namespace Wrapper
{
/** \class ComplexOutputImageParameter
 *  \brief This class represents a ComplexOutputImage parameter
 *
 * \ingroup OTBApplicationEngine
 */

class OTBApplicationEngine_EXPORT ComplexOutputImageParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef ComplexOutputImageParameter   Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(ComplexOutputImageParameter, Parameter);

  /** Set the value */
  itkSetObjectMacro(Image, ImageBaseType);

  /** Get the value */
  itkGetObjectMacro(Image, ImageBaseType);

  /** Return any value */
  void SetValue(ImageBaseType* image);

  /** Return any value */
  ImageBaseType* GetValue( void );

  /** Set/Get m_ComplexPixelType to be used when saving */
  itkSetMacro(ComplexPixelType, ComplexImagePixelType);
  itkGetMacro(ComplexPixelType, ComplexImagePixelType);

  /** Set/Get m_DefaultComplexPixelType*/
  itkSetMacro(DefaultComplexPixelType, ComplexImagePixelType);
  itkGetMacro(DefaultComplexPixelType, ComplexImagePixelType);

  /** Set/Get available RAM value */
  itkSetMacro(RAMValue, unsigned int);
  itkGetMacro(RAMValue, unsigned int);

  /** Implement the reset method (replace pixel type by default type) */
  void Reset() override
  {
    m_ComplexPixelType = m_DefaultComplexPixelType;
  }

  /** Static method to convert pixel type into string */
  static std::string ConvertPixelTypeToString(ComplexImagePixelType type);

  /** Convert a string into a ComplexImagePixelType (returns false if the
   *  conversion fails) */
  static bool ConvertStringToPixelType(const std::string &value, ComplexImagePixelType &type);

  /** Return true if a filename is set */
  bool HasValue() const override;

  void SetFileName (const char* filename);
  void SetFileName (const std::string& filename);

  itkGetStringMacro(FileName);

  void Write();

  itk::ProcessObject* GetWriter();

  void InitializeWriters();

  ParameterType GetType() const override;
  std::string ToString() const override;
  void FromString(const std::string& value) override;

protected:
  /** Constructor */
  ComplexOutputImageParameter();
  /** Destructor */
  ~ComplexOutputImageParameter() override;

  template <class TInputImageType>
    void SwitchImageWrite();

  template <class TInputVectorImageType>
    void SwitchVectorImageWrite();

  ImageBaseType::Pointer m_Image;
  std::string            m_FileName;
  ComplexImagePixelType         m_ComplexPixelType;
  ComplexImagePixelType         m_DefaultComplexPixelType;

  typedef otb::ImageFileWriter<ComplexInt16VectorImageType>  ComplexVectorInt16WriterType;
  typedef otb::ImageFileWriter<ComplexInt32VectorImageType>  ComplexVectorInt32WriterType;
  typedef otb::ImageFileWriter<ComplexFloatVectorImageType>  ComplexVectorFloatWriterType;
  typedef otb::ImageFileWriter<ComplexDoubleVectorImageType> ComplexVectorDoubleWriterType;

  ComplexVectorInt16WriterType::Pointer  m_ComplexVectorInt16Writer;
  ComplexVectorInt32WriterType::Pointer  m_ComplexVectorInt32Writer;
  ComplexVectorFloatWriterType::Pointer  m_ComplexVectorFloatWriter;
  ComplexVectorDoubleWriterType::Pointer m_ComplexVectorDoubleWriter;


private:
  ComplexOutputImageParameter(const Parameter &) = delete;
  void operator =(const Parameter&) = delete;

  unsigned int                  m_RAMValue;

}; // End class OutputImage Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
