#ifndef ossimRadarSat2Model_H
#define ossimRadarSat2Model_H

#include <ossim/projection/ossimGeometricSarSensorModel.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <iostream>
#include <list>

#include <ossim/projection/SARModel/DateTime/CivilDateTime.h>

class PlatformPosition;
class SensorParams;
class RefPoint;
/**
 * @brief This class allows for direct localisation and indirect localisation using the RadarSat2 sensor model
 * @author Magellium, Vincent Martin
 * @version 1.0
 * @date 03-20-08
 */
class OSSIMDLLEXPORT ossimRadarSat2Model : public ossimGeometricSarSensorModel
{
public:
	/**
	 * @brief Constructor
	 */
	ossimRadarSat2Model();

	/**
	 * @brief Destructor
	 */
	~ossimRadarSat2Model();

	/**
	 * @brief This function associates an image column number to a slant range when the image is georeferenced (ground projected)
	 * @param col Column coordinate of the image point
	 */
	virtual double getSlantRangeFromGeoreferenced(double col) const;

protected:

	/**
	 * @brief Number of columns
	 */
	double _nbCol;
	/**
	 * @brief Pixel spacing
	 */
	double _pixel_spacing;
	/**
	 *	@brief Slant Range FOR EACH Ground Range (SRGR) number of coefficients sets
	 */
	int   _n_srgr;
	/**
	 * @brief Slant Range FOR EACH Ground Range coefficient sets update times
	 */
	std::vector<double> _srgr_update;
	/**
	* @brief Slant Range FOR EACH Ground Range Projection reference point
	*/
	std::vector<double> _SrGr_R0 ;
	/**
	* @brief Slant Range FOR EACH Ground Range Projection coefficients
	*/
	std::vector< std::vector<double> > _SrGr_coeffs ;

private:
	virtual bool InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix);
	virtual bool InitSensorParams(const ossimKeywordlist &kwl, const char *prefix);
	virtual bool InitRefPoint(const ossimKeywordlist &kwl, const char *prefix);
	/**
	 * @brief Initializes the Slant Range to Ground Range data sets : 
	 *				_srgr_update,_SrGr_R0,_SrGr_coeffs_number,_SrGr_coeffs,_nbCol,_pixel_spacing
	 */
	virtual bool InitSRGR(const ossimKeywordlist &kwl, const char *prefix);

	bool UtcDateTimeStringToCivilDate(const std::string &utcString, CivilDateTime &outputDate);
	/**
	 * @brief Finds the SRGR data set which update time is the closest to the center scene time
	 */
	int FindSRGRSetNumber(JSDDateTime date)  const;

	TYPE_DATA
};

#endif