//*****************************************************************************
//
// License:  See top level LICENSE.txt file.
//
// AUTHOR: Garrett Potts
//
//*****************************************************************************
//  $Id: ossimActiveEdgeTable.h 14799 2009-06-30 08:54:44Z dburken $
#include <iostream>
#include <vector>
#include <list>
#include <ossim/base/ossimIrect.h>

class ossimPolygon;


class OSSIMDLLEXPORT ossimActiveEdgeTableNode
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimActiveEdgeTableNode& rhs)
      {
          return out <<"| maxy="<<rhs.theMaxY<<" |dx="
                     <<rhs.theSlope<<" |"<<"x="
                     <<rhs.theCurrentX << "|"<< std::endl;
//          return out <<"|dy="<<rhs.theDeltaY<<" |dx="
//                     <<rhs.theSlope<<" |"<<"x="
//                     <<rhs.theCurrentX << "|"<<endl;
      }
   ossimActiveEdgeTableNode(ossim_int32 maxY      = 0,
                            double slope    = 0.0,
                            double currentX = 0.0)
//       :theDeltaY(deltaY),
      :theMaxY(maxY),
       theSlope(slope),
       theCurrentX(currentX)
      {
      }
   bool operator()(const ossimActiveEdgeTableNode& left,
                 const ossimActiveEdgeTableNode& right)const
      {
         return (left.theCurrentX < right.theCurrentX);
      }
   bool operator<(const ossimActiveEdgeTableNode& right)const
      {
         return (theCurrentX < right.theCurrentX);
      }
   bool operator>(const ossimActiveEdgeTableNode& right)const
      {
         return (theCurrentX > right.theCurrentX);
      }
                            
   ossim_int32  theMaxY;
//   ossim_int32  theDeltaY;
   double theSlope; 
   double theCurrentX;
};

class ossimActiveEdgeTableHelper;
/*!
 * This class is used in fast efficient scanliine rasterization.  It will take a polygon and insert
 * it into the table sorted by y's
 */
class ossimActiveEdgeTable
{
public:
   friend class ossimActiveEdgeTableHelper;
   ossimActiveEdgeTable();

   /*!
    * Currently will only work on a single polygon.  If you call this
    * twice it currently will use the last called polygon and
    * will wipe out the previous one.
    * 
    * Note: this is used for scanline rasterization and will round
    * to integer values all vertices as they are initially added
    * to the Active Edge Table.
    */
   void addPolygon(const ossimPolygon& polygon);

   const std::list<ossimActiveEdgeTableNode>& getActiveList()const;
   
//   bool getNextScanline(list<ossimActiveEdgeTable>::const_iterator& iterator)const;
   
   ossim_int32 getCurrentScanLine()const
      {
         return theCurrentScanLine;
      }
   ossim_int32 getYShift()const
      {
         return theRectOrigin.y;
      }
   
   void initializeActiveList();
   void updateActiveEdges();
   void mergeCurrentScanLine();
   void printActiveEdgeList()const;
   
protected:
   std::vector< std::list<ossimActiveEdgeTableNode> > theTable;
   std::list<ossimActiveEdgeTableNode> theActiveList;
   
   void createTable(const ossimPolygon& polygon);
   void addEdgeToTable(const ossimActiveEdgeTableNode& edge,
                       ossim_int32 scanLine);
   /*!
    * Used in computing the number of scanlines of the passed in polygon and
    * is also used to shift the y's relative to 0,0
    */
   ossimIrect  theBoundingRect;
   ossimIpt    theRectOrigin;
   ossim_int32 theLastScanLine;
   ossim_int32 theCurrentScanLine;
};

class ossimActiveEdgeTableHelper
{
public:
   ossimActiveEdgeTableHelper(ossimActiveEdgeTable* edgeTable);

   bool advanceScanLine();

   bool getNextLineSegment(ossimIpt& start,
                           ossimIpt& end);
   
   ossim_int32 getCurrentScanLine()const
      {
         if(theEdgeTable)
         {
            return theEdgeTable->getCurrentScanLine();
         }
         return 0;
      }
   ossim_int32 getYShift()const
      {
         if(theEdgeTable)
         {
            return theEdgeTable->getYShift();
         }
         return 0;         
      }
protected:
   ossimActiveEdgeTable* theEdgeTable;
   bool theTableInitializedFlag;

   std::list<ossimActiveEdgeTableNode>::const_iterator theIterator;
};
