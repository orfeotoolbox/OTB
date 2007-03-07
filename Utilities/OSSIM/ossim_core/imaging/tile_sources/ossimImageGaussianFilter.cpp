//*******************************************************************
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// class ossimImageGaussianFilter : tile source
//*******************************************************************
// $Id: ossimImageGaussianFilter.cpp,v 1.2 2005/09/13 10:46:50 gpotts Exp $
#include <cmath>
#include <imaging/tile_sources/ossimImageGaussianFilter.h>
#include <base/property/ossimNumericProperty.h>

RTTI_DEF1(ossimImageGaussianFilter, "ossimImageGaussianFilter", ossimImageSourceFilter);

//! property names
static const char* PROPERTYNAME_GAUSSSTD      = "GaussStd";

ossimImageGaussianFilter::ossimImageGaussianFilter()
   : ossimImageSourceFilter(),
     theGaussStd(0.5)
{
   // ingredients: 
   // 2x  ConvolutionFilter1D
   theHF=new ossimConvolutionFilter1D();
   theVF=new ossimConvolutionFilter1D();

   theHF->setIsHorizontal(true);
   theVF->setIsHorizontal(false);
   updateKernels();

   //tie them up
   theVF->connectMyInputTo(0,theHF);
}

ossimImageGaussianFilter::~ossimImageGaussianFilter()
{
   delete theHF;
   delete theVF;
}

void ossimImageGaussianFilter::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property) return;

   if (property->getName() == PROPERTYNAME_GAUSSSTD) {
      ossimNumericProperty* sProperty = PTR_CAST(ossimNumericProperty,
                                                     property.get());
      if(sProperty)
      {
         setGaussStd(sProperty->asFloat64());
      }
   } else {
      ossimImageSourceFilter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimImageGaussianFilter::getProperty(const ossimString& name)const
{
   if (name == PROPERTYNAME_GAUSSSTD) {
      ossimNumericProperty* property = new ossimNumericProperty(name,ossimString::toString(getGaussStd()),1e-13,100.0);
      property->setCacheRefreshBit();
      return property;
   }
   return ossimImageSourceFilter::getProperty(name);
}

void ossimImageGaussianFilter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);
   propertyNames.push_back(PROPERTYNAME_GAUSSSTD);
}

bool ossimImageGaussianFilter::saveState(ossimKeywordlist& kwl,
                                          const char* prefix)const
{   
   kwl.add(prefix,
           PROPERTYNAME_GAUSSSTD,
           theGaussStd,
           true);   
   
   return ossimImageSourceFilter::saveState(kwl, prefix);
}


bool ossimImageGaussianFilter::loadState(const ossimKeywordlist& kwl,
                                          const char* prefix)
{
   const char* gs = kwl.find(prefix, PROPERTYNAME_GAUSSSTD);
   if(gs)
   {      
      setGaussStd(ossimString(gs).toUInt32());
   } else {
      cerr<<"ossimImageGaussianFilter : warning no "<< PROPERTYNAME_GAUSSSTD<<" in loadState"<<endl;
   }
   
   return ossimImageSourceFilter::loadState(kwl, prefix);
}

void ossimImageGaussianFilter::setGaussStd(const ossim_float64& v)
{
   theGaussStd = v;
   updateKernels();
}

void
ossimImageGaussianFilter::initialize()
{
   ossimImageSourceFilter::initialize();
   initializeProcesses();
}

ossimRefPtr<ossimImageData>
ossimImageGaussianFilter::getTile(const ossimIrect &tileRect,ossim_uint32 resLevel)
{
    if(isSourceEnabled())
    {
       return theVF->getTile(tileRect, resLevel);
    }
    if(theInputConnection)
    {
       return theInputConnection->getTile(tileRect, resLevel);
    }

    return 0;
}

void
ossimImageGaussianFilter::initializeProcesses()
{
   theHF->initialize();
   theVF->initialize();
}

void
ossimImageGaussianFilter::connectInputEvent(ossimConnectionEvent &event)
{
    ossimImageSourceFilter::connectInputEvent(event);
    if(getInput())
    {
       theHF->connectMyInputTo(0, getInput());
       initializeProcesses();
    }
    else
    {
       theHF->disconnectMyInput(0, false, false);
       initializeProcesses();
    }
}

void
ossimImageGaussianFilter::disconnectInputEvent(ossimConnectionEvent &event)
{
    ossimImageSourceFilter::disconnectInputEvent(event);
    if(getInput())
    {
       theHF->connectMyInputTo(0, getInput());
       initializeProcesses();
    }
    else
    {
       theHF->disconnectMyInput(0, false, false);
       initializeProcesses();
    }
}
void
ossimImageGaussianFilter::updateKernels()
{
   //update kernels based on GaussStd value :2.5 sigma on each side
   // symetric kernel
   static const ossim_float64 sigmaN = 2.5;
   ossim_float64 sig22   = getGaussStd()*getGaussStd()*2.0;

   ossim_uint32 halfw   = (ossim_uint32)(std::floor(getGaussStd() * sigmaN + 0.5));
   ossim_uint32 supsize = 2*halfw + 1;

   //fill with sym. gaussian (unnormalized)
   vector<ossim_float64> newk(supsize);
   ossim_float64 sum=1.0;
   ossim_float64 v;
   newk[halfw] = 1.0;
   for(ossim_int32 i=(ossim_int32)halfw; i>0 ;--i) //reverse for summing
   {
      newk[halfw + i] = newk[halfw - i] = v = std::exp(-i*i/sig22);
      sum += 2.0 * v;
   }

   //normalize
   ossim_float64   invsum=1.0/sum;
   for(ossim_uint32 i=0; i<supsize ;++i)
   {
      newk[i] *= invsum;
   }

   //send to 1d conv filters
   theHF->setKernel(newk);
   theVF->setKernel(newk);
   theHF->setCenterOffset(halfw);
   theVF->setCenterOffset(halfw);
}
