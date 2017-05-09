#ifndef AutoencoderModelFactory_h
#define AutoencoderModelFactory_h


#include "itkObjectFactoryBase.h"

namespace otb
{
	
template <class TInputValue, class TTargetValue>
class ITK_EXPORT AutoencoderModelFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef AutoencoderModelFactory             Self;
  typedef itk::ObjectFactoryBase        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class methods used to interface with the registered factories. */
  const char* GetITKSourceVersion(void) const ITK_OVERRIDE;
  const char* GetDescription(void) const ITK_OVERRIDE;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(AutoencoderModelFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    Pointer RFFactory = AutoencoderModelFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(RFFactory);
  }

protected:
  AutoencoderModelFactory();
  ~AutoencoderModelFactory() ITK_OVERRIDE;

private:
  AutoencoderModelFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};
} //namespace otb

#endif


