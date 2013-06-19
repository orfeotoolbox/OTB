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
#ifndef __otbMachineLearningModelFactoryBase_h
#define __otbMachineLearningModelFactoryBase_h

#include "itkObject.h"
#include "itkMutexLock.h"

namespace otb
{
/** \class MachineLearningModelFactoryBase
 * \brief Base class for the MachinelearningModelFactory
 *
 * This class intends to hold the static attributes that can not be
 * part of a template class (ld error).
 *
 */
class ITK_EXPORT MachineLearningModelFactoryBase : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef MachineLearningModelFactoryBase Self;
  typedef itk::Object                     Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(MachineLearningModelFactoryBase, itk::Object);

protected:
  MachineLearningModelFactoryBase();
  virtual ~MachineLearningModelFactoryBase();

  static itk::SimpleMutexLock mutex;

private:
  MachineLearningModelFactoryBase(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end namespace otb

#endif
