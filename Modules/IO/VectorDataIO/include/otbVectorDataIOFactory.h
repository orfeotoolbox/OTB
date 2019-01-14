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

#ifndef otbVectorDataIOFactory_h
#define otbVectorDataIOFactory_h

#include "itkObject.h"
#include "otbVectorDataIOBase.h"

namespace otb
{
/** \class VectorDataIOFactory
 * \brief Create instances of VectorDataIO objects using an object factory.
 *
 * \ingroup OTBVectorDataIO
 */
class ITK_EXPORT VectorDataIOFactory : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef VectorDataIOFactory           Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef VectorDataIOBase              VectorDataIOBaseType;
  typedef VectorDataIOBaseType::Pointer VectorDataIOBasePointerType;

  /** Class Methods used to interface with the registered factories */

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataIOFactory, Object);

  /** Convenient typedefs. */
  typedef VectorDataIOBase::Pointer VectorDataIOBasePointer;

  /** Mode in which the files is intended to be used */
  typedef enum { ReadMode, WriteMode } FileModeType;

  /** Create the appropriate VectorDataIO depending on the particulars of the file. */
  static VectorDataIOBasePointerType CreateVectorDataIO(const char* path, FileModeType mode);

  /** Register Built-in factories */
  static void RegisterBuiltInFactories();

protected:
  VectorDataIOFactory();
  ~VectorDataIOFactory() override;

private:
  VectorDataIOFactory(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#endif
