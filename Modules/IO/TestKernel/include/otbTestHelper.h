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

#ifndef otbTestHelper_h
#define otbTestHelper_h

#include <map>
#include <string>
#include <vector>
#include "itkObject.h"
#include "itkObjectFactory.h"
#include "otbStringUtils.h"

#include "otbMetaDataKey.h"

class OGRFeature;
class OGRGeometry;
class OGRLayer;

namespace otb
{
/**
 * \class TestHelper
 * \brief Helper class to perform the baseline comparisons during the tests
 *
 * \ingroup OTBTestKernel
 */
class ITK_ABI_EXPORT TestHelper : public itk::Object
{
public:

  /** Standard class typedefs. */
  typedef TestHelper                    Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef itk::Object                   Superclass;

  itkTypeMacro(TestHelper, itk::Object);
  itkNewMacro(Self);

  typedef std::vector<std::string> StringList;
  typedef StringList::const_iterator StringListIt;

  TestHelper() :
    m_ToleranceDiffValue(0),
    m_Epsilon(0),
    m_EpsilonBoundaryChecking(1.0e-30),
    m_ReportErrors(false),
    m_IgnoreLineOrder(false),
    m_MaxArea(1024*1024)
  {
    m_SpecialTokens.push_back(std::pair<std::string,std::string>(
      std::string("Integer"),std::string("Integer64")));
  }

  ~TestHelper() override{}

  int RegressionTestAllImages(const StringList& baselineFilenamesImage,
                              const StringList& testFilenamesImage);

  int RegressionTestAllMetaData(const StringList& baselineFilenamesMetaData,
                                const StringList& testFilenamesMetaData);

  int RegressionTestAllAscii(const StringList& baselineFilenamesAscii,
                             const StringList& testFilenamesAscii,
                             const StringList& ignoredLines);

  int RegressionTestAllDiff(const StringList& baselineFilenamesAscii,
                             const StringList& testFilenamesAscii,
                             const StringList& ignoredLines);

  int RegressionTestAllBinary(const StringList& baselineFilenamesBinary,
                              const StringList& testFilenamesBinary);

  int RegressionTestAllOgr(const StringList& baselineFilenamesOgr,
                           const StringList& testFilenamesOgr);

  itkSetMacro(ReportErrors, bool);
  itkBooleanMacro(ReportErrors);
  itkSetMacro(IgnoreLineOrder, bool);
  itkBooleanMacro(IgnoreLineOrder);

  itkSetMacro(ToleranceDiffValue, double);
  itkSetMacro(Epsilon, double);
  itkSetMacro(EpsilonBoundaryChecking, double);

private:

  std::map<std::string, int> RegressionTestBaselines(char *baselineFilename) const;

  int RegressionTestImage(int cpt, const char *testImageFilename,
                          const char *baselineImageFilename,
                          const double toleranceDiffPixelImage) const;

  int RegressionTestOgrFile(const char *testOgrFilename,
                            const char *baselineOgrFilename,
                            const double toleranceDiffValue) const;

  int RegressionTestBinaryFile(const char * testBinaryFileName,
                               const char * baselineBinaryFileName) const;

  int RegressionTestAsciiFile(const char * testAsciiFileName,
                              const char * baselineAsciiFileName,
                              const double epsilon,
                              std::vector<std::string> ignoredLines) const;

  int RegressionTestDiffFile(const char * testAsciiFileName,
                              const char * baselineAsciiFileName,
                              const double epsilon,
                              std::vector<std::string> ignoredLines) const;

  int RegressionTestMetaData(const char *testImageFilename,
                             const char *baselineImageFilename,
                             const double toleranceDiffPixelImage) const;

  bool isNumber(int i) const;
  bool isHexaNumber(int i) const;
  bool isPoint(int i) const;
  bool isMinusSign(int i) const;
  bool isAlphaNum(int i) const;
  bool isNumeric(const std::string& str) const;
  bool isScientificNumeric(const std::string& str) const;
  bool isHexaPointerAddress(const std::string& str) const;
  bool isHexaPointerAddress(const std::string& str, size_t pos, size_t size) const;
  bool isToBeIgnoredForAnyComparison(const std::string& str) const;
  std::string VectorToString(const otb::MetaDataKey::VectorType& vector) const;
  int TokenizeLine(const std::string &line, StringList &tokens) const;

  static bool IsTokenEmpty(boost::iterator_range<std::string::const_iterator> &token);

  // TODO : maybe merge this function with isToBeIgnoredForAnyComparison
  bool IsLineValid(const std::string& str, const StringList &ignoredLines) const;
  //FIXME parameters have to be cleaned up later (this is the first step of refactoring)
  bool CompareLines(const std::string& strfileref,
                    const std::string& strfiletest,
                    int& nbdiff,
                    std::ofstream& fluxfilediff,
                    int& numLine,
                    std::vector<std::string>& listStrDiffLineFileRef,
                    std::vector<std::string>& listStrDiffLineFileTest,
                    double epsilon) const;

  void
  ogrReportOnLayer(OGRLayer * ref_poLayer, const char *ref_pszWHERE, OGRGeometry *ref_poSpatialFilter,
                   OGRLayer * test_poLayer, const char *test_pszWHERE, OGRGeometry *test_poSpatialFilter,
                   int& nbdiff, double epsilon) const;

  static void DumpOGRFeature(FILE* fileid, OGRFeature* feature, char** papszOptions = nullptr);
  static void DumpOGRGeometry(FILE* fileid, OGRGeometry* geometry, const char * pszPrefix, char** papszOptions = nullptr);

  double m_ToleranceDiffValue;
  double m_Epsilon;
  double m_EpsilonBoundaryChecking;
  bool   m_ReportErrors;
  bool   m_IgnoreLineOrder;
  const unsigned int m_MaxArea;

  void AddWhiteSpace(const std::string& strIn, std::string &strOut) const;

  void CheckValueTolerance(const char *Comment, double ref, double test, int &count, bool report, double epsilon) const;

  std::vector<std::pair<std::string, std::string> > m_SpecialTokens;
};
}

#endif
