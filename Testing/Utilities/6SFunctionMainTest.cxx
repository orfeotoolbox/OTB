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
        
        if(argc!=3)
        {
                std::cout << argv[0] <<" <input filename> <output filename>" << std::endl;
                return EXIT_FAILURE;
        }
        otb_6s_real asol(0.), phi0(0.), avis(0.), phiv(0.);
        otb_6s_integer month(0), jday(0);
        otb_6s_real pressure(0.), uw(0.), uo3(0.); 
        otb_6s_integer iaer(0);
        otb_6s_real taer55(0.);
        otb_6s_real wlinf(0.), wlsup(0.);
        otb_6s_real value(0.);
        otb_6s_real otb_ratm__(0.), sast(0.), tgasm(0.), sdtott(0.), sutott(0.);
        otb_6s_integer iinf(0), isup(0);
        const unsigned int S_SIZE=1501;
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
        
        while (!fin.eof() && fin.good())
        {
                fin >> value;
                vector_wavelenght.push_back(value);
        }
        fin.close();

        otb_6s_real * s(NULL);
        s = new otb_6s_real[S_SIZE];
        for(unsigned int i=0 ; i<S_SIZE ; i++)
        {
                s[i]=0;
        }

        iinf = static_cast<otb_6s_integer>((wlinf - (float).25) / (float).0025 + (float)1.5);
        isup = static_cast<otb_6s_integer>((wlsup - (float).25) / (float).0025 + (float)1.5);
        otb_6s_integer cpt=iinf-1;
        for(unsigned int i=0 ; cpt<isup ; i++)
        {
                s[cpt] = vector_wavelenght[i];
                cpt++;
        }
        
/*        asol = 30.;
        phi0 = 0.;
        avis = 45.;
        phiv = 90.;
        month = 7;
        jday = 15;
        pressure = 1013.;
        uw = 1.424;
        uo3 = 0.344;
        iaer = 1;
        taer55 = 0.1;
        wlinf = 0.580;
        wlsup = 0.745;


        otb_6s_integer cpt=iinf-1;
//        s = new otb_6s_real[67];
        s[cpt] = 0.0000;	
        cpt++;s[cpt] = 0.0000;
        cpt++;s[cpt] = 0.0000;
        cpt++;s[cpt] = 0.0000;
        cpt++;s[cpt] = 0.0000;
        cpt++;s[cpt] = 0.0000;
        cpt++;s[cpt] = 0.0000;
        cpt++;s[cpt] = 0.0010;
        cpt++;s[cpt] = 0.0020;
        cpt++;s[cpt] = 0.0120;
        cpt++;s[cpt] = 0.0220;
        cpt++;s[cpt] = 0.0695;
        cpt++;s[cpt] = 0.1170;
        cpt++;s[cpt] = 0.2160;
        cpt++;s[cpt] = 0.3150;
        cpt++;s[cpt] = 0.4245;
        cpt++;s[cpt] = 0.5340;
        cpt++;s[cpt] = 0.6195;
        cpt++;s[cpt] = 0.7050;
        cpt++;s[cpt] = 0.7655;
        cpt++;s[cpt] = 0.8260;
        cpt++;s[cpt] = 0.8640;
        cpt++;s[cpt] = 0.9020;
        cpt++;s[cpt] = 0.9270;
        cpt++;s[cpt] = 0.9520;
        cpt++;s[cpt] = 0.9660;
        cpt++;s[cpt] = 0.9800;
        cpt++;s[cpt] = 0.9870;
        cpt++;s[cpt] = 0.9940;
        cpt++;s[cpt] = 0.9970;
        cpt++;s[cpt] = 1.0000;
        cpt++;s[cpt] = 0.9985;
        cpt++;s[cpt] = 0.9970;
        cpt++;s[cpt] = 0.9895;
        cpt++;s[cpt] = 0.9820;
        cpt++;s[cpt] = 0.9635;
        cpt++;s[cpt] = 0.9450;
        cpt++;s[cpt] = 0.9040;
        cpt++;s[cpt] = 0.8630;
        cpt++;s[cpt] = 0.7900;
        cpt++;s[cpt] = 0.7170;
        cpt++;s[cpt] = 0.6255;
        cpt++;s[cpt] = 0.5340;
        cpt++;s[cpt] = 0.4450;
        cpt++;s[cpt] = 0.3560;
        cpt++;s[cpt] = 0.2895;
        cpt++;s[cpt] = 0.2230;
        cpt++;s[cpt] = 0.1780;
        cpt++;s[cpt] = 0.1330;
        cpt++;s[cpt] = 0.1060;
        cpt++;s[cpt] = 0.0790;
        cpt++;s[cpt] = 0.0635;
        cpt++;s[cpt] = 0.0480;
        cpt++;s[cpt] = 0.0385;
        cpt++;s[cpt] = 0.0290;
        cpt++;s[cpt] = 0.0235;
        cpt++;s[cpt] = 0.0180;
        cpt++;s[cpt] = 0.0145;
        cpt++;s[cpt] = 0.0110;
        cpt++;s[cpt] = 0.0090;
        cpt++;s[cpt] = 0.0070;
        cpt++;s[cpt] = 0.0055;
        cpt++;s[cpt] = 0.0040;
        cpt++;s[cpt] = 0.0030;
        cpt++;s[cpt] = 0.0020;
        cpt++;s[cpt] = 0.0010;
        cpt++;s[cpt] = 0.0000;
*/        

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
                                                &sutott);
        std::cout << "Done !"<<std::endl;
        delete [] s;
        s = NULL;

        fout.open(argv[2]);
        fout <<" ---------------------------------------------------------"<<std::endl;
        fout << "Inputs values:"<<std::endl;
        fout << "   ----->  atmospheric reflectance :      "<<otb_ratm__<<std::endl;
        fout << "   ----->  asol :                         "<<asol<<std::endl;
        fout << "   ----->  phi0 :                         "<<phi0<<std::endl;
        fout << "   ----->  avis :                         "<<avis<<std::endl;
        fout << "   ----->  phiv :                         "<<phiv<<std::endl;
        fout << "   ----->  month :                        "<<month<<std::endl;
        fout << "   ----->  jday :                         "<<jday<<std::endl;
        fout << "   ----->  pressure :                     "<<pressure<<std::endl;
        fout << "   ----->  uw :                           "<<uw<<std::endl;
        fout << "   ----->  uo3 :                          "<<uo3<<std::endl;
        fout << "   ----->  iaer :                         "<<iaer<<std::endl;
        fout << "   ----->  taer55 :                       "<<taer55<<std::endl;
        fout << "   ----->  wlinf :                        "<<wlinf<<std::endl;
        fout << "   ----->  wlsup :                        "<<wlsup<<std::endl;
        fout << "   ----->  iinf :                         "<<iinf<<std::endl;
        fout << "   ----->  isup :                         "<<isup<<std::endl;
        fout << "   ----->  Inputs wavelenght values ["<<vector_wavelenght.size()<<"]"<<std::endl;
        for(unsigned int i=0 ; i<vector_wavelenght.size() ; i++)
        {
                fout << "            "<<vector_wavelenght[i]<<std::endl;
        }
        fout <<" ---------------------------------------------------------"<<std::endl;
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











 



