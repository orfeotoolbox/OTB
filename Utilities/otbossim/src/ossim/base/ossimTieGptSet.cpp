#include <iostream>
#include <iomanip>

#include <ossim/base/ossimTieGptSet.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimNotifyContext.h>

using namespace std;

const ossimTieGptSet&
ossimTieGptSet::operator=(const ossimTieGptSet& aSet)
{
   if (this != &aSet)
   {
      theTies       = aSet.getTiePoints();
      theMasterPath = aSet.getMasterPath();
      theSlavePath  = aSet.getSlavePath();
      theImageCov   = aSet.getImageCov();
      theGroundCov  = aSet.getGroundCov();
   }
   return *this;
}

void 
ossimTieGptSet::addTiePoint(ossimRefPtr<ossimTieGpt> aTiePt)
{
   theTies.push_back(aTiePt);
}

void 
ossimTieGptSet::clearTiePoints()
{
   theTies.clear();
}

std::ostream& ossimTieGptSet::printTab(std::ostream& os) const
{
   os<<"MasterPath: "<<getMasterPath()<<endl;
   os<<"SlavePath: "<<getSlavePath()<<endl;
   os<< std::setiosflags(std::ios::fixed) << std::setprecision(15);
   os<<"ImageCov: (2) " <<symMatrixToText(getImageCov()) <<endl;
   os<<"GroundCov: (3) "<<symMatrixToText(getGroundCov())<<endl;
   os<<"TiePoints: ("<<endl;
   for(vector<ossimRefPtr<ossimTieGpt> >::const_iterator it = theTies.begin(); it != theTies.end(); ++it)
   {
      (*it)->printTab(os);
      os<<endl;
   }
   os<<")"<<endl;
   return os;
}

//constants for XML tags
const char* SLAVEPATH_TAG  = "SlavePath";
const char* MASTERPATH_TAG = "MasterPath";
const char* IMAGECOV_TAG   = "ImageCovariance";
const char* GROUNDCOV_TAG  = "GroundCovariance";
const char* TIEPOINTS_TAG  = "SimpleTiePoint";

//exported constants
const char* ossimTieGptSet::TIEPTSET_TAG = "TiePointSet";

//export as XML/GML
ossimRefPtr<ossimXmlNode>
ossimTieGptSet::exportAsGmlNode(ossimString aGmlVersion)const
{   
   ossimRefPtr<ossimXmlNode> node(new ossimXmlNode);

   node->setTag(TIEPTSET_TAG);
   node->addAttribute("xmlns:gml","""http://www.opengis.net/gml"""); //namespace definition

   //add header information : general accuracy + path
   node->addChildNode(MASTERPATH_TAG,getMasterPath());
   node->addChildNode(SLAVEPATH_TAG,getSlavePath());
   node->addChildNode(IMAGECOV_TAG ,symMatrixToText(getImageCov()));
   node->addChildNode(GROUNDCOV_TAG,symMatrixToText(getGroundCov()));

   //add all tiepoints
   for(vector<ossimRefPtr<ossimTieGpt> >::const_iterator it = theTies.begin(); it != theTies.end(); ++it)
   {
      ossimRefPtr<ossimXmlNode> tienode = (*it)->exportAsGmlNode(aGmlVersion);
      node->addChildNode(tienode.get());
      //TBD : add attribute / counter?
   }

   return node;
}

//import from XML/GML
bool
ossimTieGptSet::importFromGmlNode(ossimRefPtr<ossimXmlNode> aGmlNode, ossimString aGmlVersion)
{
   clearTiePoints();
   //load master path
   ossimRefPtr<ossimXmlNode> mpn = aGmlNode->findFirstNode(MASTERPATH_TAG);
   if (mpn.valid())
   {
      setMasterPath(mpn->getText());
   } else {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimTieGptSet::importFromGmlNode no "<<MASTERPATH_TAG<<" tag found\n";
      //not an error
      setMasterPath("");
   }
   //load slave path
   ossimRefPtr<ossimXmlNode> spn = aGmlNode->findFirstNode(SLAVEPATH_TAG);
   if (spn.valid())
   {
      setSlavePath(spn->getText());
   } else {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimTieGptSet::importFromGmlNode no "<<SLAVEPATH_TAG<<" tag found\n";
      //not an error
      setSlavePath("");
   }

   //load image covariance
   ossimRefPtr<ossimXmlNode> icn = aGmlNode->findFirstNode(IMAGECOV_TAG);
   if (icn.valid())
   {
      NEWMAT::SymmetricMatrix icm = textToSymMatrix(icn->getText(),2);
      if (icm.Nrows()==2)
      {
         setImageCov(icm);
      } else {
         ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimTieGptSet::importFromGmlNode bad image covariance matrix\n";
         return false;
      }
   } else {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimTieGptSet::importFromGmlNode tag "<<IMAGECOV_TAG<<" not found\n";
      //not an error
      setImageCov(NEWMAT::SymmetricMatrix(2)); //maybe use no matrix at all when no info ? TBC
   }

   //load ground covariance
   ossimRefPtr<ossimXmlNode> gcn = aGmlNode->findFirstNode(GROUNDCOV_TAG);
   if (gcn.valid())
   {
      NEWMAT::SymmetricMatrix gcm = textToSymMatrix(gcn->getText(),3);
      if (gcm.Nrows()==3)
      {
         setGroundCov(gcm);
      } else {
         ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimTieGptSet::importFromGmlNode bad ground covariance matrix\n";
         return false;
      }
   } else {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimTieGptSet::importFromGmlNode tag "<<GROUNDCOV_TAG<<" not found\n";
      //not an error
      setGroundCov(NEWMAT::SymmetricMatrix(3)); //TBC : is it 0 matrix / should we use no matrix?
   }

   //load all tie points (skip errors but report them)
   vector< ossimRefPtr< ossimXmlNode > > tienodes;
   aGmlNode->findChildNodes(TIEPOINTS_TAG, tienodes);
   if (tienodes.size() <= 0)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimTieGptSet::importFromGmlNode no tag "<<TIEPOINTS_TAG<<" found\n";
      return false;
   }
   
   int badtiecount=0;
   
   for(vector< ossimRefPtr< ossimXmlNode > >::iterator it=tienodes.begin(); it!=tienodes.end(); ++it)
   {
      ossimRefPtr<ossimTieGpt> temp(new ossimTieGpt);
      if (temp->importFromGmlNode(*it,aGmlVersion)) //pointer hacks nor beautiful nor direct
      {
         addTiePoint(temp);
      } else {
         badtiecount++;
      }
   }
   if (badtiecount>0)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimTieGptSet::importFromGmlNode failed to import "<<badtiecount<<" tie point(s)\n";
   }
   return true;
}

ossimString
ossimTieGptSet::symMatrixToText(
   const NEWMAT::SymmetricMatrix& sym, 
   const ossimString& el_sep,
   const ossimString& row_sep)const
{
   // write lower half matrix (easier to see than upper half when left justifying)
   // separate elements and rows 
   ossimString res = row_sep;
   for (int i=1;i<=sym.Nrows();++i) //indices start at 1
   {
      for (int j=1;j<=i;++j)
      {
         if (j!=1) res += el_sep;
         res += ossimString::toString(sym(i,j));
      }
      res += row_sep;
   }
   return res;
}


NEWMAT::SymmetricMatrix
ossimTieGptSet::textToSymMatrix(
   const ossimString& text,
   unsigned int   dim,
   const ossimString& seps)const
{
   //sep can hold multiple possible separators characters
   //we don't know the matrix size yet, so we put everything into a buffer
   vector<double> buffer;

   vector<ossimString> vsv = text.explode(seps);
   for(vector<ossimString>::const_iterator vit=vsv.begin(); vit!=vsv.end(); ++vit)
   {
      if (vit->size() > 0)
      {
         buffer.push_back(vit->toDouble());
      }
   }

   //check number of elements
   if (buffer.size() != (dim*(dim+1))/2)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimTieGptSet::textToSymMatrix wrong element number in sym. matrix : " << buffer.size() <<"\n";
      return NEWMAT::SymmetricMatrix();
   }

   //populate lower half sym matrix
   vector<double>::const_iterator it = buffer.begin();
   NEWMAT::SymmetricMatrix sym(dim);
   for(unsigned int i=1;i<=dim;++i)
   {
      for(unsigned int j=1;j<=i;++j)
      {
         sym(i,j) = *(it++);
      }
   }
   return sym;
}

void
ossimTieGptSet::getSlaveMasterPoints(
  std::vector<ossimDpt>& imv, 
  std::vector<ossimGpt>& gdv)const
{
   //re-dim
   imv.resize(theTies.size());
   gdv.resize(theTies.size());

   //fill
   std::vector<ossimDpt>::iterator imvit = imv.begin();
   std::vector<ossimGpt>::iterator gdvit = gdv.begin();

   for(vector<ossimRefPtr<ossimTieGpt> >::const_iterator it = theTies.begin(); it != theTies.end(); ++it,++imvit,++gdvit)
   {
      *imvit = (*it)->getImagePoint();
      *gdvit = (*it)->getGroundPoint();
   }
}

void
ossimTieGptSet::getGroundBoundaries(ossimGpt& gBoundInf, ossimGpt& gBoundSup)const
{
   //init
   gBoundInf.lat = gBoundInf.lon = gBoundInf.hgt = OSSIM_DEFAULT_MAX_PIX_DOUBLE;
   gBoundSup.lat = gBoundSup.lon = gBoundSup.hgt = OSSIM_DEFAULT_MIN_PIX_DOUBLE;

   //return image and/or ground bounds
   for(vector<ossimRefPtr<ossimTieGpt> >::const_iterator it = theTies.begin(); it != theTies.end(); ++it)
   {
      const ossimTieGpt& gp = *(*it);
      if (gp.lon > gBoundSup.lon) gBoundSup.lon = gp.lon;
      if (gp.lon < gBoundInf.lon) gBoundInf.lon = gp.lon;
      if (gp.lat > gBoundSup.lat) gBoundSup.lat = gp.lat;
      if (gp.lat < gBoundInf.lat) gBoundInf.lat = gp.lat;
      if (ossim::isnan(gp.hgt) == false)
      {
         if (gp.hgt > gBoundSup.hgt) gBoundSup.hgt = gp.hgt;
         if (gp.hgt < gBoundInf.hgt) gBoundInf.hgt = gp.hgt;
      } 
   }
}
