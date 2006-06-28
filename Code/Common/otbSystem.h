/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T. Feuvrier
  Language  :   C++
  Date      :   28 juin 2006
  Version   :   
  Role      :   System operations
  $Id$

=========================================================================*/
#ifndef _otbSystem_h
#define _otbSystem_h

#include <string>

namespace otb
{


/** \class System
 * \brief System operations.
 *
 * System operations, filename manipulations, etc.
 *
 */
class System 
{
public:

  /** Standard class typedefs. */
  typedef System  Self;

  /** Get the extension of the file name */
  static std::string GetExtension( const std::string& filename );
  /** Get the root name */
  static std::string GetRootName( const std::string& filename );

};

} // namespace otb


#endif
