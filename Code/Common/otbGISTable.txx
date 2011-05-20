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
#ifndef __otbGISTable_txx
#define __otbGISTable_txx

#include "otbGISTable.h"
#include "itkPreOrderTreeIterator.h"
#include "otbMetaDataKey.h"

namespace otb
{

template <class TConnectionImplementation, class TPrecision, unsigned int TSpatialDimension>
GISTable<TConnectionImplementation, TPrecision, TSpatialDimension>
::GISTable()
{
  m_TableName = "";
  m_Connection = ConnectionType::New();
  m_Srid = -1;
}

template <class TConnectionImplementation, class TPrecision, unsigned int TSpatialDimension>
void
GISTable<TConnectionImplementation, TPrecision, TSpatialDimension>
::SetProjectionRef(const std::string& projectionRef)
{
  itk::MetaDataDictionary& dict = this->GetMetaDataDictionary();

  itk::EncapsulateMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, projectionRef);
  this->Modified();
}

template <class TConnectionImplementation, class TPrecision, unsigned int TSpatialDimension>
std::string
GISTable<TConnectionImplementation, TPrecision, TSpatialDimension>
::GetProjectionRef() const
{
  const itk::MetaDataDictionary& dict = this->GetMetaDataDictionary();

  std::string projectionRef;
  itk::ExposeMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, projectionRef);

  return projectionRef;
}

template <class TConnectionImplementation, class TPrecision, unsigned int TSpatialDimension>
void
GISTable<TConnectionImplementation, TPrecision, TSpatialDimension>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << std::endl;

  os << "Table name: " << m_TableName << std::endl;

}

} // end namespace otb

#endif
