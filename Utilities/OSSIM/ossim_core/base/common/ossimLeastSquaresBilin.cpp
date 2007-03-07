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
// Author:  Garrett Potts (gpotts@imagelinks.com
//
// Description: Source code produced by Dave Knopp
//
//*******************************************************************
//  $Id: ossimLeastSquaresBilin.cpp,v 1.4 2004/04/02 13:51:14 gpotts Exp $
#include <iostream>  // for debugging
using namespace std;
#include <base/common/ossimLeastSquaresBilin.h>
#include <base/context/ossimNotifyContext.h>

ossimLeastSquaresBilin::ossimLeastSquaresBilin()
    : bl_a(0.0)
    , bl_b(0.0)
    , bl_c(0.0)
    , bl_d(0.0)
    , AtA(NULL)
    , Atb(NULL)
{
   // allocate normal system accumulation matrices
   AtA = new NEWMAT::Matrix(4,4);
   Atb = new NEWMAT::Matrix(4,1);
   
   // ensure initilization to zero
   *AtA = 0.0;
   *Atb = 0.0;
   
   return;
}
ossimLeastSquaresBilin::ossimLeastSquaresBilin(const ossimLeastSquaresBilin &rhs)
{
   // allocate normal system accumulation matrices
   AtA = new NEWMAT::Matrix(4,4);
   Atb = new NEWMAT::Matrix(4,1);

   bl_a = rhs.bl_a;
   bl_b = rhs.bl_b;
   bl_c = rhs.bl_c;
   bl_d = rhs.bl_d;
   
   *AtA  = *rhs.AtA;
   *Atb  = *rhs.Atb;
}

ossimLeastSquaresBilin::~ossimLeastSquaresBilin()
{
   if(AtA)
   {
      delete AtA;
      AtA = NULL;
   }
   if(Atb)
   {
      delete Atb;
      Atb = NULL;
   }
}
ossimLeastSquaresBilin & ossimLeastSquaresBilin::operator = (const ossimLeastSquaresBilin &rhs)
{
   bl_a    = rhs.bl_a;
   bl_b    = rhs.bl_b;
   bl_c    = rhs.bl_c;
   bl_d    = rhs.bl_d;
   
   *AtA    = *rhs.AtA;
   *Atb    = *rhs.Atb;
   
   return *this;
}

void ossimLeastSquaresBilin::clear()
{
   *AtA    = 0.0;
   *Atb    = 0.0;  
   bl_a    = 0.0;
   bl_b    = 0.0;
   bl_c    = 0.0;
   bl_d    = 0.0;
}

void ossimLeastSquaresBilin::addSample(double xx, double yy, double zmea)
{
   // form normal system layer
   NEWMAT::Matrix AtA_layer(4,1);
   AtA_layer(1,1) = 1.0;
   AtA_layer(2,1) = xx;
   AtA_layer(3,1) = yy;
   AtA_layer(4,1) = xx*yy;
   
   // accumulate layer into normal system
   *AtA += AtA_layer * AtA_layer.t();
   *Atb += AtA_layer * zmea;
   
}

bool ossimLeastSquaresBilin::solveLS()
{
   NEWMAT::Matrix Soln(4,1);
   Soln = AtA->i() * (*Atb);
   bl_a = Soln(1,1);
   bl_b = Soln(2,1);
   bl_c = Soln(3,1);
   bl_d = Soln(4,1);
   
   
   return true;
}

bool ossimLeastSquaresBilin::getLSParms(double& pa,
                                        double& pb_x,
                                        double& pc_y,
                                        double& pd_xy)const
{
   pa    = bl_a;
   pb_x  = bl_b;
   pc_y  = bl_c;
   pd_xy = bl_d;
   
   return true;
}

void ossimLeastSquaresBilin::setLSParams(double pa,
                                         double pb_x,
                                         double pc_y,
                                         double pd_xy)
{
   bl_a = pa;
   bl_b = pb_x;
   bl_c = pc_y;
   bl_d = pd_xy;
}

                 
