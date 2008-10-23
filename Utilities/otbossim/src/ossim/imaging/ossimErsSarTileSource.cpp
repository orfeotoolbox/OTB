#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimErsSarTileSource.h>
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

#include <ossim/imaging/ErsSar/ErsSarRecordHeader.h>

#include <ossim/imaging/ErsSar/ErsSarData/ErsSarDataFactory.h>
#include <ossim/imaging/ErsSar/ErsSarData/ErsSarImageOptionsFileDescriptor.h>
#include <ossim/imaging/ErsSar/ErsSarLeader/ErsSarPlatformPositionData.h>
#include <ossim/imaging/ErsSar/ErsSarLeader/ErsSarDataSetSummary.h>
#include <ossim/imaging/ErsSar/ErsSarLeader/ErsSarFileDescriptor.h>
#include <ossim/imaging/ErsSar/ErsSarLeader/ErsSarMapProjectionData.h>
#include <ossim/imaging/ErsSar/ErsSarLeader/ErsSarFacilityData.h>

#include <iostream>
#include <fstream>
#include <ios>


RTTI_DEF1(ossimErsSarTileSource, "ossimErsSarTileSource", ossimImageHandler)

static ossimTrace traceDebug("ossimErsSarTileSource:debug");

ossimErsSarTileSource::ossimErsSarTileSource():
	_ErsSardata(NULL),
	_ErsSarleader(NULL)
{
}
	
ossimErsSarTileSource::~ossimErsSarTileSource()
{
	if (_ErsSardata != NULL)
	{
		delete _ErsSardata;
	}
	if(_ErsSarleader != NULL)
	{
		delete _ErsSarleader;
	}
}

ossimString ossimErsSarTileSource::getLongName()const
{
	return ossimString("ErsSar Image Handler");
}

ossimString ossimErsSarTileSource::getShortName()const
{
	return ossimString("ErsSar Image Handler");
}
   
bool ossimErsSarTileSource::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
	return false;
}
   
bool ossimErsSarTileSource::loadState(const ossimKeywordlist& kwl,  const char* prefix)
{
	return false;
}

bool ossimErsSarTileSource::open()
{

	if(traceDebug())
	{
		ossimNotify(ossimNotifyLevel_DEBUG)
			<< "ossimErsSarTileSource::open(filename) DEBUG: entered..."
			<< std::endl
			<< "Attempting to open file " << theImageFile << std::endl;
	}

	bool retValue = true;
	ossimFilename tempFilename = theImageFile;

	/*
	 * Creation of the class allowing to store Data file metadata 
	 */
	if (_ErsSardata != NULL)
	{
		delete _ErsSardata;
		_ErsSardata = NULL;
	}
	_ErsSardata = new ErsSarData();

	/*
	 * Creation of the class allowing to store Leader file metadata
	 */
	if(_ErsSarleader != NULL)
	{
		delete _ErsSarleader;
		_ErsSarleader = NULL;
	}

	_ErsSarleader = new ErsSarLeader();

	ErsSarRecordHeader header;
	ErsSarDataFactory factory;
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
		ErsSarRecord* record = factory.Instanciate(header.get_rec_seq());
		if (record != NULL && header.get_rec_seq() == 1)
		{
			record->Read(dataFile);
			/*
			 * ErsSar DAT file verificationTest whether 
			 */
			std::string capteur = (((ErsSarImageOptionsFileDescriptor*)record)->get_file_name()).substr(0,3);
			if ( capteur == "ERS")
			{
				/*
				 * We are in a ErsSar Data file tree, we read the remaining of the Data file
				 */
				
				dataFile>>*_ErsSardata;
				_ErsSardata->InsertRecord(header.get_rec_seq(), record);
				dataFile.close();

				if(traceDebug())
				{
					ossimNotify(ossimNotifyLevel_DEBUG)
					<< "End reading DAT file" << std::endl;
				}
				/*
				 * Leader file path contruction from the DAT file path
				 */ 
				std::string leader_file = theImageFile;
				string::size_type loc = leader_file.find( "DAT_01", 0 );
				if( loc != string::npos ) leader_file.replace(loc, 6, "LEA_01" ); // upper case test
				else {
					loc = leader_file.find( "dat_01", 0 );
					if( loc != string::npos ) leader_file.replace(loc, 6, "lea_01" ); // lower case test
					else {
						ossimNotify(ossimNotifyLevel_DEBUG)
							<< "File Name not coherent (searching for *DAT_01* or *dat_01*)  : " << theImageFile << std::endl;
					}
				}
				ossimFilename leaderFilePath(leader_file);

				if (!leaderFilePath.exists()){
						ossimNotify(ossimNotifyLevel_DEBUG)
							<< "Leader file not found (searching for *lea_01* coherent with *dat_01*)  : " << theImageFile << std::endl;
						retValue = false;
				}
				else	{
					if(traceDebug())
					{
						ossimNotify(ossimNotifyLevel_DEBUG)
						<< "Begin reading Leader file" << std::endl;
					}
					/*
					 * Leader file data reading 
					 */
					ifstream leaderFile (leaderFilePath, ios::in|ios::binary);
					leaderFile>>*_ErsSarleader;
					leaderFile.close();
					if(traceDebug())
					{
						ossimNotify(ossimNotifyLevel_DEBUG)
						<< "End reading Leader file" << std::endl;
					}
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
			<< "ossimErsSarTileSource::open() DEBUG: returning..." << std::endl;
	}
	return retValue;
}

void ossimErsSarTileSource::close()
{
}

bool ossimErsSarTileSource::isOpen()const
{
	return false;
}

bool ossimErsSarTileSource::getImageGeometry(ossimKeywordlist& kwl,const char* prefix)
{
	char name[64];

	kwl.add(prefix, ossimKeywordNames::TYPE_KW, "ossimErsSarModel", true);
	/*
	 * Adding metadata necessary to the sensor model in the keywordlist
	 */
	ErsSarFileDescriptor * leaderfiledesc = _ErsSarleader->get_ErsSarFileDescriptor();
	if(leaderfiledesc != NULL)
	{
		kwl.add(prefix, "filename",leaderfiledesc->get_file_name().c_str(),true);
	}
	else
	{
		return false;
	}
	/*
	 * Adding metadata necessary to the sensor model in the keywordlist
	 */
	ErsSarDataSetSummary * datasetSummary = _ErsSarleader->get_ErsSarDataSetSummary();
	if(datasetSummary != NULL)
	{
		kwl.add(prefix, "inp_sctim",(datasetSummary->get_inp_sctim()).c_str(),true);
		kwl.add(prefix, "ellip_maj", datasetSummary->get_ellip_maj(),true); 
		kwl.add(prefix, "ellip_min", datasetSummary->get_ellip_min(),true); 
		kwl.add(prefix, "sc_lin", datasetSummary->get_sc_lin(),true); 
		kwl.add(prefix, "sc_pix", datasetSummary->get_sc_pix(),true); 
		kwl.add(prefix, "wave_length", datasetSummary->get_wave_length(),true); 
		kwl.add(prefix, "fr", datasetSummary->get_fr(),true); 
		kwl.add(prefix, "fa", datasetSummary->get_fa(),true); 
		kwl.add(prefix, "time_dir_pix", (datasetSummary->get_time_dir_pix()).c_str(),true);  
		kwl.add(prefix, "time_dir_lin", (datasetSummary->get_time_dir_lin()).c_str(),true);
		kwl.add(prefix, "line_spacing", datasetSummary->get_line_spacing(),true);
		kwl.add(prefix, "pix_spacing", datasetSummary->get_pix_spacing(),true);
		kwl.add(prefix, "nlooks_az", datasetSummary->get_n_azilok(),true);
		kwl.add(prefix, "n_rnglok", datasetSummary->get_n_rnglok(),true);
		kwl.add(prefix, "zero_dop_range_time_f_pixel", datasetSummary->get_zero_dop_range_time_f_pixel(),true);
		kwl.add(prefix, "zero_dop_range_time_c_pixel", datasetSummary->get_zero_dop_range_time_c_pixel(),true);
		kwl.add(prefix, "zero_dop_range_time_l_pixel", datasetSummary->get_zero_dop_range_time_l_pixel(),true);
	}
	else
	{
		return false;
	}
	ErsSarMapProjectionData * mapprojectiondata = _ErsSarleader->get_ErsSarMapProjectionData();
	if(mapprojectiondata != NULL)
	{
		kwl.add(prefix, "map_proj_des",(mapprojectiondata->get_map_proj_des()).c_str(),true);
		kwl.add(prefix, "num_lines",(double) mapprojectiondata->get_num_lines(),true);
		kwl.add(prefix, "num_pix",(double) mapprojectiondata->get_num_pix_in_line(),true);
		kwl.add(prefix, "first_line_first_pixel_lat",mapprojectiondata->get_first_line_first_pixel_lat(), true);
		kwl.add(prefix, "first_line_first_pixel_lon",mapprojectiondata->get_first_line_first_pixel_lon(), true);
		kwl.add(prefix, "first_line_last_pixel_lat",mapprojectiondata->get_first_line_last_pixel_lat(), true);
		kwl.add(prefix, "first_line_last_pixel_lon",mapprojectiondata->get_first_line_last_pixel_lon(), true);
		kwl.add(prefix, "last_line_first_pixel_lat",mapprojectiondata->get_last_line_first_pixel_lat(), true);
		kwl.add(prefix, "last_line_first_pixel_lon",mapprojectiondata->get_last_line_first_pixel_lon(), true);
		kwl.add(prefix, "last_line_last_pixel_lat",mapprojectiondata->get_last_line_last_pixel_lat(), true);
		kwl.add(prefix, "last_line_last_pixel_lon",mapprojectiondata->get_last_line_last_pixel_lon(), true);
	}
	else
	{
		return false;
	}
	ErsSarPlatformPositionData * platformposition = _ErsSarleader->get_ErsSarPlatformPositionData();
	if(mapprojectiondata != NULL)
	{
		kwl.add(prefix, "neph", platformposition->get_ndata(),true);
		kwl.add(prefix, "eph_year", platformposition->get_year(),true);
		kwl.add(prefix, "eph_month", platformposition->get_month(),true);
		kwl.add(prefix, "eph_day", platformposition->get_day(),true);
		kwl.add(prefix, "eph_gmt_day", platformposition->get_gmt_day(),true);
		kwl.add(prefix, "eph_sec", platformposition->get_gmt_sec(),true);
		kwl.add(prefix, "eph_hr_angle", platformposition->get_hr_angle(),true);
		kwl.add(prefix, "eph_int", platformposition->get_data_int(),true);

		for(int i=0;i<platformposition->get_ndata();i++)
		{
			sprintf(name,"eph%i_posX",i);
			kwl.add(prefix, name,(platformposition->get_pos_vect()[i]).get_pos()[0],true);
			sprintf(name,"eph%i_posY",i);
			kwl.add(prefix, name,(platformposition->get_pos_vect()[i]).get_pos()[1],true);
			sprintf(name,"eph%i_posZ",i);
			kwl.add(prefix, name,(platformposition->get_pos_vect()[i]).get_pos()[2],true);

			sprintf(name,"eph%i_velX",i);
			kwl.add(prefix, name,(platformposition->get_pos_vect()[i]).get_vel()[0],true);
			sprintf(name,"eph%i_velY",i);
			kwl.add(prefix, name,(platformposition->get_pos_vect()[i]).get_vel()[1],true);
			sprintf(name,"eph%i_velZ",i);
			kwl.add(prefix, name,(platformposition->get_pos_vect()[i]).get_vel()[2],true);
		}		
	}
	else
	{
		return false;
	}
	/*
	 * Adding metadata necessary to the sensor model in the keywordlist
	 */
	ErsSarFacilityData * facilitydata = _ErsSarleader->get_ErsSarFacilityData();
	if(facilitydata != NULL)
	{
		kwl.add(prefix, "coef_ground_range_1",facilitydata->get_coef_ground_range_1(),true);
		kwl.add(prefix, "coef_ground_range_2",facilitydata->get_coef_ground_range_2(),true);
		kwl.add(prefix, "coef_ground_range_3",facilitydata->get_coef_ground_range_3(),true);
		kwl.add(prefix, "coef_ground_range_4",facilitydata->get_coef_ground_range_4(),true);
	}
	else
	{
		return false;
	}
	return true;
}