#include <sstream>

#include <ossim/imaging/ossimVpfAnnotationFeatureInfo.h>
#include <ossim/vec/ossimVpfCoverage.h>
#include <ossim/vec/ossimVpfLibrary.h>
#include <ossim/vec/ossimVpfFeatureClassSchema.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimPolyLine.h>
#include <ossim/base/ossimGeoPolygon.h>
#include <ossim/imaging/ossimGeoAnnotationSource.h>
#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/imaging/ossimGeoAnnotationMultiPolyLineObject.h>
#include <ossim/imaging/ossimGeoAnnotationMultiEllipseObject.h>
#include <ossim/imaging/ossimGeoAnnotationFontObject.h>
#include <ossim/imaging/ossimGeoAnnotationMultiPolyObject.h>
#include <ossim/imaging/ossimRgbImage.h>
#include <ossim/font/ossimFont.h>
#include <ossim/font/ossimFontFactoryRegistry.h>
#include <ossim/base/ossimTrace.h>

static ossimTrace traceDebug("ossimVpfAnnotationFeatureInfo:debug");

RTTI_DEF1(ossimVpfAnnotationFeatureInfo, "ossimVpfAnnotationFeatureInfo", ossimObject);

ossimVpfAnnotationFeatureInfo::ossimVpfAnnotationFeatureInfo(const ossimString& featureName,
                                                             const ossimRgbVector& penColor,
                                                             const ossimRgbVector& brushColor,
                                                             const ossimDpt& pointRadius,
                                                             int   thickness,
                                                             bool enabledFlag)
   :theName(featureName),
    thePenColor(penColor),
    theBrushColor(brushColor),
    thePointRadius(pointRadius),
    theThickness(thickness),
    theFillEnabledFlag(false),
    theEnabledFlag(enabledFlag),
    theFeatureType(ossimVpfAnnotationFeatureType_UNKNOWN),
    theFontInformation(),
    theAnnotationArray(0)
{
   ossimFont* font = ossimFontFactoryRegistry::instance()->getDefaultFont();

   if(font)
   {
      std::vector<ossimFontInformation> fontInfoArray;

      font->getFontInformation(fontInfoArray);

      if(fontInfoArray.size())
      {
         theFontInformation = fontInfoArray[0];
      }
   }
}


ossimVpfAnnotationFeatureInfo::~ossimVpfAnnotationFeatureInfo()
{
   deleteAllObjects();
}

void ossimVpfAnnotationFeatureInfo::transform(ossimImageGeometry* proj)
{
   if(theEnabledFlag)
   {
      for(int idx = 0; idx < (int)theAnnotationArray.size();++idx)
      {
         if(theAnnotationArray[idx].valid())
         {
            theAnnotationArray[idx]->transform(proj);
            theAnnotationArray[idx]->computeBoundingRect();
         }
      }
   }
}
ossimIrect ossimVpfAnnotationFeatureInfo::getBoundingProjectedRect()const
{
  ossimIrect result;
  result.makeNan();

  if(theEnabledFlag)
  {
     for(int idx = 0; idx < (int)theAnnotationArray.size();++idx)
     {
        if(theAnnotationArray[idx].valid())
	{
           ossimIrect tempRect = theAnnotationArray[idx]->getBoundingRect();
           if(!tempRect.hasNans())
           {
	      if(result.hasNans())
              {
                 result = tempRect;
              }
	      else
              {
                 result = result.combine(tempRect);
              }
           }
	}
     }
  }
  
  return result;
}

void ossimVpfAnnotationFeatureInfo::setEnabledFlag(bool flag)
{
   if(!flag && theEnabledFlag)
   {
      deleteAllObjects();
   }
   else if(flag&&(!theEnabledFlag))
   {
      theEnabledFlag = flag;
      buildFeature();
   }
   theEnabledFlag = flag;
}

void ossimVpfAnnotationFeatureInfo::buildFeature()
{
  deleteAllObjects();
  
  ossimVpfFeatureClassSchemaNode node;

  ossimVpfFeatureClassSchema schema;
  theCoverage.openFeatureClassSchema(schema);
  if(schema.getFeatureClassNode(theName,
				node))
    {
      if(node.thePrimitiveTable.contains("edg"))
      {
         theFeatureType = ossimVpfAnnotationFeatureType_LINE;
	 if(theEnabledFlag)
	   {
	     buildEdgFeature(node.theTable.trim(),
			     node.theTableKey.trim(),
			     node.thePrimitiveTable.trim(),
			     node.thePrimitiveTableKey.trim());
	   }
	}
      else if(node.thePrimitiveTable.contains("txt"))
	{
            theFeatureType = ossimVpfAnnotationFeatureType_TEXT;
	    if(theEnabledFlag)
	      {
		buildTxtFeature(node.theTable.trim(),
				node.theTableKey.trim(),
				node.thePrimitiveTable.trim(),
				node.thePrimitiveTableKey.trim());
	      }
	}
      else if(node.thePrimitiveTable.contains("fac"))
	{
            theFeatureType = ossimVpfAnnotationFeatureType_POLYGON;
	    if(theEnabledFlag)
	      {
		buildFaceFeature(node.theTable.trim(),
				 node.theTableKey.trim(),
				 node.thePrimitiveTable.trim(),
				 node.thePrimitiveTableKey.trim());
	      }
	}
      else if(node.thePrimitiveTable.contains("cnd"))
	{
            theFeatureType = ossimVpfAnnotationFeatureType_POINT;
	    if(theEnabledFlag)
	      {
		buildPointFeature("cnd",
				  node.theTable.trim(),
				  node.theTableKey.trim(),
				  node.thePrimitiveTable.trim(),
				  node.thePrimitiveTableKey.trim());
	      }
	}
      else if(node.thePrimitiveTable.contains("end"))
	{
            theFeatureType = ossimVpfAnnotationFeatureType_POINT;
	    if(theEnabledFlag)
	      {
		buildPointFeature("end",
				  node.theTable.trim(),
				  node.theTableKey.trim(),
				  node.thePrimitiveTable.trim(),
				  node.thePrimitiveTableKey.trim());
	      }
	}
      else
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << "Primitive \""<<node.thePrimitiveTable<<"\" not handled yet" << endl;
         }
      }
    }
}

void ossimVpfAnnotationFeatureInfo::drawAnnotations(ossimRgbImage* tile)
{
   if(theEnabledFlag)
   {
      int idx = 0;
      for(idx = 0; idx < (int)theAnnotationArray.size(); ++idx)
      {
         theAnnotationArray[idx]->draw(*tile);
      }
   }
}

bool ossimVpfAnnotationFeatureInfo::saveState(ossimKeywordlist& kwl,
					      const char* prefix)const
{
  kwl.add(prefix,
	  "name",
	  theName.c_str(),
	  true);
  kwl.add(prefix,
	  "description",
	  theDescription.c_str(),
	  true);
  ossimString penColor = (ossimString::toString((int)thePenColor.getR())+ " " +
			  ossimString::toString((int)thePenColor.getG())+ " " +
			  ossimString::toString((int)thePenColor.getB()));
  ossimString brushColor = (ossimString::toString((int)theBrushColor.getR())+ " " +
			    ossimString::toString((int)theBrushColor.getG())+ " " +
			    ossimString::toString((int)theBrushColor.getB()));
  kwl.add(prefix,
	  "pen_color",
	  penColor,
	  true);
  kwl.add(prefix,
	  "brush_color",
	  brushColor,
	  true);

  if(theFeatureType == ossimVpfAnnotationFeatureType_POINT)
    {
      kwl.add(prefix,
	      "point_radius",
	      ossimString::toString(thePointRadius.x) + " " +
	      ossimString::toString(thePointRadius.y),
	      true);
      kwl.add(prefix,
	      "fill_enabled",
	      theFillEnabledFlag,
	      true);
    }
  else if(theFeatureType == ossimVpfAnnotationFeatureType_LINE)
    {
      kwl.add(prefix,
	      "thickness",
	      theThickness,
	      true);
    }
  else if(theFeatureType == ossimVpfAnnotationFeatureType_POLYGON)
    {
      kwl.add(prefix,
	      "fill_enabled",
	      theFillEnabledFlag,
	      true);
      kwl.add(prefix,
	      "thickness",
	      theThickness,
	      true);
    }
  else if(theFeatureType == ossimVpfAnnotationFeatureType_TEXT)
    {
      theFontInformation.saveState(kwl,
				   (ossimString(prefix)+"font.").c_str());
    }
  kwl.add(prefix,
	  "enabled",
	  theEnabledFlag,
	  true);
  kwl.add(prefix,
	  "feature_type",
	  getFeatureTypeAsString(),
	  true);

  return true;
}

bool ossimVpfAnnotationFeatureInfo::loadState(const ossimKeywordlist& kwl,
					      const char* prefix)
{
  const char* name        = kwl.find(prefix, "name");
  const char* description = kwl.find(prefix, "description");
  const char* enabled     = kwl.find(prefix, "enabled");
  const char* fillEnabled = kwl.find(prefix, "fill_enabled");
  const char* thickness   = kwl.find(prefix, "thickness");
  const char* pointRadius = kwl.find(prefix, "point_radius");
  const char* brushColor  = kwl.find(prefix, "brush_color");
  const char* penColor    = kwl.find(prefix, "pen_color");
  
  theFontInformation.loadState(kwl,
			       (ossimString(prefix)+"font.").c_str());

  if(name)
    {
      if(theName != name)
	{
	  deleteAllObjects();
	}
      theName = name;
    }
  if(description)
    {
      theDescription = description;
    }
  if(enabled)
    {
      theEnabledFlag = ossimString(enabled).toBool();

      if(!theEnabledFlag)
	{
	  deleteAllObjects();
	}
    }
  
  if(fillEnabled)
    {
      theFillEnabledFlag = ossimString(fillEnabled).toBool();
    }
  if(thickness)
    {
      theThickness = ossimString(thickness).toInt();
    }
  if(pointRadius)
    {
      stringstream ptRadiusStream(pointRadius);
      ptRadiusStream >> thePointRadius.x >> thePointRadius.y;
    }
  if(brushColor)
    {
      int r, g, b;
      stringstream colorStream(brushColor);
      colorStream >> r >> g >> b;

      theBrushColor.setR((ossim_uint8)r);
      theBrushColor.setG((ossim_uint8)g);
      theBrushColor.setB((ossim_uint8)b);
    }
  if(penColor)
    {
      int r, g, b;
      stringstream colorStream(brushColor);
      colorStream >> r >> g >> b;

      thePenColor.setR((ossim_uint8)r);
      thePenColor.setG((ossim_uint8)g);
      thePenColor.setB((ossim_uint8)b);
    }
  
  if(theEnabledFlag&&(theAnnotationArray.size() < 1))
    {
      buildFeature();
    }
  else
    {
      setDrawingFeaturesToAnnotation();
    }

  return true;
}

ossimString ossimVpfAnnotationFeatureInfo::getFeatureTypeAsString()const
{
   switch(theFeatureType)
   {
      case ossimVpfAnnotationFeatureType_POINT:
      {
         return "point";
      }
      case ossimVpfAnnotationFeatureType_LINE:
      {
         return "line";
      }
      case ossimVpfAnnotationFeatureType_POLYGON:
      {
         return "polygon";
      }
      case ossimVpfAnnotationFeatureType_TEXT:
      {
         return "text";
      }
      default:
         break;
   }
   
  return "unknown";
}

ossimVpfAnnotationFeatureInfo::ossimVpfAnnotationFeatureType ossimVpfAnnotationFeatureInfo::getFeatureTypeFromString(const ossimString& featureType)const
{
  ossimString downCased = featureType;
  downCased = downCased.downcase();

  if(downCased.contains("point"))
    {
      return ossimVpfAnnotationFeatureType_POINT;
    }
  else if(downCased.contains("line"))
    {
      return ossimVpfAnnotationFeatureType_LINE;
    }
  else if(downCased.contains("polygon"))
    {
      return ossimVpfAnnotationFeatureType_POLYGON;
    }
  else if(downCased.contains("text"))
    {
      return ossimVpfAnnotationFeatureType_TEXT;
    }

  return ossimVpfAnnotationFeatureType_UNKNOWN;

}

void ossimVpfAnnotationFeatureInfo::deleteAllObjects()
{
   theAnnotationArray.clear();
}

void ossimVpfAnnotationFeatureInfo::setDrawingFeaturesToAnnotation()
{
   switch(theFeatureType)
   {
   case ossimVpfAnnotationFeatureType_POINT:
   {
      ossimGeoAnnotationMultiEllipseObject* annotation = 0;
      for(int idx = 0; idx < (int)theAnnotationArray.size();++idx)
      {
         annotation = (ossimGeoAnnotationMultiEllipseObject*)theAnnotationArray[idx].get();

         annotation->setColor(thePenColor.getR(),
                              thePenColor.getG(),
                              thePenColor.getB());
         annotation->setThickness(theThickness);
         annotation->setFillFlag(theFillEnabledFlag);
         annotation->setWidthHeight(thePointRadius);
      }
      
      break;
   }
   case ossimVpfAnnotationFeatureType_TEXT:
   {
      ossimGeoAnnotationFontObject* annotation = 0;
      ossimRefPtr<ossimFont> font = ossimFontFactoryRegistry::instance()->createFont(theFontInformation);

      for(int idx = 0; idx < (int)theAnnotationArray.size();++idx)
      {
         annotation = (ossimGeoAnnotationFontObject*)theAnnotationArray[idx].get();
         annotation->setColor(thePenColor.getR(),
                              thePenColor.getG(),
                              thePenColor.getB());
         annotation->setThickness(theThickness);
         
         if(font.valid())
         {
            annotation->setFont((ossimFont*)font->dup());
         }
         annotation->setPointSize(theFontInformation.thePointSize);
         annotation->setScale(theFontInformation.theScale);
         annotation->setShear(theFontInformation.theShear);
         annotation->setRotation(theFontInformation.theRotation);
      }
      break;
   }
   case ossimVpfAnnotationFeatureType_LINE:
   {
      ossimGeoAnnotationMultiPolyLineObject* annotation = 0;
      for(int idx = 0; idx < (int)theAnnotationArray.size();++idx)
      {
         annotation = (ossimGeoAnnotationMultiPolyLineObject*)theAnnotationArray[idx].get();
         annotation->setColor(thePenColor.getR(),
                              thePenColor.getG(),
                              thePenColor.getB());
         annotation->setThickness(theThickness);
      }
      
      break;
   }
   case ossimVpfAnnotationFeatureType_POLYGON:
   {
      ossimGeoAnnotationMultiPolyObject* annotation = 0;
      for(int idx = 0; idx < (int)theAnnotationArray.size();++idx)
      {
         annotation = (ossimGeoAnnotationMultiPolyObject*)theAnnotationArray[idx].get();
         annotation->setColor(thePenColor.getR(),
                              thePenColor.getG(),
                              thePenColor.getB());
         annotation->setThickness(theThickness);
         annotation->setFillFlag(theFillEnabledFlag);
      }
      break;
   }
   default:
   {
      break;
   }
   }
}


void ossimVpfAnnotationFeatureInfo::buildTxtFeature(const ossimFilename& tableName,
						    const ossimString&   tableKey,
						    const ossimFilename& /* primitiveName */,
						    const ossimString&   /* primitiveKey */)
{
  ossimFilename tableFileName      = theCoverage.getPath().dirCat(tableName);
  ossimFilename primitiveTableName;
  ossimVpfTable table;
  ossimVpfTable primitiveTable;

  if(table.openTable(tableFileName))
    {
      table.reset();
      vector<ossimString> columnValues = table.getColumnValues(tableKey.trim());
      
      primitiveTableName = theCoverage.getPath().dirCat("txt");
      
      vector<ossimString> tileIds;
      ossim_int32 stringValuePosition = 0;
      ossim_int32 shapeLinePosition = 0;
      bool isTiled = false;
      if(table.getColumnPosition("tile_id") >= 0)
	{
	  tileIds = table.getColumnValues("tile_id");
	  isTiled = true;
	}
      if(!isTiled)
	{
	  primitiveTableName = theCoverage.getPath().dirCat("txt");
	  if(!primitiveTable.openTable(primitiveTableName))
	    {
	      return;
	    }
	  stringValuePosition = primitiveTable.getColumnPosition("string");
	  shapeLinePosition   = primitiveTable.getColumnPosition("shape_line");
	}
      ossim_int32 tileId = -1;
      for(ossim_uint32 idx = 0; idx < columnValues.size();++idx)
	{
	  if(isTiled)
	    {
	      if(tileId != tileIds[idx].toInt())
		{
		  tileId = tileIds[idx].toInt();
		  ossimFilename filename = theCoverage.getLibrary()->getTileName(tileIds[idx].toInt());
		  if(theCoverage.getPath().dirCat(filename).dirCat("txt").exists())
		    {
		      primitiveTableName = theCoverage.getPath().dirCat(filename).dirCat("txt");
		    }
		  else if(theCoverage.getPath().dirCat(filename.downcase()).dirCat("txt").exists())
		    {
		      primitiveTableName = theCoverage.getPath().dirCat(filename.downcase()).dirCat("txt");
		    }
		  if(!primitiveTable.openTable(primitiveTableName))
		    {
		      return;
		    }
		  stringValuePosition = primitiveTable.getColumnPosition("string");
		  shapeLinePosition   = primitiveTable.getColumnPosition("shape_line");
		}
	    }
	  row_type row = read_row( columnValues[idx].toInt(), 
				   *primitiveTable.getVpfTableData());
	  ossim_int32 count = 0;
	  ossimDpt* ptArray = getXy(*primitiveTable.getVpfTableData(),
				    row,
				    shapeLinePosition,
				    &count);
	  ossimDpt midPoint;
	  if(ptArray)
	    {
	      midPoint = *ptArray;
	      
	      delete [] ptArray;
	    }
	  ossimString stringValue = primitiveTable.getColumnValueAsString(row,
									  stringValuePosition);
	  free_row(row,  *primitiveTable.getVpfTableData());
	  
	  if(!midPoint.hasNans())
	    {
	      ossimGpt centerPoint(midPoint.lat,
				   midPoint.lon,
				   ossim::nan());
              ossimFont* font = ossimFontFactoryRegistry::instance()->createFont(theFontInformation);
	      ossimGeoAnnotationFontObject* annotation = new ossimGeoAnnotationFontObject(centerPoint,
											  stringValue);
              annotation->setColor(thePenColor.getR(),
                                   thePenColor.getG(),
                                   thePenColor.getB());
              annotation->setThickness(theThickness);
                 
              annotation->setFont(font);
	      annotation->setPointSize(theFontInformation.thePointSize);
	      annotation->setScale(theFontInformation.theScale);
	      annotation->setShear(theFontInformation.theShear);
	      annotation->setRotation(theFontInformation.theRotation);
	      theAnnotationArray.push_back(annotation);
	    }
	}  
    }
}

void ossimVpfAnnotationFeatureInfo::buildEdgFeature(const ossimFilename& tableName,
						    const ossimString&   tableKey,
						    const ossimFilename& /* primitiveName */,
						    const ossimString&   /* primitiveKey */)
{
   ossimFilename tableFileName      = theCoverage.getPath().dirCat(tableName);
   ossimFilename primitiveTableName;
   ossimVpfTable table;
   ossimVpfTable primitiveTable;
  
   std::vector<ossimPolyLine> polyLineArray;

   if(table.openTable(tableFileName))
   {
      
      vector<ossimString> columnValues = table.getColumnValues(tableKey.trim());
      vector<ossimString> tileIds;
      bool isTiled = false;
      if(table.getColumnPosition("tile_id") >= 0)
      {
         tileIds = table.getColumnValues("tile_id");
         isTiled = true;
      }
      ossim_int32 coordinateValuePosition = 0;
      if(!isTiled)
      {
         primitiveTableName = theCoverage.getPath().dirCat("edg");
         if(!primitiveTable.openTable(primitiveTableName))
         {
            return;
         }
         coordinateValuePosition = primitiveTable.getColumnPosition("coordinates");
      }
      ossim_int32 tileId = -1;
      for(ossim_uint32 idx = 0; idx < columnValues.size();++idx)
      {
         if(isTiled)
         {
            if(tileId != tileIds[idx].toInt())
            {
               tileId = tileIds[idx].toInt();
               ossimFilename filename = theCoverage.getLibrary()->getTileName(tileIds[idx].toInt());
               if(theCoverage.getPath().dirCat(filename).dirCat("edg").exists())
               {
                  primitiveTableName = theCoverage.getPath().dirCat(filename).dirCat("edg");
               }
               else if(theCoverage.getPath().dirCat(filename.downcase()).dirCat("edg").exists())
               {
                  primitiveTableName = theCoverage.getPath().dirCat(filename.downcase()).dirCat("edg");
               }
               if(!primitiveTable.openTable(primitiveTableName))
               {
                  return;
               }
               coordinateValuePosition = primitiveTable.getColumnPosition("coordinates");
            }
         }
         ossimPolyLine polyLine;
	  
         readEdge(polyLine,
                  columnValues[idx].toInt(),
                  coordinateValuePosition,
                  primitiveTable);

	 readAttributes(polyLine, table, idx + 1); // third parm is one-based row
	  
         polyLineArray.push_back(polyLine);
      }
   }
   ossimGeoAnnotationMultiPolyLineObject* annotation = new ossimGeoAnnotationMultiPolyLineObject(polyLineArray);
   annotation->setColor(thePenColor.getR(),
                        thePenColor.getG(),
                        thePenColor.getB());
   annotation->setThickness(theThickness);
   theAnnotationArray.push_back(annotation);
}

void ossimVpfAnnotationFeatureInfo::readAttributes(ossimPolyLine& polyLine, ossimVpfTable& table, int row) {
  int numCols = table.getNumberOfColumns();

  for (int col = 0; col < numCols; col ++) {
    polyLine.addAttribute( table.getColumnValueAsString( row, col ));
  }
}

void ossimVpfAnnotationFeatureInfo::buildPointFeature(const ossimString& primitiveName,
						      const ossimFilename& tableName,
						      const ossimString&   tableKey,
						      const ossimFilename& /* primitive */,
						      const ossimString&   /* primitiveKey */)
{
  ossimFilename tableFileName      = theCoverage.getPath().dirCat(tableName);
  ossimFilename primitiveTableName;
  ossimVpfTable table;
  ossimVpfTable primitiveTable;
  
  std::vector<ossimGpt> centerPointArray;

  if(table.openTable(tableFileName))
    {
      
      vector<ossimString> columnValues = table.getColumnValues(tableKey.trim());
      vector<ossimString> tileIds;
      bool isTiled = false;
      if(table.getColumnPosition("tile_id") >= 0)
	{
	  tileIds = table.getColumnValues("tile_id");
	  isTiled = true;
	}
      ossim_int32 coordinateValuePosition = 0;
      if(!isTiled)
	{
	  primitiveTableName = theCoverage.getPath().dirCat(primitiveName);
	  if(!primitiveTable.openTable(primitiveTableName))
	    {
	      return;
	    }
	  coordinateValuePosition = primitiveTable.getColumnPosition("coordinate");
	}
      ossim_int32 tileId = -1;
      for(ossim_uint32 idx = 0; idx < columnValues.size();++idx)
	{
	  if(isTiled)
	    {
	      if(tileId != tileIds[idx].toInt())
		{
		  tileId = tileIds[idx].toInt();
		  ossimFilename filename = theCoverage.getLibrary()->getTileName(tileIds[idx].toInt());
		  if(theCoverage.getPath().dirCat(filename).dirCat(primitiveName).exists())
		    {
		      primitiveTableName = theCoverage.getPath().dirCat(filename).dirCat(primitiveName);
		    }
		  else if(theCoverage.getPath().dirCat(filename.downcase()).dirCat(primitiveName).exists())
		    {
		      primitiveTableName = theCoverage.getPath().dirCat(filename.downcase()).dirCat(primitiveName);
		    }
		  if(!primitiveTable.openTable(primitiveTableName))
		    {
		      return;
		    }
		  coordinateValuePosition = primitiveTable.getColumnPosition("coordinate");
		}
	    }
	  row_type row = read_row( columnValues[idx].toInt(), 
				   *primitiveTable.getVpfTableData());

	  ossim_int32 count = 0;
	  ossimDpt* ptArray = getXy(*primitiveTable.getVpfTableData(),
				    row,
				    coordinateValuePosition,
				    &count);
	  
	  if(ptArray)
	    {
	      for(int i = 0; i < count; ++i)
		{
		  if((fabs(ptArray[i].x) <= 180.0)&&
		     (fabs(ptArray[i].y) <= 90.0))
		    {
		      centerPointArray.push_back(ossimGpt(ptArray[i].lat,
							  ptArray[i].lon,
							  ossim::nan()));
		    }
		}
	      delete [] ptArray;
	    }
	  free_row(row,  *primitiveTable.getVpfTableData());
	}
    }
  ossimGeoAnnotationMultiEllipseObject* annotation = new ossimGeoAnnotationMultiEllipseObject(centerPointArray,
											      ossimDpt(2,2));
  annotation->setColor(thePenColor.getR(),
                       thePenColor.getG(),
                       thePenColor.getB());
  annotation->setThickness(theThickness);
  annotation->setFillFlag(theFillEnabledFlag);
  annotation->setWidthHeight(thePointRadius);
  theAnnotationArray.push_back(annotation);
}

int myFaceCount = 0;

void ossimVpfAnnotationFeatureInfo::buildFaceFeature(const ossimFilename& tableName,
						     const ossimString&   tableKey, // face_id
						     const ossimFilename& /* primitive */, //face
						     const ossimString&   /* primitiveKey */) // id
{
   ossimFilename tableFileName      = theCoverage.getPath().dirCat(tableName);
   ossimFilename primitiveTableName;
   ossimFilename rngTableName;
   ossimFilename edgTableName;
   ossimVpfTable table;
   ossimVpfTable primitiveTable;
   ossimVpfTable rngTable;
   ossimVpfTable edgTable;
   vector<ossimGeoPolygon> thePolyList;
  
   if(table.openTable(tableFileName))
   {
      vector<ossimString> columnValues = table.getColumnValues(tableKey.trim()); // fac_id
      vector<ossimString> tileIds;
      bool isTiled = false;
      if(table.getColumnPosition("tile_id") >= 0)
      {
         tileIds = table.getColumnValues("tile_id");
         isTiled = true;
      }
      // ossim_int32 coordinateValuePosition = 0;
      ossim_int32 startEdgePosition = 0;
      ossim_int32 rngPtrPosition = 0;
      if(!isTiled)
      {
         primitiveTableName = theCoverage.getPath().dirCat("fac");
         rngTableName = theCoverage.getPath().dirCat("rng");
         edgTableName = theCoverage.getPath().dirCat("edg");
         if(!primitiveTable.openTable(primitiveTableName)||
            !rngTable.openTable(rngTableName)||
            !edgTable.openTable(edgTableName))
         {
            return;
         }
         // coordinateValuePosition = edgTable.getColumnPosition("coordinates");
         startEdgePosition = rngTable.getColumnPosition("start_edge");
         rngPtrPosition = primitiveTable.getColumnPosition("ring_ptr");
      } // else will get for each face in loop below

      ossim_int32 tileId = -1;

      for(ossim_uint32 idx = 0; idx < columnValues.size();++idx) // for each face feature
      {
         if(isTiled)		// then get table names because we didn't get them above
         {
            if((tileId != tileIds[idx].toInt())||
               (tileId < 0))
            {
               tileId = tileIds[idx].toInt();
               ossimFilename filename = theCoverage.getLibrary()->getTileName(tileIds[idx].toInt());
               if(theCoverage.getPath().dirCat(filename).dirCat("fac").exists())
               {
                  primitiveTableName = theCoverage.getPath().dirCat(filename).dirCat("fac");
                  rngTableName = theCoverage.getPath().dirCat(filename).dirCat("rng");
                  edgTableName = theCoverage.getPath().dirCat(filename).dirCat("edg");
               }
               else if(theCoverage.getPath().dirCat(filename.downcase()).dirCat("fac").exists())
               {
                  primitiveTableName = theCoverage.getPath().dirCat(filename.downcase()).dirCat("fac");
                  rngTableName = theCoverage.getPath().dirCat(filename.downcase()).dirCat("rng");
                  edgTableName = theCoverage.getPath().dirCat(filename.downcase()).dirCat("edg");
               }
               if(!primitiveTable.openTable(primitiveTableName)||
                  !rngTable.openTable(rngTableName)||
                  !edgTable.openTable(edgTableName))
               {
                  return;
               }
               // coordinateValuePosition = edgTable.getColumnPosition("coordinates");
               startEdgePosition = rngTable.getColumnPosition("start_edge");
               rngPtrPosition = primitiveTable.getColumnPosition("ring_ptr");
            }
         } // if(isTiled)

         // get the outer ring id for this face
         int thisFaceId = columnValues[idx].toInt();
         int rngId = -1;

         if (thisFaceId <= primitiveTable.getNumberOfRows())
	    rngId = readRngId(thisFaceId,
			      rngPtrPosition,
			      primitiveTable);           // face table
         else
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG) << "not getting face " << thisFaceId << " from tile " << tileId << " " << primitiveTableName <<
                  " bacause it has only " << primitiveTable.getNumberOfRows() << " rows" << endl;
            }

         if(rngId > 0)
         {
            int startEdge = readStartEdgeId(rngId,
                                            startEdgePosition,
                                            rngTable);
            int outerStartEdge = startEdge;
            if(startEdge > 0 )
            {
               ossimGeoPolygon polygon;
                 
               myFaceCount ++;
                 
               int ringTableFaceIdColumn = rngTable.getColumnPosition("face_id");
               int ringFaceId = readTableCellAsInt(rngId, ringTableFaceIdColumn, rngTable);
                 
               if (thisFaceId != ringFaceId) {
                  // as of Mon Dec 20 2004, this has not been observed in vmap0 or vmap1
                  if(traceDebug())
                  {
                     ossimNotify(ossimNotifyLevel_DEBUG) << "rejecting face " << thisFaceId << " from " << tableFileName <<
                        " because it's ring (" << rngId << ") has face " << ringFaceId << " and startEdgeId " << startEdge << endl;
                  }
               } else {
                  readGeoPolygon( polygon,
                                  thisFaceId,
                                  startEdge,
                                  edgTable );
                    
                  // get the inner rings (holes)
                  int faceIdPosition = rngTable.getColumnPosition("face_id");
                  int innerRingOffset = 1;
                  int rowFaceId = -1;
                  if (rngId + innerRingOffset <= rngTable.getNumberOfRows())
                     rowFaceId = rngTable.getColumnValueAsString( rngId + innerRingOffset, faceIdPosition ).toInt();
                  while (rowFaceId == thisFaceId && rngId + innerRingOffset <= rngTable.getNumberOfRows()) {	// while there are more inner rings
                     startEdge = readStartEdgeId(rngId + innerRingOffset, startEdgePosition, rngTable);

                     if (startEdge == outerStartEdge) {
			// as of Mon Dec 20 2004, this has not been observed in vmap0 or vmap1
                        if(traceDebug())
                        {
                           ossimNotify(ossimNotifyLevel_DEBUG) << "rejecting hole in face "
                                                               << thisFaceId << " because inner ring "
                                                               << rngId + innerRingOffset
                                                               << " touches outer ring " << rngId << endl;
                        }

			innerRingOffset ++;
			rowFaceId = rngTable.getColumnValueAsString( rngId + innerRingOffset, faceIdPosition ).toInt();
			continue;
                     }

                     if(startEdge > 0 )
                     {
                        ossimGeoPolygon holePolygon;
		  
                        readGeoPolygon( holePolygon,
                                        thisFaceId,
                                        startEdge,
                                        edgTable );
                        if(holePolygon.size())
                           polygon.addHole( holePolygon );
                     }

                     innerRingOffset ++;
                     if (rngId + innerRingOffset <= rngTable.getNumberOfRows())
                        rowFaceId = rngTable.getColumnValueAsString( rngId + innerRingOffset, faceIdPosition ).toInt();
                  }

                  readAttributes(polygon, table, idx + 1); // third parm is one-based row

                  thePolyList.push_back(polygon);
               }
            }
         }
      } // each fac_id
   }
   ossimGeoAnnotationMultiPolyObject* annotation = new ossimGeoAnnotationMultiPolyObject(thePolyList);
   annotation->setColor(thePenColor.getR(),
                        thePenColor.getG(),
                        thePenColor.getB());
   annotation->setThickness(theThickness);
   annotation->setFillFlag(theFillEnabledFlag);
  
   theAnnotationArray.push_back(annotation);
}

void ossimVpfAnnotationFeatureInfo::readAttributes(ossimGeoPolygon& polygon, ossimVpfTable& table, int row) {
  int numCols = table.getNumberOfColumns();

  for (int col = 0; col < numCols; col ++) {
    ossimString s = table.getColumnValueAsString( row, col );
    polygon.addAttribute( s );
  }
}

/* GET_XY                                                                    */
/*****************************************************************************/
ossimDpt *ossimVpfAnnotationFeatureInfo::getXy(vpf_table_type table, 
					       row_type row, 
					       ossim_int32 pos, 
					       ossim_int32 *count)
{
   ossim_int32 i;
   ossimDpt *coord = 0;
  
   switch (table.header[pos].type)
   {
      case 'C':
      {
         coordinate_type temp, *ptr;
         ptr = (coordinate_type*)get_table_element(pos, row, table, &temp, count);
         coord = new ossimDpt[*count];
         if ((*count == 1) && (ptr == (coordinate_type*)0))
         {
            coord->x = (double)temp.x;
            coord->y = (double)temp.y;
         }
         else 
         {
	    for (i=0; i<*count; i++)
            {
               coord[i].x = (double)ptr[i].x;
               coord[i].y = (double)ptr[i].y;
            }
         }
         if (ptr)
         {
            free((char *)ptr);
         }
         break;
      }
      case 'Z':
      {
         tri_coordinate_type temp, *ptr;
         ptr = (tri_coordinate_type*)get_table_element (pos, row, table, &temp, count);
	 coord = new ossimDpt[*count];
         if ((*count == 1) && (ptr == (tri_coordinate_type*)0))
         {
            coord->x = (double)temp.x;
            coord->y = (double)temp.y;
         }
         else
         {
            for (i=0; i<*count; i++)
            {
               coord[i].x = (double)ptr[i].x;
               coord[i].y = (double)ptr[i].y;
            }
         }
         if (ptr)
         {
            free ((char*)ptr);
         }
         break;
      }
      case 'B':
      {
         double_coordinate_type temp, *ptr;
         ptr = (double_coordinate_type*)get_table_element (pos, row, table, &temp, count);
         coord = new ossimDpt[*count];
         if ((*count == 1) && (ptr == (double_coordinate_type*)0))
         {
            coord->x = temp.x;
            coord->y = temp.y;
         }
         else
         {
	    for (i=0; i<*count; i++)
            {
               coord[i].x = ptr[i].x;
               coord[i].y = ptr[i].y;
            }
         }
         if (ptr)
         {
	    free ((char*)ptr);
         }
         break;
      }
      case 'Y':
      {
         double_tri_coordinate_type temp, *ptr;
         ptr = (double_tri_coordinate_type*)get_table_element (pos, row, table, &temp, count);
         coord = new ossimDpt[*count];
         if ((*count == 1) && (ptr == (double_tri_coordinate_type*)0))
         {
            coord->x = temp.x;
            coord->y = temp.y;
         }
         else
         {
            for (i=0; i<*count; i++)
            {
               coord[i].x = ptr[i].x;
               coord[i].y = ptr[i].y;
            }
         }
         if (ptr)
         {
	    free((char*)ptr);
         }
         break;
      }
      
      default:
         break;
   } /* switch type */
   return (coord);
}

int ossimVpfAnnotationFeatureInfo::readTableCellAsInt (int rowNumber,
						       int colNumber,
						       ossimVpfTable& table)
{
  int result = -1;
  row_type row = read_row( rowNumber, *table.getVpfTableData());
  
  result = table.getColumnValueAsString(row, colNumber).toInt();
  
  free_row(row, *table.getVpfTableData());
  
  return result;
}

int ossimVpfAnnotationFeatureInfo::readRngId(int rowNumber,
					     int colNumber,
					     ossimVpfTable& faceTable)
{
  int idResult = -1;
  row_type row = read_row( rowNumber, 
			   *faceTable.getVpfTableData());
  
  idResult = faceTable.getColumnValueAsString(row,
					      colNumber).toInt();
  
  free_row(row, *faceTable.getVpfTableData());
  
  return idResult;
}

int ossimVpfAnnotationFeatureInfo::readStartEdgeId(int rowNumber,
						   int colNumber,
						   ossimVpfTable& rngTable)
{
  int idResult = -1;

  row_type row = read_row( rowNumber, 
			   *rngTable.getVpfTableData());
  
  idResult = rngTable.getColumnValueAsString(row,
					     colNumber).toInt();
  
  free_row(row, *rngTable.getVpfTableData());
  
  return idResult;
  
}

int ossimVpfAnnotationFeatureInfo::getEdgeKeyId (vpf_table_type& table, row_type& row, int col) {
    id_triplet_type key;
    ossim_int32 keyCount;
    get_table_element( col,
		       row,
		       table,
		       & key,
		       & keyCount );
    return key.id;
}

void ossimVpfAnnotationFeatureInfo::readGeoPolygon(ossimGeoPolygon& polygon,
						   int faceId,
						   int startEdgeId,
						   ossimVpfTable& edgTable) {

  ossim_int32 coordinatesCol = edgTable.getColumnPosition( "coordinates" );
  ossim_int32 startNodeCol = edgTable.getColumnPosition( "start_node" );
  ossim_int32 endNodeCol = edgTable.getColumnPosition( "end_node" );
  ossim_int32 rightEdgeCol = edgTable.getColumnPosition( "right_edge" );
  ossim_int32 leftEdgeCol = edgTable.getColumnPosition( "left_edge" );
  ossim_int32 rightFaceCol = edgTable.getColumnPosition( "right_face" );
  ossim_int32 leftFaceCol = edgTable.getColumnPosition( "left_face" );

  // collect edges
  vector < int > edges;
  int lastEdge = startEdgeId;
  edges.push_back( lastEdge );

  row_type row = read_row( startEdgeId, *edgTable.getVpfTableData() );
  int startNode = edgTable.getColumnValueAsString( row, startNodeCol ).toInt();
  int endNode = edgTable.getColumnValueAsString( row, endNodeCol ).toInt();

  if (startNode != endNode) { // there's more than one edge to this ring
    int rightFace = getEdgeKeyId( *edgTable.getVpfTableData(), row, rightFaceCol );
    int leftFace = getEdgeKeyId( *edgTable.getVpfTableData(), row, leftFaceCol );
    int rightEdge = getEdgeKeyId( *edgTable.getVpfTableData(), row, rightEdgeCol );
    int leftEdge = getEdgeKeyId( *edgTable.getVpfTableData(), row, leftEdgeCol );
    int nextEdge;
    int firstNode, lastNode;

    if (rightFace == leftFace) {
      if (rightEdge == leftEdge) {
	free_row(row,  *edgTable.getVpfTableData());
	// as of Mon Dec 20 2004, this has not been observed in vmap0 or vmap1
	// cout << "rejecting floating line face " << faceId << " line " << __LINE__ << endl;
	return;
      }

      // this is a dangling start edge; find a non dangling edge to use as the start edge
      int dirRight = 1;
      int nextEdge = startEdgeId;
      int nextLeftFace = leftFace;
      int nextRightFace = rightFace;
      int mobiusLimit = 1000;

      while (nextLeftFace == nextRightFace) {
	if (-- mobiusLimit < 0) {
	    free_row(row,  *edgTable.getVpfTableData());
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG) << "rejecting mobius face " << faceId << " line " << __LINE__ << endl;
            }
	    return;
	}

	int thisEdge = nextEdge;
	if (dirRight) {
	  nextEdge = getEdgeKeyId( *edgTable.getVpfTableData(), row, rightEdgeCol );
	  if (nextEdge == thisEdge)
	    dirRight = 0;
	  else if (nextEdge == startEdgeId) {
	    free_row(row,  *edgTable.getVpfTableData());
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG) << "rejecting mobius face " << faceId << " line " << __LINE__ << endl;
            }
	    return;
	  }
	} else {
	  nextEdge = getEdgeKeyId( *edgTable.getVpfTableData(), row, leftEdgeCol );
	  if (nextEdge == thisEdge) {
	    free_row(row,  *edgTable.getVpfTableData());
	    // as of Mon Dec 20 2004, this has not been observed in vmap0 or vmap1
	    // cout << "rejecting multi floating line face " << faceId << " line " << __LINE__ << endl;
	    return;
	  } else if (nextEdge == startEdgeId) {
	    free_row(row,  *edgTable.getVpfTableData());
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG) << "rejecting mobius face " << faceId << " line " << __LINE__ << endl;
            }
	    return;
	  }
	}
	free_row(row,  *edgTable.getVpfTableData());
	row = read_row( nextEdge, *edgTable.getVpfTableData() );
	nextRightFace = getEdgeKeyId( *edgTable.getVpfTableData(), row, rightFaceCol );
	nextLeftFace = getEdgeKeyId( *edgTable.getVpfTableData(), row, leftFaceCol );
      }

      startEdgeId = nextEdge;
      rightFace = nextRightFace;
      leftFace = nextLeftFace;
      free_row(row,  *edgTable.getVpfTableData());
      row = read_row( startEdgeId, *edgTable.getVpfTableData() );
      startNode = edgTable.getColumnValueAsString( row, startNodeCol ).toInt();
      endNode = edgTable.getColumnValueAsString( row, endNodeCol ).toInt();
    }

    if (rightFace == faceId) {
      nextEdge = getEdgeKeyId( *edgTable.getVpfTableData(), row, rightEdgeCol );
      firstNode = startNode;
      lastNode = endNode;
    } else if (leftFace == faceId) {
      nextEdge = getEdgeKeyId( *edgTable.getVpfTableData(), row, leftEdgeCol );
      firstNode = endNode;
      lastNode = startNode;
    }
    else
    {
       if(traceDebug())
       {
          ossimNotify(ossimNotifyLevel_DEBUG) << "rejecting face " << faceId << " line " << __LINE__ << endl;
       }
       return;
    }

    row_type nextRow;
    while (true) {
      nextRow = read_row( nextEdge, *edgTable.getVpfTableData() );
      int nextLeftEdge = getEdgeKeyId( *edgTable.getVpfTableData(), nextRow, leftEdgeCol );
      int nextRightEdge = getEdgeKeyId( *edgTable.getVpfTableData(), nextRow, rightEdgeCol );
      int nextLeftFace = getEdgeKeyId( *edgTable.getVpfTableData(), nextRow, leftFaceCol );
      int nextRightFace = getEdgeKeyId( *edgTable.getVpfTableData(), nextRow, rightFaceCol );
      int nextStartNode = edgTable.getColumnValueAsString( nextRow, startNodeCol ).toInt();
      int nextEndNode = edgTable.getColumnValueAsString( nextRow, endNodeCol ).toInt();

      while (nextLeftFace == nextRightFace) {
	int lastEnd;
	if (rightFace == faceId)
	  lastEnd = edgTable.getColumnValueAsString( row, endNodeCol ).toInt();
	else
	  lastEnd = edgTable.getColumnValueAsString( row, startNodeCol ).toInt();

	if (lastEnd == nextStartNode)
	  nextEdge = nextLeftEdge;
	else if (lastEnd == nextEndNode)
	  nextEdge = nextRightEdge;
	else {			// bad face
           if(traceDebug())
           {
              ossimNotify(ossimNotifyLevel_DEBUG) << "rejecting face " << faceId << " line " << __LINE__ << endl;
           }
	  return;
	}
	if (lastEdge == nextEdge)
        { // bad face
           if(traceDebug())
           {
              ossimNotify(ossimNotifyLevel_DEBUG) << "rejecting face " << faceId << " line " << __LINE__ << endl;
           }
           return;
	}

	free_row(nextRow,  *edgTable.getVpfTableData());
	nextRow = read_row( nextEdge, *edgTable.getVpfTableData() );
	nextLeftEdge = getEdgeKeyId( *edgTable.getVpfTableData(), nextRow, leftEdgeCol );
	nextRightEdge = getEdgeKeyId( *edgTable.getVpfTableData(), nextRow, rightEdgeCol );
	nextLeftFace = getEdgeKeyId( *edgTable.getVpfTableData(), nextRow, leftFaceCol );
	nextRightFace = getEdgeKeyId( *edgTable.getVpfTableData(), nextRow, rightFaceCol );
	nextStartNode = edgTable.getColumnValueAsString( nextRow, startNodeCol ).toInt();
	nextEndNode = edgTable.getColumnValueAsString( nextRow, endNodeCol ).toInt();
      }
	
      lastEdge = nextEdge;
      edges.push_back( lastEdge );

      {
	int lastEnd;
	if (nextRightFace == faceId) {
	  if (lastNode != nextStartNode) {
             if(traceDebug())
             {
                ossimNotify(ossimNotifyLevel_DEBUG) << "rejecting face " << faceId << " line " << __LINE__ << endl;
             }
	    return;
	  }
	  lastEnd = nextEndNode;
	} else if (nextLeftFace == faceId) {
	  if (lastNode != nextEndNode) {
             if(traceDebug())
             {
                ossimNotify(ossimNotifyLevel_DEBUG) << "rejecting face " << faceId << " line " << __LINE__ << endl;
             }
	    return;
	  }
	  lastEnd = nextStartNode;
	} else {
           if(traceDebug())
           {
              ossimNotify(ossimNotifyLevel_DEBUG) << "rejecting face " << faceId << " line " << __LINE__ << endl;
           }
	  return;
	}

	if (lastEnd == firstNode)
	  break;
      }

      free_row(row,  *edgTable.getVpfTableData());
      row = nextRow;
      startNode = nextStartNode;
      endNode = nextEndNode;
      rightFace = nextRightFace;
      leftFace = nextLeftFace;

      if (rightFace == faceId) {
	nextEdge = nextRightEdge;
	lastNode = endNode;
      } else {
	nextEdge = nextLeftEdge;
	lastNode = startNode;
      }
    }

    free_row(nextRow,  *edgTable.getVpfTableData());
    nextRow = 0;
  }

  for (vector< int >::iterator i = edges.begin(); i != edges.end(); i++) {
    ossim_int32 count = 0;
    free_row(row,  *edgTable.getVpfTableData());
    row = read_row( *i, *edgTable.getVpfTableData() );
    ossimDpt* ptArray = getXy(*edgTable.getVpfTableData(),
			      row,
			      coordinatesCol,
			      &count);
  
    if(ptArray)
    {
       int rightFace = getEdgeKeyId( *edgTable.getVpfTableData(), row, rightFaceCol );
       
       if (rightFace == faceId)
       {
	  for(int p = 0; p < count; ++p)
          {
             if((fabs(ptArray[p].x) <= 180.0)&&
                (fabs(ptArray[p].y) <= 90.0))
             {
                polygon.addPoint(ptArray[p].y, ptArray[p].x);
             }
          }
       }
       else
       {
	  for(int p = count - 1; p >= 0; --p)
          {
             if((fabs(ptArray[p].x) <= 180.0)&&
                (fabs(ptArray[p].y) <= 90.0))
             {
                polygon.addPoint(ptArray[p].y, ptArray[p].x);
             }
          }
       }
       delete [] ptArray;
    }
  }
  free_row(row,  *edgTable.getVpfTableData());
}

void ossimVpfAnnotationFeatureInfo::readEdge(ossimPolyLine& polyLine,
					     int rowNumber,
					     int colPosition,
					     ossimVpfTable& edgeTable)
{
  polyLine.clear();
  row_type row = read_row( rowNumber, 
			   *edgeTable.getVpfTableData());

  ossim_int32 count = 0;
  ossimDpt* ptArray = getXy(*edgeTable.getVpfTableData(),
			    row,
			    colPosition,
			    &count);
  
  if(ptArray)
    {
      for(int i = 0; i < count; ++i)
	{
	  if((fabs(ptArray[i].x) <= 180.0)&&
	     (fabs(ptArray[i].y) <= 90.0))
	    {
	      polyLine.addPoint(ossimDpt(ptArray[i].x,
					 ptArray[i].y));
	    }
	}
      delete [] ptArray;
    }
  free_row(row,  *edgeTable.getVpfTableData());
}


