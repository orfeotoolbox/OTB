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

#ifndef ossimEnvisatAsarModel_H
#define ossimEnvisatAsarModel_H

#include <otb/JSDDateTime.h>
#include <ossimGeometricSarSensorModel.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <EnvisatAsar/EnvisatAsarData.h>

#include <iostream>

namespace ossimplugins
{


class PlatformPosition;
class SensorParams;
class RefPoint;
/**
 * @brief This class is able to direct localisation and indirect localisation
 * using the EnvisatAsar sensor model
 */
class ossimEnvisatAsarModel : public ossimGeometricSarSensorModel
{
public:
  /**
   * @brief Constructor
   */
  ossimEnvisatAsarModel();

  /**
   * @brief Destructor
   */
  virtual ~ossimEnvisatAsarModel();

  /**
   * @brief Method to return the class name.
   * @return The name of this class.
   */
  virtual ossimString getClassName()   const;

  /**
   * @brief Returns pointer to a new instance, copy of this.
   */
  virtual ossimObject* dup() const;

  /**
   * @brief This function associates an image column number to a slant range when the image is georeferenced (ground projected)
   * @param col Column coordinate of the image point
   */
  virtual double getSlantRangeFromGeoreferenced(double col) const;

  /**
   * @brief Method to intantial model from a file.
   *
   * @param file
   *
   * @return true on success, false on error.
   */
  bool open(const ossimFilename& file);

  /**
   * @brief Method to save object state to a keyword list.
   * @param kwl Keyword list to save to.
   * @param prefix added to keys when saved.
   * @return true on success, false on error.
   */
  virtual bool saveState(ossimKeywordlist& kwl,
                         const char* prefix=0) const;

  /**
   * @brief Method to the load (recreate) the state of the object from a
   * keyword list. Return true if ok or false on error.
   * @return true if load OK, false on error
   */
  virtual bool loadState (const ossimKeywordlist &kwl, const char *prefix=0);

  /*!
   * METHOD: print()
   * Fulfills base-class pure virtual. Dumps contents of object to ostream.
   */
  virtual std::ostream& print(std::ostream& out) const;

protected:

  /**
   *  @brief Slant Range to Ground Range (SRGR) number of coefficients sets
   */
  int  _n_srgr;
  /**
   * @brief SRGR coefficient sets
   */
  std::vector< std::vector<double> > _srgr_coefset;
  /**
   * @brief SRGR coefficient sets update times
   */
  std::vector<JSDDateTime> _srgr_update;
  /**
   * @brief Pixel spacing
   */
  double _pixel_spacing;

private:
  virtual bool InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix);
  virtual bool InitSensorParams(const ossimKeywordlist &kwl, const char *prefix);
  virtual bool InitRefPoint(const ossimKeywordlist &kwl, const char *prefix);
  /**
   * @brief Initializes the Slant Range for each Ground Range data sets : _n_srgr,_srgr_coefset,_srgr_update,_pixel_spacing,_isProductGeoreferenced
   */
  virtual bool InitSRGR(const ossimKeywordlist &kwl, const char *prefix);
  /**
   * @brief Finds the SRGR data set which update time is the closest to the center scene time
   */
  std::vector<double> FindSRGRSetNumber(JSDDateTime date)  const;
  TYPE_DATA

  /**
   * @brief List of metadata contained in the Data file
   */
  EnvisatAsarData * _EnvisatAsarData;

};
}

#endif
