/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbExtendedFilenameHelper.h"
#include "otb_boost_string_header.h"
namespace otb
{

void
ExtendedFilenameHelper
::SetExtendedFileName(const char *extFname)
{
  if (extFname == ITK_NULLPTR)
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


const ExtendedFilenameHelper::OptionMapType &
ExtendedFilenameHelper
::GetOptionMap(void) const
{
  return this->m_OptionMap;
}

} // end namespace otb
