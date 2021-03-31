#ifndef otbRPCForwardTransformFactory_hxx
#define otbRPCForwardTransformFactory_hxx

#include "otbRPCForwardTransformFactory.h"
#include "otbRPCForwardTransform.h"
#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace otb
{
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
RPCForwardTransformFactory<TScalarType, NInputDimensions,NOutputDimensions>::RPCForwardTransformFactory()
{
  this->RegisterOverride("RPCTransformBase", "RPCForwardTransform", "RPC Forward Transform", 1,
                           itk::CreateObjectFunction<RPCForwardTransform<TScalarType,NInputDimensions,NOutputDimensions>>::New());
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
const char* RPCForwardTransformFactory<TScalarType, NInputDimensions,NOutputDimensions>::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
const char* RPCForwardTransformFactory<TScalarType, NInputDimensions,NOutputDimensions>::GetDescription() const
{
  return "RPC Forward Transform factory";
}

}

#endif
