/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbTestHelper_h
#define __otbTestHelper_h

#include <map>
#include <string>
#include <vector>
#include "itkObject.h"
#include "itkObjectFactory.h"

#include "otbMetaDataKey.h"

class OGRFeature;
class OGRGeometry;
class OGRLayer;

namespace otb
{
/**
 * \class TestHelper
 * \brief Helper class to perform the baseline comparisons during the tests
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
  {}

  ~TestHelper(){}

  int RegressionTestAllImages(const StringList& baselineFilenamesImage,
                              const StringList& testFilenamesImage);

  int RegressionTestAllMetaData(const StringList& baselineFilenamesMetaData,
                                const StringList& testFilenamesMetaData);

  int RegressionTestAllAscii(const StringList& baselineFilenamesAscii,
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

  int RegressionTestMetaData(const char *testImageFilename,
                             const char *baselineImageFilename,
                             const double toleranceDiffPixelImage) const;

  bool isNumber(int i) const;
  bool isHexaNumber(int i) const;
  bool isPoint(int i) const;
  bool isMinusSign(int i) const;
  bool isNumeric(const std::string& str) const;
  bool isScientificNumeric(const std::string& str) const;
  bool isHexaPointerAddress(const std::string& str) const;
  bool isToBeIgnoredForAnyComparison(const std::string& str) const;
  std::string VectorToString(const otb::MetaDataKey::VectorType& vector) const;
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
                   int& nbdiff) const;

  static void DumpOGRFeature(FILE* fileid, OGRFeature* feature, char** papszOptions = NULL);
  static void DumpOGRGeometry(FILE* fileid, OGRGeometry* geometry, const char * pszPrefix, char** papszOptions = NULL);

  double m_ToleranceDiffValue;
  double m_Epsilon;
  double m_EpsilonBoundaryChecking;
  bool   m_ReportErrors;
  bool   m_IgnoreLineOrder;
  const unsigned int m_MaxArea;

  void AddWhiteSpace(const std::string& strIn, std::string &strOut) const;
};
}

#endif
