//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include "otb/Sensor.h"

namespace ossimplugins
{

Sensor::Sensor(SensorParams const& params, PlatformPosition const& position)
   : m_params(params)
   , m_position(position)
   {}

Sensor::~Sensor()
{}

}
