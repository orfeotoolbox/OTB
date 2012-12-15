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
#ifndef __otbRandomForestsMachineLearningModelFactory_h
#define __otbRandomForestsMachineLearningModelFactory_h

#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

namespace otb
{
/** \class RandomForestsMachineLearningModelFactory
 * \brief Creation d'un instance d'un objet RandomForestsMachineLearningModel utilisant les object factory.
 */
template <class TInputValue, class TTargetValue>
class ITK_EXPORT RandomForestsMachineLearningModelFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef RandomForestsMachineLearningModelFactory             Self;
  typedef itk::ObjectFactoryBase        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const;
  virtual const char* GetDescription(void) const;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(RandomForestsMachineLearningModelFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    RandomForestsMachineLearningModelFactory::Pointer RFFactory = RandomForestsMachineLearningModelFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(RFFactory);
  }

protected:
  RandomForestsMachineLearningModelFactory();
  virtual ~RandomForestsMachineLearningModelFactory();

private:
  RandomForestsMachineLearningModelFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRandomForestsMachineLearningModelFactory.txx"
#endif

#endif
