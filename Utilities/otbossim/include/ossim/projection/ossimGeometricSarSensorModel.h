#ifndef ossimGeometricSarSensorModel_H
#define ossimGeometricSarSensorModel_H

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
 * @brief This class allows for direct localisation and indirect localisation using the geometric model of SAR sensors.
 *				This class must be specified for each sensor. 
 * @author Magellium, Vincent Martin
 * @version 1.0
 * @date 03-18-08
 */
class OSSIMDLLEXPORT ossimGeometricSarSensorModel : public ossimSensorModel
{
public:
	/**
	 * @brief Constructor
	 */
	ossimGeometricSarSensorModel();

	/**
	 * @brief Destructor
	 */
	~ossimGeometricSarSensorModel();

	/**
	 * @brief This function associates an image column number to a slant range when the image is georeferenced (ground projected)
	 * @param col Column coordinate of the image point
	 */
	virtual double getSlantRangeFromGeoreferenced(double col) const=0;

	/**
	 * @brief This function associates an image column number to a slant range
	 * @param col Column coordinate of the image point
	 */
	virtual double getSlantRange(double col) const;

	/**
	 * @brief This function associates an image line number to an azimuth time
	 * @param line Line coordinate of the image point
	 */
	virtual JSDDateTime getTime(double line) const;

	/**
	 * @brief This function is able to convert image coordinates into world coordinates the geometric model of SAR sensors.
	 * @param image_point Coordinates of the image point
	 * @param heightEllipsoid Altitude of the world point
	 * @param worldPoint Coordinates of the world point (OUT)
	 */
	virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                        const double&   heightEllipsoid,
                                        ossimGpt&       worldPoint) const;


	/**
	 * @brief This function optimizes the model according to a list of Ground Control Points. 
	 *		The input list of GCPs is appended to the one present in memory. 
	 *		An inverse localization of each ground GCP is performed. 
	 *		The error between localized GCPs and actual image coordinates is then computed and 
	 *		modeled by a linear model on each dimension. 
	 *		The time and range estimations are updated accordingly. 
	 *
	 * @param groundCoordinates : input GCP ground coordinates
	 * @param imageCoordinates : actual image coordinates corresponding to input groundCoordinates
	 *
	 */
	virtual bool optimizeModel(const std::list<ossimGpt> & groundCoordinates, const std::list<ossimDpt> & imageCoordinates) ;

	/**
   * @brief Clears _optimizationGCPsGroundCoordinates and _optimizationGCPsImageCoordinates attributes
	*			Updates the optimisation factors and bias
	*/
	virtual void clearGCPlist() ; 

	/**
   * @brief Returns _optimizationGCPsGroundCoordinates and _optimizationGCPsImageCoordinates attributes
	*
	* @retval groundCoordinates : GCP ground coordinates
	* @retval imageCoordinates : actual image coordinates corresponding to groundCoordinates
	*/
	virtual void getGCPlist(std::list<ossimGpt> &groundCoordinates, std::list<ossimDpt> & imageCoordinates) ; 

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
	
	/**
    * @brief Accessors to the optimization parameters.
	*/
	double get_optimizationFactorX() { return _optimizationFactorX; }; 
	double get_optimizationFactorY() { return _optimizationFactorY; } ; 
	double get_optimizationBiasX() { return _optimizationBiasX; } ; 
	double get_optimizationBiasY() { return _optimizationBiasY; } ;

protected:

	/**
	 * @brief Handle the position of the platform
	 */
	PlatformPosition *_platformPosition;
	SensorParams * _sensor; 
	RefPoint * _refPoint;

	/**
	 * @brief True iff the product is ground range
	 */
	bool _isProductGeoreferenced ;

	/**
	 * @brief List Ground Control Points used by the optimization
	 */
	std::list<ossimGpt> _optimizationGCPsGroundCoordinates ;
	std::list<ossimDpt> _optimizationGCPsImageCoordinates  ;

	/**
	 * @brief Optimization result : linear error correction in both dimensions
	 */
	double _optimizationFactorX ; 
	double _optimizationFactorY ; 
	double _optimizationBiasX ; 
	double _optimizationBiasY ;

private:
	/**
	 * @brief Initializes the Platform Position from a projection keywordlist
	 */
	virtual bool InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix)=0;
	/**
	 * @brief Initializes the Sensor Params from a projection keywordlist
	 */
	virtual bool InitSensorParams(const ossimKeywordlist &kwl, const char *prefix)=0;
	/**
	 * @brief Initializes the Reference Point from a projection keywordlist
	 */
	virtual bool InitRefPoint(const ossimKeywordlist &kwl, const char *prefix)=0;
	/**
	 * @brief Initializes the Slant Range to/for each Ground Range data sets from a projection keywordlist
	 */
	virtual bool InitSRGR(const ossimKeywordlist &kwl, const char *prefix)=0;

	TYPE_DATA
};

#endif