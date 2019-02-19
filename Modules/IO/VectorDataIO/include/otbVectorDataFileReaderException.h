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

#ifndef otbVectorDataFileReaderException_h
#define otbVectorDataFileReaderException_h

#include "itkMacro.h"
#include "OTBVectorDataIOExport.h"

namespace otb
{

/** \class VectorDataFileReaderException
 *
 * \brief Base exception class for IO conflicts.
 *
 * \ingroup OTBVectorDataIO
 */
class OTBVectorDataIO_EXPORT VectorDataFileReaderException : public itk::ExceptionObject
{
public:
  /** Run-time information. */
  itkTypeMacro(VectorDataFileReaderException, itk::ExceptionObject);

  /** Constructor. */
  VectorDataFileReaderException(const char *file, unsigned int line,
                                const char* message = "Error in IO",
                                const char* loc = "Unknown");

  /** Constructor. */
  VectorDataFileReaderException(const std::string& file, unsigned int line,
                                const char* message = "Error in IO",
                                const char* loc = "Unknown");

  VectorDataFileReaderException(const std::string& file, unsigned int line,
                                const std::string& message = "Error in IO",
                                const char* loc = "Unknown");

  ~VectorDataFileReaderException() override;
};

}

#endif
