/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef otbGDALRPCTransformer_h
#define otbGDALRPCTransformer_h

#include "OTBGdalAdaptersExport.h"

#include <memory>
#include <string>

#include "gdal_alg.h"
#include "cpl_string.h"

namespace otb
{
/**
 * \class GDALRPCTransformer
 * \brief This class is a wrapper around GDALCreateRPCTransformer and GDALRPCTransform
 *
 * This class aims at manipulating RPC transformations within OTB,
 * in a safe and easy way.
 *
 * To use this wrapper, one needs to call the constructor with the correct
 * parameters. Then, one can set the options, and the threshold. Finally,
 * one can call ForwardTransform or BackwardTransform, depending in the
 * desired result.
 *
 * The parameters provided to the constructor are those found in the
 * RPCParam structure defined in the otbGeometryMetadata.h file. They
 * are quite similar to what can be found in GDALRPCInfo.
 *
 * \ingroup OTBGdalAdapters
 */

class OTBGdalAdapters_EXPORT GDALRPCTransformer
{
public:
  /**
   * Build a GDALRPCTransformer
   *
   * The parameters describe the RPC model. They can be found in the
   * RPCParam structure defined in the otbGeometryMetadata.h file. They
   * are quite similar to what can be found in GDALRPCInfo. See also
   * http://geotiff.maptools.org/rpc_prop.html
   *
   */
  GDALRPCTransformer(double LineOffset, double SampleOffset, double LatOffset, double LonOffset, double HeightOffset,
	                 double LineScale, double SampleScale, double LatScale, double LonScale, double HeightScale,
					 const double LineNum[20], const double LineDen[20], const double SampleNum[20], const double SampleDen[20]);

  /**
   * Set additional options to the transformer
   *
   * <ul>
   * <li> RPC_HEIGHT: a fixed height offset to be applied to all points passed
   * in.  In this situation the Z passed into the transformation function is
   * assumed to be height above ground, and the RPC_HEIGHT is assumed to be
   * an average height above sea level for ground in the target scene.</li>
   *
   * <li> RPC_HEIGHT_SCALE: a factor used to multiply heights above ground.
   * Useful when elevation offsets of the DEM are not expressed in meters.</li>
   *
   * <li> RPC_DEM: the name of a GDAL dataset (a DEM file typically) used to
   * extract elevation offsets from. In this situation the Z passed into the
   * transformation function is assumed to be height above ground. This option
   * should be used in replacement of RPC_HEIGHT to provide a way of defining
   * a non uniform ground for the target scene</li>
   *
   * <li> RPC_DEMINTERPOLATION: the DEM interpolation ("near", "bilinear" or "cubic").
   *      Default is "bilinear".</li>
   *
   * <li> RPC_DEM_MISSING_VALUE: value of DEM height that must be used in case
   * the DEM has nodata value at the sampling point, or if its extent does not
   * cover the requested coordinate. When not specified, missing values will cause
   * a failed transform.</li>
   *
   * <li> RPC_DEM_SRS: (GDAL >= 3.2) WKT SRS, or any string recognized by
   * OGRSpatialReference::SetFromUserInput(), to be used as an override for DEM SRS.
   * Useful if DEM SRS does not have an explicit vertical component. </li>
   *
   * <li> RPC_DEM_APPLY_VDATUM_SHIFT: whether the vertical component of a compound
   * SRS for the DEM should be used (when it is present). This is useful so as to
   * be able to transform the "raw" values from the DEM expressed with respect to
   * a geoid to the heights with respect to the WGS84 ellipsoid. When this is
   * enabled, the GTIFF_REPORT_COMPD_CS configuration option will be also set
   * temporarily so as to get the vertical information from GeoTIFF
   * files. Defaults to TRUE. (GDAL >= 2.1.0)</li>
   *
   * <li> RPC_PIXEL_ERROR_THRESHOLD: overrides the dfPixErrThreshold parameter, ie
   * the error (measured in pixels) allowed in the
   * iterative solution of pixel/line to lat/long computations (the other way
   * is always exact given the equations).  (GDAL >= 2.1.0)</li>
   *
   * <li> RPC_MAX_ITERATIONS: maximum number of iterations allowed in the
   * iterative solution of pixel/line to lat/long computations. Default value is
   * 10 in the absence of a DEM, or 20 if there is a DEM.  (GDAL >= 2.1.0)</li>
   *
   * <li> RPC_FOOTPRINT: WKT or GeoJSON polygon (in long / lat coordinate space)
   * with a validity footprint for the RPC. Any coordinate transformation that
   * goes from or arrive outside this footprint will be considered invalid. This
   * is useful in situations where the RPC values become highly unstable outside
   * of the area on which they have been computed for, potentially leading to
   * undesirable "echoes" / false positives. This requires GDAL to be built against
   * GEOS.</li>
   * </ul>
   *
   * \param Name a string containing the name of the option
   * \param Value a string containing the value of the option
   *
   */
  void SetOption(std::string Name, std::string Value);

  /**
   * Set the error (measured in pixels) allowed in the
   * iterative solution of pixel/line to lat/long computations (the other way
   * is always exact given the equations). Starting with GDAL 2.1, this may also
   * be set through the RPC_PIXEL_ERROR_THRESHOLD transformer option.
   * If a negative or null value is provided, then this defaults to 0.1 pixel.
   *
   * \param PixErrThreshold the new value of the error
   *
   */
  void SetPixErrThreshold(double PixErrThreshold);

  /**
   * Compute a forward transformation
   *
   * This method performs a transformation from column/row to long/lat/height space.
   * It can work with an arbitrary number of points.
   *
   * \param x the X coordinate of the points to convert
   * \param y the Y coordinate of the points to convert
   * \param z the Z coordinate of the points to convert
   * \param nPointCount the number of points to convert
   * \return true if all points were correctly transformed
   *
   */
  bool ForwardTransform(double* x, double* y, double* z, int nPointCount=1);

  /**
   * Compute an inverse transformation
   *
   * This method performs a transformation from long/lat/height to column/row space.
   * It can work with an arbitrary number of points.
   *
   * \param x the X coordinate of the points to convert
   * \param y the Y coordinate of the points to convert
   * \param z the Z coordinate of the points to convert
   * \param nPointCount the number of points to convert
   * \return true if all points were correctly transformed
   *
   */
  bool InverseTransform(double* x, double* y, double* z, int nPointCount=1);

protected:
  /**
   * Regenerate the transformer
   *
   * Called when performing a transformation and some options were modified.
   */
  void Update();

private:
  /** Used to know if Update is required after a change in the options */
  bool m_Modified = true;
  /** The RPC model */
  GDALRPCInfo m_GDALRPCInfo;
  /** The options */
  char ** m_Options = nullptr;
  /** The error allowed in the iterative solution */
  double m_PixErrThreshold = 0.1;
  /** The transformer arguments */
  void * m_TransformArg = nullptr;
};
}
#endif
