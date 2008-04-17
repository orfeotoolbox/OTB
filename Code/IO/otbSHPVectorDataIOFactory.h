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
#ifndef __otbSHPVectorDataIOFactory_h
#define __otbSHPVectorDataIOFactory_h

#include "itkObjectFactoryBase.h"

namespace otb
{
/** \class SHPVectorDataIOFactory
 * \brief Creation d'un instance d'un objet VectorDataImageIO utilisant les object factory.
 */
template <class TData> class ITK_EXPORT SHPVectorDataIOFactory : public itk::ObjectFactoryBase
{
public:  
  /** Standard class typedefs. */
  typedef SHPVectorDataIOFactory   Self;
  typedef itk::ObjectFactoryBase  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const;
  virtual const char* GetDescription(void) const;
  
  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SHPVectorDataIOFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    SHPVectorDataIOFactory<TData>::Pointer SHPFactory = SHPVectorDataIOFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(SHPFactory);
  }

protected:
  SHPVectorDataIOFactory();
  ~SHPVectorDataIOFactory();

private:
  SHPVectorDataIOFactory(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};
  
  
} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSHPVectorDataIOFactory.txx"
#endif

#endif
