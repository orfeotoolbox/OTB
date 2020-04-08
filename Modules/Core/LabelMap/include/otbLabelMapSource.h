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


#ifndef otbLabelMapSource_h
#define otbLabelMapSource_h


#include "itkProcessObject.h"
#include "itkMacro.h"

namespace otb
{
/** \class LabelMapSource
 * \brief Base class for filter that output LabelMap
 *
 * \ingroup IO
 *
 *
 * \ingroup OTBLabelMap
 */

template <class TOutputLabelMap>
class ITK_EXPORT LabelMapSource : public itk::ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef LabelMapSource                Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LabelMapSource, itk::ProcessObject);

  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;
  typedef TOutputLabelMap                                    OutputLabelMapType;
  typedef typename TOutputLabelMap::Pointer                  OutputLabelMapPointer;

  /** Overriding GetOutput() method */
  virtual OutputLabelMapType* GetOutput(void);
  virtual OutputLabelMapType* GetOutput(DataObjectPointerArraySizeType idx);

protected:
  LabelMapSource();
  ~LabelMapSource() override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Ensure that the output vector data are cleared before processing */
  virtual void AllocateOutputs();

private:
  LabelMapSource(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelMapSource.hxx"
#endif

#endif // otbLabelMapSource_h
