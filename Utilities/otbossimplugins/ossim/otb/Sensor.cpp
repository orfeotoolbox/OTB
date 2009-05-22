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

#include <Sensor.h>
#include <SensorParams.h>
#include <PlatformPosition.h>

#include <cstring>

namespace ossimplugins
{


Sensor::Sensor(SensorParams* params, PlatformPosition* position)
{
	_params = params->Clone();
	_position = position->Clone();
}

Sensor::~Sensor()
{
	if(_params != NULL)
	{
		delete _params;
	}

	if(_position != NULL)
	{
		delete _position;
	}
}
}
