/*=========================================================================

  Programm  :   OTB (ORFEO ToolBox)
  Language  :   C++
  Date      :   7 june 2006

=========================================================================*/
#ifndef __otbMSTARImageIOFactory_h
#define __otbMSTARImageIOFactory_h

#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

namespace otb
{
/** \class MSTARImageIOFactory
 * \brief Creation d'un instance d'un objet MSTARImageIO utilisant les object factory.
 */
class ITK_EXPORT MSTARImageIOFactory : public itk::ObjectFactoryBase
{
public:  
  /** Standard class typedefs. */
  typedef MSTARImageIOFactory   Self;
  typedef itk::ObjectFactoryBase  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const;
  virtual const char* GetDescription(void) const;
  
  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MSTARImageIOFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    MSTARImageIOFactory::Pointer MSTARFactory = MSTARImageIOFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(MSTARFactory);
  }

protected:
  MSTARImageIOFactory();
  ~MSTARImageIOFactory();

private:
  MSTARImageIOFactory(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};
  
  
} // end namespace otb

#endif
