#include <ossim/projection/SARModel/Sensor.h>
#include <ossim/projection/SARModel/SensorParams.h>
#include <ossim/projection/SARModel/PlatformPosition.h>

#include <string.h>

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
