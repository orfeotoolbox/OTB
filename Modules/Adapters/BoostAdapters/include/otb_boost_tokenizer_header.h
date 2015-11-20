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
#ifndef otb_boost_tokenizer_header_h
#define otb_boost_tokenizer_header_h

#if defined(__GNUC__) || (__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#pragma GCC diagnostic pop
#else
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#endif


#endif
