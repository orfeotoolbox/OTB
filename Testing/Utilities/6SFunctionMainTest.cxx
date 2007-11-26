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
#include <fstream>
#include <vector>

#include "otb_6S.h"
#include "main_6s.h"
#include "itkMacro.h"

int SIXSFunctionMainOtbTest( int argc, char *argv[] )
{

  try 
    {
        
        otb_6s_real asol(0.), phi0(0.), avis(0.), phiv(0.);
        otb_6s_integer month(0), jday(0);
        otb_6s_real pressure(0.), uw(0.), uo3(0.); 
        otb_6s_integer iaer(0);
        otb_6s_real taer55(0.);
        otb_6s_real wlinf(0.), wlsup(0.);
        otb_6s_real value(0.);
        otb_6s_real otb_ratm__(0.), sast(0.), tgasm(0.), sdtott(0.), sutott(0.);
        std::vector<otb_6s_real> vector_wavelenght;


        std::ifstream fin;
        std::ofstream fout;
        
        //Read input file parameters
        fin.open(argv[1]);
        fin >> asol;
        fin >> phi0;
        fin >> avis;
        fin >> phiv;
        fin >> month;
        fin >> jday;

        fin >> pressure;
        fin >> uw;
        fin >> uo3;

        fin >> iaer;
        fin >> taer55;
        
        fin >> wlinf;
        fin >> wlsup;
        
        while (!fin.eof())
        {
                fin >> value;
                vector_wavelenght.push_back(value);
        }
        
        fin.close();
        

        otb_6s_real * s(NULL);
        s = new otb_6s_real[vector_wavelenght.size()];
        for(unsigned int i=0 ; i<vector_wavelenght.size() ; i++)
        {
                s[i] = vector_wavelenght[i];
        }
        
        fout.open(argv[2]);
        fout << "Inputs parameters:"<<std::endl;
        fout <<asol<<" "<<phi0 <<" "<<avis <<" "<<phiv <<" "<<month <<" "<<jday <<std::endl;
        fout <<pressure<<" "<<uw <<" "<<uo3<<std::endl;
        fout <<iaer<<std::endl;
        fout <<taer55<<std::endl;
        fout <<wlinf<<" "<<uw <<" "<<wlsup<<std::endl;
        fout <<"Wavelenght values ["<<vector_wavelenght.size()<<"]"<<std::endl;
        for(unsigned int i=0 ; i<vector_wavelenght.size() ; i++)
        {
                fout <<vector_wavelenght[i]<<std::endl;
        }
        fout.close();

        // Call 6S function main
        otb_6s_ssssss_otb_main_function(        &asol, &phi0, &avis, &phiv, &month, &jday, 
                                                &pressure, &uw, &uo3, 
	                                        &iaer, 
                                                &taer55, 
                                                &wlinf, &wlsup, 
                                                s, 
                                                &otb_ratm__, 
                                                &sast, 
                                                &tgasm, 
                                                &sdtott, 
                                                &sutott);
        delete [] s;
        s = NULL;

        fout.open(argv[2]);
        fout << "Inputs parameters:"<<std::endl;
        fout <<asol<<" "<<phi0 <<" "<<avis <<" "<<phiv <<" "<<month <<" "<<jday <<std::endl;
        fout <<pressure<<" "<<uw <<" "<<uo3<<std::endl;
        fout <<iaer<<std::endl;
        fout <<taer55<<std::endl;
        fout <<wlinf<<" "<<uw <<" "<<wlsup<<std::endl;
        fout <<"Wavelenght values ["<<vector_wavelenght.size()<<"]"<<std::endl;
        for(unsigned int i=0 ; i<vector_wavelenght.size() ; i++)
        {
                fout <<vector_wavelenght[i]<<std::endl;
        }
        fout << "Outputs values:"<<std::endl;
        fout << "   ----->  atmospheric reflectance :      "<<otb_ratm__<<std::endl;
        fout << "   ----->  atmospheric spherical albedo : "<<sast<<std::endl;
        fout << "   ----->  total gaseous transmission :   "<<tgasm<<std::endl;
        fout << "   ----->  downward transmittance :       "<<sdtott<<std::endl;
        fout << "   ----->  upward transmittance :         "<<sutott<<std::endl;
        fout.close();
  
    }
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch (...) 
    {
    std::cerr << "svmTest exception thrown" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS; 
};











 



