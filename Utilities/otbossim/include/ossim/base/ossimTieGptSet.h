#ifndef ossimTieGptSet_HEADER
#define ossimTieGptSet_HEADER

#include <iostream>
#include <vector>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/base/ossimTieGpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/matrix/newmat.h>

class ossimDpt;

/**
 * storage class for a set of geographic tie points, between master and slave images
 * + GML (OGC) serialization
 *
 * NOTES
 * master points are stored on the ground
 * ground SRS is EPSG:4326 only (WGS84) + height in meters above ellispoid
 *
 * slave points are stored as image positions
 *
 * general ground/image accuracy values are stored
 *
 * TODO :
 */
class OSSIMDLLEXPORT ossimTieGptSet
{
public:

   inline ossimTieGptSet() {}

   inline ossimTieGptSet(const ossimTieGptSet& aSet)
      :
      theTies(aSet.getTiePoints()),
      theMasterPath(aSet.getMasterPath()),
      theSlavePath(aSet.getSlavePath()),
      theImageCov(aSet.getImageCov()),
      theGroundCov(aSet.getGroundCov())
   {}

   inline ~ossimTieGptSet() {}

   const ossimTieGptSet& operator=(const ossimTieGptSet&);

   // accessors   
   inline void  setTiePoints(const vector<ossimRefPtr<ossimTieGpt> >& aTieSet) { theTies = aTieSet; }
   inline const vector<ossimRefPtr<ossimTieGpt> >& getTiePoints()const         { return theTies; }
   inline       vector<ossimRefPtr<ossimTieGpt> >& refTiePoints()              { return theTies; }

   inline void  setMasterPath(const ossimString& aPath) { theMasterPath = aPath; }
   inline const ossimString& getMasterPath()const       { return theMasterPath; }

   inline void  setSlavePath(const ossimString& aPath) { theSlavePath = aPath; }
   inline const ossimString& getSlavePath()const       { return theSlavePath; }

   inline void  setImageCov(const NEWMAT::SymmetricMatrix& aCovMat) { theImageCov = aCovMat; }
   inline const NEWMAT::SymmetricMatrix& getImageCov()const       { return theImageCov; }
   inline       NEWMAT::SymmetricMatrix& refImageCov()            { return theImageCov; }

   inline void  setGroundCov(const NEWMAT::SymmetricMatrix& aCovMat) { theGroundCov = aCovMat; }
   inline const NEWMAT::SymmetricMatrix& getGroundCov()const       { return theGroundCov; }
   inline       NEWMAT::SymmetricMatrix& refGroundCov()            { return theGroundCov; }
  
   void getSlaveMasterPoints(std::vector<ossimDpt>& imv, std::vector<ossimGpt>& gdv)const;

   inline unsigned int size()const { return (unsigned int)theTies.size(); }

   /**
    * operations
    */
   void addTiePoint(ossimRefPtr<ossimTieGpt> aTiePt);
   void clearTiePoints();

   void getGroundBoundaries(ossimGpt& gBoundInf, ossimGpt& gBoundSup)const;

   /**
    * text output : header + tab separated tie points
    */
   std::ostream& printTab(std::ostream& os) const;

   /**
    * GML features (XML) serialization
    */
   ossimRefPtr<ossimXmlNode> exportAsGmlNode(ossimString aGmlVersion="2.1.2")const;
   bool importFromGmlNode(ossimRefPtr<ossimXmlNode> aGmlNode, ossimString aGmlVersion="2.1.2");
   
  /**
   * Public data members
   */
   static const char* TIEPTSET_TAG;

protected:
   /**
    * Protected data members
    */
   std::vector<ossimRefPtr<ossimTieGpt> > theTies;      //store by reference so derived classes can be used
   ossimString                       theMasterPath; //!full or relative path to master dataset
   ossimString                       theSlavePath; //!full or relative path to slave dataset
   NEWMAT::SymmetricMatrix           theImageCov;  //! image error covariance matrix
   NEWMAT::SymmetricMatrix           theGroundCov; //! ground error covariance matrix

   /**
    * Protected methods
    */
   ossimString symMatrixToText(const NEWMAT::SymmetricMatrix& sym, 
                               const ossimString& el_sep=" ",
                               const ossimString& row_sep=";")const;

   NEWMAT::SymmetricMatrix textToSymMatrix(const ossimString& text,
                               unsigned int dim,
                               const ossimString& seps=" ;\t\r\n")const; //list of possible elements or row separators

};

#endif /* #ifndef ossimTieGptSet_HEADER */
