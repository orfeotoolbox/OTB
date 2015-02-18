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
#ifndef __otbOpticalDefaultImageMetadataInterfaceFactory_h
#define __otbOpticalDefaultImageMetadataInterfaceFactory_h


#include "itkObjectFactoryBase.h"

namespace otb
{
/** \class OpticalDefaultImageMetadataInterfaceFactory
 * \brief Creating an instance of a OpticalImageMetadataInterface object using object factory.
 * This class only provides the method of the virtual pure class OpticalImageMetadataInterface and throw exception for virtual method.
 */
class ITK_EXPORT OpticalDefaultImageMetadataInterfaceFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef OpticalDefaultImageMetadataInterfaceFactory Self;
  typedef itk::ObjectFactoryBase                      Superclass;
  typedef itk::SmartPointer<Self>                     Pointer;
  typedef itk::SmartPointer<const Self>               ConstPointer;

  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const;
  virtual const char* GetDescription(void) const;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OpticalDefaultImageMetadataInterfaceFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    OpticalDefaultImageMetadataInterfaceFactory::Pointer DefaultIMIFactory = OpticalDefaultImageMetadataInterfaceFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(DefaultIMIFactory);
  }

protected:
  OpticalDefaultImageMetadataInterfaceFactory();
  virtual ~OpticalDefaultImageMetadataInterfaceFactory();

private:
  OpticalDefaultImageMetadataInterfaceFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
