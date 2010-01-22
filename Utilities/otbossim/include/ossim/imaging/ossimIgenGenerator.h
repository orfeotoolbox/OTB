//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimIgenGenerator.h 15833 2009-10-29 01:41:53Z eshirschorn $
#ifndef ossimIgenGenerator_HEADER
#define ossimIgenGenerator_HEADER
#include <stack>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGrect.h>
#include <ossim/base/ossimGeoPolygon.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/imaging/ossimImageFileWriter.h>

class OSSIMDLLEXPORT ossimIgenGenerator
{
public:
   ossimIgenGenerator(ossimConnectableObject* input=NULL,
                      ossimConnectableObject*  output=NULL,
                      ossimObject* view=NULL)
      :thePixelType(OSSIM_PIXEL_IS_POINT),
       theTilingFlag(false),
       theTileSpacing(7.5, 7.5),
       theTileSpacingUnits(OSSIM_MINUTES),
       theTilePadding(0,0),
       theTilePaddingUnits(OSSIM_PIXEL),
       theNamingConvention("DEFAULT"),
       theSpecFileLocation(""),
       theIgenExecutable("igen"),
       theSpecFileIndex(0),
       theSpecListIndex(0),
       theTileNamingConvention("id")
      {
         theUpperLeftTileStart.makeNan();
         setInput(input);
         setOutput(output);
         setView(view);
         theInputBoundingRect.makeNan();
      }
   
   ~ossimIgenGenerator()
      {
      }
   void setOutput(ossimConnectableObject* output)
      {
         theSpecList.clear();
         generateOutputKwl(output);
      }

   void setOutput(const ossimKeywordlist& kwl);

    void setInput(ossimConnectableObject* input);
   
   void setView(ossimObject* view)
      {
         generateViewKwl(view);
      }
   
   void generateSpecList(bool outputToFileFlag = false);
   void getImageFilenamesFromSpecList(std::vector<ossimFilename>& filenameList);
   ossimGrect getBoundingGround()const;
   
   void executeSpecList();

   void setIteratorsToStart()
      {
         theSpecFileIndex = 0;
         theSpecListIndex = 0;
      }
   bool nextSpecFile(ossimFilename& specFile)
      {
         if(theSpecFileIndex >= theSpecFileList.size())
         {
            return false;
         }
         specFile = theSpecFileList[theSpecFileIndex];
         ++theSpecFileIndex;
         return true;
      }
   void getSpecs(vector<ossimKeywordlist>& specList)const
      {
         specList = theSpecList;
      }
   
   ossim_uint32 getNumberOfSpecFiles()const
      {
         return (ossim_uint32)theSpecFileList.size();
      }

   ossimFilename getSpecFilename(ossim_uint32 specFileIndex = 0)const
      {
         if(specFileIndex < theSpecFileList.size())
         {
            return theSpecFileList[specFileIndex];
         }

         return ossimFilename("");
      }
   ossimFilename buildExe(ossim_uint32 i = 0)const
      {
         ossimFilename igenExe  = getIgenExecutable();
         ossimFilename specFile = getSpecFilename(i);

         return ossimFilename( igenExe + " " + specFile);
      }
   
   void getSpec(ossimKeywordlist& kwl,
                ossim_uint32 specIndex=0)
      {
         kwl.clear();

         if(specIndex < theSpecList.size())
         {
            kwl = theSpecList[specIndex];
         }
      }
   void getInput(ossimKeywordlist& kwl,
                 const char* prefix=NULL)const
      {
         kwl.add(prefix,
                 theInputKwl,
                 true);
      }
   void getOutput(ossimKeywordlist& kwl,
                  const char* prefix=NULL)const
      {
         kwl.add(prefix,
                 theOutputKwl,
                 true);
      }
   
   void getView(ossimKeywordlist& kwl,
                const char* prefix=NULL)const
      {
         kwl.add(prefix,
                 theViewKwl,
                 true);
      }

   void setView(ossimKeywordlist& kwl,
                const char* prefix = NULL)
      {
         theViewKwl.clear();
         theViewKwl.add(prefix,
                        kwl,
                        true);
      }
   void setOutput(ossimKeywordlist& kwl,
                  const char* prefix = NULL)
      {
         theOutputKwl.clear();
         theOutputKwl.add(prefix,
                          kwl,
                          true);
      }
   void setSpecFileLocation(const ossimFilename& filename)
      {
         theSpecFileLocation = filename;
      }
   const ossimFilename& getSpecFileLocation()const
      {
         return theSpecFileLocation;
      }
   void setIgenExecutable(const ossimFilename& igenExecutable)
      {
         theIgenExecutable = igenExecutable;
      }
   const ossimFilename& getIgenExecutable()const
      {
         return theIgenExecutable;
      }
   void setOutputPolygon(const ossimGeoPolygon& poly)
      {
         theOutputGeoPolygon = poly;
      }
   void setPixelType(ossimPixelType pixelType)
      {
         thePixelType = pixelType;
      }
   void getOutputGeoPolygon(ossimGeoPolygon& poly)const
      {
         poly = theOutputGeoPolygon;
      }
   const ossimGeoPolygon& getOutputGeoPolygon()const
      {
         return theOutputGeoPolygon;
      }
   void setTilingFlag(bool flag)
      {
         theTilingFlag = flag;
      }
   void setTileSpacing(const ossimDpt& spacing)
      {
         theTileSpacing = spacing;
      }
   void setTilePadding(const ossimDpt& padding)
      {
         theTilePadding = padding;
      }
   void setTileSpacingUnits(ossimUnitType unitType)
      {
         theTileSpacingUnits = unitType;
      }
   void setTilePaddingUnits(ossimUnitType unitType)
      {
         theTilePaddingUnits = unitType;
      }
   bool getTilingFlag()const
      {
         return theTilingFlag;
      }
   const ossimDpt& getTileSpacing()const
      {
         return theTileSpacing;
      }
   const ossimDpt& getTilePadding()const
      {
         return theTilePadding;
      }
   ossimUnitType getTileSpacingUnits()const
      {
         return theTileSpacingUnits;
      }
   ossimUnitType getTilePaddingUnits()const
      {
         return theTilePaddingUnits;
      }
   ossimPixelType getPixelType()const
      {
         return thePixelType;
      }
  /*!
   * Tile naming convention is a string that can be "id" or "usgs".
   * this is used if tiling is enabled.  If id is used it will use the filename
   * as the prefix and then add to it the id's.  If usgs is used it will name 
   * it based on the USGS name.
   */
  void setTileNamingConvention(const ossimString& namingConvention)
  {
    theTileNamingConvention = namingConvention;
    theTileNamingConvention = theTileNamingConvention.downcase();

  }
   const ossimString& getTileNamingConvention()const
      {
         return theTileNamingConvention;
      }
   ossimIrect getInputBoundingRect()const;
   
   ossimProjection* createProductProjection()const;
   ossimObject*     createOutput()const;
   ossimObject*     createInput()const;
   
protected:
   ossimKeywordlist  theInputKwl;
   ossimKeywordlist  theOutputKwl;
   ossimKeywordlist  theViewKwl;
   ossimPixelType    thePixelType;
   bool              theTilingFlag;
   ossimDpt          theUpperLeftTileStart;
   ossimDpt          theTileSpacing;
   ossimUnitType     theTileSpacingUnits;
   ossimDpt          theTilePadding;
   ossimUnitType     theTilePaddingUnits;
   ossimString       theNamingConvention;
   ossimFilename     theSpecFileLocation;
   ossimFilename     theIgenExecutable;
   ossim_uint32      theSpecFileIndex;
   ossim_uint32      theSpecListIndex;
   ossimIrect        theInputBoundingRect;
   ossimGeoPolygon   theOutputGeoPolygon;
   ossimString       theTileNamingConvention;
  
   vector<ossimKeywordlist> theSpecList;
   vector<ossimFilename>    theSpecFileList;

   void generateInputKwl(ossimConnectableObject* connectable);
   void generateOutputKwl(ossimConnectableObject* connectable);
   void generateViewKwl(ossimObject* view);
   void setDefaultOutput();
   void generateTiledSpecList(bool outputToFileFlag);
   void generateGeoTiledSpecList(const ossimDpt& spacing, bool outputToFileFlag);
   void generatePixelTiledSpecList(bool outputToFileFlag);
   void generateNoTiledSpecList(bool outputToFileFlag);

   void generateChainForMultiInput(ossimConnectableObject* connectable);
   void generateChainForMultiInputRecurse(std::stack<ossimConnectableObject*>& aStack,
                                          ossimConnectableObject* connectable);

   int getTileId(const ossimDrect& rect,
                 const ossimDpt& currentPt,
                 const ossimDpt& spacing)const;
   void addPadding(ossimGeoPolygon& output,
                   const ossimGeoPolygon& input)const;
   void addPixelType(ossimKeywordlist& kwl, const char* prefix="product.");
};

#endif
