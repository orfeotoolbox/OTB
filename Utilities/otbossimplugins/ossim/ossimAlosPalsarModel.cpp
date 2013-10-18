//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
// "Copyright Centre for Remote Imaging, Sensing and Processing"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include <ossimAlosPalsarModel.h>
#include <otb/GalileanEphemeris.h>
#include <otb/GeographicEphemeris.h>

#include <otb/JSDDateTime.h>
#include <otb/GMSTDateTime.h>
#include <otb/CivilDateTime.h>

#include <ossim/base/ossimTrace.h>
#include <otb/RefPoint.h>
#include <AlosPalsar/AlosPalsarLeader.h>
#include <AlosPalsar/AlosPalsarData.h>
#include <otb/SensorParams.h>
#include <otb/PlatformPosition.h>
#include <ossim/base/ossimKeywordNames.h>


// Static trace for debugging
static ossimTrace traceDebug("ossimAlosPalsarModel:debug");

#include <string>
#include <algorithm>

namespace ossimplugins
{

   RTTI_DEF1(ossimAlosPalsarModel, "ossimAlosPalsarModel", ossimGeometricSarSensorModel);

   ossimAlosPalsarModel::ossimAlosPalsarModel():
      thePixelSpacing(0),
      theAlosPalsarLeader(NULL),
      theAlosPalsarData(NULL)
   {
   }

   ossimAlosPalsarModel::~ossimAlosPalsarModel()
   {
      if (theAlosPalsarLeader != NULL)
      {
         delete theAlosPalsarLeader;
         theAlosPalsarLeader = NULL;
      }
      if (theAlosPalsarData != NULL)
      {
         delete theAlosPalsarData;
         theAlosPalsarData = NULL;
      }
   }

   ossimString ossimAlosPalsarModel::getClassName() const
   {
      return ossimString("ossimAlosPalsarModel");
   }

   ossimObject* ossimAlosPalsarModel::dup() const
   {
      return new ossimAlosPalsarModel(*this);
   }

   double ossimAlosPalsarModel::getSlantRangeFromGeoreferenced(double /* col */) const
   {
      std::cout << "WARNING: getSlantRangeFromGeoreferenced not implemented for AlosPalsar" << std::endl;
      return 0.0;
   }

   bool ossimAlosPalsarModel::InitSensorParams(const ossimKeywordlist &kwl, const char *prefix)
   {
      const char* wave_length_str = kwl.find(prefix, "wave_length");
      double wave_length = atof(wave_length_str);
      const char* fr_str = kwl.find(prefix, "fr");
      double fr = atof(fr_str) * 1e6;
      const char* fa_str = kwl.find(prefix, "fa");
      double fa = atof(fa_str);

      ossimString time_dir_pix = kwl.find(prefix, "time_dir_pix");
      time_dir_pix.upcase();
      //std::transform(time_dir_pix.begin(), time_dir_pix.end(), time_dir_pix.begin(), toupper);
      ossimString time_dir_lin = kwl.find(prefix, "time_dir_lin");
      time_dir_lin.upcase();
      //std::transform(time_dir_lin.begin(), time_dir_lin.end(), time_dir_lin.begin(), toupper);

      //ellipsoid parameters
      const char* ellip_maj_str = kwl.find(prefix, "ellip_maj");
      double ellip_maj = atof(ellip_maj_str) * 1000.0;  // km -> m
      const char* ellip_min_str = kwl.find(prefix, "ellip_min");
      double ellip_min = atof(ellip_min_str) * 1000.0;  // km -> m

      const char* dopcen_str = kwl.find(prefix, "dopcen_range[0]");
      double dopcen = atof(dopcen_str);
      const char* dopcenLinear_str = kwl.find(prefix, "dopcen_range[1]");
      double dopcenLinear = atof(dopcenLinear_str);

      if (_sensor != NULL)
      {
         delete _sensor;
      }

      _sensor = new SensorParams();

      /*
        if(strcmp(time_dir_pix.c_str(), "INCREASE") == 0)
        {
        _sensor->set_col_direction(1);
        }
        else
        {
        _sensor->set_col_direction(-1);
        }
      */
      // WARNING: _col_direction hard-coded to 1, as level 1.0 image is not flipped.
      _sensor->set_col_direction(1);

      // TODO: Have to verify whether the time direction indicator should be always positive
      /*
        if(strcmp(time_dir_lin.c_str(), "INCREASE") == 0)
        {
        _sensor->set_lin_direction(1);
        }
        else
        {
        _sensor->set_lin_direction(-1);
        }
      */
      _sensor->set_lin_direction(1);

      _sensor->set_sightDirection(SensorParams::Right) ;

      double nlooks_az = atof(kwl.find(prefix, "nlooks_az"));
      _sensor->set_nAzimuthLook(nlooks_az);
      double n_rnglok = atof(kwl.find(prefix, "n_rnglok"));
      _sensor->set_nRangeLook(n_rnglok);

      _sensor->set_prf(fa / 1000.); // For ALOS, the fa value in the leader file is in units of mHz
      _sensor->set_sf(fr);
      _sensor->set_rwl(wave_length);

      _sensor->set_semiMajorAxis(ellip_maj) ;
      _sensor->set_semiMinorAxis(ellip_min) ;

      _sensor->set_dopcen(dopcen);
      _sensor->set_dopcenLinear(dopcenLinear);

      return true;
   }

   bool ossimAlosPalsarModel::open(const ossimFilename& file)
   {
      static const char MODULE[] = "ossimAlosPalsarModel::open";

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " entered...\n"
            << "file: " << file << "\n";
      }

      bool result = false;
      ossimFilename leaFilename = file;
      ossimFilename datFilename;

      /*
       * Creation of the class allowing to store Leader file metadata
       */
      if (theAlosPalsarLeader != NULL)
      {
         delete theAlosPalsarLeader;
         theAlosPalsarLeader = NULL;
      }

      theAlosPalsarLeader = new AlosPalsarLeader();
      theAlosPalsarData = new AlosPalsarData();

      if (leaFilename.exists())
      {
         result = isAlosPalsarLeader(leaFilename);
         if (result == false)
         {
            leaFilename = findAlosPalsarLeader(file);
         }
         result = isAlosPalsarLeader(leaFilename);

         if (result == true)
         {
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << leaFilename << " is AlosPalsar leader file..."
                  << std::endl
                  << "Begin reading Leader file" << std::endl;
            }
            /*
             * Leader file data reading
             */
            std::ifstream leaderFile(leaFilename.c_str(), ios::in | ios::binary);
            leaderFile >> *theAlosPalsarLeader;
            leaderFile.close();

            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "End reading Leader file" << std::endl;
            }

            datFilename = findAlosPalsarData(leaFilename);
            result = isAlosPalsarData(datFilename);
            if (result == true)
            {
               if (traceDebug())
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << datFilename << " is AlosPalsar data file..."
                     << std::endl
                     << "Begin reading Data file header" << std::endl;
               }
               /*
                * Read header of data file for image size info
                */
               std::ifstream dataFile(datFilename.c_str(), ios::in | ios::binary);
               dataFile >> *theAlosPalsarData;
               dataFile.close();

               if (traceDebug())
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << "End reading Data file header" << std::endl;
               }
            } // matches: if ( result=isAlosPalsarData(datFilename) == true )

            //To initialize the whole state, reusing saveState/loadState
            //FIXME: This could be at the superclass level instead
            ossimKeywordlist kwl;
            saveState(kwl);
            loadState(kwl);

         } // matches: if ( result=isAlosPalsarLeader(file) == True )

      } // matches: if ( file.exists() )

      if (traceDebug())
      {
         this->print(ossimNotify(ossimNotifyLevel_DEBUG));

         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " exit status = " << (result ? "true" : "false\n")
            << std::endl;
      }

      return result;

   }


   bool ossimAlosPalsarModel::saveState(ossimKeywordlist& kwl,
                                        const char* prefix) const
   {
      static const char MODULE[] = "ossimAlosPalsarModel::saveState";

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
      }

      bool result;

      kwl.add(prefix, ossimKeywordNames::TYPE_KW, "ossimAlosPalsarModel", true);

      if (theAlosPalsarLeader == NULL)
      {
         std::cout << "Error: AlosPalsarLeader is NULL" << std::endl;
         return false;
      }

      result = theAlosPalsarLeader->saveState(kwl);

      if (result == true)
      {
         if (theAlosPalsarData == NULL)
         {
            std::cout << "Error: AlosPalsarData is NULL" << std::endl;
            return false;
         }
         result = theAlosPalsarData->saveState(kwl);
      }

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " exit status = " << (result ? "true" : "false\n")
            << std::endl;
      }

      return result;
   }

   bool ossimAlosPalsarModel::loadState(const ossimKeywordlist &kwl, const char *prefix)
   {
      static const char MODULE[] = "ossimAlosPalsarModel::loadState";

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
      }

      const char* lookup = 0;
      ossimString s;

      // Check the type first.
      lookup = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
      if (lookup)
      {
         s = lookup;
         if (s != getClassName())
         {
            return false;
         }
      }

      // Load the base class.
//    bool result = ossimGeometricSarSensorModel::loadState(kwl, prefix);
      bool result = false;
      result = InitPlatformPosition(kwl, prefix);
      if (!result)
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\nCan't init platform position \n";
         }
      }

      if (result)
      {
         result = InitSensorParams(kwl, prefix);
         if (!result)
         {
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE
                  << "\nCan't init sensor parameters \n";
            }
         }
      }

      if (result)
      {
         result = InitRefPoint(kwl, prefix);
         if (!result)
         {
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << MODULE
                  << "\nCan't init ref point \n";
            }
         }
      }

// Products georeferenced to ground range are not handled in AlosPalsarModel
      _isProductGeoreferenced = false;
      /*
        if (result)
        {
        result = InitSRGR(kwl, prefix);
        if (!result)
        {
        if (traceDebug())
        {
        ossimNotify(ossimNotifyLevel_WARN)
        << MODULE
        << "\nCan't init ref point \n";
        }
        }
        }
      */

      return result;
   }

   bool ossimAlosPalsarModel::InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix)
   {
      // const double PI          = 3.14159265358979323846 ;
      CivilDateTime ref_civil_date;
      /*
       * Ephemerisis reference date retrieval
       */
      const char* eph_year_str = kwl.find(prefix, "eph_year");
      int eph_year = atoi(eph_year_str);
      const char* eph_month_str = kwl.find(prefix, "eph_month");
      int eph_month = atoi(eph_month_str);
      const char* eph_day_str = kwl.find(prefix, "eph_day");
      int eph_day = atoi(eph_day_str);
      const char* eph_sec_str = kwl.find(prefix, "eph_sec");
      double eph_sec = atof(eph_sec_str);


      ref_civil_date.set_year(eph_year);
      ref_civil_date.set_month(eph_month);
      ref_civil_date.set_day(eph_day);
      ref_civil_date.set_second((int)eph_sec);
      ref_civil_date.set_decimal(eph_sec - (double)((int)eph_sec));

      JSDDateTime ref_jsd_date(ref_civil_date);

      /*
       * Ephemerisis time interval retrieval
       */
      const char* eph_int_str = kwl.find(prefix, "eph_int");
      double eph_int = atof(eph_int_str);
      /*
       * Ephemerisis number retrieval
       */
      const char* neph_str = kwl.find(prefix, "neph");
      int neph = atoi(neph_str);

      Ephemeris** ephemeris = new Ephemeris*[neph];

      /*
       * Ephemerisis retrieval
       */
      for (int i = 0; i < neph; i++)
      {
         double pos[3];
         double vit[3];
         char name[64];


         sprintf(name, "eph%i_posX", i);
         const char* px_str = kwl.find(prefix, name);
         pos[0] = atof(px_str);

         sprintf(name, "eph%i_posY", i);
         const char* py_str = kwl.find(prefix, name);
         pos[1] = atof(py_str);

         sprintf(name, "eph%i_posZ", i);
         const char* pz_str = kwl.find(prefix, name);
         pos[2] = atof(pz_str);


         sprintf(name, "eph%i_velX", i);
         const char* vx_str = kwl.find(prefix, name);
         vit[0] = atof(vx_str);

         sprintf(name, "eph%i_velY", i);
         const char* vy_str = kwl.find(prefix, name);
         vit[1] = atof(vy_str);

         sprintf(name, "eph%i_velZ", i);
         const char* vz_str = kwl.find(prefix, name);
         vit[2] = atof(vz_str);

         /*
          * Ephemerisis date
          */
         JSDDateTime date(ref_jsd_date);
         date.set_second(date.get_second() + i * eph_int);
         date.NormDate();

         GeographicEphemeris* eph = new GeographicEphemeris(date, pos, vit);

         ephemeris[i] = eph;
      }

      /*
       * Antenna position interpolator creation
       */
      if (_platformPosition != NULL)
      {
         delete _platformPosition;
      }
      _platformPosition = new PlatformPosition(ephemeris, neph);

      /*
       * Free of memory used by the ephemerisis list
       */
      for (int i = 0; i < neph; i++)
      {
         delete ephemeris[i];
      }
      delete[] ephemeris;

      return true;
   }

   bool ossimAlosPalsarModel::InitRefPoint(const ossimKeywordlist &kwl, const char *prefix)
   {
      const char* sc_lin_str = kwl.find(prefix, "sc_lin");
      double sc_lin = atof(sc_lin_str);

      const char* sc_pix_str = kwl.find(prefix, "sc_pix");
      double sc_pix = atof(sc_pix_str);

      const char* inp_sctim_str = kwl.find(prefix, "inp_sctim");

      // Not available for ALOS
      //const char* rng_gate_str = kwl.find(prefix,"zero_dop_range_time_f_pixel");
      //double rng_gate = atof(rng_gate_str);

      if (_refPoint == NULL)
      {
         _refPoint = new RefPoint();
      }

      _refPoint->set_pix_col(sc_pix);
      _refPoint->set_pix_line(sc_lin);

      char year_str[5];
      for (int i = 0; i < 4; i++)
      {
         year_str[i] = inp_sctim_str[i];
      }
      year_str[4] = '\0';

      char month_str[3];
      for (int i = 4; i < 6; i++)
      {
         month_str[i-4] = inp_sctim_str[i];
      }
      month_str[2] = '\0';

      char day_str[3];
      for (int i = 6; i < 8; i++)
      {
         day_str[i-6] = inp_sctim_str[i];
      }
      day_str[2] = '\0';

      char hour_str[3];
      for (int i = 8; i < 10; i++)
      {
         hour_str[i-8] = inp_sctim_str[i];
      }
      hour_str[2] = '\0';

      char min_str[3];
      for (int i = 10; i < 12; i++)
      {
         min_str[i-10] = inp_sctim_str[i];
      }
      min_str[2] = '\0';

      char sec_str[3];
      for (int i = 12; i < 14; i++)
      {
         sec_str[i-12] = inp_sctim_str[i];
      }
      sec_str[2] = '\0';

      char mili_str[4];
      for (int i = 14; i < 17; i++)
      {
         mili_str[i-14] = inp_sctim_str[i];
      }
      mili_str[3] = '\0';

      int year = atoi(year_str);
      int month = atoi(month_str);
      int day = atoi(day_str);
      int hour = atoi(hour_str);
      int min = atoi(min_str);
      int sec = atoi(sec_str);
      double mili = atof(mili_str);


      CivilDateTime date(year, month, day, hour * 3600 + min * 60 + sec, mili / 1000.0);

      if (_platformPosition != NULL)
      {
         Ephemeris * ephemeris = _platformPosition->Interpolate((JSDDateTime)date);
         if (ephemeris == NULL) return false ;
         _refPoint->set_ephemeris(ephemeris);

         delete ephemeris;
      }
      else
      {
         return false;
      }

      const double c = 2.99792458e+8;

      const char* slantRange = kwl.find(prefix, "slant_range_to_1st_data_sample");
      double distance = atof(slantRange) + static_cast<double>(sc_pix) * _sensor->get_nRangeLook() / _sensor->get_sf() * c / 2.0;

      _refPoint->set_distance(distance);

      // in order to use ossimSensorModel::lineSampleToWorld
      const char* nbCol_str = kwl.find(prefix, "num_pix_in_line");
      const char* nbLin_str = kwl.find(prefix, "num_lines");
      theImageSize.x      = atoi(nbCol_str);
      theImageSize.y      = atoi(nbLin_str);
      theImageClipRect    = ossimDrect(0, 0, theImageSize.x - 1, theImageSize.y - 1);

// AlosPalsarModel currently does not handle GCPs
      /* Do not use GCPs for now
      // Ground Control Points extracted from the model : corner points
      std::list<ossimGpt> groundGcpCoordinates ;
      std::list<ossimDpt> imageGcpCoordinates ;
      // first line first pix
      const char* lon_str = kwl.find("first_line_first_pixel_lon");
      double lon = atof(lon_str);
      const char* lat_str = kwl.find("first_line_first_pixel_lat");
      double lat = atof(lat_str);
      if (lon > 180.0) lon -= 360.0;
      ossimDpt imageGCP1(0,0);
      ossimGpt groundGCP1(lat, lon, 0.0);
      groundGcpCoordinates.push_back(groundGCP1) ;
      imageGcpCoordinates.push_back(imageGCP1) ;
      // first line last pix
      lon_str = kwl.find("first_line_last_pixel_lon");
      lon = atof(lon_str);
      lat_str = kwl.find("first_line_last_pixel_lat");
      lat = atof(lat_str);
      if (lon > 180.0) lon -= 360.0;
      ossimDpt imageGCP2(theImageSize.x-1, 0);
      ossimGpt groundGCP2(lat, lon, 0.0);
      groundGcpCoordinates.push_back(groundGCP2) ;
      imageGcpCoordinates.push_back(imageGCP2) ;
      // last line last pix
      lon_str = kwl.find("last_line_last_pixel_lon");
      lon = atof(lon_str);
      lat_str = kwl.find("last_line_last_pixel_lat");
      lat = atof(lat_str);
      if (lon > 180.0) lon -= 360.0;
      ossimDpt imageGCP3(theImageSize.x-1,theImageSize.y-1);
      ossimGpt groundGCP3(lat, lon, 0.0);
      groundGcpCoordinates.push_back(groundGCP3) ;
      imageGcpCoordinates.push_back(imageGCP3) ;
      // last line first pix
      lon_str = kwl.find("last_line_first_pixel_lon");
      lon = atof(lon_str);
      lat_str = kwl.find("last_line_first_pixel_lat");
      lat = atof(lat_str);
      if (lon > 180.0) lon -= 360.0;
      ossimDpt imageGCP4(0,theImageSize.y-1);
      ossimGpt groundGCP4(lat, lon, 0.0);
      groundGcpCoordinates.push_back(groundGCP4) ;
      imageGcpCoordinates.push_back(imageGCP4) ;

      // Default optimization
      optimizeModel(groundGcpCoordinates, imageGcpCoordinates) ;
      */

      return true;
   }

// Note: Products georeferenced to ground range are not handled in AlosPalsarModel
//  therefore the following method will not be used
   bool ossimAlosPalsarModel::InitSRGR(const ossimKeywordlist& /* kwl */, const char* /* prefix */)
   {
      // TODO Add user warning and reference to ERS Model
      return false;
   }

   bool ossimAlosPalsarModel::isAlosPalsarLeader(const ossimFilename& file) const
   {
      std::ifstream candidate(file.c_str(), ios::in | ios::binary);
      char alosFileName[16];

      candidate.seekg(48);
      if (candidate.bad() || candidate.eof())
      {
         return false;
      }
      candidate.read(alosFileName, 16);
      if (candidate.bad() || candidate.eof())
      {
         return false;
      }
      candidate.close();

      ossimString ersString(alosFileName);

      if ((ersString.find("AL1 ") == 0) &&
          (ersString.find("PSR") == 4)  &&
          (ersString.find("SARL") == 8))
      {
         return true;
      }
      else
      {
         return false;
      }

      return true;

   }

   ossimFilename ossimAlosPalsarModel::findAlosPalsarLeader(const ossimFilename& file) const
   {
      ossimFilename leaFile = file;
      ossimString imgPrefix("IMG");
      ossimString trlPrefix("TRL");
      ossimString volPrefix("VOL");
      ossimString leaPrefix("LED");

      ossimString filename = file.fileNoExtension();
      ossimString prefix = filename.substr(0, 3);

      if ((prefix == imgPrefix) ||
          (prefix == trlPrefix) ||
          (prefix == volPrefix))
      {
         // Find the 2nd dash from the end of the string
         // since ALOS files are of the form
         // <prefix>-ALPSRP<identifier>-H<n.n>__A
         filename.replace(0, prefix.size(), leaPrefix);

         leaFile.setFile(filename);
         if (leaFile.exists())
         {
            return leaFile;
         }
      }
      return file;
   }

   bool ossimAlosPalsarModel::isAlosPalsarData(const ossimFilename& file) const
   {
      std::ifstream candidate(file.c_str(), ios::in | ios::binary);
      char alosFileName[16];

      candidate.seekg(48);
      if (candidate.bad() || candidate.eof())
      {
         return false;
      }
      candidate.read(alosFileName, 16);
      if (candidate.bad() || candidate.eof())
      {
         return false;
      }
      candidate.close();

      ossimString ersString(alosFileName);

      if ((ersString.find("AL1 ") == 0) &&
          (ersString.find("PSR") == 4)  &&
          (ersString.find("IMOP") == 8))
      {
         return true;
      }
      else
      {
         return false;
      }

      return true;
   }

   ossimFilename ossimAlosPalsarModel::findAlosPalsarData(const ossimFilename& file) const
   {
      ossimFilename dataFile = file;
      ossimString imgPrefix("IMG-HH"); // Assume the ALOS data always has at least the HH
      ossimString trlPrefix("TRL");
      ossimString volPrefix("VOL");
      ossimString leaPrefix("LED");

      ossimString filename = file.fileNoExtension();
      ossimString prefix = filename.substr(0, 3);
      if ((prefix == leaPrefix) ||
          (prefix == trlPrefix) ||
          (prefix == volPrefix))
      {
         // Find the 2nd dash from the end of the string
         // since ALOS files are of the form
         // <prefix>-ALPSRP<identifier>-H<n.n>__A
         filename.replace(0, prefix.size(), imgPrefix);

         dataFile.setFile(filename);
         if (dataFile.exists())
         {
            return dataFile;
         }
      }
      return file;
   }

} // namespace ossimplugins
