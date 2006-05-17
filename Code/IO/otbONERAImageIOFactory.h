/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   17 mai 2006
  Version   :   
  Role      :   Read/Write ONERA format data
  $Id$

=========================================================================*/
#ifndef __otbONERAImageIOFactory_h
#define __otbONERAImageIOFactory_h

#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

namespace otb
{
/** \class ONERAImageIOFactory
 * \brief Creation d'un instance d'un objet ONERAImageIO utilisant les object factory.
 */
class ITK_EXPORT ONERAImageIOFactory : public itk::ObjectFactoryBase
{
public:  
  /** Standard class typedefs. */
  typedef ONERAImageIOFactory   Self;
  typedef itk::ObjectFactoryBase  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const;
  virtual const char* GetDescription(void) const;
  
  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ONERAImageIOFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    ONERAImageIOFactory::Pointer ONERAFactory = ONERAImageIOFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(ONERAFactory);
  }

protected:
  ONERAImageIOFactory();
  ~ONERAImageIOFactory();

private:
  ONERAImageIOFactory(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};
  
  
} // end namespace otb

#endif
