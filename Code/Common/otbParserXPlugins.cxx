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
#include "otbParserXPlugins.h"

#include "otbMath.h"
#include "itkNumericTraits.h"

#include <vector>

namespace otb
{


void bands::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {

      assert(a_iArgc==2);
      assert(a_pArg[0]->GetType()=='m');
      assert(a_pArg[1]->GetType()=='m');

      // Get the argument from the argument input vector
      mup::matrix_type a = a_pArg[0]->GetArray();
      mup::matrix_type b = a_pArg[1]->GetArray();


      int nbrows = b.GetRows(); assert(nbrows==1); // Bands selection is done by a row vector
      int nbcols = b.GetCols();

      mup::matrix_type res(1,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        res.At(0,k)=a.At(0,b.At(0,k).GetInteger()-1); //-1 : to make first band have rank #1 (and not 0)

      // The return value is passed by writing it to the reference ret
      *ret = res;

    }


void conv::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      assert(a_pArg[0]->GetType()=='m');

      // Get the argument from the argument input vector
      mup::matrix_type m1 = a_pArg[0]->GetArray();
      

      int nbrows = m1.GetRows();
      int nbcols = m1.GetCols();

      mup::matrix_type res(1,a_iArgc-1,0);

      for (int k=1; k<a_iArgc; ++k)
      {
  
        float sum=0.0;

        mup::matrix_type m2 = a_pArg[k]->GetArray();

        if ( (m2.GetRows() != nbrows) || (m2.GetCols() != nbcols) )
        {
            mup::ErrorContext err;
            err.Errc = mup::ecMATRIX_DIMENSION_MISMATCH;
            err.Arg = a_iArgc;
            err.Ident = GetIdent();
            throw mup::ParserError(err);
        }


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

      int nbrows2 = b.GetRows();
      int nbcols2 = b.GetCols();

      if ( (nbrows != nbrows2) || (nbcols != nbcols2) )
      {
          mup::ErrorContext err;
          err.Errc = mup::ecMATRIX_DIMENSION_MISMATCH;
          err.Ident = GetIdent();
          throw mup::ParserError(err);
      }

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          {
            assert(vcl_abs(b.At(p,k).GetFloat()) > 1e-6);
            res.At(p,k) = a.At(p,k).GetFloat() / b.At(p,k).GetFloat();
          }
          

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

      int nbrows2 = b.GetRows();
      int nbcols2 = b.GetCols();

      if ( (nbrows != nbrows2) || (nbcols != nbcols2) )
      {
          mup::ErrorContext err;
          err.Errc = mup::ecMATRIX_DIMENSION_MISMATCH;
          err.Ident = GetIdent();
          throw mup::ParserError(err);
      }

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = a.At(p,k).GetFloat() * b.At(p,k).GetFloat();
        
      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void ElementWisePower::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {

      assert(a_iArgc==2);
      assert(a_pArg[0]->GetType()=='m');

      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();
      const double pw = a_pArg[1]->GetFloat();

      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
        {
          assert(a.At(p,k).GetFloat() >= 0 );
          res.At(p,k) = vcl_pow(a.At(p,k).GetFloat(),pw);
        }

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void ndvi::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
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


void cat::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {

      std::vector<double> vect;
      int nbrows,nbcols;
      mup::matrix_type m1;

      for (int k=0; k<a_iArgc; ++k)
      {

        // Get the argument from the argument input vector
        switch (a_pArg[k]->GetType())
        {
          case 'm':

              m1 = a_pArg[k]->GetArray();

              nbrows = m1.GetRows();
              nbcols = m1.GetCols();

              assert(nbrows==1);

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
      mup::matrix_type res(1,vect.size(),0);
      for (int j=0; j<vect.size(); j++)
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
      mup::matrix_type res(1,vect.size(),0);
      for (int j=0; j<vect.size(); j++)
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
                sum += vcl_pow(mean - m1.At(i,j).GetFloat(),2);
          
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
      mup::matrix_type res(1,vect.size(),0);
      for (int j=0; j<vect.size(); j++)
            res.At(0,j) = vect[j];
      *ret = res;
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
      mup::matrix_type res(1,vect.size(),0);
      for (int j=0; j<vect.size(); j++)
            res.At(0,j) = vect[j];
      *ret = res;
    }


void vmin::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
  
      std::vector<double> vect;
      double min;

      int nbrows,nbcols;
      mup::matrix_type m1;

      for (int k=0; k<a_iArgc; ++k)
      {

        // Get the argument from the argument input vector
        switch (a_pArg[k]->GetType())
        {
          case 'm':

            min = itk::NumericTraits<double>::max();

            m1 = a_pArg[k]->GetArray();

            nbrows = m1.GetRows();
            nbcols = m1.GetCols();

            for (int i=0; i<nbrows; i++)
              for (int j=0; j<nbcols; j++)
                if (m1.At(i,j).GetFloat() < min )
                  min = m1.At(i,j).GetFloat();

            vect.push_back(min);

          break;
    
          case 'i':
            min = itk::NumericTraits<double>::max();

            if ( (double) a_pArg[k]->GetInteger() < min)
              min = (double) a_pArg[k]->GetInteger();

            vect.push_back(min);
          break;
        
          case 'f':
            min = itk::NumericTraits<double>::max();

            if ( a_pArg[k]->GetFloat() < min)
              min = a_pArg[k]->GetFloat();

            vect.push_back(min);
          break;
        }
      }

      // The return value is passed by writing it to the reference ret
      mup::matrix_type res(1,vect.size(),0);
      for (int j=0; j<vect.size(); j++)
            res.At(0,j) = vect[j];
      *ret = res;
    }


void vmax::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
  
      std::vector<double> vect;
      double max;

      int nbrows,nbcols;
      mup::matrix_type m1;

      for (int k=0; k<a_iArgc; ++k)
      {

        // Get the argument from the argument input vector
        switch (a_pArg[k]->GetType())
        {
          case 'm':

            max = itk::NumericTraits<double>::min();

            m1 = a_pArg[k]->GetArray();

            nbrows = m1.GetRows();
            nbcols = m1.GetCols();

            for (int i=0; i<nbrows; i++)
              for (int j=0; j<nbcols; j++)
                if (m1.At(i,j).GetFloat() > max )
                  max = m1.At(i,j).GetFloat();

            vect.push_back(max);

          break;
    
          case 'i':
            max = itk::NumericTraits<double>::min();

            if ( (double) a_pArg[k]->GetInteger() > max)
              max = (double) a_pArg[k]->GetInteger();

            vect.push_back(max);
          break;
        
          case 'f':
            max = itk::NumericTraits<double>::min();

            if ( a_pArg[k]->GetFloat() > max)
              max = a_pArg[k]->GetFloat();

            vect.push_back(max);
          break;
        }
      }

      // The return value is passed by writing it to the reference ret
      mup::matrix_type res(1,vect.size(),0);
      for (int j=0; j<vect.size(); j++)
            res.At(0,j) = vect[j];
      *ret = res;
    }



//--------------------------------------------------------------------------------------------------------//
void vcos::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();


      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = vcl_cos(a.At(p,k).GetFloat());
          

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vsin::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();


      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = vcl_sin(a.At(p,k).GetFloat());

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vtan::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();


      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = vcl_tan(a.At(p,k).GetFloat());

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vtanh::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();


      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = vcl_tanh(a.At(p,k).GetFloat());

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vsinh::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();


      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = vcl_sinh(a.At(p,k).GetFloat());

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vcosh::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();


      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = vcl_cosh(a.At(p,k).GetFloat());

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vlog::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();


      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = vcl_log(a.At(p,k).GetFloat());

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vlog10::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();


      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = vcl_log10(a.At(p,k).GetFloat());

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vabs::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();


      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = vcl_abs(a.At(p,k).GetFloat());

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vexp::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();


      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
          res.At(p,k) = vcl_exp(a.At(p,k).GetFloat());

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }


void vsqrt::Eval(mup::ptr_val_type &ret, const mup::ptr_val_type *a_pArg, int a_iArgc)
    {
      // Get the argument from the argument input vector
      const mup::matrix_type a = a_pArg[0]->GetArray();


      int nbrows = a.GetRows();
      int nbcols = a.GetCols();

      mup::matrix_type res(nbrows,nbcols,0.);

      for (int k=0; k<nbcols; ++k)
        for (int p=0; p<nbrows; ++p)
        {
          assert(a.At(p,k).GetFloat() >= 0 );
          res.At(p,k) = vcl_sqrt(a.At(p,k).GetFloat());
        }

      // The return value is passed by writing it to the reference ret
      *ret = res;
    }

}//end namespace otb

