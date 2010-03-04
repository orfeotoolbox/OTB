/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbOGRVectorDataIOFactory_txx
#define __otbOGRVectorDataIOFactory_txx

#include "otbOGRVectorDataIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"
#include "otbOGRVectorDataIO.h"

namespace otb
{
template<class TData>
OGRVectorDataIOFactory<TData>::OGRVectorDataIOFactory()
{
  this->RegisterOverride("otbVectorDataIOBase",
                         "otbOGRVectorDataIO",
                         "OGR Vectordata IO",
                         1,
                         itk::CreateObjectFunction<OGRVectorDataIO<TData> >::New());
}
template<class TData>
OGRVectorDataIOFactory<TData>::~OGRVectorDataIOFactory()
{
}
template<class TData>
const char*
OGRVectorDataIOFactory<TData>::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}
template<class TData>
const char*
OGRVectorDataIOFactory<TData>::GetDescription() const
{
  return "OGR VectorDataIO Factory, allows the loading of ogr supported vector data into OTB";
}

} // end namespace otb

#endif
