#ifndef ossimTieGpt_HEADER
#define ossimTieGpt_HEADER

#include <iostream>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimReferenced.h>

class ossimDpt;

/**
 * storage class for tie point between ground and image
 * based on ossimGpt
 * + GML feature (OGC) serialization
 *
 * NOTES
 * accuracy is not stored here (need to derive object if need a per-point accuracy) 
 * GML storage is WGS84 only, it stores above ellipsoid height (m)
 *
 * TODO : 
 * -support other datum (easy) / ground projection (big change in OSSIM)
 * -unify with ossimTDpt
 */
class OSSIMDLLEXPORT ossimTieGpt :  
                              public ossimReferenced,
                              public ossimGpt
{
public:

   inline ossimTieGpt() : 
      ossimReferenced(),
      ossimGpt(),
      tie(),
      score(0.0)
      {}

   inline ossimTieGpt(const ossimGpt& aPt, const ossimDpt& aTie, const ossim_float64& aScore) : 
      ossimReferenced(),
      ossimGpt(aPt),
      tie(aTie),
      score(aScore)
      {}
         
   inline ossimTieGpt(const ossimTieGpt& tpt) : 
      ossimReferenced(tpt),
      ossimGpt(tpt),
      tie(tpt.tie),
      score(tpt.score)
      {}
   inline ~ossimTieGpt() {}

   const ossimTieGpt& operator=(const ossimTieGpt&);

   inline void            setGroundPoint(const ossimGpt& mPt) { ossimGpt::operator=(mPt); }
   inline const ossimGpt& getGroundPoint()const               { return *this; }
   inline       ossimGpt& refGroundPoint()                    { return *this; }

   inline void            setImagePoint(const ossimDpt& sPt) { tie=sPt; }
   inline const ossimDpt& getImagePoint()const               { return tie; }
   inline       ossimDpt& refImagePoint()                    { return tie; }

   inline void            setScore(const ossim_float64& s) { score=s; }
   inline const ossim_float64& getScore()const             { return score; }
   inline       ossim_float64& refScore()                  { return score; }

   void makeNan() 
      {
       ossimGpt::makeNan();
       tie.makeNan();
       score=ossim::nan();
      }
   
   bool hasNans()const
   {
      return (ossimGpt::hasNans() || tie.hasNans() || (ossim::isnan(score)));
   }
   
   bool isNan()const
   {
      return (ossimGpt::isNan() && tie.isNan() && (ossim::isnan(score)));
   }
   /**
    * text serialization
    */
   std::ostream& print(std::ostream& os) const;
   std::ostream& printTab(std::ostream& os) const;

   friend OSSIMDLLEXPORT std::ostream& operator<<(std::ostream& os,
                                                  const ossimTieGpt& pt);
   /**
    * Method to input the formatted string of the "operator<<".
    *
    * Expected format:  ( ( ossimGpt ), ( ossimDpt ), 0.50000000000000 )
    *                     --*this---- , ----tie-----, ---score--------
    * 
    */
   friend OSSIMDLLEXPORT std::istream& operator>>(std::istream& is,
                                                  ossimTieGpt& pt);
   /**
    * GML feauture (XML) serialization
    */
   ossimRefPtr<ossimXmlNode> exportAsGmlNode(ossimString aGmlVersion="2.1.2")const;
   bool importFromGmlNode(ossimRefPtr<ossimXmlNode> aGmlNode, ossimString aGmlVersion="2.1.2");
   
   /**
    * Public data members
    */
   ossimDpt      tie;
   ossim_float64 score;
};

inline const ossimTieGpt& ossimTieGpt::operator=(const ossimTieGpt& pt)
{
   if (this != &pt)
   {
      ossimReferenced::operator=(pt);
      ossimGpt::operator=(pt);
      tie   = pt.tie;
      score = pt.score;
   }
   
   return *this;
}

#endif /* #ifndef ossimTieGpt_HEADER */
