/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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


#ifndef otbBandMathXImageFilter_h
#define otbBandMathXImageFilter_h

#include "itkConstNeighborhoodIterator.h"
#include "itkImageToImageFilter.h"
#include "itkArray.h"

#include "otbStreamingStatisticsVectorImageFilter.h"
#include "otbParserX.h"

#include <vector>
#include <string>


namespace otb
{
/** \class BandMathXImageFilter
 * \brief Performs mathematical operations on the input images
 * according to the formula specified by the user.
 *
 * This filter is based on the mathematical parser library muParserX.
 * The built in functions and operators list is available at:
 * http://articles.beltoforion.de/article.php?a=muparserx.
 *
 * In order to use this filter, at least one input image is to be
 * set. An associated variable name can be specified or not by using
 * the corresponding SetNthInput method. For the jth (j=1..T) input image, if
 * no associated variable name has been specified, a default variable
 * name is given by concatenating the prefix "im" with the
 * corresponding input index plus one (for instance, im1 is related to the first input).
 * If the jth input image is multidimensional, then the variable imj represents a vector whose components are related to its bands.
 * In order to access the kth band, the variable observes the following pattern : imjbk.
 *
 * \sa Parser
 *
 * \ingroup Streamed
 * \ingroup Threaded
 *
 * \ingroup OTBMathParserX
 */

template< class TImage >
class ITK_EXPORT BandMathXImageFilter
  : public itk::ImageToImageFilter< TImage , TImage >
{
public:
  /** Standard class typedefs. */
  typedef BandMathXImageFilter< TImage >                 Self;
  typedef itk::ImageToImageFilter< TImage , TImage>             Superclass;
  typedef itk::SmartPointer< Self >                     Pointer;
  typedef itk::SmartPointer< const Self >               ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BandMathXImageFilter, ImageToImageFilter);

  /** Some convenient typedefs. */
  typedef TImage                                      ImageType;
  typedef typename ImageType::ConstPointer           ConstImagePointer;
  typedef typename ImageType::Pointer                ImagePointer;
  typedef typename ImageType::RegionType             ImageRegionType;
  typedef typename itk::ConstNeighborhoodIterator<TImage>::RadiusType RadiusType;
  typedef typename ImageType::PixelType::ValueType   PixelValueType;
  typedef typename ImageType::PixelType              PixelType;
  typedef typename ImageType::IndexType              IndexType;
  typedef typename ImageType::PointType              OrigineType;
  typedef typename ImageType::SpacingType            SpacingType;
  typedef ParserX                                     ParserType;
  typedef typename ParserType::ValueType             ValueType;
  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Typedef for statistic computing. */
  typedef StreamingStatisticsVectorImageFilter<ImageType> StreamingStatisticsVectorImageFilterType;
  typedef typename StreamingStatisticsVectorImageFilterType::Pointer StreamingStatisticsVectorImageFilterPointerType;
  typedef typename StreamingStatisticsVectorImageFilterType::MatrixType MatrixType;

  /** Set the nth filter input with or without a specified associated variable name */
  using Superclass::SetNthInput;
  void SetNthInput( DataObjectPointerArraySizeType idx, const ImageType * image);
  void SetNthInput( DataObjectPointerArraySizeType idx, const ImageType * image, const std::string& varName);

  /** Return a pointer on the nth filter input */
  ImageType * GetNthInput(DataObjectPointerArraySizeType idx);

  /** Set an expression to be parsed */
  void SetManyExpressions(bool flag);

  /** Set an expression to be parsed */
  void SetExpression(const std::string& expression);

  /** Return the nth expression to be parsed */
  std::string GetExpression(int) const;

  /** Set a matrix (or a vector) */
  void SetMatrix(const std::string& name, const std::string& definition);

  /** Set a constant */
  void SetConstant(const std::string& name, double value);

  /** Export constants and expressions to a given filename */
  void ExportContext(const std::string& filename);

  /** Import constants and expressions from a given filename */
  void ImportContext(const std::string& filename);

  /** Return the variable and constant names */
  std::vector<std::string> GetVarNames() const;


protected :
  BandMathXImageFilter();
  ~BandMathXImageFilter() override;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void GenerateOutputInformation() override;
  void GenerateInputRequestedRegion() override;

  void BeforeThreadedGenerateData() override;
  void ThreadedGenerateData(const ImageRegionType& outputRegionForThread, itk::ThreadIdType threadId ) override;
  void AfterThreadedGenerateData() override;

private :

  bool globalStatsDetected() const
  {
    return (m_StatsVarDetected.size()>0);
  }

  typedef struct {
      std::string name;
      ValueType   value;
      int         type;
      int         info[5];
  } adhocStruct;


  BandMathXImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  void AddVariable(adhocStruct&);
  void CheckImageDimensions();
  void PrepareParsers();
  void PrepareParsersGlobStats();
  void OutputsDimensions();

  std::vector<std::string>                  m_Expression;
  std::vector< std::vector<ParserType::Pointer> > m_VParser;
  std::vector< std::vector<adhocStruct> >   m_AImage;
  std::vector< adhocStruct >                m_VVarName;
  std::vector< adhocStruct >                m_VAllowedVarNameAuto;
  std::vector< adhocStruct >                m_VAllowedVarNameAddedByUser;
  std::vector< adhocStruct >                m_VFinalAllowedVarName;   // m_VFinalAllowedVarName = m_VAllowedVarNameAuto + m_VAllowedVarNameAddedByUser
  std::vector< adhocStruct >                m_VNotAllowedVarName;
  std::vector< unsigned int >                        m_outputsDimensions;

  unsigned int                             m_SizeNeighbourhood;

  std::vector< int >                        m_StatsVarDetected; // input image ID for which global statistics have been detected

  std::vector< unsigned int >              m_NeighDetected; // input image ID for which neighbourhood have been detected
  std::vector < RadiusType >                m_NeighExtremaSizes;

  long                                  m_UnderflowCount;
  long                                  m_OverflowCount;
  itk::Array<long>                      m_ThreadUnderflow;
  itk::Array<long>                      m_ThreadOverflow;

  bool                                  m_ManyExpressions;

};

}//end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBandMathXImageFilter.hxx"
#endif

#endif
