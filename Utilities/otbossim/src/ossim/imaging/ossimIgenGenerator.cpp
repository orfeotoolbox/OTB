//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimIgenGenerator.cpp 20206 2011-11-04 15:16:31Z dburken $
#include <ossim/imaging/ossimIgenGenerator.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimGeoPolyCutter.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimUsgsQuad.h>

void ossimIgenGenerator::generateSpecList(bool outputToFileFlag)
{
   if(theTilingFlag&&
      (theTileSpacingUnits!=OSSIM_UNIT_UNKNOWN)&&
      (!theTileSpacing.hasNans()))
   {
      generateTiledSpecList(outputToFileFlag);
   }
   else
   {
      generateNoTiledSpecList(outputToFileFlag);
   }
   
}

void ossimIgenGenerator::getImageFilenamesFromSpecList(std::vector<ossimFilename>& filenameList)
{
   if(!theSpecList.size())
   {
      generateSpecList();
   }
   if(theSpecList.size())
   {
      filenameList.clear();
      for(ossim_uint32 i = 0; i < theSpecList.size(); ++i)
      {
         const char* filename = theSpecList[i].find("object2.",  ossimKeywordNames::FILENAME_KW);

         if(filename)
         {
            filenameList.push_back(ossimFilename(filename));
         }
         else
         {
            filenameList.clear();
            return;
         }
      }
   }
   
}

ossimGrect ossimIgenGenerator::getBoundingGround()const
{
   return ossimGrect(theOutputGeoPolygon[0],
                     theOutputGeoPolygon[1],
                     theOutputGeoPolygon[2],
                     theOutputGeoPolygon[3]);
}

void ossimIgenGenerator::executeSpecList()
{
}

void ossimIgenGenerator::setInput(ossimConnectableObject* input)
{
   theSpecList.clear();
   generateInputKwl(input);
   ossimImageSource* inputInterface = PTR_CAST(ossimImageSource,
                                                        input);
   if(inputInterface)
   {
      theInputBoundingRect = inputInterface->getBoundingRect();
   }
}


void ossimIgenGenerator::generateChainForMultiInput(ossimConnectableObject* connectable)
{
   std::stack<ossimConnectableObject*> aStack;

   generateChainForMultiInputRecurse(aStack, connectable);

   ossim_int32 objectIndex = 1;
   while(!aStack.empty())
   {
      ossimConnectableObject* current = aStack.top();
      aStack.pop();
      ossimString prefix = ossimString("object") + ossimString::toString(objectIndex) + ".";

      current->saveState(theInputKwl, prefix.c_str());

      ++objectIndex;
   }
   theInputKwl.add("type",
                   "ossimImageChain",
                   true);
   
}

void ossimIgenGenerator::generateChainForMultiInputRecurse(std::stack<ossimConnectableObject*>& aStack,
                                                           ossimConnectableObject* connectable)
{
   

   if(connectable)
   {
      aStack.push(connectable);
      for(ossim_uint32 i = 0; i < connectable->getNumberOfInputs(); ++i)
      {
         generateChainForMultiInputRecurse(aStack,
                                           connectable->getInput(i));
      }
   }   
}


ossimProjection* ossimIgenGenerator::createProductProjection()const
{
   return ossimProjectionFactoryRegistry::instance()->createProjection(theViewKwl);
}

ossimObject* ossimIgenGenerator::createOutput()const
{
   return ossimObjectFactoryRegistry::instance()->createObject(theOutputKwl);
}

ossimObject* ossimIgenGenerator::createInput()const
{
   return ossimObjectFactoryRegistry::instance()->createObject(theInputKwl);
}

void ossimIgenGenerator::generateInputKwl(ossimConnectableObject* connectable)
{
   theInputKwl.clear();
   if(connectable)
   {
      // if it has at least one non null input
      // then  we must build a chain and save it out
      if(connectable->getInput())
      {
         generateChainForMultiInput(connectable);
      }
      else // it is a single input
      {
         connectable->saveState(theInputKwl);
      }
   }
}

void ossimIgenGenerator::generateOutputKwl(ossimConnectableObject* connectable)
{
   theOutputKwl.clear();
   if(connectable)
   {
      connectable->saveState(theOutputKwl);
   }
}

void ossimIgenGenerator::generateViewKwl(ossimObject* view)
{
   theViewKwl.clear();
   if(view)
   {
      view->saveState(theViewKwl);
   }
}
   
void ossimIgenGenerator::setDefaultOutput()
{
   
}

void ossimIgenGenerator::generateTiledSpecList(bool outputToFileFlag)
{
   theSpecList.clear();
   theSpecFileList.clear();
   ossimDpt spacing = theTileSpacing;

   switch(theTileSpacingUnits)
   {
      case OSSIM_MINUTES:
      {
         spacing.x /= 60;
         spacing.y /= 60;
         generateGeoTiledSpecList(spacing, outputToFileFlag);
         break;
      }
      case OSSIM_SECONDS:
      {
         spacing.x /= 3600;
         spacing.y /= 3600;
         generateGeoTiledSpecList(spacing, outputToFileFlag);
         break;
      }
      case OSSIM_DEGREES:
      {
         generateGeoTiledSpecList(spacing, outputToFileFlag);
         break;
      }
      case OSSIM_PIXEL:
      {
         generatePixelTiledSpecList(outputToFileFlag);
         break;
      }
      default:
         break;
   }
}

void ossimIgenGenerator::generatePixelTiledSpecList(bool outputToFileFlag)
{
   ossimRefPtr<ossimProjection> proj = createProductProjection();

   if((!proj)||(theOutputGeoPolygon.size() != 4))
   {
      return;
   }
   
   ossimIrect inputRect = getInputBoundingRect();
   inputRect.stretchToTileBoundary(theTileSpacing);
   ossimGeoPolygon tempPoly;
   ossimObject* obj = createInput();
   ossimImageChain* chain = PTR_CAST(ossimImageChain,
                                     obj);
   ossimGeoPolyCutter* cutter = new ossimGeoPolyCutter;
   chain->add(cutter);
   
   ossimKeywordlist kwl;
   ossimFilename outfile = theOutputKwl.find(ossimKeywordNames::FILENAME_KW);
   
   if(outfile == "")
   {
      cerr << "Valid filename not given, returning!" << endl;
   }
   
   kwl.add("object2.", theOutputKwl, true);
   kwl.add("product.projection.",theViewKwl, true);
   addPixelType(kwl, "product.");   
   ossimDpt p1;
   ossimDpt p2;
   ossimDpt p3;
   ossimDpt p4;

   
   proj->worldToLineSample(theOutputGeoPolygon[0], p1);
   proj->worldToLineSample(theOutputGeoPolygon[1], p2);
   proj->worldToLineSample(theOutputGeoPolygon[2], p3);
   proj->worldToLineSample(theOutputGeoPolygon[3], p4);

   ossimIrect bounds(p1, p2, p3, p4);
   if(inputRect.completely_within(bounds))
   {
      inputRect = bounds;
   }

   bounds.stretchToTileBoundary(theTileSpacing);
   
   ossim_sint32 i = 1;
   for(ossim_sint32 ulY = bounds.ul().y; ulY < bounds.lr().y; ulY+=(ossim_sint32)theTileSpacing.y)
   {
      for(ossim_sint32 ulX = bounds.ul().x; ulX < bounds.lr().x; ulX+=(ossim_sint32)theTileSpacing.x)
      {
         ossimDrect rect(ulX,
                         ulY,
                         ulX + (theTileSpacing.x - 1),
                         ulY + (theTileSpacing.y - 1));
         ossimGpt gpt1;
         ossimGpt gpt2;
         ossimGpt gpt3;
         ossimGpt gpt4;

         proj->lineSampleToWorld(rect.ul(), gpt1);
         proj->lineSampleToWorld(rect.ur(), gpt2);
         proj->lineSampleToWorld(rect.lr(), gpt3);
         proj->lineSampleToWorld(rect.ll(), gpt4);
         
         tempPoly.clear();
         tempPoly.addPoint(gpt1);
         tempPoly.addPoint(gpt2);
         tempPoly.addPoint(gpt3);
         tempPoly.addPoint(gpt4);
         addPadding(tempPoly, tempPoly);
         cutter->setPolygon(tempPoly);
         chain->saveState(kwl, "object1.");
         
         ossimFilename specName(theSpecFileLocation);
         specName = specName.dirCat(ossimFilename("igen" + ossimString::toString(i) + ".spec"));
         ossimFilename tilename = outfile.path();
         if(theTileNamingConvention == "id")
         {
            int tileId = getTileId(inputRect, ossimDpt(ulX+theTileSpacing.x/2, ulY+theTileSpacing.y/2), theTileSpacing);
            tilename = tilename.dirCat(ossimFilename(outfile.fileNoExtension() + "_" +
                                                     ossimString::toString(tileId)));
            tilename.setExtension(outfile.ext());
         }
         
         kwl.add("object2.",
                 ossimKeywordNames::FILENAME_KW,
                 tilename,
                 true);
         kwl.add("object2.input_connection1",
                 kwl.find("object1.id"),
                 true);
         theSpecFileList.push_back(specName);
         if(outputToFileFlag)
         {
            kwl.write(specName.c_str());
         }
         else
         {
            theSpecList.push_back(kwl);
         }
         i++;
      }
   }
}

void ossimIgenGenerator::generateGeoTiledSpecList(const ossimDpt& spacing, bool outputToFileFlag)
{
   ossimRefPtr<ossimProjection> proj = createProductProjection();
   if((proj.valid())&&(theOutputGeoPolygon.size() == 4))
   {
      ossimDrect rect;

      ossimIrect inputRect = getInputBoundingRect();
      ossimGpt gpt1;
      ossimGpt gpt2;
      ossimGpt gpt3;
      ossimGpt gpt4;
      proj->lineSampleToWorld(inputRect.ul(), gpt1);
      proj->lineSampleToWorld(inputRect.ur(), gpt2);
      proj->lineSampleToWorld(inputRect.lr(), gpt3);
      proj->lineSampleToWorld(inputRect.ll(), gpt4);

      ossimDrect inputGroundRect(gpt1,
                                 gpt2,
                                 gpt3,
                                 gpt4,
                                 OSSIM_RIGHT_HANDED);
      
      rect = ossimDrect(theOutputGeoPolygon[0],
                        theOutputGeoPolygon[1],
                        theOutputGeoPolygon[2],
                        theOutputGeoPolygon[3],
                        OSSIM_RIGHT_HANDED);

      rect.stretchToTileBoundary(spacing);
      inputGroundRect.stretchToTileBoundary(spacing);
      double ulLat = rect.ul().lat;
      double ulLon = rect.ul().lon;
      ossimGeoPolygon tempPoly;
      ossimObject* obj = createInput();
      ossimImageChain* chain = PTR_CAST(ossimImageChain,
                                        obj);
      ossimGeoPolyCutter* cutter = new ossimGeoPolyCutter;
      chain->add(cutter);
      
      ossimKeywordlist kwl;
      ossimFilename outfile = theOutputKwl.find(ossimKeywordNames::FILENAME_KW);

      if(outfile == "")
      {
         cerr << "Valid filename not given, returning!" << endl;
      }
      
      kwl.add("object2.", theOutputKwl, true);
      kwl.add("object2.input_connection1",
              kwl.find("object1.id"),
              true);
      kwl.add("product.projection.",theViewKwl, true);
      addPixelType(kwl, "product.");   
      int i = 1;
      int row = 0;
      int col = 0;
      //      int tileId = 0;
      while(ulLat > rect.ll().lat)
      {
         ulLon = rect.ul().lon;
         col = 0;
         while(ulLon < rect.ur().lon)
         {
            tempPoly.clear();
            tempPoly.addPoint(ulLat, ulLon, ossim::nan(), theOutputGeoPolygon[0].datum());
            tempPoly.addPoint(ulLat, ulLon+spacing.lon, ossim::nan(), theOutputGeoPolygon[0].datum());
            tempPoly.addPoint(ulLat-spacing.lat, ulLon+spacing.lon, ossim::nan(), theOutputGeoPolygon[0].datum());
            tempPoly.addPoint(ulLat-spacing.lat, ulLon, ossim::nan(), theOutputGeoPolygon[0].datum());
            
            addPadding(tempPoly, tempPoly);
            cutter->setPolygon(tempPoly);
            chain->saveState(kwl, "object1.");

            ossimFilename specName(theSpecFileLocation);
            specName = specName.dirCat(ossimFilename("igen" + ossimString::toString(i) + ".spec"));
	    ossimFilename tilename = outfile.path();
	    if(theTileNamingConvention == "id")
	      {
		int tileId = getTileId(inputGroundRect, ossimDpt(ulLon, ulLat), spacing);
		tilename = tilename.dirCat(ossimFilename(outfile.fileNoExtension() + "_" +
							 ossimString::toString(tileId)));
		tilename.setExtension(outfile.ext());
	      }
	    else
	      {
                 ossimUsgsQuad quad(ossimGpt(ulLat - (spacing.x), ulLon+(spacing.x), 0.0));
                 if(spacing.x == 7.5/60.0)
                 {
                    tilename = tilename.dirCat(ossimFilename(quad.quarterQuadName().downcase().trim()));
                 }
                 else
                 {
                    tilename = tilename.dirCat(ossimFilename(quad.quarterQuadSegName().downcase().trim()));
                 }
                 
                 tilename.setExtension(outfile.ext());
	      }

	    kwl.add("object2.",
		    ossimKeywordNames::FILENAME_KW,
		    tilename,
		    true);
            kwl.add("object2.input_connection1",
                    kwl.find("object1.id"),
                    true);
            theSpecFileList.push_back(specName);
            if(outputToFileFlag)
            {
               kwl.write(specName.c_str());
            }
            else
            {
               theSpecList.push_back(kwl);
            }
            ulLon += spacing.lon;
            ++i;
            ++col;
         }
         ++row;
         ulLat -= spacing.lat;
      }

   }   
}


void ossimIgenGenerator::generateNoTiledSpecList(bool outputToFileFlag)
{
   theSpecList.clear();
   theSpecFileList.clear();
   
   if(theViewKwl.getSize()&&
      theInputKwl.getSize()&&
      theOutputKwl.getSize())
   {
      ossimKeywordlist kwl;
      if(theOutputGeoPolygon.size())
      {
         ossimRefPtr<ossimObject> obj = createInput();
         ossimRefPtr<ossimImageChain> chain = PTR_CAST(ossimImageChain,
                                                       obj.get());
         if(chain.valid())
         {
            ossimGeoPolyCutter* cutter = new ossimGeoPolyCutter;
            
            cutter->setPolygon(theOutputGeoPolygon);
            
            chain->add(cutter);
            chain->saveState(kwl, "object1.");
         }
         else
         {
            kwl.add("object1.",theInputKwl, true);
         }
         obj = 0;
      }
      else
      {
         kwl.add("object1.",theInputKwl, true);
      }
      kwl.add("object2.", theOutputKwl, true);
      kwl.add("object2.input_connection1",
              kwl.find("object1.id"),
              true);
      kwl.add("product.projection.",theViewKwl, true);
      addPixelType(kwl, "product.");   

      ossimFilename specName(theSpecFileLocation +
                             "/"+
                             "igen1.spec");
      
      theSpecFileList.push_back(specName);
      if(outputToFileFlag)
      {
         kwl.write(specName.c_str());
      }
      else
      {
         theSpecList.push_back(kwl);
      }
      
      kwl.clear();
   }
}

int ossimIgenGenerator::getTileId(const ossimDrect& rect,
				  const ossimDpt& currentPt,
				  const ossimDpt& spacing)const
{
   ossimDpt ul(rect.ul());
   
   double relY = fabs(ul.y - currentPt.y);
   double relX = fabs(ul.x - currentPt.x);
   
   
   ossim_int32 spacingY           = (ossim_int32)(relY/spacing.y);
   ossim_int32 spacingX           = (ossim_int32)(relX/spacing.x);
   ossim_int32 maxHorizontalTiles = (ossim_int32)( (rect.width()-1)/spacing.x);
   
   return spacingY*maxHorizontalTiles + spacingX;
}


ossimIrect ossimIgenGenerator::getInputBoundingRect()const
{
   ossimRefPtr<ossimObject> obj = createInput();
   ossimImageSource* inter = PTR_CAST(ossimImageSource, obj.get());
   ossimIrect result;

   result.makeNan();
   if(inter)
   {
      result = inter->getBoundingRect();
   }
   return result;
}

void ossimIgenGenerator::addPadding(ossimGeoPolygon& output,
                                    const ossimGeoPolygon& input)const
{
   output = input;
   switch(theTilePaddingUnits)
   {
      case OSSIM_PIXEL:
      {
         ossimRefPtr<ossimProjection> proj = createProductProjection();
         if(proj.valid())
         {
            ossimDpt ul;
            ossimDpt ur;
            ossimDpt lr;
            ossimDpt ll;

            if(input.getOrdering() == OSSIM_CLOCKWISE_ORDER)
            {
               proj->worldToLineSample(input[0], ul);
               proj->worldToLineSample(input[1], ur);
               proj->worldToLineSample(input[2], lr);
               proj->worldToLineSample(input[3], ll);
            }
            else
            {
               proj->worldToLineSample(input[0], ul);
               proj->worldToLineSample(input[1], ll);
               proj->worldToLineSample(input[2], lr);
               proj->worldToLineSample(input[3], ur);
            
            }
            
            ul -= theTilePadding;
            ur += ossimDpt(theTilePadding.x,
                           -theTilePadding.y);
            lr += theTilePadding;
            ll += ossimDpt(-theTilePadding.x,
                           theTilePadding.y);

            proj->lineSampleToWorld(ul, output[0]);
            proj->lineSampleToWorld(ur, output[1]);
            proj->lineSampleToWorld(lr, output[2]);
            proj->lineSampleToWorld(ll, output[3]);

            output.setOrdering(OSSIM_CLOCKWISE_ORDER);
         
         }
         break;
      }
      case OSSIM_DEGREES:
      case OSSIM_MINUTES:
      case OSSIM_SECONDS:
      {
         ossimGpt ul;
         ossimGpt ur;
         ossimGpt lr;
         ossimGpt ll;
         ossimDpt spacing = theTilePadding;

         if(theTilePaddingUnits==OSSIM_MINUTES)
         {
            spacing.x /= 60;
            spacing.y /= 60;
         }
         else if(theTilePaddingUnits==OSSIM_MINUTES)
         {
            spacing.x /= 3600;
            spacing.y /= 3600;
         }
      
         if(input.getOrdering() == OSSIM_CLOCKWISE_ORDER)
         {
            ul = input[0];
            ur = input[1];
            lr = input[2];
            ll = input[3];
         }
         else
         {
            ul = input[0];
            ll = input[1];
            lr = input[2];
            ur = input[3];
         }
         ul.lat += spacing.lat;
         ul.lon -= spacing.lon;
         ur.lat += spacing.lat;
         ur.lon += spacing.lon;
         lr.lat -= spacing.lat;
         lr.lon += spacing.lon;
         ll.lat -= spacing.lat;
         ll.lon -= spacing.lon;

         output[0] = ul;
         output[1] = ur;
         output[2] = lr;
         output[3] = ll;
         output.setOrdering(OSSIM_CLOCKWISE_ORDER);
      
         break;
      }
      case OSSIM_METERS:
      {
         break;
      }
      default:
         break;
   }
}

void ossimIgenGenerator::addPixelType(ossimKeywordlist& kwl, const char* prefix)
{
   if(thePixelType == OSSIM_PIXEL_IS_POINT)
   {
      kwl.add(prefix,
              ossimKeywordNames::PIXEL_TYPE_KW,
              "PIXEL_IS_POINT",
              true);
   }
   else
   {
      kwl.add(prefix,
              ossimKeywordNames::PIXEL_TYPE_KW,
              "PIXEL_IS_AREA",
              true);
   }
}

void ossimIgenGenerator::setOutput(const ossimKeywordlist& kwl)
{
   theOutputKwl.clear();

   theOutputKwl = kwl;
}
