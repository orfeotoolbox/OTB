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


#include "otbLeafParameters.h"


namespace otb
{


/**
 * Constructor
 */

LeafParameters::LeafParameters()
{
  m_Cab    = 30.0;
  m_Car    = 10.0;
  m_CBrown = 0.0;
  m_Cw     = 0.015;
  m_Cm     = 0.009;
  m_N      = 1.2;
}

/**
 * Destructor
 */
LeafParameters::~LeafParameters()
{
}

/**PrintSelf method */
void LeafParameters::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << "Chlorophyll content       : " << m_Cab << std::endl;
  os << "Carotenoid content        : " << m_Car << std::endl;
  os << "Brown pigment content     : " << m_CBrown << std::endl;
  os << "EWT                       : " << m_Cw << std::endl;
  os << "LMA                       : " << m_Cm << std::endl;
  os << "structure coef            : " << m_N << std::endl;
}


} // end namespace otb
