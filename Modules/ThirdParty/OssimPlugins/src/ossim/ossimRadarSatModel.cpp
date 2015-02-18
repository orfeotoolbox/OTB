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

#include <ossimRadarSatModel.h>
#include <otb/GalileanEphemeris.h>
#include <otb/GeographicEphemeris.h>

#include <otb/JSDDateTime.h>
#include <otb/GMSTDateTime.h>
#include <otb/CivilDateTime.h>

#include <otb/PlatformPosition.h>
#include <otb/SensorParams.h>
#include <otb/RefPoint.h>
#include <otb/SarSensor.h>

#include <RadarSat/Data/DataFactory.h>
#include <RadarSat/Data/ImageOptionsFileDescriptor.h>

#include <RadarSat/VolumeDir/VolumeDescriptorRecord.h>
#include <RadarSat/VolumeDir/VolumeDirFactory.h>

#include <RadarSat/Data/ProcessedDataRecord.h>
#include <RadarSat/CommonRecord/ProcessingParameters.h>
#include <RadarSat/Leader/PlatformPositionData.h>

namespace ossimplugins
{
RTTI_DEF1(ossimRadarSatModel, "ossimRadarSatModel", ossimGeometricSarSensorModel);

// Static trace for debugging
static ossimTrace traceDebug("ossimRadarSatModel:debug");

ossimRadarSatModel::ossimRadarSatModel():
  ossimGeometricSarSensorModel(),
  _n_srgr(0),
  _pixel_spacing(0),
  _data(NULL),
  _leader(NULL),
  _trailer(NULL),
  _volumeDir(NULL)
{
}

ossimRadarSatModel::ossimRadarSatModel(const ossimRadarSatModel& rhs):
  ossimGeometricSarSensorModel(rhs),
  _n_srgr(rhs._n_srgr),
  _pixel_spacing(rhs._pixel_spacing),
  _data(new Data(*(rhs._data))),
  _leader(new Leader(*(rhs._leader))),
  _trailer(new Trailer(*(rhs._trailer))),
  _volumeDir(new VolumeDir(*(rhs._volumeDir)))
{
}

ossimRadarSatModel::~ossimRadarSatModel()
{
  if (_data != 0)
  {
    delete _data;
  }

  if (_leader != 0)
  {
    delete _leader;
  }
  
  if (_trailer != 0)
  {
    delete _trailer;
  }

  if (_volumeDir != 0)
  {
    delete _volumeDir;
  }
}

ossimObject* ossimRadarSatModel::dup() const
{
   return new ossimRadarSatModel(*this);
}

ossimString ossimRadarSatModel::getClassName() const
{
   return ossimString("ossimRadarSatModel");
}

double ossimRadarSatModel::getSlantRangeFromGeoreferenced(double col) const
{
  if (_n_srgr==0) return(-1) ;

  double relativeGroundRange ;

  // in the case of Georeferenced images, _refPoint->get_distance() contains the ground range
  relativeGroundRange = _refPoint->get_distance() + _sensor->get_col_direction() * (col-_refPoint->get_pix_col())* _pixel_spacing ;

  int numSet = FindSRGRSetNumber((_refPoint->get_ephemeris())->get_date()) ;
  /**
   * @todo : could be improved (date choice)
   */

  double slantRange = _srgr_coefset[numSet][0]
              + _srgr_coefset[numSet][1]*relativeGroundRange
              + _srgr_coefset[numSet][2]*pow(relativeGroundRange,2)
              + _srgr_coefset[numSet][3]*pow(relativeGroundRange,3)
              + _srgr_coefset[numSet][4]*pow(relativeGroundRange,4)
              + _srgr_coefset[numSet][5]*pow(relativeGroundRange,5);

  return  slantRange ;
}

bool ossimRadarSatModel::open(const ossimFilename& file)
{
  //traceDebug.setTraceFlag(true);
  if(traceDebug())
    {
    ossimNotify(ossimNotifyLevel_DEBUG)
      << "ossimRadarSatTileSource::open(filename) DEBUG: entered..."
      << std::endl
      << "Attempting to open file " << file << std::endl;
    }

  bool retValue = true;
  ossimFilename tempFilename = file;

  /*
   * Detect if we use vdf file or dat file
   */
  ossimFilename dataFilePath;
  ossimFilename volumeDirectoryFilePath;
  std::string input_file = file;
  string::size_type loc_DAT = input_file.find( "DAT_01", 0 );
  string::size_type loc_dat = input_file.find( "dat_01", 0 );
  if ( (loc_DAT != string::npos ) || ( loc_dat != string::npos ) )
    {
    dataFilePath = input_file.c_str();
    if (loc_DAT != string::npos ) input_file.replace(loc_DAT, 6, "VDF_DAT");
    if (loc_dat != string::npos ) input_file.replace(loc_dat, 6, "vdf_dat");
    volumeDirectoryFilePath = input_file.c_str();
    }
  else
    {
    string::size_type loc_VDF = input_file.find( "VDF_DAT", 0 );
    string::size_type loc_vdf = input_file.find( "vdf_dat", 0 );
    if ( (loc_VDF != string::npos ) || ( loc_vdf != string::npos ) )
      {
      volumeDirectoryFilePath = input_file.c_str();
      if (loc_VDF != string::npos ) input_file.replace(loc_VDF, 7, "DAT_01");
      if (loc_vdf != string::npos ) input_file.replace(loc_vdf, 7, "dat_01");
      dataFilePath = input_file.c_str();
      }
    else
      {
      if(traceDebug())
        {
        ossimNotify(ossimNotifyLevel_DEBUG)
             << "File Name not coherent (searching for *DAT_01* or *dat_01* or *vdf_dat* or *VDF_DAT* )  : " << file << std::endl;
        }
      return false;
      }
    }

   //Creation of the class allowing to store the metadata from the Volume Directory File
  if (_volumeDir != NULL)
    {
    delete _volumeDir;
    _volumeDir = NULL;
    }
  _volumeDir = new VolumeDir();

  //Creation of the class allowing to store the metadata from the Data file
  if (_data != NULL)
    {
    delete _data;
    _data = NULL;
    }
  _data = new Data();

  //Creation of the class allowing to store the metadata from the Leader file
  if(_leader != NULL)
    {
    delete _leader;
    _leader = NULL;
    }
  _leader = new Leader();

  // Creation of the class allowing to store the metadata from the Trailer file
  if(_trailer != NULL)
    {
    delete _trailer;
    _trailer = NULL;
    }
  _trailer = new Trailer();


  RadarSatRecordHeader headerVDF;
  VolumeDirFactory factoryVDF;
  ifstream volumeDirFile (volumeDirectoryFilePath.c_str(), ios::in|ios::binary);
  volumeDirFile>>headerVDF;
  if(volumeDirFile.eof())
    {
    volumeDirFile.close();
    retValue =  false;
    }
  else
    {
    if(traceDebug())
      {
      ossimNotify(ossimNotifyLevel_DEBUG) << "Begin reading VDF file" << std::endl;
      }

    RadarSatRecord* recordVDF = factoryVDF.Instanciate(headerVDF.get_rec_seq());
    if (recordVDF != NULL && headerVDF.get_rec_seq() == 1)
      {
      recordVDF->Read(volumeDirFile);

      //Test if the input File is a valid Radarsat VDF file
      if ((((VolumeDescriptorRecord*) recordVDF)->get_logvol_id()).substr(0, 10) == "RSAT-1-SAR")
        {
        std::string subProduct = (((VolumeDescriptorRecord*) recordVDF)->get_logvol_id()).substr(11, 3);
        if  ( (subProduct == "RAW") || (subProduct == "SPG") || (subProduct == "SSG") )
          {
          if(traceDebug())
            ossimNotify(ossimNotifyLevel_DEBUG) << "RAW, SPG and SSG format are not supported" <<std::endl;

          volumeDirFile.close();
          return false;
          }
        else if ( (subProduct == "SCN") || (subProduct == "SGX") || (subProduct == "SLC") )
          {
          if(traceDebug())
            ossimNotify(ossimNotifyLevel_DEBUG) << "SCN, SGX and SLC format are supported but not tested" <<std::endl;
          }
        else if ( (subProduct ==  "SCW") || (subProduct == "SGF") )
          {
          if(traceDebug())
            ossimNotify(ossimNotifyLevel_DEBUG) << "SCW and SGF format are supported and tested" <<std::endl;
          }

        //Reading of the remaining of the volume directory file

        volumeDirFile.close();
        volumeDirFile.open(volumeDirectoryFilePath.c_str(), ios::in | ios::binary);
        volumeDirFile >> *_volumeDir;
        volumeDirFile.close();

        if (traceDebug())
          {
          ossimNotify(ossimNotifyLevel_DEBUG) << "End reading VDF file" << std::endl;
          }

        RadarSatRecordHeader headerDAT;
        DataFactory factoryDAT;
        ifstream dataFile (dataFilePath.c_str(), ios::in|ios::binary);
        dataFile>>headerDAT;
        if(dataFile.eof())
          {
          dataFile.close();
          retValue =  false;
          }
        else
          {
          if(traceDebug())
            {
            ossimNotify(ossimNotifyLevel_DEBUG)<< "Begin reading DAT file" << std::endl;
            }

          RadarSatRecord* recordDAT = factoryDAT.Instanciate(headerDAT.get_rec_seq());
          if (recordDAT != NULL && headerDAT.get_rec_seq() == 1)
            {
            recordDAT->Read(dataFile);
            dataFile.close();
            /*
             * Reading the remaining of the data file
             */
            dataFile.open(dataFilePath.c_str(), ios::in|ios::binary);
            dataFile>>*_data;
            dataFile.close();

            /*
             * Commenting this line :
             * With SARDEGNA file (a corrupted file without number of lines)
             * this line makes disappear the auto-computation of number of lines
             * by file seeking.
             * It does not seem to add any information either (same keywordlist)
             *
             * TODO : investigate : what is it supposed to do ?
             */
            //_data->InsertRecord(headerDAT.get_rec_seq(), recordDAT);

            if(traceDebug())
              {
              ossimNotify(ossimNotifyLevel_DEBUG) << "End reading DAT file" << std::endl;
              }
            } // TODO move to end
          }// TODO move to end

        /*
         * Leader file path construction from the DAT file path
         * Warning : the filename case has to be homogenous
         */
        std::string leader_file = dataFilePath;
        string::size_type loc = leader_file.find( "DAT_01", 0 );
        if( loc != string::npos ) leader_file.replace(loc, 6, "LEA_01" ); // upper case test
        else
          {
          loc = leader_file.find( "dat_01", 0 );
          if( loc != string::npos ) leader_file.replace(loc, 6, "lea_01" ); // lower case test
          else
            {
            ossimNotify(ossimNotifyLevel_DEBUG) << "File Name not coherent (searching for *DAT_01* or *dat_01*)  : " << file << std::endl;
            }
          }
        ossimFilename leaderFilePath(leader_file);
        if (!leaderFilePath.exists())
          {
          ossimNotify(ossimNotifyLevel_DEBUG) << "Leader file not found (searching for *lea_01* coherent with *dat_01*)  : " << file << std::endl;
          retValue = false;
          }
        else
          {
          if(traceDebug())
            {
            ossimNotify(ossimNotifyLevel_DEBUG) << "Begin reading Leader file" << std::endl;
            }
          /*
           * Leader file data reading
           */
          ifstream leaderFile (leaderFilePath.c_str(), ios::in|ios::binary);
          leaderFile>>*_leader;
          leaderFile.close();
          if(traceDebug())
            {
            ossimNotify(ossimNotifyLevel_DEBUG) << "End reading Leader file" << std::endl;
            }
          }

        /*
         * Trailer file path construction from the DAT file path
         * Warning : the filename case has to be homogenous
         */
        std::string trailer_file = dataFilePath;
        loc = trailer_file.find( "DAT_01", 0 );
        if( loc != string::npos ) trailer_file.replace(loc, 6, "TRA_01" ); // upper case test
        else
          {
          loc = trailer_file.find( "dat_01", 0 );
          if( loc != string::npos ) trailer_file.replace(loc, 6, "tra_01" ); // lower case test
          else
            {
            ossimNotify(ossimNotifyLevel_DEBUG) << "File Name not coherent (searching for *DAT_01* or *dat_01*)  : " << file << std::endl;
            }
          }
        ossimFilename trailerFilePath(trailer_file);
        if (!trailerFilePath.exists())
          {
          ossimNotify(ossimNotifyLevel_DEBUG) << "Trailer file not found (searching for *tra_01* coherent with *dat_01*)  : " << file << std::endl;
          retValue = false;
          }
        else
          {
          if(traceDebug())
            {
            ossimNotify(ossimNotifyLevel_DEBUG) << "Begin reading Trailer file" << std::endl;
            }
          /*
           * Trailer file data reading
           */
          ifstream trailerFile (trailerFilePath.c_str(), ios::in|ios::binary);
          trailerFile>>*_trailer;
          trailerFile.close();
          if(traceDebug())
            {
            ossimNotify(ossimNotifyLevel_DEBUG) << "End reading Trailer file" << std::endl;
            }
          }
        }
      else
        {
        volumeDirFile.close();
        retValue = false;
        }
      }
    else
      {
      volumeDirFile.close();
      retValue = false;
      }
    }

  ossimKeywordlist geom_kwl;
  this->internalSaveState(geom_kwl);
  this->internalLoadState(geom_kwl);

  // Assign the ossimSensorModel::theBoundGndPolygon
  ossimGpt ul;
  ossimGpt ur;
  ossimGpt lr;
  ossimGpt ll;
  lineSampleToWorld(theImageClipRect.ul(), ul);
  lineSampleToWorld(theImageClipRect.ur(), ur);
  lineSampleToWorld(theImageClipRect.lr(), lr);
  lineSampleToWorld(theImageClipRect.ll(), ll);
  setGroundRect(ul, ur, lr, ll);  // ossimSensorModel method.

  if(traceDebug())
    {
    ossimNotify(ossimNotifyLevel_DEBUG) << "ossimRadarSatModel::open() DEBUG: returning..." << std::endl;
    }

  return retValue;
}


bool ossimRadarSatModel::internalSaveState(ossimKeywordlist& kwl,
                                           const char* prefix) const
{
  static const char MODULE[] = "ossimRadarSatModel::internalSaveState";

  if (traceDebug())
  {
     ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
  }

  char name[64];

    kwl.add(prefix, ossimKeywordNames::TYPE_KW, "ossimRadarSatModel", true);

    FileDescriptor * fileDescriptor = _leader->get_FileDescriptor() ;
    if(fileDescriptor != NULL)
    {
      kwl.add(prefix, "file_name",fileDescriptor->get_file_name().c_str(),true);
    }
    else
    {
      return false;
    }

    DataSetSummary * datasetSummary = _leader->get_DataSetSummary();
    if(datasetSummary == NULL)
      {
      delete datasetSummary;
      datasetSummary = _trailer->get_DataSetSummary();
      }

    if(datasetSummary != NULL)
    {
      kwl.add(prefix, "inp_sctim",datasetSummary->get_inp_sctim().c_str(),true);

      kwl.add(prefix, "asc_des",datasetSummary->get_asc_des().c_str(),true);

      kwl.add(prefix, "pro_lat",datasetSummary->get_pro_lat(),true);
      kwl.add(prefix, "pro_long",datasetSummary->get_pro_long(),true);
      kwl.add(prefix, "pro_head",datasetSummary->get_pro_head(),true);

      kwl.add(prefix, "ellip_des",datasetSummary->get_ellip_des().c_str(),true);
      kwl.add(prefix, "ellip_maj",datasetSummary->get_ellip_maj(),true);
      kwl.add(prefix, "ellip_min",datasetSummary->get_ellip_min(),true);

      kwl.add(prefix, "sc_lin",datasetSummary->get_sc_lin(),true);
      kwl.add(prefix, "sc_pix",datasetSummary->get_sc_pix(),true);

      kwl.add(prefix, "plat_lat",datasetSummary->get_plat_lat(),true);
      kwl.add(prefix, "plat_long",datasetSummary->get_plat_long(),true);
      kwl.add(prefix, "plat_head",datasetSummary->get_plat_head(),true);

      kwl.add(prefix, "wave_length",datasetSummary->get_wave_length(),true);

      kwl.add(prefix, "fr",datasetSummary->get_fr(),true);

      kwl.add(prefix, "rng_gate",datasetSummary->get_rng_gate(),true);

      kwl.add(prefix, "fa",datasetSummary->get_fa(),true);

      kwl.add(prefix, "n_azilok",datasetSummary->get_n_azilok(),true);
      kwl.add(prefix, "n_rnglok",datasetSummary->get_n_rnglok(),true);
      kwl.add(prefix, "bnd_azilok",datasetSummary->get_bnd_azilok(),true);
      kwl.add(prefix, "bnd_rnglok",datasetSummary->get_bnd_rnglok(),true);
      kwl.add(prefix, "bnd_azi",datasetSummary->get_bnd_azi(),true);
      kwl.add(prefix, "bnd_rng",datasetSummary->get_bnd_rng(),true);

      kwl.add(prefix, "alt_dopcen0",datasetSummary->get_alt_dopcen()[0],true);
      kwl.add(prefix, "alt_dopcen1",datasetSummary->get_alt_dopcen()[1],true);
      kwl.add(prefix, "alt_dopcen2",datasetSummary->get_alt_dopcen()[2],true);

      kwl.add(prefix, "crt_dopcen0",datasetSummary->get_crt_dopcen()[0],true);
      kwl.add(prefix, "crt_dopcen1",datasetSummary->get_crt_dopcen()[1],true);
      kwl.add(prefix, "crt_dopcen2",datasetSummary->get_crt_dopcen()[2],true);

      kwl.add(prefix, "time_dir_pix",datasetSummary->get_time_dir_pix().c_str(),true);
      kwl.add(prefix, "time_dir_lin",datasetSummary->get_time_dir_lin().c_str(),true);

      kwl.add(prefix, "terrain_height",datasetSummary->get_terrain_h(),true);

      kwl.add(prefix, "line_spacing",datasetSummary->get_line_spacing(),true);
    }
    else
    {
      ossimNotify(ossimNotifyLevel_DEBUG) << "It is not possible to add dataSetSummary from _leader or _trailer file" <<std::endl;
      return false;
    }

    ProcessingParameters * processingParameters = _leader->get_ProcessingParameters();
    if(processingParameters == NULL)
      {
      delete processingParameters;
      processingParameters = _trailer->get_ProcessingParameters();
      }

    if(processingParameters != NULL)
    {
      kwl.add(prefix, "n_srgr",processingParameters->get_n_srgr(),true);

      for (int i=0;i<processingParameters->get_n_srgr();i++)
      {
        sprintf(name,"srgr_update%i",i);
        kwl.add(prefix, name,((processingParameters->get_srgr_coefset()[i]).get_srgr_update()).c_str(),true);

        sprintf(name,"srgr_coef%iA",i);
        kwl.add(prefix, name,(processingParameters->get_srgr_coefset()[i]).get_srgr_coef()[0],true);
        sprintf(name,"srgr_coef%iB",i);
        kwl.add(prefix, name,(processingParameters->get_srgr_coefset()[i]).get_srgr_coef()[1],true);
        sprintf(name,"srgr_coef%iC",i);
        kwl.add(prefix, name,(processingParameters->get_srgr_coefset()[i]).get_srgr_coef()[2],true);
        sprintf(name,"srgr_coef%iD",i);
        kwl.add(prefix, name,(processingParameters->get_srgr_coefset()[i]).get_srgr_coef()[3],true);
        sprintf(name,"srgr_coef%iE",i);
        kwl.add(prefix, name,(processingParameters->get_srgr_coefset()[i]).get_srgr_coef()[4],true);
        sprintf(name,"srgr_coef%iF",i);
        kwl.add(prefix, name,(processingParameters->get_srgr_coefset()[i]).get_srgr_coef()[5],true);
      }

      kwl.add(prefix, "pixel_spacing",processingParameters->get_pixel_spacing(),true);

      kwl.add(prefix, "lookDirection",(processingParameters->get_sens_orient()).c_str(),true);
    }
    else
    {
      ossimNotify(ossimNotifyLevel_DEBUG) << "It is not possible to add processingParameters from _leader or _trailer file" <<std::endl;
      return false;
    }

    PlatformPositionData * platformPositionData = _leader->get_PlatformPositionData();
    if(platformPositionData != NULL)
    {
      kwl.add(prefix, "neph",platformPositionData->get_ndata(),true);
      kwl.add(prefix, "eph_int",platformPositionData->get_data_int(),true);

      kwl.add(prefix, "eph_year",platformPositionData->get_year(),true);
      kwl.add(prefix, "eph_month",platformPositionData->get_month(),true);
      kwl.add(prefix, "eph_day",platformPositionData->get_day(),true);
      kwl.add(prefix, "eph_sec",platformPositionData->get_gmt_sec(),true);
      kwl.add(prefix, "hr_angle",platformPositionData->get_hr_angle(),true);

      for(int i=0;i<platformPositionData->get_ndata();i++)
      {
        sprintf(name,"eph%i_posX",i);
        kwl.add(prefix, name,(platformPositionData->get_pos_vect()[i]).get_pos()[0],true);
        sprintf(name,"eph%i_posY",i);
        kwl.add(prefix, name,(platformPositionData->get_pos_vect()[i]).get_pos()[1],true);
        sprintf(name,"eph%i_posZ",i);
        kwl.add(prefix, name,(platformPositionData->get_pos_vect()[i]).get_pos()[2],true);

        sprintf(name,"eph%i_velX",i);
        kwl.add(prefix, name,(platformPositionData->get_pos_vect()[i]).get_vel()[0],true);
        sprintf(name,"eph%i_velY",i);
        kwl.add(prefix, name,(platformPositionData->get_pos_vect()[i]).get_vel()[1],true);
        sprintf(name,"eph%i_velZ",i);
        kwl.add(prefix, name,(platformPositionData->get_pos_vect()[i]).get_vel()[2],true);
      }
    }
    else
    {
      ossimNotify(ossimNotifyLevel_DEBUG) << "It is not possible to add platformPositionData from _leader file" <<std::endl;
      return false;
    }

    ImageOptionsFileDescriptor * imageOptionsFileDescriptor = _data->get_ImageOptionsFileDescriptor();
    if(imageOptionsFileDescriptor != NULL)
    {
      kwl.add(prefix, "nbLin",imageOptionsFileDescriptor->get_nlin(),true);
      kwl.add(prefix, "nbCol",imageOptionsFileDescriptor->get_ngrp(),true);
    }
    else
    {
      ossimNotify(ossimNotifyLevel_DEBUG) << "It is not possible to add imageOptionsFileDescriptor from _data file" <<std::endl;
      return false;
    }

    ProcessedDataRecord * firstProcessedDataRecord = _data->get_FirstProcessedDataRecord();
    if(firstProcessedDataRecord != NULL)
    {
      sprintf(name,"cornersLon%i",0);
      kwl.add(prefix, name,((float) (firstProcessedDataRecord->get_lon_first()))/1000000.0,true);
      sprintf(name,"cornersLat%i",0);
      kwl.add(prefix, name,((float) (firstProcessedDataRecord->get_lat_first()))/1000000.0,true);
      sprintf(name,"cornersLon%i",1);
      kwl.add(prefix, name,((float) (firstProcessedDataRecord->get_lon_last()))/1000000.0,true);
      sprintf(name,"cornersLat%i",1);
      kwl.add(prefix, name,((float) (firstProcessedDataRecord->get_lat_last()))/1000000.0,true);
      kwl.add(prefix, "acq_msec_first",firstProcessedDataRecord->get_acq_msec(),true);
    }
    else
    {
      ossimNotify(ossimNotifyLevel_DEBUG) << "It is not possible to add firstProcessedDataRecord from _data file" <<std::endl;
      return false;
    }

    ProcessedDataRecord * lastProcessedDataRecord = _data->get_LastProcessedDataRecord();
    if(lastProcessedDataRecord != NULL)
    {
      sprintf(name,"cornersLon%i",2);
      kwl.add(prefix, name,((float) (lastProcessedDataRecord->get_lon_first()))/1000000.0,true);
      sprintf(name,"cornersLat%i",2);
      kwl.add(prefix, name,((float) (lastProcessedDataRecord->get_lat_first()))/1000000.0,true);
      sprintf(name,"cornersLon%i",3);
      kwl.add(prefix, name,(((float) lastProcessedDataRecord->get_lon_last()))/1000000.0,true);
      sprintf(name,"cornersLat%i",3);
      kwl.add(prefix, name,((float) (lastProcessedDataRecord->get_lat_last()))/1000000.0,true);
      kwl.add(prefix, "acq_msec_last",lastProcessedDataRecord->get_acq_msec(),true);
    }
    else
    {
      ossimNotify(ossimNotifyLevel_DEBUG) << "It is not possible to add lastProcessedDataRecord from _data file" <<std::endl;
      return false;
    }

    return true;
}

bool ossimRadarSatModel::saveState(ossimKeywordlist& kwl,
                                                 const char* prefix) const
{
  bool result = this->internalSaveState(kwl, prefix);

  if (result)
  {
    result = ossimGeometricSarSensorModel::saveState(kwl, prefix);
  }

  return result;
}

bool ossimRadarSatModel::internalLoadState (const ossimKeywordlist &kwl,
                                            const char *prefix)
{
  static const char MODULE[] = "ossimRadarSatModel::internalLoadState";
  if (traceDebug())
  {
     ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
  }

  this->InitSRGR(kwl, prefix);
  this->InitSensorParams(kwl, prefix);
  this->InitPlatformPosition(kwl, prefix);
  this->InitRefPoint(kwl, prefix);

  if (traceDebug())
  {
     ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exit...\n";
  }
  return true;
}

bool ossimRadarSatModel::loadState (const ossimKeywordlist &kwl,
                                    const char *prefix)
{
  static const char MODULE[] = "ossimRadarSatModel::loadState";
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

  //bool result = ossimGeometricSarSensorModel::loadState(kwl, prefix);

  bool result = true;
  if (result)
    {
    result = this->internalLoadState(kwl, prefix);
    }

  if (traceDebug())
  {
     ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exit...\n";
  }
  return result;
}

bool ossimRadarSatModel::InitSensorParams(const ossimKeywordlist &kwl, const char *prefix)
{
  const char* wave_length_str = kwl.find(prefix,"wave_length");
  double wave_length = atof(wave_length_str);
  const char* fr_str = kwl.find(prefix,"fr");
  double fr = atof(fr_str);
  const char* fa_str = kwl.find(prefix,"fa");
  double fa = atof(fa_str);

  /* //number of different looks
  const char* n_azilok_str = kwl.find(prefix,"n_azilok");
  double n_azilok = atof(n_azilok_str);
  const char* n_rnglok_str = kwl.find(prefix,"n_rnglok");
  double n_rnglok = atof(n_rnglok_str);
  */

  // look bandwidth rate (for localisation purpose)
  const char* bnd_azilok_str = kwl.find(prefix,"bnd_azilok");
  double bnd_azilok = atof(bnd_azilok_str);
  const char* bnd_rnglok_str = kwl.find(prefix,"bnd_rnglok");
  double bnd_rnglok = atof(bnd_rnglok_str);
  const char* bnd_azi_str = kwl.find(prefix,"bnd_azi");
  double bnd_azi = atof(bnd_azi_str);
  const char* bnd_rng_str = kwl.find(prefix,"bnd_rng");
  double bnd_rng = atof(bnd_rng_str);

  double n_azilok = bnd_azi/bnd_azilok;
  double n_rnglok = bnd_rng/bnd_rnglok;

  const char* time_dir_pix = kwl.find(prefix,"time_dir_pix");
  const char* time_dir_lin = kwl.find(prefix,"time_dir_lin");

  //ellipsoid parameters
  const char* ellip_maj_str = kwl.find(prefix,"ellip_maj");
  double ellip_maj = atof(ellip_maj_str) * 1000.0;  // km -> m
  const char* ellip_min_str = kwl.find(prefix,"ellip_min");
  double ellip_min = atof(ellip_min_str) * 1000.0;  // km -> m

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

  const char* lookDirection_str = kwl.find(prefix,"lookDirection");
  ossimString lookDirection(lookDirection_str) ;
  lookDirection.trim(" ") ; // eliminates trailing blanks
  if (lookDirection == "NORMAL") _sensor->set_sightDirection(SensorParams::Right) ;
  else _sensor->set_sightDirection(SensorParams::Left) ;

  _sensor->set_prf(fa);
  _sensor->set_sf(fr);
  _sensor->set_rwl(wave_length);
  _sensor->set_nAzimuthLook(n_azilok);
  _sensor->set_nRangeLook(n_rnglok);

  _sensor->set_semiMajorAxis(ellip_maj) ;
  _sensor->set_semiMinorAxis(ellip_min) ;

  return true;
}

bool ossimRadarSatModel::InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix)
{
  const double _PI          = 3.14159265358979323846 ;
  CivilDateTime ref_civil_date;
  /*
   * Ephemerisis reference date retrieval
   */
  const char* eph_year_str = kwl.find(prefix,"eph_year");
  int eph_year = atoi(eph_year_str);
  const char* eph_month_str = kwl.find(prefix,"eph_month");
  int eph_month = atoi(eph_month_str);
  const char* eph_day_str = kwl.find(prefix,"eph_day");
  int eph_day = atoi(eph_day_str);
  const char* eph_sec_str = kwl.find(prefix,"eph_sec");
  double eph_sec = atof(eph_sec_str);

  ref_civil_date.set_year(eph_year);
  ref_civil_date.set_month(eph_month);
  ref_civil_date.set_day(eph_day);
  ref_civil_date.set_second((int)eph_sec);
  ref_civil_date.set_decimal( eph_sec-(double)((int)eph_sec));


  JSDDateTime ref_jsd_date(ref_civil_date);

  const char* eph_int_str = kwl.find(prefix, "eph_int");
  double eph_int = atof(eph_int_str);
  /*
   * Ephemerisis number retrieval
   */
  const char* neph_str = kwl.find(prefix,"neph");
  int neph = atoi(neph_str);

  Ephemeris** ephemeris = new Ephemeris*[neph];

  const char* hr_angle_str = kwl.find(prefix,"hr_angle");
  double greenwich_mha_ref = atof(hr_angle_str);

  GMSTDateTime * greenwich_mha_ref2000 = new GMSTDateTime();
  greenwich_mha_ref2000->set_origine(GMSTDateTime::AN2000);
  ref_civil_date.AsGMSTDateTime(greenwich_mha_ref2000);

  /*
   * Ephemerisis retrieval
   */
  for (int i=0;i<neph;i++)
  {
    double pos[3];
    double vit[3];
    char name[64];


    sprintf(name,"eph%i_posX",i);
    const char* px_str = kwl.find(prefix,name);
    pos[0] = atof(px_str);

    sprintf(name,"eph%i_posY",i);
    const char* py_str = kwl.find(prefix,name);
    pos[1] = atof(py_str);

    sprintf(name,"eph%i_posZ",i);
    const char* pz_str = kwl.find(prefix,name);
    pos[2] = atof(pz_str);


    sprintf(name,"eph%i_velX",i);
    const char* vx_str = kwl.find(prefix,name);
    vit[0] = atof(vx_str) * 1.0e-3;;

    sprintf(name,"eph%i_velY",i);
    const char* vy_str = kwl.find(prefix,name);
    vit[1] = atof(vy_str) * 1.0e-3;;

    sprintf(name,"eph%i_velZ",i);
    const char* vz_str = kwl.find(prefix,name);
    vit[2] = atof(vz_str) * 1.0e-3;;

    /*
     * Ephemerisis date
     */
    JSDDateTime date(ref_jsd_date);
    date.set_second(date.get_second() + i * eph_int);
    date.NormDate();

    /*
     * Date creation for referential change
     */
    GMSTDateTime * greenwich_mha = new GMSTDateTime();
    greenwich_mha->set_origine(GMSTDateTime::AN2000);
    date.AsGMSTDateTime(greenwich_mha);

    double angle = greenwich_mha_ref+(greenwich_mha->get_tms()-greenwich_mha_ref2000->get_tms())*180.0/_PI;
    angle = fmod(angle,360.0);

    /*
     * Referential change
     */
    GalileanEphemeris * tmpEphemeris = new GalileanEphemeris(date,pos,vit);
    GeographicEphemeris* eph = new GeographicEphemeris();

    tmpEphemeris->ToGeographic(angle,eph);
    ephemeris[i] = eph;

    delete tmpEphemeris;
    delete greenwich_mha;

  }

  delete greenwich_mha_ref2000;

  /*
   * Antenna position interpolator creation
   */
  if (_platformPosition != NULL)
  {
    delete _platformPosition;
  }
  _platformPosition = new PlatformPosition(ephemeris,neph);

  /*
   * Free of memory used by the ephemerisis list
   */
  for (int i=0;i<neph;i++)
  {
    delete ephemeris[i];
  }
  delete[] ephemeris;

  return true;
}

bool ossimRadarSatModel::InitRefPoint(const ossimKeywordlist &kwl, const char *prefix)
{
  const char* sc_lin_str = kwl.find(prefix,"sc_lin");
  double sc_lin = atof(sc_lin_str);

  const char* sc_pix_str = kwl.find(prefix,"sc_pix");
  double sc_pix = atof(sc_pix_str);

  const char* inp_sctim_str = kwl.find(prefix,"inp_sctim");

  const char* rng_gate_str = kwl.find(prefix,"rng_gate");
  double rng_gate = atof(rng_gate_str);

  if(_refPoint == NULL)
  {
    _refPoint = new RefPoint();
  }

  _refPoint->set_pix_col(sc_pix);
  _refPoint->set_pix_line(sc_lin);

  char year_str[5];
  for (int i=0;i<4;i++)
  {
    year_str[i] = inp_sctim_str[i];
  }
  year_str[4] = '\0';

  char month_str[3];
  for (int i=4;i<6;i++)
  {
    month_str[i-4] = inp_sctim_str[i];
  }
  month_str[2] = '\0';

  char day_str[3];
  for (int i=6;i<8;i++)
  {
    day_str[i-6] = inp_sctim_str[i];
  }
  day_str[2] = '\0';

  char hour_str[3];
  for (int i=8;i<10;i++)
  {
    hour_str[i-8] = inp_sctim_str[i];
  }
  hour_str[2] = '\0';

  char min_str[3];
  for (int i=10;i<12;i++)
  {
    min_str[i-10] = inp_sctim_str[i];
  }
  min_str[2] = '\0';

  char sec_str[3];
  for (int i=12;i<14;i++)
  {
    sec_str[i-12] = inp_sctim_str[i];
  }
  sec_str[2] = '\0';

  char mili_str[4];
  for (int i=14;i<17;i++)
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


  CivilDateTime date(year, month, day, hour * 3600 + min * 60 + sec, mili/1000.0);

  if(_platformPosition != NULL)
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

  double c = 2.99792458e+8;

  double distance = (rng_gate + ((double)sc_pix)*_sensor->get_nRangeLook()/_sensor->get_sf()) * (c/2.0);

  // in the case of Georeferenced images, the "relative" ground range is stored in place of the slant range
  // (used for SlantRange computation relative to reference point, necessary for optimization)
  // here, the pixelDirection is ignored since the RDS reference point is always at the scene centre
  if (_isProductGeoreferenced) {
    distance = _refPoint->get_pix_col() * _pixel_spacing ;
  }

  _refPoint->set_distance(distance);


//// essai : line spacing et speed satellite
//  const char* line_spacing_str = kwl.find(prefix,"line_spacing");
//  double line_spacing = atof(line_spacing_str);
//  Ephemeris * ephemeris = _refPoint->get_ephemeris() ;
//  double velSat = sqrt(pow(ephemeris->get_speed()[0], 2)+ pow(ephemeris->get_speed()[1], 2)+ pow(ephemeris->get_speed()[2], 2));
//  double prfeq = velSat / line_spacing ;
//  _sensor->set_prf(prfeq);
//  _sensor->set_nAzimuthLook(1.0);
///**
// * @todo : effacer
// */

  // in order to use ossimSensorModel::lineSampleToWorld
  const char* nbCol_str = kwl.find(prefix,"nbCol");
  const char* nbLin_str = kwl.find(prefix,"nbLin");

  theImageSize.x      = atoi(nbCol_str);
  theImageSize.y      = atoi(nbLin_str);
  theImageClipRect    = ossimDrect(0, 0, theImageSize.x-1, theImageSize.y-1);

  // sensor PRF update in the case of ground projected products
  if (_isProductGeoreferenced) {
    const char* acq_msec_first_str = kwl.find("acq_msec_first");
    double acq_msec_first = atof(acq_msec_first_str);
    const char* acq_msec_last_str = kwl.find("acq_msec_last");
    double acq_msec_last = atof(acq_msec_last_str);

    double actualPRF = 1000.0*theImageSize.y/(acq_msec_last-acq_msec_first) ;
    _sensor->set_nAzimuthLook(_sensor->get_prf()/actualPRF);
  }

  // Ground Control Points extracted from the model : scene center
  std::list<ossimGpt> groundGcpCoordinates ;
  std::list<ossimDpt> imageGcpCoordinates ;
  const char* lon_str = kwl.find("pro_long");
  double lon = atof(lon_str);
  const char* lat_str = kwl.find("pro_lat");
  double lat = atof(lat_str);
  //const char* height_str = kwl.find("terrain_height");
  double height = 0 ; //atof(height_str) ;

  ossimDpt imageGCP(sc_pix,sc_lin);
  ossimGpt groundGCP(lat, lon, height);
  groundGcpCoordinates.push_back(groundGCP) ;
  imageGcpCoordinates.push_back(imageGCP) ;

  // Ground Control Points extracted from the model : scene corners
  // upper left corner
  lon = atof(kwl.find("cornersLon0"));
  lat = atof(kwl.find("cornersLat0"));
  ossimDpt imageGCP0(0,0);
  ossimGpt groundGCP0(lat, lon, height);
  groundGcpCoordinates.push_back(groundGCP0) ;
  imageGcpCoordinates.push_back(imageGCP0) ;
  // upper right corner
  lon = atof(kwl.find("cornersLon1"));
  lat = atof(kwl.find("cornersLat1"));
  ossimDpt imageGCP1(theImageSize.x-1,0);
  ossimGpt groundGCP1(lat, lon, height);
  groundGcpCoordinates.push_back(groundGCP1) ;
  imageGcpCoordinates.push_back(imageGCP1) ;
  // lower left corner
  lon = atof(kwl.find("cornersLon2"));
  lat = atof(kwl.find("cornersLat2"));
  ossimDpt imageGCP2(0,theImageSize.y-1);
  ossimGpt groundGCP2(lat, lon, height);
  groundGcpCoordinates.push_back(groundGCP2) ;
  imageGcpCoordinates.push_back(imageGCP2) ;
  // lower right corner
  lon = atof(kwl.find("cornersLon3"));
  lat = atof(kwl.find("cornersLat3"));
  ossimDpt imageGCP3(theImageSize.x-1,theImageSize.y-1);
  ossimGpt groundGCP3(lat, lon, height);
  groundGcpCoordinates.push_back(groundGCP3) ;
  imageGcpCoordinates.push_back(imageGCP3) ;

  // Default optimization
  optimizeModel(groundGcpCoordinates, imageGcpCoordinates) ;

  return true;
}

bool ossimRadarSatModel::InitSRGR(const ossimKeywordlist &kwl, const char *prefix)
{
  // product type : format "RSAT-1-SAR-{SLC,SGG,SGC,...}-..."
  const char* file_name_str = kwl.find(prefix,"file_name");
  char format[4] ;
  // product type (SLC,SGG,SGC,...) extraction
  for (int j=11;j<14;j++)
  {
    format[j-11] = file_name_str[j];
  }
  format[3] = '\0';
  std::string format_str(format);

  _isProductGeoreferenced = (format_str=="SGX") || (format_str=="SGF") || (format_str=="SCW");

  // pixel spacing
  const char* pixel_spacing_str = kwl.find(prefix,"pixel_spacing");
  _pixel_spacing = atof(pixel_spacing_str);

  // number of SRGR sets
  const char* n_srgr_str = kwl.find(prefix,"n_srgr");
  _n_srgr = atoi(n_srgr_str);

  char name[64];
  for (int i=0;i<_n_srgr;i++)
  {
    // SRGR update time, format : YYYY-DDD-HH:MM:SS.SSS
    sprintf(name,"srgr_update%i",i);
    const char* srgr_update_str = kwl.find(prefix,name);

    // years and days are ignored in the following (conversion from CivilDate to JSDDate requires a YYYY-MM-DD format)
    // could be improved
    char hour_str[3];
    for (int j=9;j<11;j++)
    {
      hour_str[j-9] = srgr_update_str[j];
    }
    hour_str[2] = '\0';

    char min_str[3];
    for (int j=12;j<14;j++)
    {
      min_str[j-12] = srgr_update_str[j];
    }
    min_str[2] = '\0';

    char sec_str[3];
    for (int j=15;j<17;j++)
    {
      sec_str[j-15] = srgr_update_str[j];
    }
    sec_str[2] = '\0';

    char mili_str[4];
    for (int j=18;j<21;j++)
    {
      mili_str[j-18] = srgr_update_str[j];
    }
    mili_str[3] = '\0';

    int hour = atoi(hour_str);
    int min = atoi(min_str);
    int sec = atoi(sec_str);
    double mili = atof(mili_str);
    _srgr_update[i] = hour * 3600 + min * 60 + sec + mili/1000.0 ;

    // SRGR coefficients
    sprintf(name,"srgr_coef%iA",i);
    const char* srgr_coef_strA = kwl.find(prefix,name);
    _srgr_coefset[i][0] = atof(srgr_coef_strA);
    sprintf(name,"srgr_coef%iB",i);
    const char* srgr_coef_strB = kwl.find(prefix,name);
    _srgr_coefset[i][1] = atof(srgr_coef_strB);
    sprintf(name,"srgr_coef%iC",i);
    const char* srgr_coef_strC = kwl.find(prefix,name);
    _srgr_coefset[i][2] = atof(srgr_coef_strC);
    sprintf(name,"srgr_coef%iD",i);
    const char* srgr_coef_strD = kwl.find(prefix,name);
    _srgr_coefset[i][3] = atof(srgr_coef_strD);
    sprintf(name,"srgr_coef%iE",i);
    const char* srgr_coef_strE = kwl.find(prefix,name);
    _srgr_coefset[i][4] = atof(srgr_coef_strE);
    sprintf(name,"srgr_coef%iF",i);
    const char* srgr_coef_strF = kwl.find(prefix,name);
    _srgr_coefset[i][5] = atof(srgr_coef_strF);
  }
  return true;
}

int ossimRadarSatModel::FindSRGRSetNumber(JSDDateTime date) const
{
  if (_n_srgr==0) return(-1) ;

  double delays[20];
  for (int i=0;i<_n_srgr;i++)
  {
    delays[i] = fabs(date.get_second()+date.get_decimal()-_srgr_update[i]) ;
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
  return setNumber ;
}
}
