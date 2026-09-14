/*
 * Copyright (C) 2005-2026 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbWrapperInputImageParameter_h
#define otbWrapperInputImageParameter_h


#include "otbWrapperParameter.h"
#include "otbNewMacro.h"

#include "itkImageBase.h"

#include <string>

namespace otb
{
namespace Wrapper
{
/** \class InputImageParameter
 *  \brief This class represents an InputImage parameter
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT InputImageParameter
: public Parameter
{
public:
  // Standard class typedefs
  using Self         = InputImageParameter;
  using Superclass   = Parameter;
  using Pointer      = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;

  /** Defining ::New() static method */
  otbNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(InputImageParameter, Parameter);

  static constexpr auto Type = ParameterType_InputImage;

  struct Connector
  {
    itk::Object::Pointer app;
    std::string          key;
    bool                 isMem;
  };

  /** Set value from filename */
  bool SetFromFileName(std::string filename);
  itkGetConstReferenceMacro(FileName, std::string);

  void SetConnection(Connector c)
  {
    m_Connection = std::move(c);
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

  ParameterType GetType() const override
  {
    return Type;
  }
  std::string   ToString() const override;
  void FromString(const std::string& value) override;

protected:
  /**
   * Default (& init) constructor.
   * Initialize a new instance with its parameter information, and permits to enable reuse of image
   * parts already used.
   *
   * \param[in] info                   Parameter information (name, key, description)
   * \param[in] mustReuseLoadedRegion  Enable reuse of image part previously load for previous
   *                                   output strip/stream.
   * \param[in] streamHeight           Permits to indirectly control RAM usage during image loading.
   *                                   Setting this parameter overrides whatever
   *                                   `ConfigurationManager::GetMaxImageRowsReadAtOnce()` returns.
   */
  InputImageParameter(
      Info info = Info{},
      bool mustReuseLoadedRegion = false,
      unsigned long streamHeight = 0 // expect an ambiguity if you pass an int instead of un unsigned long
  );

  /// \overload
  InputImageParameter(
      Info info,
      unsigned long streamHeight // expect an ambiguity if you pass an int instead of un unsigned long
  );

  /** Destructor */
  ~InputImageParameter() override = default;

private:
  InputImageParameter(const Parameter&) = delete;
  void operator=(const Parameter&) = delete;

  std::string                 m_FileName;
  itk::ProcessObject::Pointer m_Reader = nullptr;

  ImageBaseType::Pointer      m_Image = nullptr;

  itk::ProcessObject::Pointer m_OutputCaster = nullptr;
  itk::DataObject::Pointer    m_OutputCasted = nullptr;

  /** Option to limit the number of lines loaded at a time.
   * The option is considered _set_ when it's strictly positive (> 0).
   * Sometimes loading an image region requires twice as much RAM than what the buffered region
   * would use, and we may not have that much memory. This paremeter will tell to stream the
   * loading.
   */
  unsigned long m_StreamHeight                      = 0;

  /// Autorize reuse of previously loaded image region
  bool                      m_mustReuseLoadedRegion = false;

private:
  /** */
  template <typename TOutputImage, typename TInputImage>
  TOutputImage* Cast(TInputImage*);

  /** Store the loaded image filename */
  std::string m_PreviousFileName;

  /** flag : are we using a filename or an image pointer as an input */
  bool        m_UseFilename = true;

  Connector   m_Connection{};

}; // End class InputImage Parameter

template <>
struct ParameterTypeTraits<ParameterType_InputImage>
{
  using Type = InputImageParameter;
};

static_assert(ParameterTypeTraits<ParameterType_InputImage>::Type::Type == ParameterType_InputImage);

} // End namespace Wrapper
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#  include "otbWrapperInputImageParameter.hxx"
#endif

#endif
