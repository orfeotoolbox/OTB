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

#include "otbTileRecord.h"

namespace otb
{
TileRecord::TileRecord()
{
  m_FileName = "";
  m_Date = time(NULL);
}

TileRecord::~TileRecord()
{
}

void TileRecord::SetFileName(std::string _arg)
{
  m_FileName = _arg;
}

std::string TileRecord::GetFileName()
{
  return m_FileName;
}

void TileRecord::SetDate()
{
  m_Date = time(NULL);
}

time_t TileRecord::GetDate()
{
  return m_Date;
}
}
