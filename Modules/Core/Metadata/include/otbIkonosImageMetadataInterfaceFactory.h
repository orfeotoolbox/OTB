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
#ifndef __otbIkonosImageMetadataInterfaceFactory_h
#define __otbIkonosImageMetadataInterfaceFactory_h


#include "itkObjectFactoryBase.h"

namespace otb
{
/** \class IkonosImageMetadataInterfaceFactory
 * \brief Creating an instance of a ImageMetadataInterface object using object factory.
 *
 * \ingroup OTBMetadata
 */
class ITK_EXPORT IkonosImageMetadataInterfaceFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef IkonosImageMetadataInterfaceFactory Self;
  typedef itk::ObjectFactoryBase              Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const;
  virtual const char* GetDescription(void) const;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(IkonosImageMetadataInterfaceFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    IkonosImageMetadataInterfaceFactory::Pointer ikonosIMIFactory = IkonosImageMetadataInterfaceFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(ikonosIMIFactory);
  }

protected:
  IkonosImageMetadataInterfaceFactory();
  virtual ~IkonosImageMetadataInterfaceFactory();

private:
  IkonosImageMetadataInterfaceFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
