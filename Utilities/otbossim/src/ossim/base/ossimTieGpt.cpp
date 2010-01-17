#include <iostream>
#include <iomanip>

#include <ossim/base/ossimTieGpt.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimNotifyContext.h>

//*******************************************************************

std::ostream& ossimTieGpt::print(std::ostream& os) const
{
   os << "( ";
   os << dynamic_cast<const ossimGpt&>(*this);
   os << ", ";
   os << tie;
   os << ", ";
   if (ossim::isnan(score) == false)
   {
      os << std::setiosflags(std::ios::fixed) << std::setprecision(15);
      os << score;
   } else {
      os << "nan";
   }
   os << " )";

   return os;
}

std::ostream& ossimTieGpt::printTab(std::ostream& os) const
{
   os << std::setiosflags(std::ios::fixed) << std::setprecision(15);
   os<< lond() ;
   os<<"\t"<< latd();
   os<<"\t"<< height();
   os<<"\t"<< tie.x;
   os<<"\t"<< tie.y;
   os<<"\t"<< score;

   return os;
}

std::ostream& operator<<(std::ostream& os, const ossimTieGpt& pt)
{
   return pt.print(os);
}

std::istream& operator>>(std::istream& is, ossimTieGpt& pt)
{
   //---
   // Expected input format:
   // ( (ossimDPt), (ossimDpt), score )
   // score is real or nan
   //---

   // Start with a nan point.
   pt.makeNan();
   // Check the stream.
   if (!is) return is;

   ossimString tempString;

   // Gobble the "(".
   is >> tempString;

   //get the first point
   is>>dynamic_cast<ossimGpt&>(pt);

   // Eat the ",".
   char c;
   is.get(c);

   //get the second point
   is>>pt.tie;

   // Eat the second ",".
   is.get(c);

   // Get the score
   const int SZ = 64; // real number buffer size
   char tempChars[SZ];
   is.get(tempChars, SZ, ',');
   if (!is) return is;
   tempChars[SZ-1] = '\0';
   tempString = tempChars;
   tempString.trim();
   if (tempString == "nan")
   {
      pt.score = ossim::nan();
   }
   else
   {
      pt.score = tempString.toDouble();
   }

   // Gobble the trailing ")".
   is >> tempString;
   
   // Finished
   return is;
}

//constants for GML 2.1.2
const char* GROUND_GML2      = "ground/gml:Point";
const char* IMAGE_GML2       = "image/gml:Point";
const char* SCORE_GML2       = "score";
const char* COORD_GML2       = "gml:coord";
const char* COORDINATES_GML2 = "gml:coordinates";

ossimRefPtr<ossimXmlNode>
ossimTieGpt::exportAsGmlNode(ossimString aGmlVersion)const
{   
   ossimRefPtr<ossimXmlNode> node(new ossimXmlNode);
   // check datum to be WGS84
   if ( !(datum()->operator==(*(ossimDatumFactory::instance()->wgs84()))) )
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimTieGpt::exportAsGmlNode datum must be WGS84\n";
      return node;
   }
   // check nans in lon/lat and in tie
   if (isLatNan() || isLonNan() || tie.hasNans())
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimTieGpt::exportAsGmlNode positions have nan\n";
      return node;
   }
   if (aGmlVersion[static_cast<std::string::size_type>(0)] == '2')
   {
      node->setTag("SimpleTiePoint");
      // note: no "fid" attribute (feature id")
      //store Ground Point WGS84 + height above ellipsoid in meters
      ossimRefPtr<ossimXmlNode> gcoord =  node->addNode(ossimString(GROUND_GML2) + "/" + COORD_GML2 );
      gcoord->addChildNode("X",ossimString::toString(lond()));
      gcoord->addChildNode("Y",ossimString::toString(latd()));
      if (!isHgtNan())
      {
        gcoord->addChildNode("Z",ossimString::toString(height())); //above ellipsoid
      }

      // store image tie point
      ossimRefPtr<ossimXmlNode> tcoord =  node->addNode(ossimString(IMAGE_GML2) + "/" + COORD_GML2);
      tcoord->addChildNode("X",ossimString::toString(tie.x));
      tcoord->addChildNode("Y",ossimString::toString(tie.y));

      //store score (change name to confidence?)
      node->addNode(SCORE_GML2,ossimString::toString(score));
   } else {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimTieGpt::exportAsGmlNode Unsupported GML version : " << aGmlVersion <<"\n";
   }
   return node;
}

bool
ossimTieGpt::importFromGmlNode(ossimRefPtr<ossimXmlNode> aGmlNode, ossimString aGmlVersion)
{
   //assuming datum is EPSG:4326 (aka WGS84)
   //feature has to be a SimpleTiePoint feature
   //TBD : add support for coord instead of coordinates
   //TBD : more robust type checks (for X,Y,Z and score) - create extra protected function

   //clear data
   makeNan();
   tie.makeNan();
   score = 0;

   if (aGmlVersion[static_cast<std::string::size_type>(0)] == '2')
   {
      //read ground point
      ossimRefPtr<ossimXmlNode> gn = aGmlNode->findFirstNode(GROUND_GML2);
      ossimRefPtr<ossimXmlNode> gcoord = gn->findFirstNode(COORD_GML2);
      if (gcoord.valid())
      {
         //read coord
         ossimRefPtr<ossimXmlNode> gx = gcoord->findFirstNode("X");
         if (gx.valid())
         {
            lond(ossimString(gx->getText()).toDouble());
         } else {
            ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimTieGpt::importFromGmlNode no ground X found in coord\n";
            return false;
         }
         ossimRefPtr<ossimXmlNode> gy = gcoord->findFirstNode("Y");
         if (gy.valid())
         {
            latd(ossimString(gy->getText()).toDouble());
         } else {
            ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimTieGpt::importFromGmlNode no ground Y found in coord\n";
            return false;
         }
         ossimRefPtr<ossimXmlNode> gz = gcoord->findFirstNode("Z");
         if (gz.valid())
         {
            height(ossimString(gz->getText()).toDouble());
         } // no Z value is possible
      }
      else {
         //try to read coordinates
         //TBD
         ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimTieGpt::importFromGmlNode gml:coordinates not developped yet for ground\n";
         return false;
      }

      //read image point
      ossimRefPtr<ossimXmlNode> in = aGmlNode->findFirstNode(IMAGE_GML2);
      ossimRefPtr<ossimXmlNode> icoord = in->findFirstNode(COORD_GML2);
      if (icoord.valid())
      {
         //read coord
         ossimRefPtr<ossimXmlNode> ix = icoord->findFirstNode("X");
         if (ix.valid())
         {
            tie.x = ossimString(ix->getText()).toDouble();
         } else {
            ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimTieGpt::importFromGmlNode no image X found in coord\n";
            return false;
         }
         ossimRefPtr<ossimXmlNode> iy = icoord->findFirstNode("Y");
         if (iy.valid())
         {
            tie.y = ossimString(iy->getText()).toDouble();
         } else {
            ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimTieGpt::importFromGmlNode no image Y found in coord\n";
            return false;
         }
         //don't read Z value (shouldn't be any)
      }
      else {
         //try to read coordinates
         //TBD
         ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimTieGpt::importFromGmlNode gml:coordinates not developped yet for image\n";
         return false;
      }

      //read score
      ossimRefPtr<ossimXmlNode> scoren = aGmlNode->findFirstNode(SCORE_GML2);
      if (scoren.valid())
      {
         score = ossimString(scoren->getText()).toDouble();
      } else {
         score = 0.0;
      }
      return true;
   } else {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimTieGpt::importFromGmlNode Unsupported GML version : " << aGmlVersion <<"\n";
      return false;
   }
}
