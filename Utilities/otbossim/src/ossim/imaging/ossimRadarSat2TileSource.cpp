#include <ossim/imaging/ossimRadarSat2TileSource.h>

#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimDirectory.h>
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


RTTI_DEF1(ossimRadarSat2TileSource, "ossimRadarSat2TileSource", ossimImageHandler)

static ossimTrace traceDebug("ossimRadarSat2TileSource:debug");

ossimRadarSat2TileSource::ossimRadarSat2TileSource():
	_annotation(NULL)
{
}
	
ossimRadarSat2TileSource::~ossimRadarSat2TileSource()
{
	if(_annotation != NULL)
	{
		delete _annotation;
	}
}

ossimString ossimRadarSat2TileSource::getLongName()const
{
	return ossimString("RadarSat2 Image Handler");
}

ossimString ossimRadarSat2TileSource::getShortName()const
{
	return ossimString("RadarSat2 Image Handler");
}
   
bool ossimRadarSat2TileSource::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
	return false;
}
   
bool ossimRadarSat2TileSource::loadState(const ossimKeywordlist& kwl,  const char* prefix)
{
	return false;
}

bool ossimRadarSat2TileSource::open()
{
	if(traceDebug())
	{
		ossimNotify(ossimNotifyLevel_DEBUG)
			<< "ossimRadarSat2TileSource::open(filename) DEBUG: entered..."
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

	_annotation = new Rds2Annotation();
	
	/*
	 * Uses XML annotation file corresponding to the input "image file"
	 */
	ossimFilename annotationFilename = theImageFile;
	if ((annotationFilename.ext()).downcase() != "xml") {
		// assigned to a "product.xml" file in the same directory
		annotationFilename = annotationFilename.path();
		annotationFilename +=  annotationFilename.thePathSeparator ; 
		annotationFilename +=  "product.xml" ; 
	}
				
	if (annotationFilename.exists())
	{
		if(traceDebug())
		{
			ossimNotify(ossimNotifyLevel_DEBUG)
			<< "Begin reading Annotation file" << std::endl;
		}

		/*
		 * Checks whether the XML file corresponds to a RadarSat2 product
		 */
		ossimXmlDocument docXML(annotationFilename) ; 
		ossimString xpathTest("/product/sourceAttributes/satellite") ;
		// note : the satellite name could be tested ("RADARSAT-1, RADARSAT-2)
		std::vector<ossimRefPtr<ossimXmlNode> > listeResultat ; 
		docXML.findNodes(xpathTest, listeResultat) ; 
		if (listeResultat.size() != 1 ) 
		{
			if(traceDebug())
			{
				ossimNotify(ossimNotifyLevel_DEBUG)
				<< "ossimRadarSat2TileSource::open() DEBUG: the file does not correspond to a RadarSat2 XML annotation file" << std::endl;
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
			<< "ossimRadarSat2TileSource::open() DEBUG: returning..." << std::endl;
	}
	return retValue;
}

void ossimRadarSat2TileSource::close()
{
}

bool ossimRadarSat2TileSource::isOpen()const
{
	return false;
}

bool ossimRadarSat2TileSource::getImageGeometry(ossimKeywordlist& kwl,const char* prefix)
{
	kwl.add(prefix, ossimKeywordNames::TYPE_KW, "ossimRadarSat2Model", true);

	/*
	 * Insertion of data related to the sensor model acquisition parameters into the keywordlist
	 */
	
	kwl.add(prefix, "lineTimeOrdering",_annotation->get_lineTimeOrdering().c_str(),true);
	kwl.add(prefix, "pixelTimeOrdering",_annotation->get_pixelTimeOrdering().c_str(),true);
	kwl.add(prefix, "orbitDirection",_annotation->get_orbitDirection().c_str(),true);
	kwl.add(prefix, "lookDirection",_annotation->get_lookDirection().c_str(),true);

	kwl.add(prefix, "ellip_maj",_annotation->get_ellip_maj(),true);
	kwl.add(prefix, "ellip_min",_annotation->get_ellip_min(),true);

	kwl.add(prefix, "central_freq",_annotation->get_central_freq(),true);

	kwl.add(prefix, "fr",_annotation->get_fr(),true);

	kwl.add(prefix, "fa",_annotation->get_fa(),true);

	kwl.add(prefix, "n_azilok",_annotation->get_n_azilok(),true);
	kwl.add(prefix, "n_rnglok",_annotation->get_n_rnglok(),true);

	kwl.add(prefix, "zeroDopplerTimeFirstLine",_annotation->get_zeroDopplerTimeFirstLine().c_str(),true);
	kwl.add(prefix, "slantRangeNearEdge",_annotation->get_slantRangeNearEdge(),true);
	
	kwl.add(prefix, "productType",(_annotation->get_productType()).c_str(),true);

	kwl.add(prefix, "nbCol",_annotation->get_nbCol(),true);
	kwl.add(prefix, "nbLin",_annotation->get_nbLin(),true);
	kwl.add(prefix, "pixel_spacing_mean",_annotation->get_pixel_spacing_mean(),true);

	int nbCoeffs = (_annotation->get_SrGr_update()).size() ; 

	kwl.add(prefix, "SrGr_coeffs_number",nbCoeffs,true);

	char name[64];
	for(int i=0;i<nbCoeffs;i++)
	{
		for(int j=0;j<((_annotation->get_SrGr_coeffs())[i]).size();j++)
		{
			sprintf(name,"SrGr_coeffs_%i_%i",i,j);
			kwl.add(prefix, name,(_annotation->get_SrGr_coeffs())[i][j],true);
		}
		sprintf(name,"SrGr_R0_%i",i);
		kwl.add(prefix,name,_annotation->get_SrGr_R0()[i],true);
		sprintf(name,"SrGr_update_%i",i);
		kwl.add(prefix,name,(_annotation->get_SrGr_update()[i]).c_str(),true);
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

	int cornersLonSize = (_annotation->get_cornersLon()).size() ;
	kwl.add(prefix, "nTiePoints",cornersLonSize ,true);

	for(int i=0;i<_annotation->get_cornersLon().size();i++)
	{
		sprintf(name,"cornersLon%i",i);
		kwl.add(prefix, name,(_annotation->get_cornersLon())[i],true);
		sprintf(name,"cornersLat%i",i);
		kwl.add(prefix, name,(_annotation->get_cornersLat())[i],true);
		sprintf(name,"cornersHeight%i",i);
		kwl.add(prefix, name,(_annotation->get_cornersHeight())[i],true);
		sprintf(name,"cornersCol%i",i);
		kwl.add(prefix, name,(_annotation->get_cornersCol())[i],true);
		sprintf(name,"cornersLin%i",i);
		kwl.add(prefix, name,(_annotation->get_cornersLin())[i],true);
	}

	kwl.add(prefix, "terrain_h",_annotation->get_terrain_h(),true);

	return true;
}