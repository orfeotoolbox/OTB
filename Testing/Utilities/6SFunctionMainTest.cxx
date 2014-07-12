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
#include <iomanip>
#include <fstream>
#include <vector>
#include <cstdlib>

#include "otb_6S.h"
#include "main_6s.h"
#include "itkMacro.h"

int SIXSFunctionMainOtbTest( int argc, char *argv[] )
{

  try
    {

        if(argc!=3)
        {
                std::cout << argv[0] <<" <input filename> <output filename>" << std::endl;
                return EXIT_FAILURE;
        }
        otb_6s_doublereal asol(0.), phi0(0.), avis(0.), phiv(0.);
        otb_6s_integer month(0), jday(0);
        otb_6s_doublereal pressure(0.), uw(0.), uo3(0.);
        otb_6s_integer iaer(0);
        otb_6s_doublereal taer55(0.);
        otb_6s_doublereal wlinf(0.), wlsup(0.);
        otb_6s_doublereal value(0.);
        otb_6s_doublereal otb_ratm__(0.), sast(0.), tgasm(0.), sdtott(0.), sutott(0.);
        otb_6s_doublereal tdif_up (0.),tdir_up(0.), tdif_up_ray(0.), tdif_up_aer(0.);

        otb_6s_integer iinf(0), isup(0);
        const unsigned int S_SIZE=1501;
        std::vector<otb_6s_doublereal> vector_wavelength;

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

        while (fin.good())
        {
                fin >> value;
                vector_wavelength.push_back(value);
        }
        fin.close();

        otb_6s_doublereal * s(NULL);
        s = new otb_6s_doublereal[S_SIZE];
        for(unsigned int i=0 ; i<S_SIZE ; i++)
        {
                s[i]=0;
        }

        iinf = static_cast<otb_6s_integer>((wlinf - (float).25) / (float).0025 + (float)1.5);
        isup = static_cast<otb_6s_integer>((wlsup - (float).25) / (float).0025 + (float)1.5);
        otb_6s_integer cpt=iinf-1;
        for(unsigned int i=0 ; cpt<isup ; i++)
        {
                s[cpt] = vector_wavelength[i];
                cpt++;
        }

        // Call 6S function main
        std::cout << "Start otb_6s_ssssss_otb_main_function() ..."<<std::endl;

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
                                                &sutott,
                                                &tdif_up,
                                                &tdir_up,
                                                &tdif_up_ray,
                                                &tdif_up_aer);

        std::cout << "Done !"<<std::endl;
        delete [] s;
        s = NULL;

        fout.open(argv[2]);
        fout <<" ---------------------------------------------------------"<<std::endl;
        fout << "Inputs values:"<<std::endl;
        fout << "   ----->  asol :                                      "<<asol<<std::endl;
        fout << "   ----->  phi0 :                                      "<<phi0<<std::endl;
        fout << "   ----->  avis :                                      "<<avis<<std::endl;
        fout << "   ----->  phiv :                                      "<<phiv<<std::endl;
        fout << "   ----->  month :                                     "<<month<<std::endl;
        fout << "   ----->  jday :                                      "<<jday<<std::endl;
        fout << "   ----->  pressure :                                  "<<pressure<<std::endl;
        fout << "   ----->  uw :                                        "<<uw<<std::endl;
        fout << "   ----->  uo3 :                                       "<<uo3<<std::endl;
        fout << "   ----->  iaer :                                      "<<iaer<<std::endl;
        fout << "   ----->  taer55 :                                    "<<taer55<<std::endl;
        fout << "   ----->  wlinf :                                     "<<wlinf<<std::endl;
        fout << "   ----->  wlsup :                                     "<<wlsup<<std::endl;
        fout << "   ----->  iinf :                                      "<<iinf<<std::endl;
        fout << "   ----->  isup :                                      "<<isup<<std::endl;
        fout <<" ---------------------------------------------------------"<<std::endl;
        fout << "Outputs values:"<<std::endl;
        fout << "   ----->  atmospheric reflectance :                   "<<std::setprecision(10)<<otb_ratm__<<std::endl;
        fout << "   ----->  atmospheric spherical albedo :              "<<sast<<std::endl;
        fout << "   ----->  total gaseous transmission :                "<<tgasm<<std::endl;
        fout << "   ----->  downward transmittance :                    "<<sdtott<<std::endl;
        fout << "   ----->  upward transmittance :                      "<<sutott<<std::endl;
        fout << "   ----->  upward diffuse transmittance :              "<<tdif_up<<std::endl;
        fout << "   ----->  upward direct transmittance :               "<<tdir_up<<std::endl;
        fout << "   ----->  upward diffuse transmittance for rayleigh : "<<tdif_up_ray<<std::endl;
        fout << "   ----->  upward diffuse transmittance for aerosols : "<<tdif_up_aer<<std::endl;
        fout <<" ---------------------------------------------------------"<<std::endl;
        fout << "   ----->  Inputs wavelength values ["<<vector_wavelength.size()<<"]"<<std::endl;
        for(unsigned int i=0 ; i<vector_wavelength.size() ; i++)
        {
                fout << "            "<<vector_wavelength[i]<<std::endl;
        }

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
