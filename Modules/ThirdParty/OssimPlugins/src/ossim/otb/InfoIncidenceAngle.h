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

#ifndef InfoIncidenceAngle_h
#define InfoIncidenceAngle_h

#include <vector>
#include <ossim/base/ossimString.h>
#include <ossimPluginConstants.h>

class ossimKeywordlist;

namespace ossimplugins
{


/**
 * @ingroup InfoIncidenceAngle
 * @brief This class represents an InfoIncidenceAngle
 */
class OSSIM_PLUGINS_DLL InfoIncidenceAngle
{
public:
   /**
    * @brief Constructor
    */
   InfoIncidenceAngle();

   /**
    * @brief Destructor
    */
   virtual ~InfoIncidenceAngle();

   /**
    * @brief Copy constructor
    */
   InfoIncidenceAngle(const InfoIncidenceAngle& rhs);

   /**
    * @brief Affectation operator
    */
   InfoIncidenceAngle& operator=(const InfoIncidenceAngle& rhs);

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

   void set_refRow(ossim_uint32 value)
   {
      _refRow = value;
   }
   void set_refColumn(ossim_uint32 value)
   {
      _refColumn = value;
   }
   void set_incidenceAngle(double value)
   {
      _incidenceAngle = value;
   }


protected:

      /**
       * @brief Reference Row (refRow node).
       */
      ossim_uint32 _refRow;

      /**
       * @brief Reference Column (refColumn node).
       */
      ossim_uint32 _refColumn;
      /**
       * @brief IncidenceAngle (incidenceAngle node).
       */
      double _incidenceAngle;

private:
};
}

#endif
