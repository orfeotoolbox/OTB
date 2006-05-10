/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   21 avril 2006
  Version   :   
  Role      :   Création d'une classe contenant la définition des clefs
  		permettant d'accéder aux métadonnées du dictionnaire
  $Id$

=========================================================================*/
#ifndef __otbMetaDataKey_h
#define __otbMetaDataKey_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "gdal.h"
#include "itkDataObject.h"
#include <iostream>
#include <string>
#include <vector>

namespace otb
{
/** \class MetaDataKey
 *
 * \brief Creation of an otb::MetaDataKey class that defines the
 * key of metadata used in the metadata dictionary.
 *
 */
class /*ITK_EXPORT*/ MetaDataKey
{
public:

  typedef MetaDataKey Self;
  
  MetaDataKey(){};
  virtual ~MetaDataKey(){};
 
  static const std::string m_DriverShortNameKey; 
  static const std::string m_DriverLongNameKey; 
      
  static const std::string m_ProjectionRefKey;
  
  static const std::string m_GCPProjectionKey;
  static const std::string m_GCPParametersKey; 
  static const std::string m_GCPCountKey;
  
  static const std::string m_GeoTransformKey;
  
  static const std::string m_MetadataKey; 
  static const std::string m_SubMetadataKey; 
  
  static const std::string m_UpperLeftCornerKey;
  static const std::string m_UpperRightCornerKey;
  static const std::string m_LowerLeftCornerKey;
  static const std::string m_LowerRightCornerKey;
  
  static const std::string m_ColorTableNameKey;
  static const std::string m_ColorEntryCountKey;
  static const std::string m_ColorEntryAsRGBKey;

  enum  KeyType {
  	TSTRING,
  	TENTIER,
	TDOUBLE,
	TOTB_GCP,
	TVECTOR
  };
  
  typedef struct 
  {
  	std::string keyname;
  	KeyType type; 
  } KeyTypeDef ;
  
  KeyType GetKeyType(std::string name);

  typedef std::vector<double> VectorType;

private:
  MetaDataKey(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  

  
}; 

/** \class OTB_GCP
 *
 * \brief This OTB_GCP class is used to manege the GCP parameters
 * in OTB.
 *
 */
class ITK_EXPORT OTB_GCP
{
public:


    /** Unique identifier, often numeric */
    std::string m_Id; 

    /** Informational message or "" */
    std::string m_Info;

    /** Pixel (x) location of GCP on raster */
    double      m_GCPCol;
    
    /** Line (y) location of GCP on raster */
    double      m_GCPRow;

    /** X position of GCP in georeferenced space */
    double      m_GCPX;

    /** Y position of GCP in georeferenced space */
    double      m_GCPY;

    /** Elevation of GCP, or zero if not known */
    double      m_GCPZ;


    OTB_GCP(){};
    OTB_GCP( const GDAL_GCP * psGCP )
    {
    	m_Id = std::string(psGCP->pszId);
    	m_Info = std::string(psGCP->pszInfo);
    	m_GCPRow = psGCP->dfGCPLine;
    	m_GCPCol = psGCP->dfGCPPixel;
    	m_GCPX = psGCP->dfGCPX;
    	m_GCPY = psGCP->dfGCPY;
    	m_GCPZ = psGCP->dfGCPZ;
    }
    virtual ~OTB_GCP(){};

  OTB_GCP(const OTB_GCP& pGcp)
  { 
    m_Id = pGcp.m_Id; 
    m_Info = pGcp.m_Info; 
    m_GCPCol = pGcp.m_GCPCol; 
    m_GCPRow = pGcp.m_GCPRow; 
    m_GCPX = pGcp.m_GCPX; 
    m_GCPY = pGcp.m_GCPY; 
    m_GCPZ = pGcp.m_GCPZ; 
  }
  
  void operator=(const OTB_GCP& pGcp) 
  { 
    m_Id = pGcp.m_Id; 
    m_Info = pGcp.m_Info; 
    m_GCPCol = pGcp.m_GCPCol; 
    m_GCPRow = pGcp.m_GCPRow; 
    m_GCPX = pGcp.m_GCPX; 
    m_GCPY = pGcp.m_GCPY; 
    m_GCPZ = pGcp.m_GCPZ; 
  }

  void Print(std::ostream& os)const
  {
   os << " 	GCP Id = " << this->m_Id << std::endl;
   os << " 	GCP Info =  " << this->m_Info << std::endl;  
   os << " 	GCP (Row,Col) = (" << this->m_GCPRow << "," << this->m_GCPCol << ")" << std::endl;
   os << " 	GCP (X,Y,Z) = (" << this->m_GCPX << "," << this->m_GCPY << "," << this->m_GCPZ << ")" << std::endl;
  }

};
  
} // end namespace otb


#endif
