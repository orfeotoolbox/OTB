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
// $Id: ossimEquationCombiner.cpp 15766 2009-10-20 12:37:09Z gpotts $

#include <cstdlib>
#include <sstream>
using namespace std;

#include <ossim/imaging/ossimEquationCombiner.h>
#include <ossim/imaging/ossimCastTileSourceFilter.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimConvolutionSource.h>
#include <ossim/imaging/ossimSubImageTileSource.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/matrix/newmatio.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimNotifyContext.h>

RTTI_DEF1(ossimEquationCombiner, "ossimEquationCombiner", ossimImageCombiner);

static const char* EQUATION_KW = "equation";


class ossimBinaryOpAdd : public ossimEquationCombiner::ossimBinaryOp
{
public:
   virtual double apply(double v1, double v2)const
      {
         return v1 + v2;
      }
};

class ossimBinaryOpAnd : public ossimEquationCombiner::ossimBinaryOp
{
public:
   virtual double apply(double v1, double v2)const
      {
         return (double)(((ossim_uint32)v1) & ((ossim_uint32)v2));
      }
};

class ossimBinaryOpOr : public ossimEquationCombiner::ossimBinaryOp
{
public:
   virtual double apply(double v1, double v2)const
      {
         return (double)(((ossim_uint32)v1) | ((ossim_uint32)v2));
      }
};

class ossimBinaryOpXor : public ossimEquationCombiner::ossimBinaryOp
{
public:
   virtual double apply(double v1, double v2)const
      {
         return (double)(((ossim_uint32)v1) ^ ((ossim_uint32)v2));
      }
};

class ossimBinaryOpSub : public ossimEquationCombiner::ossimBinaryOp
{
public:
   virtual double apply(double v1, double v2)const
      {
         return v1 - v2;
      }
};

class ossimBinaryOpMax : public ossimEquationCombiner::ossimBinaryOp
{
public:
   virtual double apply(double v1, double v2)const
      {
         return std::max(v1, v2);
      }
};

class ossimBinaryOpMin : public ossimEquationCombiner::ossimBinaryOp
{
public:
   virtual double apply(double v1, double v2)const
      {
         return std::min(v1, v2);
      }
};

class ossimBinaryOpMul : public ossimEquationCombiner::ossimBinaryOp
{
public:
   virtual double apply(double v1, double v2)const
      {
         return v1 * v2;
      }
};

class ossimBinaryOpDiv : public ossimEquationCombiner::ossimBinaryOp
{
public:
   virtual double apply(double v1, double v2)const
      {
         if(fabs(v2)>FLT_EPSILON)
            return v1 / v2;
         
         return 1.0/FLT_EPSILON;
      }
};

class ossimBinaryOpMod : public ossimEquationCombiner::ossimBinaryOp
{
public:
   virtual double apply(double v1, double v2)const
      {
         if(fabs(v2)>FLT_EPSILON)
            return fmod(v1,v2);
         
         return 1.0/FLT_EPSILON;
      }
};

class ossimBinaryOpPow : public ossimEquationCombiner::ossimBinaryOp
{
public:
   virtual double apply(double v1, double v2)const
      {
         return pow(v1, v2);
      }
};
// boolean operators
class ossimBinaryOpEqual : public ossimEquationCombiner::ossimBinaryOp
{
public:
   virtual double apply(double v1, double v2)const
      {
         return (v1==v2)?1.0:0.0;
      }
};
class ossimBinaryOpGreater : public ossimEquationCombiner::ossimBinaryOp
{
public:
   virtual double apply(double v1, double v2)const
      {
         return (v1>v2)?1.0:0.0;
      }
};
class ossimBinaryOpGreaterOrEqual : public ossimEquationCombiner::ossimBinaryOp
{
public:
   virtual double apply(double v1, double v2)const
      {
         return (v1>=v2)?1.0:0.0;
      }
};
class ossimBinaryOpLess : public ossimEquationCombiner::ossimBinaryOp
{
public:
   virtual double apply(double v1, double v2)const
      {
         return (v1<v2)?1.0:0.0;
      }
};
class ossimBinaryOpLessOrEqual : public ossimEquationCombiner::ossimBinaryOp
{
public:
   virtual double apply(double v1, double v2)const
      {
         return (v1<=v2)?1.0:0.0;
      }
};
class ossimBinaryOpDifferent : public ossimEquationCombiner::ossimBinaryOp
{
public:
   virtual double apply(double v1, double v2)const
      {
         return (v1!=v2)?1.0:0.0;
      }
};
class ossimUnaryOpNot : public ossimEquationCombiner::ossimUnaryOp
{
public:
   virtual double apply(double v)const
      {
         return 1-v;
      }
};

class ossimUnaryOpAbs : public ossimEquationCombiner::ossimUnaryOp
{
public:
   virtual double apply(double v)const
      {
         return fabs(v);
      }
};

class ossimUnaryOpOnesComplement : public ossimEquationCombiner::ossimUnaryOp
{
public:
   virtual double apply(double v)const
      {
         return (double)((ossim_uint8)~((ossim_uint8)v));
      }
};

class ossimUnaryOpLog : public ossimEquationCombiner::ossimUnaryOp
{
public:
   virtual double apply(double v)const
      {
         return log(v);
      }
};

class ossimUnaryOpLog10 : public ossimEquationCombiner::ossimUnaryOp
{
public:
   virtual double apply(double v)const
      {
         return log10(v);
      }
};

class ossimUnaryOpNeg : public ossimEquationCombiner::ossimUnaryOp
{
public:
   virtual double apply(double v)const
      {
         return -v;
      }
};

class ossimUnaryOpSqrt : public ossimEquationCombiner::ossimUnaryOp
{
public:
   virtual double apply(double v)const
      {
         if(v >= 0)
         {
            return sqrt(v);
         }
         
         return -1;
      }
};

class ossimUnaryOpExp : public ossimEquationCombiner::ossimUnaryOp
{
public:
   virtual double apply(double v)const
      {
         return exp(v);
      }
};

class ossimUnaryOpSin : public ossimEquationCombiner::ossimUnaryOp
{
public:
   virtual double apply(double v)const
      {
         return sin(v);
      }
};

class ossimUnaryOpSind : public ossimEquationCombiner::ossimUnaryOp
{
public:
   virtual double apply(double v)const
      {
         return sin(v*M_PI/180.0);
      }
};

class ossimUnaryOpASin : public ossimEquationCombiner::ossimUnaryOp
{
public:
   virtual double apply(double v)const
      {
         if(v > 1) v = 1;
         if(v < -1) v = -1;
         return asin(v);
      }
};

class ossimUnaryOpASind : public ossimEquationCombiner::ossimUnaryOp
{
public:
   virtual double apply(double v)const
      {
         if(v > 1) v = 1;
         if(v < -1) v = -1;
         return (180/M_PI)*asin(v);
      }
};

class ossimUnaryOpACos : public ossimEquationCombiner::ossimUnaryOp
{
public:
   virtual double apply(double v)const
      {
         if(v > 1) v = 1;
         if(v < -1) v = -1;
         return acos(v);
      }
};

class ossimUnaryOpACosd : public ossimEquationCombiner::ossimUnaryOp
{
public:
   virtual double apply(double v)const
      {
         if(v > 1) v = 1;
         if(v < -1) v = -1;
         return (180/M_PI)*acos(v);
      }
};

class ossimUnaryOpCos : public ossimEquationCombiner::ossimUnaryOp
{
public:
   virtual double apply(double v)const
      {
         return cos(v);
      }
};

class ossimUnaryOpCosd : public ossimEquationCombiner::ossimUnaryOp
{
public:
   virtual double apply(double v)const
      {
         return cos(v*M_PI/180.0);
      }
};

class ossimUnaryOpTan : public ossimEquationCombiner::ossimUnaryOp
{
public:
   virtual double apply(double v)const
      {
         return tan(v);
      }
};

class ossimUnaryOpTand : public ossimEquationCombiner::ossimUnaryOp
{
public:
   virtual double apply(double v)const
      {
         return tan(v*M_PI/180.0);
      }
};

class ossimUnaryOpATan : public ossimEquationCombiner::ossimUnaryOp
{
public:
   virtual double apply(double v)const
      {
         return atan(v);
      }
};

class ossimUnaryOpATand : public ossimEquationCombiner::ossimUnaryOp
{
public:
   virtual double apply(double v)const
      {
         return (180/M_PI)*atan(v);
      }
};


ossimEquationCombiner::ossimEquationCombiner()
   :ossimImageCombiner(),
    theOutputScalarType(OSSIM_FLOAT64),
    theEquation(""),
    theLexer(NULL),
    theTile(NULL),
    theCastFilter(NULL),
    theCastOutputFilter(NULL)
{
   theLexer      = new ossimEquTokenizer;
   theCastFilter = new ossimCastTileSourceFilter;
   theCastFilter->setOutputScalarType(OSSIM_FLOAT64);
}

ossimEquationCombiner::~ossimEquationCombiner()
{
   if(theLexer)
   {
      delete theLexer;
      theLexer = NULL;
   }

   if(theCastFilter.valid())
   {
      theCastFilter->disconnect();
      theCastFilter = 0;
   }

   if(theCastOutputFilter.valid())
   {
      theCastOutputFilter->disconnect();
      theCastOutputFilter = 0;
   }
   // make sure they are cleared
   clearStacks();
}

double ossimEquationCombiner::getNullPixelValue(ossim_uint32 band)const
{
   
   if(theEquation == "")
   {
      if(getInput())
      {
         ossimImageSource* inter = PTR_CAST(ossimImageSource, getInput());
         if(inter)
         {
            return inter->getNullPixelValue(band);
         }
      }
   }
   return ossim::defaultNull(getOutputScalarType());
}   

double ossimEquationCombiner::getMinPixelValue(ossim_uint32 band)const
{
   if(theEquation == "")
   {
      if(getInput())
      {
         ossimImageSource* inter = PTR_CAST(ossimImageSource, getInput());
         if(inter)
         {
            return inter->getMinPixelValue(band);
         }
      }
   }
   return ossim::defaultMin(getOutputScalarType());
}

double ossimEquationCombiner::getMaxPixelValue(ossim_uint32 band)const
{
   if(theEquation == "")
   {
      if(getInput())
      {
         ossimImageSource* inter = PTR_CAST(ossimImageSource, getInput());
         if(inter)
         {
            return inter->getMaxPixelValue(band);
         }
      }
   }
   return ossim::defaultMax(getOutputScalarType());         
}

ossimScalarType ossimEquationCombiner::getOutputScalarType() const
{

   if(theEquation == "")
   {
      if(getInput())
      {
         ossimImageSource* inter = PTR_CAST(ossimImageSource, getInput());
         if(inter)
         {
            return inter->getOutputScalarType();
         }
      }
   }
   
   return theOutputScalarType;
}


ossimRefPtr<ossimImageData> ossimEquationCombiner::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if(!theTile)
   {
      initialize();
   }
   long w = tileRect.width();
   long h = tileRect.height();
   long tw = theTile->getWidth();
   long th = theTile->getHeight();
   
   if(theEquation != "")
   {
      theTile->setImageRectangle(tileRect);
      
      if(w*h != tw*th)
      {
         theTile->initialize();
      }
      else
      {
         theTile->makeBlank();
      }
      theCurrentResLevel = resLevel;
      
      ossimRefPtr<ossimImageData> outputTile =  parseEquation();

      if(theCastOutputFilter.valid())
      {
         outputTile = theCastOutputFilter->applyCast(outputTile);
      }
      
      return outputTile;
   }
   else
   {
      if(getInput())
      {
         ossimImageSource* inter =
            PTR_CAST(ossimImageSource, getInput());
         if(inter)
         {
            return inter->getTile(tileRect, resLevel);
         }
      }
   }

   return ossimRefPtr<ossimImageData>();
}

void ossimEquationCombiner::setOutputScalarType(ossimScalarType scalarType)
{
   if(theOutputScalarType != scalarType)
   {
      theOutputScalarType = scalarType;

      if(theOutputScalarType == OSSIM_SCALAR_UNKNOWN)
      {
         theOutputScalarType = OSSIM_FLOAT64;
      }
      if(theCastOutputFilter.valid())
      {
         theCastOutputFilter = 0;
      }

      if(theOutputScalarType != OSSIM_FLOAT64)
      {
         theCastOutputFilter = new ossimCastTileSourceFilter;
         theCastOutputFilter->setOutputScalarType(theOutputScalarType);
         theCastOutputFilter->connectMyInputTo(0, this);
         theCastOutputFilter->initialize();
      }
   }
}

void ossimEquationCombiner::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property) return;
   
   if(property->getName() == "Equation")
   {
      theEquation = property->valueToString();
   }
   else if(property->getName() == "Output scalar type")
   {
      setOutputScalarType(ossimScalarTypeLut::instance()->
                          getScalarTypeFromString(property->valueToString()));
   }
   else
   {
      ossimImageCombiner::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimEquationCombiner::getProperty(const ossimString& name)const
{
   if(name == "Equation")
   {
      ossimStringProperty* stringProp = new ossimStringProperty("Equation",
								theEquation,
								false);
      stringProp->clearChangeType();
      stringProp->setReadOnlyFlag(false);
      stringProp->setCacheRefreshBit();
      
      return stringProp;
   }
   else if(name == "Output scalar type")
   {
      ossimScalarTypeLut* sl = ossimScalarTypeLut::instance();
      
      std::vector<ossimString> scalarNames;

      ossim_int32 tableSize = (ossim_int32)sl->getTableSize();
      ossim_int32 idx;

      for(idx = 0; idx < tableSize; ++idx)
      {
         scalarNames.push_back(sl->getEntryString(idx));
      }
      ossimStringProperty* stringProp = new ossimStringProperty("Output scalar type",
								sl->getEntryString((ossim_int32)theOutputScalarType),
								false,
								scalarNames);
      stringProp->clearChangeType();
      stringProp->setReadOnlyFlag(false);
      stringProp->setCacheRefreshBit();
      
      return stringProp;
      
   }

   return ossimImageCombiner::getProperty(name);
}

void ossimEquationCombiner::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageCombiner::getPropertyNames(propertyNames);
   propertyNames.push_back("Equation");
   propertyNames.push_back("Output scalar type");
}


void ossimEquationCombiner::initialize()
{
   ossimImageCombiner::initialize();

   theTile = ossimImageDataFactory::instance()->create(this, OSSIM_FLOAT64, getNumberOfOutputBands(), getTileWidth(), getTileHeight());
   theTile->initialize();
   if(theCastOutputFilter.valid())
   {
      theCastOutputFilter->initialize();
   }
}

void ossimEquationCombiner::assignValue()
{
   if(!theValueStack.empty())
   {
      if(theValueStack.top().type == OSSIM_EQU_IMAGE_DATA_TYPE)
      {
         ossimImageData* topData = theValueStack.top().d.imageDataValue;
         ossim_uint32 minBands = std::min(theTile->getNumberOfBands(),
                                    topData->getNumberOfBands());
         ossim_uint32 maxBands = theTile->getNumberOfBands();
         ossim_uint32 band   = 0;
         ossim_uint32 offset = 0;
         ossim_uint32 size = theTile->getWidth()*theTile->getHeight();

         if(topData->getDataObjectStatus() == OSSIM_PARTIAL)
         {
            for(band = 0; band < minBands; ++band)
            {
               double* inBuf  = (double*)topData->getBuf(band);
               double* outBuf = (double*)theTile->getBuf(band);
               double np      = topData->getNullPix(band);
               if(outBuf && inBuf)
               {
                  for(offset = 0; offset < size; ++offset)
                  {
                     if(*inBuf != np)
                     {
                        *outBuf = *inBuf;
                     }
                     ++outBuf;
                     ++inBuf;
                  }
               }
            }
            for(;band < maxBands; ++band)
            {
               double* inBuf  = (double*)topData->getBuf(minBands-1);
               double* outBuf = (double*)theTile->getBuf(band);
               double np      = topData->getNullPix(band);
               
               if(outBuf && inBuf)
               {
                  for(offset = 0; offset < size; ++offset)
                  {
                     if(*inBuf != np)
                     {
                        *outBuf = *inBuf;
                     }
                     ++outBuf;
                     ++inBuf;
                  }
               }
            }

         }
         else if(topData->getDataObjectStatus() == OSSIM_FULL)
         {
            for(band = 0; band < minBands; ++band)
            {
               double* inBuf  = (double*)theValueStack.top().d.imageDataValue->getBuf(band);
               double* outBuf = (double*)theTile->getBuf(band);
               if(outBuf && inBuf)
               {
                  for(offset = 0; offset < size; ++offset)
                  {
                     *outBuf = *inBuf;
                     ++outBuf;
                     ++inBuf;
                  }
               }
            }
            for(;band < maxBands; ++band)
            {
               double* inBuf  = (double*)theValueStack.top().d.imageDataValue->getBuf(minBands-1);
               double* outBuf = (double*)theTile->getBuf(band);
               
               if(outBuf && inBuf)
               {
                  for(offset = 0; offset < size; ++offset)
                  {
                     *outBuf = *inBuf;
                     ++outBuf;
                     ++inBuf;
                  }
               }
            }
         }
         
         // Delete the object indirectly through an ossimRefPtr.
         ossimRefPtr<ossimImageData> id = theValueStack.top().d.imageDataValue;
         id = NULL;
      }
      else
      {
         double* buf  = static_cast<double*>(theTile->getBuf());
         ossim_uint32 size = theTile->getSize();
         double value = (double)theValueStack.top().d.doubleValue;
         
         for(ossim_uint32 offset = 0; offset < size; ++offset)
         {
            *buf = value;
            ++buf;
         }
      }
      
      theValueStack.pop();
   }
}

void ossimEquationCombiner::clearStacks()
{

   while(!theValueStack.empty())
   {
      if(theValueStack.top().type == OSSIM_EQU_IMAGE_DATA_TYPE)
      {
         // Delete the object indirectly through an ossimRefPtr.
         ossimRefPtr<ossimImageData> id = theValueStack.top().d.imageDataValue;
         id = NULL;
      }

      theValueStack.pop();
   }
}

void ossimEquationCombiner::clearArgList(vector<ossimEquValue>& argList)
{
   for(ossim_uint32 i = 0; i < argList.size(); ++i)
   {
      if(argList[i].type == OSSIM_EQU_IMAGE_DATA_TYPE)
      {
         if(argList[i].d.imageDataValue)
         {
            // Delete the object indirectly through an ossimRefPtr.
            ossimRefPtr<ossimImageData> id = argList[i].d.imageDataValue;
            id = NULL;
            argList[i].d.imageDataValue = (ossimImageData*)NULL;
         }
      }
   }
   argList.clear();
}

void ossimEquationCombiner::deleteArgList(vector<ossimEquValue>& args)
{
   int i = 0;

   for(i = 0; i < (int)args.size(); ++i)
   {
      if(args[i].type == OSSIM_EQU_IMAGE_DATA_TYPE)
      {
         if(args[i].d.imageDataValue)
         {
            // Delete the object indirectly through an ossimRefPtr.
            ossimRefPtr<ossimImageData> id = args[i].d.imageDataValue;
            id = NULL;
            args[i].d.imageDataValue = NULL;
         }
      }
   }
   
   args.clear();
}

bool ossimEquationCombiner::parseArgList(vector<ossimEquValue>& args,
                                         bool popValueStack)
{
   bool result = true;
   
   if(theCurrentId == OSSIM_EQU_TOKEN_LEFT_PAREN)
   {
      theCurrentId = theLexer->yylex();
      do
      {
         if(parseExpression())
         {
            if(!theValueStack.empty())
            {
               args.push_back(theValueStack.top());
               if(popValueStack)
               {
                  theValueStack.pop();
               }
            }
            else
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "The expression at  arg " << (args.size()+1)
                  << " is empty" << endl;
               
               result = false;
            }
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN)
               <<"Unable to parse expression" << endl;
            result = false;
         }
         
         if(theCurrentId == OSSIM_EQU_TOKEN_COMMA)
         {
            theCurrentId = theLexer->yylex();
         }
         else if(theCurrentId != OSSIM_EQU_TOKEN_RIGHT_PAREN)
         {
            ossimNotify(ossimNotifyLevel_WARN)
               <<"Missing comma in argument list" << endl;
            result = false;
         }
         
      }while(result&&(theCurrentId != OSSIM_EQU_TOKEN_RIGHT_PAREN));
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "Starting left parenthesis missing from arg list" << endl;
      result = false;
   }
   
   if(theCurrentId == OSSIM_EQU_TOKEN_RIGHT_PAREN)
   {
      theCurrentId = theLexer->yylex(); // skip past right parenthesis
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         <<"No matching right parenthesis for arg list" << endl;
      result = false;
   }

   if(!result && popValueStack)
   {
      clearArgList(args);
   }
   return result;
}

bool ossimEquationCombiner::parseAssignBand()
{
   bool result = true;
   
   vector<ossimEquValue> argList;

   if(parseArgList(argList))
   {
      if((argList.size() == 3) ||
         (argList.size() == 4))
      {
         ossimEquValue v3 = argList[2];
         ossimEquValue v2 = argList[1];
         ossimEquValue v1 = argList[0];
         
         if(argList.size() == 3)
         {
            if((v1.type == OSSIM_EQU_IMAGE_DATA_TYPE) &&
               (v2.type == OSSIM_EQU_DOUBLE_TYPE))
            {
               ossimImageData *data = (ossimImageData*)v1.d.imageDataValue->dup();
               ossimEquValue v;
               
               if(v3.type == OSSIM_EQU_IMAGE_DATA_TYPE)
               {
                  if(data->getBuf()&&
                     v3.d.imageDataValue->getBuf())
                  {
                     if((ossim_uint32)(v2.d.doubleValue) < data->getNumberOfBands())
                     {
                        data->assignBand(v3.d.imageDataValue,
                                         0,
                                         (ossim_uint32)v2.d.doubleValue);
                        
                     }
                  }
               }
               else
               {
                  if(data->getBuf()&&
                     (ossim_uint32)v2.d.doubleValue < data->getNumberOfBands())
                  {  
                     ossim_uint32 upper = data->getWidth()*data->getHeight();
                     double* buf = (double*)data->getBuf((ossim_uint32)v2.d.doubleValue);
                     double value = v3.d.doubleValue;
                     if(buf)
                     {
                        for(ossim_uint32 i = 0; i < upper; ++i)
                        {
                           *buf = value;
                           ++buf;
                        }  
                     }
                     else
                     {
                        result = false;
                     }
                  }
               }
               if(result)
               {
                  data->validate();
                  v.type = OSSIM_EQU_IMAGE_DATA_TYPE;
                  v.d.imageDataValue = data;
                  theValueStack.push(v);
               }
            }
            else
            {
               result = false;
            }
         }
         else 
         {
            ossimEquValue v4 = argList[3];
            if((v1.type == OSSIM_EQU_IMAGE_DATA_TYPE) &&
               (v2.type == OSSIM_EQU_DOUBLE_TYPE)&&
               (v3.type == OSSIM_EQU_IMAGE_DATA_TYPE)&&
               (v4.type == OSSIM_EQU_DOUBLE_TYPE))
            {
               ossimImageData *data = (ossimImageData*)v1.d.imageDataValue->dup();
               ossimEquValue v;
               v.type = OSSIM_EQU_IMAGE_DATA_TYPE;
               v.d.imageDataValue = data;
               if(data->getBuf()&&v3.d.imageDataValue->getBuf())
               {
                  
                  ossim_uint32 index1 = (ossim_uint32)v4.d.doubleValue;
                  ossim_uint32 index2 = (ossim_uint32)v2.d.doubleValue;
                  
                  if((index1 > data->getNumberOfBands()) ||
                     (index1 > v3.d.imageDataValue->getNumberOfBands()))
                  {
                     result = false;
                  }
                  else
                  {     
                     data->assignBand(v3.d.imageDataValue,
                                      index1,
                                      index2);
                     data->validate();
                  }
                  
               }
               
               theValueStack.push(v);
            }
         }
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Invalid number of arguments to assign_band" << endl;
         result = false;
      }
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "unable to parse arguments for assign band" << endl;
      result = false;
   }

   clearArgList(argList);

   return result;
}

bool ossimEquationCombiner::parseStdFuncs()
{
   bool result = true;

   switch(theCurrentId)
   {
   case OSSIM_EQU_TOKEN_ASSIGN_BAND:
   {
      theCurrentId = theLexer->yylex();
      if(!parseAssignBand())
      {
         result = false;
      }
      
      break;
   }
   case OSSIM_EQU_TOKEN_CONV:
   {
      theCurrentId = theLexer->yylex();
      vector<ossimEquValue> args;
      if(parseArgList(args))
      {
         ossimImageData* resultImage = (ossimImageData*)NULL;
         if(applyConvolution(resultImage,
                              args))
         {
            if(resultImage)
            {
               ossimEquValue v;
               v.type = OSSIM_EQU_IMAGE_DATA_TYPE;
               v.d.imageDataValue = resultImage;
               
               theValueStack.push(v);
            }
            else
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "function conv error: resulting image is NULL" << endl;
               result = false;
            }
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "Unable to apply convolution" << endl;
            result = false;
         }
      }
      else
      {
         result = false;
      }
      
      break;
   }
   case OSSIM_EQU_TOKEN_CLAMP:
   {
      theCurrentId = theLexer->yylex();
      vector<ossimEquValue> args;
      if(parseArgList(args))
      {
         ossimImageData* resultImage = (ossimImageData*)NULL;
         if(applyClamp(resultImage,
                       args))
         {
            if(resultImage)
            {
               ossimEquValue v;
               v.type = OSSIM_EQU_IMAGE_DATA_TYPE;
               v.d.imageDataValue = resultImage;
               
               theValueStack.push(v);
            }
            else
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "function clamp error: resulting image is NULL" << endl;
               result = false;
            }
            
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "Unable to apply clamp" << endl;
            result = false;
         }
      }
      else
      {
         result = false;
      }
      
      break;
   }
   case OSSIM_EQU_TOKEN_BAND:
   {
    // need to parse the following rule for blurr function
      //
      // band(image data, number)
      theCurrentId = theLexer->yylex();


      vector<ossimEquValue> argList;

      if(parseArgList(argList))
      {
         if(argList.size() == 2)
         {
            ossimEquValue v1 = argList[0];
            ossimEquValue v2 = argList[1];
            ossimImageData* tempData = NULL;
            ossim_uint32 bandNumber           = 0;
            if(v1.type == OSSIM_EQU_IMAGE_DATA_TYPE)
            {
               tempData = v1.d.imageDataValue;
            }
            else
            {
               result = false;
            }
            if(v2.type == OSSIM_EQU_DOUBLE_TYPE)
            {
               bandNumber = (ossim_uint32)(v2.d.doubleValue);
            }
            else
            {
               result = false;
            }
            if(bandNumber > tempData->getNumberOfBands())
            {
               result = false;
            }
            if(result)
            {
               ossimImageData* data = new ossimImageData(this,
                                                         OSSIM_FLOAT64,
                                                         1);
               data->setWidthHeight(tempData->getWidth(),
                                    tempData->getHeight());
               data->setOrigin(tempData->getOrigin());
               data->setNullPix(tempData->getNullPix(bandNumber),
                                0);
               data->setMinPix(tempData->getMinPix(bandNumber),
                               0);
               data->setMaxPix(tempData->getMaxPix(bandNumber),
                               0);
               data->initialize();
               
               if((tempData->getBuf())&&
                  (bandNumber < tempData->getNumberOfBands()))
               {
                  data->assignBand(tempData,
                                   bandNumber,
                                   0);                           
                  data->validate();
               }
               ossimEquValue v;
               v.type = OSSIM_EQU_IMAGE_DATA_TYPE;
               v.d.imageDataValue = data;
               theValueStack.push(v);
            }
            
            if(tempData)
            {
               // Delete the object indirectly through an ossimRefPtr.
               ossimRefPtr<ossimImageData> id = tempData;
               tempData = NULL;
            }
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "Invalid number of args in function band" << endl;
            
            result = false;
         }
      }
      else
      {
         result = false;
      }
      
      break;
   }
   case OSSIM_EQU_TOKEN_BLURR:
   {
      theCurrentId = theLexer->yylex();
      vector<ossimEquValue> args;
      if(parseArgList(args))
      {
         ossimImageData* resultImage = (ossimImageData*)NULL;
         if(applyBlurr(resultImage,
                       args))
         {
            if(resultImage)
            {
               ossimEquValue v;
               v.type = OSSIM_EQU_IMAGE_DATA_TYPE;
               v.d.imageDataValue = resultImage;
               
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
         }
         else
         {
            result = false;
         }
      }
      else
      {
         result = false;
      }
      
      break;
   }
   case OSSIM_EQU_TOKEN_SHIFT:
   {
      theCurrentId = theLexer->yylex();
      vector<ossimEquValue> args;
      if(parseArgList(args))
      {
         ossimImageData* resultImage = (ossimImageData*)NULL;
         if(applyShift(resultImage,
                       args))
         {
            if(resultImage)
            {
               ossimEquValue v;
               v.type = OSSIM_EQU_IMAGE_DATA_TYPE;
               v.d.imageDataValue = resultImage;
               
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
         }
         else
         {
            result = false;
         }
      }
      else
      {
         result = false;
      }
      
      break;
   }
   case OSSIM_EQU_TOKEN_MAX:
   case OSSIM_EQU_TOKEN_MIN:
   {
      ossimBinaryOp* op = NULL;
      if(theCurrentId == OSSIM_EQU_TOKEN_MIN) op = new ossimBinaryOpMin;
      else op = new ossimBinaryOpMax;
      
      int argCount = 0;
      theCurrentId = theLexer->yylex();
      if(theCurrentId == OSSIM_EQU_TOKEN_LEFT_PAREN)
      {
         theCurrentId = theLexer->yylex();
         bool done = false;
         while(!done)
         {
            if(parseExpression())
            {
               
               ++argCount;
               if(theCurrentId == OSSIM_EQU_TOKEN_RIGHT_PAREN)
               {
                  theCurrentId = theLexer->yylex();
                  
                  done = true;
               }
               else if(theCurrentId == OSSIM_EQU_TOKEN_COMMA)
               {
                  theCurrentId = theLexer->yylex();
               }
               else
               {
                  result = false;
                  done = true;
               }
            }
            else
            {
               done = true;
               result = false;
            }
         }
         if((argCount > 1)&&result)
         {
            result = true;
            
            ossimEquValue v;
            ossimEquValue v1;
            ossimEquValue v2;

            v2 = theValueStack.top();
            theValueStack.pop();
            v1 = theValueStack.top();
            theValueStack.pop();
            argCount -=2;

            do
            {
               if(applyOp(*op,
                          v,
                          v1,
                          v2))
               {
                  theValueStack.push(v);
               }
               else
               {
                  result = false;
                  argCount = 0;
                  
               }
               --argCount;
               
               if((argCount>0)&&result)
               {
                  v2 = theValueStack.top();
                  theValueStack.pop();
                  v1 = theValueStack.top();
                  theValueStack.pop();
               }
               
            }while((argCount > 0)&&(result));
         }
         else
         {
            result = false;
         }
      }
      else
      {
         result = false;
      }
      if(op)
      {
         delete op;
         op = NULL;
      }
      break;
   }
   case OSSIM_EQU_TOKEN_ABS:
   {
      theCurrentId = theLexer->yylex();
      if(theCurrentId == OSSIM_EQU_TOKEN_LEFT_PAREN)
      {
         theCurrentId = theLexer->yylex();
         result = parseExpression();
         if(theCurrentId == OSSIM_EQU_TOKEN_RIGHT_PAREN)
         {
            if(theValueStack.size() > 0)
            {
               theCurrentId = theLexer->yylex();
               ossimEquValue v;
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
               
               applyOp(ossimUnaryOpAbs(),
                       v,
                       v1);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
         }
      }
      else
      {
         result = false;
      }
      break;
   }
   case OSSIM_EQU_TOKEN_SIN:
   {
      theCurrentId = theLexer->yylex();
      if(theCurrentId == OSSIM_EQU_TOKEN_LEFT_PAREN)
      {
         theCurrentId = theLexer->yylex();
         result = parseExpression();
         if(theCurrentId == OSSIM_EQU_TOKEN_RIGHT_PAREN)
         {
            if(theValueStack.size() > 0)
            {
               theCurrentId = theLexer->yylex();
               ossimEquValue v;
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
               
               applyOp(ossimUnaryOpSin(),
                       v,
                       v1);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
         }
      }
      else
      {
         result = false;
      }
      break;
   }
   case OSSIM_EQU_TOKEN_SIND:
   {
      theCurrentId = theLexer->yylex();
      if(theCurrentId == OSSIM_EQU_TOKEN_LEFT_PAREN)
      {
         theCurrentId = theLexer->yylex();
         result = parseExpression();
         if(theCurrentId == OSSIM_EQU_TOKEN_RIGHT_PAREN)
         {
            if(theValueStack.size() > 0)
            {
               theCurrentId = theLexer->yylex();
               ossimEquValue v;
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
               
               applyOp(ossimUnaryOpSind(),
                       v,
                       v1);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
         }
      }
      else
      {
         result = false;
      }
      break;
   }
   case OSSIM_EQU_TOKEN_ASIN:
   {
      theCurrentId = theLexer->yylex();
      if(theCurrentId == OSSIM_EQU_TOKEN_LEFT_PAREN)
      {
         theCurrentId = theLexer->yylex();
         result = parseExpression();
         if(theCurrentId == OSSIM_EQU_TOKEN_RIGHT_PAREN)
         {
            if(theValueStack.size() > 0)
            {
               theCurrentId = theLexer->yylex();
               ossimEquValue v;
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
               
               applyOp(ossimUnaryOpASin(),
                       v,
                       v1);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
         }
      }
      else
      {
         result = false;
      }
      break;
   }
   case OSSIM_EQU_TOKEN_ASIND:
   {
      theCurrentId = theLexer->yylex();
      if(theCurrentId == OSSIM_EQU_TOKEN_LEFT_PAREN)
      {
         theCurrentId = theLexer->yylex();
         result = parseExpression();
         if(theCurrentId == OSSIM_EQU_TOKEN_RIGHT_PAREN)
         {
            if(theValueStack.size() > 0)
            {
               theCurrentId = theLexer->yylex();
               ossimEquValue v;
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
               
               applyOp(ossimUnaryOpASind(),
                       v,
                       v1);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
         }
      }
      else
      {
         result = false;
      }
      break;
   }
   case OSSIM_EQU_TOKEN_COS:
   {
      theCurrentId = theLexer->yylex();
      if(theCurrentId == OSSIM_EQU_TOKEN_LEFT_PAREN)
      {
         theCurrentId = theLexer->yylex();
         result = parseExpression();
         if(theCurrentId == OSSIM_EQU_TOKEN_RIGHT_PAREN)
         {
            if(theValueStack.size() > 0)
            {
               theCurrentId = theLexer->yylex();
               ossimEquValue v;
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
               
               applyOp(ossimUnaryOpCos(),
                       v,
                       v1);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
         }
      }
      else
      {
         result = false;
      }
      break;
   }
   case OSSIM_EQU_TOKEN_COSD:
   {
      theCurrentId = theLexer->yylex();
      if(theCurrentId == OSSIM_EQU_TOKEN_LEFT_PAREN)
      {
         theCurrentId = theLexer->yylex();
         result = parseExpression();
         if(theCurrentId == OSSIM_EQU_TOKEN_RIGHT_PAREN)
         {
            if(theValueStack.size() > 0)
            {
               theCurrentId = theLexer->yylex();
               ossimEquValue v;
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
               
               applyOp(ossimUnaryOpCosd(),
                       v,
                       v1);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
         }
      }
      else
      {
         result = false;
      }
      break;
   }
   case OSSIM_EQU_TOKEN_ACOS:
   {
      theCurrentId = theLexer->yylex();
      if(theCurrentId == OSSIM_EQU_TOKEN_LEFT_PAREN)
      {
         theCurrentId = theLexer->yylex();
         result = parseExpression();
         if(theCurrentId == OSSIM_EQU_TOKEN_RIGHT_PAREN)
         {
            if(theValueStack.size() > 0)
            {
               theCurrentId = theLexer->yylex();
               ossimEquValue v;
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
               
               applyOp(ossimUnaryOpACos(),
                       v,
                       v1);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
         }
      }
      else
      {
         result = false;
      }
      break;
   }
   case OSSIM_EQU_TOKEN_ACOSD:
   {
      theCurrentId = theLexer->yylex();
      if(theCurrentId == OSSIM_EQU_TOKEN_LEFT_PAREN)
      {
         theCurrentId = theLexer->yylex();
         result = parseExpression();
         if(theCurrentId == OSSIM_EQU_TOKEN_RIGHT_PAREN)
         {
            if(theValueStack.size() > 0)
            {
               theCurrentId = theLexer->yylex();
               ossimEquValue v;
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
               
               applyOp(ossimUnaryOpACosd(),
                       v,
                       v1);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
         }
      }
      else
      {
         result = false;
      }
      break;
   }
   case OSSIM_EQU_TOKEN_TAN:
   {
      theCurrentId = theLexer->yylex();
      if(theCurrentId == OSSIM_EQU_TOKEN_LEFT_PAREN)
      {
         theCurrentId = theLexer->yylex();
         result = parseExpression();
         if(theCurrentId == OSSIM_EQU_TOKEN_RIGHT_PAREN)
         {
            if(theValueStack.size() > 0)
            {
               theCurrentId = theLexer->yylex();
               ossimEquValue v;
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
               
               applyOp(ossimUnaryOpTan(),
                       v,
                       v1);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
         }
      }
      else
      {
         result = false;
      }
      break;
   }
   case OSSIM_EQU_TOKEN_TAND:
   {
      theCurrentId = theLexer->yylex();
      if(theCurrentId == OSSIM_EQU_TOKEN_LEFT_PAREN)
      {
         theCurrentId = theLexer->yylex();
         result = parseExpression();
         if(theCurrentId == OSSIM_EQU_TOKEN_RIGHT_PAREN)
         {
            if(theValueStack.size() > 0)
            {
               theCurrentId = theLexer->yylex();
               ossimEquValue v;
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
               
               applyOp(ossimUnaryOpTand(),
                       v,
                       v1);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
         }
      }
      else
      {
         result = false;
      }
      break;
   }
   case OSSIM_EQU_TOKEN_ATAN:
   {
      theCurrentId = theLexer->yylex();
      if(theCurrentId == OSSIM_EQU_TOKEN_LEFT_PAREN)
      {
         theCurrentId = theLexer->yylex();
         result = parseExpression();
         if(theCurrentId == OSSIM_EQU_TOKEN_RIGHT_PAREN)
         {
            if(theValueStack.size() > 0)
            {
               theCurrentId = theLexer->yylex();
               ossimEquValue v;
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
               
               applyOp(ossimUnaryOpATan(),
                       v,
                       v1);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
         }
      }
      else
      {
         result = false;
      }
      break;
   }
   case OSSIM_EQU_TOKEN_ATAND:
   {
      theCurrentId = theLexer->yylex();
      if(theCurrentId == OSSIM_EQU_TOKEN_LEFT_PAREN)
      {
         theCurrentId = theLexer->yylex();
         result = parseExpression();
         if(theCurrentId == OSSIM_EQU_TOKEN_RIGHT_PAREN)
         {
            if(theValueStack.size() > 0)
            {
               theCurrentId = theLexer->yylex();
               ossimEquValue v;
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
               
               applyOp(ossimUnaryOpATand(),
                       v,
                       v1);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
         }
      }
      else
      {
         result = false;
      }
      break;
   }
   case OSSIM_EQU_TOKEN_LOG:
   {
      theCurrentId = theLexer->yylex();
      if(theCurrentId == OSSIM_EQU_TOKEN_LEFT_PAREN)
      {
         theCurrentId = theLexer->yylex();
         result = parseExpression();
         if(theCurrentId == OSSIM_EQU_TOKEN_RIGHT_PAREN)
         {
            if(theValueStack.size() > 0)
            {
               theCurrentId = theLexer->yylex();
               ossimEquValue v;
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
               
               applyOp(ossimUnaryOpLog(),
                       v,
                       v1);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
         }
      }
      else
      {
         result = false;
      }
      break;
   }
   case OSSIM_EQU_TOKEN_LOG10:
   {
      theCurrentId = theLexer->yylex();
      if(theCurrentId == OSSIM_EQU_TOKEN_LEFT_PAREN)
      {
         theCurrentId = theLexer->yylex();
         result = parseExpression();
         if(theCurrentId == OSSIM_EQU_TOKEN_RIGHT_PAREN)
         {
            if(theValueStack.size() > 0)
            {
               theCurrentId = theLexer->yylex();
               ossimEquValue v;
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
               
               applyOp(ossimUnaryOpLog10(),
                       v,
                       v1);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
         }
      }
      else
      {
         result = false;
      }
      break;
   }
   case OSSIM_EQU_TOKEN_SQRT:
   {
      theCurrentId = theLexer->yylex();
      if(theCurrentId == OSSIM_EQU_TOKEN_LEFT_PAREN)
      {
         theCurrentId = theLexer->yylex();
         result = parseExpression();
         if(theCurrentId == OSSIM_EQU_TOKEN_RIGHT_PAREN)
         {
            if(theValueStack.size() > 0)
            {
               theCurrentId = theLexer->yylex();
               ossimEquValue v;
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
               
               applyOp(ossimUnaryOpSqrt(),
                       v,
                       v1);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
         }
      }
      else
      {
         result = false;
      }
      break;
   }
   case OSSIM_EQU_TOKEN_EXP:
   {
      theCurrentId = theLexer->yylex();
      if(theCurrentId == OSSIM_EQU_TOKEN_LEFT_PAREN)
      {
         theCurrentId = theLexer->yylex();
         result = parseExpression();
         if(theCurrentId == OSSIM_EQU_TOKEN_RIGHT_PAREN)
         {
            if(theValueStack.size() > 0)
            {
               theCurrentId = theLexer->yylex();
               ossimEquValue v;
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
               
               applyOp(ossimUnaryOpExp(),
                       v,
                       v1);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
         }
      }
      else
      {
         result = false;
      }
      break;
   }
   default:
   {
      result = false;
   }
   }

   return result;
}

bool ossimEquationCombiner::parseUnaryFactor()
{
   bool result = false;

   
   if(theCurrentId == OSSIM_EQU_TOKEN_MINUS)
   {
      theCurrentId = theLexer->yylex();

      if(parseFactor())
      {
         if(theValueStack.size() > 0)
         {
            ossimEquValue v;
            ossimEquValue v1 = theValueStack.top();
            theValueStack.pop();
            
            applyOp(ossimUnaryOpNeg(),
                    v,
                    v1);
            
            theValueStack.push(v);
         }
         else
         {
            result = false;
         }

         result = true;
      }
      else
      {
         result = false;
      }
   }
   else if(theCurrentId == OSSIM_EQU_TOKEN_TILDE)
   {
      theCurrentId = theLexer->yylex();

      if(parseFactor())
      {
         if(theValueStack.size() > 0)
         {
            ossimEquValue v;
            ossimEquValue v1 = theValueStack.top();
            theValueStack.pop();
            
            applyOp(ossimUnaryOpOnesComplement(),
                    v,
                    v1);
            
            theValueStack.push(v);
         }
         else
         {
            result = false;
         }

         result = true;
      }
      else
      {
         result = false;
      }
   }
   return result;
}

bool ossimEquationCombiner::parseFactor()
{
   bool result = false;

   switch(theCurrentId)
   {
   case OSSIM_EQU_TOKEN_CONSTANT:
   {
      ossimEquValue v;

      v.type = OSSIM_EQU_DOUBLE_TYPE;
      v.d.doubleValue = atof(theLexer->YYText());
      theValueStack.push(v);
      
      theCurrentId = theLexer->yylex();
      
      result =  true;
      break;
   }
   case OSSIM_EQU_TOKEN_PI:
   {
      ossimEquValue v;

      v.type = OSSIM_EQU_DOUBLE_TYPE;
      v.d.doubleValue = M_PI;
      theValueStack.push(v);
      
      theCurrentId = theLexer->yylex();
      
      result =  true;
      break;      
   }
   case OSSIM_EQU_TOKEN_IMAGE_VARIABLE:
   {
      theCurrentId = theLexer->yylex();
      if(theCurrentId == OSSIM_EQU_TOKEN_LEFT_ARRAY_BRACKET)
      {
         theCurrentId = theLexer->yylex();
         if(parseExpression())
         {
            if(!theValueStack.empty())
            {
               if(theValueStack.top().type == OSSIM_EQU_DOUBLE_TYPE)
               {
                  if(theCurrentId == OSSIM_EQU_TOKEN_RIGHT_ARRAY_BRACKET)
                  {
                     theCurrentId = theLexer->yylex();
                     ossim_uint32 index = (ossim_uint32)theValueStack.top().d.doubleValue;
                     theValueStack.pop();
                     ossimRefPtr<ossimImageData> data = getNewImageData(index);
                     result = true;
                     if(data.valid())
                     {
                        ossimEquValue v;
                        v.type = OSSIM_EQU_IMAGE_DATA_TYPE;
                        v.d.imageDataValue = data.release();
                        theValueStack.push(v);

                     }
                     else
                     {
                        ossimNotify(ossimNotifyLevel_WARN)
                           <<"Data is NULL for array operation" << endl;
                     }
                     result = true;
                  }
                  else
                  {
                     ossimNotify(ossimNotifyLevel_WARN)
                        << "Mismatched Right array bracket" << endl;
                  }
               }
               else
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                     << "Expression between array brackets is not a number"
                     << endl;
               }
            }
            else
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "no expression within array brackets" << endl;
            }
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "Unabel to parse expression"<<endl;
         }
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            <<"Need left array brackets to access an input source"<<endl;
      }
      
      break;
   }
   case OSSIM_EQU_TOKEN_LEFT_PAREN:
   {
      theCurrentId = theLexer->yylex();
      if(parseExpression())
      {
         if(theCurrentId == OSSIM_EQU_TOKEN_RIGHT_PAREN)
         {
            result       =  true;
            theCurrentId = theLexer->yylex();
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "Right parenthesis missing" << endl;
            result = false;
         }
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Unable to parse expression within parenthesis" << endl;
         result = false;
      }
      
      break;
   }
   }
   if(!result) result = parseUnaryFactor();
   
   if(!result) result = parseStdFuncs();
   
   return result;
}

bool ossimEquationCombiner::parseRestOfTerm()
{
   //---
   // Parse the following rule:
   // RestOfTerm: * Factor RestOfTerm | / Factor RestOfTerm |
   // ^ Factor RestOfTerm
   //---
   bool result = true;
   
   switch(theCurrentId)
   {
      case OSSIM_EQU_TOKEN_MULT:
      {
         theCurrentId = theLexer->yylex();
         if(parseFactor())
         {
            if(theValueStack.size() > 1)
            {
               ossimEquValue v;
            
               ossimEquValue v2 = theValueStack.top();
               theValueStack.pop();
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
            
               applyOp(ossimBinaryOpMul(),
                       v,
                       v1,
                       v2);
            
               theValueStack.push(v);
            }
            else
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "Multiplication requires two arguments" << endl;
               result = false;
            }
            if(result)
            {
               result = parseRestOfTerm();
            }
         }
         else
         {
            result = false;
         }
         break;
      }
      case OSSIM_EQU_TOKEN_DIV:
      {
         theCurrentId = theLexer->yylex();
         if(parseFactor())
         {
            if(theValueStack.size() > 1)
            {
               ossimEquValue v;
            
               ossimEquValue v2 = theValueStack.top();
               theValueStack.pop();
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
            
               applyOp(ossimBinaryOpDiv(),
                       v,
                       v1,
                       v2);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
            if(result)
            {
               result = parseRestOfTerm();
            }
         }
         else
         {
            result = false;
         }

         break;
      }
      case OSSIM_EQU_TOKEN_XOR:
      {
         theCurrentId = theLexer->yylex();
         if(parseFactor())
         {
            if(theValueStack.size() > 1)
            {
               ossimEquValue v;
            
               ossimEquValue v2 = theValueStack.top();
               theValueStack.pop();
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
            
               applyOp(ossimBinaryOpXor(),
                       v,
                       v1,
                       v2);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
            if(result)
            {
               result = parseRestOfTerm();
            }
         }
         else
         {
            result = false;
         }

         break;
      }
      case OSSIM_EQU_TOKEN_AMPERSAND:
      {
         theCurrentId = theLexer->yylex();
         if(parseFactor())
         {
            if(theValueStack.size() > 1)
            {
               ossimEquValue v;
            
               ossimEquValue v2 = theValueStack.top();
               theValueStack.pop();
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
            
               applyOp(ossimBinaryOpAnd(),
                       v,
                       v1,
                       v2);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
            if(result)
            {
               result = parseRestOfTerm();
            }
         }
         else
         {
            result = false;
         }

         break;
      }
      case OSSIM_EQU_TOKEN_OR_BAR:
      {
         theCurrentId = theLexer->yylex();
         if(parseFactor())
         {
            if(theValueStack.size() > 1)
            {
               ossimEquValue v;
            
               ossimEquValue v2 = theValueStack.top();
               theValueStack.pop();
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
            
               applyOp(ossimBinaryOpOr(),
                       v,
                       v1,
                       v2);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
            if(result)
            {
               result = parseRestOfTerm();
            }
         }
         else
         {
            result = false;
         }

         break;
      }
      case OSSIM_EQU_TOKEN_MOD:
      {
         theCurrentId = theLexer->yylex();
         if(parseFactor())
         {
            if(theValueStack.size() > 1)
            {
               ossimEquValue v;
            
               ossimEquValue v2 = theValueStack.top();
               theValueStack.pop();
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
            
               applyOp(ossimBinaryOpMod(),
                       v,
                       v1,
                       v2);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
            if(result)
            {
               result = parseRestOfTerm();
            }
         }
         else
         {
            result = false;
         }

         break;
      }
      case OSSIM_EQU_TOKEN_POWER:
      {
         theCurrentId = theLexer->yylex();
         if(parseFactor())
         {
            if(theValueStack.size() > 1)
            {
               ossimEquValue v;
            
               ossimEquValue v2 = theValueStack.top();
               theValueStack.pop();
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
            
               applyOp(ossimBinaryOpPow(),
                       v,
                       v1,
                       v2);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
            if(result)
            {
               result = parseRestOfTerm();
            }
         }
         else
         {
            result = false;
         }
         break;
      }
      case OSSIM_EQU_TOKEN_BEQUAL:
      {
         theCurrentId = theLexer->yylex();
         if(parseFactor())
         {
            if(theValueStack.size() > 1)
            {
               ossimEquValue v;
            
               ossimEquValue v2 = theValueStack.top();
               theValueStack.pop();
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
            
               applyOp(ossimBinaryOpEqual(),
                       v,
                       v1,
                       v2);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
            if(result)
            {
               result = parseRestOfTerm();
            }
         }
         else
         {
            result = false;
         }

         break;
      }
      case OSSIM_EQU_TOKEN_BGREATER:
      {
         theCurrentId = theLexer->yylex();
         if(parseFactor())
         {
            if(theValueStack.size() > 1)
            {
               ossimEquValue v;
            
               ossimEquValue v2 = theValueStack.top();
               theValueStack.pop();
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
            
               applyOp(ossimBinaryOpGreater(),
                       v,
                       v1,
                       v2);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
            if(result)
            {
               result = parseRestOfTerm();
            }
         }
         else
         {
            result = false;
         }

         break;
      }
      case OSSIM_EQU_TOKEN_BGREATEROREQUAL:
      {
         theCurrentId = theLexer->yylex();
         if(parseFactor())
         {
            if(theValueStack.size() > 1)
            {
               ossimEquValue v;
            
               ossimEquValue v2 = theValueStack.top();
               theValueStack.pop();
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
            
               applyOp(ossimBinaryOpGreaterOrEqual(),
                       v,
                       v1,
                       v2);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
            if(result)
            {
               result = parseRestOfTerm();
            }
         }
         else
         {
            result = false;
         }

         break;
      }
      case OSSIM_EQU_TOKEN_BLESS:
      {
         theCurrentId = theLexer->yylex();
         if(parseFactor())
         {
            if(theValueStack.size() > 1)
            {
               ossimEquValue v;
            
               ossimEquValue v2 = theValueStack.top();
               theValueStack.pop();
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
            
               applyOp(ossimBinaryOpLess(),
                       v,
                       v1,
                       v2);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
            if(result)
            {
               result = parseRestOfTerm();
            }
         }
         else
         {
            result = false;
         }

         break;
      }
      case OSSIM_EQU_TOKEN_BLESSOREQUAL:
      {
         theCurrentId = theLexer->yylex();
         if(parseFactor())
         {
            if(theValueStack.size() > 1)
            {
               ossimEquValue v;
            
               ossimEquValue v2 = theValueStack.top();
               theValueStack.pop();
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
            
               applyOp(ossimBinaryOpLessOrEqual(),
                       v,
                       v1,
                       v2);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
            if(result)
            {
               result = parseRestOfTerm();
            }
         }
         else
         {
            result = false;
         }

         break;
      }
      case OSSIM_EQU_TOKEN_BDIFFERENT:
      {
         theCurrentId = theLexer->yylex();
         if(parseFactor())
         {
            if(theValueStack.size() > 1)
            {
               ossimEquValue v;
            
               ossimEquValue v2 = theValueStack.top();
               theValueStack.pop();
               ossimEquValue v1 = theValueStack.top();
               theValueStack.pop();
            
               applyOp(ossimBinaryOpDifferent(),
                       v,
                       v1,
                       v2);
            
               theValueStack.push(v);
            }
            else
            {
               result = false;
            }
            if(result)
            {
               result = parseRestOfTerm();
            }
         }
         else
         {
            result = false;
         }

         break;
      }

   }
   
   return result;
}

bool ossimEquationCombiner::parseTerm()
{
   // parse the following rule:
   //
   // Term : Factor RestOfTerm
   
   bool result = false;

   result = parseFactor();
   
   if(result)
   {
      result = parseRestOfTerm();
   }
   
   return result;
}

bool ossimEquationCombiner::parseRestOfExp()
{
   // parse the following rule:
   // RestOfExpression : + Term RestOfExpression | - Term RestOfExpression | epsilon
   //
   bool result = true;
   
   if(theCurrentId == OSSIM_EQU_TOKEN_PLUS)
   {
      theCurrentId = theLexer->yylex();
      if(parseTerm())
      {

         if(theValueStack.size() > 1)
         {
            ossimEquValue v;
            
            ossimEquValue v2 = theValueStack.top();
            theValueStack.pop();
            ossimEquValue v1 = theValueStack.top();
            theValueStack.pop();
            
            
            applyOp(ossimBinaryOpAdd(),
                    v,
                    v1,
                    v2);              
            
            theValueStack.push(v);
         }
         else
         {
            result = false;
         }
         if(result)
         {
            result = parseRestOfExp();
         }
      }
      else
      {
        result =  false;
      }
   }
   else if(theCurrentId == OSSIM_EQU_TOKEN_MINUS)
   {
      theCurrentId = theLexer->yylex();
      if(parseTerm())
      {
         if(theValueStack.size() > 1)
         {
            ossimEquValue v;
            
            ossimEquValue v2 = theValueStack.top();
            theValueStack.pop();
            ossimEquValue v1 = theValueStack.top();
            theValueStack.pop();
            
            applyOp(ossimBinaryOpSub(),
                    v,
                    v1,
                    v2);
            
            theValueStack.push(v);
         }
         else
         {
            result = false;
         }
         if(result)
         {
            result = parseRestOfExp();
         }
      }
      else
      {
         result =  false;
      }
   }
   
   return result;
}

ossimRefPtr<ossimImageData> ossimEquationCombiner::getImageData(ossim_uint32 index)
{
   ossimRefPtr<ossimImageData> result;
   ossimConnectableObject* obj = getInput(index);
   
   if(obj)
   {
      theCastFilter->connectMyInputTo(0, obj);
      result= (theCastFilter->getTile(theTile->getImageRectangle(),
                                      theCurrentResLevel));
      
      if(result.valid())
      {
         result->setMinPix(theTile->getMinPix(), theTile->getNumberOfBands());
         result->setMaxPix(theTile->getMaxPix(), theTile->getNumberOfBands());
      }
   }

   return result;
}

ossimRefPtr<ossimImageData> ossimEquationCombiner::getNewImageData(
   ossim_uint32 index)
{
   ossimRefPtr<ossimImageData> result = getImageData(index);

   if(result.valid())
   {
      if(result->getBuf())
      {
         result = (ossimImageData*)result->dup();
      }
   }

   return result;
}

bool ossimEquationCombiner::parseExpression()
{
   // parse the following rule:
   // expression : Term ResOfExpression
   //
   
   bool result = false;
   
   if(parseTerm())
   {
      result =  parseRestOfExp();
   }

   return result;
}

ossimRefPtr<ossimImageData> ossimEquationCombiner::parseEquation()
{
   ostringstream s;

   s << theEquation;

   istringstream inS(s.str());
   theLexer->switch_streams(&inS, &ossimNotify(ossimNotifyLevel_WARN));

   theCurrentId = theLexer->yylex();

   while(theCurrentId)
   {
      if(!parseExpression())
      {
         break;
      }
   }

   if(!theValueStack.empty())
   {
      assignValue();
      theTile->validate();
      clearStacks();
   }
   
   return theTile;
}    

bool ossimEquationCombiner::applyClamp(ossimImageData* &result,
                                       const vector<ossimEquValue>& argList)
{
   if(result)
   {
      // Delete the object indirectly through an ossimRefPtr.
      ossimRefPtr<ossimImageData> id = result;
      id = NULL;
      result = (ossimImageData*) NULL;
   }
   if(argList.size() <3)
   {
      return false;
   }

   if(argList[0].type == OSSIM_EQU_DOUBLE_TYPE)
   {
      return false;
   }
   else if( (argList[1].type == OSSIM_EQU_DOUBLE_TYPE)&&
            (argList[2].type == OSSIM_EQU_DOUBLE_TYPE))
   {
      result = argList[0].d.imageDataValue;
      
      if(argList[0].d.imageDataValue)
      {
         ossimDataObjectStatus status = result->getDataObjectStatus();
         if((status != OSSIM_NULL) &&
            (status != OSSIM_EMPTY))
         {
            double minValue = argList[1].d.doubleValue;
            double maxValue = argList[2].d.doubleValue;
            
            if(minValue > maxValue)
            {
               std::swap(minValue, maxValue);
            }
            
            int band   = 0;
            int offset = 0;
            
            int upperBoundBand   = result->getNumberOfBands();
            int offsetUpperBound = result->getWidth()*result->getHeight();

            if(status == OSSIM_PARTIAL)
            {
               for(band = 0; band < upperBoundBand; ++band)
               {
                  double np = static_cast<double>(result->getNullPix(band));
                  double *buf = static_cast<double*>(result->getBuf(band));
                  for(offset = 0; offset < offsetUpperBound; ++ offset)
                  {
                     if( *buf != np )
                     {
                        if( (*buf) < minValue) *buf = minValue;
                        else if( (*buf) >maxValue) *buf = maxValue;
                     }
                     ++buf;
                  }
               }
            }
            else
            {
               for(band = 0; band < upperBoundBand; ++band)
               {
                  double *buf = static_cast<double*>(result->getBuf(band));
                  for(offset = 0; offset < offsetUpperBound; ++ offset)
                  {
                     if( (*buf) < minValue) *buf = minValue;
                     else if( (*buf) >maxValue) *buf = maxValue;
                     ++buf;
                  }
               }
            }
         }
      }
         
      return true;
   }
   return false;
}

bool ossimEquationCombiner::applyConvolution(ossimImageData* &result,
                                             const vector<ossimEquValue>& argList)
{
   if(result)
   {
      // Delete the object indirectly through an ossimRefPtr.
      ossimRefPtr<ossimImageData> id = result;
      id = NULL;
      result = (ossimImageData*) NULL;
   }
   if(argList.size() <4) return false;
   
   for(ossim_uint32 i = 0; i < argList.size(); ++i)
   {
      if(argList[i].type != OSSIM_EQU_DOUBLE_TYPE)
      {
         return false;
      }
   }

   ossim_uint32 index = (ossim_uint32)argList[0].d.doubleValue;
   int rows = (int)argList[1].d.doubleValue;
   int cols = (int)argList[2].d.doubleValue;
   
   if((rows*cols) != (int)(argList.size()-3))
   {
      return false;
   }
   
   NEWMAT::Matrix m(rows,cols);

    int count = 3;
    for(int r = 0; r< rows;++r)
    {
       for(int c=0;c<cols;++c)
       {
          m[r][c] = argList[count].d.doubleValue;
          ++count;
       }
    }
   ossimConnectableObject* obj = getInput(index);
   if(obj)
   {
      ossimRefPtr<ossimConvolutionSource> conv = new ossimConvolutionSource(NULL, m);

      conv->connectMyInputTo(0, obj);
      theCastFilter->connectMyInputTo(0, conv.get());
      
      ossimRefPtr<ossimImageData> tempData =
         theCastFilter->getTile(theTile->getImageRectangle(),
                                theCurrentResLevel);
      if(tempData.valid())
      {
         result = (ossimImageData*)tempData->dup();
      }
      else
      {
         result = (ossimImageData*)theTile->dup();
      }
      conv->disconnect();
      conv = 0;
   }
   if(result)
   {
      return true;
   }

   return false;
}

bool ossimEquationCombiner::applyBlurr(ossimImageData* &result,
                                       const vector<ossimEquValue>& argList)
{
   if(result)
   {
      // Delete the object indirectly through an ossimRefPtr.
      ossimRefPtr<ossimImageData> id = result;
      id = NULL;
      result = (ossimImageData*) NULL;
   }
   if(argList.size() !=3) return false;
   
   for(ossim_uint32 i = 0; i < argList.size(); ++i)
   {
      if(argList[i].type != OSSIM_EQU_DOUBLE_TYPE)
      {
         return false;
      }
   }

   ossim_uint32 index = (ossim_uint32)argList[0].d.doubleValue;
   int rows = (int)argList[1].d.doubleValue;
   int cols = (int)argList[2].d.doubleValue;
   
   NEWMAT::Matrix m(rows, cols);

   m = 1.0/(rows*cols);

   ossimConnectableObject* obj = getInput(index);
   if(obj)
   {
      ossimRefPtr<ossimConvolutionSource> conv = new ossimConvolutionSource(NULL,
                                                                m);

      conv->connectMyInputTo(0, obj);
      theCastFilter->connectMyInputTo(0, conv.get());
      theCastFilter->initialize();

      ossimRefPtr<ossimImageData> tempData =
         theCastFilter->getTile(theTile->getImageRectangle(),
                                theCurrentResLevel);
      if(tempData.valid())
      {
         result = (ossimImageData*)tempData->dup();
      }
      conv->disconnect();
      conv = 0;
   }

   if(result)
   {
      return true;
   }

   return false;
}

bool ossimEquationCombiner::applyShift(ossimImageData* &result,
                                       const vector<ossimEquValue>& argList)
{
   if(result)
   {
      // Delete the object indirectly through an ossimRefPtr.
      ossimRefPtr<ossimImageData> id = result;
      id = NULL;
      result = (ossimImageData*) NULL;
   }
   if(argList.size() !=3) return false;
   
   for(ossim_uint32 i = 0; i < argList.size(); ++i)
   {
      if(argList[i].type != OSSIM_EQU_DOUBLE_TYPE)
      {
         return false;
      }
   }

   ossim_uint32 index = (ossim_uint32)argList[0].d.doubleValue;
   int x = (int)argList[1].d.doubleValue;
   int y = (int)argList[2].d.doubleValue;

   ossimConnectableObject* obj = getInput(index);
   if(obj)
   {
      ossimRefPtr<ossimSubImageTileSource> shiftSource =
         new ossimSubImageTileSource(NULL, ossimIpt(x, y));

      shiftSource->connectMyInputTo(0, obj);
      theCastFilter->connectMyInputTo(0, shiftSource.get());

       ossimRefPtr<ossimImageData> tempData =
          theCastFilter->getTile(theTile->getImageRectangle(),
                                 theCurrentResLevel);
       if(tempData.valid())
       {
          result = (ossimImageData*)tempData->dup();
       }
      shiftSource->disconnect();
      shiftSource = 0;
   }

   if(result)
   {
      return true;
   }

   return false;
}

bool ossimEquationCombiner::applyOp(const ossimBinaryOp& op,
                                    ossimEquValue& result,
                                    ossimEquValue& v1,
                                    ossimEquValue& v2)
{
   bool returnValue = true;
   
   if(v1.type == OSSIM_EQU_DOUBLE_TYPE)
   {
      if(v2.type == OSSIM_EQU_DOUBLE_TYPE)
      {
         result.type = OSSIM_EQU_DOUBLE_TYPE;
         result.d.doubleValue = op.apply(v1.d.doubleValue, v2.d.doubleValue);
      }
      else if(v2.type == OSSIM_EQU_IMAGE_DATA_TYPE)
      {
         returnValue = applyOp(op,
                               v1.d.doubleValue,
                               v2.d.imageDataValue);
         
         result.type = OSSIM_EQU_IMAGE_DATA_TYPE;
         result.d.imageDataValue = v2.d.imageDataValue;
      }
      else
      {
         returnValue = false;
      }
   }
   else if(v1.type == OSSIM_EQU_IMAGE_DATA_TYPE)
   {
      if(v2.type == OSSIM_EQU_DOUBLE_TYPE)
      {
         
         returnValue = applyOp(op,
                               v1.d.imageDataValue,
                               v2.d.doubleValue);
         
         result.type = OSSIM_EQU_IMAGE_DATA_TYPE;
         result.d.imageDataValue = v1.d.imageDataValue;
         returnValue = true;
      }
      else if(v2.type == OSSIM_EQU_IMAGE_DATA_TYPE)
      {
         returnValue = applyOp(op,
                               v1.d.imageDataValue,
                               v2.d.imageDataValue);
         result.type = OSSIM_EQU_IMAGE_DATA_TYPE;
         result.d.imageDataValue = v1.d.imageDataValue;

         // Delete the object indirectly through an ossimRefPtr.
         ossimRefPtr<ossimImageData> id = v2.d.imageDataValue;
         id = NULL;
         v2.d.imageDataValue = (ossimImageData*)NULL;
         returnValue = true;
      }
      else
      {
         returnValue = false;
      }
   }
   else
   {
      returnValue = false;
   }
   
   return returnValue;
}

bool ossimEquationCombiner::applyOp(const ossimBinaryOp& op,
                                    ossimImageData* v1,
                                    double          v2)
{   
   double* buf = static_cast<double*>(v1->getBuf());
   if(!buf) return true;
   ossimDataObjectStatus status = v1->getDataObjectStatus();

   if(status == OSSIM_EMPTY || status == OSSIM_NULL)
   {
      return true;
   }

   if(status == OSSIM_FULL )
   {
      ossim_uint32 size = v1->getSize();
      double value = (static_cast<double>(v2));
     
      for(ossim_uint32 i = 0; i < size; ++i)
      {
         *buf = (double)op.apply(*buf, value);
         ++buf;
      }
   }
   else
   {
      ossim_uint32 sizePerBand = v1->getSizePerBand();
      ossim_uint32 numberOfBands = v1->getNumberOfBands();

      if(numberOfBands)
      {
         for(ossim_uint32 band = 0; band < numberOfBands; ++band)
         {
            double* buf = static_cast<double*>(v1->getBuf(band));

            if(buf)
            {
               double  np  = static_cast<double>(v1->getNullPix()[band]);
               
               for(ossim_uint32 offset = 0; offset < sizePerBand;++offset)
               {
                  if(*buf != np)
                  {
                     *buf = (double)op.apply(*buf, v2);
                  }
                  ++buf;
               }
            }
         }
      }
   }
   return true;
}

bool ossimEquationCombiner::applyOp(const ossimBinaryOp& op,
                                    double          v1,
                                    ossimImageData* v2)
{   
   double* buf = static_cast<double*>(v2->getBuf());
   if(!buf) return true;
   ossimDataObjectStatus status = v2->getDataObjectStatus();

   if(status == OSSIM_EMPTY || status == OSSIM_NULL)
   {
      return true;
   }

   if(status == OSSIM_FULL )
   {
      ossim_uint32 size = v2->getSize();
      double value = (static_cast<double>(v1));
     
      for(ossim_uint32 i = 0; i < size; ++i)
      {
         *buf = (double)op.apply(value, *buf);
         ++buf;
      }
   }
   else
   {
      ossim_uint32 sizePerBand = v2->getSizePerBand();
      ossim_uint32 numberOfBands = v2->getNumberOfBands();

      if(numberOfBands)
      {
         for(ossim_uint32 band = 0; band < numberOfBands; ++band)
         {
            double* buf = static_cast<double*>(v2->getBuf(band));

            if(buf)
            {
               double  np  = static_cast<double>(v2->getNullPix()[band]);
               
               for(ossim_uint32 offset = 0; offset < sizePerBand; ++offset)
               {
                  if(*buf != np)
                  {
                     *buf = (double)op.apply((double)v1, *buf);
                  }
                  ++buf;
               }
            }
         }
      }
   }
   
   return true;
}

bool ossimEquationCombiner::applyOp(const ossimBinaryOp& op,
                                    ossimImageData* v1,
                                    ossimImageData* v2)
{
   ossim_uint32 minNumberOfBands = std::min(v1->getNumberOfBands(), v2->getNumberOfBands());
   ossim_uint32 maxNumberOfBands = std::max(v1->getNumberOfBands(), v2->getNumberOfBands());
   
   ossim_uint32 size = v1->getWidth()*v1->getHeight();
   ossimDataObjectStatus status1 = v1->getDataObjectStatus();
   ossimDataObjectStatus status2 = v2->getDataObjectStatus();

   double** bandV1   = new double*[maxNumberOfBands];
   double** bandV2   = new double*[maxNumberOfBands];
   double* bandV1Np  = new double[maxNumberOfBands];
   double* bandV2Np  = new double[maxNumberOfBands];
   ossim_uint32 band = 0;
   for(band = 0; band < minNumberOfBands; ++band)
   {
      bandV1[band]   = (double*)v1->getBuf(band);
      bandV2[band]   = (double*)v2->getBuf(band);
      bandV1Np[band] = (double)v1->getNullPix(band);
      bandV2Np[band] = (double)v2->getNullPix(band);
   }
   if(v1->getNumberOfBands() < v2->getNumberOfBands())
   {
      for(band = 0; band < maxNumberOfBands; ++band)
      {
         bandV1[band]   = (double*)v1->getBuf(minNumberOfBands-1);
         bandV2[band]   = (double*)v2->getBuf(band);
         bandV1Np[band] = (double)v1->getNullPix(minNumberOfBands-1);
         bandV2Np[band] = (double)v2->getNullPix(band);
      }
   }
   else if(v2->getNumberOfBands() < v1->getNumberOfBands())
   {
      for(band = 0; band < maxNumberOfBands; ++band)
      {
         bandV1[band]   = (double*)v1->getBuf(band);
         bandV2[band]   = (double*)v2->getBuf(minNumberOfBands-1);
         bandV1Np[band] = (double)v1->getNullPix(band);
         bandV2Np[band] = (double)v2->getNullPix(minNumberOfBands-1);
      }
   }

   if(status1 == OSSIM_EMPTY)
   {
      if(status2 == OSSIM_FULL)
      {
         for(band = 0; band < maxNumberOfBands; ++band)
         {
            double* buf1 = bandV1[band];
            double* buf2 = bandV2[band];
            
            for(ossim_uint32 i = 0; i < size; ++i)
            {
               *buf1 = *buf2;
               ++buf1;
               ++buf2;
            }
         }
      }
      else if(status2 == OSSIM_PARTIAL)
      {
         for(band = 0; band < maxNumberOfBands; ++band)
         {
            double* buf1 = bandV1[band];
            double* buf2 = bandV2[band];
            double  nullPix2 = bandV2Np[band];
            for(ossim_uint32 i = 0; i < size; ++i)
            {
               if(*buf2 != nullPix2)
               {
                  *buf1 = *buf2;
               }
               ++buf1;
               ++buf2;
            }
         }
      }
      v1->setDataObjectStatus(status2);
   }
   else if((status1 == OSSIM_FULL)&&
           (status2 == OSSIM_FULL))
   { 
      for(band = 0; band < maxNumberOfBands; ++band)
      {
         double* buf1 = bandV1[band];
         double* buf2 = bandV2[band];
         
         for(ossim_uint32 i = 0; i < size; ++i)
         {
            *buf1 = op.apply(*buf1, *buf2);
            ++buf1;
            ++buf2;
         }
      }
   }
   else if((status1 == OSSIM_FULL)&&
           (status2 == OSSIM_PARTIAL))
   {
      for(band = 0; band < maxNumberOfBands; ++band)
      {
         double* buf1 = bandV1[band];
         double* buf2 = bandV2[band];
         double  nullPix2 = bandV2Np[band];
         for(ossim_uint32 i = 0; i < size; ++i)
         {
            if(*buf2 != nullPix2)
            {
               *buf1 = op.apply(*buf1, *buf2);
            }
               
            ++buf1;
            ++buf2;
         }
      }
   }
   else if((status1 == OSSIM_PARTIAL)&&
           (status2 == OSSIM_FULL))
   {
      for(band = 0; band < maxNumberOfBands; ++band)
      {
         double* buf1 = bandV1[band];
         double* buf2 = bandV2[band];
         double  nullPix1 = bandV1Np[band];
         for(ossim_uint32 i = 0; i < size; ++i)
         {
            if(*buf1 != nullPix1)
            {
               *buf1 = op.apply(*buf1, *buf2);
            }
               
            ++buf1;
            ++buf2;
         }
      }
   }
   else if((status1 == OSSIM_PARTIAL)&&
           (status2 == OSSIM_PARTIAL))
   {
      for(band = 0; band < maxNumberOfBands; ++band)
      {
         double* buf1 = bandV1[band];
         double* buf2 = bandV2[band];
         double  nullPix1 = bandV1Np[band];
         double  nullPix2 = bandV2Np[band];
         for(ossim_uint32 i = 0; i < size; ++i)
         {
            if((*buf1 != nullPix1)&&
               (*buf2 != nullPix2))
            {
               *buf1 = op.apply(*buf1, *buf2);
            }
               
            ++buf1;
            ++buf2;
         }
      }
   }

   delete [] bandV1;
   delete [] bandV2;
   delete [] bandV1Np;
   delete [] bandV2Np;
   
   return true;
}


bool ossimEquationCombiner::applyOp(const ossimUnaryOp& op,
                                    ossimEquValue& result,
                                    ossimEquValue& v)
{
   bool returnValue = true;
   
   if(v.type == OSSIM_EQU_DOUBLE_TYPE)
   {
      result.type = OSSIM_EQU_DOUBLE_TYPE;
      result.d.doubleValue = op.apply(v.d.doubleValue);
   }
   else if(v.type == OSSIM_EQU_IMAGE_DATA_TYPE)
   {
      returnValue = applyOp(op,
                            v.d.imageDataValue);
      result.type = OSSIM_EQU_IMAGE_DATA_TYPE;
      result.d.imageDataValue = v.d.imageDataValue;
      returnValue = true;
   }
   else
   {
      returnValue = false;
   }
   
   return returnValue;
}

bool ossimEquationCombiner::applyOp(const ossimUnaryOp& op,
                                    ossimImageData* v)
{   
   double* buf = static_cast<double*>(v->getBuf());
   if(!buf) return true;
   ossimDataObjectStatus status = v->getDataObjectStatus();

   if(status == OSSIM_EMPTY || status == OSSIM_NULL)
   {
      return true;
   }

   if(status == OSSIM_FULL )
   {
      ossim_uint32 size = v->getSize();
     
      for(ossim_uint32 i = 0; i < size; ++i)
      {
         *buf = (double)op.apply(*buf);
         ++buf;
      }
   }
   else
   {
      ossim_uint32 sizePerBand   = v->getSizePerBand();
      ossim_uint32 numberOfBands = v->getNumberOfBands();

      if(numberOfBands)
      {
         for(ossim_uint32 band = 0; band < numberOfBands; ++band)
         {
            double* buf = static_cast<double*>(v->getBuf(band));

            if(buf)
            {
               double  np  = static_cast<double>(v->getNullPix()[band]);
               
               for(ossim_uint32 offset = 0; offset < sizePerBand;++offset)
               {
                  if(*buf != np)
                  {
                     *buf = (double)op.apply(*buf);
                  }
                  ++buf;
               }
            }
         }
      }
   }
   
   return true;
}

bool ossimEquationCombiner::saveState(ossimKeywordlist& kwl,
                                      const char* prefix)const
{
   ossimString outputScalarType =
      ossimScalarTypeLut::instance()->getEntryString(theOutputScalarType);
   
   kwl.add(prefix,
           EQUATION_KW,
           theEquation.c_str(),
           true);

   kwl.add(prefix,
           "output_scalar_type",
           outputScalarType.c_str(),
           true);
   
   return ossimImageCombiner::saveState(kwl,
                                        prefix);
}

bool ossimEquationCombiner::loadState(const ossimKeywordlist& kwl,
                                      const char* prefix)
{
   const char* equ    = kwl.find(prefix, EQUATION_KW);
   const char* scalar = kwl.find(prefix, "output_scalar_type");
   
   bool result = ossimImageCombiner::loadState(kwl,
                                               prefix);
   
   if(equ)
   {
      theEquation = equ;
   }

   if(scalar)
   {      
      setOutputScalarType(ossimScalarTypeLut::instance()->
                          getScalarTypeFromString(scalar));
   }

   return result;
}

