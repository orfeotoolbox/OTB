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

#include <boost/fusion/include/std_pair.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

BOOST_FUSION_ADAPT_STRUCT(otb::internal::SingleBandRange, (int,index) );

namespace otb
{

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct BandRangeParser
  : qi::grammar<Iterator, std::vector<GenericBandRange >()>
{
BandRangeParser()
  : BandRangeParser::base_type(query)
{
    query =  (pair | pelem ) % qi::lit(';');
    pair  =  -(qi::int_) >> ':' >> -(qi::int_);
    pelem =  qi::int_;
}
qi::rule<Iterator, std::vector<GenericBandRange >()> query;
qi::rule<Iterator, std::pair<int,int>()> pair;
qi::rule<Iterator, internal::SingleBandRange()> pelem;
};

template <typename Iterator>
struct OptionParser
  : qi::grammar<Iterator, ExtendedFilenameHelper::OptionMapType()>
{
OptionParser()
  : OptionParser::base_type(query)
  {
  query = ( pair | '&' >> pair ) >> *( '&' >> pair);
  pair = key >> '=' >> key;
  key = +(~qi::char_("=&"));
  }
qi::rule<Iterator, ExtendedFilenameHelper::OptionMapType()> query;
qi::rule<Iterator, std::pair<std::string, std::string>()> pair;
qi::rule<Iterator, std::string()> key;
};


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
      OptionParser<std::string::iterator> p;    // create instance of parser
      bool result = qi::parse(tmp1[1].begin(), tmp1[1].end(), p, m_OptionMap);
      if (!result)
        {
        itkGenericExceptionMacro( << "Can't parse extended filename :"<<tmp1[1]);
        }
      else
        {
        for (OptionMapType::iterator iter=m_OptionMap.begin() ; iter != m_OptionMap.end() ; ++iter)
          {
          std::cout << iter->first << " -> "<< iter->second << std::endl;
          }
        }

#if 0
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
#endif
      }
    }
}


ExtendedFilenameHelper::OptionMapType
ExtendedFilenameHelper
::GetOptionMap(void) const
{
  return this->m_OptionMap;
}

bool
ExtendedFilenameHelper
::ParseBandRange(std::string value, std::vector<GenericBandRange>& output)
{
  std::string::iterator begin = value.begin();
  std::string::iterator end = value.end();

  BandRangeParser<std::string::iterator> p;    // create instance of parser
  std::vector<GenericBandRange > m;        // map to receive results
  bool result = qi::parse(begin, end, p, m);

  if (result)
    {
    output = m;
    }
  return result;
}

} // end namespace otb
