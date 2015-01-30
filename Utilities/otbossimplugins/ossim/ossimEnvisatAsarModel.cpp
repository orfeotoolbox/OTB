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

#include <cmath>
#include <cstdio>

#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordNames.h>

#include <ossimEnvisatAsarModel.h>
#include <otb/GalileanEphemeris.h>
#include <otb/GeographicEphemeris.h>

#include <otb/GMSTDateTime.h>
#include <otb/CivilDateTime.h>
#include <otb/MJDDateTime.h>

#include <otb/PlatformPosition.h>
#include <otb/SensorParams.h>
#include <otb/RefPoint.h>
#include <otb/SarSensor.h>

namespace ossimplugins
{


   RTTI_DEF1(ossimEnvisatAsarModel, "ossimEnvisatAsarModel", ossimGeometricSarSensorModel);

// Static trace for debugging
   static ossimTrace traceDebug("ossimEnvisatAsarModel:debug");

   ossimEnvisatAsarModel::ossimEnvisatAsarModel():
      _n_srgr(0),
      _pixel_spacing(0),
      _EnvisatAsarData(NULL)
   {
   }



   ossimEnvisatAsarModel::~ossimEnvisatAsarModel()
   {
      if(_EnvisatAsarData != NULL)
      {
         delete _EnvisatAsarData;
      }
   }

   ossimString ossimEnvisatAsarModel::getClassName() const
   {
      return ossimString("ossimEnvisatAsarModel");
   }

   ossimObject* ossimEnvisatAsarModel::dup() const
   {
      return new ossimEnvisatAsarModel(*this);
   }

   double ossimEnvisatAsarModel::getSlantRangeFromGeoreferenced(double col) const
   {
      const double CLUM        = 2.99792458e+8 ;

      double GR, GR0, relativeGroundRange, slantRange;

      if (_n_srgr==0) return(-1) ;

      std::vector<double> coefset = FindSRGRSetNumber((_refPoint->get_ephemeris())->get_date()) ;

      GR = _sensor->get_col_direction() * (col)* _pixel_spacing ;
      GR0 = coefset[0];
      relativeGroundRange = GR-GR0;
      slantRange = coefset[1]
         + coefset[2]*relativeGroundRange
         + coefset[3]*pow(relativeGroundRange,2)
         + coefset[4]*pow(relativeGroundRange,3)
         + coefset[5]*pow(relativeGroundRange,4);

      return  slantRange ;
   }


   bool ossimEnvisatAsarModel::open(const ossimFilename& file)
   {
      bool retValue = true;

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "ossimEnvisatAsarTileSource::open(filename) DEBUG: entered..." << std::endl
                                             << "Attempting to open file " << file << std::endl;
      }

      /*
       * Creation of the class allowing to store EnvisatAsarData file metadata
       */
      if (_EnvisatAsarData != NULL)
      {
         delete _EnvisatAsarData;
         _EnvisatAsarData = NULL;
      }

      /*
       * Opening and test of the file
       */
      ossimFilename Filename = file;
      ifstream dataFile(Filename.c_str(), ios::in | ios::binary);
      if (dataFile.eof())
      {
         dataFile.close();
         retValue = false;
      }
      else
      {
         mph mph_rec;
         dataFile >> mph_rec;

         // test if it is an ENVISAT Data file
         if (mph_rec.is_valid())
         {
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG) << "Begin reading EnvisatAsar file" << std::endl;
            }
            /*
             * Reading EnvisatAsarData file data
             */
            dataFile.seekg(0);
            _EnvisatAsarData = new EnvisatAsarData();
            dataFile >> *_EnvisatAsarData;
            dataFile.close();
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG) << "End reading EnvisatAsar file" << std::endl;
            }
         }
         else
         {
            dataFile.close();
            retValue = false;
         }
      }

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "ossimEnvisatAsarTileSource::open() DEBUG: returning..." << std::endl;
      }

      return retValue;
   }


   bool ossimEnvisatAsarModel::saveState(ossimKeywordlist& kwl,
                                         const char* prefix) const
   {
      char name[64];

      kwl.add(prefix, ossimKeywordNames::TYPE_KW, "ossimEnvisatAsarModel", true);

      /*
       * Adding metadata necessary to the sensor model into the keywordlist
       * Data derived from the SPH record
       */
      sph* sph_rec = _EnvisatAsarData->get_sph();
      if(sph_rec != NULL)
      {
         kwl.add(prefix, "pixel_spacing", sph_rec->get_range_spacing().c_str(), true );
         kwl.add(prefix, "line_time_interval", sph_rec->get_line_time_interval().c_str(), true );
      }
      else
      {
         return false;
      }
      /*
       * Adding metadata necessary to the sensor model into the keywordlist
       * Data derived from the Main Processing Parameters record
       */
      MainProcessingParameters* MPP_rec = _EnvisatAsarData->get_MainProcessingParameters();
      if(MPP_rec != NULL)
      {
         kwl.add(prefix, "num_pix", (double)MPP_rec->get_num_samples_per_line(), true );
         kwl.add(prefix, "num_lines", (double)MPP_rec->get_num_output_lines(), true );
         kwl.add(prefix, "wave_length", MPP_rec->get_radar_freq(), true );
         kwl.add(prefix, "range_sampling_rate", MPP_rec->get_range_samp_rate(), true );
         kwl.add(prefix, "prf", MPP_rec->get_prf(), true );
         kwl.add(prefix, "avg_scene_height", MPP_rec->get_avg_scene_height_ellpsoid(), true );
         kwl.add(prefix, "is_groundrange", MPP_rec->get_srgr_flag(), true );
         kwl.add(prefix, "state_vector_time_1_day", (double)MPP_rec->get_state_vector_time_1_day(), true );
         kwl.add(prefix, "state_vector_time_1_sec", (double)MPP_rec->get_state_vector_time_1_sec(), true );
         kwl.add(prefix, "state_vector_time_1_microsec", (double)MPP_rec->get_state_vector_time_1_microsec(), true );
         kwl.add(prefix, "x_pos_1", (double)MPP_rec->get_x_pos_1(), true );
         kwl.add(prefix, "y_pos_1", (double)MPP_rec->get_y_pos_1(), true );
         kwl.add(prefix, "z_pos_1", (double)MPP_rec->get_z_pos_1(), true );
         kwl.add(prefix, "x_vel_1", (double)MPP_rec->get_x_vel_1(), true );
         kwl.add(prefix, "y_vel_1", (double)MPP_rec->get_y_vel_1(), true );
         kwl.add(prefix, "z_vel_1", (double)MPP_rec->get_z_vel_1(), true );
         kwl.add(prefix, "state_vector_time_2_day",(double) MPP_rec->get_state_vector_time_2_day(), true );
         kwl.add(prefix, "state_vector_time_2_sec", (double)MPP_rec->get_state_vector_time_2_sec(), true );
         kwl.add(prefix, "state_vector_time_2_microsec", (double)MPP_rec->get_state_vector_time_2_microsec(), true );
         kwl.add(prefix, "x_pos_2", (double)MPP_rec->get_x_pos_2(), true );
         kwl.add(prefix, "y_pos_2", (double)MPP_rec->get_y_pos_2(), true );
         kwl.add(prefix, "z_pos_2", (double)MPP_rec->get_z_pos_2(), true );
         kwl.add(prefix, "x_vel_2", (double)MPP_rec->get_x_vel_2(), true );
         kwl.add(prefix, "y_vel_2", (double)MPP_rec->get_y_vel_2(), true );
         kwl.add(prefix, "z_vel_2", (double)MPP_rec->get_z_vel_2(), true );
         kwl.add(prefix, "state_vector_time_3_day", (double)MPP_rec->get_state_vector_time_3_day(), true );
         kwl.add(prefix, "state_vector_time_3_sec", (double)MPP_rec->get_state_vector_time_3_sec(), true );
         kwl.add(prefix, "state_vector_time_3_microsec", (double)MPP_rec->get_state_vector_time_3_microsec(), true );
         kwl.add(prefix, "x_pos_3", (double)MPP_rec->get_x_pos_3(), true );
         kwl.add(prefix, "y_pos_3", (double)MPP_rec->get_y_pos_3(), true );
         kwl.add(prefix, "z_pos_3", (double)MPP_rec->get_z_pos_3(), true );
         kwl.add(prefix, "x_vel_3", (double)MPP_rec->get_x_vel_3(), true );
         kwl.add(prefix, "y_vel_3", (double)MPP_rec->get_y_vel_3(), true );
         kwl.add(prefix, "z_vel_3", (double)MPP_rec->get_z_vel_3(), true );
         kwl.add(prefix, "state_vector_time_4_day", (double)MPP_rec->get_state_vector_time_4_day(), true );
         kwl.add(prefix, "state_vector_time_4_sec", (double)MPP_rec->get_state_vector_time_4_sec(), true );
         kwl.add(prefix, "state_vector_time_4_microsec", (double)MPP_rec->get_state_vector_time_4_microsec(), true );
         kwl.add(prefix, "x_pos_4", (double)MPP_rec->get_x_pos_4(), true );
         kwl.add(prefix, "y_pos_4", (double)MPP_rec->get_y_pos_4(), true );
         kwl.add(prefix, "z_pos_4", (double)MPP_rec->get_z_pos_4(), true );
         kwl.add(prefix, "x_vel_4", (double)MPP_rec->get_x_vel_4(), true );
         kwl.add(prefix, "y_vel_4", (double)MPP_rec->get_y_vel_4(), true );
         kwl.add(prefix, "z_vel_4", (double)MPP_rec->get_z_vel_4(), true );
         kwl.add(prefix, "state_vector_time_5_day", (double)MPP_rec->get_state_vector_time_5_day(), true );
         kwl.add(prefix, "state_vector_time_5_sec", (double)MPP_rec->get_state_vector_time_5_sec(), true );
         kwl.add(prefix, "state_vector_time_5_microsec", (double)MPP_rec->get_state_vector_time_5_microsec(), true );
         kwl.add(prefix, "x_pos_5", (double)MPP_rec->get_x_pos_5(), true );
         kwl.add(prefix, "y_pos_5",(double) MPP_rec->get_y_pos_5(), true );
         kwl.add(prefix, "z_pos_5", (double)MPP_rec->get_z_pos_5(), true );
         kwl.add(prefix, "x_vel_5", (double)MPP_rec->get_x_vel_5(), true );
         kwl.add(prefix, "y_vel_5", (double)MPP_rec->get_y_vel_5(), true );
         kwl.add(prefix, "z_vel_5", (double)MPP_rec->get_z_vel_5(), true );
      }
      else
      {
         return false;
      }
      /*
       * Adding metadata necessary to the sensor model into the keywordlist
       * Data derived from the Geolocation Grid record - Reference Point
       */
      GeolocationGrid* GG_rec = _EnvisatAsarData->get_GeolocationGrid(0);
      if(GG_rec != NULL)
      {
         kwl.add(prefix, "first_zero_doppler_time_day", (double)GG_rec->get_first_zero_doppler_time_day(), true );
         kwl.add(prefix, "first_zero_doppler_time_sec", (double)GG_rec->get_first_zero_doppler_time_sec(), true );
         kwl.add(prefix, "first_zero_doppler_time_microsec", (double)GG_rec->get_first_zero_doppler_time_microsec(), true );
         kwl.add(prefix, "line_num", (double)GG_rec->get_line_num(), true );
         kwl.add(prefix, "samp_num", (double)(GG_rec->get_samp_numbers())[0], true );
         kwl.add(prefix, "slant_range_time", (double)(GG_rec->get_slant_range_times())[0], true );
      }
      else
      {
         return false;
      }
      /*
       * Adding metadata necessary to the sensor model into the keywordlist
       * Data derived from the Geolocation Grid record - Corners
       */
      GG_rec = _EnvisatAsarData->get_GeolocationGrid(0);
      if(GG_rec != NULL)
      {
         kwl.add(prefix, "UL_line", (double)GG_rec->get_line_num(), true );
         kwl.add(prefix, "UL_col", (double)(GG_rec->get_samp_numbers())[0], true );
         kwl.add(prefix, "UL_lat", (double)(GG_rec->get_lats())[0], true );
         kwl.add(prefix, "UL_lon", (double)(GG_rec->get_longs())[0], true );
         kwl.add(prefix, "UR_line", (double)GG_rec->get_line_num(), true );
         kwl.add(prefix, "UR_col", (double)(GG_rec->get_samp_numbers())[10], true );
         kwl.add(prefix, "UR_lat", (double)(GG_rec->get_lats())[10], true );
         kwl.add(prefix, "UR_lon", (double)(GG_rec->get_longs())[10], true );
      }
      else
      {
         return false;
      }
      GG_rec = _EnvisatAsarData->get_GeolocationGrid(10);
      if(GG_rec != NULL)
      {
         kwl.add(prefix, "LL_line", (double)GG_rec->get_line_num(), true );
         kwl.add(prefix, "LL_col", (double)(GG_rec->get_samp_numbers())[0], true );
         kwl.add(prefix, "LL_lat", (double)(GG_rec->get_lats())[0], true );
         kwl.add(prefix, "LL_lon", (double)(GG_rec->get_longs())[0], true );
         kwl.add(prefix, "LR_line", (double)GG_rec->get_line_num(), true );
         kwl.add(prefix, "LR_col", (double)(GG_rec->get_samp_numbers())[10], true );
         kwl.add(prefix, "LR_lat", (double)(GG_rec->get_lats())[10], true );
         kwl.add(prefix, "LR_lon", (double)(GG_rec->get_longs())[10], true );
      }
      else
      {
         return false;
      }

      /*
       * Adding metadata necessary to the sensor model into the keywordlist
       */
      int n_srgr = 0;
      SRGRConversionParameters * SRGRParameters = _EnvisatAsarData->get_SRGRConversionParameters(0);
      if(SRGRParameters != NULL)
      {
         n_srgr = _EnvisatAsarData->get_num_ds(SRGRParameters);

         for (int i=0;i<n_srgr;i++)
         {
            SRGRConversionParameters * SRGR = _EnvisatAsarData->get_SRGRConversionParameters(i);

            sprintf(name,"srgr_update_day%i",i);
            kwl.add(prefix, name, (double)SRGR->get_first_zero_doppler_time_day(),true);

            sprintf(name,"srgr_update_sec%i",i);
            kwl.add(prefix, name, (double)SRGR->get_first_zero_doppler_time_sec(),true);

            sprintf(name,"srgr_update_microsec%i",i);
            kwl.add(prefix, name, (double)SRGR->get_first_zero_doppler_time_microsec(),true);

            sprintf(name,"srgr_coef%iA",i);
            kwl.add(prefix, name,(double)SRGR->get_ground_range_origin(),true);

            sprintf(name,"srgr_coef%iB",i);
            kwl.add(prefix, name,(double)SRGR->get_srgr_coef()[0],true);
            sprintf(name,"srgr_coef%iC",i);
            kwl.add(prefix, name,(double)SRGR->get_srgr_coef()[1],true);
            sprintf(name,"srgr_coef%iD",i);
            kwl.add(prefix, name,(double)SRGR->get_srgr_coef()[2],true);
            sprintf(name,"srgr_coef%iE",i);
            kwl.add(prefix, name,(double)SRGR->get_srgr_coef()[3]*1e10,true);
            sprintf(name,"srgr_coef%iF",i);
            kwl.add(prefix, name,(double)SRGR->get_srgr_coef()[4]*1e10,true);
         }
      }

      kwl.add(prefix, "n_srgr" , n_srgr,true);

      return true;
   }

   bool ossimEnvisatAsarModel::loadState (const ossimKeywordlist &kwl,
                                          const char *prefix)
   {
      InitSensorParams(kwl, prefix);
      InitPlatformPosition(kwl, prefix);
      InitRefPoint(kwl, prefix);
      InitSRGR(kwl, prefix);
      return true;
   }

   std::ostream& ossimEnvisatAsarModel::print(std::ostream& out) const
   {
      // Capture the original flags.
      std::ios_base::fmtflags f = out.flags();

      out << setprecision(15) << setiosflags(ios::fixed)
          << "\nossimEnvisatAsarModel data members:\n"
          << "_pixel_spacing: " << _pixel_spacing << "\n"
          << "_n_srgr: " << _n_srgr << "\n";

      ossimGeometricSarSensorModel::print(out);

      // Reset flags.
      out.setf(f);


      return out;
   }

   bool ossimEnvisatAsarModel::InitSensorParams(const ossimKeywordlist &kwl, const char *prefix)
   {
      const double CLUM = 2.99792458e+8;
      const char* wave_length_str = kwl.find(prefix,"wave_length");
      double wave_length = CLUM / atof(wave_length_str);
      const char* fr_str = kwl.find(prefix,"range_sampling_rate");
      double fr = atof(fr_str);

      // product type : Slant Range or Ground Range
      _isProductGeoreferenced = atoi(kwl.find(prefix,"is_groundrange"));
      double fa;
      if (!_isProductGeoreferenced)
      {
         const char* fa_str = kwl.find(prefix,"prf");
         fa = atof(fa_str);
      }
      else
      {
         const char* fa_str = kwl.find(prefix,"line_time_interval");
         fa = 1.0/atof(fa_str);
      }

      // TODO : v�rifier que ceci est ok pour les produits IMP
      const char* time_dir_pix = "INCREASE";
      const char* time_dir_lin = "INCREASE";

      if(_sensor != NULL)
      {
         delete _sensor;
      }

      _sensor = new SensorParams();

      if(strcmp(time_dir_pix, "INCREASE") == 0)
      {
         _sensor->set_col_direction(1);
      }
      else
      {
         _sensor->set_col_direction(-1);
      }

      if(strcmp(time_dir_lin, "INCREASE") == 0)
      {
         _sensor->set_lin_direction(1);
      }
      else
      {
         _sensor->set_lin_direction(-1);
      }

      _sensor->set_prf(fa);
      _sensor->set_sf(fr);
      _sensor->set_rwl(wave_length);

      return true;
   }

   bool ossimEnvisatAsarModel::InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix)
   {
      const int neph    = 5;
      // long val_long;
      // unsigned long val_ulong1;
      // unsigned long val_ulong2;
      CivilDateTime ref_civil_date;

      Ephemeris** ephemeris = new Ephemeris*[neph];

      for (int i=1;i<=neph;i++)
      {
         double pos[3];
         double vel[3];
         char name[64];

         /*
          * Date de l'ephemeride
          */
         sprintf(name,"state_vector_time_%i_day",i);
         long day = (long) atof(kwl.find(prefix,name));
         sprintf(name,"state_vector_time_%i_sec",i);
         unsigned long sec = (unsigned long) atof(kwl.find(prefix,name));
         sprintf(name,"state_vector_time_%i_microsec",i);
         unsigned long microsec = (unsigned long) atof(kwl.find(prefix,name));
         MJDDateTime mjdDate(day, sec, microsec);
         JSDDateTime jsd_date(mjdDate);
         /*
          * Position de l'ephemeride
          */
         sprintf(name,"x_pos_%i",i);
         pos[0] = (double) atoi(kwl.find(prefix,name)) * 1.0e-2;
         sprintf(name,"y_pos_%i",i);
         pos[1] = (double) atoi(kwl.find(prefix,name)) * 1.0e-2;
         sprintf(name,"z_pos_%i",i);
         pos[2] = (double) atoi(kwl.find(prefix,name)) * 1.0e-2;
         /*
          * Vitesse de l'ephemeride
          */
         sprintf(name,"x_vel_%i",i);
         vel[0] = (double) atoi(kwl.find(prefix,name)) * 1.0e-5;
         sprintf(name,"y_vel_%i",i);
         vel[1] = (double) atoi(kwl.find(prefix,name)) * 1.0e-5;
         sprintf(name,"z_vel_%i",i);
         vel[2] = (double) atoi(kwl.find(prefix,name)) * 1.0e-5;

         GeographicEphemeris* eph = new GeographicEphemeris(jsd_date, pos, vel);

         ephemeris[i-1] = eph;
      }

      if (_platformPosition != NULL)
      {
         delete _platformPosition;
      }
      _platformPosition = new PlatformPosition(ephemeris,neph);

      /*
       * Liberation de la memoire utilis�e par la liste des ephemerides
       * ->Le constructeur effectue une copie des ephemerides
       */
      for (int i=0;i<neph;i++)
      {
         delete ephemeris[i];
      }
      delete[] ephemeris;

      return true;
   }



   bool ossimEnvisatAsarModel::InitRefPoint(const ossimKeywordlist &kwl, const char *prefix)
   {

      // Reference image position
      if(_refPoint == NULL)
      {
         _refPoint = new RefPoint();
      }
      double sc_pix = atof(kwl.find(prefix,"samp_num")) - 1.0;
      _refPoint->set_pix_col(sc_pix);
      double sc_lin = atof(kwl.find(prefix,"line_num")) - 1.0;
      _refPoint->set_pix_line(sc_lin);

      // Reference Point distance
      double c = 2.99792458e+8;
      double distance = atof(kwl.find(prefix,"slant_range_time")) * (c/2.0) * 1.0e-9;
      _refPoint->set_distance(distance);


      // Reference Point Ephemeris
      long day_ref = (long)atof(kwl.find(prefix,"first_zero_doppler_time_day"));
      unsigned long sec_ref = (unsigned long)atof(kwl.find(prefix,"first_zero_doppler_time_sec"));
      unsigned long microsec_ref = (unsigned long)atof(kwl.find(prefix,"first_zero_doppler_time_microsec"));

      MJDDateTime mjdDate_ref(day_ref, sec_ref, microsec_ref);
      JSDDateTime jsd_date_ref(mjdDate_ref);

      if(_platformPosition != NULL)
      {
         Ephemeris * ephemeris = _platformPosition->Interpolate(jsd_date_ref);
         if (ephemeris == NULL) return false ;
         _refPoint->set_ephemeris(ephemeris);
         delete ephemeris;
      }
      else return false;

      // Adjustement of the model using the four corner points
      // in order to use ossimSensorModel::lineSampleToWorld
      const char* nbCol_str = kwl.find(prefix,"num_pix");
      const char* nbLin_str = kwl.find(prefix,"num_lines");
      theImageSize.x      = atoi(nbCol_str);
      theImageSize.y      = atoi(nbLin_str);
      theImageClipRect    = ossimDrect(0, 0, theImageSize.x-1, theImageSize.y-1);

      // Ground Control Points extracted from the model : corner points
      std::list<ossimGpt> groundGcpCoordinates ;
      std::list<ossimDpt> imageGcpCoordinates ;
      double h = atof(kwl.find("avg_scene_height"));

      // first line first pix
      double line = atof(kwl.find("UL_line")) - 1;
      double col  = atof(kwl.find("UL_col"))  - 1;
      double lon  = atof(kwl.find("UL_lon")) * 1e-6;
      double lat  = atof(kwl.find("UL_lat")) * 1e-6;
      if (lon > 180.0) lon -= 360.0;
      ossimDpt imageGCP1(col,line);
      ossimGpt groundGCP1(lat, lon, h);
      groundGcpCoordinates.push_back(groundGCP1) ;
      imageGcpCoordinates.push_back(imageGCP1) ;
      // first line last pix
      line = atof(kwl.find("UR_line")) - 1;
      col  = atof(kwl.find("UR_col"))  - 1;
      lon  = atof(kwl.find("UR_lon")) * 1e-6;
      lat  = atof(kwl.find("UR_lat")) * 1e-6;
      if (lon > 180.0) lon -= 360.0;
      ossimDpt imageGCP2(col,line);
      ossimGpt groundGCP2(lat, lon, h);
      groundGcpCoordinates.push_back(groundGCP2) ;
      imageGcpCoordinates.push_back(imageGCP2) ;
      // last line last pix
      line = atof(kwl.find("LR_line")) - 1;
      col  = atof(kwl.find("LR_col"))  - 1;
      lon  = atof(kwl.find("LR_lon")) * 1e-6;
      lat  = atof(kwl.find("LR_lat")) * 1e-6;
      if (lon > 180.0) lon -= 360.0;
      ossimDpt imageGCP3(col,line);
      ossimGpt groundGCP3(lat, lon, h);
      groundGcpCoordinates.push_back(groundGCP3) ;
      imageGcpCoordinates.push_back(imageGCP3) ;
      // last line first pix
      line = atof(kwl.find("LL_line")) - 1;
      col  = atof(kwl.find("LL_col"))  - 1;
      lon  = atof(kwl.find("LL_lon")) * 1e-6;
      lat  = atof(kwl.find("LL_lat")) * 1e-6;
      if (lon > 180.0) lon -= 360.0;
      ossimDpt imageGCP4(col,line);
      ossimGpt groundGCP4(lat, lon, h);
      groundGcpCoordinates.push_back(groundGCP4) ;
      imageGcpCoordinates.push_back(imageGCP4) ;

      // Default optimization
      optimizeModel(groundGcpCoordinates, imageGcpCoordinates) ;

      return true;
   }
   bool ossimEnvisatAsarModel::InitSRGR(const ossimKeywordlist &kwl, const char *prefix)
   {
      // product type : Slant Range or Ground Range
      _isProductGeoreferenced = atoi(kwl.find(prefix,"is_groundrange"));

      // pixel spacing
      _pixel_spacing = atof(kwl.find(prefix,"pixel_spacing"));

      // number of SRGR sets
      _n_srgr = atoi(kwl.find(prefix,"n_srgr"));


      char name[64];
      for (int i=0;i<_n_srgr;i++)
      {
         // SRGR update time
         sprintf(name,"srgr_update_day%i",i);
         long day_ref = (long)atof(kwl.find(prefix,name));
         sprintf(name,"srgr_update_sec%i",i);
         unsigned long sec_ref = (unsigned long)atof(kwl.find(prefix,name));
         sprintf(name,"srgr_update_microsec%i",i);
         unsigned long microsec_ref = (unsigned long)atof(kwl.find(prefix,name));
         MJDDateTime mjdDate_ref(day_ref, sec_ref, microsec_ref);
         JSDDateTime jsd_date_ref(mjdDate_ref);
         _srgr_update.push_back(jsd_date_ref);

         std::vector<double> srgr_set;
         // SRGR coefficients
         sprintf(name,"srgr_coef%iA",i);
         const char* srgr_coef_strA = kwl.find(prefix,name);
         srgr_set.push_back(atof(srgr_coef_strA));
         sprintf(name,"srgr_coef%iB",i);
         const char* srgr_coef_strB = kwl.find(prefix,name);
         srgr_set.push_back(atof(srgr_coef_strB));
         sprintf(name,"srgr_coef%iC",i);
         const char* srgr_coef_strC = kwl.find(prefix,name);
         srgr_set.push_back(atof(srgr_coef_strC));
         sprintf(name,"srgr_coef%iD",i);
         const char* srgr_coef_strD = kwl.find(prefix,name);
         srgr_set.push_back(atof(srgr_coef_strD));
         sprintf(name,"srgr_coef%iE",i);
         const char* srgr_coef_strE = kwl.find(prefix,name);
         srgr_set.push_back(atof(srgr_coef_strE)*1e-10);
         sprintf(name,"srgr_coef%iF",i);
         const char* srgr_coef_strF = kwl.find(prefix,name);
         srgr_set.push_back(atof(srgr_coef_strF)*1e-10);

         _srgr_coefset.push_back(srgr_set);
      }


      return true;
   }


   std::vector<double> ossimEnvisatAsarModel::FindSRGRSetNumber(JSDDateTime date) const
   {
      //if (_n_srgr==0) return NULL ;

      std::vector<double> delays;
      double delay;
      for (int i=0;i<_n_srgr;i++)
      {
         JSDDateTime datetmp(_srgr_update.at(i));
         delay = date.get_second()+date.get_decimal() - (datetmp.get_second() + datetmp.get_decimal());
         delays.push_back( fabs(delay) );
      }

      int setNumber = 0 ;
      double min_delay = delays[0] ;
      for (int i=1;i<_n_srgr;i++)
      {
         if (delays[i]<min_delay) {
            setNumber = i ;
            min_delay = delays[i] ;
         }
      }

      return _srgr_coefset[setNumber];
   }
}
