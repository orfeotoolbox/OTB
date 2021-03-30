#ifndef otbSensorTransformFactory_h
#define otbSensorTransformFactory_h
#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkMutexLock.h"
#include "otbSensorTransformBase.h"
#include "otbGenericMapProjection.h"

namespace otb
{
/** \class SensorTransformFactory
 * \brief Creation of object instance using object factory.
 *
 * \ingroup OTBCore
 */
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
class SensorTransformFactory : public itk::Object
{
public:
	/** Standard class typedefs. */
	typedef SensorTransformFactory   Self;
	typedef itk::Object                   Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;
	typedef typename SensorTransformBase<TScalarType, NInputDimensions,NOutputDimensions>::Pointer SensorTransformTypePointer;

	/** Class Methods used to interface with the registered factories */

	/** Run-time type information (and related methods). */
	itkTypeMacro(SensorTransformFactory, itk::Object);

	/** Create the appropriate transform. */
    static SensorTransformTypePointer CreateTransform(const ImageMetadata &imd,TransformDirection::TransformationDirection d);

	static void CleanFactories();

protected:
	SensorTransformFactory();
	~SensorTransformFactory() override;

private:
	SensorTransformFactory(const Self&) = delete;
	void operator=(const Self&) = delete;

	/** Register Built-in factories */
	static void RegisterBuiltInFactories();

	/** Register a single factory, ensuring it has not been registered
		* twice */
	static void RegisterFactory(itk::ObjectFactoryBase* factory);
};
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSensorTransformFactory.hxx"
#endif

#endif // OTBSENSORTRANSFORMFACTORY_H
