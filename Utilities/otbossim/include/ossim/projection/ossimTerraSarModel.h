#ifndef ossimTerraSarModel_H
#define ossimTerraSarModel_H

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
#include <list>

#include <ossim/projection/SARModel/DateTime/CivilDateTime.h>

class PlatformPosition;
class SensorParams;
class RefPoint;
/**
 * @brief This class allows for direct localisation and indirect localisation using the TerraSar sensor model
 * @author Magellium, Vincent Martin
 * @version 1.0
 * @date 02-20-08
 */
class OSSIMDLLEXPORT ossimTerraSarModel : public ossimSensorModel
{
public:
	/**
	 * @brief Constructor
	 */
	ossimTerraSarModel();

	/**
	 * @brief Destructor
	 */
	~ossimTerraSarModel();

	/**
	 * @brief This function associates an image column number to a slant range when the image is georeferenced (ground projected)
	 * @param col Column coordinate of the image point
	 */
	double getSlantRangeFromGeoreferenced(double col) const;

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
	 * @brief This function is able to convert image coordonnates into world coordinates using the TerraSar sensor model
	 * @param image_point Coordinates of the image point
	 * @param heightEllipsoid Altitude of the world point
	 * @param worldPoint Coordinates of the world point (OUT)
	 */
	virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                        const double&   heightEllipsoid,
                                        ossimGpt&       worldPoint) const;


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
	 * @brief Number of columns
	 */
	double _nbCol;
	/**
	* @brief Slant Range TO Ground Range Projection reference point
	*/
	double _SrToGr_R0 ;
	/**
	* @brief Slant Range TO Ground Range Projection exponents
	*/
	std::vector<int> _SrToGr_exponent ;
		/**
	* @brief Slant Range TO Ground Range Projection coefficients
	*/
	std::vector<double> _SrToGr_coeffs ;
	/**
	 * @brief Scene Center range time
	 */
	double _sceneCenterRangeTime;
	/**
	 * @brief Slant Range TO Ground Range scaling factor at scene center
	 */
	double _SrToGr_scaling_factor ;

private:
	bool InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix);
	bool InitSensorParams(const ossimKeywordlist &kwl, const char *prefix);
	bool InitRefPoint(const ossimKeywordlist &kwl, const char *prefix);
	/**
	 * @brief Initializes the Slant Range to Ground Range data sets : 
	 *				_SrToGr_R0,_SrToGr_coeffs_number,_SrToGr_exponent,_SrToGr_coeffs,_nbCol
	 */
	bool InitSRGR(const ossimKeywordlist &kwl, const char *prefix);

	bool UtcDateTimeStringToCivilDate(const std::string &utcString, CivilDateTime &outputDate);

	TYPE_DATA
};

#endif
