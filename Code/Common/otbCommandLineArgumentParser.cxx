#include "otbCommandLineArgumentParser.h"

#include <assert.h>
#include <iostream>

namespace otb
{

void 
CommandLineArgumentParser
::AddOption(const char *name, const int nParameters, const char * comment)
{
  // Create a structure for the command
  OptionType option;
  option.CommonName = std::string(name);
  option.CommentName = std::string(comment);
  option.NumberOfParameters = nParameters;

  // Add the option to the map
  m_OptionMap[std::string(name)] = option;
}

void 
CommandLineArgumentParser
::AddSynonim(const char *option, const char *synonim)
{
  std::string strOption(option);
  std::string strSynonim(synonim);

  // The option should exist!
  assert(m_OptionMap.find(strOption) != m_OptionMap.end());

  // Create a new option object
  OptionType o;
  o.NumberOfParameters = m_OptionMap[strOption].NumberOfParameters;
  o.CommentName = m_OptionMap[strOption].CommentName;
  o.CommonName = strOption;

  // Insert the option into the map
  m_OptionMap[strSynonim] = o;
}

bool 
CommandLineArgumentParser
::TryParseCommandLine(int argc, char *argv[], 
                      CommandLineArgumentParseResult &outResult,
                      bool failOnUnknownTrailingParameters)
{
  // Clear the result
  outResult.Clear();

  m_ProgramName = std::string(argv[0]);

  // Go through the arguments
  for(int i=1; i < argc; i++)
    {
    // Get the next argument
    std::string arg(argv[i]);

    // Check if the argument is known
    if(m_OptionMap.find(arg) == m_OptionMap.end())
      {
      if(failOnUnknownTrailingParameters)
        {
        // Unknown argument found
        std::cerr << "Unrecognized command line option '" << arg << "'" << std::endl;
        return false;
        }
      else return true;
      }

    // Check if the number of parameters is correct
    int nParameters = m_OptionMap[arg].NumberOfParameters;
    if(i+nParameters >= argc) 
      {
      // Too few parameters
      std::cerr << "Too few parameters to command line option '" << arg << "'" << std::endl;
      return false;
      }

    // Tell the result that the option has been encountered
    outResult.AddOption(m_OptionMap[arg].CommonName,nParameters);

    // Pass in the parameters
    for(int j=0;j<nParameters;j++,i++)
      outResult.AddParameter(m_OptionMap[arg].CommonName,std::string(argv[i+1]));
    
    }

  // Everything is good
  return true;
}

void
CommandLineArgumentParser
::PrintUsage(std::ostream& os/*, itk::Indent indent*/)const
{
        os << " Usage : "<<m_ProgramName<<std::endl;
        OptionMapType::const_iterator iterMap = m_OptionMap.begin();
        
//        iterMap.first
/*        while ( ! iterMap.end() )
        {
                os << iterMap<<std::endl;
                ++iterMap;
        }*/

}


// --------- CommandLineArgumentParseResult  ----------------------------------------
void
CommandLineArgumentParseResult
::PrintSelf(std::ostream& os/*, itk::Indent indent*/)const
{


}








bool 
CommandLineArgumentParseResult
::IsOptionPresent(const char *option)
{
  return (m_OptionMap.find(std::string(option)) != m_OptionMap.end());
}

const char * 
CommandLineArgumentParseResult
::GetOptionParameter(const char *option, unsigned int number)
{
  assert(IsOptionPresent(option));
  assert(number < m_OptionMap[std::string(option)].size());

  return m_OptionMap[std::string(option)][number].c_str();
}


void  
CommandLineArgumentParseResult
::Clear()
{
  m_OptionMap.clear();
}

void  
CommandLineArgumentParseResult
::AddOption(const std::string &option, int nParms)
{
  ParameterArrayType pat;
  pat.reserve(nParms);
  m_OptionMap[option] = pat;
}

void  
CommandLineArgumentParseResult
::AddParameter(const std::string &option, const std::string &parameter)
{
  m_OptionMap[option].push_back(parameter);  
}

}


