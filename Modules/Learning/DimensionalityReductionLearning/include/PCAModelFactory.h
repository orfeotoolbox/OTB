#ifndef PCAModelFactory_h
#define PCAModelFactory_h


#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

namespace otb
{
	
template <class TInputValue, class TTargetValue>
class ITK_EXPORT PCAModelFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef PCAModelFactory   Self;
  typedef itk::ObjectFactoryBase        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class methods used to interface with the registered factories. */
  const char* GetITKSourceVersion(void) const ITK_OVERRIDE;
  const char* GetDescription(void) const ITK_OVERRIDE;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(PCAModelFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    Pointer PCAFactory = PCAModelFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(PCAFactory);
  }

protected:
  PCAModelFactory();
  ~PCAModelFactory() ITK_OVERRIDE;

private:
  PCAModelFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};



} //namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "PCAModelFactory.txx"
#endif

#endif


