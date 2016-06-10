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
#ifndef __otbOGRVectorDataIOFactory_h
#define __otbOGRVectorDataIOFactory_h

#include "itkObjectFactoryBase.h"

namespace otb
{
/** \class OGRVectorDataIOFactory
 * \brief Create an instance of an OGRVectorDataIOFactory.
 *
 * \ingroup OTBIOGDAL
 */
class ITK_EXPORT OGRVectorDataIOFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef OGRVectorDataIOFactory        Self;
  typedef itk::ObjectFactoryBase        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class methods used to interface with the registered factories. */
  const char* GetITKSourceVersion(void) const ITK_OVERRIDE;
  const char* GetDescription(void) const ITK_OVERRIDE;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OGRVectorDataIOFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    OGRVectorDataIOFactory::Pointer OGRFactory = OGRVectorDataIOFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(OGRFactory);
  }

protected:
  OGRVectorDataIOFactory();
  ~OGRVectorDataIOFactory() ITK_OVERRIDE;

private:
  OGRVectorDataIOFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
