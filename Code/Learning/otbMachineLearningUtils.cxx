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

#include "otbMachineLearningUtils.h"

#include <fstream>
#include <string>
#include <algorithm>

bool ReadDataFile(const char * infname, InputListSampleType * samples, TargetListSampleType * labels)
{
  std::ifstream ifs;
  ifs.open(infname);

  if(!ifs)
    {
    std::cerr<<"Could not read file "<<infname<<std::endl;
    return false;
    }

  unsigned int nbfeatures = 0;

    while (!ifs.eof())
      {
      std::string line;
      std::getline(ifs, line);

      if(nbfeatures == 0)
        {
        nbfeatures = std::count(line.begin(),line.end(),' ')-1;
        std::cout<<"Found "<<nbfeatures<<" features per samples"<<std::endl;
        }

      if(line.size()>1)
        {

        InputSampleType sample(nbfeatures);
        sample.Fill(0);

        std::string::size_type pos = line.find_first_of(" ", 0);

        // Parse label
        TargetSampleType label;
        label[0] = atoi(line.substr(0, pos).c_str());

        bool endOfLine = false;

        unsigned int id = 0;

        while(!endOfLine)
          {
          std::string::size_type nextpos = line.find_first_of(" ", pos+1);

          if(nextpos == std::string::npos)
            {
            endOfLine = true;
            nextpos = line.size()-1;
            }
          else
            {
            std::string feature = line.substr(pos,nextpos-pos);
            std::string::size_type semicolonpos = feature.find_first_of(":");
            id = atoi(feature.substr(0,semicolonpos).c_str());
            sample[id] = atof(feature.substr(semicolonpos+1,feature.size()-semicolonpos).c_str());
            pos = nextpos;
            }
          }
        samples->PushBack(sample);
        labels->PushBack(label);
        }
      }

    std::cout<<"Retrieved "<<samples->Size()<<" samples"<<std::endl;

  ifs.close();

  return true;
}
