//*******************************************************************
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// class ossimImageGaussianFilter : tile source
//*******************************************************************
// $Id: ossimImageGaussianFilter.h,v 1.2 2005/09/12 13:39:07 gpotts Exp $
#ifndef ossimImageGaussianFilter_HEADER
#define ossimImageGaussianFilter_HEADER

#include <imaging/tile_sources/ossimImageSourceFilter.h>
#include <imaging/tile_sources/ossimConvolutionFilter1D.h>

class OSSIM_DLL ossimImageGaussianFilter : public ossimImageSourceFilter
{
public:
   //! own methods
   ossimImageGaussianFilter();
   virtual ~ossimImageGaussianFilter();

   inline ossim_float64 getGaussStd()const { return theGaussStd; }
   void setGaussStd(const ossim_float64& v);

   //! inherited methods
   virtual void initialize();
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect &tileRect,ossim_uint32 resLevel=0);

   virtual void connectInputEvent(ossimConnectionEvent &event);
   virtual void disconnectInputEvent(ossimConnectionEvent &event);

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   virtual bool loadState(const ossimKeywordlist& kwl,const char* prefix = 0);   
   virtual bool saveState(ossimKeywordlist& kwl,const char* prefix = 0)const;
   
protected:
   //! protected methods
   void initializeProcesses();
   void updateKernels();

   //! parameters
   ossim_float64 theGaussStd;

   //! subprocesses
   ossimConvolutionFilter1D* theHF; //horizontal filter
   ossimConvolutionFilter1D* theVF; //vertical filter

TYPE_DATA
};

#endif /* #ifndef ossimImageGaussianFilter_HEADER */
