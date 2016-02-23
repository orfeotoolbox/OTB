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
#include "itksys/SystemTools.hxx"


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
    ofs << "  _get_comp_words_by_ref cur prev" << std::endl;
    ofs << "  case \"$cur\" in" << std::endl;
    ofs << "    -*)" << std::endl;
    ofs << "      key_list=\"";
    for (unsigned int k=0 ; k < keys.size() ; k++ )
      {
      ofs << "-" << keys[k] << " ";
      }
    ofs << "\"" << std::endl;
    ofs << "      COMPREPLY=( $( compgen -W '$key_list' -- $cur) )" << std::endl;
    ofs << "      return 0" << std::endl;
    ofs << "      ;;" << std::endl;
    ofs << "  esac" << std::endl;
  
  //case "$prev" in
    //-type)
    //key_list="none linear log2"
    //COMPREPLY=( $( compgen -W '$key_list' -- $cur) )
    //;;
  //esac

    ofs << "  return 0" << std::endl;
    ofs << "}" << std::endl;
    ofs << "complete -o default -F _otbcli_" << appList[i] << " otbcli_"<< appList[i] << std::endl;
    }
  ofs.close();

  return EXIT_SUCCESS;
}
