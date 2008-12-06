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
#ifndef __otbKMLVectorDataIOFactory_txx
#define __otbKMLVectorDataIOFactory_txx

#include "otbKMLVectorDataIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"
#include "otbKMLVectorDataIO.h"

namespace otb
{
template<class TData>
KMLVectorDataIOFactory<TData>::KMLVectorDataIOFactory()
{
  this->RegisterOverride("otbVectorDataIOBase",
                         "otbKMLVectorDataIO",
                         "KML Vectordata IO",
                         1,
                         itk::CreateObjectFunction<KMLVectorDataIO<TData> >::New());
}
template<class TData>
KMLVectorDataIOFactory<TData>::~KMLVectorDataIOFactory()
{
}
template<class TData>
const char*
KMLVectorDataIOFactory<TData>::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}
template<class TData>
const char*
KMLVectorDataIOFactory<TData>::GetDescription() const
{
  return "KML VectorDataIO Factory, allows the loading of KML vector data into OTB";
}

} // end namespace otb

#endif
