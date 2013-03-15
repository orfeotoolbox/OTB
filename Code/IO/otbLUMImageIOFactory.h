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
#ifndef __otbLUMImageIOFactory_h
#define __otbLUMImageIOFactory_h

#include "itkObjectFactoryBase.h"
#include "otbImageIOBase.h"

namespace otb
{
/** \class LUMImageIOFactory
 * \brief Creation d'un instance d'un objet LUMImageIO utilisant les object factory.
 */
class ITK_EXPORT LUMImageIOFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef LUMImageIOFactory             Self;
  typedef itk::ObjectFactoryBase        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const;
  virtual const char* GetDescription(void) const;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LUMImageIOFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    LUMImageIOFactory::Pointer LUMFactory = LUMImageIOFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(LUMFactory);
  }

protected:
  LUMImageIOFactory();
  virtual ~LUMImageIOFactory();

private:
  LUMImageIOFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
