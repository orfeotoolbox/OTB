#ifndef otbDEMHandler_h
#define otbDEMHandler_h

#include "otbImage.h"
#include "otbGDALDriverManagerWrapper.h"

// C++ 17 : use std::optional instead
#include <boost/optional.hpp>

namespace otb
{
/**
 * GetHeightAboveEllipsoid():
 * - SRTM and geoid both available: srtm_value + geoid_offset
 * - No SRTM but geoid available: geoid_offset
 * - SRTM available, but no geoid: srtm_value
 * - No SRTM and no geoid available: default height above ellipsoid
 *
 * GetHeightAboveMSL():
 * - SRTM and geoid both available: srtm_value
 * - No SRTM but geoid available: 0
 * - SRTM available, but no geoid: srtm_value
 * - No SRTM and no geoid available: 0
 * 
*/

class DEMHandler: public itk::Object
{
public:
  /** Standard class typedefs. */
  using Self =          DEMHandler;
  using Superclass =    itk::Object;
  using Pointer =       itk::SmartPointer<Self>;
  using ConstPointer =  itk::SmartPointer<const Self>;

  using PointType =     itk::Point<double, 2>;

  /** Retrieve the singleton instance */
  static Pointer Instance();

  /** Run-time type information (and related methods). */
  itkTypeMacro(DEMHandler, Object);

  /** Try to open the DEM directory. */
  void OpenDEMFile(const std::string & path);

  /** Try to open the DEM directory. */
  void OpenDEMDirectory(const char* DEMDirectory);
  void OpenDEMDirectory(const std::string& DEMDirectory);

  /** return true if the directory contain raster */
  bool IsValidDEMDirectory(const char* DEMDirectory);

  bool OpenGeoidFile(const char* geoidFile);

  bool OpenGeoidFile(const std::string& geoidFile);

  double GetHeightAboveEllipsoid(double lon, double lat);

  double GetHeightAboveEllipsoid(const PointType& geoPoint);

  double GetHeightAboveMSL(double lon, double lat);

  double GetHeightAboveMSL(const PointType& geoPoint);

  unsigned int GetDEMCount() const;
  
  itkGetMacro(DefaultHeightAboveEllipsoid, double);
  itkSetMacro(DefaultHeightAboveEllipsoid, double);

  /** Get DEM directory */
  std::string GetDEMDirectory(unsigned int idx = 0) const;

  /** Get Geoid file */
  std::string GetGeoidFile() const;

  void ClearDEMs();

protected: 
  DEMHandler(); 

  ~DEMHandler() = default;

  boost::optional<double> GetDEMValue(double lon, double lat, GDALDataset* ds);
  
  void CreateShiftedDataset();
  
private:

private:
  DEMHandler(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** List of RAII capsules on all opened DEM datasets for memory management */
  std::vector<otb::GDALDatasetWrapper::Pointer> m_DatasetList;
  
  /** Pointer to the DEM dataset */
  GDALDataset * m_Dataset;

  GDALDataset* m_shiftedDS;
  
  /** Pointer to the geoid dataset */
  GDALDataset* m_GeoidDS;
  
  bool m_ApplyVerticalDatum;
  
  GDALRasterIOExtraArg m_RasterIOArgs;

  static Pointer m_Singleton;

  /** Default height above elliposid, used when no DEM or geoid height is available. */
  double m_DefaultHeightAboveEllipsoid;

  /** List of the DEM directories currently opened */
  std::vector<std::string> m_DEMDirectories;

  /** Filename of the current geoid */
  std::string m_GeoidFilename;

};

}
#endif
