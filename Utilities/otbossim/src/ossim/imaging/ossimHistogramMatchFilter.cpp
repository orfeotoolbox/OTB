#include <ossim/imaging/ossimHistogramMatchFilter.h>
#include <ossim/base/ossimFilenameProperty.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/base/ossimVisitor.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageHandler.h>

RTTI_DEF1(ossimHistogramMatchFilter, "ossimHistogramMatchFilter", ossimImageSourceFilter);
ossimHistogramMatchFilter::ossimHistogramMatchFilter()
   :ossimImageSourceFilter(),
    theAutoLoadInputHistogramFlag(false)
{
   theInputHistogramEqualizer = new ossimHistogramEqualization;
   theTargetHistogramEqualizer = new ossimHistogramEqualization;

   theInputHistogramEqualizer->setInverseFlag(false);
   theTargetHistogramEqualizer->setInverseFlag(true);
   theTargetHistogramEqualizer->connectMyInputTo(0, theInputHistogramEqualizer.get());
}

ossimRefPtr<ossimImageData> ossimHistogramMatchFilter::getTile(const ossimIrect& tileRect,
                                                               ossim_uint32 resLevel)
{
   if(isSourceEnabled())
   {
      return theTargetHistogramEqualizer->getTile(tileRect, resLevel);
   }
   if(theAutoLoadInputHistogramFlag&&
      (theInputHistogramFilename==""))
   {
      autoLoadInputHistogram();
   }
   if(theInputConnection)
   {
      return theInputConnection->getTile(tileRect, resLevel);
   }

   return 0;
}


void ossimHistogramMatchFilter::setInputHistogram(const ossimFilename& inputHistogram)
{
   theInputHistogramFilename = inputHistogram;
   theInputHistogramEqualizer->setHistogram(inputHistogram);
   theInputHistogramEqualizer->initialize();
}

void ossimHistogramMatchFilter::setTargetHistogram(const ossimFilename& targetHistogram)
{
   theTargetHistogramFilename = targetHistogram;
   theTargetHistogramEqualizer->setHistogram(targetHistogram);
   theTargetHistogramEqualizer->initialize();
}

void ossimHistogramMatchFilter::connectInputEvent(ossimConnectionEvent& event)
{
   ossimImageSourceFilter::connectInputEvent(event);
   if(getInput())
   {
      theInputHistogramEqualizer->connectMyInputTo(0, getInput());
      theInputHistogramEqualizer->initialize();
      theTargetHistogramEqualizer->initialize();
   }
   else
   {
      theInputHistogramEqualizer->disconnectMyInput(0, false, false);
      theInputHistogramEqualizer->initialize();
      theTargetHistogramEqualizer->initialize();
   }
}

void ossimHistogramMatchFilter::disconnectInputEvent(ossimConnectionEvent& event)
{
   ossimImageSourceFilter::disconnectInputEvent(event);
   if(getInput())
   {
      theInputHistogramEqualizer->connectMyInputTo(0, getInput());
      theInputHistogramEqualizer->initialize();
      theTargetHistogramEqualizer->initialize();
   }
   else
   {
      theInputHistogramEqualizer->disconnectMyInput(0, false, false);
      theInputHistogramEqualizer->initialize();
      theTargetHistogramEqualizer->initialize();
   }
}

void ossimHistogramMatchFilter::initialize()
{
   ossimImageSourceFilter::initialize();
   if(theAutoLoadInputHistogramFlag)
   {
      if(theInputHistogramFilename == "")
      {
         autoLoadInputHistogram();
      }
   }

   theInputHistogramEqualizer->initialize();
   theTargetHistogramEqualizer->initialize();
}

void ossimHistogramMatchFilter::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property.valid()) return;
   
   ossimString name = property->getName();
   if(name == "input_histogram_filename")
   {
      setInputHistogram(ossimFilename(property->valueToString()));
      return;
   }
   else if(name == "target_histogram_filename")
   {
      setTargetHistogram(ossimFilename(property->valueToString()));
      return;
   }
   else if(name == "auto_load_input_histogram_flag")
   {
      theAutoLoadInputHistogramFlag = property->valueToString().toBool();
      if(theAutoLoadInputHistogramFlag)
      {
         initialize();
      }
      return;
   }
   ossimImageSourceFilter::setProperty(property);
}

ossimRefPtr<ossimProperty> ossimHistogramMatchFilter::getProperty(const ossimString& name)const
{
   if(name == "input_histogram_filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, theInputHistogramFilename);
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      filenameProp->setCacheRefreshBit();
      return filenameProp;
   }
   else if(name == "target_histogram_filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, theTargetHistogramFilename);
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      filenameProp->setCacheRefreshBit();

      return filenameProp;
   }
   if(name == "auto_load_input_histogram_flag")
   {
      ossimBooleanProperty* boolProp = new ossimBooleanProperty(name,
                                                                theAutoLoadInputHistogramFlag);
      boolProp->setCacheRefreshBit();
      return boolProp;
   }
   return ossimImageSourceFilter::getProperty(name);
}

void ossimHistogramMatchFilter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);
   
   propertyNames.push_back("auto_load_input_histogram_flag");
   propertyNames.push_back("input_histogram_filename");
   propertyNames.push_back("target_histogram_filename");
}

bool ossimHistogramMatchFilter::loadState(const ossimKeywordlist& kwl,
                                          const char* prefix)
{
   const char* inputFilename     = kwl.find(prefix, "input_histogram_filename");
   const char* targetFilename    = kwl.find(prefix, "target_histogram_filename");
   const char* autoLoadInputFlag = kwl.find(prefix, "auto_load_input_histogram_flag");

   if(autoLoadInputFlag)
   {
      theAutoLoadInputHistogramFlag = ossimString(autoLoadInputFlag).toBool();
   }
   
   if(inputFilename&&!theAutoLoadInputHistogramFlag)
   {
      setInputHistogram(ossimFilename(inputFilename));
   }
   else
   {
      setInputHistogram(ossimFilename(""));
   }

   if(targetFilename)
   {
      setTargetHistogram(ossimFilename(targetFilename));
   }
   else
   {
      setTargetHistogram(ossimFilename(""));
   }
   
   return ossimImageSourceFilter::loadState(kwl, prefix);
}

bool ossimHistogramMatchFilter::saveState(ossimKeywordlist& kwl,
                                          const char* prefix)const
{
   if(theAutoLoadInputHistogramFlag)
   {
      kwl.add(prefix,
              "input_histogram_filename",
              "",
              true);
   }
   else
   {
      kwl.add(prefix,
              "input_histogram_filename",
              theInputHistogramFilename.c_str(),
              true);
   }
   kwl.add(prefix,
           "target_histogram_filename",
           theTargetHistogramFilename.c_str(),
           true);
   kwl.add(prefix,
           "auto_load_input_histogram_flag",
           theAutoLoadInputHistogramFlag,
           true);
   

   return ossimImageSourceFilter::saveState(kwl, prefix);
}

void ossimHistogramMatchFilter::autoLoadInputHistogram()
{
   ossimTypeNameVisitor visitor(ossimString("ossimImageHandler"),
                                true,
                                ossimVisitor::VISIT_CHILDREN|ossimVisitor::VISIT_INPUTS);
   accept(visitor);
   
   if ( visitor.getObjects().size() )
   {
      ossimRefPtr<ossimImageHandler> ih = visitor.getObjectAs<ossimImageHandler>( 0 );
      if ( ih.valid() )
      {
         ossimRefPtr<ossimProperty> prop = ih->getProperty("histogram_filename");
         if( prop.valid() )
         {
            ossimFilename inputHisto = ossimFilename(prop->valueToString());
            if( inputHisto.exists() )
            {
               setInputHistogram(inputHisto);
            }
         }
      }
   }
}
