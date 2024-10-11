/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbVectorDataFileWriter_h
#define otbVectorDataFileWriter_h

#include "itkProcessObject.h"
#include "itkMacro.h"
#include "otbVectorDataIOBase.h"
#include <string>

namespace otb
{
/** \brief Base exception class for IO problems during writing.
 *
 * \class ImageFileWriterException
 *
 * \ingroup OTBVectorDataIO
 */
class VectorDataFileWriterException : public itk::ExceptionObject
{
public:
  /** Run-time information. */
  itkTypeMacro(VectorDataFileWriterException, ExceptionObject);

  /** Constructor. */
  VectorDataFileWriterException(const char* file, unsigned int line, const char* message = "Error in IO", const char* loc = "Unknown")
    : ExceptionObject(file, line, message, loc)
  {
  }

  /** Constructor. */
  VectorDataFileWriterException(const std::string& file, unsigned int line, const char* message = "Error in IO", const char* loc = "Unknown")
    : ExceptionObject(file, line, message, loc)
  {
  }
};

/** \class VectorDataFileWriter
 *  \brief This class writes vector data file format (shapefile and KML)
 *
 *  \ingroup IO
 *
 * \ingroup OTBVectorDataIO
 */

template <class TInputVectorData>
class ITK_EXPORT VectorDataFileWriter : public itk::ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef VectorDataFileWriter          Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataFileWriter, itk::ProcessObject);

  typedef TInputVectorData InputVectorDataType;
  typedef VectorDataIOBase VectorDataIOBaseType;

  /** Set/Get the image input of this writer.  */
  using Superclass::SetInput;
  void SetInput(const InputVectorDataType* input);
  const InputVectorDataType* GetInput(void);
  const InputVectorDataType* GetInput(unsigned int idx);

  /** Does the real work. */
  virtual void Write();

  void GenerateData() override;

  void Update() override
  {
    this->Write();
  }

  /** Specify the name of the output shapefile to write. */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);

protected:
  VectorDataFileWriter();
  ~VectorDataFileWriter() override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  std::string                            m_FileName; // The file to be read
  typename VectorDataIOBaseType::Pointer m_VectorDataIO;
  bool                                   m_UserSpecifiedVectorDataIO; // track whether the VectorDataIO
  bool                                   m_FactorySpecifiedVectorDataIO;

private:
  VectorDataFileWriter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataFileWriter.hxx"
#endif

#endif // otbVectorDataFileWriter_h
