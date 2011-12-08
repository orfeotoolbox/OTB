//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef RefPoint_h
#define RefPoint_h

#include <ossimPluginConstants.h>

class ossimKeywordlist;

namespace ossimplugins
{


class Ephemeris;


/**
 * @brief This class handles the referential point
 */
class OSSIM_PLUGINS_DLL RefPoint
{
public:
   /**
    * @brief Constuctor
    */
   RefPoint();
   /**
    * @brief Destructor
    */
   ~RefPoint();
   /**
    * @brief Copy constructor
    */
   RefPoint(const RefPoint& rhs);
   /**
    * @brief Affectation operator
    */
   RefPoint& operator=(const RefPoint& rhs);

   void set_ephemeris(Ephemeris* ephemeris);
   void set_distance(double distance);
   void set_pix_line(double pix_line);
   void set_pix_col(double pix_col);

   Ephemeris* get_ephemeris();
   double get_distance() const;
   double get_pix_line() const;
   double get_pix_col() const;

   RefPoint* Clone()
   {
      return new RefPoint(*this);
   };

   /**
    * @brief Method to save object state to a keyword list.
    * @param kwl Keyword list to save to.
    * @param prefix added to keys when saved.
    * @return true on success, false on error.
    */
   bool saveState(ossimKeywordlist& kwl, const char* prefix=0) const;

   /**
    * @brief Method to the load (recreate) the state of the object from a
    * keyword list. Return true if ok or false on error.
    * @return true if load OK, false on error
    */
   bool loadState (const ossimKeywordlist &kwl, const char *prefix=0);

protected:
   Ephemeris* _ephemeris;
   double _distance;
   double _pix_line;
   double _pix_col;
private:
};
}

#endif
