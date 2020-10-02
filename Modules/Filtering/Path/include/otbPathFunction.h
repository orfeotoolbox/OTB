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

#ifndef otbPathFunction_h
#define otbPathFunction_h

#include "itkFunctionBase.h"

namespace otb
{

/** \class PathFunction
 * \brief Evaluates a function of an image over a specific path.
 *
 * PathFunction is a baseclass for all objects that evaluates
 * a function of an image using a path list.
 * This class is templated over the input image type, the path type
 * and the function output and the coordinate representation type
 * (e.g. float or double).
 *
 * The input path is set via method SetInputPath().
 *
 *
 * \sa Path
 *
 * \ingroup PathFunction
 *
 * \ingroup OTBPath
 */
template <class TInputPath, class TOutput>
class ITK_EXPORT PathFunction : public itk::FunctionBase<TInputPath, TOutput>
{
public:
  /** Standard class typedefs. */
  typedef PathFunction Self;
  typedef itk::FunctionBase<TInputPath, TOutput> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(PathFunction, itk::FunctionBase);

  /** InputPathType typedef support. */
  typedef typename Superclass::InputType InputPathType;

  /** InputPathPointer typedef support */
  typedef typename InputPathType::ConstPointer InputPathConstPointer;

  /** OutputType typedef support. */
  typedef typename Superclass::OutputType OutputType;

  /** Set the input path. */
  virtual void SetInputPath(const InputPathType* ptr);

  /** Get the input path. */
  const InputPathType* GetInputPath() const
  {
    return m_Path.GetPointer();
  }

protected:
  PathFunction();
  ~PathFunction() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  InputPathConstPointer m_Path;

private:
  PathFunction(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPathFunction.hxx"
#endif

#endif
