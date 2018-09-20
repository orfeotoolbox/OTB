/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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


#include "otbTestHelper.h"
#include "otbMacro.h"
#include "otbStringUtils.h"
#include <iostream>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <string>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include "ogrsf_frmts.h"
#pragma GCC diagnostic pop
#else
#include "ogrsf_frmts.h"
#endif

#include "itksys/SystemTools.hxx"
#include "itksys/Directory.hxx"
#include "itksys/RegularExpression.hxx"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbDifferenceImageFilter.h"
#include "otbPrintableImageFilter.h"
#include "otbStreamingShrinkImageFilter.h"
#include "otbOGRVersionProxy.h"

#include "otbConfigure.h"

#define ITK_TEST_DIMENSION_MAX 6

// OGR Headers files
#include "cpl_conv.h"
#include "cpl_string.h"
#include "cpl_multiproc.h"

#define otbPrintDiff(comment, refStr, testStr) \
  std::cout << "   ----    '" << comment << "' checking   ---------------------------" << std::endl; \
  std::cout << "   Base << " << refStr << std::endl; \
  std::cout << "   Test >> " << testStr << std::endl;

#define otbCheckStringValue(Comment, RefStr, TestStr, count, report) \
    { \
    std::string strRefStr(""); if (RefStr != NULL) strRefStr = RefStr; \
    std::string strTestStr(""); if (TestStr != NULL) strTestStr = TestStr; \
    if (strRefStr != strTestStr) \
      { \
      count++; \
      if (report) \
        { \
        otbPrintDiff(Comment, strRefStr, strTestStr); \
        } \
      } \
    }
#define otbCheckValue(Comment, Ref, Test, count, report) \
    { \
    if (Ref != Test) \
      { \
      count++; \
      if (report) \
        { \
        otbPrintDiff(Comment, Ref, Test); \
        } \
      } \
    }

namespace otb
{

int TestHelper::RegressionTestAllImages(const StringList& baselineFilenamesImage,
                                        const StringList& testFilenamesImage)
{
  int result = 0;
  StringListIt itbaselineFilenames = baselineFilenamesImage.begin();
  StringListIt itTestFilenames = testFilenamesImage.begin();
  int                                cpt(1);
  // For each couple of baseline and test file, do the comparison
  for (; (itbaselineFilenames != baselineFilenamesImage.end())
         && (itTestFilenames != testFilenamesImage.end());
       ++itbaselineFilenames, ++itTestFilenames)
    {
    std::string baselineFilenameImage = (*itbaselineFilenames);
    std::string testFilenameImage = (*itTestFilenames);

    std::map<std::string, int> baselines =
      this->RegressionTestBaselines(const_cast<char*>(baselineFilenameImage.c_str()));
    std::map<std::string, int>::reverse_iterator baseline = baselines.rbegin();
    int multiResult = 1;
    std::cout << "Number of baseline images: " << baselines.size() << std::endl;
    while (baseline != baselines.rend() && (multiResult != 0))
      {
      std::cout << "Testing non-regression on image: " << (baseline->first) << std::endl;
      this->ReportErrorsOff();
      baseline->second = this->RegressionTestImage(cpt, testFilenameImage.c_str(),
                                                        (baseline->first).c_str(),
                                                        m_ToleranceDiffValue);

      multiResult = baseline->second;
      ++baseline;
      }
    if (multiResult != 0)
      {
      baseline = baselines.rbegin();
      this->ReportErrorsOn();
      baseline->second
        = this->RegressionTestImage(cpt, testFilenameImage.c_str(),
                                         (baseline->first).c_str(),
                                         m_ToleranceDiffValue);
      }
    ++cpt;
    result += multiResult;
    }
  return result;
}

int TestHelper::RegressionTestAllMetaData(const StringList& baselineFilenamesMetaData,
                                          const StringList& testFilenamesMetaData)
{
  int result = 0;
  // Creates iterators on baseline filenames vector and test filenames vector
  StringListIt itbaselineFilenames = baselineFilenamesMetaData.begin();
  StringListIt itTestFilenames = testFilenamesMetaData.begin();
  // For each couple of baseline and test file, do the comparison
  for (; (itbaselineFilenames != baselineFilenamesMetaData.end())
         && (itTestFilenames != testFilenamesMetaData.end());
       ++itbaselineFilenames, ++itTestFilenames)
    {
    std::string baselineFilenameImage = (*itbaselineFilenames);
    std::string testFilenameImage = (*itTestFilenames);

    std::map<std::string, int> baselines =
      this->RegressionTestBaselines(const_cast<char*>(baselineFilenameImage.c_str()));
    std::map<std::string, int>::reverse_iterator baseline = baselines.rbegin();
    int multiResult = 1;
    std::cout << "Number of baseline images: " << baselines.size() << std::endl;
    while (baseline != baselines.rend() && (multiResult != 0))
      {
      std::cout << "Testing non-regression on image: " << (baseline->first) << std::endl;
      this->ReportErrorsOff();
      baseline->second = this->RegressionTestMetaData(testFilenameImage.c_str(),
                                                           (baseline->first).c_str(),
                                                           m_ToleranceDiffValue);

      multiResult = baseline->second;
      ++baseline;
      }
    if (multiResult != 0)
      {
      baseline = baselines.rbegin();
      this->ReportErrorsOn();
      baseline->second
        = this->RegressionTestMetaData(testFilenameImage.c_str(),
                                            (baseline->first).c_str(),
                                            m_ToleranceDiffValue);
      }
    result += multiResult;
    }
  return result;
}

int TestHelper::RegressionTestAllAscii(const StringList& baselineFilenamesAscii,
                                       const StringList& testFilenamesAscii,
                                       const StringList& ignoredLines)
{
  int result = 0;
  // Creates iterators on baseline filenames vector and test filenames vector
  StringListIt itbaselineFilenames = baselineFilenamesAscii.begin();
  StringListIt itTestFilenames = testFilenamesAscii.begin();
  StringListIt itIgnoredLines = ignoredLines.begin();

  // Warning message
  if (ignoredLines.size() > 0)
    {
    std::cout << "The lines containing the expressions ";
    for (; itIgnoredLines != ignoredLines.end(); ++itIgnoredLines)
      {
      std::cout << (*itIgnoredLines) << " ";
      }
    std::cout << "are not considered" << std::endl;
    }

  // For each couple of baseline and test file, do the comparison
  for (; (itbaselineFilenames != baselineFilenamesAscii.end())
         && (itTestFilenames != testFilenamesAscii.end());
       ++itbaselineFilenames, ++itTestFilenames)
    {
    std::string baselineFilenameAscii = (*itbaselineFilenames);
    std::string testFilenameAscii = (*itTestFilenames);

    std::map<std::string,
             int> baselines =
      this->RegressionTestBaselines(const_cast<char*>(baselineFilenameAscii.c_str()));
    std::map<std::string, int>::reverse_iterator baseline = baselines.rbegin();
    int multiResult = 1;
    std::cout << "Number of baseline files: " << baselines.size() << std::endl;
    while (baseline != baselines.rend() && (multiResult != 0))
      {
      std::cout << "Testing non-regression on file: " << (baseline->first) << std::endl;
      this->ReportErrorsOff();
      baseline->second = this->RegressionTestAsciiFile(testFilenameAscii.c_str(),
                                                            (baseline->first).c_str(),
                                                            m_Epsilon,
                                                            ignoredLines);

      multiResult = baseline->second;
      ++baseline;
      }
    if (multiResult != 0)
      {
      baseline = baselines.rbegin();
      this->ReportErrorsOn();
      baseline->second
        = this->RegressionTestAsciiFile(testFilenameAscii.c_str(),
                                             (baseline->first).c_str(),
                                             m_Epsilon,
                                             ignoredLines);
      }
    result += multiResult;
    }

  return result;
}

int TestHelper::RegressionTestAllDiff(const StringList& baselineFilenamesAscii,
                                       const StringList& testFilenamesAscii,
                                       const StringList& ignoredLines)
{
  int result = 0;
  // Creates iterators on baseline filenames vector and test filenames vector
  StringListIt itbaselineFilenames = baselineFilenamesAscii.begin();
  StringListIt itTestFilenames = testFilenamesAscii.begin();
  StringListIt itIgnoredLines = ignoredLines.begin();

  // Warning message
  if (ignoredLines.size() > 0)
    {
    std::cout << "The lines containing the expressions ";
    for (; itIgnoredLines != ignoredLines.end(); ++itIgnoredLines)
      {
      std::cout << (*itIgnoredLines) << " ";
      }
    std::cout << "are not considered" << std::endl;
    }

  // For each couple of baseline and test file, do the comparison
  for (; (itbaselineFilenames != baselineFilenamesAscii.end())
         && (itTestFilenames != testFilenamesAscii.end());
       ++itbaselineFilenames, ++itTestFilenames)
    {
    std::string baselineFilenameAscii = (*itbaselineFilenames);
    std::string testFilenameAscii = (*itTestFilenames);

    std::map<std::string,
             int> baselines =
      this->RegressionTestBaselines(const_cast<char*>(baselineFilenameAscii.c_str()));
    std::map<std::string, int>::reverse_iterator baseline = baselines.rbegin();
    int multiResult = 1;
    std::cout << "Number of baseline files: " << baselines.size() << std::endl;
    while (baseline != baselines.rend() && (multiResult != 0))
      {
      std::cout << "Testing non-regression on file: " << (baseline->first) << std::endl;
      this->ReportErrorsOff();
      baseline->second = this->RegressionTestDiffFile(testFilenameAscii.c_str(),
                                                            (baseline->first).c_str(),
                                                            m_Epsilon,
                                                            ignoredLines);

      multiResult = baseline->second;
      ++baseline;
      }
    if (multiResult != 0)
      {
      baseline = baselines.rbegin();
      this->ReportErrorsOn();
      baseline->second
        = this->RegressionTestDiffFile(testFilenameAscii.c_str(),
                                             (baseline->first).c_str(),
                                             m_Epsilon,
                                             ignoredLines);
      }
    result += multiResult;
    }

  return result;
}

int TestHelper::RegressionTestAllBinary(const StringList& baselineFilenamesBinary,
                                        const StringList& testFilenamesBinary)
{
  int result = 0;
  // Creates iterators on baseline filenames vector and test filenames vector
  StringListIt itbaselineFilenames = baselineFilenamesBinary.begin();
  StringListIt itTestFilenames = testFilenamesBinary.begin();

  // For each couple of baseline and test file, do the comparison
  for (; (itbaselineFilenames != baselineFilenamesBinary.end())
         && (itTestFilenames != testFilenamesBinary.end());
       ++itbaselineFilenames, ++itTestFilenames)
    {
    std::string baselineFilenameBinary = (*itbaselineFilenames);
    std::string testFilenameBinary = (*itTestFilenames);

    std::map<std::string,
             int> baselines =
      this->RegressionTestBaselines(const_cast<char*>(baselineFilenameBinary.c_str()));
    std::map<std::string, int>::reverse_iterator baseline = baselines.rbegin();
    int multiResult = 1;
    std::cout << "Number of baseline files: " << baselines.size() << std::endl;
    while (baseline != baselines.rend() && (multiResult != 0))
      {
      std::cout << "Testing non-regression on file: " << (baseline->first) << std::endl;
      this->ReportErrorsOff();
      baseline->second = this->RegressionTestBinaryFile(testFilenameBinary.c_str(),
                                                             (baseline->first).c_str());

      multiResult = baseline->second;
      ++baseline;
      }
    if (multiResult != 0)
      {
      baseline = baselines.rbegin();
      this->ReportErrorsOn();
      baseline->second
        = this->RegressionTestBinaryFile(testFilenameBinary.c_str(),
                                              (baseline->first).c_str());
      }
    result += multiResult;
    }

  return result;
}

int TestHelper::RegressionTestAllOgr(const StringList& baselineFilenamesOgr,
                                     const StringList& testFilenamesOgr)
{
  int result = 0;
  // Creates iterators on baseline filenames vector and test filenames vector
  StringListIt itbaselineFilenames = baselineFilenamesOgr.begin();
  StringListIt itTestFilenames = testFilenamesOgr.begin();
  // For each couple of baseline and test file, do the comparison
  for (; (itbaselineFilenames != baselineFilenamesOgr.end())
         && (itTestFilenames != testFilenamesOgr.end());
       ++itbaselineFilenames, ++itTestFilenames)
    {
    std::string baselineFilenameOgr = (*itbaselineFilenames);
    std::string testFilenameOgr = (*itTestFilenames);

    std::map<std::string,
             int> baselines =
      this->RegressionTestBaselines(const_cast<char*>(baselineFilenameOgr.c_str()));
    std::map<std::string, int>::reverse_iterator baseline = baselines.rbegin();
    int multiResult = 1;
    std::cout << "Number of baseline OGR files: " << baselines.size() << std::endl;
    while (baseline != baselines.rend() && (multiResult != 0))
      {
      std::cout << "Testing non-regression on OGR file: " << (baseline->first) << std::endl;
      this->ReportErrorsOff();
      baseline->second = this->RegressionTestOgrFile(testFilenameOgr.c_str(),
                                                          (baseline->first).c_str(),
                                                          m_ToleranceDiffValue);

      multiResult = baseline->second;
      ++baseline;
      }
    if (multiResult != 0)
      {
      baseline = baselines.rbegin();
      this->ReportErrorsOn();
      baseline->second
        = this->RegressionTestOgrFile(testFilenameOgr.c_str(),
                                           (baseline->first).c_str(),
                                           m_ToleranceDiffValue);
      }
    result += multiResult;
    }

  return result;
}


int TestHelper::RegressionTestAsciiFile(const char * testAsciiFileName, const char * baselineAsciiFileName,
                                        const double epsilon, std::vector<std::string> ignoredLines) const
{
  std::ifstream fluxfiletest(testAsciiFileName);
  std::ifstream fluxfileref(baselineAsciiFileName);

  std::string diffAsciiFileName(testAsciiFileName);
  diffAsciiFileName += ".diff.txt";
  std::ofstream fluxfilediff;
  if (m_ReportErrors)
    {
    fluxfilediff.open(diffAsciiFileName);
    }

  std::string strfiletest;
  std::string strfileref;

  int nbdiff(0);
  int numLine(1);
  if (!fluxfiletest)
    {
    itkGenericExceptionMacro(<< "Impossible to open the test ASCII file <" << testAsciiFileName << ">.");
    }
  if (!fluxfileref)
    {
    itkGenericExceptionMacro(<< "Impossible to open the baseline ASCII file <" << baselineAsciiFileName << ">.");
    }

  //fill up a vector of string, in which each element is one line of the file
  std::vector<std::string> listLineFileRef;
  std::vector<std::string> listLineFileTest;
  while (std::getline(fluxfileref, strfileref))
    {
    listLineFileRef.push_back(strfileref);
    }
  fluxfileref.close();

  while (std::getline(fluxfiletest, strfiletest))
    {
    listLineFileTest.push_back(strfiletest);
    }
  fluxfiletest.close();

  if (m_IgnoreLineOrder)
    {
    std::sort(listLineFileRef.begin(), listLineFileRef.end());
    std::sort(listLineFileTest.begin(), listLineFileTest.end());
    }

  //These are to save up the differences
  std::vector<std::string> listStrDiffLineFileRef;
  std::vector<std::string> listStrDiffLineFileTest;

  std::vector<std::string>::iterator itRef = listLineFileRef.begin();
  std::vector<std::string>::iterator itTest = listLineFileTest.begin();
//   while (std::getline(fluxfileref, strfileref) != 0)
 while ((itRef != listLineFileRef.end()) && (itTest != listLineFileTest.end()))
    {
    strfileref = *itRef;
    strfiletest = *itTest;

    //otbMsgDevMacro(<< "Comparing " << strfileref << " -with- " << strfiletest);

    //Check is the current line should be ignored
    bool ignoreCurrentLineRef = false;
    bool ignoreCurrentLineTest = false;
    if (ignoredLines.size() > 0)
      {
      for (std::vector<std::string>::iterator itIgnoredLines = ignoredLines.begin();
           itIgnoredLines != ignoredLines.end(); ++itIgnoredLines)
        {
        std::string            ignoredLinesAscii = (*itIgnoredLines);
        std::string::size_type loc = strfileref.find(ignoredLinesAscii);
        if (loc != std::string::npos)
          {
          ignoreCurrentLineRef = true;
          }
        loc = strfiletest.find(ignoredLinesAscii);
        if (loc != std::string::npos)
          {
          ignoreCurrentLineTest = true;
          }
        }
      }


    // Ignore lines with "RTTI" and "Modified Time" inside by default : to avoid multibaseline
    // between Linux and Win32 plateforms
    if ((!ignoreCurrentLineRef) && (!ignoreCurrentLineTest))
      {
      if(isToBeIgnoredForAnyComparison(strfileref))
        ignoreCurrentLineRef = true;

      if(isToBeIgnoredForAnyComparison(strfiletest))
        ignoreCurrentLineTest = true;
      }

    //Compare the lines only if none is supposed to be ignored
    //Note: the iterator increment will take care of moving only the
    //ignored one if the order does not matter
    bool differenceFoundInCurrentLine = false;
//    if (ignoreCurrentLineRef || ignoreCurrentLineTest)
//      {
//      otbMsgDevMacro( << "* ignoring");
//      }
    if ((!ignoreCurrentLineRef) && (!ignoreCurrentLineTest))
      {
      differenceFoundInCurrentLine = CompareLines(strfileref, strfiletest, nbdiff, fluxfilediff, numLine,
                                                  listStrDiffLineFileRef, listStrDiffLineFileTest, epsilon);
//      if (!differenceFoundInCurrentLine)
//        {
//        otbMsgDevMacro( << "* no difference found");
//        }
      }

    if (ignoreCurrentLineRef) ++itRef;
    if (ignoreCurrentLineTest) ++itTest;

    if (m_IgnoreLineOrder)
      {
      if ((!ignoreCurrentLineRef) && (!ignoreCurrentLineTest))
        {
        if (differenceFoundInCurrentLine)
          {
          if (*itRef > *itTest)
            {
            fluxfilediff << "Additional line in test file: " << " : " << *itTest << std::endl;
            ++itTest;
            }
          else
            {
            fluxfilediff << "Additional line in ref file: " << " : " << *itTest << std::endl;
            ++itRef;
            }
          }
        else
          {
          ++itRef;
          ++itTest;
          }
        }
      }
    else
      {
		if ((!ignoreCurrentLineRef) && (!ignoreCurrentLineTest))
		{
			++itRef;
			++itTest;
		}
      }

    }

  //Here, the line by line comparison is finished and at least one (ref or test) is at the end
  //we simply output the content of the other one

  while (itRef != listLineFileRef.end())
    {
    strfileref = *itRef;
    std::stringstream buffstreamRef;
    std::string       strRef = "";
    buffstreamRef << strfileref;
    buffstreamRef >> strRef;
    fluxfilediff << "Additional line in ref file: " << numLine << " : " << strRef << std::endl;
    nbdiff++;
    if (m_ReportErrors)
      {
      listStrDiffLineFileRef.push_back(strfileref);
      }
    ++itRef;
    }

  while (itTest != listLineFileTest.end())
    {
    strfiletest = *itTest;
    std::stringstream buffstreamTest;
    std::string       strTest = "";
    buffstreamTest << strfiletest;
    buffstreamTest >> strTest;
    fluxfilediff << "Additional line in test file: " << numLine << " : " << strTest << std::endl;
    nbdiff++;
    if (m_ReportErrors)
      {
      listStrDiffLineFileTest.push_back(strfiletest);
      }
    ++itTest;
    }

  if (m_ReportErrors)
    {
    fluxfilediff.close();
    }

  if (nbdiff != 0 && m_ReportErrors)
    {
    std::cout << "<DartMeasurement name=\"ASCIIFileError\" type=\"numeric/int\">";
    std::cout << nbdiff;
    std::cout << "</DartMeasurement>" << std::endl;
    std::cout << "================================================================" << std::endl;
    std::cout << "baseline ASCII File : " << baselineAsciiFileName << std::endl;
    std::cout << "Test ASCII File     : " << testAsciiFileName << std::endl;
    std::cout << "Diff ASCII File     : " << diffAsciiFileName << std::endl;
    std::cout << "Tolerance value     : " << epsilon << std::endl;
    std::cout << "Tolerance max check : " << m_EpsilonBoundaryChecking << std::endl;

    //FIXME won't be intuitive for the non order case
    unsigned int numLineDiff = std::min(listStrDiffLineFileRef.size(), listStrDiffLineFileTest.size());
    std::cout << "Nb lines different : " << numLineDiff << std::endl;
    for (unsigned int i = 0; i < numLineDiff; ++i)
      {
      std::cout << "   -------------------------------" << std::endl;
      std::cout << "   Base << " << listStrDiffLineFileRef[i] << std::endl;
      std::cout << "   Test >> " << listStrDiffLineFileTest[i] << std::endl;
      }
    for (unsigned int i = numLineDiff; i < listStrDiffLineFileRef.size(); ++i)
      {
      std::cout << "   -------------------------------" << std::endl;
      std::cout << "   Base << " << listStrDiffLineFileRef[i] << std::endl;
      }
    for (unsigned int i = numLineDiff; i < listStrDiffLineFileTest.size(); ++i)
      {
      std::cout << "   -------------------------------" << std::endl;
      std::cout << "   Test >> " << listStrDiffLineFileTest[i] << std::endl;
      }
    }
  return (nbdiff != 0) ? 1 : 0;
}

int TestHelper::RegressionTestDiffFile(const char * testAsciiFileName, const char * baselineAsciiFileName,
                                        const double epsilon, StringList ignoredLines) const
{
  typedef std::vector<boost::iterator_range<std::string::const_iterator> > TokenListType;
  
  std::ifstream fluxfiletest(testAsciiFileName);
  std::ifstream fluxfileref(baselineAsciiFileName);

  std::string diffAsciiFileName(testAsciiFileName);
  diffAsciiFileName += ".diff.txt";
  std::ofstream fluxfilediff;
  if (m_ReportErrors)
    {
    fluxfilediff.open(diffAsciiFileName);
    }

  std::string strfiletest;
  std::string strfileref;

  int nbdiff(0);
  if (!fluxfiletest)
    {
    itkGenericExceptionMacro(<< "Impossible to open the test ASCII file <" << testAsciiFileName << ">.");
    }
  if (!fluxfileref)
    {
    itkGenericExceptionMacro(<< "Impossible to open the baseline ASCII file <" << baselineAsciiFileName << ">.");
    }

  //fill up a vector of string, in which each element is one line of the file
  StringList listLineFileRef;
  StringList listLineFileTest;
  while (std::getline(fluxfileref, strfileref))
    {
    listLineFileRef.push_back(strfileref);
    }
  fluxfileref.close();

  while (std::getline(fluxfiletest, strfiletest))
    {
    listLineFileTest.push_back(strfiletest);
    }
  fluxfiletest.close();

  if (m_IgnoreLineOrder)
    {
    std::sort(listLineFileRef.begin(), listLineFileRef.end());
    std::sort(listLineFileTest.begin(), listLineFileTest.end());
    }

  //These are to save up the differences
  StringList listStrDiffLineFile;

  // filter ignored lines
  // TODO
  // filter lines with hexa address (isHexaPointerAddress)
  // use  isToBeIgnoredForAnyComparison()

  // Iterate over the baseline lines
  unsigned int posTest = 0;
  unsigned int posRef = 0;
  unsigned int curPosTest;
  TokenListType tokenRef;
  TokenListType tokenTest;
  TokenListType tokenTestSelected;
  std::string separators(" \t,;:=[]{}()<>");
  std::string emptyStr("");
  unsigned int nbTokenRef = 0;
  unsigned int nbTokenTest = 0;
  unsigned int nbTokenTestSelected = 0;
  bool isFirstMissingRefLine = true;

  for (posRef=0 ; posRef<listLineFileRef.size() ; posRef++)
    {
    std::string &curLineRef = listLineFileRef[posRef];
    if (!IsLineValid(curLineRef,ignoredLines))
      {
      continue;
      }
    boost::split(tokenRef, curLineRef, boost::is_any_of(separators));
    // remove empty tokens
    TokenListType::iterator refEndFiltered = std::remove_if(tokenRef.begin(), tokenRef.end(), IsTokenEmpty);
    tokenRef.resize(refEndFiltered-tokenRef.begin());
    nbTokenRef = tokenRef.size();
    if (nbTokenRef == 0)
      {
      // ignore lines without tokens
      continue;
      }
    unsigned int bestLinePos = posTest;
    int bestCommonTokens = -1;
    std::vector<unsigned int> validTestLines;
    std::vector<unsigned int> skippedTestLines;
    std::vector<unsigned int> differencesPosSelected;
    for (curPosTest = posTest ; curPosTest < listLineFileTest.size() ; curPosTest++)
      {
      // stop searching after 30 lines tested
      if (curPosTest > posTest + 30)
        {
        break;
        }
      std::string &curLineTest = listLineFileTest[curPosTest];
      if (!IsLineValid(curLineTest,ignoredLines))
        {
        continue;
        }
      boost::split(tokenTest, curLineTest, boost::is_any_of(separators));
      // remove empty tokens
      TokenListType::iterator testEndFiltered = std::remove_if(tokenTest.begin(), tokenTest.end(), IsTokenEmpty);
      tokenTest.resize(testEndFiltered-tokenTest.begin());
      nbTokenTest = tokenTest.size();
      if (nbTokenTest == 0)
        {
        // ignore lines without tokens
        continue;
        }
      //if (curPosTest == posTest)
      //  {
      //  tokenTestSelected = tokenTest;
      //  }

      // from this point : the test line is considered valid
      validTestLines.push_back(curPosTest);

      // get number of equivalent separators and tokens to find the best match
      int commonTokens = 0;
      int firstCommonTokens = 0;
      int firstNumericToken = -1;
      unsigned int commonSeparators = 0;
      std::vector<unsigned int> differencesPos;
      for (unsigned int i = 0 ; i < std::min(nbTokenRef,nbTokenTest) ; i++)
        {
        //------ comparing previous separator ------
        std::string sepRef("");
        std::string sepTest("");
        unsigned int endSepRef;
        unsigned int endSepTest;
        unsigned int startSepRef;
        unsigned int startSepTest;
        if (i)
          {
          startSepRef  = tokenRef[i-1].end() - curLineRef.begin();
          startSepTest = tokenTest[i-1].end() - curLineTest.begin();
          endSepRef  = tokenRef[i].begin() - curLineRef.begin();
          endSepTest = tokenTest[i].begin() - curLineTest.begin();
          }
        else
          {
          startSepRef  = 0;
          startSepTest = 0;
          endSepRef  = tokenRef[0].begin() - curLineRef.begin();
          endSepTest = tokenTest[0].begin() - curLineTest.begin();
          }
        for (unsigned int iRef=startSepRef ; iRef < endSepRef ; iRef++ )
          {
          if (curLineRef[iRef] != ' ' && curLineRef[iRef] != '\t')
            sepRef += curLineRef[iRef];
          }
        for (unsigned int iTest=startSepTest ; iTest < endSepTest ; iTest++ )
          {
          if (curLineTest[iTest] != ' ' && curLineTest[iTest] != '\t')
            sepTest += curLineTest[iTest];
          }
        if (sepRef.compare(sepTest) == 0)
          {
          commonSeparators++;
          }
        else
          {
          break;
          }
        // ----------- comparing current token --------------------
        bool areTokensEquivalent = false;
        // first : try direct string comparison
        if (curLineRef.compare(
              tokenRef[i].begin() - curLineRef.begin(),
              tokenRef[i].size(),
              curLineTest,
              tokenTest[i].begin() - curLineTest.begin(),
              tokenTest[i].size()) == 0)
          {
          areTokensEquivalent = true;
          }
        else
          {
          // examine other cases :
          // test if hexadecimal address
          bool isRefTokenHexa = isHexaPointerAddress(
                                  curLineRef,
                                  tokenRef[i].begin()-curLineRef.begin(),
                                  tokenRef[i].size());
          bool isTestTokenHexa = isHexaPointerAddress(
                                  curLineTest,
                                  tokenTest[i].begin()-curLineTest.begin(),
                                  tokenTest[i].size());
          if (isRefTokenHexa && isTestTokenHexa)
            {
            // these tokens are equivalent (we don't compare pointer address)
            areTokensEquivalent = true;
            }
          else
            {
            double vTest=0.;
            double vRef=0.;
            double vNorm=0.;
            // cast ref token
            bool isRefTokenNum = true;
            try
              {
              vRef = boost::lexical_cast<double>(tokenRef[i]);
              // record the first numeric token position
              if (firstNumericToken<0)
                {
                firstNumericToken = static_cast<int>(i);
                }
              }
            catch (boost::bad_lexical_cast &)
              {
              isRefTokenNum = false;
              }
            // cast test token
            bool isTestTokenNum = true;
            try
              {
              vTest = boost::lexical_cast<double>(tokenTest[i]);
              }
            catch (boost::bad_lexical_cast &)
              {
              isTestTokenNum = false;
              }
            if (isRefTokenNum && isTestTokenNum)
              {
              // test difference against epsilon
              vNorm = (std::abs(vRef) + std::abs(vTest)) * 0.5;
              if ((vNorm <= m_EpsilonBoundaryChecking) //make sure that either the test of the ref are non 0
                || (std::abs(vRef-vTest) <= epsilon * vNorm)) //epsilon as relative error
                {
                // these tokens are equivalent
                areTokensEquivalent = true;
                }
              }
            else
              {
              // test for special tokens
              for (unsigned int j=0 ; j<m_SpecialTokens.size() ; j++)
                {
                if (curLineRef.compare(
                      tokenRef[i].begin() - curLineRef.begin(),
                      tokenRef[i].size(),
                      m_SpecialTokens[j].first) == 0 &&
                    curLineTest.compare(
                      tokenTest[i].begin() - curLineTest.begin(),
                      tokenTest[i].size(),
                      m_SpecialTokens[j].second) == 0)
                  {
                  areTokensEquivalent = true;
                  break;
                  }
                }
              }
            }
          }
        if (areTokensEquivalent)
          {
          commonTokens++;
          if (differencesPos.empty())
            {
            firstCommonTokens++;
            }
          }
        else
          {
          differencesPos.push_back(i);
          }
        }
      // finish iteration of all tokens from reference line to detect first numeric token
      for (unsigned int i = std::min(nbTokenRef,nbTokenTest) ; i < nbTokenRef ; i++)
        {
        try
          {
          boost::lexical_cast<double>(tokenRef[i]);
          // record the first numeric token position
          if (firstNumericToken<0)
            {
            firstNumericToken = static_cast<int>(i);
            }
          }
        catch (boost::bad_lexical_cast &)
          {
          }
        }

      if (commonSeparators < std::min(nbTokenRef,nbTokenTest))
        {
        // All the separators are not equivalent : reject line
        continue;
        }
      if (firstNumericToken > firstCommonTokens)
        {
        // if there are non-numeric tokens before a numeric one, all non-numeric
        // tokens must match (we make sure to compare the same field name)
        continue;
        }
      if (firstCommonTokens > bestCommonTokens)
        {
        bestCommonTokens = firstCommonTokens;
        bestLinePos = curPosTest;
        tokenTestSelected = tokenTest;
        nbTokenTestSelected = nbTokenTest;
        differencesPosSelected = differencesPos;
        skippedTestLines.clear();
        unsigned int k=0;
        while (validTestLines[k] < curPosTest)
          {
          skippedTestLines.push_back(validTestLines[k]);
          k++;
          }
        }
      // test if lines are identic
      if (static_cast<unsigned int>(firstCommonTokens) == std::max(nbTokenRef,nbTokenTest))
        {
        break;
        }
      }

    // depending on the best number of common tokens ...
    if (bestCommonTokens <= 0)
      {
      // line not found in test
      if (isFirstMissingRefLine)
        {
        listStrDiffLineFile.push_back("   -------------------------------");
        isFirstMissingRefLine = false;
        }
      listStrDiffLineFile.push_back(std::string("   Base << ")+curLineRef);
      if (m_ReportErrors)
        {
        fluxfilediff << "In baseline l."<<posRef+1<<" : "<<curLineRef<< std::endl;
        }
      nbdiff++;
      }
    else
      {
      // record skipped lines in test file
      for (unsigned int k=0 ; k < skippedTestLines.size() ; k++)
        {
        isFirstMissingRefLine = true;
        if (k== 0)
          listStrDiffLineFile.push_back("   -------------------------------");
        listStrDiffLineFile.push_back(std::string("   Test >> ")+listLineFileTest[skippedTestLines[k]]);
        if (m_ReportErrors)
          {
          fluxfilediff << "In test     l."<<skippedTestLines[k]+1<<" : "<<listLineFileTest[skippedTestLines[k]] << std::endl;
          }
        nbdiff++;
        }

      if (bestCommonTokens < static_cast<int>(std::max(nbTokenRef,nbTokenTestSelected)))
        {
        nbdiff++;
        std::string &lineTestSelected = listLineFileTest[bestLinePos];
        // record the diff
        std::ostringstream ossBase;
        std::ostringstream ossTest;
        for (unsigned int k=0 ; k < std::min(nbTokenRef,nbTokenTestSelected) ; k++)
          {
          // record the separator
          unsigned int sizeSepRef;
          unsigned int startSepRef;
          if (k)
            {
            startSepRef = tokenRef[k-1].end()-curLineRef.begin();
            sizeSepRef = tokenRef[k].begin()-tokenRef[k-1].end();
            }
          else
            {
            startSepRef = 0;
            sizeSepRef = tokenRef[0].begin()-curLineRef.begin();
            }
          ossBase << curLineRef.substr(startSepRef, sizeSepRef);
          for (unsigned int j=0 ; j<sizeSepRef ; j++)
            {
            if (curLineRef[startSepRef+j] == '\t')
              {
              ossTest << "\t";
              }
            else
              {
              ossTest << " ";
              }
            }
          // record the token
          bool isTokenEquivalent = true;
          for (unsigned int t=0 ; t < differencesPosSelected.size() ; t++)
            {
            if (k == differencesPosSelected[t])
              {
              isTokenEquivalent = false;
              break;
              }
            }
          if (isTokenEquivalent)
            {
            ossBase << curLineRef.substr(tokenRef[k].begin()-curLineRef.begin(),
                                         tokenRef[k].size());
            ossTest << std::string(tokenRef[k].size(), '_');
            }
          else
            {
            unsigned int maxTokenSize = std::max(tokenRef[k].size(),tokenTestSelected[k].size());
            ossBase << curLineRef.substr(tokenRef[k].begin()-curLineRef.begin(),
                                         tokenRef[k].size())
                    << std::string(maxTokenSize - tokenRef[k].size(), ' ');
            ossTest << lineTestSelected.substr(tokenTestSelected[k].begin()-lineTestSelected.begin(),
                                               tokenTestSelected[k].size())
                    << std::string(maxTokenSize - tokenTestSelected[k].size(), ' ');
            }
          }
        // process token in excess on any side
        if (nbTokenRef >= nbTokenTestSelected)
          {
          ossBase << curLineRef.substr(tokenRef[nbTokenTestSelected-1].end()-curLineRef.begin());
          if (nbTokenRef > nbTokenTestSelected)
            ossTest << "|";
          }
        else if (nbTokenRef < nbTokenTestSelected)
          {
          ossBase << "|";
          ossTest << lineTestSelected.substr(tokenTestSelected[nbTokenRef-1].end()-lineTestSelected.begin());
          }
        listStrDiffLineFile.push_back("   -------------------------------");
        listStrDiffLineFile.push_back(std::string("   Base << ")+ossBase.str());
        listStrDiffLineFile.push_back(std::string("   Test >> ")+ossTest.str());
        if (m_ReportErrors)
          {
          fluxfilediff << "In baseline l."<< posRef+1 <<" : "<< ossBase.str() << std::endl;
          fluxfilediff << "In test     l."<< bestLinePos+1 << " : "<<ossTest.str() << std::endl;
          }
        isFirstMissingRefLine = true;
        }

      // update posTest
      posTest = bestLinePos + 1;
      }
    }

  // process remaining lines in test file
  for (curPosTest = posTest ; curPosTest < listLineFileTest.size() ; curPosTest++)
    {
    std::string &curLineTest = listLineFileTest[curPosTest];
    if (!IsLineValid(curLineTest,ignoredLines))
      {
      continue;
      }
    boost::split(tokenTest, curLineTest, boost::is_any_of(separators));
    // remove empty tokens
    TokenListType::iterator testEndFiltered = std::remove_if(tokenTest.begin(), tokenTest.end(), IsTokenEmpty);
    tokenTest.resize(testEndFiltered-tokenTest.begin());
    nbTokenTest = tokenTest.size();
    if (nbTokenTest == 0)
      {
      // ignore lines without tokens
      continue;
      }
    // this test line is valid
    //listStrDiffLineFile.push_back("   -------------------------------");
    listStrDiffLineFile.push_back(std::string("   Test >> ")+curLineTest);
    if (m_ReportErrors)
      {
      fluxfilediff << "In test     l."<<curPosTest+1<<" : "<<curLineTest << std::endl;
      }
    nbdiff++;
    }
  
  if (m_ReportErrors)
    {
    fluxfilediff.close();
    }

  if (nbdiff != 0 && m_ReportErrors)
    {
    std::cout << "<DartMeasurement name=\"ASCIIFileError\" type=\"numeric/int\">";
    std::cout << nbdiff;
    std::cout << "</DartMeasurement>" << std::endl;
    std::cout << "================================================================" << std::endl;
    std::cout << "baseline ASCII File : " << baselineAsciiFileName << std::endl;
    std::cout << "Test ASCII File     : " << testAsciiFileName << std::endl;
    std::cout << "Diff ASCII File     : " << diffAsciiFileName << std::endl;
    std::cout << "Tolerance value     : " << epsilon << std::endl;
    std::cout << "Tolerance max check : " << m_EpsilonBoundaryChecking << std::endl;
    std::cout << "Nb lines different : " << nbdiff << std::endl;
    
    for (unsigned int i=0 ; i<listStrDiffLineFile.size() ; i++)
      {
      std::cout << listStrDiffLineFile[i] << std::endl;
      }
    }
  
  return (nbdiff != 0) ? 1 : 0;
}

bool TestHelper::IsTokenEmpty(boost::iterator_range<std::string::const_iterator> &token)
{
  return token.empty();
}

int TestHelper::TokenizeLine(const std::string &line, StringList &tokens) const
{
  std::string::size_type pos;
  tokens.clear();
  std::string::size_type counter = 0;
  bool isSeparator;
  bool isToken = false;
  int curChar;
  for (pos = 0 ; pos < line.size() ; pos++)
    {
    isSeparator = true;
    curChar = (int)(line[pos]);
    // test if alphanumeric or '.' or '-' or '^' or '_' or '/'
    if (isAlphaNum(curChar) || isPoint(curChar) || isMinusSign(curChar) ||
        curChar == 94 || curChar == 95 || curChar == 47)
      {
      isSeparator = false;
      }

    if (isToken)
      {
      // already in a token
      if (isSeparator)
        {
        // end of the token : record it
        tokens.push_back(line.substr(pos-counter, counter));
        isToken = false;
        counter = 0;tokens.push_back(line.substr(pos-counter, counter));
        }
      else
        {
        // still in a token
        counter++;
        }
      }
    else
      {
      // not in a token
      if (isSeparator)
        {
        // do nothing
        }
      else
        {
        // start of a token
        isToken = true;
        counter++;
        }
      }
    }
  // handle final token
  if (isToken)
    {
    tokens.push_back(line.substr(line.size()-counter, counter));
    }

  return tokens.size();
}

bool TestHelper::IsLineValid(const std::string& str, const StringList &ignoredLines) const
{
  bool ret = true;
  if (isToBeIgnoredForAnyComparison(str))
    {
    ret = false;
    }
  else
    {
    for (unsigned int i=0 ; i<ignoredLines.size() ; i++)
      {
      if (str.find(ignoredLines[i]) != std::string::npos)
        {
        ret = false;
        break;
        }
      }
    }
  return ret;
}

/******************************************/
/******************************************/
/******************************************/

int TestHelper::RegressionTestBinaryFile(const char * testBinaryFileName, const char * baselineBinaryFileName) const
{
  int           nbdiff(0);
  std::ifstream fluxfiletest(testBinaryFileName, std::ifstream::binary);
  std::ifstream fluxfileref(baselineBinaryFileName, std::ifstream::binary);
  if (fluxfiletest.fail())
    {
    itkGenericExceptionMacro(<< "Impossible to open the test binary file <" << testBinaryFileName << ">.");
    }
  if (!fluxfileref)
    {
    itkGenericExceptionMacro(<< "Impossible to open the baseline binary file <" << baselineBinaryFileName << ">.");
    }
  while (fluxfiletest.good() && fluxfileref.good())
    {
    if (fluxfiletest.get() != fluxfileref.get())
      {
      nbdiff++;
      }
    }
  fluxfiletest.close();
  fluxfileref.close();

  if (nbdiff != 0 && m_ReportErrors)
    {
    std::cout << "<DartMeasurement name=\"BINARYFileError\" type=\"numeric/int\">";
    std::cout << nbdiff;
    std::cout << "</DartMeasurement>" << std::endl;
    }
  return (nbdiff != 0) ? 1 : 0;
}

int TestHelper::RegressionTestImage(int cpt, const char *testImageFilename, const char *baselineImageFilename,
                                    const double toleranceDiffPixelImage) const
{
  // Use the factory mechanism to read the test and baseline files and convert them to double

  typedef otb::VectorImage<double, 2>        ImageType;
  typedef otb::ImageFileReader<ImageType>    ReaderType;

  // Read the baseline file
  ReaderType::Pointer baselineReader = ReaderType::New();
  baselineReader->SetFileName(baselineImageFilename);
  try
    {
    baselineReader->UpdateLargestPossibleRegion();
    }
  catch (itk::ExceptionObject& e)
    {
    itkGenericExceptionMacro(<< "Exception detected while reading "
                             << baselineImageFilename << " : " << e.GetDescription());
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
    itkGenericExceptionMacro(<< "Exception detected while reading "
                             << testImageFilename << " : " << e.GetDescription());
    return 1000;
    }

  // The sizes of the baseline and test image must match
  ImageType::SizeType baselineSize;
  baselineSize = baselineReader->GetOutput()->GetLargestPossibleRegion().GetSize();
  ImageType::SizeType testSize;
  testSize = testReader->GetOutput()->GetLargestPossibleRegion().GetSize();

  if (baselineSize != testSize)
    {
    std::cerr << "The size of the baseline image and Test image do not match!" << std::endl;
    std::cerr << "baseline image: " << baselineImageFilename << " has size " << baselineSize << std::endl;
    std::cerr << "Test image:     " << testImageFilename << " has size " << testSize << std::endl;
    return 1;
    }

  // Now compare the two images
  typedef otb::DifferenceImageFilter<ImageType, ImageType> DiffType;
  DiffType::Pointer diff = DiffType::New();
  diff->SetValidInput(baselineReader->GetOutput());
  diff->SetTestInput(testReader->GetOutput());
  diff->SetDifferenceThreshold(toleranceDiffPixelImage);
  diff->UpdateLargestPossibleRegion();

  ImageType::PixelType status = diff->GetTotalDifference();
  unsigned long        numberOfPixelsWithDifferences = diff->GetNumberOfPixelsWithDifferences();

  //Write only one this message
  if (m_ReportErrors == 0)
    {
    otbGenericMsgDebugMacro(<< "RegressionTestImage DifferenceThreshold: " << toleranceDiffPixelImage);
    otbGenericMsgDebugMacro(<< "Status diff->GetTotalDifference:         "
                            << status << " for " << numberOfPixelsWithDifferences << " pixel(s).");
    }
  // if there are discrepencies, create an diff image
  if (status.GetSquaredNorm() > 0 && m_ReportErrors)
    {
    typedef otb::PrintableImageFilter<ImageType>               RescaleType;
    typedef otb::ImageFileWriter<RescaleType::OutputImageType> WriterType;

    RescaleType::Pointer rescale = RescaleType::New();
    WriterType::Pointer writer = WriterType::New();
    writer->SetInput(rescale->GetOutput());

    std::cout << "<DartMeasurement name=\"ImageError " << cpt << "\" type=\"numeric/double\">";
    std::cout << status;
    std::cout << "</DartMeasurement>" << std::endl;

    std::cout << "<DartMeasurement name=\"NumberOfPixelsWithDifferences " << cpt << "\" type=\"numeric/integer\">";
    std::cout << numberOfPixelsWithDifferences;
    std::cout << "</DartMeasurement>" << std::endl;

    std::cout << "<DartMeasurement name=\"ToleranceDiffPixelImage\" type=\"numeric/double\">";
    std::cout << toleranceDiffPixelImage;
    std::cout << "</DartMeasurement>" << std::endl;

    diff->UpdateOutputInformation();
    ImageType::SizeType size = diff->GetOutput()->GetLargestPossibleRegion().GetSize();

    if( size[0]*size[1] <= m_MaxArea)
      {
      std::ostringstream diffName;
      diffName << testImageFilename << ".diff.png";
      try
        {
        rescale->SetInput(diff->GetOutput());

        for (unsigned int i = 1; i <= std::min(diff->GetOutput()->GetNumberOfComponentsPerPixel(), 3U); ++i)
          {
          rescale->SetChannel(i);
          }

        rescale->Update();
        }
      catch (...)
        {
        itkGenericExceptionMacro(<< "Error during rescale of " << diffName.str());
        }
      writer->SetFileName(diffName.str());
      try
        {
        writer->Update();
        }
      catch (...)
        {
        itkGenericExceptionMacro(<< "Error during write of " << diffName.str());
        }

      std::cout << "<DartMeasurementFile name=\"DifferenceImage " << cpt << "\" type=\"image/png\">";
      std::cout << diffName.str();
      std::cout << "</DartMeasurementFile>" << std::endl;
      }
    else
      {
      std::cout << "<DartMeasurement name=\"DifferenceImage " << cpt << "\" type=\"text/string\">";
      std::cout << "*************************************************************\n";
      std::cout << "********** DIFFERENCE IMAGE TOO BIG TO BE SUBMITTED *********\n";
      std::cout << "*************************************************************";
      std::cout << "</DartMeasurement>" << std::endl;
      }

    baselineReader->UpdateOutputInformation();
    size = baselineReader->GetOutput()->GetLargestPossibleRegion().GetSize();

    if( size[0]*size[1] <= m_MaxArea)
      {
      std::ostringstream baseName;
      baseName << testImageFilename << ".base.png";
      try
        {
        rescale->SetInput(baselineReader->GetOutput());
        rescale->Update();
        }
      catch (...)
        {
        itkGenericExceptionMacro(<< "Error during rescale of " << baseName.str());
        }
      try
        {
        writer->SetFileName(baseName.str());
        writer->Update();
        }
      catch (...)
        {
        itkGenericExceptionMacro(<< "Error during write of " << baseName.str());
        }

      std::cout << "<DartMeasurementFile name=\"BaselineImage " << cpt << "\" type=\"image/png\">";
      std::cout << baseName.str();
      std::cout << "</DartMeasurementFile>" << std::endl;
      }
    else
      {
      std::cout << "<DartMeasurement name=\"BaselineImage " << cpt << "\" type=\"text/string\">";
      std::cout << "*************************************************************\n";
      std::cout << "*********** BASELINE IMAGE TOO BIG TO BE SUBMITTED **********\n";
      std::cout << "*************************************************************";
      std::cout << "</DartMeasurement>" << std::endl;
      }

    testReader->UpdateOutputInformation();
    size = testReader->GetOutput()->GetLargestPossibleRegion().GetSize();

    if( size[0]*size[1] <= m_MaxArea)
      {
      std::ostringstream testName;
      testName << testImageFilename << ".test.png";
      try
        {
        rescale->SetInput(testReader->GetOutput());
        rescale->Update();
        }
      catch (...)
        {
        itkGenericExceptionMacro(<< "Error during rescale of " << testName.str());
        }
      try
        {
        writer->SetFileName(testName.str());
        writer->Update();
        }
      catch (...)
        {
        itkGenericExceptionMacro(<< "Error during write of " << testName.str());
        }

      std::cout << "<DartMeasurementFile name=\"TestImage " << cpt << "\" type=\"image/png\">";
      std::cout << testName.str();
      std::cout << "</DartMeasurementFile>" << std::endl;
      }
    else
      {
      std::cout << "<DartMeasurement name=\"TestImage " << cpt << "\" type=\"text/string\">";
      std::cout << "*************************************************************\n";
      std::cout << "************* TEST IMAGE TOO BIG TO BE SUBMITTED ************\n";
      std::cout << "*************************************************************";
      std::cout << "</DartMeasurement>" << std::endl;
      }
    }
  return (status.GetSquaredNorm() > 0) ? 1 : 0;
}

int TestHelper::RegressionTestMetaData(const char *testImageFilename, const char *baselineImageFilename,
                                       const double /*toleranceDiffPixelImage*/) const
{
  // Use the factory mechanism to read the test and baseline files and convert them to double
  typedef otb::Image<double, ITK_TEST_DIMENSION_MAX> ImageType;
  typedef otb::ImageFileReader<ImageType>            ReaderType;

  // Read the baseline file
  ReaderType::Pointer baselineReader = ReaderType::New();
  baselineReader->SetFileName(baselineImageFilename);
  try
    {
    baselineReader->GenerateOutputInformation();
    }
  catch (itk::ExceptionObject& e)
    {
    itkGenericExceptionMacro(<< "Exception detected while reading "
                             << baselineImageFilename << " : " << e.GetDescription());
    }

  // Read the baseline file
  ReaderType::Pointer testReader = ReaderType::New();
  testReader->SetFileName(testImageFilename);
  try
    {
    testReader->GenerateOutputInformation();
    }
  catch (itk::ExceptionObject& e)
    {
    itkGenericExceptionMacro(<< "Exception detected while reading "
                             << baselineImageFilename << " : " << e.GetDescription());
    }

  unsigned int errcount = 0;
  // The sizes of the baseline and test image must match
  ImageType::SizeType baselineSize;
  baselineSize = baselineReader->GetOutput()->GetLargestPossibleRegion().GetSize();
  ImageType::SizeType testSize;
  testSize = testReader->GetOutput()->GetLargestPossibleRegion().GetSize();

  if (baselineSize != testSize)
    {
    std::cerr << "The size of the baseline image and Test image do not match!" << std::endl;
    std::cerr << "baseline image: " << baselineImageFilename << " has size " << baselineSize << std::endl;
    std::cerr << "Test image:     " << testImageFilename << " has size " << testSize << std::endl;
    errcount++;
    }
  ImageType::Pointer blImPtr = baselineReader->GetOutput();
  ImageType::Pointer testImPtr = testReader->GetOutput();

  // test origin
  if (blImPtr->GetOrigin() != testImPtr->GetOrigin())
    {
    std::cerr << "The origin of the baseline image and Test image do not match!" << std::endl;
    std::cerr << "baseline image: " << baselineImageFilename << " has origin " << blImPtr->GetOrigin() << std::endl;
    std::cerr << "Test image:     " << testImageFilename << " has origin " << testImPtr->GetOrigin() << std::endl;
    errcount++;
    }

  // test spacing
  if (blImPtr->GetSignedSpacing() != testImPtr->GetSignedSpacing())
    {
    std::cerr << "The spacing of the baseline image and Test image do not match!" << std::endl;
    std::cerr << "baseline image: " << baselineImageFilename << " has spacing " << blImPtr->GetSignedSpacing() << std::endl;
    std::cerr << "Test image:     " << testImageFilename << " has spacing " << testImPtr->GetSignedSpacing() << std::endl;
    errcount++;
    }

  // test projection reference
  if (blImPtr->GetProjectionRef().compare(testImPtr->GetProjectionRef()) != 0)
    {
    std::cerr << "The projection reference of the baseline image and Test image do not match!" << std::endl;
    std::cerr << "baseline image: " << baselineImageFilename << " has projection reference "
              << blImPtr->GetProjectionRef() << std::endl;
    std::cerr << "Test image:     " << testImageFilename << " has projection reference "
              << testImPtr->GetProjectionRef() << std::endl;
    errcount++;
    }

  // test Geographic transform
  if (blImPtr->GetGeoTransform() != testImPtr->GetGeoTransform())
    {
    std::cerr << "The geographic transform of the baseline image and Test image do not match!" << std::endl;
    std::cerr << "baseline image: " << baselineImageFilename << " has geographic transform " << VectorToString(
      blImPtr->GetGeoTransform()) << std::endl;
    std::cerr << "Test image:     " << testImageFilename << " has geographic transform " << VectorToString(
      testImPtr->GetGeoTransform()) << std::endl;
    errcount++;
    }

  // test upper left corner
  if (blImPtr->GetUpperLeftCorner() != testImPtr->GetUpperLeftCorner())
    {
    std::cerr << "The upper left corner of the baseline image and Test image do not match!" << std::endl;
    std::cerr << "baseline image: " << baselineImageFilename << " has upper left corner " << VectorToString(
      blImPtr->GetUpperLeftCorner()) << std::endl;
    std::cerr << "Test image:     " << testImageFilename << " has upper left corner " << VectorToString(
      testImPtr->GetUpperLeftCorner()) << std::endl;
    errcount++;
    }

  // test upper right corner
  if (blImPtr->GetUpperRightCorner() != testImPtr->GetUpperRightCorner())
    {
    std::cerr << "The upper right corner of the baseline image and Test image do not match!" << std::endl;
    std::cerr << "baseline image: " << baselineImageFilename << " has upper right corner " << VectorToString(
      blImPtr->GetUpperRightCorner()) << std::endl;
    std::cerr << "Test image:     " << testImageFilename << " has upper right corner " << VectorToString(
      testImPtr->GetUpperRightCorner()) << std::endl;
    errcount++;
    }

  // test lower left corner
  if (blImPtr->GetLowerLeftCorner() != testImPtr->GetLowerLeftCorner())
    {
    std::cerr << "The lower left corner  of the baseline image and Test image do not match!" << std::endl;
    std::cerr << "baseline image: " << baselineImageFilename << " has lower left corner " << VectorToString(
      blImPtr->GetLowerLeftCorner()) << std::endl;
    std::cerr << "Test image:     " << testImageFilename << " has lower left corner " << VectorToString(
      testImPtr->GetLowerLeftCorner()) << std::endl;
    errcount++;
    }

  // test lower right corner
  if (blImPtr->GetLowerRightCorner() != testImPtr->GetLowerRightCorner())
    {
    std::cerr << "The lower right of the baseline image and Test image do not match!" << std::endl;
    std::cerr << "baseline image: " << baselineImageFilename << " has lower right corner " << VectorToString(
      blImPtr->GetLowerRightCorner()) << std::endl;
    std::cerr << "Test image:     " << testImageFilename << " has lower right corner " << VectorToString(
      testImPtr->GetLowerRightCorner()) << std::endl;
    errcount++;
    }

  // test gcp projection
  if (blImPtr->GetGCPProjection().compare(testImPtr->GetGCPProjection()) != 0)
    {
    std::cerr << "The gcp projection of the baseline image and Test image do not match!" << std::endl;
    std::cerr << "baseline image: " << baselineImageFilename << " has gcp projection " << blImPtr->GetGCPProjection()
              << std::endl;
    std::cerr << "Test image:     " << testImageFilename << " has gcp projection " << testImPtr->GetGCPProjection()
              << std::endl;
    errcount++;
    }

  // test gcp count
  if (blImPtr->GetGCPCount() != testImPtr->GetGCPCount())
    {
    std::cerr << "The gcp count of the baseline image and Test image do not match!" << std::endl;
    std::cerr << "baseline image: " << baselineImageFilename << " has gcp count " << blImPtr->GetGCPCount()
              << std::endl;
    std::cerr << "Test image:     " << testImageFilename << " has gcp count " << testImPtr->GetGCPCount() << std::endl;
    errcount++;
    }
  else
    {
    for (unsigned int i = 0; i < blImPtr->GetGCPCount(); ++i)
      {
      if ((blImPtr->GetGCPId(i).compare(testImPtr->GetGCPId(i)) != 0) || (blImPtr->GetGCPInfo(i).compare(
                                                                            testImPtr->GetGCPInfo(i)) != 0) ||
          (blImPtr->GetGCPRow(i) != testImPtr->GetGCPRow(i)) || (blImPtr->GetGCPCol(
                                                                   i)
                                                                 != testImPtr->GetGCPCol(i)) ||
          (blImPtr->GetGCPX(i) != testImPtr->GetGCPX(i)) || (blImPtr->GetGCPY(i)
                                                             !=
                                                             testImPtr
                                                             ->GetGCPY(i)) ||
          (blImPtr->GetGCPZ(i) != testImPtr->GetGCPZ(i)))
        {
        std::cerr << "The GCP number " << i << " of the baseline image and Test image do not match!" << std::endl;
        std::cerr << "baseline image: " << baselineImageFilename << " has gcp number " << i << " (" << "id: "
                  << blImPtr->GetGCPId(i) << ", " << "info: " << blImPtr->GetGCPInfo(i) << ", " << "row: "
                  << blImPtr->GetGCPRow(i) << ", " << "col: " << blImPtr->GetGCPCol(i) << ", " << "X: "
                  << blImPtr->GetGCPX(i) << ", " << "Y: " << blImPtr->GetGCPY(i) << ", " << "Z: " << blImPtr->GetGCPZ(i)
                  << ")" << std::endl;
        std::cerr << "Test image:     " << testImageFilename << " has gcp  number " << i << " (" << "id: "
                  << testImPtr->GetGCPId(i) << ", " << "info: " << testImPtr->GetGCPInfo(i) << ", " << "row: "
                  << testImPtr->GetGCPRow(i) << ", " << "col: " << testImPtr->GetGCPCol(i) << ", " << "X: "
                  << testImPtr->GetGCPX(i) << ", " << "Y: " << testImPtr->GetGCPY(i) << ", " << "Z: "
                  << testImPtr->GetGCPZ(i) << ")" << std::endl;
        errcount++;
        }
      }
    }
  if (errcount > 0)
    {
    std::cout << "<DartMeasurement name=\"MetadataError\" type=\"numeric/int\">";
    std::cout << errcount;
    std::cout << "</DartMeasurement>" << std::endl;
    }
  return errcount;

}

//
// Generate all of the possible baselines
// The possible baselines are generated fromn the baselineFilename using the following algorithm:
// 1) strip the suffix
// 2) append a digit _x
// 3) append the original suffix.
// It the file exists, increment x and continue
//
std::map<std::string, int> TestHelper::RegressionTestBaselines(char *baselineFilename) const
{
  std::map<std::string, int> baselines;
  baselines[std::string(baselineFilename)] = 0;

  std::string myPath = itksys::SystemTools::GetFilenamePath(baselineFilename);
  itksys::Directory myDir;
  myDir.Load(myPath.c_str());
  const unsigned int nbFiles = myDir.GetNumberOfFiles();

  const std::string originalBaseline = itksys::SystemTools::GetFilenameWithoutLastExtension(baselineFilename);
  const unsigned int sizeRef = originalBaseline.size();
  const bool HasExtension = (sizeRef != itksys::SystemTools::GetFilenameName(baselineFilename).size());
  const std::string blFileExtension = itksys::SystemTools::GetFilenameLastExtension(baselineFilename);

  myPath.append("/");
  itksys::RegularExpression reg;
  reg.compile("^\\.[0-9]");

  for (unsigned int i = 0; i < nbFiles; i++)
    {
    std::string curFilename;
    const std::string curFileExtension = itksys::SystemTools::GetFilenameLastExtension(myDir.GetFile(i));
    if (HasExtension)
      {
      curFilename = itksys::SystemTools::GetFilenameWithoutLastExtension(myDir.GetFile(i));
      }
    else
      {
      curFilename = itksys::SystemTools::GetFilenameName(myDir.GetFile(i));
      }

    if ((curFilename.substr(0, sizeRef) == originalBaseline)
        && reg.find(curFilename.substr(sizeRef, curFilename.size()))
        && (curFileExtension.compare(blFileExtension) == 0))
      {
      std::string myFile = myPath;
      baselines[myFile.append(myDir.GetFile(i))] = 0;
      }
    }

  return baselines;
}

int TestHelper::RegressionTestOgrFile(const char *testOgrFilename, const char *baselineOgrFilename,
                                      const double toleranceDiffValue) const
{
  const char *ref_pszDataSource = baselineOgrFilename;
  const char *test_pszDataSource = testOgrFilename;
  //const char *ref_pszWHERE = NULL;
  //const char *test_pszWHERE = NULL;
  int         bReadOnly = FALSE;
  int         nbdiff(0);
  /* -------------------------------------------------------------------- */
  /*      Open data source.                                               */
  /* -------------------------------------------------------------------- */
  otb::ogr::version_proxy::GDALDatasetType *ref_poDS = nullptr;
  otb::ogr::version_proxy::GDALDriverType *  ref_poDriver = nullptr;
  //OGRGeometry *  ref_poSpatialFilter = NULL;
  otb::ogr::version_proxy::GDALDatasetType *test_poDS = nullptr;
  otb::ogr::version_proxy::GDALDriverType *  test_poDriver = nullptr;
  //OGRGeometry *  test_poSpatialFilter = NULL;

  ref_poDS = otb::ogr::version_proxy::Open(ref_pszDataSource, false);
  if (ref_poDS == nullptr && !bReadOnly)
    {
    ref_poDS = otb::ogr::version_proxy::Open(ref_pszDataSource, true);
    bReadOnly = TRUE;
    if (ref_poDS != nullptr && m_ReportErrors)
      {
      std::cout << "Had to open REF data source read-only."<<std::endl;
      }
    }
  test_poDS = otb::ogr::version_proxy::Open(ref_pszDataSource, bReadOnly);
  if (test_poDS == nullptr && !bReadOnly)
    {
    test_poDS = otb::ogr::version_proxy::Open(ref_pszDataSource, bReadOnly);

    bReadOnly = TRUE;

    if (test_poDS != nullptr && m_ReportErrors)
      {
      std::cout << "Had to open TEST data source read-only."<<std::endl;
      }
    }
  /* -------------------------------------------------------------------- */
  /*      Report failure                                                  */
  /* -------------------------------------------------------------------- */
  if (ref_poDS == nullptr)
    {

    if (m_ReportErrors)
      {
      std::cout << "FAILURE:\n" "Unable to open REF datasource `" << ref_pszDataSource << "' with the following drivers." << std::endl;

      std::vector<std::string> drivers = ogr::version_proxy::GetAvailableDriversAsStringVector();

      for (std::vector<std::string>::const_iterator it = drivers.begin();it!=drivers.end();++it)
        {
        std::cout << "  -> " << *it << std::endl;
        }
      }
    return (1);
    }
  ref_poDriver = ref_poDS->GetDriver();
  CPLAssert(ref_poDriver != NULL);

  if (test_poDS == nullptr)
    {
    if (m_ReportErrors)
      {
      std::cout << "FAILURE:\n""Unable to open TEST datasource `" << test_pszDataSource << "' with the following drivers." << std::endl;

      std::vector<std::string> drivers = ogr::version_proxy::GetAvailableDriversAsStringVector();

      for (std::vector<std::string>::const_iterator it = drivers.begin();it!=drivers.end();++it)
        {
        std::cout << "  -> " << *it << std::endl;
        }
      }
    return (1);
    }
  test_poDriver = test_poDS->GetDriver();
  CPLAssert(test_poDriver != NULL);

  /* -------------------------------------------------------------------- */
  /*      Some information messages.                                      */
  /* -------------------------------------------------------------------- */
  otbCheckStringValue("INFO: using driver", GDALGetDriverShortName(ref_poDriver), GDALGetDriverShortName(test_poDriver), nbdiff, m_ReportErrors);

  // TODO: Improve this check as it will stop as soon as one of the
  // list ends (i.e. it does not guarantee that all files are present)
  std::vector<std::string> refFileList = otb::ogr::version_proxy::GetFileListAsStringVector(ref_poDS);
  std::vector<std::string> testFileList = otb::ogr::version_proxy::GetFileListAsStringVector(test_poDS);

  unsigned int fileId = 0;

  while (fileId < refFileList.size() && fileId < testFileList.size())
    {
    std::string strRefName(refFileList[fileId]);
    std::string strTestName(testFileList[fileId]);
    if (strRefName != strTestName)
      {
      if (!m_ReportErrors)
        {
        otbPrintDiff("WARNING: INFO: Internal data source files were different",
                     strRefName,
                     strTestName);
        }
      }
    ++fileId;
    }

  /* -------------------------------------------------------------------- */
  /*      Process each data source layer.                                 */
  /* -------------------------------------------------------------------- */
  otbCheckValue("GetLayerCount()", ref_poDS->GetLayerCount(), test_poDS->GetLayerCount(), nbdiff, m_ReportErrors);

  for (int iLayer = 0; iLayer < ref_poDS->GetLayerCount(); ++iLayer)
    {
    OGRLayer *ref_poLayer = ref_poDS->GetLayer(iLayer);
    OGRLayer *test_poLayer = test_poDS->GetLayer(iLayer);

    if (ref_poLayer == nullptr)
      {
      if (m_ReportErrors)
        std::cout << "FAILURE: Couldn't fetch advertised layer " << iLayer <<
        " for REF data source" << std::endl;
      return (1);
      }
    if (test_poLayer == nullptr)
      {
      if (m_ReportErrors)
        std::cout << "FAILURE: Couldn't fetch advertised layer " << iLayer <<
        " for REF data source" << std::endl;
      return (1);
      }

    //Check Layer inforamtion
    ogrReportOnLayer(ref_poLayer, nullptr, nullptr, test_poLayer, nullptr, nullptr, nbdiff);

    //If no difference, check the feature
    if (nbdiff == 0)
      {
      OGRFeature *      ref_poFeature = nullptr;
      OGRFeature *      test_poFeature = nullptr;
      std::string       basefilename(test_pszDataSource);
      int               nbFeature(0);
      std::stringstream oss2;
      oss2.str("");
      oss2 << iLayer;

      while ((ref_poFeature = ref_poLayer->GetNextFeature()) != nullptr)
        {
        test_poFeature = test_poLayer->GetNextFeature();
        std::stringstream oss;
        oss.str("");
        oss << nbFeature;

        std::string ref_filename = basefilename + "_temporary_layer_" + oss2.str() + "_feature_" + oss.str()
                                   + "_ref.txt";
        std::string test_filename = basefilename + "_temporary_layer_" + oss2.str() + "_feature_" + oss.str()
                                    + "_test.txt";
        FILE *ref_f(nullptr);
        ref_f = fopen(ref_filename.c_str(), "w");
        if (ref_f == nullptr)
          {
          itkGenericExceptionMacro(<< "Impossible to create ASCII file <" << ref_filename << ">.");
          }
        DumpOGRFeature(ref_f, ref_poFeature);
        OGRFeature::DestroyFeature( ref_poFeature );
        fclose(ref_f);

        FILE *test_f(nullptr);
        test_f = fopen(test_filename.c_str(), "w");
        if (test_f == nullptr)
          {
          itkGenericExceptionMacro(<< "Impossible to create ASCII file <" << test_filename << ">.");
          }
        DumpOGRFeature(test_f, test_poFeature);
        OGRFeature::DestroyFeature( test_poFeature );
        fclose(test_f);

        //Check ASCII comparison
        std::vector<std::string> ignoredLines;
        ignoredLines.clear();
        nbdiff = RegressionTestAsciiFile(test_filename.c_str(), ref_filename.c_str(), toleranceDiffValue,
                                         ignoredLines);

        nbFeature++;
        }
      // If no verbose and an diff was found, exit checking. The full checking will be executed in verbose mode
      if ((!m_ReportErrors) && (nbdiff != 0)) return (1);
      } //if(nbdiff == 0)

    }

  /* -------------------------------------------------------------------- */
  /*      Close down.                                                     */
  /* -------------------------------------------------------------------- */
  GDALClose( ref_poDS );
  GDALClose( test_poDS );

  return (nbdiff != 0) ? 1 : 0;
}

void TestHelper::DumpOGRFeature(FILE* fpOut, OGRFeature* feature, char** papszOptions)
{
  if (fpOut == nullptr) fpOut = stdout;

  if(!feature)
    {
    fprintf(fpOut, "NULL feature encountered\n");
    return;
    }

  fprintf(fpOut, "OGRFeature:%lld\n", (GIntBig) feature->GetFID());

  const char* pszDisplayFields =
    CSLFetchNameValue(papszOptions, "DISPLAY_FIELDS");
  if (pszDisplayFields == nullptr || CSLTestBoolean(pszDisplayFields))
    {
    for (int iField = 0; iField < feature->GetFieldCount(); iField++)
      {
      OGRFieldDefn *poFDefn = feature->GetDefnRef()->GetFieldDefn(iField);

      fprintf(fpOut, "  %s (%s) = ",
              poFDefn->GetNameRef(),
              OGRFieldDefn::GetFieldTypeName(poFDefn->GetType()));

      if (ogr::version_proxy::IsFieldSetAndNotNull(feature, iField)) fprintf(fpOut, "%s\n", feature->GetFieldAsString(iField));
      else fprintf(fpOut, "(null)\n");

      }
    }

  if (feature->GetStyleString() != nullptr)
    {
    const char* pszDisplayStyle =
      CSLFetchNameValue(papszOptions, "DISPLAY_STYLE");
    if (pszDisplayStyle == nullptr || CSLTestBoolean(pszDisplayStyle))
      {
      fprintf(fpOut, "  Style = %s\n", feature->GetStyleString());
      }
    }

  if (feature->GetGeometryRef() != nullptr)
    {
    const char* pszDisplayGeometry =
      CSLFetchNameValue(papszOptions, "DISPLAY_GEOMETRY");
    if (!(pszDisplayGeometry != nullptr && EQUAL(pszDisplayGeometry, "NO")))
      {
      DumpOGRGeometry(fpOut, feature->GetGeometryRef(), "  ", papszOptions);
      }
    }

  fprintf(fpOut, "\n");
}

void TestHelper::DumpOGRGeometry(FILE* fp, OGRGeometry* geometry, const char * pszPrefix, char** papszOptions)
{
  char *pszWkt = nullptr;

  if (pszPrefix == nullptr) pszPrefix = "";

  if (fp == nullptr) fp = stdout;

  const char* pszDisplayGeometry =
    CSLFetchNameValue(papszOptions, "DISPLAY_GEOMETRY");
  if (pszDisplayGeometry != nullptr && EQUAL(pszDisplayGeometry, "SUMMARY"))
    {
    OGRLineString *        poLine;
    OGRPolygon *           poPoly;
    OGRLinearRing *        poRing;
    OGRGeometryCollection *poColl;
    fprintf(fp, "%s%s : ", pszPrefix, geometry->getGeometryName());
    switch (geometry->getGeometryType())
      {
      case wkbUnknown:
      case wkbNone:
        break;
      case wkbPoint:
      case wkbPoint25D:
        break;
      case wkbLineString:
      case wkbLineString25D:
        poLine = (OGRLineString*) geometry;
        fprintf(fp, "%d points\n", poLine->getNumPoints());
        break;
      case wkbPolygon:
      case wkbPolygon25D:
        {
        int ir;
        int nRings;
        poPoly = (OGRPolygon*) geometry;
        poRing = poPoly->getExteriorRing();
        nRings = poPoly->getNumInteriorRings();
        fprintf(fp, "%d points", poRing->getNumPoints());
        if (nRings)
          {
          fprintf(fp, ", %d inner rings (", nRings);
          for (ir = 0; ir < nRings; ir++)
            {
            if (ir) fprintf(fp, ", ");
            fprintf(fp, "%d points",
                    poPoly->getInteriorRing(ir)->getNumPoints());
            }
          fprintf(fp, ")");
          }
        fprintf(fp, "\n");
        break;
        }
      case wkbMultiPoint:
      case wkbMultiPoint25D:
      case wkbMultiLineString:
      case wkbMultiLineString25D:
      case wkbMultiPolygon:
      case wkbMultiPolygon25D:
      case wkbGeometryCollection:
      case wkbGeometryCollection25D:
        {
        int ig;
        poColl = (OGRGeometryCollection*) geometry;
        fprintf(fp, "%d geometries:\n", poColl->getNumGeometries());
        for (ig = 0; ig < poColl->getNumGeometries(); ig++)
          {
          OGRGeometry * poChild = (OGRGeometry*) poColl->getGeometryRef(ig);
          fprintf(fp, "%s", pszPrefix);
          DumpOGRGeometry(fp, poChild, pszPrefix, papszOptions);
          }
        break;
        }
      case wkbLinearRing:
        break;
      default:
        break;
      }
    }
  else if (pszDisplayGeometry == nullptr || CSLTestBoolean(pszDisplayGeometry) ||
           EQUAL(pszDisplayGeometry, "WKT"))
    {
    if (geometry->exportToWkt(&pszWkt) == OGRERR_NONE)
      {
      fprintf(fp, "%s%s\n", pszPrefix, pszWkt);
      CPLFree(pszWkt);
      }
    }
}

// Regression Testing Code

bool TestHelper::isNumber(int i) const
{
  return ((i > 47) && (i < 58));
}

bool TestHelper::isHexaNumber(int i) const
{
  return (((i > 47) && (i < 58)) ||
          ((i > 96) && (i < 103)) ||
          ((i > 64) && (i < 71)));
}

bool TestHelper::isPoint(int i) const
{
  return (i == 46);
}

bool TestHelper::isMinusSign(int i) const
{
  return (i == 45);
}

bool TestHelper::isAlphaNum(int i) const
{
  return isNumber(i) || ((i > 64) && (i < 91)) || ((i > 96) && (i < 123));
}

bool TestHelper::isNumeric(const std::string& str) const
{
  int          nbOfPoints = 0;
  int          nbOfNumbers = 0;
  int          number;
  unsigned int i = 0;
  bool         result = true;

  while ((i < str.size()) && (result == true))
    {
    number = str[i];

    if (isPoint(number)) ++nbOfPoints;
    if (isNumber(number)) ++nbOfNumbers;
    if ((!isNumber(number) && !isPoint(number) &&
         !isMinusSign(number)) || (isMinusSign(number) && (i != 0))) result = false;

    ++i;
    }
  if ((str.size() == 0) || (nbOfPoints > 1) /*||(nbOfNumbers==0)*/) result = false;

  return result;
}

bool TestHelper::isScientificNumeric(const std::string& str) const
{
       if(str.size() == 0)
       {
              return false;
       }
  int number(0);
  number = str[0];
  bool pointDetected(false);
  bool eDetected(false);
  bool signDetected(false);

  // Analyse first character (+, -, 0...9)
  unsigned int cpt(0);
  if ((str[0] != '+') && (str[0] != '-') && (!isNumber(number))) return false;
  if ((str[0] == '+') || (str[0] == '-'))
    {
    ++cpt;
    signDetected = true;
    }

  while (cpt < str.size())
    {
    if (str[cpt] == '.')
      {
      // Exit false if two points or if e already detected
      if ((pointDetected == true) || (eDetected == true)) return false;
      pointDetected = true;
      }
    else if ((str[cpt] == 'e') || (str[cpt] == 'E'))
      {
      // Exit false if two e
      if (eDetected == true) return false;
      eDetected = true;
      }
    else if ((str[cpt] == '-') || (str[cpt] == '+'))
      {
      // Exit false if already sign with no E detected
      if ((signDetected == true) && (eDetected == false)) return false;
      signDetected = true;
      }
    else
      {
      number = str[cpt];
      if (!isNumber(number)) return false;
      }
    ++cpt;
    }

  return true;
}

bool TestHelper::isToBeIgnoredForAnyComparison(const std::string& str) const
{
  if (str.find("RTTI") != std::string::npos) return true;
  if (str.find("Modified Time") != std::string::npos) return true;
  if (str.find("PipelineMTime:") != std::string::npos) return true;
  return false;
}

bool TestHelper::isHexaPointerAddress(const std::string& str) const
{
  unsigned int size(0);
  bool         result(false);
  unsigned int start(0);

  if( str.size() < 2 )
  {
       return false;
  }
  //If (0xadresss)
  if ((str[0] == 40) && (str[str.size() - 1] == 41) && (str[1] == 48) && (str[2] == 120))
    {
    result = true;
    start = 3;
    size = str.size() - 1;
    }
  //If 0xadresss
  else if ((str[0] == 48) && (str[1] == 120))
    {
    result = true;
    start = 2;
    size = str.size();
    }
  // if 01adress
  else if ((str[0] == 48) && (str[1] == 49))
    {
    result = true;
    start = 2;
    size = str.size();
    }

  unsigned int i(start);
  while (result && (i < size))
    {
    result = result && isHexaNumber(str[i]);
    ++i;
    }
  return result;
}

bool TestHelper::isHexaPointerAddress(const std::string& str, size_t pos, size_t size) const
{
  if (size < 3)
    {
    return false;
    }

  // pointer address has to begin with '0x'
  // it may also start with '00' with a size of 16 (64bit address)
  // or start with '0' with a size of 8 (32bit address)
  // Note: the last pattern seems a bit weak but cases like '00B5FA18' or
  // '01C46D80' both are valid hexadecimal addresses on windows
  if (str[pos] != 48)
    {
    return false;
    }
  if (!(str[pos+1] == 120 ||
        (str[pos+1] == 48 && size == 16) ||
        (size == 8)))
    {
    return false;
    }

  // check all other characters are in [A-Fa-f0-9]
  for (unsigned int i=2 ; i<size ; i++)
    {
    if (!isHexaNumber(str[pos+i]))
      {
      return false;
      }
    }
  return true;
}

void TestHelper::AddWhiteSpace(const std::string& strIn, std::string &strOut) const
{
  std::string strLine = strIn;

  std::vector<std::string> keys;
  keys.push_back("[");
  keys.push_back("]");
  keys.push_back("(");
  keys.push_back(")");
  keys.push_back(",");
  keys.push_back("=");
  keys.push_back(":");
  keys.push_back(";");
  keys.push_back("</");
  keys.push_back(">");

  std::vector<std::string> keysOut;
  keysOut.push_back("[ ");
  keysOut.push_back(" ]");
  keysOut.push_back("( ");
  keysOut.push_back(" )");
  keysOut.push_back(" , ");
  keysOut.push_back(" = ");
  keysOut.push_back(" : ");
  keysOut.push_back(" , ");
  keysOut.push_back(" </");
  keysOut.push_back("> ");


  for (unsigned int it = 0; it < keys.size(); ++it)
    {
    size_t found;
    found=strLine.find(keys[it]);
    while (found!=std::string::npos)
      {
        strLine.replace(found, 1, keysOut[it]);
        found=strLine.find(keys[it], found + keysOut[it].size());
      }
    }

  strOut = strLine;
}

bool TestHelper::CompareLines(const std::string& strfileref,
                              const std::string& strfiletest,
                              int& nbdiff,
                              std::ofstream& fluxfilediff,
                              int& numLine,
                              std::vector<std::string>& listStrDiffLineFileRef,
                              std::vector<std::string>& listStrDiffLineFileTest,
                              double epsilon) const
{
  // add white spaces
  std::string strLineRef;
  std::string strLineTest;
  AddWhiteSpace(strfileref, strLineRef);
  AddWhiteSpace(strfiletest, strLineTest);
  //otbMsgDevMacro(<<"Comparing (after replace) : " << strLineRef << " vs " << strLineTest);

  std::stringstream buffstreamRef, buffstreamTest;
  buffstreamRef << strLineRef;
  buffstreamTest << strLineTest;
  //Number of differences in the current line
  bool differenceFoundInCurrentLine = false;

  enum TypeEtat
    {
    ETAT_NUM, ETAT_CHAR
    };

  TypeEtat etatPrec(ETAT_NUM), etatCour(ETAT_NUM);

  while (buffstreamRef.peek() != EOF)
    {
    std::string strRef = "";
    std::string strTest = "";

    std::string strNumRef = "";
    std::string strCharRef = "";
    std::string strNumTest = "";
    std::string strCharTest = "";

    buffstreamRef >> strRef;
    buffstreamTest >> strTest;
    //otbMsgDevMacro(<< "sub comparison of the line, strRef: " << strRef << " || strTest: " << strTest);

    std::string charTmpRef = "";
    std::string charTmpTest = "";

    if (!isHexaPointerAddress(strRef))
      {
      //Analyse if strRef contains scientific value (ex: "-142.124e-012")
      if (isScientificNumeric(strRef))
        {
        if (!isScientificNumeric(strTest))
          {
          if (m_ReportErrors)
            {
            fluxfilediff << "Diff at line " << numLine << " : compare numeric value with no numeric value ("
                         << strRef << strRef << " != " << strTest << ")" << std::endl;
            differenceFoundInCurrentLine = true;
            }
          nbdiff++;

          }
        else
          {
          float vRef = atof(strRef.c_str());
          float vTest = atof(strTest.c_str());
          float vNorm = (std::abs(vRef) + std::abs(vTest))/2;
          //otbMsgDevMacro(<< "numerical comparison: " <<vRef << " vs " <<vTest << " -> "
          //               << "vNorm= " << vNorm << ", " << std::abs(vRef-vTest) << " > "<< epsilon * vNorm
          //               << "? -> " << (std::abs(vRef-vTest) > epsilon * vNorm ));
          if ((vNorm > m_EpsilonBoundaryChecking) //make sure that either the test of the ref are non 0
              && (std::abs(vRef-vTest) > epsilon * vNorm)) //epsilon as relative error
            {
            if (m_ReportErrors)
              {
              fluxfilediff << "Diff at line " << numLine << " : std::abs ( (" << strRef << ") - (" << strTest
                  << ") ) > " << epsilon << std::endl;
              differenceFoundInCurrentLine = true;
              }
            nbdiff++;
            }
          }
        }
      else
        {
        unsigned int i = 0;
        while (i < strRef.size())
          {
          charTmpRef = strRef[i];

          if (i < strTest.size())
            {
            charTmpTest = strTest[i];
            }

          //otbMsgDevMacro(<< "characterRef: " << charTmpRef << " || characterTest: " << charTmpTest );

          if (isNumeric(charTmpRef)) etatCour = ETAT_NUM;
          else etatCour = ETAT_CHAR;

          // "reference" state initialization.
          if (i == 0) etatPrec = etatCour;

          // Case where there's a number after characteres.
          if ((etatCour == ETAT_NUM) && (etatPrec == ETAT_CHAR))
            {
            if (strCharRef != strCharTest)
              {
              if (m_ReportErrors)
                {
                fluxfilediff << "Diff at line " << numLine << " : " << strCharRef << " != " << strCharTest
                             << std::endl;
                differenceFoundInCurrentLine = true;
                }
              nbdiff++;
              }

            strCharRef = "";
            strCharTest = "";
            strNumRef = charTmpRef;
            strNumTest = charTmpTest;
            }
          // Case where there's a character after numbers.
          else if ((etatCour == ETAT_CHAR) && (etatPrec == ETAT_NUM))
            {

            if ((strNumRef != strNumTest) && (std::abs(atof(strNumRef.c_str())) > m_EpsilonBoundaryChecking)
                && (std::abs(atof(strNumRef.c_str()) - atof(strNumTest.c_str()))
                    > epsilon * std::abs(atof(strNumRef.c_str())))) //epsilon as relative error
              {
              if (m_ReportErrors)
                {
                fluxfilediff << "Diff at line " << numLine << " : std::abs ( (" << strNumRef << ") - ("
                             << strNumTest << ") ) > " << epsilon << std::endl;
                differenceFoundInCurrentLine = true;
                }
              nbdiff++;
              }

            strNumRef = "";
            strNumTest = "";
            strCharRef = charTmpRef;
            strCharTest = charTmpTest;
            }
          else if (etatCour == etatPrec)
            {
            if (etatCour == ETAT_CHAR)
              {
              strCharRef += charTmpRef;
              strCharTest += charTmpTest;
              }
            else
              {
              strNumRef += charTmpRef;
              strNumTest += charTmpTest;
              }
            }

          etatPrec = etatCour;
          ++i;
          }

        // test last part
        if (etatCour == ETAT_CHAR)
          {
          if (strCharRef != strCharTest)
            {
            if (m_ReportErrors)
              {
              fluxfilediff << "Diff at line " << numLine << " : " << strCharRef << " != " << strCharTest << std::endl;
              differenceFoundInCurrentLine = true;
              }
            nbdiff++;
            }
          }
        else
          {
          if ((strNumRef != strNumTest) && (std::abs(atof(strNumRef.c_str())) > m_EpsilonBoundaryChecking)
              && (std::abs(atof(strNumRef.c_str()) - atof(strNumTest.c_str()))
                  > epsilon * std::abs(atof(strNumRef.c_str()))))    //epsilon as relative error
            {
            if (m_ReportErrors)
              {
              fluxfilediff << "Diff at line " << numLine << " : std::abs( (" << strNumRef << ") - (" << strNumTest
                            << ") ) > " << epsilon << std::endl;
              differenceFoundInCurrentLine = true;
              }
            nbdiff++;
            }
          }
        } // else
      } // if(!isHexaPointerAddress(strRef))
    else
      {
      if (m_ReportErrors)
        {
        fluxfilediff << "Pointer address found at line " << numLine << " : " << strRef
                     << " -> comparison skipped." << std::endl;
        }
      }
    }

  numLine++;
  //Store alls differences lines
  if (differenceFoundInCurrentLine && m_ReportErrors)
    {
    listStrDiffLineFileRef.push_back(strfileref);
    listStrDiffLineFileTest.push_back(strfiletest);
    }

  return differenceFoundInCurrentLine;
}

std::string TestHelper::VectorToString(const otb::MetaDataKey::VectorType& vector) const
{
  std::stringstream oss;
  oss.str("");
  otb::MetaDataKey::VectorType::const_iterator it = vector.begin();
  oss << "[";
  while (it != vector.end())
    {
    oss << (*it);
    ++it;
    if (it == vector.end())
      {
      oss << "]";
      break;
      }
    else
      {
      oss << ", ";
      }
    }
  return oss.str();
}

/************************************************************************/
/*                           ReportOnLayer()                            */
/************************************************************************/

void TestHelper::ogrReportOnLayer(OGRLayer * ref_poLayer,
                                  const char *ref_pszWHERE,
                                  OGRGeometry *ref_poSpatialFilter,
                                  OGRLayer * test_poLayer,
                                  const char *test_pszWHERE,
                                  OGRGeometry *test_poSpatialFilter,
                                  int& nbdiff) const

{
  OGRFeatureDefn *ref_poDefn = ref_poLayer->GetLayerDefn();
  OGRFeatureDefn *test_poDefn = test_poLayer->GetLayerDefn();

  /* -------------------------------------------------------------------- */
  /*      Set filters if provided.                                        */
  /* -------------------------------------------------------------------- */
  otbCheckStringValue("pszWHERE", ref_pszWHERE, test_pszWHERE, nbdiff, m_ReportErrors);

  if (ref_pszWHERE != nullptr) ref_poLayer->SetAttributeFilter(ref_pszWHERE);

  if (ref_poSpatialFilter != nullptr) ref_poLayer->SetSpatialFilter(ref_poSpatialFilter);

  if (test_pszWHERE != nullptr) test_poLayer->SetAttributeFilter(test_pszWHERE);

  if (test_poSpatialFilter != nullptr) test_poLayer->SetSpatialFilter(test_poSpatialFilter);

  /* -------------------------------------------------------------------- */
  /*      Report various overall information.                             */
  /* -------------------------------------------------------------------- */
  printf("\n");

  otbCheckStringValue("Geometry", OGRGeometryTypeToName(ref_poDefn->GetGeomType()),
                      OGRGeometryTypeToName(test_poDefn->GetGeomType()), nbdiff, m_ReportErrors);

  otbCheckValue("Feature Count", ref_poLayer->GetFeatureCount(),
                test_poLayer->GetFeatureCount(), nbdiff, m_ReportErrors);

  OGREnvelope ref_oExt;
  OGREnvelope test_oExt;

  otbCheckValue("GetExtent", ref_poLayer->GetExtent(&ref_oExt, TRUE), test_poLayer->GetExtent(&test_oExt,
                                                                                              TRUE), nbdiff,
                m_ReportErrors);

  if (ref_poLayer->GetExtent(&ref_oExt, TRUE) == OGRERR_NONE)
    {
    otbCheckValue("Extent: MinX", ref_oExt.MinX, test_oExt.MinX, nbdiff, m_ReportErrors);
    otbCheckValue("Extent: MinY", ref_oExt.MinY, test_oExt.MinY, nbdiff, m_ReportErrors);
    otbCheckValue("Extent: MaxX", ref_oExt.MaxX, test_oExt.MaxX, nbdiff, m_ReportErrors);
    otbCheckValue("Extent: MaxY", ref_oExt.MaxY, test_oExt.MaxY, nbdiff, m_ReportErrors);
    }

  char *ref_pszWKT;
  char *test_pszWKT;

  if (ref_poLayer->GetSpatialRef() == nullptr) ref_pszWKT = CPLStrdup("(unknown)");
  else
    {
    ref_poLayer->GetSpatialRef()->exportToPrettyWkt(&ref_pszWKT);
    }
  if (test_poLayer->GetSpatialRef() == nullptr) test_pszWKT = CPLStrdup("(unknown)");
  else
    {
    test_poLayer->GetSpatialRef()->exportToPrettyWkt(&test_pszWKT);
    }

  otbCheckStringValue("Layer SRS WKT", ref_pszWKT, test_pszWKT, nbdiff, m_ReportErrors);

  CPLFree(ref_pszWKT);
  CPLFree(test_pszWKT);

  otbCheckStringValue("FID Column", ref_poLayer->GetFIDColumn(), test_poLayer->GetFIDColumn(), nbdiff, m_ReportErrors);
  otbCheckStringValue("Geometry Column", ref_poLayer->GetGeometryColumn(),
                      test_poLayer->GetGeometryColumn(), nbdiff, m_ReportErrors);
  otbCheckValue("GetFieldCount", ref_poDefn->GetFieldCount(), test_poDefn->GetFieldCount(), nbdiff, m_ReportErrors);
  if (ref_poDefn->GetFieldCount() == test_poDefn->GetFieldCount())
    {
    for (int iAttr = 0; iAttr < ref_poDefn->GetFieldCount(); ++iAttr)
      {
      OGRFieldDefn *ref_poField = ref_poDefn->GetFieldDefn(iAttr);
      OGRFieldDefn *test_poField = test_poDefn->GetFieldDefn(iAttr);

      otbCheckStringValue("Field GetName", ref_poField->GetNameRef(),
                          test_poField->GetNameRef(), nbdiff, m_ReportErrors);
      otbCheckStringValue("Field GetFieldTypeName", ref_poField->GetFieldTypeName(
                            ref_poField->GetType()), test_poField->GetFieldTypeName(
                            test_poField->GetType()), nbdiff, m_ReportErrors);
      otbCheckValue("Field GetWidth", ref_poField->GetWidth(), test_poField->GetWidth(), nbdiff, m_ReportErrors);
      otbCheckValue("Field GetPrecision", ref_poField->GetPrecision(),
                    test_poField->GetPrecision(), nbdiff, m_ReportErrors);
      }
    }

  /* -------------------------------------------------------------------- */
  /*      Read, and dump features.                                        */
  /* -------------------------------------------------------------------- */

  //Check the feature contains only if no differences found

}

}
