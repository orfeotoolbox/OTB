/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   11 janvier 2005
  Version   :   
  Role      :   Création d'instances d'objet GDALImageIO utilisant les object factory
  $Id$

=========================================================================*/
#ifndef __otbGDALImageIOFactory_h
#define __otbGDALImageIOFactory_h

#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

namespace otb
{
/** \class GDALImageIOFactory
 * \brief Creation d'un instance d'un objet GDALImageIO utilisant les object factory.
 */
class ITK_EXPORT GDALImageIOFactory : public itk::ObjectFactoryBase
{
public:  
  /** Standard class typedefs. */
  typedef GDALImageIOFactory   Self;
  typedef itk::ObjectFactoryBase  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const;
  virtual const char* GetDescription(void) const;
  
  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

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
  ~GDALImageIOFactory();

private:
  GDALImageIOFactory(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};
  
  
} // end namespace otb

#endif
