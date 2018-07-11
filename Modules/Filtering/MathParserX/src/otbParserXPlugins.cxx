/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbParserXPlugins.h"

#include "otbMath.h"
#include "itkNumericTraits.h"

#include <vector>

namespace otb
{


void bands::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      
      if (a_iArgc != 2)
        return;

      assert(a_pArg[0]->GetType()=='m');
      assert(a_pArg[1]->GetType()=='m');

      // Get the argument from the argument input vector
      mup::matrix_type a = a_pArg[0]->GetArray();
      mup::matrix_type b = a_pArg[1]->GetArray();

      int nbcols = b.GetCols();

      assert(a.GetRows()==1); // Bands selection is done on a row vector
      assert(b.GetRows()==1); // Bands selection is done by a row vector

      mup::matrix_type res(1,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        res.At(0,k)=a.At(0,b.At(0,k).GetInteger()-1); //-1 : to make first band have rank #1 (and not 0)

      // The return value is passed by writing it to the reference ret
      *ret = res;

    }


void dotpr::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      assert(a_pArg[0]->GetType()=='m');

      // Get the argument from the argument input vector
      mup::matrix_type m1 = a_pArg[0]->GetArray();
      

      int nbrows = m1.GetRows();
      int nbcols = m1.GetCols();

      mup::matrix_type res(1,a_iArgc-1,0.);

      for (int k=1; k<a_iArgc; ++k)
      {
  
        float sum=0.0;

        assert(a_pArg[k]->GetType()=='m');
        mup::matrix_type m2 = a_pArg[k]->GetArray();

        assert(m2.GetRows() == nbrows);
        assert(m2.GetCols() == nbcols);

        for (int i=0; i<nbrows; i++)
          for (int j=0; j<nbcols; j++)
            sum += m1.At(i,j).GetFloat() * m2.At(i,j).GetFloat();

        
        res.At(0,k-1)=sum;
      }


      // The return value is passed by writing it to the reference ret
      *ret = res;
    }

void ElementWiseDivision::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int)
    {

      assert(a_pArg[0]->GetType()=='m');
      assert(a_pArg[1]->GetType()=='m');

      const mup::matrix_type a = a_pArg[0]->GetArray();
      const mup::matrix_type b = a_pArg[1]->GetArray();

      
      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      assert(nbrows == b.GetRows());
      assert(nbcols == b.GetCols());

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          {
            assert(std::abs(b.At(p,k).GetFloat()) > 1e-9);
            res.At(p,k) = a.At(p,k).GetFloat() / b.At(p,k).GetFloat();
          }
          

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void DivisionByScalar::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int)
    {

      assert(a_pArg[0]->GetType()=='m');
      const mup::matrix_type a = a_pArg[0]->GetArray();
      mup::matrix_type b;

      double scalar = 0;
      switch (a_pArg[1]->GetType())
      {
        case 'i':
          scalar = (double) a_pArg[1]->GetInteger();
        break;
    
        case 'f':
          scalar = a_pArg[1]->GetFloat();
        break;

        case 'm':
          b = a_pArg[1]->GetArray();
          assert(b.GetRows() == 1);
          assert(b.GetCols() == 1);
          scalar = b.At(0,0);
        break;
      }
      
      assert(std::abs(scalar) > 1e-9);

      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = a.At(p,k).GetFloat() / scalar;
          

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void ElementWiseMultiplication::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int)
    {

      assert(a_pArg[0]->GetType()=='m');
      assert(a_pArg[1]->GetType()=='m');

      const mup::matrix_type a = a_pArg[0]->GetArray();
      const mup::matrix_type b = a_pArg[1]->GetArray();
      
      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      assert(nbrows == b.GetRows());
      assert(nbcols == b.GetCols());

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = a.At(p,k).GetFloat() * b.At(p,k).GetFloat();
        
      // The return value is passed by writing it to the reference ret
      *ret = res;
    }

void MultiplicationByScalar::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int)
    {

      assert(a_pArg[0]->GetType()=='m');
      const mup::matrix_type a = a_pArg[0]->GetArray();
      mup::matrix_type b;

      double scalar(1.);
      switch (a_pArg[1]->GetType())
      {
        case 'i':
          scalar = (double) a_pArg[1]->GetInteger();
        break;
    
        case 'f':
          scalar = a_pArg[1]->GetFloat();
        break;

        case 'm':
          b = a_pArg[1]->GetArray();
          assert(b.GetRows() == 1);
          assert(b.GetCols() == 1);
          scalar = b.At(0,0);
        break;
      }

      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = a.At(p,k).GetFloat() * scalar;
          

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void ElementWisePower::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc))
    {

      assert(a_pArg[0]->GetType()=='m');
      assert(a_pArg[1]->GetType()=='m');

      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();
      const mup::matrix_type b = a_pArg[1]->GetArray();

      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      assert(nbrows == b.GetRows());
      assert(nbcols == b.GetCols());

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
        {
          assert(a.At(p,k).GetFloat() >= 0 );
          res.At(p,k) = std::pow(a.At(p,k).GetFloat(),b.At(p,k).GetFloat());
        }

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void PowerByScalar::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int)
    {

      assert(a_pArg[0]->GetType()=='m');
      const mup::matrix_type a = a_pArg[0]->GetArray();
      mup::matrix_type b;

      double scalar(1.);
      switch (a_pArg[1]->GetType())
      {
        case 'i':
          scalar = (double) a_pArg[1]->GetInteger();
        break;
    
        case 'f':
          scalar = a_pArg[1]->GetFloat();
        break;

        case 'm':
          b = a_pArg[1]->GetArray();
          assert(b.GetRows() == 1);
          assert(b.GetCols() == 1);
          scalar = b.At(0,0);
        break;
      }

      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = std::pow(a.At(p,k).GetFloat() , scalar);
          

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void ndvi::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      // Check for arg length
      if(a_iArgc != 2)
        return;

      // Get the argument from the argument input vector
      mup::float_type r = a_pArg[0]->GetFloat();
      mup::float_type niri = a_pArg[1]->GetFloat();


     // The return value is passed by writing it to the reference ret
      if ( std::abs(r + niri) < 1E-6 )
          *ret = 0.;
      else
          *ret = (niri-r)/(niri+r);
    }


void cat::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {

      std::vector<double> vect;
      int nbcols;
      mup::matrix_type m1;

      for (int k=0; k<a_iArgc; ++k)
      {

        // Get the argument from the argument input vector
        switch (a_pArg[k]->GetType())
        {
          case 'm':

              m1 = a_pArg[k]->GetArray();

             
              nbcols = m1.GetCols();

              assert(m1.GetRows()==1);

              for (int j=0; j<nbcols; j++)
                  vect.push_back( m1.At(0,j).GetFloat());

          break;
    
          case 'i':
            vect.push_back( (double) a_pArg[k]->GetInteger());
          break;
        
          case 'f':
            vect.push_back( (double) a_pArg[k]->GetFloat());
          break;
        }
      }

      // The return value is passed by writing it to the reference ret
      mup::matrix_type res(1,vect.size(),0.);
      for (unsigned int j=0; j<vect.size(); j++)
            res.At(0,j) = vect[j];
      *ret = res;
    }


void mean::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {

      std::vector<double> vect;
      int nbrows,nbcols;
      mup::matrix_type m1;
      double sum;

      for (int k=0; k<a_iArgc; ++k)
      {

        // Get the argument from the argument input vector
        switch (a_pArg[k]->GetType())
        {
          case 'm':

            m1 = a_pArg[k]->GetArray();

            nbrows = m1.GetRows();
            nbcols = m1.GetCols();
          
            sum=0.0;

            for (int i=0; i<nbrows; i++)
              for (int j=0; j<nbcols; j++)
                sum += m1.At(i,j).GetFloat();

            vect.push_back( sum / (double) (nbrows*nbcols) );


          break;
    
          case 'i':
            vect.push_back( (double) a_pArg[k]->GetInteger());
          break;
        
          case 'f':
            vect.push_back( (double) a_pArg[k]->GetFloat());
          break;
        }
      }

      // The return value is passed by writing it to the reference ret
      mup::matrix_type res(1,vect.size(),0.);
      for (unsigned int j=0; j<vect.size(); j++)
            res.At(0,j) = vect[j];
      *ret = res;
    }


void var::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {

      std::vector<double> vect;
      int nbrows,nbcols;
      mup::matrix_type m1;
      double sum,mean;

      for (int k=0; k<a_iArgc; ++k)
      {

        // Get the argument from the argument input vector
        switch (a_pArg[k]->GetType())
        {
          case 'm':

            m1 = a_pArg[k]->GetArray();

            nbrows = m1.GetRows();
            nbcols = m1.GetCols();
          
            sum=0.0;

            for (int i=0; i<nbrows; i++)
              for (int j=0; j<nbcols; j++)
               sum += m1.At(i,j).GetFloat();

            mean = sum / (double) (nbrows*nbcols);
          
            sum=0.0;

            for (int i=0; i<nbrows; i++)
              for (int j=0; j<nbcols; j++)
                sum += std::pow(mean - m1.At(i,j).GetFloat(),2);
          
            vect.push_back( sum / (double) (nbrows*nbcols) );
    
          break;
    
          case 'i':
            vect.push_back( 0.0 );
          break;
        
          case 'f':
            vect.push_back( 0.0 );
          break;
        }
      }

      // The return value is passed by writing it to the reference ret
      mup::matrix_type res(1,vect.size(),0.);
      for (unsigned int j=0; j<vect.size(); j++)
        res.At(0,j) = static_cast<mup::float_type>(vect[j]);
      *ret = res;
    }


void corr::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int itkNotUsed(a_iArgc))
    {

      assert(a_pArg[0]->GetType()=='m');
      assert(a_pArg[1]->GetType()=='m');

      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();
      const mup::matrix_type b = a_pArg[1]->GetArray();

      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      assert(nbrows == b.GetRows());
      assert(nbcols == b.GetCols());

      double mean1=0.0;
      for (int i=0; i<nbrows; i++)
        for (int j=0; j<nbcols; j++)
         mean1 += a.At(i,j).GetFloat();
      mean1 = mean1 / (double) (nbrows*nbcols);

      double var1=0.0;
      for (int i=0; i<nbrows; i++)
        for (int j=0; j<nbcols; j++)
          var1 += std::pow(mean1 - a.At(i,j).GetFloat(),2);
      var1 = var1 / (double) (nbrows*nbcols);

      double mean2=0.0;
      for (int i=0; i<nbrows; i++)
        for (int j=0; j<nbcols; j++)
         mean2 += b.At(i,j).GetFloat();
      mean2 = mean2 / (double) (nbrows*nbcols);

      double var2=0.0;
      for (int i=0; i<nbrows; i++)
        for (int j=0; j<nbcols; j++)
          var2 += std::pow(mean2 - b.At(i,j).GetFloat(),2);
      var2 = var2 / (double) (nbrows*nbcols);

      double cross=0.0;
      for (int i=0; i<nbrows; i++)
        for (int j=0; j<nbcols; j++)
         cross += (a.At(i,j).GetFloat()-mean1)*(b.At(i,j).GetFloat()-mean2);
      cross = cross / (double) (nbrows*nbcols);


      *ret = cross / ( std::sqrt(var1)*std::sqrt(var2) );
    }


void median::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {

      std::vector<double> vect,tempvect;
      int nbrows,nbcols;
      mup::matrix_type m1;


      for (int k=0; k<a_iArgc; ++k)
      {
        tempvect.clear();
        // Get the argument from the argument input vector
        switch (a_pArg[k]->GetType())
        {
          case 'm':

            m1 = a_pArg[k]->GetArray();

            nbrows = m1.GetRows();
            nbcols = m1.GetCols();

            for (int i=0; i<nbrows; i++)
              for (int j=0; j<nbcols; j++)
                tempvect.push_back(m1.At(i,j).GetFloat());

            std::sort(tempvect.begin(),tempvect.end());

            vect.push_back( tempvect[(int) (tempvect.size()/2.)] );

          break;
    
          case 'i':
            vect.push_back( (double) a_pArg[k]->GetInteger());
          break;
        
          case 'f':
            vect.push_back( (double) a_pArg[k]->GetFloat());
          break;
        }
      }

      // The return value is passed by writing it to the reference ret
      mup::matrix_type res(1,vect.size(),0.);
      for (unsigned int j=0; j<vect.size(); j++)
        res.At(0,j) = static_cast<mup::float_type>(vect[j]);
      *ret = res;
    }


void maj::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {

      std::vector<int> vect,tempvect;
      int nbrows,nbcols,score,bestScore,majElmt;
      mup::matrix_type m1;


      for (int k=0; k<a_iArgc; ++k)
      {
        tempvect.clear();
        // Get the argument from the argument input vector
        switch (a_pArg[k]->GetType())
        {
          case 'm':

            m1 = a_pArg[k]->GetArray();

            nbrows = m1.GetRows();
            nbcols = m1.GetCols();

            for (int i=0; i<nbrows; i++)
              for (int j=0; j<nbcols; j++)
                tempvect.push_back( (int) (m1.At(i,j).GetFloat() + 0.5) );

            std::sort(tempvect.begin(),tempvect.end());

            bestScore = 0;
            score = 0;

            for(unsigned int t=0; t<tempvect.size()-1; t++)
            {

              score++;
              if (tempvect[t] != tempvect[t+1])
              {
                  if (score > bestScore)
                  {
                    bestScore=score;
                    majElmt=tempvect[t];
                  }
                  score=0;
              }
            }

            vect.push_back( majElmt );

          break;
    
          case 'i':
            vect.push_back( (double) a_pArg[k]->GetInteger());
          break;
        
          case 'f':
            vect.push_back( (double) a_pArg[k]->GetFloat());
          break;
        }
      }

      // The return value is passed by writing it to the reference ret
      mup::matrix_type res(1,vect.size(),0.);
      for (unsigned int j=0; j<vect.size(); j++)
        res.At(0,j) = static_cast<mup::float_type>(vect[j]);
      *ret = res;
    }


//--------------------------------------------------------------------------------------------------------//
void vnorm::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      // Check for arg length
      if(a_iArgc != 1)
        return;

      int nbrows,nbcols;
      mup::matrix_type m1;
      double sum=0.0;

      assert( a_iArgc==1 );
      assert(a_pArg[0]->GetType()=='m');

      m1 = a_pArg[0]->GetArray();

      nbrows = m1.GetRows();
      nbcols = m1.GetCols();

      for (int i=0; i<nbrows; i++)
        for (int j=0; j<nbcols; j++)
          sum += std::pow(m1.At(i,j).GetFloat(),2.0);


      // The return value is passed by writing it to the reference ret
      mup::matrix_type res(1,1,std::sqrt(sum));
      *ret = res;
  }


void vmin::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      if(a_iArgc != 1)
        return;
  
      std::vector<double> vect;
      double min;

      int nbrows,nbcols;
      mup::matrix_type m1;

      assert(a_pArg[0]->GetType()=='m');

      min = itk::NumericTraits<double>::max();

      m1 = a_pArg[0]->GetArray();

      nbrows = m1.GetRows();
      nbcols = m1.GetCols();

      for (int i=0; i<nbrows; i++)
        for (int j=0; j<nbcols; j++)
          if (m1.At(i,j).GetFloat() < min )
            min = m1.At(i,j).GetFloat();

      // The return value is passed by writing it to the reference ret
      mup::matrix_type res(1,1,min);
      *ret = res;
    }


void vmax::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      if(a_iArgc != 1)
        return;
  
      std::vector<double> vect;
      double max;

      int nbrows,nbcols;
      mup::matrix_type m1;

 
      assert(a_pArg[0]->GetType()=='m');

      max = itk::NumericTraits<double>::min();

      m1 = a_pArg[0]->GetArray();

      nbrows = m1.GetRows();
      nbcols = m1.GetCols();

      for (int i=0; i<nbrows; i++)
        for (int j=0; j<nbcols; j++)
          if (m1.At(i,j).GetFloat() > max )
            max = m1.At(i,j).GetFloat();

      // The return value is passed by writing it to the reference ret
      mup::matrix_type res(1,1,max);
      *ret = res;
    }


void vect2scal::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      if(a_iArgc != 1)
        return;

      assert(a_pArg[0]->GetType()=='m');

      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();

      assert(a.GetRows() == 1);
      assert(a.GetCols() == 1);

      mup::float_type res;

      res = a.At(0,0).GetFloat();
          

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vcos::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      if(a_iArgc != 1)
        return;

      assert(a_pArg[0]->GetType()=='m');

      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();


      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = std::cos(a.At(p,k).GetFloat());
          

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }
    
    
void vacos::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      if(a_iArgc != 1)
        return;

      assert(a_pArg[0]->GetType()=='m');

      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();


      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = std::acos(a.At(p,k).GetFloat());
          

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vsin::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      if(a_iArgc != 1)
        return;

      assert(a_pArg[0]->GetType()=='m');

      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();

      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = std::sin(a.At(p,k).GetFloat());

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vasin::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      if(a_iArgc != 1)
        return;

      assert(a_pArg[0]->GetType()=='m');

      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();


      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = std::asin(a.At(p,k).GetFloat());
          

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vtan::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      if(a_iArgc != 1)
        return;

      assert(a_pArg[0]->GetType()=='m');

      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();

      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = std::tan(a.At(p,k).GetFloat());

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vatan::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      if(a_iArgc != 1)
        return;

      assert(a_pArg[0]->GetType()=='m');

      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();

      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = std::atan(a.At(p,k).GetFloat());

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vtanh::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      if(a_iArgc != 1)
        return;

      assert(a_pArg[0]->GetType()=='m');

      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();


      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = std::tanh(a.At(p,k).GetFloat());

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vsinh::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      if(a_iArgc != 1)
        return;
      
      assert(a_pArg[0]->GetType()=='m');

      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();


      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = std::sinh(a.At(p,k).GetFloat());

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vcosh::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      if(a_iArgc != 1)
        return;

      assert(a_pArg[0]->GetType()=='m');

      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();


      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = std::cosh(a.At(p,k).GetFloat());

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vlog::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      if(a_iArgc != 1)
        return;

      assert(a_pArg[0]->GetType()=='m');

      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();

      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = std::log(a.At(p,k).GetFloat());

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vlog10::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      if(a_iArgc != 1)
        return; 

      assert(a_pArg[0]->GetType()=='m');

      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();


      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = std::log10(a.At(p,k).GetFloat());

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vabs::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {     
      if(a_iArgc != 1)
        return; 
      
      assert(a_pArg[0]->GetType()=='m');

      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();

      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = std::abs(a.At(p,k).GetFloat());

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vexp::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      if(a_iArgc != 1)
        return; 

      assert(a_pArg[0]->GetType()=='m');

      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();


      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = std::exp(a.At(p,k).GetFloat());

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vsqrt::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      if(a_iArgc != 1)
        return; 

      assert(a_pArg[0]->GetType()=='m');

      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();

      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
        {
          assert(a.At(p,k).GetFloat() >= 0 );
          res.At(p,k) = std::sqrt(a.At(p,k).GetFloat());
        }

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }

}//end namespace otb

