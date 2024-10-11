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


#ifndef otbVectorDataSource_h
#define otbVectorDataSource_h

#include "OTBVectorDataBaseExport.h"
#include "itkProcessObject.h"
#include "itkMacro.h"

namespace otb
{
/** \class VectorDataSource
 * \brief Filter hierarchy for generating VectorData
 *
 *
 * \ingroup IO
 *
 *
 * \ingroup OTBVectorDataBase
 */

template <class TOutputVectorData>
class ITK_ABI_EXPORT VectorDataSource : public itk::ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef VectorDataSource              Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataSource, itk::ProcessObject);

  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;
  typedef TOutputVectorData                                  OutputVectorDataType;
  typedef typename TOutputVectorData::Pointer                OutputVectorDataPointer;

  /** Graft the specified DataObject onto this ProcessObject's output.
   * This method grabs a handle to the specified DataObject's bulk
   * data to used as its output's own bulk data.
   * Most importantly, however, it leaves the Source ivar untouched so
   * the original pipeline routing is intact. This method is used when
   * a process object is implemented using a mini-pipeline which is
   * defined in its GenerateData() method.
   *  */
  virtual void GraftOutput(itk::DataObject* output);

  /** Graft the specified data object onto this ProcessObject's idx'th
   * output. This is similar to the GraftOutput method except it
   * allows you to specify which output is affected. The specified index
   * must be a valid output number (less than
   * ProcessObject::GetNumberOfOutputs()). See the GraftOutput for
   * general usage information. */
  virtual void GraftNthOutput(unsigned int idx, itk::DataObject* output);

  /** Overriding GetOutput() method */
  virtual OutputVectorDataType* GetOutput(void);
  virtual OutputVectorDataType* GetOutput(DataObjectPointerArraySizeType idx);

protected:
  VectorDataSource();
  ~VectorDataSource() override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Ensure that the output vector data are cleared before processing */
  virtual void AllocateOutputs();

private:
  VectorDataSource(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataSource.hxx"
#endif

#endif // otbVectorDataSource_h
