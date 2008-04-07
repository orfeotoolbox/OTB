#include <ossim/imaging/ossimTerraSarTileSource.h>

#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/imaging/ossimImageDataFactory.h>

#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimXmlNode.h>

#include <iostream>
#include <fstream>
#include <ios>


RTTI_DEF1(ossimTerraSarTileSource, "ossimTerraSarTileSource", ossimImageHandler)

static ossimTrace traceDebug("ossimTerraSarTileSource:debug");

ossimTerraSarTileSource::ossimTerraSarTileSource():
	_annotation(NULL)
{
}
	
ossimTerraSarTileSource::~ossimTerraSarTileSource()
{
	if(_annotation != NULL)
	{
		delete _annotation;
	}
}

ossimString ossimTerraSarTileSource::getLongName()const
{
	return ossimString("TerraSar Image Handler");
}

ossimString ossimTerraSarTileSource::getShortName()const
{
	return ossimString("TerraSar Image Handler");
}
   
bool ossimTerraSarTileSource::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
	return false;
}
   
bool ossimTerraSarTileSource::loadState(const ossimKeywordlist& kwl,  const char* prefix)
{
	return false;
}

bool ossimTerraSarTileSource::open()
{
	if(traceDebug())
	{
		ossimNotify(ossimNotifyLevel_DEBUG)
			<< "ossimTerraSarTileSource::open(filename) DEBUG: entered..."
			<< std::endl
			<< "Attempting to open file " << theImageFile << std::endl;
	}

	bool retValue = true;

	/*
	 * Creation of the XML annotation file data storage class
	 */
	if(_annotation != NULL)
	{
		delete _annotation;
		_annotation = NULL;
	}

	_annotation = new TsxAnnotation();
				
	if (theImageFile.exists())
	{
		if(traceDebug())
		{
			ossimNotify(ossimNotifyLevel_DEBUG)
			<< "Begin reading Annotation file" << std::endl;
		}

		/*
		 * Checks whether the XML file corresponds to a Level1B TerraSar product
		 */
		ossimXmlDocument docXML(theImageFile) ; 
		ossimString xpathTest("/level1Product") ;
		std::vector<ossimRefPtr<ossimXmlNode> > listeResultat ; 
		docXML.findNodes(xpathTest, listeResultat) ; 
		if (listeResultat.size() != 1 ) 
		{
			if(traceDebug())
			{
				ossimNotify(ossimNotifyLevel_DEBUG)
				<< "ossimTerraSarTileSource::open() DEBUG: the file does not correspond to a level1B TerraSar XML annotation file" << std::endl;
			}
			retValue = false;
		}
		else
		{
			/*
			* Annotation file data reading
			*/
			if (! _annotation->Parse(docXML))
			{
				if(traceDebug())
				{
					ossimNotify(ossimNotifyLevel_DEBUG)
					<< "XML annotation file parsing error" << std::endl;
				}
				retValue = false;
			}
			else
			{
				if(traceDebug())
				{
					ossimNotify(ossimNotifyLevel_DEBUG)
					<< "End reading Annotation file" << std::endl;
				}
			}
		}
	}
	else
	{
		retValue = false;
	}
	

	if(traceDebug())
	{
		ossimNotify(ossimNotifyLevel_DEBUG)
			<< "ossimTerraSarTileSource::open() DEBUG: returning..." << std::endl;
	}
	return retValue;
}

void ossimTerraSarTileSource::close()
{
}

bool ossimTerraSarTileSource::isOpen()const
{
	return false;
}

bool ossimTerraSarTileSource::getImageGeometry(ossimKeywordlist& kwl,const char* prefix)
{
	kwl.add(prefix, ossimKeywordNames::TYPE_KW, "ossimTerraSarModel", true);

	/*
	 * Insertion of data related to the sensor model acquisition parameters into the keywordlist
	 */
	
	kwl.add(prefix, "imageDataStartWith",_annotation->get_imageDataStartWith().c_str(),true);
	kwl.add(prefix, "orbitDirection",_annotation->get_orbitDirection().c_str(),true);
	kwl.add(prefix, "lookDirection",_annotation->get_lookDirection().c_str(),true);
	kwl.add(prefix, "inp_sctim",_annotation->get_inp_sctim().c_str(),true);

	kwl.add(prefix, "pro_lat",_annotation->get_pro_lat(),true);
	kwl.add(prefix, "pro_long",_annotation->get_pro_long(),true);
	kwl.add(prefix, "pro_head",_annotation->get_pro_head(),true);

	kwl.add(prefix, "ellip_maj",_annotation->get_ellip_maj(),true);
	kwl.add(prefix, "ellip_min",_annotation->get_ellip_min(),true);

	kwl.add(prefix, "sc_lin",_annotation->get_sc_lin(),true);
	kwl.add(prefix, "sc_pix",_annotation->get_sc_pix(),true);

	kwl.add(prefix, "central_freq",_annotation->get_central_freq(),true);

	kwl.add(prefix, "fr",_annotation->get_fr(),true);

	kwl.add(prefix, "sc_rng",_annotation->get_sceneCenterRangeTime(),true);
	kwl.add(prefix, "start_rng",_annotation->get_rng_gate(),true);
	kwl.add(prefix, "end_rng",_annotation->get_rng_gate_end(),true);

	kwl.add(prefix, "azimuthStartTime",_annotation->get_azimuthStartTime().c_str(),true);
	kwl.add(prefix, "azimuthStopTime",_annotation->get_azimuthStopTime().c_str(),true);

	kwl.add(prefix, "fa",_annotation->get_fa(),true);

	kwl.add(prefix, "n_azilok",_annotation->get_n_azilok(),true);
	kwl.add(prefix, "n_rnglok",_annotation->get_n_rnglok(),true);
	
	kwl.add(prefix, "rangeProjectionType",(_annotation->get_rangeProjectionType()).c_str(),true);

	kwl.add(prefix, "SrToGr_R0",_annotation->get_SrToGr_R0(),true);

	kwl.add(prefix, "nbCol",_annotation->get_nbCol(),true);
	kwl.add(prefix, "nbLin",_annotation->get_nbLin(),true);
	kwl.add(prefix, "pixel_spacing_mean",_annotation->get_pixel_spacing_mean(),true);
	kwl.add(prefix, "pixel_spacing_near",_annotation->get_pixel_spacing_near(),true);
	kwl.add(prefix, "pixel_spacing_far",_annotation->get_pixel_spacing_far(),true);

	int nbCoeffs = (_annotation->get_SrToGr_coeffs()).size() ; 

	kwl.add(prefix, "SrToGr_coeffs_number",nbCoeffs,true);

	char name[64];
	for(int i=0;i<nbCoeffs;i++)
	{
		sprintf(name,"SrToGr_coeffs_%i",i);
		kwl.add(prefix, name,(_annotation->get_SrToGr_coeffs())[i],true);

		sprintf(name,"SrToGr_exps_%i",i);
		kwl.add(prefix, name,(_annotation->get_SrToGr_exps())[i],true);
	}

	kwl.add(prefix, "neph",_annotation->get_ndata(),true);

	for(int i=0;i<_annotation->get_ndata();i++)
	{
		sprintf(name,"eph%i_date",i);
		kwl.add(prefix, name,(_annotation->get_eph_date()[i]).c_str(),true);

		sprintf(name,"eph%i_posX",i);
		kwl.add(prefix, name,(_annotation->get_posX()[i]),true);
		sprintf(name,"eph%i_posY",i);
		kwl.add(prefix, name,(_annotation->get_posY()[i]),true);
		sprintf(name,"eph%i_posZ",i);
		kwl.add(prefix, name,(_annotation->get_posZ()[i]),true);

		sprintf(name,"eph%i_velX",i);
		kwl.add(prefix, name,(_annotation->get_velX()[i]),true);
		sprintf(name,"eph%i_velY",i);
		kwl.add(prefix, name,(_annotation->get_velY()[i]),true);
		sprintf(name,"eph%i_velZ",i);
		kwl.add(prefix, name,(_annotation->get_velZ()[i]),true);
	}

	for(int i=0;i<5;i++)
	{
		sprintf(name,"cornersLon%i",i);
		kwl.add(prefix, name,(_annotation->get_cornersLon())[i],true);
		sprintf(name,"cornersLat%i",i);
		kwl.add(prefix, name,(_annotation->get_cornersLat())[i],true);
		sprintf(name,"cornersCol%i",i);
		kwl.add(prefix, name,(_annotation->get_cornersCol())[i],true);
		sprintf(name,"cornersLin%i",i);
		kwl.add(prefix, name,(_annotation->get_cornersLin())[i],true);
	}

	kwl.add(prefix, "terrain_h",_annotation->get_terrain_h(),true);

	return true;
}