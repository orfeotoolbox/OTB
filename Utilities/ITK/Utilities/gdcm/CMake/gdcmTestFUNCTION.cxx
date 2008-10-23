/*=========================================================================
                                                                                
  Program:   gdcm
  Module:    $RCSfile: gdcmTestFUNCTION.cxx,v $
  Language:  C++
  Date:      $Date: 2006-07-10 20:08:21 $
  Version:   $Revision: 1.3 $
                                                                                
  Copyright (c) CREATIS (Centre de Recherche et d'Applications en Traitement de
  l'Image). All rights reserved. See Doc/License.txt or
  http://www.creatis.insa-lyon.fr/Public/Gdcm/License.html for details.
                                                                                
     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
                                                                                
=========================================================================*/

// Minimal test for existence of __FUNCTION__ pseudo-macro
#include <string.h>

int TestFUNCTION()
{
#ifdef __BORLANDC__
  #ifndef __FUNCTION__
    #define __FUNCTION__ __FUNC__
  #endif
#endif
  const char *f = __FUNCTION__;
  int r = strcmp( "TestFUNCTION", f);
  return r;
}
 
int main()
{
  return TestFUNCTION();
}

