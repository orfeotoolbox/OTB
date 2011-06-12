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
#ifndef __otbKMLVectorDataIOFactory_h
#define __otbKMLVectorDataIOFactory_h

#include "itkObjectFactoryBase.h"

namespace otb
{
/** \class KMLVectorDataIOFactory
 * \brief Creating an instance of a VectorDataImageIO object using object factory.
 */
class ITK_EXPORT KMLVectorDataIOFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef KMLVectorDataIOFactory        Self;
  typedef itk::ObjectFactoryBase        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const;
  virtual const char* GetDescription(void) const;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(KMLVectorDataIOFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    KMLVectorDataIOFactory::Pointer KMLFactory = KMLVectorDataIOFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(KMLFactory);
  }

protected:
  KMLVectorDataIOFactory();
  virtual ~KMLVectorDataIOFactory();

private:
  KMLVectorDataIOFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
