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

#include <ossimGeometricSarSensorModel.h>

#include <otb/Ephemeris.h>
#include <otb/PlatformPosition.h>
#include <otb/SensorParams.h>
#include <otb/RefPoint.h>
#include <otb/SarSensor.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/projection/ossimCoarseGridModel.h>
#include <ossim/elevation/ossimElevManager.h>
#include <cmath>
#include <string>

namespace ossimplugins
{


   static const char PRODUCT_GEOREFERENCED_FLAG_KW[] = "product_georeferenced_flag";
   static const char OPTIMIZATION_FACTOR_X_KW[] = "optimization_factor_x";
   static const char OPTIMIZATION_FACTOR_Y_KW[] = "optimization_factor_y";
   static const char OPTIMIZATION_BIAS_X_KW[] = "optimization_bias_x";
   static const char OPTIMIZATION_BIAS_Y_KW[] = "optimization_bias_y";

   const char* ossimGeometricSarSensorModel::CREATE_OCG_PREF_KW = "geometric_sar_sensor_model.create_ocg";

   RTTI_DEF1(ossimGeometricSarSensorModel, "ossimGeometricSarSensorModel", ossimSensorModel);

   static ossimTrace traceDebug("ossimGeometricSarSensorModel:debug");

   ossimGeometricSarSensorModel::ossimGeometricSarSensorModel()
      :
      ossimSensorModel(),
      _platformPosition(0),
      _sensor(0),
      _refPoint(0),
      _sarSensor(0),
      _isProductGeoreferenced(false),
      _optimizationFactorX(0.0),
      _optimizationFactorY(0.0),
      _optimizationBiasX(0.0),
      _optimizationBiasY(0.0)
   {
   }

   ossimGeometricSarSensorModel::ossimGeometricSarSensorModel(
      const ossimGeometricSarSensorModel& rhs)
      :
      ossimSensorModel(rhs),
      _platformPosition(rhs._platformPosition?rhs._platformPosition->Clone():(PlatformPosition*)0),
      _sensor(rhs._sensor?rhs._sensor->Clone():(SensorParams*)0),
      _refPoint(rhs._refPoint?rhs._refPoint->Clone():( RefPoint *)0),
      _isProductGeoreferenced(rhs._isProductGeoreferenced),
      _optimizationFactorX(rhs._optimizationFactorX),
      _optimizationFactorY(rhs._optimizationFactorY),
      _optimizationBiasX(rhs._optimizationBiasX),
      _optimizationBiasY(rhs._optimizationBiasY),
      _imageFilename(rhs._imageFilename),
      _productXmlFile(rhs._productXmlFile)
   {
     if(_sensor && _platformPosition)
       {
       _sarSensor = new SarSensor(_sensor,_platformPosition);
       }
     else
       {
       _sarSensor = NULL;
       }
   }

   ossimGeometricSarSensorModel::~ossimGeometricSarSensorModel()
   {
      if (_platformPosition != 0)
      {
         delete _platformPosition;
         _platformPosition = 0;
      }

      if(_sensor != 0)
      {
         delete _sensor;
         _sensor = 0;
      }

      if (_sarSensor != 0)
      {
         delete _sarSensor;
         _sarSensor = 0;
      }

      if(_refPoint != 0)
      {
         delete _refPoint;
         _refPoint = 0;
      }
   }

   double ossimGeometricSarSensorModel::getSlantRange(double col) const
   {
      const double CLUM        = 2.99792458e+8 ;

      double dist = _refPoint->get_distance()
         + _sensor->get_col_direction() * (col - (_refPoint->get_pix_col())) * ((CLUM / 2.0) * _sensor->get_nRangeLook() / _sensor->get_sf()) ;

      return  dist;
   }

   JSDDateTime ossimGeometricSarSensorModel::getTime(double line) const
   {
      double dt =  _sensor->get_lin_direction() * (line - _refPoint->get_pix_line()) * _sensor->get_nAzimuthLook() / _sensor->get_prf() ;
      JSDDateTime time = _refPoint->get_ephemeris()->get_date();
      time.set_second(time.get_second() + dt);
      time.NormDate();

      return time;
   }

   bool ossimGeometricSarSensorModel::getPlatformPositionAtLine(double line, vector<double>& position, vector<double>& speed)
   {
      JSDDateTime time = getTime(line);
      return _platformPosition->getPlatformPositionAtTime(time,position,speed);
   }

   void ossimGeometricSarSensorModel::lineSampleHeightToWorld(
      const ossimDpt& image_point,
      const double&   heightEllipsoid,
      ossimGpt&       worldPoint) const
   {

      if (!_sarSensor)
      {
         // bad design consequence, should be fixed.
         _sarSensor = new SarSensor(_sensor, _platformPosition);
      }
      double lon, lat;
      // const double CLUM        = 2.99792458e+8 ;
   
      // optimization
      double col = image_point.x - (image_point.x * _optimizationFactorX + _optimizationBiasX) ;
      double line = image_point.y - (image_point.y * _optimizationFactorY + _optimizationBiasY) ;

      JSDDateTime azimuthTime = getTime(line) ;

      // Slant range computation, depending on the product type
      double slantRange;
      if (_isProductGeoreferenced)
      {
         slantRange = getSlantRangeFromGeoreferenced(col) ;
      }
      else
      {
         slantRange = getSlantRange(col) ;
      }
   
      int etatLoc = _sarSensor->ImageToWorld(slantRange, azimuthTime, heightEllipsoid, lon, lat);

      if(traceDebug())
      {
         switch (etatLoc)
         {
            case 0:
               ossimNotify(ossimNotifyLevel_DEBUG) << "successful call to lineSampleHeightToWorld" << std::endl;
               break;
            case 1:
               ossimNotify(ossimNotifyLevel_DEBUG) << "lineSampleHeightToWorld : no real root to the equation belongs to the imaging ray" << std::endl;
               break;
            case 2:
               ossimNotify(ossimNotifyLevel_DEBUG) << "lineSampleHeightToWorld : no real root to the equation" << std::endl;
               break;
            default :
               ossimNotify(ossimNotifyLevel_DEBUG) << "lineSampleHeightToWorld : unknown error case" << std::endl;
               break;
         }
      }

      worldPoint.lat = lat;
      worldPoint.lon = lon;
      worldPoint.hgt = heightEllipsoid ;
   }

   void ossimGeometricSarSensorModel::clearGCPlist() {
      _optimizationGCPsGroundCoordinates.clear();
      _optimizationGCPsImageCoordinates.clear();

      // optimization model update
      optimizeModel(_optimizationGCPsGroundCoordinates, _optimizationGCPsImageCoordinates) ;
   }

   void ossimGeometricSarSensorModel::getGCPlist(std::list<ossimGpt> & groundCoordinates, std::list<ossimDpt> & imageCoordinates) {
      groundCoordinates = _optimizationGCPsGroundCoordinates ;
      imageCoordinates = _optimizationGCPsImageCoordinates ;
   }

   bool ossimGeometricSarSensorModel::optimizeModel(const std::list<ossimGpt> & groundCoordinates, const std::list<ossimDpt> & imageCoordinates)
   {
      if (groundCoordinates.size() != imageCoordinates.size()) return false ;

      // no optimization is used during the GCP localization error computation
      _optimizationFactorX = 0.0 ;
      _optimizationFactorY = 0.0 ;
      _optimizationBiasX = 0.0 ;
      _optimizationBiasY = 0.0 ;

      // appends the user input GCPs to the GCPs already present
      _optimizationGCPsGroundCoordinates.insert(_optimizationGCPsGroundCoordinates.end(), groundCoordinates.begin(), groundCoordinates.end()) ;
      _optimizationGCPsImageCoordinates.insert(_optimizationGCPsImageCoordinates.end(), imageCoordinates.begin(), imageCoordinates.end()) ;

      // no GCP : no optimization
      if (groundCoordinates.size() == 0) return true ;

      // Inverse projection of each Ground Control Point
      std::list<ossimGpt>::iterator itGround = _optimizationGCPsGroundCoordinates.begin() ;
      std::list<ossimDpt> inverseLocResults ;
      while (itGround != _optimizationGCPsGroundCoordinates.end())
      {
         ossimDpt itLoc ;
         this->worldToLineSample(*itGround,itLoc);
         inverseLocResults.push_back(itLoc) ;
         itGround++;
      }

      // error computation
      int nbPoints = _optimizationGCPsGroundCoordinates.size() ;
      double xErrorMean = 0.0, yErrorMean = 0.0, xActualPow = 0.0, yActualPow = 0.0, xActualMean = 0.0, yActualMean = 0.0,
         xErrorByActualMean = 0.0, yErrorByActualMean = 0.0 ;
      double xLocalError, yLocalError ;

      std::list<ossimDpt>::iterator itActualCoords = _optimizationGCPsImageCoordinates.begin() ;
      std::list<ossimDpt>::iterator itEstimatedCoords = inverseLocResults.begin() ;
      while ((itActualCoords != _optimizationGCPsImageCoordinates.end())&&(itEstimatedCoords != inverseLocResults.end())) {
         xLocalError = itActualCoords->x - itEstimatedCoords->x ;
         yLocalError = itActualCoords->y - itEstimatedCoords->y ;

         xErrorMean += xLocalError ;
         yErrorMean += yLocalError ;
         xActualMean += itActualCoords->x ;
         yActualMean += itActualCoords->y ;
         xActualPow += itActualCoords->x * itActualCoords->x ;
         yActualPow += itActualCoords->y * itActualCoords->y ;
         xErrorByActualMean += xLocalError * itActualCoords->x ;
         yErrorByActualMean += yLocalError * itActualCoords->y ;

         ++itActualCoords;
         ++itEstimatedCoords;
      }

      if(nbPoints>0)
        {
        xErrorMean /= nbPoints ;
        yErrorMean /= nbPoints ;
        xActualMean /= nbPoints ;
        yActualMean /= nbPoints ;
        xActualPow /= nbPoints ;
        yActualPow /= nbPoints ;
        xErrorByActualMean /= nbPoints ;
        yErrorByActualMean /= nbPoints ;
        }

      // linear regression
      if (fabs(xActualPow - xActualMean*xActualMean) > FLT_EPSILON)
         _optimizationFactorX = (xErrorByActualMean - xErrorMean * xActualMean) / (xActualPow - xActualMean*xActualMean) ;
      if (fabs(yActualPow - yActualMean*yActualMean) > FLT_EPSILON)
         _optimizationFactorY = (yErrorByActualMean - yErrorMean * yActualMean) / (yActualPow - yActualMean*yActualMean) ;
      _optimizationBiasX = xErrorMean - _optimizationFactorX * xActualMean ;
      _optimizationBiasY = yErrorMean - _optimizationFactorY * yActualMean ;

      //// Comparison between the estimated image coordinates of each GCP and its actual image coordinates
      //double lineBias = 0.0, columnBias = 0.0 ;
      //int nbPtsUsed = 0;

      //std::list<ossimDpt>::iterator itActualCoords = imageCoordinates.begin() ;
      //std::list<ossimDpt>::iterator itEstimatedCoords = inverseLocResults.begin() ;
      //while ((itActualCoords != imageCoordinates.end())&&(itEstimatedCoords != inverseLocResults.end())) {
      //
      //  columnBias += (itActualCoords->x - itEstimatedCoords->x ) ;
      //  lineBias += (itActualCoords->y - itEstimatedCoords->y ) ;
      //
      //  nbPtsUsed++;
      //  itActualCoords++;
      //  itEstimatedCoords++;
      //}

      //// Computation of bias in line and column : mean deviations
      //lineBias /= nbPtsUsed ;
      //columnBias /= nbPtsUsed ;

      //// Update of the model Reference Point
      //_refPoint->set_pix_col(_refPoint->get_pix_col() - columnBias);
      //_refPoint->set_pix_line(_refPoint->get_pix_line() - lineBias);

      return true ;
   }

   bool ossimGeometricSarSensorModel::saveState(ossimKeywordlist& kwl,
                                                const char* prefix) const

   {
      static const char MODULE[] = "ossimGeometricSarSensorModel::saveState";

      bool result = false;

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
      }

      if (_platformPosition && _sensor && _refPoint)
      {
         if ( _platformPosition->saveState(kwl, prefix) )
         {
            if ( _sensor->saveState(kwl, prefix) )
            {
               result = _refPoint->saveState(kwl, prefix);

               if (result)
               {
                  kwl.add(prefix,
                          PRODUCT_GEOREFERENCED_FLAG_KW,
                          (_isProductGeoreferenced?"true":"false"));
                  kwl.add(prefix,
                          OPTIMIZATION_FACTOR_X_KW,
                          _optimizationFactorX);
                  kwl.add(prefix,
                          OPTIMIZATION_FACTOR_Y_KW,
                          _optimizationFactorY);
                  kwl.add(prefix,
                          OPTIMIZATION_BIAS_X_KW,
                          _optimizationBiasX);
                  kwl.add(prefix,
                          OPTIMIZATION_BIAS_Y_KW,
                          _optimizationBiasY);

                  result = ossimSensorModel::saveState(kwl, prefix);
               }
            }
         }
      }

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " exit status = " << (result?"true":"false\n")
            << std::endl;
      }

      return result;
   }

   bool ossimGeometricSarSensorModel::loadState(const ossimKeywordlist &kwl,
                                                const char *prefix)
   {
      static const char MODULE[] = "ossimGeometricSarSensorModel::loadState";

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
      }

      bool result = true;

      // Load the base class;
      if ( ossimSensorModel::loadState(kwl, prefix) == false )
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\nossimSensorModel::loadState failed!\n";
         }
         result = false;
      }

      // Load the platform position state.
      if ( !_platformPosition)
      {
         _platformPosition = new PlatformPosition();
      }
      if ( _platformPosition->loadState(kwl, prefix) == false )
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\n_platformPosition->loadState failed!\n";
         }
         result = false;
      }

      // Load the sensor position state.
      if ( !_sensor)
      {
         _sensor = new SensorParams();
      }
      if ( _sensor->loadState(kwl, prefix) == false )
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\n_sensor->loadState failed!\n";
         }
         result = false;
      }

      _sarSensor = new SarSensor(_sensor, _platformPosition);

      // Load the ref point.
      if ( !_refPoint)
      {
         _refPoint = new RefPoint();
      }
      if ( _refPoint->loadState(kwl, prefix) == false )
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\n_refPoint->loadState failed!\n";
         }
         result = false;
      }

      const char* lookup = 0;
      ossimString s;

      lookup = kwl.find(prefix, PRODUCT_GEOREFERENCED_FLAG_KW);
      if (lookup)
      {
         s = lookup;
         _isProductGeoreferenced = s.toBool();
      }
      else
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\nRequired keyword not found: "
               << PRODUCT_GEOREFERENCED_FLAG_KW << "\n";
         }
         result = false;
      }

      lookup = kwl.find(prefix, OPTIMIZATION_FACTOR_X_KW);
      if (lookup)
      {
         s = lookup;
         _optimizationFactorX = s.toDouble();
      }
      else
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\nRequired keyword not found: "
               << OPTIMIZATION_FACTOR_X_KW << "\n";
         }
         result = false;
      }

      lookup = kwl.find(prefix, OPTIMIZATION_FACTOR_Y_KW);
      if (lookup)
      {
         s = lookup;
         _optimizationFactorY = s.toDouble();
      }
      else
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\nRequired keyword not found: "
               << OPTIMIZATION_FACTOR_Y_KW << "\n";
         }
         result = false;
      }

      lookup = kwl.find(prefix,OPTIMIZATION_BIAS_X_KW);
      if (lookup)
      {
         s = lookup;
         _optimizationBiasX= s.toDouble();
      }
      else
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\nRequired keyword not found: "
               << OPTIMIZATION_BIAS_X_KW << "\n";
         }
         result = false;
      }

      lookup = kwl.find(prefix,OPTIMIZATION_BIAS_Y_KW);
      if (lookup)
      {
         s = lookup;
         _optimizationBiasY = s.toDouble();
      }
      else
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\nRequired keyword not found: "
               << OPTIMIZATION_BIAS_X_KW << "\n";
         }
         result = false;
      }

      // if (result && traceDebug())
//    if (result)
//    {
//       ossimNotify(ossimNotifyLevel_DEBUG)
//          << "calling saveState to verify loadState..." << endl;

//       ossimKeywordlist kwl2;
//       saveState(kwl2, 0);

//       ossimNotify(ossimNotifyLevel_DEBUG)
//          << "saveState result after loadState:"  << kwl2 << endl;
//    }

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " exit status = " << (result?"true":"false\n")
            << std::endl;
      }

      return result;
   }

#if 0
   if(!InitSRGR(kwl, prefix))
   {
      return false;
   }
   if(!InitPlatformPosition(kwl, prefix))
   {
      return false;
   }
   if(!InitSensorParams(kwl, prefix))
   {
      return false;
   }
   if(!InitRefPoint(kwl, prefix))
   {
      return false;
   }
   return true;
}
#endif

std::ostream& ossimGeometricSarSensorModel::print(std::ostream& out) const
{
  std::ios::fmtflags f(out.flags());

   out << setprecision(15) << setiosflags(ios::fixed)
       << "\nossimGeometricSarSensorModel class data members:\n";

   const char* prefix = 0;
   ossimKeywordlist kwl;
   if (_platformPosition)
   {
      _platformPosition->saveState(kwl, prefix);
   }
   if (_sensor)
   {
      _sensor->saveState(kwl, prefix);
   }
   if (_refPoint)
   {
      _refPoint->saveState(kwl, prefix);
   }

   kwl.add(prefix,
           PRODUCT_GEOREFERENCED_FLAG_KW,
           (_isProductGeoreferenced?"true":"false"));
   kwl.add(prefix,
           OPTIMIZATION_FACTOR_X_KW,
           _optimizationFactorX);
   kwl.add(prefix,
           OPTIMIZATION_FACTOR_Y_KW,
           _optimizationFactorY);
   kwl.add(prefix,
           OPTIMIZATION_BIAS_X_KW,
           _optimizationBiasX);
   kwl.add(prefix,
           OPTIMIZATION_BIAS_Y_KW,
           _optimizationBiasY);

   out << kwl;

   out.flags(f);

   return ossimSensorModel::print(out);
}

//*************************************************************************************************
// Creates replacement coarse grid model if user requested via ossim preferences keyword.
// Returns true if load OK, false on error
//*************************************************************************************************
bool ossimGeometricSarSensorModel::createReplacementOCG()
{
   // Read the preferences to determine if we need to do this:
   ossimString str (ossimPreferences::instance()->findPreference(CREATE_OCG_PREF_KW));
   if (!str.toBool())
      return true; // this is not an error condition

   // Compute the coarse grid:
   _replacementOcgModel = new ossimCoarseGridModel;
   _replacementOcgModel->setInterpolationError(0.1);
   _replacementOcgModel->setMinGridSpacing(50);
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_NOTICE)<<"\nComputing coarse grid..."<<endl;
   }
   _replacementOcgModel->buildGrid(theImageClipRect, this, 500.00, true, false);

   // Save the coarse grid to appropriate location given image filename:
   bool status = _replacementOcgModel->saveCoarseGrid(_imageFilename); // this saves geom file as well

   return status;
}

//*****************************************************************************
//  METHOD: ossimGeometricSarSensorModel::lineSampleToWorld(image_pt, &gpt)
//
//  Intersects the ray associated with image_pt with the available elevation
//  model. Returns ground point
//
//*****************************************************************************
void ossimGeometricSarSensorModel::lineSampleToWorld(const ossimDpt& image_point,
                                                     ossimGpt&       gpt) const
{
   bool debug = false;  // setable via interactive debugger
   if (traceDebug() || debug)  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::lineSampleToWorld:entering..." << std::endl;

   if(image_point.hasNans())
   {
      gpt.makeNan();
      return;
   }
   //***
   // Extrapolate if image point is outside image:
   //***
   if (!insideImage(image_point)&&(!theExtrapolateImageFlag))
   {
      gpt = extrapolate(image_point);
      return;
   }

   //***
   // Determine imaging ray and invoke elevation source object's services to
   // intersect ray with terrain model:
   //***
   //    ossimEcefRay ray;    In the case of SAR sensors, ray intersection does not have meaning anymore...
   //    imagingRay(image_point, ray);
   //    ossimElevManager::instance()->intersectRay(ray, gpt);

   static const double LON_LAT_THRESHOLD  = .00001; // acceptable lon lat error (degrees)
   static const int    MAX_NUM_ITERATIONS = 30;
   static const double LON_LAT_STEP       = .00001;
   int iters = 0;

   //
   // Utilize iterative scheme for arriving at ground point. Begin with guess
   // at RefGndPt:
   //
   gpt.lond(theRefGndPt.lond());
   gpt.latd(theRefGndPt.latd());
   gpt.height(ossimElevManager::instance()->getHeightAboveEllipsoid(gpt));

   ossimGpt gpt_dlat;
   ossimGpt gpt_dlon;

   ossimDpt ip, ip_dlat, ip_dlon;
   double du_dlat, du_dlon, dv_dlat, dv_dlon;
   double delta_lat, delta_lon, delta_u, delta_v;
   double inverse_norm;
   bool done = false;

   //***
   // Begin iterations:
   //***
   do
   {
      //***
      // establish perturbed ground points about the guessed point:
      //***
      gpt_dlat.latd(gpt.latd() + LON_LAT_STEP);
      gpt_dlat.lond(gpt.lond());
      gpt_dlat.height(ossimElevManager::instance()->getHeightAboveEllipsoid(gpt_dlat));
      gpt_dlon.latd(gpt.latd());
      gpt_dlon.lond(gpt.lond() + LON_LAT_STEP);
      gpt_dlon.height(ossimElevManager::instance()->getHeightAboveEllipsoid(gpt_dlon));

      //***
      // Compute numerical partials at current guessed point:
      //***
      worldToLineSample(gpt, ip);
      worldToLineSample(gpt_dlat, ip_dlat);
      worldToLineSample(gpt_dlon, ip_dlon);

      if(ip.hasNans() || ip_dlat.hasNans() || ip_dlon.hasNans())
      {
         gpt.makeNan();
         return;
      }

      du_dlat = (ip_dlat.u - ip.u) / LON_LAT_STEP;
      du_dlon = (ip_dlon.u - ip.u) / LON_LAT_STEP;
      dv_dlat = (ip_dlat.v - ip.v) / LON_LAT_STEP;
      dv_dlon = (ip_dlon.v - ip.v) / LON_LAT_STEP;

      //
      // Test for convergence:
      //
      delta_u = image_point.u - ip.u;
      delta_v = image_point.v - ip.v;

      //
      // Compute linearized estimate of ground point given ip delta:
      //
      inverse_norm = du_dlon*dv_dlat - dv_dlon*du_dlat; // fg-eh

      if (!ossim::almostEqual(inverse_norm, 0.0, DBL_EPSILON))
      {
         delta_lon = (delta_u*dv_dlat - delta_v*du_dlat)/inverse_norm;
         delta_lat = (delta_v*du_dlon - delta_u*dv_dlon)/inverse_norm;
         gpt.latd(gpt.latd() + delta_lat);
         gpt.lond(gpt.lond() + delta_lon);
         gpt.height(ossimElevManager::instance()->getHeightAboveEllipsoid(gpt));
      }
      else
      {
         delta_lon = 0;
         delta_lat = 0;
      }
      done = ((fabs(delta_lon) < LON_LAT_THRESHOLD) && (fabs(delta_lat) < LON_LAT_THRESHOLD));
      iters++;
   } while ((!done) && (iters < MAX_NUM_ITERATIONS));

   if (traceDebug() || debug)
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "image_point = " << image_point << std::endl;
      //       ossimNotify(ossimNotifyLevel_DEBUG) << "ray = " << ray << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "gpt = " << gpt << std::endl;
   }

   if (traceDebug() || debug)  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSensorModel::lineSampleToWorld: returning..." << std::endl;
   return;
}


void ossimGeometricSarSensorModel::set_platformPosition(PlatformPosition* platformPosition)
{
   if(_platformPosition != 0)
   {
      delete _platformPosition;
      _platformPosition = 0;
   }
   _platformPosition = platformPosition->Clone();
}

void ossimGeometricSarSensorModel::set_sensorParams(SensorParams* sensorParams)
{
   if(_sensor != 0)
   {
      delete _sensor;
      _sensor = 0;
   }
   _sensor = sensorParams->Clone();
}

void ossimGeometricSarSensorModel::set_refPoint(RefPoint* refPoint)
{
   if(_refPoint != 0)
   {
      delete _refPoint;
      _refPoint = 0;
   }
   _refPoint = refPoint->Clone();
}


PlatformPosition* ossimGeometricSarSensorModel::get_platformPosition() const
{
   return _platformPosition;
}


SensorParams* ossimGeometricSarSensorModel::get_sensorParams() const
{
   return _sensor;
}


RefPoint* ossimGeometricSarSensorModel::get_refPoint() const
{
   return _refPoint;
}



}
