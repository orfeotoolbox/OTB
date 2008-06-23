#ifndef ossimTerraSarModel_H
#define ossimTerraSarModel_H

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
 * @brief This class allows for direct localisation and indirect localisation using the TerraSar sensor model
 * @author Magellium, Vincent Martin
 * @version 1.0
 * @date 02-20-08
 */
class OSSIMDLLEXPORT ossimTerraSarModel : public ossimGeometricSarSensorModel
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
	virtual double getSlantRangeFromGeoreferenced(double col) const;

protected:

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

	/**
	 * @brief alternative SRGR computation coefficients
	 */
	double _alt_srgr_coefset[3];

private:
	virtual bool InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix);
	virtual bool InitSensorParams(const ossimKeywordlist &kwl, const char *prefix);
	virtual bool InitRefPoint(const ossimKeywordlist &kwl, const char *prefix);
	/**
	 * @brief Initializes the Slant Range to Ground Range data sets : 
	 *				_SrToGr_R0,_SrToGr_coeffs_number,_SrToGr_exponent,_SrToGr_coeffs,_nbCol
	 */
	virtual bool InitSRGR(const ossimKeywordlist &kwl, const char *prefix);

	bool UtcDateTimeStringToCivilDate(const std::string &utcString, CivilDateTime &outputDate);

	TYPE_DATA
};

#endif