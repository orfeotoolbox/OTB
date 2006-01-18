/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   11 janvier 2005
  Version   :   
  Role      :   Création d'instances d'objet ImageIO utilisant les object factory
  $Id$

=========================================================================*/
#ifndef __otbCAIImageIOFactory_h
#define __otbCAIImageIOFactory_h

#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

namespace otb
{
/** \class CAIImageIOFactory
 * \brief Creation d'un instance d'un objet CAIImageIO utilisant les object factory.
 */
class ITK_EXPORT CAIImageIOFactory : public itk::ObjectFactoryBase
{
public:  
  /** Standard class typedefs. */
  typedef CAIImageIOFactory   Self;
  typedef itk::ObjectFactoryBase  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const;
  virtual const char* GetDescription(void) const;
  
  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(CAIImageIOFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    CAIImageIOFactory::Pointer CAIFactory = CAIImageIOFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(CAIFactory);
  }

protected:
  CAIImageIOFactory();
  ~CAIImageIOFactory();

private:
  CAIImageIOFactory(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};
  
  
} // end namespace otb

#endif
