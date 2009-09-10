/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbI18n_h
#define __otbI18n_h

#include "otbConfigure.h"

#if defined(OTB_I18N)
#include <libintl.h>
#endif

//To enable stringization after macro replacement
#define _QUOTEME(x) #x
#define QUOTEME(x) _QUOTEME(x)


#if defined(OTB_I18N)
#define otbI18nMacro() \
  setlocale( LC_ALL, QUOTEME(OTB_LANG) );\
  bindtextdomain( "otb", QUOTEME(OTB_LANG_LOCATION) );\
  textdomain( "otb" );\
  std::cout << "Language: " << QUOTEME(OTB_LANG) << std::endl;\
  std::cout << "Language location: " << QUOTEME(OTB_LANG_LOCATION) << std::endl;
#else
#define otbI18nMacro()\
std::cout << "No internationalization" << std::endl;
#endif

#if defined(OTB_I18N)
#define otbGetTextMacro(msgString)\
gettext(msgString)
#else
#define otbGetTextMacro(msgString)\
msgString
#endif

#endif