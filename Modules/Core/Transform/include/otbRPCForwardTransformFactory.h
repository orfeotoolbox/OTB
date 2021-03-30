#ifndef otbRPCForwardTransformFactory_h
#define otbRPCForwardTransformFactory_h

#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

namespace otb
{
/** \class RPCForwardTransformFactory
 * \brief Creation d'un instance d'un objet RPCForwardTransformFactory utilisant les object factory.
 *
 * \ingroup OTBCore
 */
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
class ITK_EXPORT RPCForwardTransformFactory : public itk::ObjectFactoryBase
{
public:
	/** Standard class typedefs. */
	typedef RPCForwardTransformFactory Self;
	typedef itk::ObjectFactoryBase            Superclass;
	typedef itk::SmartPointer<Self>           Pointer;
	typedef itk::SmartPointer<const Self>     ConstPointer;

	/** Class methods used to interface with the registered factories. */
	const char* GetITKSourceVersion(void) const override;
	const char* GetDescription(void) const override;

	/** Method for class instantiation. */
	itkFactorylessNewMacro(Self);

	/** Run-time type information (and related methods). */
	itkTypeMacro(RPCForwardTransformFactory, itk::ObjectFactoryBase);

	/** Register one factory of this type  */
	static void RegisterOneFactory(void)
	{
		Pointer RPCForwardFactory = RPCForwardTransformFactory::New();
		itk::ObjectFactoryBase::RegisterFactory(RPCForwardFactory);
	}

protected:
	RPCForwardTransformFactory();
	~RPCForwardTransformFactory() override = default;

private:
	RPCForwardTransformFactory(const Self&) = delete;
	void operator=(const Self&) = delete;
};
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRPCForwardTransformFactory.hxx"
#endif

#endif // OTBRPCFORWARDTRANSFORMFACTORY_H
