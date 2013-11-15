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


#include <iostream>
#include "otbHermitianEigenAnalysis.h"
#include "itkVariableLengthVector.h"
#include "vnl/algo/vnl_complex_eigensystem.h"
#include <complex>


int otbHermitianEigenAnalysisTest(int argc, char * argv[])
{
  typedef itk::Vector<float, 9> MatrixType;
  typedef itk::Vector<float, 3> EigenvalueType;
  typedef itk::Vector<itk::Vector<float, 6>, 3> EigenMatrixType;

  typedef otb::HermitianEigenAnalysis<MatrixType, EigenvalueType, EigenMatrixType> FilterType;

  EigenMatrixType resEigVal;

  itk::Vector<float, 6> temp;
  temp[0] = 0.162793;
  temp[1] = -0.432753;
  temp[2] = 0.766443;
  temp[3] = 1.08418e-18;
  temp[4] = 0.338266;
  temp[5] = 0.290456;
  resEigVal[0] = temp;
  temp[0] = 0.420547;
  temp[1] = -0.406821;
  temp[2] = -0.541437;
  temp[3] = 0.112727;
  temp[4] = 0.58546;
  temp[5] = -0.0949371;
  resEigVal[1] = temp;
  temp[0] = 0.664613;
  temp[1] = -0.0463679;
  temp[2] = 0.206196;
  temp[3] = -0.253351;
  temp[4] = -0.361684;
  temp[5] =  -0.564465;
  resEigVal[2] = temp;

  EigenvalueType resVect;
  resVect[0] = 8.22892;
  resVect[1] = -1.08457;
  resVect[2] = -5.64434;
  
  FilterType filter;

  MatrixType mat;
  mat.Fill(0.);

  float val = 0;
  for(unsigned int i=0; i<mat.Size(); ++i)
    {
      mat[i] = val;
      val += 0.5;
    }
  std::cout<<"Matrix: "<<std::endl;
  std::cout<<mat<<std::endl;

  EigenvalueType vect;
  EigenMatrixType eigVal;
  // Only method used in SARPolarimetry...
  filter.ComputeEigenValuesAndVectors( mat, vect, eigVal);

  if( (vcl_abs(resVect[0] - vect[0]) > 1e-5) || (vcl_abs(resVect[1] - vect[1]) > 1e-5) || (vcl_abs(resVect[2] - vect[2]) > 1e-5) )
  {
    std::cout<<"Test gives :"<<std::endl;
    std::cout<<"Values: "<<vect<<std::endl;
    std::cout<<"Wanted results are :"<<std::endl;
    std::cout<<"Values: "<<resVect<<std::endl;

    return EXIT_FAILURE;
  }

  bool isOK = true;
  for(unsigned int i=0; i<6; ++i)
    {
      for(unsigned int j=0; j<3; ++j)
        {
          if(vcl_abs(eigVal[j][i] - resEigVal[j][i]) > 1e-5)
            {
              isOK = false;
            }
        }
    }
  if( isOK == false )
  {
    std::cout<<"Test gives :"<<std::endl;
    std::cout<<"Vectors: "<<eigVal<<std::endl;
    std::cout<<"Wanted results are :"<<std::endl;
    std::cout<<"Vectors: "<<resEigVal<<std::endl;
   
    return EXIT_FAILURE;
  }

  std::cout<<"Values: "<<vect<<std::endl;
  std::cout<<"Vectors: "<<eigVal<<std::endl;


  typedef std::complex<double> ComplexType;
  vnl_matrix<ComplexType> vnlMat(3, 3, 0);
  vnlMat[0][0] = ComplexType(0., 0.);
  vnlMat[0][1] = ComplexType(1.5, 2.);
  vnlMat[0][2] = ComplexType(2.5, 3.);
  vnlMat[1][0] = ComplexType(1.5, -2.);
  vnlMat[1][1] = ComplexType(0.5, 0.);
  vnlMat[1][2] = ComplexType(3.5, 4.);
  vnlMat[2][0] = ComplexType(2.5, -3.);
  vnlMat[2][1] = ComplexType(3.5, -4.);
  vnlMat[2][2] = ComplexType(1., 0.);

  std::cout<<"Matrix:: "<<std::endl;
  vnlMat.print(std::cout);

  vnl_complex_eigensystem syst(vnlMat, true, true);

  vnl_matrix< ComplexType > pm = syst.L;
 std::cout<<"Left:: "<<std::endl;
  pm.print(std::cout);
  std::cout<<"Right:: "<<std::endl;


  pm = syst.R;
  pm.print(std::cout);
  std::cout<<"W:: "<<std::endl;
  vnl_vector< ComplexType > lol = syst.W;

  for(unsigned i=0; i<lol.size(); ++i)
  {
    std::cout<<"  "<<lol[i];
  }
  std::cout<<std::endl;

  return EXIT_SUCCESS;
}
