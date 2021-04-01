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
#ifndef otbRPCInverseTransformFactory_h
#define otbRPCInverseTransformFactory_h
#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

namespace otb
{
/** \class RPCInverseTransformFactory
 * \brief Creates an instance of RPCInverseTransform using itk object factory.
 *
 * \ingroup OTBTransform
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
