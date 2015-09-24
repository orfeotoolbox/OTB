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
#include "otbNoDataHelper.h"

#include "itkMetaDataDictionary.h"
#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"

namespace otb
{

bool ReadNoDataFlags(const itk::MetaDataDictionary& dict, std::vector<bool> & flags, std::vector<double> & values)
{
  bool ret = itk::ExposeMetaData<std::vector<bool> >(dict,MetaDataKey::NoDataValueAvailable,flags);

  if (ret)
    ret = itk::ExposeMetaData<std::vector<double> >(dict,MetaDataKey::NoDataValue,values);
  
  return ret;
}

void WriteNoDataFlags(const std::vector<bool> & flags, const std::vector<double> & values, itk::MetaDataDictionary& dict)
{
 itk::EncapsulateMetaData<std::vector<bool> >(dict,MetaDataKey::NoDataValueAvailable,flags);
 itk::EncapsulateMetaData<std::vector<double> >(dict,MetaDataKey::NoDataValue,values);
}

} // End namespace otb
