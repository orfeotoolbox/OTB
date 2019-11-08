/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include <otbImageKeywordlist.h>
#include <iostream>
#include <sstream>
#include <iterator>

double convertStringToDouble(const std::string& s)
{
  std::istringstream i(s);
  double             x;
  if (!(i >> x))
    return 0;
  return x;
}

int otbTestImageKeywordlist(int argc, char* argv[])
{

  if (argc != 6)
  {
    std::cout << argv[0] << " <input img filename> <input needed keyword list> <input list of tols for each needed kw> "
              << " <output kw list filename> <bool test all kw>" << std::endl;

    return EXIT_FAILURE;
  }

  const unsigned int Dimension = 2;
  typedef double     InputPixelType;
  typedef otb::VectorImage<InputPixelType, Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType> ReaderType;

  const char*        inputFilename = argv[1];
  std::istringstream issNeededKw(argv[2]);
  std::istringstream issTols(argv[3]);
  char*              outGeomFilename = argv[4];
  bool               checkAllKw      = argv[5];

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();

  otb::ImageKeywordlist kwlist = reader->GetOutput()->GetImageKeywordlist();

  if (!(kwlist.GetSize() > 0))
  {
    std::cerr << "Error : imageKeywordlist is empty." << std::endl;
    return EXIT_FAILURE;
  }

  typedef otb::ImageKeywordlist::KeywordlistMap KeywordlistMapType;
  KeywordlistMapType                            kwmap = kwlist.GetKeywordlist();

  if (checkAllKw) // Check all keywords (ignore argv[2] and argv[3])
  {

    // Write keywordlist, read it, compare it to the original list
    WriteGeometry(kwlist, outGeomFilename);
    otb::ImageKeywordlist kwlist2 = otb::ReadGeometryFromGEOMFile(outGeomFilename);
    KeywordlistMapType    kwmap2  = kwlist2.GetKeywordlist();

    if (kwmap.size() != kwmap2.size())
    {
      std::cerr << "Error : keyword lists don't have the same size (orig / reloaded): " << kwmap.size() << " / " << kwmap2.size() << std::endl;
      return EXIT_FAILURE;
    }

    KeywordlistMapType::iterator kwlistIt = kwmap.begin();

    double val1, val2;
    while (kwlistIt != kwmap.end())
    {
      val1 = convertStringToDouble(kwlistIt->second);

      KeywordlistMapType::iterator it = kwmap2.find(kwlistIt->first);
      if (it != kwmap2.end())
      {
        val2 = convertStringToDouble(it->second);
      }
      else
      {
        std::cerr << "Error : couldn't find key " << kwlistIt->first << " in reloaded keyword list." << std::endl;
        return EXIT_FAILURE;
      }
      if (fabs(val2 - val1) > 0.01)
      {
        std::cerr << "Error : orig key/value (" << kwlistIt->first << " " << kwlistIt->second << ")" << std::endl
                  << "reloaded key/value (" << kwlistIt->first << " " << kwmap2[kwlistIt->first] << ")" << std::endl;
        return EXIT_FAILURE;
      }
      ++kwlistIt;
    }
  }
  else // Check only needed keywords
  {
    // #1 keywordlist, only check the needed keywords
    /*-------------------------------------*/

    // Split the string into many tokens, with whitespaces as separators
    std::list<std::string> neededKw;
    copy(std::istream_iterator<std::string>(issNeededKw), std::istream_iterator<std::string>(), back_inserter(neededKw));

    std::list<std::string> missingKw;
    for (std::list<std::string>::iterator neededIt = neededKw.begin(); neededIt != neededKw.end(); ++neededIt)
    {
      bool foundNeededKw = false;
      for (KeywordlistMapType::iterator kwlistIt = kwmap.begin(); kwlistIt != kwmap.end(); ++kwlistIt)
      {
        std::size_t found = kwlistIt->first.find(*neededIt);
        if (found != std::string::npos)
        {
          foundNeededKw = true;
        }
      }

      if (!foundNeededKw)
        missingKw.push_back(*neededIt);
    }

    if ((neededKw.size() > 0) && (missingKw.size() > 0))
    {
      std::cerr << "Error : some keywords were not found; missing keywords : " << std::endl;
      for (std::list<std::string>::iterator itm = missingKw.begin(); itm != missingKw.end(); ++itm)
        std::cerr << *itm << std::endl;
      return EXIT_FAILURE;
    }
    /*-------------------------------------*/


    // #2 Write keywordlist, read it, compare it to the original list
    WriteGeometry(kwlist, outGeomFilename);
    otb::ImageKeywordlist kwlist2 = otb::ReadGeometryFromGEOMFile(outGeomFilename);
    KeywordlistMapType    kwmap2  = kwlist2.GetKeywordlist();

    // Split the string into many tokens, with whitespaces as separators
    std::list<std::string> tols;
    copy(std::istream_iterator<std::string>(issTols), std::istream_iterator<std::string>(), back_inserter(tols));

    if (tols.size() != neededKw.size())
    {
      std::cerr << "Error : needed keywords list should have the same size as the tolerances one (needed / tols): " << neededKw.size() << " " << tols.size()
                << std::endl;
      return EXIT_FAILURE;
    }

    std::map<std::string, double> mapKwTol;
    std::pair<std::map<std::string, double>::iterator, bool> ret;
    std::list<std::string>::iterator tolsIt     = tols.begin();
    std::list<std::string>::iterator neededKwIt = neededKw.begin();
    while ((tolsIt != tols.end()) && (neededKwIt != neededKw.end()))
    {
      mapKwTol.insert(std::pair<std::string, double>(*neededKwIt, convertStringToDouble(*tolsIt)));
      ++tolsIt;
      ++neededKwIt;
    }

    if (kwmap.size() != kwmap2.size())
    {
      std::cerr << "Error : keyword lists don't have the same size (orig / reloaded): " << kwmap.size() << " / " << kwmap2.size() << std::endl;
      return EXIT_FAILURE;
    }

    for (std::list<std::string>::iterator neededIt = neededKw.begin(); neededIt != neededKw.end(); ++neededIt)
    {
      KeywordlistMapType::iterator kwlistIt  = kwmap.begin();
      KeywordlistMapType::iterator kwlistIt2 = kwmap2.begin();

      while ((kwlistIt != kwmap.end()) && (kwlistIt2 != kwmap2.end()))
      {
        std::size_t found = kwlistIt->first.find(*neededIt);
        if (found != std::string::npos) // keyword found
        {

          if (fabs(convertStringToDouble(kwlistIt->second) - convertStringToDouble(kwlistIt2->second)) > mapKwTol[*neededIt])
          {
            std::cerr << "Error : orig key/value (" << kwlistIt->first << " " << kwlistIt->second << ")" << std::endl
                      << "reloaded key/value (" << kwlistIt2->first << " " << kwlistIt2->second << ")" << std::endl;
            return EXIT_FAILURE;
          }
        }

        ++kwlistIt;
        ++kwlistIt2;
      }
    }
  }
  return EXIT_SUCCESS;
}
