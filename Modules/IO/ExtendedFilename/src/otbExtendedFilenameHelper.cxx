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
#include "otbExtendedFilenameHelper.h"
#include "otb_boost_string_header.h"
namespace otb
{

void
ExtendedFilenameHelper
::SetExtendedFileName(const char *extFname)
{
  if (extFname == NULL)
  {
    itkGenericExceptionMacro( << "Filename is NULL" );
  }
  this->m_ExtendedFileName = extFname;
  this->m_OptionMap.clear();
  std::vector<std::string> tmp1;
  std::vector<std::string> tmp2;
  if (!m_ExtendedFileName.empty())
    {
    boost::split(tmp1, m_ExtendedFileName, boost::is_any_of("?"), boost::token_compress_on);
    this->SetSimpleFileName(tmp1[0]);
    if (tmp1.size()>1)
      {
      boost::split(tmp2, tmp1[1], boost::is_any_of("&"), boost::token_compress_on);
      for (unsigned int i=0; i<tmp2.size(); i++)
       if (tmp2[i].length() >0)
        {
			std::vector<std::string> tmp;
			boost::split(tmp, tmp2[i], boost::is_any_of("="), boost::token_compress_on);
			if (tmp.size()>1)
				{
				  if (tmp[1].length()>0)
					  {
						  if (m_OptionMap[tmp[0]].empty())
							{
								m_OptionMap[tmp[0]]=tmp[1];
							}
						  else
							{
								itkWarningMacro("Duplicated option detected: " << tmp[0] << ". Using value " << tmp[1] << ".");
							}
					  }
				  else
					itkGenericExceptionMacro( << "Value for option '" << tmp[0] << "' is not set.");
				}
        }
      }
    }
}


ExtendedFilenameHelper::OptionMapType
ExtendedFilenameHelper
::GetOptionMap(void) const
{
  return this->m_OptionMap;
}

} // end namespace otb
