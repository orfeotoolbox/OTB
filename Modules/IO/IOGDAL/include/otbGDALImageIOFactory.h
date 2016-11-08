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

#ifndef otbGDALImageIOFactory_h
#define otbGDALImageIOFactory_h

#include "itkObjectFactoryBase.h"

#include "OTBIOGDALExport.h"

namespace otb
{
/** \class GDALImageIOFactory
 * \brief Creation d'un instance d'un objet GDALImageIO utilisant les object factory.
 *
 * \ingroup OTBIOGDAL
 */
class OTBIOGDAL_EXPORT GDALImageIOFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef GDALImageIOFactory            Self;
  typedef itk::ObjectFactoryBase        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class methods used to interface with the registered factories. */
  const char* GetITKSourceVersion(void) const ITK_OVERRIDE;
  const char* GetDescription(void) const ITK_OVERRIDE;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);
  static GDALImageIOFactory * FactoryNew() { return new GDALImageIOFactory; }

  /** Run-time type information (and related methods). */
  itkTypeMacro(GDALImageIOFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    GDALImageIOFactory::Pointer GDALFactory = GDALImageIOFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(GDALFactory);
  }

protected:
  GDALImageIOFactory();
  ~GDALImageIOFactory() ITK_OVERRIDE;

private:
  GDALImageIOFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
