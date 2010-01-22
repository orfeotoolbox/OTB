// ossimTDpt
// storage class for tie point (no height, no projection information)
// based on ossimDpt
// 
#ifndef ossimTDpt_HEADER
#define ossimTDpt_HEADER

#include <iostream>

#include <ossim/base/ossimDpt.h>

// Forward class declarations.
class ossimIpt;
class ossimFpt;
class ossimDpt3d;
class ossimGpt;

class OSSIMDLLEXPORT ossimTDpt : public ossimDpt
{
public:

   inline ossimTDpt() : ossimDpt(), tie(), score(0.0) {}

   inline ossimTDpt(const ossimDpt& aPt, const ossimDpt& aTie, const ossim_float64& aScore) : ossimDpt(aPt), tie(aTie), score(aScore) {}
         
   inline ossimTDpt(const ossimTDpt& tpt) : ossimDpt(tpt), tie(tpt.tie), score(tpt.score) {}

   const ossimTDpt& operator=(const ossimTDpt&);

   inline void            setMasterPoint(const ossimDpt& mPt) { ossimDpt::operator=(mPt); }
   inline const ossimDpt& getMasterPoint()const               { return *this; }
   inline       ossimDpt& refMasterPoint()                    { return *this; }

   inline void            setSlavePoint(const ossimDpt& sPt) { tie=sPt; }
   inline const ossimDpt& getSlavePoint()const               { return tie; }
   inline       ossimDpt& refSlavePoint()                    { return tie; }

   void makeNan() 
   {
      ossimDpt::makeNan();
      tie.makeNan();
      score=ossim::nan();
   }
   
   bool hasNans()const
   {
      return (ossimDpt::hasNans() || tie.hasNans() || (ossim::isnan(score)));
   }
   
   bool isNan()const
   {
      return (ossimDpt::isNan() && tie.isNan() && (ossim::isnan(score)));
   }

   std::ostream& print(std::ostream& os) const;
   std::ostream& printTab(std::ostream& os) const;

   friend OSSIMDLLEXPORT std::ostream& operator<<(std::ostream& os,
                                                  const ossimTDpt& pt);

   /**
    * Method to input the formatted string of the "operator<<".
    *
    * Expected format:  ( ( ossimDpt ), ( ossimDpt ), 0.50000000000000 )
    *                     --*this---- , ----tie-----, ---score--------
    * 
    */
   friend OSSIMDLLEXPORT std::istream& operator>>(std::istream& is,
                                                  ossimTDpt& pt);
   
   //***
   // Public data members:
   //***
   ossimDpt      tie;
   ossim_float64 score;
};

inline const ossimTDpt& ossimTDpt::operator=(const ossimTDpt& pt)
{
   if (this != &pt)
   {
      ossimDpt::operator=(pt);
      tie   = pt.tie;
      score = pt.score;
   }
   
   return *this;
}

#endif /* #ifndef ossimTDpt_HEADER */
