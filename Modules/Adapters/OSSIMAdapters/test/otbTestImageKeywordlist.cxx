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

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include <otbImageKeywordlist.h>
#include <iostream>
#include <sstream>
#include <iterator>

double convertToDouble(const std::string& s)
{
  std::istringstream i(s);
  double x;
  i >> x;
  return x;
}

int otbTestImageKeywordlist(int argc, char* argv[])
{
  const unsigned int Dimension = 2;
  typedef double InputPixelType;
  typedef otb::VectorImage<InputPixelType, Dimension>  InputImageType;
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  
  const char*        inputFilename = argv[1];
  std::istringstream issNeededKw(argv[2]);
  std::istringstream issTols(argv[3]);
  char * outGeomFilename = argv[4];
  bool checkAllKw = argv[5];
  
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->UpdateOutputInformation();
  
  otb::ImageKeywordlist kwlist = reader->GetOutput()->GetImageKeywordlist();
  
  if (!(kwlist.GetSize() > 0))
  {
       std::cerr<<"Error : imageKeywordlist is empty."<<std::endl;
       return EXIT_FAILURE;
  } 

  typedef otb::ImageKeywordlist::KeywordlistMap KeywordlistMapType;
  KeywordlistMapType kwmap = kwlist.GetKeywordlist();
  
  if (checkAllKw) // Check all keywords (ignore argv[2] and argv[3])
  {
	  
	  // Write keywordlist, read it, compare it to the original list
	  WriteGeometry(kwlist,outGeomFilename);
	  otb::ImageKeywordlist kwlist2 = otb::ReadGeometryFromGEOMFile(outGeomFilename);
	  KeywordlistMapType kwmap2 = kwlist2.GetKeywordlist();
	  
	  if (kwmap.size() != kwmap2.size() )
	  {
		std::cerr << "Error : keyword lists don't have the same size (orig / reloaded): " << kwmap.size() << " / " << kwmap2.size() << std::endl;
		return EXIT_FAILURE;
	  }
	  
	  KeywordlistMapType::iterator kwlistIt = kwmap.begin();
	  
	  double val1,val2;
	  while ( kwlistIt!=kwmap.end() )
	  {
		  val1 = convertToDouble(kwlistIt->second);
		  
		  KeywordlistMapType::iterator it = kwmap2.find(kwlistIt->first);
		  if (it != kwmap2.end() )
		  {
		    val2 = convertToDouble(it->second);
		  }
		  else
		  {
		     std::cerr << "Error : couldn't find key " << kwlistIt->first << " in reloaded keyword list." << std::endl;
			 return EXIT_FAILURE;  
		  }
		  if ( fabs( val2 - val1 ) > 0.01 )
				{
					std::cerr << "Error : orig key/value (" << kwlistIt->first << " " << kwlistIt->second << ")"<< std::endl
					    << "reloaded key/value (" << kwlistIt->first << " " << kwmap2[kwlistIt->first] << ")"<< std::endl;
					return EXIT_FAILURE;
				}
		  ++kwlistIt;
	  }
	  
  }
  else // Check only needed keywords
  {
	  // #1 keywordlist, only check the needed keywords
	  /*-------------------------------------*/
	  
	  //Split the string into many tokens, with whitespaces as separators
	  std::list<std::string> neededKw;
	  copy(std::istream_iterator<std::string>(issNeededKw),
		 std::istream_iterator<std::string>(),
		 back_inserter(neededKw));
		 
	  std::list<std::string> missingKw;
	  for(std::list<std::string>::iterator neededIt=neededKw.begin(); neededIt!=neededKw.end(); ++neededIt)
	  {
		bool foundNeededKw = false;
		for(KeywordlistMapType::iterator kwlistIt=kwmap.begin(); kwlistIt!=kwmap.end(); ++kwlistIt)
		{
			std::size_t found = kwlistIt->first.find(*neededIt);
			if (found!=std::string::npos)
			{   
				foundNeededKw = true;
			}
		}
		
		if (!foundNeededKw)
			missingKw.push_back(*neededIt);
	  }
	  
	  if ( (neededKw.size()>0) && (missingKw.size()>0) )
	  {
		std::cerr << "Error : some keywords were not found; missing keywords : " << std::endl;
		for (std::list<std::string>::iterator itm = missingKw.begin(); itm != missingKw.end(); ++itm)  
		   std::cerr << *itm << std::endl;
		return EXIT_FAILURE;
	  }
	  /*-------------------------------------*/
	  
	  
	  // #2 Write keywordlist, read it, compare it to the original list
	  WriteGeometry(kwlist,outGeomFilename);
	  otb::ImageKeywordlist kwlist2 = otb::ReadGeometryFromGEOMFile(outGeomFilename);
	  KeywordlistMapType kwmap2 = kwlist2.GetKeywordlist();
	  
	  //Split the string into many tokens, with whitespaces as separators
	  std::list<std::string> tols;
	  copy(std::istream_iterator<std::string>(issTols),
		 std::istream_iterator<std::string>(),
		 back_inserter(tols));
		 
	  if (tols.size() != neededKw.size() )
	  {
		std::cerr << "Error : needed keywords list should have the same size as the tolerances one (needed / tols): " 
		          << neededKw.size() << " " << tols.size() << std::endl;
		return EXIT_FAILURE; 
	  }
		 
	  std::map<std::string,double> mapKwTol;
	  std::pair<std::map<std::string,double>::iterator,bool> ret;
	  std::list<std::string>::iterator tolsIt=tols.begin();
	  std::list<std::string>::iterator neededKwIt=neededKw.begin();
	  while ( (tolsIt!=tols.end()) && (neededKwIt!=neededKw.end()) )
	  {		
	     mapKwTol.insert(  std::pair<std::string,double>(*neededKwIt,  convertToDouble(*tolsIt) )   );
	     ++tolsIt;
	     ++neededKwIt;
	  }
	  
	  if (kwmap.size() != kwmap2.size() )
	  {
		std::cerr << "Error : keyword lists don't have the same size (orig / reloaded): " << kwmap.size() << " / " << kwmap2.size() << std::endl;
		return EXIT_FAILURE;
	  }
	  
	  for(std::list<std::string>::iterator neededIt=neededKw.begin(); neededIt!=neededKw.end(); ++neededIt)
	  {
		KeywordlistMapType::iterator kwlistIt = kwmap.begin();
		KeywordlistMapType::iterator kwlistIt2 = kwmap2.begin();
		
		while ( (kwlistIt!=kwmap.end()) && (kwlistIt2!=kwmap2.end()) )
		{
			std::size_t found = kwlistIt->first.find(*neededIt);
			if (found!=std::string::npos) // keyword found
			{   
				
				if ( fabs( convertToDouble(kwlistIt->second) - convertToDouble(kwlistIt2->second)) > mapKwTol[*neededIt] )
				{
					std::cerr << "Error : orig key/value (" << kwlistIt->first << " " << kwlistIt->second << ")"<< std::endl
					    << "reloaded key/value (" << kwlistIt2->first << " " << kwlistIt2->second << ")"<< std::endl;
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

