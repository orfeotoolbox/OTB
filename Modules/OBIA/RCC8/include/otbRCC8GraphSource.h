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

#ifndef otbRCC8GraphSource_h
#define otbRCC8GraphSource_h

#include "itkProcessObject.h"

namespace otb
{
/** \class RCC8GraphSource
 *  \brief Base class for all the filters producing an otbRCC8Graph
 * \ingroup DataSources
 *
 * \ingroup OTBRCC8
 */
template <class TOutputGraph>
class ITK_EXPORT RCC8GraphSource
  : public itk::ProcessObject
{
public:
  /** Standard typedefs */
  typedef RCC8GraphSource               Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Type macro */
  itkNewMacro(Self);
  /** Creation through object factory macro */
  itkTypeMacro(RCC8GraphSource, itk::ProcessObject);
  /** Data object pointer type */
  typedef itk::DataObject::Pointer DataObjectPointer;
  /** Template parameter typedef*/
  typedef TOutputGraph                      OutputGraphType;
  typedef typename OutputGraphType::Pointer OutputGraphPointerType;
  /** Overriding of the GetOutput() method */
  virtual OutputGraphType * GetOutput(void);

protected:
  /** Constructor */
  RCC8GraphSource();
  /** Destructor */
  ~RCC8GraphSource() override {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  RCC8GraphSource(const Self &) = delete;
  void operator =(const Self&) = delete;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRCC8GraphSource.hxx"
#endif
#endif
