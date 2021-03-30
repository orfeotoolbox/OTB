#ifndef otbRPCInverseTransformFactory_h
#define otbRPCInverseTransformFactory_h
#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

namespace otb
{
/** \class RPCInverseTransformFactory
 * \brief Creation d'un instance d'un objet RPCInverseTransform utilisant les object factory.
 *
 * \ingroup OTBCore
 */
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
class ITK_EXPORT RPCInverseTransformFactory : public itk::ObjectFactoryBase
{
public:
	/** Standard class typedefs. */
	typedef RPCInverseTransformFactory Self;
	typedef itk::ObjectFactoryBase            Superclass;
	typedef itk::SmartPointer<Self>           Pointer;
	typedef itk::SmartPointer<const Self>     ConstPointer;

	/** Class methods used to interface with the registered factories. */
	const char* GetITKSourceVersion(void) const override;
	const char* GetDescription(void) const override;

	/** Method for class instantiation. */
	itkFactorylessNewMacro(Self);

	/** Run-time type information (and related methods). */
	itkTypeMacro(RPCInverseTransformFactory, itk::ObjectFactoryBase);

	/** Register one factory of this type  */
	static void RegisterOneFactory(void)
	{
		Pointer RPCInverseFactory = RPCInverseTransformFactory::New();
		itk::ObjectFactoryBase::RegisterFactory(RPCInverseFactory);
	}

protected:
	RPCInverseTransformFactory();
	~RPCInverseTransformFactory() override = default;

private:
	RPCInverseTransformFactory(const Self&) = delete;
	void operator=(const Self&) = delete;
};
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRPCInverseTransformFactory.hxx"
#endif

#endif // OTBRPCINVERSETRANSFORMFACTORY_H
