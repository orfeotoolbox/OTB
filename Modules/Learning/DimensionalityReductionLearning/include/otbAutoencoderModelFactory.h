#ifndef AutoencoderModelFactory_h
#define AutoencoderModelFactory_h


//#include <shark/Models/TiedAutoencoder.h>
//#include <shark/Models/Autoencoder.h>
#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

namespace otb
{
	
template <class TInputValue, class TTargetValue, class NeuronType>
class ITK_EXPORT AutoencoderModelFactoryBase : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef AutoencoderModelFactoryBase   Self;
  typedef itk::ObjectFactoryBase        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class methods used to interface with the registered factories. */
  const char* GetITKSourceVersion(void) const ITK_OVERRIDE;
  const char* GetDescription(void) const ITK_OVERRIDE;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(AutoencoderModelFactoryBase, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    Pointer AEFactory = AutoencoderModelFactoryBase::New();
    itk::ObjectFactoryBase::RegisterFactory(AEFactory);
  }

protected:
  AutoencoderModelFactoryBase();
  ~AutoencoderModelFactoryBase() ITK_OVERRIDE;

private:
  AutoencoderModelFactoryBase(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};



/*
template <class TInputValue, class TTargetValue>
using AutoencoderModelFactory = AutoencoderModelFactoryBase<TInputValue, TTargetValue, shark::Autoencoder< shark::TanhNeuron, shark::LinearNeuron>>  ;


template <class TInputValue, class TTargetValue>
using TiedAutoencoderModelFactory = AutoencoderModelFactoryBase<TInputValue, TTargetValue, shark::TiedAutoencoder< shark::TanhNeuron, shark::LinearNeuron>>  ;
*/

} //namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbAutoencoderModelFactory.txx"
#endif

#endif


