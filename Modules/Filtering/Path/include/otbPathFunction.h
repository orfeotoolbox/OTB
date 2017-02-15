/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
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
template <class TInputPath,
    class TOutput>
class ITK_EXPORT PathFunction :
  public itk::FunctionBase<TInputPath, TOutput>
{
public:

  /** Standard class typedefs. */
  typedef PathFunction                           Self;
  typedef itk::FunctionBase<TInputPath, TOutput> Superclass;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(PathFunction, itk::FunctionBase);

  /** InputPathType typedef support. */
  typedef typename Superclass::InputType InputPathType;

  /** InputPathPointer typedef support */
  typedef typename InputPathType::ConstPointer InputPathConstPointer;

  /** OutputType typedef support. */
  typedef typename Superclass::OutputType OutputType;

  /** Set the input path. */
  virtual void SetInputPath(const InputPathType * ptr);

  /** Get the input path. */
  const InputPathType * GetInputPath() const
  {
    return m_Path.GetPointer();
  }

protected:
  PathFunction();
  ~PathFunction() ITK_OVERRIDE {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

  InputPathConstPointer m_Path;

private:
  PathFunction(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPathFunction.txx"
#endif

#endif
