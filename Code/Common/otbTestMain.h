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


#include "itkWin32Header.h"
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h> // fopen, fread, ...

#include "itkNumericTraits.h"
#include "itkMultiThreader.h"
#include "itkImage.h"
#include "otbImageFileReader.h"

//THOMAS
#include "otbImageFileWriter.h"

//ROMAIN
#include "otbMacro.h" // otb::StringStream

#include "itkImageRegionConstIterator.h"
#include "itkSubtractImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkExtractImageFilter.h"
#include "itkDifferenceImageFilter.h"
#include "itkImageRegion.h"

#define ITK_TEST_DIMENSION_MAX 6

typedef int (*MainFuncPointer)(int , char* [] );
std::map<std::string, MainFuncPointer> StringToTestFunctionMap;

#define REGISTER_TEST(test) \
extern int test(int, char* [] ); \
StringToTestFunctionMap[#test] = test

std::map<std::string,int> RegressionTestBaselines (char *);

int RegressionTestImage (const char *, const char *, int, const double);
int RegressionTestBinaryFile(const char *, const char *, int);
int RegressionTestAsciiFile(const char *, const char *, int, const double);

void RegisterTests();
void PrintAvailableTests()
{
  std::cout << "Tests diponibles:\n";
  std::map<std::string, MainFuncPointer>::iterator j = StringToTestFunctionMap.begin();
  int i = 0;
  while(j != StringToTestFunctionMap.end())
    {
    std::cout << i << ". " << j->first << "\n";
    ++i;
    ++j;
    }
}

int main(int ac, char* av[] )
{
  double lToleranceDiffPixelImage(0);
  double lEpsilon(0);
  char *baselineFilenameImage = NULL;
  char *testFilenameImage = NULL;
  char *baselineFilenameBinary = NULL;
  char *testFilenameBinary = NULL;
  char *baselineFilenameAscii = NULL;
  char *testFilenameAscii = NULL;

// On some sgi machines, threads and stl don't mix.
#if defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 730
   itk::MultiThreader::SetGlobalDefaultNumberOfThreads(1);
#endif

  RegisterTests();
  std::string testToRun;
  if(ac < 2)
    {
    PrintAvailableTests();
    std::cout << "Pour lancer un test, entrer son numero: ";
    int testNum = 0;
    std::cin >> testNum;
    std::map<std::string, MainFuncPointer>::iterator j = StringToTestFunctionMap.begin();
    int i = 0;
    while(j != StringToTestFunctionMap.end() && i < testNum)
      {
      ++i;
      ++j;
      }
    if(j == StringToTestFunctionMap.end())
      {
      std::cerr << testNum << " n'est pas un numero de test valid\n";
      return -1;
      }
    testToRun = j->first;
    }
  else
    {
    if (strcmp(av[1], "--with-threads") == 0)
      {
      int numThreads = atoi(av[2]);
      itk::MultiThreader::SetGlobalDefaultNumberOfThreads(numThreads);
      av += 2;
      ac -= 2;
      }
    else if (strcmp(av[1], "--without-threads") == 0)
      {
      itk::MultiThreader::SetGlobalDefaultNumberOfThreads(1);
      av += 1;
      ac -= 1;
      }
    if (strcmp(av[1], "--compare-image") == 0)
      {
      lToleranceDiffPixelImage = (double)(::atof(av[2]));
      baselineFilenameImage = av[3];
      testFilenameImage = av[4];
      av += 4;
      ac -= 4;
      }
    else if (strcmp(av[1], "--compare-binary") == 0)
      {
      baselineFilenameBinary = av[2];
      testFilenameBinary = av[3];
      av += 3;
      ac -= 3;
      }
    else if (strcmp(av[1], "--compare-ascii") == 0)
      {
      lEpsilon = (double)(::atof(av[2]));	
      baselineFilenameAscii = av[3];
      testFilenameAscii = av[4];
      av += 4;
      ac -= 4;
      }
    testToRun = av[1];
    }
  std::map<std::string, MainFuncPointer>::iterator j = StringToTestFunctionMap.find(testToRun);
  if(j != StringToTestFunctionMap.end())
    {
    MainFuncPointer f = j->second;
    int result;
    try
      {
      // Invoke the test's "main" function.
      result = (*f)(ac-1, av+1);
      
      if (result!=0)
       	itkGenericExceptionMacro(<<"Bad function return, no regresion test !");
            
otbMsgDebugMacro(<<"----------------     DEBUT Controle NON-REGRESION  ------------------- ");
      // Make a list of possible baselines
      // Test de non regression sur des images
      if (baselineFilenameImage && testFilenameImage)
        {
        std::map<std::string,int> baselines = RegressionTestBaselines(baselineFilenameImage);
        std::map<std::string,int>::iterator baseline = baselines.begin();
        std::string bestBaseline;
        int bestBaselineStatus = itk::NumericTraits<int>::max();
        while (baseline != baselines.end())
          {
          baseline->second = RegressionTestImage(testFilenameImage,
                                                 (baseline->first).c_str(),
                                                 0,
                                                 lToleranceDiffPixelImage);
          if (baseline->second < bestBaselineStatus)
            {
            bestBaseline = baseline->first;
            bestBaselineStatus = baseline->second;
            }
          if (baseline->second == 0)
            {
            break;
            }
          ++baseline;
          }
        // if the best we can do still has errors, generate the error images
        if (bestBaselineStatus)
          {
          baseline->second = RegressionTestImage(testFilenameImage,
                                                 bestBaseline.c_str(),
                                                 1,
                                                 lToleranceDiffPixelImage);
          }
        result += bestBaselineStatus;
        }
      // Test de non regression sur des fichiers ascii
      if (baselineFilenameAscii && testFilenameAscii)
        {
        
  	std::map<std::string,int> baselines;
  	baselines[std::string(baselineFilenameAscii)] = 0;
        std::map<std::string,int>::iterator baseline = baselines.begin();
        baseline->second = RegressionTestAsciiFile(testFilenameAscii,
                                                 (baseline->first).c_str(),
                                                 0,
                                                 lEpsilon);
        if (baseline->second != 0)
            {
	        baseline->second = RegressionTestAsciiFile(testFilenameAscii,
                                                 (baseline->first).c_str(),
                                                 1,
                                                 lEpsilon);
            }
        result += baseline->second;
        }

      // Test de non regression sur des fichiers binaires
      if (baselineFilenameBinary && testFilenameBinary)
        {
        
  	std::map<std::string,int> baselines;
  	baselines[std::string(baselineFilenameBinary)] = 0;
        std::map<std::string,int>::iterator baseline = baselines.begin();
        baseline->second = RegressionTestBinaryFile(testFilenameBinary,
                                                 (baseline->first).c_str(),
                                                 0);
        if (baseline->second != 0)
            {
	        baseline->second = RegressionTestBinaryFile(testFilenameBinary,
                                                 (baseline->first).c_str(),
                                                 1);
            }
        result += baseline->second;
        }

otbMsgDebugMacro(<<"----------------     FIN Controle NON-REGRESION  ------------------- ");

      }
    catch(const itk::ExceptionObject& e)
      {
      std::cerr << "OTB outil de tests a leve une exception de type ITK:\n";
      std::cerr << e.GetFile() << ":" << e.GetLine() << ":\n"
                << e.GetDescription() << "\n";
      result = -1;
      }
    catch(const std::exception& e)
      {
      std::cerr << "OTB outil de tests a leve une exception standard:\n";
      std::cerr << e.what() << "\n";
      result = -1;
      }
    catch(...)
      {
      std::cerr << "OTB outil de tests a leve une exception non identifiee !!!\n";
      result = -1;
      }
    return result;
    }
  PrintAvailableTests();
  std::cerr << "Echec: " << testToRun << ": Pas de test identifie " << testToRun << "\n";
  return -1;
}

// Regression Testing Code

bool isNumber(int i)
{
	return ((i>47)&&(i<58));
}

bool isPoint(int i)
{
	return (i==46);
}

bool isMinusSign(int i)
{
	return (i==45);
}

bool isNumeric(std::string str)
{
	int nbOfPoints = 0 ;
	int nbOfNumbers = 0 ;
	int number ;
	int i = 0 ;
	bool result = true ;

    	while ((i<str.size())&&(result==true))
	{
		number = str[i];
		
		if (isPoint(number))
			nbOfPoints++ ;
		if (isNumber(number))
			nbOfNumbers++;
		if ((!isNumber(number)&&!isPoint(number)&&!isMinusSign(number))
		  ||(isMinusSign(number)&&(i!=0)))
			result = false ;
			
		i++;
	}
	if ((str.size()==0)||(nbOfPoints > 1)/*||(nbOfNumbers==0)*/)
		result = false ;

	return result;
}

int RegressionTestAsciiFile(const char * testAsciiFileName, const char * baselineAsciiFileName, int reportErrors, const double epsilon)
{
	std::ifstream fluxfiletest(testAsciiFileName);
	std::ifstream fluxfileref(baselineAsciiFileName);
	enum TypeEtat { ETAT_NUM, ETAT_CHAR } ;

	std::string diffAsciiFileName(testAsciiFileName);
	diffAsciiFileName += ".diff.txt" ;
	std::ofstream fluxfilediff(diffAsciiFileName.c_str());
	
	std::string strfiletest;
	std::string strfileref;

	int nbdiff(0);
	int numLine(1);
    	if (!fluxfiletest)
    	{
    		std::cerr << "Impossible to open the test ASCII file <"<<testAsciiFileName<<">.\n";
    		return 1000;
    	}
    	if (!fluxfileref)
    	{
    		std::cerr << "Impossible to open the baseline ASCII file <"<<baselineAsciiFileName<<">.\n";
    		return 1000;
    	}

	TypeEtat etatPrec, etatCour ;

	while ( std::getline(fluxfileref,strfileref)!=0  )
	{
		otb::StringStream buffstreamRef, buffstreamTest ;
					
		std::getline(fluxfiletest,strfiletest);
			
		buffstreamRef << strfileref ;
		buffstreamTest << strfiletest ;
	
		while (buffstreamRef.peek() != EOF)
		{
			std::string strRef;
			std::string strTest;
			
			std::string strNumRef;
			std::string strCharRef;
			std::string strNumTest;
			std::string strCharTest;
			
			buffstreamRef >> strRef ;
			buffstreamTest >> strTest ;
			
			bool chgt= false;
			std::string charTmpRef;
			std::string charTmpTest;
	
			unsigned int i=0;

			while (i < strRef.size())
			{
				charTmpRef=strRef[i];
				charTmpTest=strTest[i];
				
				if (isNumeric(charTmpRef))
					etatCour = ETAT_NUM;
				else 
					etatCour = ETAT_CHAR;
				
				// initialisation de l'état de "référence"
				if (i==0)
					etatPrec=etatCour;

				// Cas où l'on a un chiffre après des caractères
				if ((etatCour==ETAT_NUM)&&(etatPrec==ETAT_CHAR))
				{
					if ( strCharRef != strCharTest )
					{
						fluxfilediff << "Diff at line " << numLine 
						     << " : " << strCharRef
						     << " != " << strCharTest << std::endl ;
						nbdiff++;
					}
										
					strCharRef="";
					strCharTest="";
					strNumRef=charTmpRef;
					strNumTest=charTmpTest;
					chgt=true;
				}
				// Cas où l'on a un caractère après des chiffres
				else if ((etatCour==ETAT_CHAR)&&(etatPrec==ETAT_NUM))
				{
					
					if (fabs(atof(strNumRef.c_str())-atof(strNumTest.c_str())) > epsilon)
					{
						fluxfilediff << "Diff at line " << numLine << " : fabs ( (" 
						     << strNumRef << ") - (" << strNumTest
						     << ") ) > " << epsilon << std::endl ;
						nbdiff++;
					}	
					
					strNumRef="";
					strNumTest="";
					strCharRef=charTmpRef;
					strCharTest=charTmpTest;
					chgt=true;
				}
				else if (etatCour==etatPrec)
				{
					if (etatCour==ETAT_CHAR)
					{
						strCharRef+=charTmpRef;
						strCharTest+=charTmpTest;
					}						
					else
					{
						strNumRef+=charTmpRef;
						strNumTest+=charTmpTest;
					}
				}

				etatPrec = etatCour;
				i++;
			}	
			
			// Cas le plus simple : chaine de caractere ou valeur numérique entre 2 separateurs
			if (!chgt)
			{
				if (isNumeric(strRef))
				{
					
					if (fabs(atof(strRef.c_str())-atof(strTest.c_str())) > epsilon)
					{
						fluxfilediff << "Diff at line " << numLine << " : fabs( (" 
							     << strRef << ") - (" << strTest
							     << ") ) > " << epsilon << std::endl ;
						nbdiff++;
					}
				}
				else 
				{
					if ( strRef != strTest )
					{
						fluxfilediff << "Diff at line " << numLine 
							     << " : " << strRef
							     << " != " << strTest << std::endl ;
						nbdiff++;
					}
				}
			}
		}
		numLine++;
	}
	
	fluxfiletest.close();
	fluxfileref.close();
	fluxfilediff.close();
	
	if ( nbdiff!=0 && reportErrors)
	{
    		std::cout << "<DartMeasurement name=\"ASCIIFileError\" type=\"numeric/int\">";
    		std::cout << nbdiff;
    		std::cout <<  "</DartMeasurement>" << std::endl;
	}
  return (nbdiff != 0) ? 1 : 0;
}

int RegressionTestBinaryFile(const char * testBinaryFileName, const char * baselineBinaryFileName, int reportErrors)
{
	int nbdiff(0);
	FILE *fluxfiletest=NULL;	
	FILE *fluxfileref=NULL;
	fluxfiletest = fopen(testBinaryFileName,"rb");
	fluxfileref = fopen(baselineBinaryFileName,"rb");
	if ( fluxfiletest==NULL	)
	{
    		std::cerr << "Impossible to open the test BINARY file <"<<testBinaryFileName<<">.\n";
    		return 1000;
	}
	if ( fluxfileref==NULL	)
	{
    		std::cerr << "Impossible to open the baseline BINARY file <"<<baselineBinaryFileName<<">.\n";
    		return 1000;
	}

 	while( !feof(fluxfiletest) && !feof(fluxfileref) )
 	{
    		if ( fgetc(fluxfiletest) != fgetc(fluxfileref) )
    		{ 
    			nbdiff++;
    		}
    	}
/*    	if ( feof(fluxfiletest) != feof(fluxfileref) )
    	{ 
      		fprintf(stderr,"L'un des fichiers n'a pas été lu entièrement\n");
    	}*/
 	fclose(fluxfiletest);
  	fclose(fluxfileref);
	if ( nbdiff!=0 && reportErrors)
	{
    		std::cout << "<DartMeasurement name=\"BINARYFileError\" type=\"numeric/int\">";
    		std::cout << nbdiff;
    		std::cout <<  "</DartMeasurement>" << std::endl;
	}
	return (nbdiff != 0) ? 1 : 0;
}


int RegressionTestImage (const char *testImageFilename, const char *baselineImageFilename, int reportErrors, const double toleranceDiffPixelImage)
{
  // Use the factory mechanism to read the test and baseline files and convert them to double
  typedef itk::Image<double,ITK_TEST_DIMENSION_MAX> ImageType;
  typedef itk::Image<unsigned char,ITK_TEST_DIMENSION_MAX> OutputType;
  typedef itk::Image<unsigned char,2> DiffOutputType;
  typedef otb::ImageFileReader<ImageType> ReaderType;

  // Read the baseline file
  ReaderType::Pointer baselineReader = ReaderType::New();
    baselineReader->SetFileName(baselineImageFilename);
  try
    {
    baselineReader->UpdateLargestPossibleRegion();
    }
  catch (itk::ExceptionObject& e)
    {
    std::cerr << "Exception detected while reading " << baselineImageFilename << " : "  << e.GetDescription();
    return 1000;
    }

  // Read the file generated by the test
  ReaderType::Pointer testReader = ReaderType::New();
    testReader->SetFileName(testImageFilename);
  try
    {
    testReader->UpdateLargestPossibleRegion();
    }
  catch (itk::ExceptionObject& e)
    {
    std::cerr << "Exception detected while reading " << testImageFilename << " : "  << e.GetDescription() << std::endl;
    return 1000;
    }

  // The sizes of the baseline and test image must match
  ImageType::SizeType baselineSize;
    baselineSize = baselineReader->GetOutput()->GetLargestPossibleRegion().GetSize();
  ImageType::SizeType testSize;
    testSize = testReader->GetOutput()->GetLargestPossibleRegion().GetSize();
  
  if (baselineSize != testSize)
    {
    std::cerr << "The size of the Baseline image and Test image do not match!" << std::endl;
    std::cerr << "Baseline image: " << baselineImageFilename
              << " has size " << baselineSize << std::endl;
    std::cerr << "Test image:     " << testImageFilename
              << " has size " << testSize << std::endl;
    return 1;
    }


    otbMsgDebugMacro(<< "RegressionTestImage DifferenceThreshold : "<<toleranceDiffPixelImage);
  // Now compare the two images
  typedef itk::DifferenceImageFilter<ImageType,ImageType> DiffType;
  DiffType::Pointer diff = DiffType::New();
    diff->SetValidInput(baselineReader->GetOutput());
    diff->SetTestInput(testReader->GetOutput());
    diff->SetDifferenceThreshold(toleranceDiffPixelImage);
    diff->UpdateLargestPossibleRegion();

  double status = diff->GetTotalDifference();

    otbMsgDebugMacro(<< "Status diff->GetTotalDifference : "<<status);
  // if there are discrepencies, create an diff image
  if (status && reportErrors)
    {
    typedef itk::RescaleIntensityImageFilter<ImageType,OutputType> RescaleType;
    typedef itk::ExtractImageFilter<OutputType,DiffOutputType> ExtractType;
    typedef otb::ImageFileWriter<DiffOutputType> WriterType;
    typedef itk::ImageRegion<ITK_TEST_DIMENSION_MAX> RegionType;
    OutputType::IndexType index; index.Fill(0);
    OutputType::SizeType size; size.Fill(0);

    RescaleType::Pointer rescale = RescaleType::New();
      rescale->SetOutputMinimum(itk::NumericTraits<unsigned char>::NonpositiveMin());
      rescale->SetOutputMaximum(itk::NumericTraits<unsigned char>::max());
      rescale->SetInput(diff->GetOutput());
      rescale->UpdateLargestPossibleRegion();

    RegionType region;
    region.SetIndex(index);
    
    size = rescale->GetOutput()->GetLargestPossibleRegion().GetSize();
    for (unsigned int i = 2; i < ITK_TEST_DIMENSION_MAX; i++)
      {
      size[i] = 0;
      }
    region.SetSize(size);

    ExtractType::Pointer extract = ExtractType::New();
      extract->SetInput(rescale->GetOutput());
      extract->SetExtractionRegion(region);

    WriterType::Pointer writer = WriterType::New();
      writer->SetInput(extract->GetOutput());

    std::cout << "<DartMeasurement name=\"ImageError\" type=\"numeric/double\">";
    std::cout << status;
    std::cout <<  "</DartMeasurement>" << std::endl;

    ::itk::OStringStream diffName;
      diffName << testImageFilename << ".diff.png";
    try
      {
      rescale->SetInput(diff->GetOutput());
      rescale->Update();
      }
    catch (...)
      {
      std::cerr << "Error during rescale of " << diffName.str() << std::endl;
      }
    writer->SetFileName(diffName.str().c_str());
    try
      {
      writer->Update();
      }
    catch (...)
      {
      std::cerr << "Error during write of " << diffName.str() << std::endl;
      }

    std::cout << "<DartMeasurementFile name=\"DifferenceImage\" type=\"image/png\">";
    std::cout << diffName.str();
    std::cout << "</DartMeasurementFile>" << std::endl;

    ::itk::OStringStream baseName;
    baseName << testImageFilename << ".base.png";
    try
      {
      rescale->SetInput(baselineReader->GetOutput());
      rescale->Update();
      }
    catch (...)
      {
      std::cerr << "Error during rescale of " << baseName.str() << std::endl;
      }
    try
      {
      writer->SetFileName(baseName.str().c_str());
      writer->Update();
      }
    catch (...)
      {
      std::cerr << "Error during write of " << baseName.str() << std::endl;
      }

    std::cout << "<DartMeasurementFile name=\"BaselineImage\" type=\"image/png\">";
    std::cout << baseName.str();
    std::cout << "</DartMeasurementFile>" << std::endl;

    ::itk::OStringStream testName;
    testName << testImageFilename << ".test.png";
    try
      {
      rescale->SetInput(testReader->GetOutput());
      rescale->Update();
      }
    catch (...)
      {
      std::cerr << "Error during rescale of " << testName.str()
                << std::endl;
      }
    try
      {
      writer->SetFileName(testName.str().c_str());
      writer->Update();
      }
    catch (...)
      {
      std::cerr << "Error during write of " << testName.str() << std::endl;
      }

    std::cout << "<DartMeasurementFile name=\"TestImage\" type=\"image/png\">";
    std::cout << testName.str();
    std::cout << "</DartMeasurementFile>" << std::endl;


    }
  return (status != 0) ? 1 : 0;
}

//
// Generate all of the possible baselines
// The possible baselines are generated fromn the baselineFilename using the following algorithm:
// 1) strip the suffix
// 2) append a digit _x
// 3) append the original suffix.
// It the file exists, increment x and continue
//
std::map<std::string,int> RegressionTestBaselines (char *baselineFilename)
{
  std::map<std::string,int> baselines;
  baselines[std::string(baselineFilename)] = 0;

  std::string originalBaseline(baselineFilename);

  int x = 0;
  std::string::size_type suffixPos = originalBaseline.rfind(".");
  std::string suffix;
  if (suffixPos != std::string::npos)
    {
    suffix = originalBaseline.substr(suffixPos,originalBaseline.length());
    originalBaseline.erase(suffixPos,originalBaseline.length());
    }
  while (++x)
    {
    ::itk::OStringStream filename;
    filename << originalBaseline << "." << x << suffix;
    std::ifstream filestream(filename.str().c_str());
    if (!filestream)
      {
        break;
      }
    baselines[filename.str()] = 0;
    filestream.close();
    }
  return baselines;
}


