//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// ossimThreeParamDatum. This is a base class for all
// three param datums.
//*******************************************************************
//  $Id: ossimThreeParamDatum.h,v 1.5 2003/05/13 11:31:03 dburken Exp $

#ifndef ossimThreeParamDatum_HEADER
#define ossimThreeParamDatum_HEADER
#include "ossimDatum.h"

class OSSIMDLLEXPORT ossimThreeParamDatum : public ossimDatum
{
public:
   ossimThreeParamDatum(const ossimString &code, const ossimString &name,
                        const ossimEllipsoid* anEllipsoid,
                        ossim_float64 sigmaX,
                        ossim_float64 sigmaY,
                        ossim_float64 sigmaZ,
                        ossim_float64 westLongitude,
                        ossim_float64 eastLongitude,
                        ossim_float64 southLatitude,
                        ossim_float64 northLatitude,
                        ossim_float64 param1,
                        ossim_float64 param2,
                        ossim_float64 param3)
      : ossimDatum(code, name, anEllipsoid,
              sigmaX, sigmaY, sigmaZ,
              westLongitude, eastLongitude,
              southLatitude, northLatitude),
        theParam1(param1),
        theParam2(param2),
        theParam3(param3)
      {}
   virtual ~ossimThreeParamDatum(){}

   /*!
    * Will shift a ground point to this datum.  If they are the same
    * datum then nothing happens.
    */
   virtual ossimGpt    shift(const ossimGpt    &aPt)const;
   
   virtual ossimGpt    shiftToWgs84(const ossimGpt &aPt)const;
   virtual ossimGpt    shiftFromWgs84(const ossimGpt &aPt)const;
   virtual ossim_float64 param1()const{return theParam1;}
   virtual ossim_float64 param2()const{return theParam2;}
   virtual ossim_float64 param3()const{return theParam3;}
   virtual ossim_float64 param4()const{return 0.0;}
   virtual ossim_float64 param5()const{return 0.0;}
   virtual ossim_float64 param6()const{return 0.0;}
   virtual ossim_float64 param7()const{return 1.0;}

protected:
   
   ossim_float64 theParam1;
   ossim_float64 theParam2;
   ossim_float64 theParam3;
};


#endif
