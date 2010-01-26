#ifndef ossimImageStatistics_HEADER
#define ossimImageStatistics_HEADER
#include <ossim/base/ossimSource.h>

class OSSIMDLLEXPORT ossimImageStatisticsSource : public ossimSource
{
public:
   ossimImageStatisticsSource();
   
   
   virtual void computeStatistics();
   
   virtual bool canConnectMyInputTo(ossim_int32 inputIndex,
                                    const ossimConnectableObject* object)const;
   
   const std::vector<ossim_float64>& getMean()const;
   const std::vector<ossim_float64>& getMin()const;
   const std::vector<ossim_float64>& getMax()const;
   
protected:
   virtual ~ossimImageStatisticsSource();
   void clearStatistics();
   template <class T>
   void computeStatisticsTemplate(T dummyVariable);
   void setStatsSize(ossim_uint32 size);
   
   std::vector<ossim_float64> theMean;
   std::vector<ossim_float64> theMin;
   std::vector<ossim_float64> theMax;
};

#endif
