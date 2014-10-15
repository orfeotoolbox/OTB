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
#ifndef __otbParserXPlugins_h
#define __otbParserXPlugins_h


#include "otbMath.h"
#include "mpParser.h"

namespace otb
{

class ndvi : public mup::ICallback
  {
public:
    ndvi():ICallback(mup::cmFUNC, "ndvi", 2)
    {}

    virtual void Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      // Get the argument from the argument input vector
      mup::float_type r = a_pArg[0]->GetFloat();
      
      mup::float_type niri = a_pArg[1]->GetFloat();


     // The return value is passed by writing it to the reference ret
      if ( vcl_abs(r + niri) < 1E-6 )
          *ret = 0.;  
      else
          *ret = (niri-r)/(niri+r);
    }

    const mup::char_type* GetDesc() const
    {
      return "NDVI";
    }

    IToken* Clone() const
    {
      return new ndvi(*this);
    }
  };

}//end namespace otb

#endif
