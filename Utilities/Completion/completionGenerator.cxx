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

#include "otbWrapperApplicationRegistry.h"
#include "otbWrapperParameter.h"
#include "otbWrapperChoiceParameter.h"
#include "itksys/SystemTools.hxx"

/**
 * Small executable to :
 *  - find available application
 *  - get list of parameters
 *  - and fill a bash completion script
 *
 * This script can be sourced to be used, or deployed into a folder such as
 *    /etc/bash_completion.d
 * For choice parameters, the completion will propose the available choices
 */

int main(int argc, char* argv[])
{
  if (argc < 3)
    {
    std::cerr << "Usage : " << argv[0] << " module_path  output_script" << std::endl;
    return EXIT_FAILURE;
    }  
  std::string module_path(argv[1]);
  std::string output_path(argv[2]);

  otb::Wrapper::ApplicationRegistry::SetApplicationPath(module_path);
  
  std::vector<std::string> appList = otb::Wrapper::ApplicationRegistry::GetAvailableApplications(false);
  
  otb::Wrapper::Application::Pointer appPtr;
  
  std::ofstream ofs;
  ofs.open(output_path.c_str());
  
  for (unsigned int i=0 ; i < appList.size() ; ++i)
    {
    appPtr = otb::Wrapper::ApplicationRegistry::CreateApplication(appList[i],false);
    if (appPtr.IsNull())
      {
      std::cout << "Error loading application "<< appList[i] << std::endl;
      return EXIT_FAILURE;
      }
    std::vector<std::string> keys = appPtr->GetParametersKeys();
    
    ofs << "# completion for application "<< appList[i] << std::endl;
    ofs << "_otbcli_"<<appList[i]<<"()" << std::endl;
    ofs << "{" << std::endl;
    
    ofs << "  local cur prev" << std::endl;
    ofs << "  COMPREPLY=()" << std::endl;
    ofs << "  cur=\"${COMP_WORDS[COMP_CWORD]}\"" << std::endl;
    ofs << "  prev=\"${COMP_WORDS[COMP_CWORD-1]}\"" << std::endl;
    ofs << "  case \"$cur\" in" << std::endl;
    ofs << "    -*)" << std::endl;
    ofs << "      key_list=\"";
    for (unsigned int k=0 ; k < keys.size() ; k++ )
      {
      ofs << "-" << keys[k] << " ";
      }
    ofs << "\"" << std::endl;
    ofs << "      COMPREPLY=( $( compgen -W \"$key_list\" -- $cur) )" << std::endl;
    ofs << "      return 0" << std::endl;
    ofs << "      ;;" << std::endl;
    ofs << "  esac" << std::endl;

    // detect choice parameters
    std::vector<std::string> choiceKeys;
    for (unsigned int k=0 ; k < keys.size() ; k++ )
      {
      if ( appPtr->GetParameterType(keys[k]) == otb::Wrapper::ParameterType_Choice)
        {
        choiceKeys.push_back(keys[k]);
        }
      }
    if (choiceKeys.size())
      {
      ofs << "  case \"$prev\" in" << std::endl;
      for (unsigned int k=0 ; k < choiceKeys.size() ; k++)
        {
        ofs << "    -" << choiceKeys[k] << ")" << std::endl;
        ofs << "      key_list=\"";
        std::vector<std::string> choices = dynamic_cast<otb::Wrapper::ChoiceParameter*>(appPtr->GetParameterByKey(choiceKeys[k]))->GetChoiceKeys();
        for (unsigned int j=0 ; j < choices.size() ; j++)
          {
          ofs << choices[j];
          if (j+1 < choices.size() )
            {
            ofs << " ";
            }
          }
        ofs << "\"" << std::endl;
        ofs << "      COMPREPLY=( $( compgen -W \"$key_list\" -- $cur) )" << std::endl;
        ofs << "      ;;" << std::endl;
        }
      ofs << "  esac" << std::endl;
      }

    ofs << "  return 0" << std::endl;
    ofs << "}" << std::endl;
    ofs << "complete -o default -F _otbcli_" << appList[i] << " otbcli_"<< appList[i] << std::endl;
    }
  ofs.close();

  return EXIT_SUCCESS;
}
