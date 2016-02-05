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
#include "otbGDALDriverManagerWrapper.h"

#include <vector>

#include "otb_boost_string_header.h"
#include "otbSystem.h"

namespace otb
{

GDALDatasetWrapper
::GDALDatasetWrapper(): m_Dataset(NULL)
{
}

GDALDatasetWrapper
::~GDALDatasetWrapper()
{
  if( m_Dataset )
    {
    GDALClose(m_Dataset);
    m_Dataset = NULL;
    }
}

// GetDataSet
const GDALDataset *
GDALDatasetWrapper
::GetDataSet() const
{
  return m_Dataset;
}


GDALDataset *
GDALDatasetWrapper
::GetDataSet()
{
  return m_Dataset;
}


// IsJPEG2000
bool
GDALDatasetWrapper::IsJPEG2000() const
{
  if (m_Dataset == NULL)
    {
    return false;
    }
  std::string driverName(m_Dataset->GetDriver()->GetDescription());
  if (driverName.compare("JP2OpenJPEG") == 0 ||
      driverName.compare("JP2KAK") == 0 ||
      driverName.compare("JP2ECW") == 0)
    {
    return true;
    }
  return false;
}

} // end namespace otb
