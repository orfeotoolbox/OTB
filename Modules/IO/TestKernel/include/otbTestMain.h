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

#ifndef otbTestMain_h
#define otbTestMain_h

#include "otbConfigure.h"
#include "otbConfigurationManager.h"

#include <map>
#include <string>

#include "itkMultiThreader.h"
#include "otbMacro.h"

#include "otbOGRDriversInit.h"
#include "otbTestHelper.h"
#include "otbImageFileReaderException.h"

#include "itkMersenneTwisterRandomVariateGenerator.h"

#ifdef OTB_USE_MPI
#include "otbMPIConfig.h"
#endif

typedef int (*MainFuncPointer)(int, char* []);
std::map<std::string, MainFuncPointer> StringToTestFunctionMap;

#define REGISTER_TEST(test)       \
  extern int test(int, char* []); \
  StringToTestFunctionMap[#test] = test

void RegisterTests();
void PrintAvailableTests()
{
  std::cout << "Tests available:\n";
  std::map<std::string, MainFuncPointer>::iterator j = StringToTestFunctionMap.begin();
  int i = 0;
  while (j != StringToTestFunctionMap.end())
  {
    std::cout << i << ". " << j->first << "\n";
    ++i;
    ++j;
  }
}

// apply dedicated treatment to test
void LoadTestEnv()
{
  // Set seed for rand and itk mersenne twister
  srand(1);
  itk::Statistics::MersenneTwisterRandomVariateGenerator::GetInstance()->SetSeed(121212);
}

/** CheckOption, utility function to test the presence of a given token
 *  in the argument list. The function also checks if there are at least 'min'
 *  arguments left to parse the token (token included) */
bool CheckOption(char* arg, const char* token, int ac, int min)
{
  if (strcmp(arg, token) == 0)
  {
    if (min < ac)
    {
      return true;
    }
    else
    {
      std::cerr << "Can't parse token '" << token << "', need at least " << min - 1 << " arguments after.\n";
    }
  }
  return false;
}

int main(int ac, char* av[])
{
#ifdef OTB_USE_MPI
  otb::MPIConfig::Instance()->Init(ac, av);
#endif

  bool                lFlagRegression(false);
  std::vector<double> toleranceDiffValues;
  std::vector<double> toleranceOgrValues;
  std::vector<double> toleranceMetaValues;
  std::vector<double> toleranceAsciiValues;
  bool                lIgnoreOrder(false);
  double              epsilonBoundary(0.0);
  double              lToleranceRatio(0.0);

  typedef otb::TestHelper::StringList StringList;
  StringList                          baselineFilenamesBinary;
  StringList                          testFilenamesBinary;
  StringList                          baselineFilenamesMetaData;
  StringList                          testFilenamesMetaData;
  StringList                          baselineFilenamesOgr;
  StringList                          testFilenamesOgr;

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
    int i = 0;
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
    int ac0 = ac + 1;
    while (strncmp(av[1], "--", 2) == 0 && ac0 > ac)
    {
      ac0 = ac;
      if (CheckOption(av[1], "--with-threads", ac, 2))
      {
        int numThreads = atoi(av[2]);
        itk::MultiThreader::SetGlobalDefaultNumberOfThreads(numThreads);
        av += 2;
        ac -= 2;
      }
      else if (CheckOption(av[1], "--without-threads", ac, 1))
      {
        itk::MultiThreader::SetGlobalDefaultNumberOfThreads(1);
        av += 1;
        ac -= 1;
      }
      else if (CheckOption(av[1], "--ignore-order", ac, 1))
      {
        lIgnoreOrder = true;
        av += 1;
        ac -= 1;
      }
      else if (CheckOption(av[1], "--epsilon-boundary", ac, 2))
      {
        epsilonBoundary = atof(av[2]);
        av += 2;
        ac -= 2;
      }
      else if (CheckOption(av[1], "--compare-image", ac, 4))
      {
        lFlagRegression = true;
        toleranceDiffValues.push_back((double)(::atof(av[2])));
        baselineFilenamesImage.push_back(av[3]);
        testFilenamesImage.push_back(av[4]);
        av += 4;
        ac -= 4;
      }
      else if (CheckOption(av[1], "--compare-n-images", ac, 3))
      {
        lFlagRegression = true;
        // Number of comparisons to do
        unsigned int nbComparisons = (unsigned int)(::atoi(av[3]));
        double       tol           = (double)(::atof(av[2]));
        toleranceDiffValues.reserve(toleranceDiffValues.size() + nbComparisons);
        baselineFilenamesImage.reserve(baselineFilenamesImage.size() + nbComparisons);
        testFilenamesImage.reserve(testFilenamesImage.size() + nbComparisons);
        // Retrieve all the file names
        for (unsigned int i = 0; i < nbComparisons; ++i)
        {
          toleranceDiffValues.push_back(tol);
          baselineFilenamesImage.push_back(av[4 + 2 * i]);
          testFilenamesImage.push_back(av[5 + 2 * i]);
        }
        av += 3 + 2 * nbComparisons;
        ac -= 3 + 2 * nbComparisons;
      }
      else if (CheckOption(av[1], "--compare-binary", ac, 3))
      {
        lFlagRegression = true;
        baselineFilenamesBinary.reserve(1);
        testFilenamesBinary.reserve(1);
        baselineFilenamesBinary.push_back(av[2]);
        testFilenamesBinary.push_back(av[3]);
        av += 3;
        ac -= 3;
      }
      else if (CheckOption(av[1], "--compare-n-binary", ac, 2))
      {
        lFlagRegression            = true;
        unsigned int nbComparisons = (unsigned int)(::atoi(av[2]));
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
      else if (CheckOption(av[1], "--compare-ascii", ac, 4))
      {
        lFlagRegression = true;
        toleranceAsciiValues.push_back((double)(::atof(av[2])));
        baselineFilenamesAscii.push_back(av[3]);
        testFilenamesAscii.push_back(av[4]);
        av += 4;
        ac -= 4;
      }
      else if (CheckOption(av[1], "--compare-n-ascii", ac, 3))
      {
        lFlagRegression = true;
        double tol      = (double)(::atof(av[2]));
        // Number of comparisons to do
        unsigned int nbComparisons = (unsigned int)(::atoi(av[3]));
        baselineFilenamesAscii.reserve(baselineFilenamesAscii.size() + nbComparisons);
        testFilenamesAscii.reserve(testFilenamesAscii.size() + nbComparisons);
        // Retrieve all the file names
        for (unsigned int i = 0; i < nbComparisons; ++i)
        {
          toleranceAsciiValues.push_back(tol);
          baselineFilenamesAscii.push_back(av[4 + 2 * i]);
          testFilenamesAscii.push_back(av[5 + 2 * i]);
        }
        av += 3 + 2 * nbComparisons;
        ac -= 3 + 2 * nbComparisons;
      }
      else if (CheckOption(av[1], "--ignore-lines-with", ac, 2))
      {
        unsigned int nbIgnoredLines = (unsigned int)(::atoi(av[2]));
        for (unsigned int i = 0; i < nbIgnoredLines; ++i)
        {
          ignoredLines.push_back(av[3 + i]);
        }
        av += 2 + nbIgnoredLines;
        ac -= 2 + nbIgnoredLines;
      }
      /************************************************************************/
      else if (CheckOption(av[1], "--compare-metadata", ac, 4))
      {
        lFlagRegression = true;
        toleranceMetaValues.push_back((double)(::atof(av[2])));
        baselineFilenamesMetaData.push_back(av[3]);
        testFilenamesMetaData.push_back(av[4]);
        av += 4;
        ac -= 4;
      }
      else if (CheckOption(av[1], "--compare-ogr", ac, 4))
      {
        lFlagRegression = true;
        toleranceOgrValues.push_back((double)(::atof(av[2])));
        baselineFilenamesOgr.push_back(av[3]);
        testFilenamesOgr.push_back(av[4]);
        av += 4;
        ac -= 4;
      }
      else if (CheckOption(av[1], "--tolerance-ratio", ac, 2))
      {
        lToleranceRatio = (double)(::atof(av[2]));
        av += 2;
        ac -= 2;
      }
    } // end while

    // Check there is one argument left for the test
    if (ac >= 2)
    {
      testToRun = av[1];
    }
  }

  otb::ConfigurationManager::InitOpenMPThreads();

  std::map<std::string, MainFuncPointer>::iterator j = StringToTestFunctionMap.find(testToRun);
  int result = EXIT_SUCCESS;
  // If the test doesn't exists
  if (j == StringToTestFunctionMap.end())
  {
    PrintAvailableTests();
    std::cerr << "Failure: no test identified for '" << testToRun << "'\n";
    result = -1;
  }
  else
  {
    otb::Logger::Instance()->LogSetupInformation();
    MainFuncPointer f = j->second;
    try
    {
      // Invoke the test's "main" function.
      result = (*f)(ac - 1, av + 1);
      if (result != EXIT_SUCCESS)
      {
        std::cout << "-> Test EXIT FAILURE (" << result << ")." << std::endl;
        itkGenericExceptionMacro(<< "Function returns EXIT_FAILURE (not from regression, failure inside the test)");
      }
    }
    catch (const std::exception& e)
    {
      std::cerr << "otbTestMain '" << testToRun << "': exception caught:" << std::endl;
      std::cerr << e.what() << std::endl;
      result = EXIT_FAILURE;
    }
    catch (...)
    {
      std::cerr << "otbTestMain '" << testToRun << "': unknown exception caught!" << std::endl;
      result = EXIT_FAILURE;
    }

    bool checkBaseline = true;
    if (result != EXIT_SUCCESS)
    {
      checkBaseline = false;
      result        = -1;
    }

#ifdef OTB_USE_MPI
    if (otb::MPIConfig::Instance()->GetMyRank() != 0)
    {
      checkBaseline = false;
    }
#endif

    if (checkBaseline)
    {
      std::cout << " -> Test EXIT SUCCESS." << std::endl;
      if (lFlagRegression == false)
      {
        std::cout << "-------------  No control baseline tests    -------------" << std::endl;
      }
      else
      {

        try
        {
          std::cout << "-------------  Start control baseline tests    -------------" << std::endl;
          // Make a list of possible baselines

          testHelper->SetIgnoreLineOrder(lIgnoreOrder);
          testHelper->SetToleranceRatio(lToleranceRatio);
          if (epsilonBoundary != 0.0)
          {
            testHelper->SetEpsilonBoundaryChecking(epsilonBoundary);
          }
          /***********************************************************************************/
          // Non regression test for images
          if ((baselineFilenamesImage.size() > 0) && (testFilenamesImage.size() > 0))
          {
            result += testHelper->RegressionTestAllImages(baselineFilenamesImage, testFilenamesImage, toleranceDiffValues);
          }
          /***********************************************************************************/
          // Non-regression test for metadata.
          if ((baselineFilenamesMetaData.size() > 0) && (testFilenamesMetaData.size() > 0))
          {
            result += testHelper->RegressionTestAllMetaData(baselineFilenamesMetaData, testFilenamesMetaData, toleranceMetaValues);
          }
          /***********************************************************************************/
          // Non regression test for ascii files
          if ((baselineFilenamesAscii.size() > 0) && (testFilenamesAscii.size() > 0))
          {
            // result += testHelper->RegressionTestAllAscii(baselineFilenamesAscii, testFilenamesAscii, ignoredLines);
            result += testHelper->RegressionTestAllDiff(baselineFilenamesAscii, testFilenamesAscii, toleranceAsciiValues, ignoredLines);
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
            result += testHelper->RegressionTestAllOgr(baselineFilenamesOgr, testFilenamesOgr, toleranceOgrValues);
          }
          std::cout << "-------------  End control baseline tests    -------------" << std::endl;
        }
        catch (itk::ExceptionObject& e)
        {
          std::cerr << "otbTestMain 'control baseline test': ITK Exception thrown:" << std::endl;
          std::cerr << e.GetFile() << ":" << e.GetLine() << ":" << std::endl;
          std::cerr << e.GetDescription() << std::endl;
          result = -1;
        }
        catch (std::bad_alloc& err)
        {
          std::cerr << "otbTestMain 'control baseline test': Exception bad_alloc thrown: " << std::endl;
          std::cerr << (char*)err.what() << std::endl;
          result = -1;
        }
        catch (const std::exception& e)
        {
          std::cerr << "otbTestMain 'control baseline test': std::exception  thrown:" << std::endl;
          std::cerr << e.what() << std::endl;
          result = -1;
        }
        catch (...)
        {
          std::cerr << "otbTestMain 'control baseline test': Unknown exception thrown !" << std::endl;
          result = -1;
        }

      } // if there is a baseline control

    } // if checkBaseline
  }   // if test function exists

#ifdef OTB_USE_MPI
  otb::MPIConfig::Instance()->terminate();
#endif
  return result;
}

#endif
