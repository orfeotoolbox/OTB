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
#ifndef otbSarDefaultImageMetadataInterfaceFactory_h
#define otbSarDefaultImageMetadataInterfaceFactory_h


#include "itkObjectFactoryBase.h"

namespace otb
{
/** \class SarDefaultImageMetadataInterfaceFactory
 * \brief Creating an instance of a SarImageMetadataInterface object using object factory.
 * This class only provides the method of the virtual pure class SarImageMetadataInterface and throw exception for virtual method.
 *
 * \ingroup OTBMetadata
 */
class ITK_EXPORT SarDefaultImageMetadataInterfaceFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef SarDefaultImageMetadataInterfaceFactory Self;
  typedef itk::ObjectFactoryBase                  Superclass;
  typedef itk::SmartPointer<Self>                 Pointer;
  typedef itk::SmartPointer<const Self>           ConstPointer;

  /** Class methods used to interface with the registered factories. */
  const char* GetITKSourceVersion(void) const ITK_OVERRIDE;
  const char* GetDescription(void) const ITK_OVERRIDE;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SarDefaultImageMetadataInterfaceFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    SarDefaultImageMetadataInterfaceFactory::Pointer DefaultIMIFactory = SarDefaultImageMetadataInterfaceFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(DefaultIMIFactory);
  }

protected:
  SarDefaultImageMetadataInterfaceFactory();
  ~SarDefaultImageMetadataInterfaceFactory() ITK_OVERRIDE;

private:
  SarDefaultImageMetadataInterfaceFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
