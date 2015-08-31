#include "ossimSentinel1Model.h"

namespace ossimplugins
{

// Define Trace flags for use within this file:
   static ossimTrace traceExec  ("ossimSentinel1Model:exec");
   static ossimTrace traceDebug ("ossimSentinel1Model:debug");

   RTTI_DEF1(ossimSentinel1Model, "ossimSentinel1Model", ossimSarModel);

//*************************************************************************************************
// Constructor
//*************************************************************************************************
   ossimSentinel1Model::ossimSentinel1Model()
      : ossimSarModel()
   {
      theManifestDoc = new ossimXmlDocument();
      theProduct = new ossimSentinel1ProductDoc();
      this->clearFields();

   }

    void ossimSentinel1Model::clearFields()
    {
       theOCN = false;
       theSLC = false;
       theManifestKwl.clear();
       theManifestFile = ossimFilename::NIL;
       theProductXmlFile = ossimFilename::NIL;
       theProduct->clearFields();

    }


//*************************************************************************************************
// Constructor
//*************************************************************************************************
   ossimSentinel1Model::ossimSentinel1Model(const ossimSentinel1Model& rhs)
      :ossimSarModel(rhs)
   {

   }

//*************************************************************************************************
// Destructor
//*************************************************************************************************
   ossimSentinel1Model::~ossimSentinel1Model()
   {
      theProduct = 0;
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG DESTRUCTOR: ~ossimSentinel1Model(): entering..." << std::endl;
   }

//*************************************************************************************************
// Infamous DUP
//*************************************************************************************************
   ossimObject* ossimSentinel1Model::dup() const
   {
      return new ossimSentinel1Model(*this);
   }

//*************************************************************************************************
// Print
//*************************************************************************************************
   std::ostream& ossimSentinel1Model::print(std::ostream& out) const
   {
      // Capture stream flags since we are going to mess with them.
      std::ios_base::fmtflags f = out.flags();

      out << "\nDump of ossimSentinel1Model at address " << (hex) << this
          << (dec)
          << "\n------------------------------------------------"
          << "\n  theImageID            = " << theImageID
          << "\n  theImageSize          = " << theImageSize

          << "\n------------------------------------------------"
          << "\n  " << endl;

      // Set the flags back.
      out.flags(f);
//      return ossimGeometricSarSensorModel::print(out);
      return ossimSensorModel::print(out);

   }

//*************************************************************************************************
// Save State
//*************************************************************************************************
   bool ossimSentinel1Model::saveState(ossimKeywordlist& kwl,
                                      const char* prefix) const
   {

      kwl.add(prefix,
              ossimKeywordNames::TYPE_KW,
              "ossimSentinel1Model",
              true);

      kwl.addList(theManifestKwl, true);

         if(theProduct.get())
         {
            kwl.addList(theProduct->getProductKwl(), true);
            //   theProduct->saveState(kwl, prefix);
         }
         ossimSarModel::saveState(kwl, prefix);
         return true;
   }


//*************************************************************************************************
// Load State
//*************************************************************************************************
   bool ossimSentinel1Model::loadState(const ossimKeywordlist& kwl,
                                      const char* prefix)
   {
      //theManifestKwl.addList(kwl, true);

      ossimSarModel::loadState(kwl, prefix);
      return true;
   }

   bool ossimSentinel1Model::findSafeManifest(const ossimFilename& file, ossimFilename& manifestFile)
   {
      manifestFile = ossimFilename(file.path().path() + "/manifest.safe");

      if(!manifestFile.exists())
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << "manifest.safe " << manifestFile << " doesn't exist ...\n";
         }
         return false;
      }
      return true;
   }

   bool ossimSentinel1Model::open(const ossimFilename& file)
   {

      static const char MODULE[] = "ossimplugins::ossimSentinel1Model::open";
      //traceDebug.setTraceFlag(true);

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
      }

      bool result = false;

      if ( !file.exists() || (file.ext().downcase() != "tiff") )
      {
         return false;
      }
      else
      {
         theGSD.makeNan();

         ossimFilename safeFile;
         bool foundManifestFile = findSafeManifest(file, safeFile);
         while (foundManifestFile)
         {
            if(theManifestDoc.get())
            {
               if( !theManifestDoc->openFile(safeFile))
               {
                  std::cerr << MODULE << "error at line:" << __LINE__ << std::endl;
                  break;
               }
            }
            else
            {
               ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " theManifestDoc..\n";
               break;
            }

            if ( !this->isSentinel1(safeFile))
            {
               std::cerr << MODULE << "error at line:" << __LINE__ << std::endl;
               break;
            }

            ossimString productFile;
            if ( !this->getAnnotationFileLocation(safeFile, "^product"))
            {
               std::cerr << MODULE << "error at line:" << __LINE__ << std::endl;
               break;
            }

            // Set the image ID to the scene ID.
            if ( !this->getImageId( theImageID ) )
            {
               std::cerr << MODULE << "error at line:" << __LINE__ << std::endl;
               break;
            }

            if ( !this->standAloneProductInformation( ) )
            {
               std::cerr << MODULE << "error at line:" << __LINE__ << std::endl;
               break;
            }

            // Set the sensor ID to the mission ID.
            if ( !this->initSensorID( theSensorID ) )
            {
               std::cerr << MODULE << "error at line:" << __LINE__ << std::endl;
               break;
            }

            if ( !this->readProduct( safeFile ) )
            {
               std::cerr << MODULE << "error at line:" << __LINE__ << std::endl;
               break;
            }

            if ( !this->initImageSize( theImageSize ) )
             {
                std::cerr << MODULE << "error at line:" << __LINE__ << std::endl;
                break;
             }

            theImageClipRect = ossimDrect( 0, 0, theImageSize.x-1, theImageSize.y-1 );
            theSubImageOffset.x = 0.0;
            theSubImageOffset.y = 0.0;

            if ( !this->initGsd( theGSD ) )
             {
                std::cerr << MODULE << "error at line:" << __LINE__ << std::endl;
                break;
             }

            theMeanGSD = (theGSD.x + theGSD.y)/2.0;

            if ( !this->initSRGR( ) )
            {
               std::cerr << MODULE << "error at line:" << __LINE__ << std::endl;
               break;
            }

            // if ( !theSafeManifest->initPlatformPosition( ) )
            // {
            //    break;
            // }
            // else
            // {
            //    std::cerr << MODULE << "error at line:" << __LINE__ << std::endl;
            // }


            return true;
         }

//    // If we broke out of the while, something happened...
         return false;
      }

   }


   bool ossimSentinel1Model::getImageId( ossimString& s) const
   {
      ossimString xpath;
      xpath = "/xfdu:XFDU/metadataSection/metadataObject/metadataWrap/xmlData/s1sarl1:standAloneProductInformation/s1sarl1:missionDataTakeID";
      return ossim::getPath(xpath, theManifestDoc.get(), s);
   }

   bool ossimSentinel1Model::initSensorID(ossimString& s)
   {
      const ossimRefPtr<ossimXmlNode> safePlatform = theManifestDoc->getRoot()->findFirstNode("metadataSection/metadataObject/metadataWrap/xmlData/safe:platform");
      ossimString familyName, instrumentId;
      bool ret1 = safePlatform->getChildTextValue(familyName, "safe:familyName");
      bool ret2 = safePlatform->getChildTextValue(instrumentId, "safe:number");

      theManifestKwl.add("support_data.",
                     "instrument",
                      "S1" + instrumentId,
                      true);

      s = familyName + instrumentId;
      return (ret1 && ret2);
   }

   bool ossimSentinel1Model::getAnnotationFileLocation(const ossimFilename &manifestFile, const char* pattern)
   {

      static const char MODULE[] = "ossimSentinel1SafeManifest::getAnnotationFileLocation";
      //traceDebug.setTraceFlag(true);
      const ossimString prefix = "support_data.";
      const ossimString xpath =  "/xfdu:XFDU/dataObjectSection/dataObject";

      vector<ossimRefPtr<ossimXmlNode> > xml_nodes;

      theManifestDoc->findNodes(xpath, xml_nodes);

      ossimRefPtr<ossimXmlNode> dataObjectNode = xml_nodes[0];
      ossimString dataObjectId;
      dataObjectNode->getAttributeValue(dataObjectId , "ID");

      if ( dataObjectId.match(pattern) )
      {
         ossim::getPath(xpath +  "/byteStream/fileLocation", theManifestDoc.get(), theProductXmlFile);
         xml_nodes.clear();
         theManifestDoc->findNodes(xpath +  "/byteStream/fileLocation", xml_nodes);
         if(xml_nodes.size() < 1 )
         {
            std::cerr << "error :" << __LINE__ << std::endl;
            return  false;
         }
         xml_nodes[0]->getAttributeValue(theProductXmlFile , "href");
         theProductXmlFile.setPath(manifestFile.path() + "/annotation");

      }
      return true;
   }


   bool ossimSentinel1Model::standAloneProductInformation()
   {
      static const char MODULE[] = "ossimSentinel1ProductDoc::parseSafe";

      const ossimString prefix = "support_data.";

      const ossimRefPtr<ossimXmlNode> safeProcessing = theManifestDoc->getRoot()->findFirstNode("metadataSection/metadataObject/metadataWrap/xmlData/safe:processing");
      const ossimRefPtr<ossimXmlNode> facility = safeProcessing->findFirstNode("safe:facility");
      const ossimRefPtr<ossimXmlNode> software = facility->findFirstNode("safe:software");
      const ossimString org = facility->getAttributeValue("organisation");
      const ossimString name = software->getAttributeValue("name");
      const ossimString version = software->getAttributeValue("version");

      theManifestKwl.add(prefix,
                      "Processing_system_identifier",
                      org + " " + name + " " + version,
                      true);

      theManifestKwl.add(prefix,
                      ossimKeywordNames::DATE_KW,
                      safeProcessing->getAttributeValue("start"),
                      true);

      const ossimRefPtr<ossimXmlNode> acquisitionPeriod = theManifestDoc->getRoot()->findFirstNode("metadataSection/metadataObject/metadataWrap/xmlData/safe:acquisitionPeriod");
      ossimString acqStartTime = acquisitionPeriod->getChildTextValue("safe:startTime");

      theManifestKwl.add(prefix,
                      "first_line_time",
                      acqStartTime,
                      true);

      theManifestKwl.add(prefix,
                      "last_line_time",
                      acquisitionPeriod->getChildTextValue("safe:stopTime"),
                      true);
      //RK
      theManifestKwl.add(prefix,
                      ossimKeywordNames::IMAGE_DATE_KW,
                      acqStartTime,
                      true);

      const ossimRefPtr<ossimXmlNode> instrumentNode =
         theManifestDoc->getRoot()->findFirstNode("metadataSection/metadataObject/metadataWrap/xmlData/safe:platform/safe:instrument");
      ossimString swath =  instrumentNode->getChildTextValue("s1sarl1:swath");
      ossimString acquisition_mode =  instrumentNode->getChildTextValue("s1sarl1:mode");

      if( acquisition_mode.empty())
      {
         ossimRefPtr<ossimXmlNode>  instrumentModeNode =  instrumentNode->findFirstNode("safe:extension/s1sarl1:instrumentMode");
         if(instrumentModeNode.get())
         {
            acquisition_mode = instrumentModeNode->getChildTextValue("s1sarl1:mode");
            swath = instrumentModeNode->getChildTextValue("s1sarl1:swath");
         }
      }

      theManifestKwl.add(prefix,
                      "acquisition_mode",
                      acquisition_mode,
                      true);

      theManifestKwl.add(prefix,
                      "swath",
                      swath,
                      true);

      if (acquisition_mode == "IW" || acquisition_mode == "EW")
         theProduct->setTOPSAR(true);

      const ossimRefPtr<ossimXmlNode> orbitReference =
         theManifestDoc->getRoot()->findFirstNode("metadataSection/metadataObject/metadataWrap/xmlData/safe:orbitReference");

      std::vector<ossimRefPtr<ossimXmlNode> > orbitNumberNodes;
      const ossimString orbitReference_xpath = "/xfdu:XFDU/metadataSection/metadataObject/metadataWrap/xmlData/safe:orbitReference";
         theManifestDoc->findNodes(orbitReference_xpath + "/safe:orbitNumber", orbitNumberNodes);

      std::vector<ossimRefPtr<ossimXmlNode> >::const_iterator it = orbitNumberNodes.begin();
      while( it != orbitNumberNodes.end())
      {
         const ossimRefPtr<ossimXmlAttribute> attribute =    (*it)->findAttribute("type");
         if( attribute.get() )
         {
            if( attribute->getValue() == "start" )
            {
            theManifestKwl.add(prefix, "abs_orbit", (*it)->getText(), true);
            break;
            }
         }
         ++it;
      }

      orbitNumberNodes.clear();
      theManifestDoc->findNodes(orbitReference_xpath + "/safe:relativeOrbitNumber", orbitNumberNodes);

      std::vector<ossimRefPtr<ossimXmlNode> >::const_iterator it2 = orbitNumberNodes.begin();
      while( it2 != orbitNumberNodes.end())
      {
         const ossimRefPtr<ossimXmlAttribute> attribute =    (*it2)->findAttribute("start");
         if( attribute.get() )
         {
            if( attribute->getValue() == "start" )
            {
               theManifestKwl.add(prefix, "rel_orbit", (*it2)->getText(), true);
               break;
            }
         }
         ++it2;
      }

      theManifestKwl.add(prefix, "orbit_cycle", orbitReference->getChildTextValue("safe:cycleNumber"), true);

      ossimString orbit_pass = "";
      orbit_pass = orbitReference->getChildTextValue("s1:pass");

      if( orbit_pass.empty() )
      {
         orbit_pass = orbitReference->getChildTextValue("safe:extension/s1:orbitProperties/s1:pass");
      }

      theManifestKwl.add(prefix,
                         "orbit_pass",
                         orbit_pass,
                         true);

      ossimString productType = "unknown";
      const ossimRefPtr<ossimXmlNode> standAloneProductInformation =
         theManifestDoc->getRoot()->findFirstNode("metadataSection/metadataObject/metadataWrap/xmlData/s1sarl1:standAloneProductInformation");

      if (theOCN)
      {
         productType = "OCN";
      }
      else
      {
         if (standAloneProductInformation.get())
            productType  = standAloneProductInformation->getChildTextValue("s1sarl1:productType");
      }

      theManifestKwl.add(prefix,
                      "product_type",
                      productType,
                      true);

      if( productType.contains("SLC" ) )
      {
         //ossimKeywordNames::PIXEL_TYPE_KW;  RK
         theManifestKwl.add("sample_type", "COMPLEX",  true);
         theProduct->setSLC(true);

      }
      else
      {
         theManifestKwl.add("sample_type", "DETECTED",  true);
         theManifestKwl.add(prefix, "srgr_flag", "true",  true);
      }

      return true;
   }

   bool ossimSentinel1Model::isSentinel1(const ossimFilename &manifestFile )
   {
      theOCN = isLevel2(manifestFile);
      if( isLevel1(manifestFile) || theOCN  || isLevel0(manifestFile))
         return true;
      else
         return false;
   }

   bool ossimSentinel1Model::isLevel1(const ossimFilename& file)
   {
      bool productXmlCheck = checkDirectory(file, "annotation", ".xml");

      return productXmlCheck && checkDirectory(file, "measurement", ".tiff");
   }

   bool ossimSentinel1Model::isLevel2(const ossimFilename& file)
   {

      return checkDirectory(file, "measurement",".nc");
   }

   bool ossimSentinel1Model::isLevel0(const ossimFilename& file)
   {
      return checkDirectory(file, "measurement",".dat");
   }

   bool ossimSentinel1Model::checkDirectory(const ossimFilename& file, const char* d, const char* ext)
   {
      //check dir is valid first
      ossimDirectory dir = ossimDirectory(file.path() + "/" + d + "/");
      std::vector<ossimFilename> result;
      dir.findAllFilesThatMatch(result, ext);
      if ( result.size() < 1 )
      {
         if (traceExec())
         {
            ossimNotify(ossimNotifyLevel_FATAL)
               << " DEBUG:" << " checkDirectory failed for: " << file.path()   << "/" << d << " with ext ="<< ext << std::endl;
         }
         return false;
      }

      return true;
   }

   bool ossimSentinel1Model::readProduct(const ossimFilename &manifestFile )
   {
      theProduct->setMetadataDirectory( manifestFile.path() );
      bool ret = theProduct->readProductMetadata( );
      if ( ret )
      {
         theProduct->readCalibrationMetadata();
         theProduct->readNoiseMetadata();
      }
      else
      {
         ossimNotify(ossimNotifyLevel_FATAL) << " theProduct->readProductMetadata() failed" << std::endl;
      }

      return true;
   }

} //end namespace
