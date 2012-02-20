//----------------------------------------------------------------------------
// File:     ossimRpfComponentIdLut.h
// 
// License:  See top level LICENSE.txt file.
//
// Author:   David Burken
//
// Description: See class description.
// 
//----------------------------------------------------------------------------
// $Id: ossimRpfComponentIdLut.h 20324 2011-12-06 22:25:23Z dburken $

#ifndef ossimRpfComponentIdLut_HEADER
#define ossimRpfComponentIdLut_HEADER 1

#include <ossim/base/ossimLookUpTable.h>
#include <ossim/support_data/ossimRpfConstants.h>

/**
 * @class ossimRpfComponentIdLut
 *
 * @brief Lookup table for RPF section/component ID's.
 * 
 * Used to convert from RPF section/component ID's to strings and vice versa.
 * See MIL-STD-2411-1, section 5.1.1 for detailed information. Defines located
 * in ossimRpfConstants.h.  This class is a singleton, only one of them so all
 * callers must go through the instance method like:
 * ossimRpfComponentIdLut::instance()->getEntryString(id);
 */
class OSSIMDLLEXPORT ossimRpfComponentIdLut : public ossimLookUpTable
{
public:

   /** @return The static instance of an ossimRpfComponentIdLut object. */
   static ossimRpfComponentIdLut* instance();

   /** @brief destructor */
   virtual ~ossimRpfComponentIdLut();

   /** @return Keyword: ("rpf_component_id", "") */
   virtual ossimKeyword getKeyword() const;
   
private:
   /** @brief Hidden from use constructor. */
   ossimRpfComponentIdLut();

   /** @brief The single instance of this class. */
   static ossimRpfComponentIdLut* theInstance;
};

#endif /* #ifndef ossimRpfComponentIdLut_HEADER */
