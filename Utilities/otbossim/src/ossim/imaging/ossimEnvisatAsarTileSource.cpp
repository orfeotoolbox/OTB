#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimEnvisatAsarTileSource.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimAlbersProjection.h>
#include <ossim/projection/ossimCylEquAreaProjection.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimVanDerGrintenProjection.h>
#include <ossim/projection/ossimLambertConformalConicProjection.h>
#include <ossim/projection/ossimMercatorProjection.h>
#include <ossim/projection/ossimMillerProjection.h>
#include <ossim/projection/ossimOrthoGraphicProjection.h>
#include <ossim/projection/ossimPolyconicProjection.h>
#include <ossim/projection/ossimSinusoidalProjection.h>
#include <ossim/projection/ossimTransMercatorProjection.h>
#include <ossim/projection/ossimCassiniProjection.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimDatum.h>

#include <ossim/imaging/EnvisatAsar/EnvisatAsarDataFactory.h>
#include <ossim/imaging/EnvisatAsar/sph.h>
#include <ossim/imaging/EnvisatAsar/mph.h>
#include <ossim/imaging/EnvisatAsar/SQ_ADSR.h>
#include <ossim/imaging/EnvisatAsar/MainProcessingParameters.h>
#include <ossim/imaging/EnvisatAsar/SRGRConversionParameters.h>
#include <ossim/imaging/EnvisatAsar/DopplerCentroidParameters.h>
#include <ossim/imaging/EnvisatAsar/ChirpParameters.h>
#include <ossim/imaging/EnvisatAsar/AntennaElevationPatterns.h>
#include <ossim/imaging/EnvisatAsar/GeolocationGrid.h>

#include <iostream>
#include <fstream>
#include <ios>
#include <vector>

RTTI_DEF1(ossimEnvisatAsarTileSource, "ossimEnvisatAsarTileSource", ossimImageHandler)

static ossimTrace traceDebug("ossimEnvisatAsarTileSource:debug");

ossimEnvisatAsarTileSource::ossimEnvisatAsarTileSource():
	_EnvisatAsarData(NULL)
{
}
	
ossimEnvisatAsarTileSource::~ossimEnvisatAsarTileSource()
{
	if(_EnvisatAsarData != NULL)
	{
		delete _EnvisatAsarData;
	}
}

ossimString ossimEnvisatAsarTileSource::getLongName()const
{
	return ossimString("EnvisatAsar Image Handler");
}

ossimString ossimEnvisatAsarTileSource::getShortName()const
{
	return ossimString("EnvisatAsar Image Handler");
}
   
bool ossimEnvisatAsarTileSource::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
	return false;
}
   
bool ossimEnvisatAsarTileSource::loadState(const ossimKeywordlist& kwl,  const char* prefix)
{
	return false;
}

bool ossimEnvisatAsarTileSource::open()
{
	bool retValue = true;

	if(traceDebug())
	{
		ossimNotify(ossimNotifyLevel_DEBUG)
			<< "ossimEnvisatAsarTileSource::open(filename) DEBUG: entered..."
			<< std::endl
			<< "Attempting to open file " << theImageFile << std::endl;
	}

	/*
	 * Creation de la classe peremttant le stockage des métadonnées du EnvisatAsarData file
	 */
	if(_EnvisatAsarData != NULL)
	{
		delete _EnvisatAsarData;
		_EnvisatAsarData = NULL;
	}

	/*
	 * Ouverture et test du fichier
	 */
	ossimFilename Filename = theImageFile;
	ifstream dataFile (Filename, ios::in|ios::binary);
	if(dataFile.eof())
	{
		dataFile.close();
		retValue =  false;
	}
	else
	{
		mph mph_rec;
		dataFile>>mph_rec;

		// test if it is an ENVISAT Data file
		if (mph_rec.is_valid())
		{
			if(traceDebug())
			{
				ossimNotify(ossimNotifyLevel_DEBUG)
				<< "Begin reading EnvisatAsar file" << std::endl;
			}
			/*
			 * Lecture des données du EnvisatAsarData file
			 */
			dataFile.seekg(0);
			_EnvisatAsarData = new EnvisatAsarData();
			dataFile>>*_EnvisatAsarData;
			dataFile.close();
			if(traceDebug())
			{
				ossimNotify(ossimNotifyLevel_DEBUG)
				<< "End reading EnvisatAsar file" << std::endl;
			}
		}
		else
		{
			dataFile.close();
			retValue = false;
		}
	}

	if(traceDebug())
	{
		ossimNotify(ossimNotifyLevel_DEBUG)
			<< "ossimEnvisatAsarTileSource::open() DEBUG: returning..." << std::endl;
	}


	return retValue;
}

void ossimEnvisatAsarTileSource::close()
{
}

bool ossimEnvisatAsarTileSource::isOpen()const
{
	return false;
}


bool ossimEnvisatAsarTileSource::getImageGeometry(ossimKeywordlist& kwl,const char* prefix)
{
	char name[64];

	kwl.add(prefix, ossimKeywordNames::TYPE_KW, "ossimEnvisatAsarModel", true);

	/*
	 * Ajout des données nécessaires au modèle de capteur dans la liste des mots clefs
	 * Données issus du record SPH
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
	 * Ajout des données nécessaires au modèle de capteur dans la liste des mots clefs
	 * Données issus du record Main Processing Parameters
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
		kwl.add(prefix, "state_vector_time_1_day",static_cast<ossim_int32>(MPP_rec->get_state_vector_time_1_day()), true );
		kwl.add(prefix, "state_vector_time_1_sec", (double)MPP_rec->get_state_vector_time_1_sec(), true );
		kwl.add(prefix, "state_vector_time_1_microsec", (double)MPP_rec->get_state_vector_time_1_microsec(), true );
		kwl.add(prefix, "x_pos_1",static_cast<ossim_int32>(MPP_rec->get_x_pos_1()), true );
		kwl.add(prefix, "y_pos_1",static_cast<ossim_int32>(MPP_rec->get_y_pos_1()), true );
		kwl.add(prefix, "z_pos_1",static_cast<ossim_int32>(MPP_rec->get_z_pos_1()), true );
		kwl.add(prefix, "x_vel_1",static_cast<ossim_int32>(MPP_rec->get_x_vel_1()), true );
		kwl.add(prefix, "y_vel_1",static_cast<ossim_int32>(MPP_rec->get_y_vel_1()), true );
		kwl.add(prefix, "z_vel_1",static_cast<ossim_int32>(MPP_rec->get_z_vel_1()), true );
		kwl.add(prefix, "state_vector_time_2_day", static_cast<ossim_int32>(MPP_rec->get_state_vector_time_2_day()), true );
		kwl.add(prefix, "state_vector_time_2_sec", (double)MPP_rec->get_state_vector_time_2_sec(), true );
		kwl.add(prefix, "state_vector_time_2_microsec", (double)MPP_rec->get_state_vector_time_2_microsec(), true );
		kwl.add(prefix, "x_pos_2", static_cast<ossim_int32>(MPP_rec->get_x_pos_2()), true );
		kwl.add(prefix, "y_pos_2", static_cast<ossim_int32>(MPP_rec->get_y_pos_2()), true );
		kwl.add(prefix, "z_pos_2", static_cast<ossim_int32>(MPP_rec->get_z_pos_2()), true );
		kwl.add(prefix, "x_vel_2", static_cast<ossim_int32>(MPP_rec->get_x_vel_2()), true );
		kwl.add(prefix, "y_vel_2", static_cast<ossim_int32>(MPP_rec->get_y_vel_2()), true );
		kwl.add(prefix, "z_vel_2", static_cast<ossim_int32>(MPP_rec->get_z_vel_2()), true );
		kwl.add(prefix, "state_vector_time_3_day", static_cast<ossim_int32>(MPP_rec->get_state_vector_time_3_day()), true );
		kwl.add(prefix, "state_vector_time_3_sec", (double)MPP_rec->get_state_vector_time_3_sec(), true );
		kwl.add(prefix, "state_vector_time_3_microsec", (double)MPP_rec->get_state_vector_time_3_microsec(), true );
		kwl.add(prefix, "x_pos_3", static_cast<ossim_int32>(MPP_rec->get_x_pos_3()), true );
		kwl.add(prefix, "y_pos_3", static_cast<ossim_int32>(MPP_rec->get_y_pos_3()), true );
		kwl.add(prefix, "z_pos_3", static_cast<ossim_int32>(MPP_rec->get_z_pos_3()), true );
		kwl.add(prefix, "x_vel_3", static_cast<ossim_int32>(MPP_rec->get_x_vel_3()), true );
		kwl.add(prefix, "y_vel_3", static_cast<ossim_int32>(MPP_rec->get_y_vel_3()), true );
		kwl.add(prefix, "z_vel_3", static_cast<ossim_int32>(MPP_rec->get_z_vel_3()), true );
		kwl.add(prefix, "state_vector_time_4_day", static_cast<ossim_int32>(MPP_rec->get_state_vector_time_4_day()), true );
		kwl.add(prefix, "state_vector_time_4_sec", (double)MPP_rec->get_state_vector_time_4_sec(), true );
		kwl.add(prefix, "state_vector_time_4_microsec", (double)MPP_rec->get_state_vector_time_4_microsec(), true );
		kwl.add(prefix, "x_pos_4",static_cast<ossim_int32>(MPP_rec->get_x_pos_4()), true );
		kwl.add(prefix, "y_pos_4",static_cast<ossim_int32>(MPP_rec->get_y_pos_4()), true );
		kwl.add(prefix, "z_pos_4",static_cast<ossim_int32>(MPP_rec->get_z_pos_4()), true );
		kwl.add(prefix, "x_vel_4",static_cast<ossim_int32>(MPP_rec->get_x_vel_4()), true );
		kwl.add(prefix, "y_vel_4",static_cast<ossim_int32>(MPP_rec->get_y_vel_4()), true );
		kwl.add(prefix, "z_vel_4",static_cast<ossim_int32>(MPP_rec->get_z_vel_4()), true );
		kwl.add(prefix, "state_vector_time_5_day",static_cast<ossim_int32>( MPP_rec->get_state_vector_time_5_day()), true );
		kwl.add(prefix, "state_vector_time_5_sec", (double)MPP_rec->get_state_vector_time_5_sec(), true );
		kwl.add(prefix, "state_vector_time_5_microsec", (double)MPP_rec->get_state_vector_time_5_microsec(), true );
		kwl.add(prefix, "x_pos_5",static_cast<ossim_int32>(MPP_rec->get_x_pos_5()), true );
		kwl.add(prefix, "y_pos_5",static_cast<ossim_int32>(MPP_rec->get_y_pos_5()), true );
		kwl.add(prefix, "z_pos_5",static_cast<ossim_int32>(MPP_rec->get_z_pos_5()), true );
		kwl.add(prefix, "x_vel_5",static_cast<ossim_int32>(MPP_rec->get_x_vel_5()), true );
		kwl.add(prefix, "y_vel_5",static_cast<ossim_int32>(MPP_rec->get_y_vel_5()), true );
		kwl.add(prefix, "z_vel_5",static_cast<ossim_int32>(MPP_rec->get_z_vel_5()), true );
	} 
	else
	{
		return false;
	}
	/*
	 * Ajout des données nécessaires au modèle de capteur dans la liste des mots clefs
	 * Données issus du record Geolocation Grid - Point de reference
	 */
	GeolocationGrid* GG_rec = _EnvisatAsarData->get_GeolocationGrid(0);
	if(GG_rec != NULL)
	{
		kwl.add(prefix, "first_zero_doppler_time_day", static_cast<ossim_int32>(GG_rec->get_first_zero_doppler_time_day()), true ); 
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
	 * Ajout des données nécessaires au modèle de capteur dans la liste des mots clefs
	 * Données issus du record Geolocation Grid - Coins
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
	 * Ajout des données nécessaires au modèle de capteur dans la liste des mots clefs
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
