//----------------------------------------------------------------------------
// File: ossimFileProcessorInterface.h
// 
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Class interface ossimFileProcessorInterface.
//
// Has pure virtual "processFile" method that derived classes must
// implement to be concrete. 
// 
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimFileProcessorInterface_HEADER
#define ossimFileProcessorInterface_HEADER 1

class ossimFilename;

/** @class ossimFileProcessorInterface */
class ossimFileProcessorInterface
{
public:
   
   /** @brief default constructor */
   ossimFileProcessorInterface(){}

   /** @brief virtual destructor. */
   virtual ~ossimFileProcessorInterface(){}

   /**
    * @brief Pure virtual processFile method. Derived classed must implement.
    * @param file to process.
    */
   virtual void processFile(const ossimFilename& file) = 0;
};

#endif /* #ifndef ossimFileProcessorInterface_HEADER */
