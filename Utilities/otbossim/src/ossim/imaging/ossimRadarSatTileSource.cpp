#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimRadarSatTileSource.h>
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

#include <ossim/imaging/RadarSat/RadarSatRecordHeader.h>

#include <ossim/imaging/RadarSat/Data/DataFactory.h>
#include <ossim/imaging/RadarSat/Data/ImageOptionsFileDescriptor.h>
#include <ossim/imaging/RadarSat/Leader/DataHistogramSignalData.h>
#include <ossim/imaging/RadarSat/Leader/DataQuality.h>
#include <ossim/imaging/RadarSat/Leader/DataSetSummary.h>
#include <ossim/imaging/RadarSat/Leader/FileDescriptor.h>
#include <ossim/imaging/RadarSat/Leader/DataHistogramProcessedData.h>
#include <ossim/imaging/RadarSat/Leader/ProcessingParameters.h>
#include <ossim/imaging/RadarSat/Leader/PlatformPositionData.h>
#include <ossim/imaging/RadarSat/Leader/AttitudeData.h>
#include <ossim/imaging/RadarSat/Leader/RadiometricData.h>
#include <ossim/imaging/RadarSat/Leader/RadiometricCompensationData.h>

#include <iostream>
#include <fstream>
#include <ios>


RTTI_DEF1(ossimRadarSatTileSource, "ossimRadarSatTileSource", ossimImageHandler)

static ossimTrace traceDebug("ossimRadarSatTileSource:debug");

ossimRadarSatTileSource::ossimRadarSatTileSource():
	_data(NULL),
	_leader(NULL)
{
}
	
ossimRadarSatTileSource::~ossimRadarSatTileSource()
{
	if (_data != NULL)
	{
		delete _data;
	}
	if(_leader != NULL)
	{
		delete _leader;
	}
}

ossimString ossimRadarSatTileSource::getLongName()const
{
	return ossimString("RadarSat Image Handler");
}

ossimString ossimRadarSatTileSource::getShortName()const
{
	return ossimString("RadarSat Image Handler");
}
   
bool ossimRadarSatTileSource::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
	return false;
}
   
bool ossimRadarSatTileSource::loadState(const ossimKeywordlist& kwl,  const char* prefix)
{
	return false;
}

bool ossimRadarSatTileSource::open()
{
	if(traceDebug())
	{
		ossimNotify(ossimNotifyLevel_DEBUG)
			<< "ossimRadarSatTileSource::open(filename) DEBUG: entered..."
			<< std::endl
			<< "Attempting to open file " << theImageFile << std::endl;
	}

	bool retValue = true;
	ossimFilename tempFilename = theImageFile;

	/*
	 * Creation de la classe permettant le stockage des metadonnées du Data file
	 */
	if (_data != NULL)
	{
		delete _data;
		_data = NULL;
	}
	_data = new Data();

	/*
	 * Creation de la classe peremttant le stockage des métadonnées du Leader file
	 */
	if(_leader != NULL)
	{
		delete _leader;
		_leader = NULL;
	}

	_leader = new Leader();

	RadarSatRecordHeader header;
	DataFactory factory;
	ifstream dataFile (tempFilename, ios::in|ios::binary);
	dataFile>>header;
	if(dataFile.eof())
	{
		dataFile.close();
		retValue =  false;
	}
	else
	{
		if(traceDebug())
		{
			ossimNotify(ossimNotifyLevel_DEBUG)
			<< "Begin reading DAT file" << std::endl;
		}
		RadarSatRecord* record = factory.Instanciate(header.get_rec_seq());
		if (record != NULL && header.get_rec_seq() == 1)
		{
			record->Read(dataFile);
			/*
			 * On test si on a bien un fichier DAT de RadarSat
			 */
			if ( (((ImageOptionsFileDescriptor*)record)->get_file_name()).substr(0,10) == "RSAT-1-SAR")
			{
				/*
				 * Nous somme bien dans un ficher Data d'une arborescence RadarSat, on lit le reste du fichier Data
				 */
				
				dataFile>>*_data;
				_data->InsertRecord(header.get_rec_seq(), record);
				dataFile.close();

				if(traceDebug())
				{
					ossimNotify(ossimNotifyLevel_DEBUG)
					<< "End reading DAT file" << std::endl;
				}
				/*
				 * On construit le chemin du Leader file
				 */
				ossimFilename leaderFilePath = tempFilename.setFile("LEA_01");
				if (leaderFilePath.exists())
				{
					if(traceDebug())
					{
						ossimNotify(ossimNotifyLevel_DEBUG)
						<< "Begin reading Leader file" << std::endl;
					}
					/*
					 * Lecture des données du leader file
					 */
					ifstream leaderFile (leaderFilePath, ios::in|ios::binary);
					leaderFile>>*_leader;
					leaderFile.close();
					if(traceDebug())
					{
						ossimNotify(ossimNotifyLevel_DEBUG)
						<< "End reading Leader file" << std::endl;
					}
				}
				else
				{
					retValue = false;
				}
				
			}
			else
			{
				dataFile.close();
				retValue = false;
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
			<< "ossimRadarSatTileSource::open() DEBUG: returning..." << std::endl;
	}
	return retValue;
}

void ossimRadarSatTileSource::close()
{
}

bool ossimRadarSatTileSource::isOpen()const
{
	return false;
}

bool ossimRadarSatTileSource::getImageGeometry(ossimKeywordlist& kwl,const char* prefix)
{
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

	/*
	 * Ajout des données nécessaires au modèle de capteur dans la liste des mots clefs
	 */
	DataSetSummary * datasetSummary = _leader->get_DataSetSummary();
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
	}
	else
	{
		return false;
	}

	/*
	 * Ajout des données nécessaires au modèle de capteur dans la liste des mots clefs
	 */
	ProcessingParameters * processingParameters = _leader->get_ProcessingParameters();
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
	}
	else
	{
		return false;
	}

	/*
	 * Ajout des données nécessaires au modèle de capteur dans la liste des mots clefs
	 */
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
		return false;
	}

	int t;
	t = 10;
	return true;
}