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
#ifndef __otbTestMain_h
#define __otbTestMain_h

#include "otbConfigure.h"

#include <map>
#include <string>
#include <iostream>

#include "itkMultiThreader.h"
#include "itkMacro.h"

#include "otbOGRDriversInit.h"
#include "otbTestHelper.h"

#include "itkMersenneTwisterRandomVariateGenerator.h"


typedef int (*MainFuncPointer)(int, char*[]);
std::map<std::string, MainFuncPointer> StringToTestFunctionMap;

#define REGISTER_TEST(test) \
  extern int test(int, char*[]); \
  StringToTestFunctionMap[# test] = test

void RegisterTests();
void PrintAvailableTests()
{
  std::cout << "Tests available:\n";
  std::map<std::string, MainFuncPointer>::iterator j = StringToTestFunctionMap.begin();
  int                                              i = 0;
  while (j != StringToTestFunctionMap.end())
    {
    std::cout << i << ". " << j->first << "\n";
    ++i;
    ++j;
    }
}

//apply dedicated treatment to test
void LoadTestEnv()
{
  //Set seed for rand and itk mersenne twister
  srand(1);
  itk::Statistics::MersenneTwisterRandomVariateGenerator::GetInstance()->SetSeed(121212);
}

int main(int ac, char* av[])
{
  bool   lFlagRegression(false);
  double lToleranceDiffValue(0);
  double lEpsilon(0);
  bool   lIgnoreOrder(false);
  double epsilonBoundary(0.0);

  typedef otb::TestHelper::StringList   StringList;
  StringList baselineFilenamesBinary;
  StringList testFilenamesBinary;
  StringList baselineFilenamesMetaData;
  StringList testFilenamesMetaData;
  StringList baselineFilenamesOgr;
  StringList testFilenamesOgr;

  StringList baselineFilenamesImage;
  StringList testFilenamesImage;
  StringList baselineFilenamesAscii;
  StringList testFilenamesAscii;
  StringList ignoredLines;
  ignoredLines.clear();

  otb::ogr::Drivers::Init();
  otb::TestHelper::Pointer testHelper = otb::TestHelper::New();

  RegisterTests();

  LoadTestEnv(); // load specific treatments for testing
  std::string testToRun;
  if (ac < 2)
    {
    PrintAvailableTests();
    std::cout << "To launch a test, enter its number: ";
    int testNum = 0;
    std::cin >> testNum;
    std::map<std::string, MainFuncPointer>::iterator j = StringToTestFunctionMap.begin();
    int                                              i = 0;
    while (j != StringToTestFunctionMap.end() && i < testNum)
      {
      ++i;
      ++j;
      }
    if (j == StringToTestFunctionMap.end())
      {
      std::cerr << testNum << " is not a valid test number\n";
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
    if (strcmp(av[1], "--ignore-order") == 0)
      {
      lIgnoreOrder = true;
      av += 1;
      ac -= 1;
      }
    if (strcmp(av[1], "--epsilon-boundary") == 0)
      {
      epsilonBoundary = atof(av[2]);
      av += 2;
      ac -= 2;
      }
    if (strcmp(av[1], "--compare-image") == 0)
      {
      lFlagRegression = true;
      lToleranceDiffValue = (double) (::atof(av[2]));
      baselineFilenamesImage.reserve(1);
      testFilenamesImage.reserve(1);
      baselineFilenamesImage.push_back(av[3]);
      testFilenamesImage.push_back(av[4]);
      av += 4;
      ac -= 4;
      }
    else if (strcmp(av[1], "--compare-n-images") == 0)
      {
      lFlagRegression = true;
      lToleranceDiffValue = (double) (::atof(av[2]));
      // Number of comparisons to do
      unsigned int nbComparisons = (unsigned int) (::atoi(av[3]));
      baselineFilenamesImage.reserve(nbComparisons);
      testFilenamesImage.reserve(nbComparisons);
      // Retrieve all the file names
      for (unsigned int i = 0; i < nbComparisons; ++i)
        {
        baselineFilenamesImage.push_back(av[4 + 2 * i]);
        testFilenamesImage.push_back(av[5 + 2 * i]);
        }
      av += 3 + 2 * nbComparisons;
      ac -= 3 + 2 * nbComparisons;
      }
    else if (strcmp(av[1], "--compare-binary") == 0)
      {
      lFlagRegression = true;
      baselineFilenamesBinary.reserve(1);
      testFilenamesBinary.reserve(1);
      baselineFilenamesBinary.push_back(av[2]);
      testFilenamesBinary.push_back(av[3]);
      av += 3;
      ac -= 3;
      }
    else if (strcmp(av[1], "--compare-n-binary") == 0)
      {
      lFlagRegression = true;
      unsigned int nbComparisons = (unsigned int) (::atoi(av[2]));
      baselineFilenamesBinary.reserve(nbComparisons);
      testFilenamesBinary.reserve(nbComparisons);
      // Retrieve all the file names
      for (unsigned int i = 0; i < nbComparisons; ++i)
        {
        baselineFilenamesBinary.push_back(av[3 + 2 * i]);
        testFilenamesBinary.push_back(av[4 + 2 * i]);
        }
      av += 2 + 2 * nbComparisons;
      ac -= 2 + 2 * nbComparisons;
      }
    /************************************************************************/
    // COMPARE ASCII
    else if (strcmp(av[1], "--compare-ascii") == 0)
      {
      lFlagRegression = true;
      lEpsilon = (double) (::atof(av[2]));
      baselineFilenamesAscii.reserve(1);
      testFilenamesAscii.reserve(1);
      baselineFilenamesAscii.push_back(av[3]);
      testFilenamesAscii.push_back(av[4]);
      av += 4;
      ac -= 4;

      if (ac > 1)
        {
        if (strcmp(av[1], "--ignore-lines-with") == 0)
          {
          unsigned int nbIgnoredLines = (unsigned int) (::atoi(av[2]));
          for (unsigned int i = 0; i < nbIgnoredLines; ++i)
            {
            ignoredLines.push_back(av[3 + i]);
            }
          av += 2 + nbIgnoredLines;
          ac -= 2 + nbIgnoredLines;
          }
        }
      }
    /************************************************************************/
    else if (strcmp(av[1], "--compare-n-ascii") == 0)
      {
      lFlagRegression = true;
      lEpsilon = (double) (::atof(av[2]));
      // Number of comparisons to do
      unsigned int nbComparisons = (unsigned int) (::atoi(av[3]));
      baselineFilenamesAscii.reserve(nbComparisons);
      testFilenamesAscii.reserve(nbComparisons);
      // Retrieve all the file names
      for (unsigned int i = 0; i < nbComparisons; ++i)
        {
        baselineFilenamesAscii.push_back(av[4 + 2 * i]);
        testFilenamesAscii.push_back(av[5 + 2 * i]);
        }
      av += 3 + 2 * nbComparisons;
      ac -= 3 + 2 * nbComparisons;

      if (ac > 1)
        {
        if (strcmp(av[1], "--ignore-lines-with") == 0)
          {
          unsigned int nbIgnoredLines = (unsigned int) (::atoi(av[2]));
          for (unsigned int i = 0; i < nbIgnoredLines; ++i)
            {
            ignoredLines.push_back(av[3 + i]);
            }
          av += 2 + nbIgnoredLines;
          ac -= 2 + nbIgnoredLines;
          }
        }

      }
    else if (strcmp(av[1], "--compare-metadata") == 0)
      {
      lFlagRegression = true;
      lToleranceDiffValue = (double) (::atof(av[2]));
      baselineFilenamesMetaData.reserve(1);
      testFilenamesMetaData.reserve(1);
      baselineFilenamesMetaData.push_back(av[3]);
      testFilenamesMetaData.push_back(av[4]);
      av += 4;
      ac -= 4;
      }

    else if (strcmp(av[1], "--compare-ogr") == 0)
      {
      lFlagRegression = true;
      lToleranceDiffValue = (double) (::atof(av[2]));
      baselineFilenamesOgr.reserve(1);
      testFilenamesOgr.reserve(1);
      baselineFilenamesOgr.push_back(av[3]);
      testFilenamesOgr.push_back(av[4]);
      av += 4;
      ac -= 4;
      }
    testToRun = av[1];
    }

  std::map<std::string, MainFuncPointer>::iterator j = StringToTestFunctionMap.find(testToRun);
  // If the test doesn't exists
  if ( j == StringToTestFunctionMap.end() )
      {
        PrintAvailableTests();
        std::cerr << "Failure: " << testToRun << ": no test identified " << testToRun << "\n";
        return -1;
      }
  else
    {
    MainFuncPointer f = j->second;
    int             result;
    try
      {
      // Invoke the test's "main" function.
      result = (*f)(ac - 1, av + 1);
      if (result != EXIT_SUCCESS )
        {
        std::cout << "-> Test EXIT FAILURE (" << result << ")." << std::endl;
        itkGenericExceptionMacro(<< "Function returns EXIT_FAILURE (not from regression, failure inside the test)");
        }
      }
    catch (itk::ExceptionObject& e)
      {
      std::cerr << "otbTestMain '" << testToRun << "': ITK Exception thrown:" << std::endl;
      std::cerr << e.GetFile() << ":" << e.GetLine() << ":" << std::endl;
      std::cerr << e.GetDescription() << std::endl;
      result = EXIT_FAILURE;
      }
    catch (std::bad_alloc& err)
      {
      std::cerr << "otbTestMain '" << testToRun << "': Exception bad_alloc thrown: " << std::endl;
      std::cerr << (char*) err.what() << std::endl;
      result = EXIT_FAILURE;
      }
    catch (const std::exception& e)
      {
      std::cerr << "otbTestMain '" << testToRun << "': std::exception  thrown:" << std::endl;
      std::cerr << e.what() <<  std::endl;
      result = EXIT_FAILURE;
      }
    catch (...)
      {
      std::cerr << "otbTestMain '" << testToRun << "': Unknown exception thrown !" << std::endl;
      result = EXIT_FAILURE;
      }

    if (result != EXIT_SUCCESS )
      {
        return -1;
      }


    result = EXIT_SUCCESS;
    std::cout << " -> Test EXIT SUCCESS." << std::endl;
    if (lFlagRegression == false)
      {
      std::cout << "-------------  No control baseline tests    -------------" << std::endl;
      return result;
      }

    try
      {
      std::cout << "-------------  Start control baseline tests    -------------" << std::endl;
      // Make a list of possible baselines

      testHelper->SetIgnoreLineOrder(lIgnoreOrder);
      testHelper->SetToleranceDiffValue(lToleranceDiffValue); // What's the difference
      testHelper->SetEpsilon(lEpsilon); // maybe we should consolidate...
      if (epsilonBoundary != 0.0)
        {
        testHelper->SetEpsilonBoundaryChecking(epsilonBoundary);
        }
      /***********************************************************************************/
      // Non regression test for images
      if ((baselineFilenamesImage.size() > 0) && (testFilenamesImage.size() > 0))
        {
        result += testHelper->RegressionTestAllImages(baselineFilenamesImage, testFilenamesImage);
        }
      /***********************************************************************************/
      // Non-regression test for metadata.
      if ((baselineFilenamesMetaData.size() > 0) && (testFilenamesMetaData.size() > 0))
        {
        result += testHelper->RegressionTestAllMetaData(baselineFilenamesMetaData, testFilenamesMetaData);
        }

      /***********************************************************************************/
      // Non regression test for ascii files
      if ((baselineFilenamesAscii.size() > 0) && (testFilenamesAscii.size() > 0))
        {
        result += testHelper->RegressionTestAllAscii(baselineFilenamesAscii, testFilenamesAscii, ignoredLines);
        }
      /******************************************************************************/
      // Non regression test for binary files
      if ((baselineFilenamesBinary.size() > 0) && (testFilenamesBinary.size() > 0))
        {
        result += testHelper->RegressionTestAllBinary(baselineFilenamesBinary, testFilenamesBinary);
        }
      /******************************************************************************/
      // Non regression test for OGR files
      if ((baselineFilenamesOgr.size() > 0) && (testFilenamesOgr.size() > 0))
        {
        result += testHelper->RegressionTestAllOgr(baselineFilenamesOgr, testFilenamesOgr);
        }

      }
    catch (itk::ExceptionObject& e)
      {
      std::cerr << "otbTestMain 'control baseline test': ITK Exception thrown:" << std::endl;
      std::cerr << e.GetFile() << ":" << e.GetLine() << ":" << std::endl;
      std::cerr << e.GetDescription() << std::endl;
      return -1;
      }
    catch (std::bad_alloc& err)
      {
      std::cerr << "otbTestMain 'control baseline test': Exception bad_alloc thrown: " << std::endl;
      std::cerr << (char*) err.what() << std::endl;
      return -1;
      }
    catch (const std::exception& e)
      {
      std::cerr << "otbTestMain 'control baseline test': std::exception  thrown:" << std::endl;
      std::cerr << e.what() <<  std::endl;
      return -1;
      }
    catch (...)
      {
      std::cerr << "otbTestMain 'control baseline test': Unknown exception thrown !" << std::endl;
      return -1;
      }
    std::cout << "-------------  End control baseline tests    -------------" << std::endl;

    return result;
    }
}

#endif
