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
#ifndef __otbSVMMachineLearningModelFactory_h
#define __otbSVMMachineLearningModelFactory_h

#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

namespace otb
{
/** \class SVMMachineLearningModelFactory
 * \brief Creation d'un instance d'un objet SVMMachineLearningModel utilisant les object factory.
 */
template <class TInputValue, class TTargetValue>
class ITK_EXPORT SVMMachineLearningModelFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef SVMMachineLearningModelFactory             Self;
  typedef itk::ObjectFactoryBase        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const;
  virtual const char* GetDescription(void) const;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SVMMachineLearningModelFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    SVMMachineLearningModelFactory::Pointer SVMFactory = SVMMachineLearningModelFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(SVMFactory);
  }

protected:
  SVMMachineLearningModelFactory();
  virtual ~SVMMachineLearningModelFactory();

private:
  SVMMachineLearningModelFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSVMMachineLearningModelFactory.txx"
#endif

#endif
