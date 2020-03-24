/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "otbGeometryMetadata.h"

#include <iostream>

namespace otb
{

OTB_GCP::OTB_GCP() : m_GCPCol(0), m_GCPRow(0), m_GCPX(0), m_GCPY(0), m_GCPZ(0)
{
}

OTB_GCP::~OTB_GCP()
{
}

void OTB_GCP::Print(std::ostream& os) const
{
  os << "   GCP Id = " << this->m_Id << std::endl;
  os << "   GCP Info =  " << this->m_Info << std::endl;
  os << "   GCP (Row, Col) = (" << this->m_GCPRow << "," << this->m_GCPCol << ")" << std::endl;
  os << "   GCP (X, Y, Z) = (" << this->m_GCPX << "," << this->m_GCPY << "," << this->m_GCPZ << ")" << std::endl;
}

} // end namespace otb
