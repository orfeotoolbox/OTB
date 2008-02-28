#include <ossim/projection/SARModel/Sensor.h>
#include <ossim/projection/SARModel/SensorParams.h>
#include <ossim/projection/SARModel/RefPoint.h>
#include <ossim/projection/SARModel/PlatformPosition.h>

#include <string.h>

Sensor::Sensor(SensorParams* params, PlatformPosition* position, RefPoint* refpoint)
{
	_params = params->Clone();
	_position = position->Clone();
	_refpoint = refpoint->Clone();
	
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
	
	if(_refpoint != NULL)
	{
		delete _refpoint;
	}
}
