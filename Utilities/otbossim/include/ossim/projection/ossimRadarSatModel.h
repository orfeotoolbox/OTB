#ifndef ossimRadarSatModel_H
#define ossimRadarSatModel_H

#include <ossim/projection/ossimSensorModel.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimEcefRay.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimMatrix3x3.h>
#include <iostream>


class PlatformPosition;
class SensorParams;
class RefPoint;
/**
 * @brief This class is able to direct localisation and indirect localisation using the RadarSat sensor model
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 04-01-08
 */
class OSSIMDLLEXPORT ossimRadarSatModel : public ossimSensorModel
{
public:
	/**
	 * @brief Constructor
	 */
	ossimRadarSatModel();

	/**
	 * @brief Destructor
	 */
	~ossimRadarSatModel();

	/**
	 * @brief This function is able to convert image coordonnates in world coordinates using the RadarSat sensor model
	 * @param image_point Coordinates of the image point
	 * @param heightEllipsoid Altitude of the world point
	 * @param worldPoint Coordinates of the world point (OUT)
	 */
	virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                        const double&   heightEllipsoid,
                                        ossimGpt&       worldPoint) const;

	/**
     * @brief Returns pointer to a new instance, copy of this.
	 * @remark This function always return NULL;
     */
   virtual ossimObject* dup() const { return 0; };

   /**
    * @brief
	* @return true when it's better (more accurate / fast) to use forward (from ground to image) than inverse(from image to ground)
	* @remark This function always return false
	*/
   inline virtual bool useForward()const {return false;};

   /**
    * @brief Method to the load (recreate) the state of the object from a keyword list. Return true if ok or false on error.
	* @return true if load OK, false on error
	*/
   virtual bool loadState (const ossimKeywordlist &kwl, const char *prefix=0);
protected:

	/**
	 * @brief Handle the position of the platform
	 */
	PlatformPosition *_platformPosition;
	SensorParams * _sensor; 
	RefPoint * _refPoint;
private:
	bool InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix);
	bool InitSensorParams(const ossimKeywordlist &kwl, const char *prefix);
	bool InitRefPoint(const ossimKeywordlist &kwl, const char *prefix);
	TYPE_DATA
};

#endif