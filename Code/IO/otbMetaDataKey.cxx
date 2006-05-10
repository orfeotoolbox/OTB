/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   21 avril 2006
  Version   :   
  Role      :   Création d'une otb::MetaDataKey
  $Id$

=========================================================================*/


#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "otbMetaDataKey.h"

#define NBKEYS	15

namespace otb
{

const std::string MetaDataKey::m_DriverShortNameKey = "DriverShortName"; 
const std::string MetaDataKey::m_DriverLongNameKey = "DriverLongName"; 
  
const std::string MetaDataKey::m_ProjectionRefKey = "ProjectionRef";

const std::string MetaDataKey::m_GCPProjectionKey = "GCPProjection";
const std::string MetaDataKey::m_GCPParametersKey = "GCP_";
const std::string MetaDataKey::m_GCPCountKey = "GCPCount";

const std::string MetaDataKey::m_GeoTransformKey = "GeoTransform";

const std::string MetaDataKey::m_MetadataKey = "Metadata_"; 
const std::string MetaDataKey::m_SubMetadataKey = "SubMetadata_";
 
const std::string MetaDataKey::m_UpperLeftCornerKey = "UpperLeftCorner";
const std::string MetaDataKey::m_UpperRightCornerKey = "UpperRightCorner";
const std::string MetaDataKey::m_LowerLeftCornerKey = "LowerLeftCorner";
const std::string MetaDataKey::m_LowerRightCornerKey = "LowerRightCorner";

const std::string MetaDataKey::m_ColorTableNameKey = "ColorTableName";
const std::string MetaDataKey::m_ColorEntryCountKey = "ColorEntryCount";
const std::string MetaDataKey::m_ColorEntryAsRGBKey = "ColorEntryAsRGB";

const MetaDataKey::KeyTypeDef Types[] = 
  {
  	{MetaDataKey::m_DriverShortNameKey, 		MetaDataKey::TSTRING},
  	{MetaDataKey::m_DriverLongNameKey, 		MetaDataKey::TSTRING},
  	{MetaDataKey::m_ProjectionRefKey, 		MetaDataKey::TSTRING},
  	{MetaDataKey::m_GCPProjectionKey, 		MetaDataKey::TSTRING},
  	{MetaDataKey::m_GCPParametersKey ,	    	MetaDataKey::TOTB_GCP},
  	{MetaDataKey::m_GCPCountKey,	    		MetaDataKey::TENTIER},
  	{MetaDataKey::m_GeoTransformKey,	    	MetaDataKey::TSTRING},
  	{MetaDataKey::m_MetadataKey,	    		MetaDataKey::TSTRING},
  	{MetaDataKey::m_UpperLeftCornerKey,	    	MetaDataKey::TVECTOR},
  	{MetaDataKey::m_UpperRightCornerKey,    	MetaDataKey::TVECTOR},
	{MetaDataKey::m_LowerLeftCornerKey,    		MetaDataKey::TVECTOR},
	{MetaDataKey::m_LowerRightCornerKey,    	MetaDataKey::TVECTOR},
	{MetaDataKey::m_ColorTableNameKey,    		MetaDataKey::TSTRING},
	{MetaDataKey::m_ColorEntryCountKey,   		MetaDataKey::TENTIER},
	{MetaDataKey::m_ColorEntryAsRGBKey,    		MetaDataKey::TVECTOR}
	
	
  }; 
  
MetaDataKey::KeyType MetaDataKey::GetKeyType(std::string name)
{
   MetaDataKey::KeyType ktype;

   for ( unsigned int i=0; i<NBKEYS; i++)
      {
//      if ( strcmp(name.c_str(),Types[i].keyname.c_str()) == 0 ) 

        int x = name.find(Types[i].keyname);
        
        if ( (name.find(Types[i].keyname) >= 0 ) &&
             (name.find(Types[i].keyname) <= name.size() ) ) 
         {	
         ktype = Types[i].type;
         break;
         }
         
      }
   return(ktype);     	
}


} // end namespace otb
