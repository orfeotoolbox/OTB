#ifndef SOMModelFactory_h
#define SOMModelFactory_h


#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

namespace otb
{
	
template <class TInputValue, class TTargetValue, unsigned int MapDimension>
class ITK_EXPORT SOMModelFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef SOMModelFactory   Self;
  typedef itk::ObjectFactoryBase        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class methods used to interface with the registered factories. */
  const char* GetITKSourceVersion(void) const ITK_OVERRIDE;
  const char* GetDescription(void) const ITK_OVERRIDE;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SOMModelFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    Pointer SOMFactory = SOMModelFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(SOMFactory);
  }

protected:
  SOMModelFactory();
  ~SOMModelFactory() ITK_OVERRIDE;

private:
  SOMModelFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};



} //namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "SOMModelFactory.txx"
#endif

#endif


