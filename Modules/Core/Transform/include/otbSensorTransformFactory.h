/*
 * Copyright (C) 2005-2021 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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
 * \ingroup OTBTransform
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
