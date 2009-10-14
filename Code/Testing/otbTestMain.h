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

#include "itkWin32Header.h"
#include <map>
#include <string>
#include <iostream>
#include <fstream>

#include "itkMultiThreader.h"

#include "otbTestHelper.h"



typedef int (*MainFuncPointer)(int , char* [] );
std::map<std::string, MainFuncPointer> StringToTestFunctionMap;

#define REGISTER_TEST(test) \
extern int test(int, char* [] ); \
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

int main(int ac, char* av[] )
{
  bool lFlagRegression(false);
  double lToleranceDiffValue(0);
  double lEpsilon(0);
  bool lIgnoreOrder(false);
  double epsilonBoundary(0.0);

  std::vector<std::string> baselineFilenamesBinary;
  std::vector<std::string> testFilenamesBinary;
  std::vector<std::string> baselineFilenamesMetaData;
  std::vector<std::string> testFilenamesMetaData;
  std::vector<std::string> baselineFilenamesOgr;
  std::vector<std::string> testFilenamesOgr;

  std::vector<std::string> baselineFilenamesImage;
  std::vector<std::string> testFilenamesImage;
  std::vector<std::string> baselineFilenamesAscii;
  std::vector<std::string> testFilenamesAscii;
  std::vector<std::string> ignoredLines;
  ignoredLines.clear();

  otb::TestHelper testHelper;

// On some sgi machines, threads and stl don't mix.
#if defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 730
  itk::MultiThreader::SetGlobalDefaultNumberOfThreads(1);
#endif

  RegisterTests();
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
      lToleranceDiffValue = (double)(::atof(av[2]));
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
      lToleranceDiffValue = (double)(::atof(av[2]));
      // Number of comparisons to do
      unsigned int nbComparisons=(unsigned int)(::atoi(av[3]));
      baselineFilenamesImage.reserve(nbComparisons);
      testFilenamesImage.reserve(nbComparisons);
      // Retrieve all the file names
      for (unsigned int i = 0; i<nbComparisons;++i)
      {
        baselineFilenamesImage.push_back(av[4+2*i]);
        testFilenamesImage.push_back(av[5+2*i]);
      }
      av+=3+2*nbComparisons;
      ac-=3+2*nbComparisons;
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
      unsigned int nbComparisons=(unsigned int)(::atoi(av[2]));
      baselineFilenamesBinary.reserve(nbComparisons);
      testFilenamesBinary.reserve(nbComparisons);
      // Retrieve all the file names
      for (unsigned int i = 0; i<nbComparisons;++i)
      {
        baselineFilenamesBinary.push_back(av[3+2*i]);
        testFilenamesBinary.push_back(av[4+2*i]);
      }
      av+=2+2*nbComparisons;
      ac-=2+2*nbComparisons;
    }
    else if (strcmp(av[1], "--compare-ascii") == 0)
    {
      lFlagRegression = true;
      lEpsilon = (double)(::atof(av[2]));
      baselineFilenamesAscii.reserve(1);
      testFilenamesAscii.reserve(1);
      baselineFilenamesAscii.push_back(av[3]);
      testFilenamesAscii.push_back(av[4]);
      av += 4;
      ac -= 4;

      if ( ac > 1 )
      {
        if (strcmp(av[1], "--ignore-lines-with") == 0)
        {
          unsigned int nbIgnoredLines=(unsigned int)(::atoi(av[2]));
          for (unsigned int  i=0; i<nbIgnoredLines; ++i )
          {
            ignoredLines.push_back(av[3+i]);
          }
          av += 2+nbIgnoredLines;
          ac -= 2+nbIgnoredLines;
        }
      }
    }
    else if (strcmp(av[1], "--compare-n-ascii") == 0)
    {
      lFlagRegression = true;
      lEpsilon = (double)(::atof(av[2]));
      // Number of comparisons to do
      unsigned int nbComparisons=(unsigned int)(::atoi(av[3]));
      baselineFilenamesAscii.reserve(nbComparisons);
      testFilenamesAscii.reserve(nbComparisons);
      // Retrieve all the file names
      for (unsigned int i = 0; i<nbComparisons;++i)
      {
        baselineFilenamesAscii.push_back(av[4+2*i]);
        testFilenamesAscii.push_back(av[5+2*i]);
      }
      av+=3+2*nbComparisons;
      ac-=3+2*nbComparisons;

      if ( ac > 1 )
      {
        if (strcmp(av[1], "--ignore-lines-with") == 0)
        {
          unsigned int nbIgnoredLines=(unsigned int)(::atoi(av[2]));
          for (unsigned int  i=0; i<nbIgnoredLines; ++i )
          {
            ignoredLines.push_back(av[3+i]);
          }
          av += 2+nbIgnoredLines;
          ac -= 2+nbIgnoredLines;
        }
      }

    }
    else if (strcmp(av[1], "--compare-metadata") == 0)
    {
      lFlagRegression = true;
      lToleranceDiffValue = (double)(::atof(av[2]));
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
      lToleranceDiffValue = (double)(::atof(av[2]));
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
  if (j != StringToTestFunctionMap.end())
  {
    MainFuncPointer f = j->second;
    int result, multiResult;
    try
    {
      // Invoke the test's "main" function.
      result = (*f)(ac-1, av+1);
      if (result!=EXIT_SUCCESS)
      {
        std::cout << "-> Test EXIT FAILURE ("<<result<<")."<<std::endl;
        itkGenericExceptionMacro(<<"Function returns EXIT_FAILURE (no regression test)");
      }
    }
    catch ( itk::ExceptionObject & e )
    {
      std::cerr << "otbTestMain '"<<testToRun<<"': ITK Exception thrown:" << std::endl;
      std::cerr << e.GetFile() << ":" << e.GetLine() << ":"<< std::endl;
      std::cerr << e.GetDescription() << std::endl;
      result = EXIT_FAILURE;
    }
    catch ( std::bad_alloc & err )
    {
      std::cerr << "otbTestMain '"<<testToRun<<"': Exception bad_alloc thrown: "<< std::endl;
      std::cerr <<(char*)err.what()<< std::endl;
      result = EXIT_FAILURE;
    }
    catch (const std::exception& e)
    {
      std::cerr << "otbTestMain '"<<testToRun<<"': std::exception  thrown:"<< std::endl;
      std::cerr << e.what() <<  std::endl;
      result = EXIT_FAILURE;
    }
    catch ( ... )
    {
      std::cerr << "otbTestMain '"<<testToRun<<"': Unknown exception thrown !" << std::endl;
      result = EXIT_FAILURE;
    }

    if (result==EXIT_SUCCESS)
    {
      result = 0;
      std::cout << " -> Test EXIT SUCCESS."<<std::endl;
      if ( lFlagRegression == false )
      {
        std::cout << "-------------  No control baseline tests    -------------"<<std::endl;
      }
      else
      {
        try
        {
          std::cout << "-------------  Start control baseline tests    -------------"<<std::endl;
          // Make a list of possible baselines

          if (lIgnoreOrder)
          {
            testHelper.IgnoreLineOrderOn();
          }
          else
          {
            testHelper.IgnoreLineOrderOff();
          }

          if (epsilonBoundary != 0.0)
          {
            testHelper.SetEpsilonBoundaryChecking(epsilonBoundary);
          }

          // Non regression test for images
          if ((baselineFilenamesImage.size()>0) && (testFilenamesImage.size()>0))
          {
            // Creates iterators on baseline filenames vector and test filenames vector
            std::vector<std::string>::iterator itbaselineFilenames = baselineFilenamesImage.begin();
            std::vector<std::string>::iterator itTestFilenames = testFilenamesImage.begin();
            int cpt(1);
            // For each couple of baseline and test file, do the comparison
            for (;(itbaselineFilenames != baselineFilenamesImage.end())
                 &&(itTestFilenames != testFilenamesImage.end());
                 ++itbaselineFilenames,++itTestFilenames)
            {
              std::string baselineFilenameImage = (*itbaselineFilenames);
              std::string testFilenameImage = (*itTestFilenames);


              std::map<std::string,int> baselines = testHelper.RegressionTestbaselines(const_cast<char*>(baselineFilenameImage.c_str()));
              std::map<std::string,int>::reverse_iterator baseline = baselines.rbegin();
              multiResult = 1;
              std::cout<<"Number of baseline images: "<<baselines.size()<<std::endl;
              while (baseline!=baselines.rend() && (multiResult!=0))
              {
                std::cout<<"Testing non-regression on image: "<<(baseline->first).c_str()<<std::endl;
                testHelper.ReportErrorsOff();
                baseline->second = testHelper.RegressionTestImage(cpt,testFilenameImage.c_str(),
                                                       (baseline->first).c_str(),
                                                       lToleranceDiffValue);

                multiResult = baseline->second;
                ++baseline;
              }
              if (multiResult != 0)
              {
                baseline = baselines.rbegin();
                testHelper.ReportErrorsOn();
                baseline->second
                    = testHelper.RegressionTestImage(cpt,testFilenameImage.c_str(),
                                      (baseline->first).c_str(),
                                      lToleranceDiffValue);
              }
              cpt++;
              result += multiResult;
            }

          }

          // Non-regression test for metadata.
          if ((baselineFilenamesMetaData.size()>0) && (testFilenamesMetaData.size()>0))
          {
            // Creates iterators on baseline filenames vector and test filenames vector
            std::vector<std::string>::iterator itbaselineFilenames = baselineFilenamesMetaData.begin();
            std::vector<std::string>::iterator itTestFilenames = testFilenamesMetaData.begin();
            // For each couple of baseline and test file, do the comparison
            for (;(itbaselineFilenames != baselineFilenamesMetaData.end())
                 &&(itTestFilenames != testFilenamesMetaData.end());
                 ++itbaselineFilenames,++itTestFilenames)
            {
              std::string baselineFilenameImage = (*itbaselineFilenames);
              std::string testFilenameImage = (*itTestFilenames);

              std::map<std::string,int> baselines = testHelper.RegressionTestbaselines(const_cast<char*>(baselineFilenameImage.c_str()));
              std::map<std::string,int>::reverse_iterator baseline = baselines.rbegin();
              multiResult = 1;
              std::cout<<"Number of baseline images: "<<baselines.size()<<std::endl;
              while (baseline!=baselines.rend() && (multiResult!=0))
              {
                std::cout<<"Testing non-regression on image: "<<(baseline->first).c_str()<<std::endl;
                testHelper.ReportErrorsOff();
                baseline->second = testHelper.RegressionTestMetaData(testFilenameImage.c_str(),
                                   (baseline->first).c_str(),
                                   lToleranceDiffValue);

                multiResult = baseline->second;
                ++baseline;
              }
              if (multiResult != 0)
              {
                baseline = baselines.rbegin();
                testHelper.ReportErrorsOn();
                baseline->second
                    = testHelper.RegressionTestMetaData(testFilenameImage.c_str(),
                                         (baseline->first).c_str(),
                                         lToleranceDiffValue);
              }
              result += multiResult;
            }
          }

          // Non regression test for ascii files
          if ((baselineFilenamesAscii.size()>0) && (testFilenamesAscii.size()>0))
          {

            // Creates iterators on baseline filenames vector and test filenames vector
            std::vector<std::string>::iterator itbaselineFilenames = baselineFilenamesAscii.begin();
            std::vector<std::string>::iterator itTestFilenames = testFilenamesAscii.begin();
            std::vector<std::string>::iterator itIgnoredLines = ignoredLines.begin();

            // Warning message
            if (ignoredLines.size() > 0 )
            {
              std::cout << "The lines containing the expressions ";
              for (;itIgnoredLines!=ignoredLines.end();++itIgnoredLines)
              {
                std::cout << (*itIgnoredLines) <<" ";
              }
              std::cout << "are not considered"<< std::endl;
            }

            // For each couple of baseline and test file, do the comparison
            for (;(itbaselineFilenames != baselineFilenamesAscii.end())
                 &&(itTestFilenames != testFilenamesAscii.end());
                 ++itbaselineFilenames,++itTestFilenames)
            {
              std::string baselineFilenameAscii = (*itbaselineFilenames);
              std::string testFilenameAscii = (*itTestFilenames);

              std::map<std::string,int> baselines = testHelper.RegressionTestbaselines(const_cast<char*>(baselineFilenameAscii.c_str()));
              std::map<std::string,int>::reverse_iterator baseline = baselines.rbegin();
              multiResult = 1;
              std::cout<<"Number of baseline files: "<<baselines.size()<<std::endl;
              while (baseline!=baselines.rend() && (multiResult!=0))
              {
                std::cout<<"Testing non-regression on file: "<<(baseline->first).c_str()<<std::endl;
                testHelper.ReportErrorsOff();
                baseline->second = testHelper.RegressionTestAsciiFile(testFilenameAscii.c_str(),
                                   (baseline->first).c_str(),
                                   lEpsilon,
                                   ignoredLines);

                multiResult = baseline->second;
                ++baseline;
              }
              if (multiResult != 0)
              {
                baseline = baselines.rbegin();
                testHelper.ReportErrorsOn();
                baseline->second
                    = testHelper.RegressionTestAsciiFile(testFilenameAscii.c_str(),
                                          (baseline->first).c_str(),
                                          lEpsilon,
                                          ignoredLines);
              }
              result += multiResult;
            }
          }
/******************************************/
/******************************************/
/******************************************/
          // Non regression test for binary files
          if ((baselineFilenamesBinary.size()>0) && (testFilenamesBinary.size()>0))
          {
            // Creates iterators on baseline filenames vector and test filenames vector
            std::vector<std::string>::iterator itbaselineFilenames = baselineFilenamesBinary.begin();
            std::vector<std::string>::iterator itTestFilenames = testFilenamesBinary.begin();

            // For each couple of baseline and test file, do the comparison
            for (;(itbaselineFilenames != baselineFilenamesBinary.end())
                 &&(itTestFilenames != testFilenamesBinary.end());
                 ++itbaselineFilenames,++itTestFilenames)
            {
              std::string baselineFilenameBinary = (*itbaselineFilenames);
              std::string testFilenameBinary = (*itTestFilenames);

              std::map<std::string,int> baselines = testHelper.RegressionTestbaselines(const_cast<char*>(baselineFilenameBinary.c_str()));
              std::map<std::string,int>::reverse_iterator baseline = baselines.rbegin();
              multiResult = 1;
              std::cout<<"Number of baseline files: "<<baselines.size()<<std::endl;
              while (baseline!=baselines.rend() && (multiResult!=0))
              {
                std::cout<<"Testing non-regression on file: "<<(baseline->first).c_str()<<std::endl;
                testHelper.ReportErrorsOff();
                baseline->second = testHelper.RegressionTestBinaryFile(testFilenameBinary.c_str(),
                                   (baseline->first).c_str());

                multiResult = baseline->second;
                ++baseline;
              }
              if (multiResult != 0)
              {
                baseline = baselines.rbegin();
                testHelper.ReportErrorsOn();
                baseline->second
                    = testHelper.RegressionTestBinaryFile(testFilenameBinary.c_str(),
                                           (baseline->first).c_str());
              }
              result += multiResult;
            }
          }
          // Non regression test for OGR files
          if ((baselineFilenamesOgr.size()>0) && (testFilenamesOgr.size()>0))
          {
            // Creates iterators on baseline filenames vector and test filenames vector
            std::vector<std::string>::iterator itbaselineFilenames = baselineFilenamesOgr.begin();
            std::vector<std::string>::iterator itTestFilenames = testFilenamesOgr.begin();
            // For each couple of baseline and test file, do the comparison
            for (;(itbaselineFilenames != baselineFilenamesOgr.end())
                 &&(itTestFilenames != testFilenamesOgr.end());
                 ++itbaselineFilenames,++itTestFilenames)
            {
              std::string baselineFilenameOgr = (*itbaselineFilenames);
              std::string testFilenameOgr = (*itTestFilenames);

              std::map<std::string,int> baselines = testHelper.RegressionTestbaselines(const_cast<char*>(baselineFilenameOgr.c_str()));
              std::map<std::string,int>::reverse_iterator baseline = baselines.rbegin();
              multiResult = 1;
              std::cout<<"Number of baseline OGR files: "<<baselines.size()<<std::endl;
              while (baseline!=baselines.rend() && (multiResult!=0))
              {
                std::cout<<"Testing non-regression on OGR file: "<<(baseline->first).c_str()<<std::endl;
                testHelper.ReportErrorsOff();
                baseline->second = testHelper.RegressionTestOgrFile(testFilenameOgr.c_str(),
                                    (baseline->first).c_str(),
                                    lToleranceDiffValue);

                multiResult = baseline->second;
                ++baseline;
              }
              if (multiResult != 0)
              {
                baseline = baselines.rbegin();
                testHelper.ReportErrorsOn();
                baseline->second
                    = testHelper.RegressionTestOgrFile(testFilenameOgr.c_str(),
                                            (baseline->first).c_str(),
                                            lToleranceDiffValue);
              }
              result += multiResult;
            }
          }

        }
        catch ( itk::ExceptionObject & e )
        {
          std::cerr << "otbTestMain 'control baseline test': ITK Exception thrown:" << std::endl;
          std::cerr << e.GetFile() << ":" << e.GetLine() << ":"<< std::endl;
          std::cerr << e.GetDescription() << std::endl;
          return -1;
        }
        catch ( std::bad_alloc & err )
        {
          std::cerr << "otbTestMain 'control baseline test': Exception bad_alloc thrown: "<< std::endl;
          std::cerr <<(char*)err.what()<< std::endl;
          return -1;
        }
        catch (const std::exception& e)
        {
          std::cerr << "otbTestMain 'control baseline test': std::exception  thrown:"<< std::endl;
          std::cerr << e.what() <<  std::endl;
          return -1;
        }
        catch ( ... )
        {
          std::cerr << "otbTestMain 'control baseline test': Unknown exception thrown !" << std::endl;
          return -1;
        }
        std::cout << "-------------  End control baseline tests    -------------"<<std::endl;
      } // Fin else de if( lFlagRegression == false )
    } // if (result==EXIT_SUCCESS)
    return result;
  }
  PrintAvailableTests();
  std::cerr << "Failure: " << testToRun << ": no test identified " << testToRun << "\n";
  return -1;
}

#endif

