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
#include <ossim/projection/SARModel/DateTime/JSDDateTime.h>

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
	 * @brief This function associates an image column number to a slant range
	 * @param col Column coordinate of the image point
	 */
	double getSlantRange(double col) const;

	/**
	 * @brief This function associates an image line number to an azimuth time
	 * @param line Line coordinate of the image point
	 */
	JSDDateTime getTime(double line) const;

	/**
	 * @brief This function associates an image column number to a slant range when the image is georeferenced (ground projected)
	 * @param col Column coordinate of the image point
	 */
	double getSlantRangeFromGeoreferenced(double col) const;

	/**
	 * @brief This function is able to convert image coordonnates in world coordinates using the RadarSat sensor model
	 * @param image_point Coordinates of the image point
	 * @param heightEllipsoid Altitude of the world point
	 * @param worldPoint Coordinates of the world point (OUT)
	 */
	virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                        const double&   heightEllipsoid,
                                        ossimGpt&       worldPoint) const;

  void lineSampleToWorld(const ossimDpt& image_point,
                                         ossimGpt&       worldPoint)
    const;

	/**
	 * @brief This function optimizes the model according to a list of Ground Control Points. 
	 *		An inverse localization of each ground GCP is performed. The mean shift between localized GCPs and actual image coordinates 
	 *		is then computed. The Reference Point from the model is updated accordingly. 
	 *
	 * @param groundCoordinates : GCP ground coordinates
	 * @param imageCoordinates : actual image coordinates corresponding to groundCoordinates
	 */
	bool optimizeModel(std::list<ossimGpt> groundCoordinates, std::list<ossimDpt> imageCoordinates) ;
	
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

	/**
	 * @brief True iff the RadarSat product is a SGX or SGF product
	 */
	bool _isProductGeoreferenced ;

	/**
	 *	@brief Slant Range for each Ground Range (SRGR) number of coefficients sets
	 */
	int   _n_srgr;
	/**
	 * @brief SRGR coefficient sets
	 */
	double _srgr_coefset[20][6];
	/**
	 * @brief SRGR coefficient sets update times
	 */
	double _srgr_update[20];
	/**
	 * @brief Pixel spacing
	 */
	double _pixel_spacing;

private:
	bool InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix);
	bool InitSensorParams(const ossimKeywordlist &kwl, const char *prefix);
	bool InitRefPoint(const ossimKeywordlist &kwl, const char *prefix);
	/**
	 * @brief Initializes the Slant Range for each Ground Range data sets : _n_srgr,_srgr_coefset,_srgr_update,_pixel_spacing,_isProductGeoreferenced
	 */
	bool InitSRGR(const ossimKeywordlist &kwl, const char *prefix);
	/**
	 * @brief Finds the SRGR data set which update time is the closest to the center scene time
	 */
	int FindSRGRSetNumber(JSDDateTime date)  const;
	TYPE_DATA
	
};

#endif
