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

#ifndef otbVectorDataFileReader_h
#define otbVectorDataFileReader_h

#include "otbVectorDataSource.h"
#include "otbVectorDataIOBase.h"
#include <string>

namespace otb
{
/** \class VectorDataFileReaderException
 *
 * \brief Base exception class for IO conflicts.
 *
 * \ingroup OTBVectorDataIO
 */
class VectorDataFileReaderException : public itk::ExceptionObject
{
public:
  /** Run-time information. */
  itkTypeMacro(VectorDataFileReaderException, itk::ExceptionObject);

  /** Constructor. */
  VectorDataFileReaderException(const char* file, unsigned int line, const char* message = "Error in IO", const char* loc = "Unknown")
    : itk::ExceptionObject(file, line, message, loc)
  {
  }

  /** Constructor. */
  VectorDataFileReaderException(const std::string& file, unsigned int line, const char* message = "Error in IO", const char* loc = "Unknown")
    : itk::ExceptionObject(file, line, message, loc)
  {
  }

  VectorDataFileReaderException(const std::string& file, unsigned int line, const std::string& message = "Error in IO", const char* loc = "Unknown")
    : itk::ExceptionObject(file, line, message, loc)
  {
  }
};

/** \brief Data source that reads vector data from a single file.
 *
 * This source object is a general filter to read data from
 * a variety of vector data file formats. It works with a VectorDataIOBase subclass
 * to actually do the reading of the data. Object factory machinery
 * can be used to automatically create the VectorDataIOBase, or the
 * VectorDataIOBase can be manually created and set. Note that this
 * class reads data from a single file.
 *
 * TOutputVectorData is the type expected by the external users of the
 * filter. If data stored in the file is stored in a different format
 * then specified by TOutputVectorData, than this filter converts data
 * between the file type and the external expected type.
 *
 * A Pluggable factory pattern is used this allows different kinds of readers
 * to be registered (even at run time) without having to modify the
 * code in this class. Normally just setting the FileName with the
 * appropriate suffix is enough to get the reader to instantiate the
 * correct VectorDataIO and read the file properly. However, some files (like
 * raw binary format) have no accepted suffix, so you will have to
 * manually create the VectorDataIO instance of the write type.
 *
 * \sa VectorDataIOBase
 *
 */

template <class TOutputVectorData>
class ITK_EXPORT VectorDataFileReader : public VectorDataSource<TOutputVectorData>
{
public:
  /** Standard class typedefs. */
  typedef VectorDataFileReader                Self;
  typedef VectorDataSource<TOutputVectorData> Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataFileReader, VectorDataSource);

  typedef TOutputVectorData OutputVectorType;
  typedef VectorDataIOBase  VectorDataIOBaseType;

  itkStaticConstMacro(VDimension, unsigned int, OutputVectorType::DataNodeType::Dimension);
  typedef itk::Vector<double, VDimension> SpacingType;
  typedef itk::Point<double, VDimension>  PointType;

  /** Specify the file to read */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);

  /** Set/Get the VectorDataIO helper class. Often this is created via the object
   * factory mechanism that determines whether a particular VectorDataIO can
   * read a certain file. This method provides a way to get the VectorDataIO
   * instance that is created. Or you can directly specify the VectorDataIO
   * to use to read a particular file in case the factory mechanism will
   * not work properly (e.g., unknown or unusual extension). */
  void SetVectorDataIO(VectorDataIOBaseType* vectorDataIO);
  itkGetObjectMacro(VectorDataIO, VectorDataIOBaseType);

  /** Prepare the allocation of the output vector data during the first back
   * propagation of the pipeline. */
  void GenerateOutputInformation(void) override;

  /** Does the real work. */
  void GenerateData() override;

protected:
  VectorDataFileReader();
  ~VectorDataFileReader() override;
  std::string m_ExceptionMessage;

  typename VectorDataIOBaseType::Pointer m_VectorDataIO;
  bool                                   m_UserSpecifiedVectorDataIO; // keep track whether the

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  std::string m_FileName; // The file to be read

private:
  VectorDataFileReader(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Test whether the given filename exist and it is readable.
      If the file doesn't exist or it is not readable, and exception with an
      appropriate message will be thrown. */
  void TestFileExistenceAndReadability();
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataFileReader.hxx"
#endif

#endif // otbVectorDataFileReader_h
