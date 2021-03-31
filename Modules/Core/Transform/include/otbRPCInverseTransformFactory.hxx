#ifndef otbRPCInverseTransformFactory_hxx
#define otbRPCInverseTransformFactory_hxx

#include "otbRPCInverseTransformFactory.h"
#include "otbRPCInverseTransform.h"
#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace otb
{
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
RPCInverseTransformFactory<TScalarType, NInputDimensions,NOutputDimensions>::RPCInverseTransformFactory()
{
  this->RegisterOverride("otbSensorTransformBase", "otbRPCInverseTransform", "RPC Inverse Transform", 1,
                           itk::CreateObjectFunction<RPCInverseTransform<TScalarType,NInputDimensions,NOutputDimensions>>::New());
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
const char* RPCInverseTransformFactory<TScalarType, NInputDimensions,NOutputDimensions>::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
const char* RPCInverseTransformFactory<TScalarType, NInputDimensions,NOutputDimensions>::GetDescription() const
{
  return "RPC Inverse Transform factory";
}

}

#endif
