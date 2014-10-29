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

#ifndef __otbBandMathImageFilterX_h
#define __otbBandMathImageFilterX_h

#include "itkInPlaceImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkArray.h"

#include "otbParserX.h"


namespace otb
{
/** \class BandMathImageFilterX
 * \brief Performs a mathematical operation on the input images
 * according to the formula specified by the user.
 *
 * This filter is based on the mathematical parser library muParser.
 * The built in functions and operators list is available at:
 * http://muparser.sourceforge.net/mup_features.html#idDef2
 *
 * OTB additional functions:
 * ndvi(r, niri)
 *
 * OTB additional constants:
 * e - log2e - log10e - ln2 - ln10 - pi - euler
 *
 * In order to use this filter, at least one input image is to be
 * set. An associated variable name can be specified or not by using
 * the corresponding SetNthInput method. For the nth input image, if
 * no associated variable name has been spefified, a default variable
 * name is given by concatenating the letter "b" (for band) and the
 * corresponding input index.
 * Next step is to set the expression according to the variable
 * names. For example, in the default case with three input images the
 * following expression is valid :
 * "ndvi(b1, b2)*b3"
 *
 * As an additional functionality, the filter also granted access to
 * indexes information under special virtual bands named idxX, idxY
 * for the images indexes and idxPhyX, idxPhyY for the physical
 * indexes.
 * It allows the user to perform, for example a spatial processing
 * aiming to suppress a determined area :
 * "if(sqrt((idxPhyX-105.3)*(idxPhyX-105.3)+
 *          (idxPhyY-207.1)*(idxPhyY-207.1))>100, b1, 0)"
 * This expression replace the physical zone around the point of
 * physical index (105.3; 207.1) by a black area
 * This functionality assumes that all the band involved have the same
 * spacing and origin.
 *
 *
 * \sa Parser
 *
 * \ingroup Streamed
 * \ingroup Threaded
 */

template< class TImage >
class ITK_EXPORT BandMathImageFilterX
  : public itk::InPlaceImageFilter< TImage >
{
public:
  /** Standard class typedefs. */
  typedef BandMathImageFilterX< TImage >                 Self;
  typedef itk::InPlaceImageFilter< TImage >             Superclass;
  typedef itk::SmartPointer< Self >                     Pointer;
  typedef itk::SmartPointer< const Self >               ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BandMathImageFilterX, InPlaceImageFilter);

  /** Some convenient typedefs. */
  typedef TImage                                  ImageType;
  typedef typename ImageType::ConstPointer        ImagePointer;
  typedef typename ImageType::RegionType          ImageRegionType;
  typedef typename ImageType::PixelType           PixelType;
  typedef typename ImageType::IndexType           IndexType;
  typedef typename ImageType::PointType           OrigineType;
  typedef typename ImageType::SpacingType         SpacingType;
  typedef ParserX                                  ParserType;
  typedef typename ParserType::ValueType          ValueType;

  /** Set the nth filter input with or without a specified associated variable name */
  void SetNthInput( unsigned int idx, const ImageType * image);
  void SetNthInput( unsigned int idx, const ImageType * image, const std::string& varName);

  /** Set the expression to be parsed */
  void SetExpression(const std::string& expression);

  /** Set a matrix (or a vector) */
  void SetMatrix(const std::string& name, const std::string& definition);

  /** Return the expression to be parsed */
  std::string GetExpression(int) const;

  /** Return the nth filter input associated variable name */
  std::vector<std::string>& GetVarNames() const;

  /** Return a pointer on the nth filter input */
  ImageType * GetNthInput(unsigned int idx);

protected :
  BandMathImageFilterX();
  virtual ~BandMathImageFilterX();
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void AllocateOutputs();

  void BeforeThreadedGenerateData();
  void ThreadedGenerateData(const ImageRegionType& outputRegionForThread, itk::ThreadIdType threadId );
  void AfterThreadedGenerateData();

private :

  typedef struct {
      std::string name;
      ValueType   value;
      int         type;
      int         info[5];
  } adhocStruct;

  BandMathImageFilterX(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  void AddVariable(adhocStruct&);
  void PrepareParsers();
  void OutputsDimensions();

  std::vector<std::string>                  m_Expression;
  std::vector<ParserType::Pointer>          m_VParser;
  std::vector< std::vector<adhocStruct> >   m_AImage;
  std::vector< adhocStruct >                m_VVarName;
  std::vector< adhocStruct >                m_VAllowedVarNameAuto;
  std::vector< adhocStruct >                m_VAllowedVarNameAddedByUser;
  std::vector< adhocStruct >                m_VFinalAllowedVarName;   // m_VFinalAllowedVarName = m_VAllowedVarNameAuto + m_VAllowedVarNameAddedByUser
  std::vector< adhocStruct >                m_VNotAllowedVarName;
  unsigned int                             m_NbVar;
  std::vector< int >                        m_outputsDimensions;

  SpacingType                              m_Spacing;
  OrigineType                              m_Origin;

  long                                  m_UnderflowCount;
  long                                  m_OverflowCount;
  itk::Array<long>                      m_ThreadUnderflow;
  itk::Array<long>                      m_ThreadOverflow;
};

}//end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBandMathImageFilterX.txx"
#endif

#endif
