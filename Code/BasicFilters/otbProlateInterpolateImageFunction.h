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
#ifndef __otbProlateInterpolateImageFunction_h
#define __otbProlateInterpolateImageFunction_h

#include "otbGenericInterpolateImageFunction.h"
#include "itkSize.h"
#include "itkNumericTraits.h"
#include "vnl/vnl_math.h"
#include "vnl/algo/vnl_fft_1d.h"
namespace otb
{

namespace Function
{

/** \class ProlateFunction
 *
 * \brief Prolate function give the value of the prolate profile following
 * one of the 2 image dimension.
 *
 */
template<class TInput, class TOutput>
class ProlateFunction
{
public:
  ProlateFunction() : m_Radius(1)
  {
  const double PROLATE_COEFFICIENTS[] = {
    0.00125,      0.00124999,   0.00124998,   0.00124997,   0.00124995,   0.00124992,  0.00124989,   0.00124985,
    0.0012498,    0.00124975,   0.00124969,   0.00124962,
    0.00124955,   0.00124948,   0.00124939,   0.0012493,    0.00124921,   0.00124911,  0.001249,     0.00124888,
    0.00124876,   0.00124864,   0.0012485,    0.00124837,
    0.00124822,   0.00124807,   0.00124791,   0.00124775,   0.00124758,   0.0012474,   0.00124722,   0.00124703,
    0.00124684,   0.00124664,   0.00124643,   0.00124622,
    0.001246,     0.00124578,   0.00124555,   0.00124531,   0.00124507,   0.00124482,  0.00124456,   0.0012443,
    0.00124403,   0.00124376,   0.00124348,   0.00124319,
    0.0012429,    0.0012426,    0.0012423,    0.00124199,   0.00124167,   0.00124135,  0.00124102,   0.00124069,
    0.00124035,   0.00124,      0.00123965,   0.00123929,
    0.00123892,   0.00123855,   0.00123817,   0.00123779,   0.0012374,    0.001237,    0.0012366,    0.00123619,
    0.00123578,   0.00123536,   0.00123494,   0.0012345,
    0.00123407,   0.00123362,   0.00123317,   0.00123272,   0.00123225,   0.00123179,  0.00123131,   0.00123083,
    0.00123035,   0.00122985,   0.00122936,   0.00122885,
    0.00122834,   0.00122783,   0.0012273,    0.00122678,   0.00122624,   0.0012257,   0.00122516,   0.0012246,
    0.00122405,   0.00122348,   0.00122291,   0.00122234,
    0.00122175,   0.00122117,   0.00122057,   0.00121997,   0.00121937,   0.00121876,  0.00121814,   0.00121752,
    0.00121689,   0.00121625,   0.00121561,   0.00121497,
    0.00121431,   0.00121366,   0.00121299,   0.00121232,   0.00121165,   0.00121096,  0.00121028,   0.00120958,
    0.00120888,   0.00120818,   0.00120747,   0.00120675,
    0.00120603,   0.0012053,    0.00120457,   0.00120383,   0.00120308,   0.00120233,  0.00120157,   0.00120081,
    0.00120004,   0.00119927,   0.00119849,   0.0011977,
    0.00119691,   0.00119611,   0.00119531,   0.0011945,    0.00119368,   0.00119286,  0.00119204,   0.00119121,
    0.00119037,   0.00118953,   0.00118868,   0.00118782,
    0.00118696,   0.0011861,    0.00118523,   0.00118435,   0.00118347,   0.00118258,  0.00118169,   0.00118079,
    0.00117988,   0.00117897,   0.00117805,   0.00117713,
    0.00117621,   0.00117527,   0.00117433,   0.00117339,   0.00117244,   0.00117149,  0.00117053,   0.00116956,
    0.00116859,   0.00116761,   0.00116663,   0.00116564,
    0.00116465,   0.00116365,   0.00116265,   0.00116164,   0.00116062,   0.0011596,   0.00115858,   0.00115754,
    0.00115651,   0.00115547,   0.00115442,   0.00115337,
    0.00115231,   0.00115124,   0.00115018,   0.0011491,    0.00114802,   0.00114694,  0.00114585,   0.00114475,
    0.00114365,   0.00114255,   0.00114143,   0.00114032,
    0.0011392,    0.00113807,   0.00113694,   0.0011358,    0.00113466,   0.00113351,  0.00113236,   0.0011312,
    0.00113003,   0.00112887,   0.00112769,   0.00112651,
    0.00112533,   0.00112414,   0.00112295,   0.00112175,   0.00112054,   0.00111933,  0.00111812,   0.0011169,
    0.00111567,   0.00111444,   0.00111321,   0.00111197,
    0.00111072,   0.00110947,   0.00110822,   0.00110696,   0.00110569,   0.00110442,  0.00110315,   0.00110187,
    0.00110059,   0.0010993,    0.001098,     0.0010967,
    0.0010954,    0.00109409,   0.00109277,   0.00109146,   0.00109013,   0.0010888,   0.00108747,   0.00108613,
    0.00108479,   0.00108344,   0.00108209,   0.00108073,
    0.00107937,   0.001078,     0.00107663,   0.00107526,   0.00107387,   0.00107249,  0.0010711,    0.0010697,
    0.0010683,    0.0010669,    0.00106549,   0.00106408,
    0.00106266,   0.00106124,   0.00105981,   0.00105838,   0.00105694,   0.0010555,   0.00105405,   0.0010526,
    0.00105115,   0.00104969,   0.00104823,   0.00104676,
    0.00104528,   0.00104381,   0.00104233,   0.00104084,   0.00103935,   0.00103785,  0.00103636,   0.00103485,
    0.00103334,   0.00103183,   0.00103031,   0.00102879,
    0.00102727,   0.00102574,   0.0010242,    0.00102267,   0.00102112,   0.00101958,  0.00101803,   0.00101647,
    0.00101491,   0.00101335,   0.00101178,   0.00101021,
    0.00100863,   0.00100705,   0.00100547,   0.00100388,   0.00100228,   0.00100069,  0.000999085,  0.00099748,
    0.000995871,  0.000994258,  0.00099264,   0.000991019,
    0.000989394,  0.000987764,  0.000986131,  0.000984494,  0.000982852,  0.000981207, 0.000979558,  0.000977905,
    0.000976248,  0.000974587,  0.000972922,  0.000971254,
    0.000969581,  0.000967905,  0.000966225,  0.000964541,  0.000962853,  0.000961162, 0.000959466,  0.000957767,
    0.000956065,  0.000954358,  0.000952648,  0.000950934,
    0.000949217,  0.000947495,  0.00094577,   0.000944042,  0.00094231,   0.000940574, 0.000938834,  0.000937092,
    0.000935345,  0.000933595,  0.000931841,  0.000930084,
    0.000928323,  0.000926559,  0.000924792,  0.000923021,  0.000921246,  0.000919468, 0.000917686,  0.000915902,
    0.000914113,  0.000912322,  0.000910527,  0.000908728,
    0.000906927,  0.000905122,  0.000903314,  0.000901502,  0.000899687,  0.000897869, 0.000896048,  0.000894223,
    0.000892395,  0.000890564,  0.00088873,   0.000886892,
    0.000885052,  0.000883208,  0.000881361,  0.000879511,  0.000877658,  0.000875802, 0.000873943,  0.000872081,
    0.000870216,  0.000868347,  0.000866476,  0.000864602,
    0.000862724,  0.000860844,  0.000858961,  0.000857075,  0.000855186,  0.000853294, 0.000851399,  0.000849502,
    0.000847601,  0.000845698,  0.000843791,  0.000841882,
    0.000839971,  0.000838056,  0.000836139,  0.000834219,  0.000832296,  0.000830371, 0.000828442,  0.000826511,
    0.000824578,  0.000822642,  0.000820703,  0.000818761,
    0.000816817,  0.000814871,  0.000812922,  0.00081097,   0.000809016,  0.000807059, 0.000805099,  0.000803138,
    0.000801173,  0.000799207,  0.000797237,  0.000795266,
    0.000793292,  0.000791315,  0.000789337,  0.000787355,  0.000785372,  0.000783386, 0.000781398,  0.000779407,
    0.000777415,  0.00077542,   0.000773422,  0.000771423,
    0.000769421,  0.000767417,  0.000765411,  0.000763403,  0.000761393,  0.00075938,  0.000757365,  0.000755349,
    0.00075333,   0.000751309,  0.000749286,  0.000747261,
    0.000745234,  0.000743205,  0.000741173,  0.00073914,   0.000737105,  0.000735069, 0.00073303,   0.000730989,
    0.000728946,  0.000726902,  0.000724855,  0.000722807,
    0.000720757,  0.000718705,  0.000716652,  0.000714596,  0.000712539,  0.00071048,  0.000708419,  0.000706357,
    0.000704293,  0.000702227,  0.00070016,   0.000698091,
    0.00069602,   0.000693948,  0.000691874,  0.000689798,  0.000687721,  0.000685643, 0.000683563,  0.000681481,
    0.000679398,  0.000677313,  0.000675227,  0.00067314,
    0.000671051,  0.000668961,  0.000666869,  0.000664776,  0.000662681,  0.000660586, 0.000658489,  0.00065639,
    0.00065429,   0.000652189,  0.000650087,  0.000647984,
    0.000645879,  0.000643773,  0.000641666,  0.000639558,  0.000637448,  0.000635337, 0.000633226,  0.000631113,
    0.000628999,  0.000626884,  0.000624768,  0.000622651,
    0.000620533,  0.000618414,  0.000616293,  0.000614172,  0.00061205,   0.000609927, 0.000607803,  0.000605679,
    0.000603553,  0.000601426,  0.000599299,  0.000597171,
    0.000595042,  0.000592912,  0.000590781,  0.000588649,  0.000586517,  0.000584384, 0.000582251,  0.000580116,
    0.000577981,  0.000575845,  0.000573709,  0.000571572,
    0.000569434,  0.000567296,  0.000565157,  0.000563018,  0.000560878,  0.000558738, 0.000556597,  0.000554455,
    0.000552313,  0.000550171,  0.000548028,  0.000545884,
    0.000543741,  0.000541596,  0.000539452,  0.000537307,  0.000535162,  0.000533016, 0.00053087,   0.000528724,
    0.000526578,  0.000524431,  0.000522284,  0.000520137,
    0.000517989,  0.000515842,  0.000513694,  0.000511546,  0.000509398,  0.000507249, 0.000505101,  0.000502953,
    0.000500804,  0.000498655,  0.000496507,  0.000494358,
    0.000492209,  0.000490061,  0.000487912,  0.000485763,  0.000483615,  0.000481466, 0.000479318,  0.00047717,
    0.000475021,  0.000472873,  0.000470725,  0.000468578,
    0.00046643,   0.000464283,  0.000462136,  0.000459989,  0.000457842,  0.000455696, 0.00045355,   0.000451404,
    0.000449259,  0.000447114,  0.000444969,  0.000442825,
    0.000440681,  0.000438537,  0.000436394,  0.000434251,  0.000432109,  0.000429967, 0.000427826,  0.000425685,
    0.000423545,  0.000421405,  0.000419266,  0.000417127,
    0.000414989,  0.000412852,  0.000410715,  0.000408579,  0.000406443,  0.000404308, 0.000402174,  0.000400041,
    0.000397908,  0.000395776,  0.000393645,  0.000391514,
    0.000389385,  0.000387256,  0.000385128,  0.000383,     0.000380874,  0.000378748, 0.000376624,  0.0003745,
    0.000372377,  0.000370255,  0.000368134,  0.000366014,
    0.000363895,  0.000361777,  0.00035966,   0.000357544,  0.000355429,  0.000353315, 0.000351202,  0.000349091,
    0.00034698,   0.000344871,  0.000342762,  0.000340655,
    0.000338549,  0.000336444,  0.00033434,   0.000332238,  0.000330137,  0.000328037, 0.000325938,  0.000323841,
    0.000321745,  0.00031965,   0.000317556,  0.000315464,
    0.000313374,  0.000311284,  0.000309196,  0.00030711,   0.000305025,  0.000302941, 0.000300859,  0.000298778,
    0.000296699,  0.000294621,  0.000292544,  0.00029047,
    0.000288397,  0.000286325,  0.000284255,  0.000282186,  0.00028012,   0.000278054, 0.000275991,  0.000273929,
    0.000271869,  0.00026981,   0.000267753,  0.000265698,
    0.000263645,  0.000261593,  0.000259543,  0.000257495,  0.000255449,  0.000253404, 0.000251362,  0.000249321,
    0.000247282,  0.000245245,  0.00024321,   0.000241176,
    0.000239145,  0.000237116,  0.000235088,  0.000233062,  0.000231039,  0.000229017, 0.000226998,  0.00022498,
    0.000222965,  0.000220951,  0.00021894,   0.00021693,
    0.000214923,  0.000212918,  0.000210915,  0.000208914,  0.000206915,  0.000204919, 0.000202924,  0.000200932,
    0.000198942,  0.000196955,  0.000194969,  0.000192986,
    0.000191005,  0.000189026,  0.00018705,   0.000185076,  0.000183104,  0.000181135, 0.000179167,  0.000177203,
    0.00017524,   0.000173281,  0.000171323,  0.000169368,
    0.000167415,  0.000165465,  0.000163517,  0.000161572,  0.000159629,  0.000157689, 0.000155752,  0.000153816,
    0.000151884,  0.000149954,  0.000148026,  0.000146101,
    0.000144179
    };

  const size_t PROLATE_COEFFICIENTS_SIZE = sizeof(PROLATE_COEFFICIENTS) / sizeof(PROLATE_COEFFICIENTS[0]);

  m_OriginalProfile.reserve(PROLATE_COEFFICIENTS_SIZE);
  m_OriginalProfile.assign(PROLATE_COEFFICIENTS, PROLATE_COEFFICIENTS+PROLATE_COEFFICIENTS_SIZE);

  }

  ~ProlateFunction() {}

  typedef std::vector<double> VectorType;
  // Accessors definitions
  void SetRadius(unsigned int rad)
  {
    m_Radius = rad;
  }
  unsigned int GetRadius() const
  {
    return m_Radius;
  }
  unsigned int GetOriginalProfileSize() const
  {
    return m_OriginalProfile.size();
  }
  VectorType GetOriginalProfile() const
  {
    return m_OriginalProfile;
  }

  double ComputeEnergy(double resampleRatio) const;

  inline TOutput operator ()(const TInput& A) const
  {
    const size_t originalProfileSize = m_OriginalProfile.size();
    TOutput val = itk::NumericTraits<TOutput>::Zero;
    if (A != itk::NumericTraits<TInput>::Zero && vcl_abs(A) != static_cast<TInput>(m_Radius) && m_Radius != 0)
      {
      double ival = static_cast<double>(originalProfileSize) * static_cast<double>(vcl_abs(A)) /
                    static_cast<double>(m_Radius);
      double ivalFloor = vcl_floor(ival);
      double left = ival - ivalFloor;

      if (ivalFloor < originalProfileSize - 1)
        {
        val = left * m_OriginalProfile[static_cast<unsigned int>(ivalFloor)] +
              (1 - left) * m_OriginalProfile[static_cast<unsigned int>(ivalFloor) + 1];
        }
      else
        {
        itkGenericExceptionMacro(<< "Out of Profile limits (" << ivalFloor << " -1 > 721)");
        }
      }
    else
      {
      if (A == itk::NumericTraits<TInput>::Zero || m_Radius == 0)
        {
        val = m_OriginalProfile[0];
        }
      else
        {
        if (vcl_abs(A) == static_cast<TInput>(m_Radius))
          {
          val = m_OriginalProfile[originalProfileSize - 1];
          }
        }
      }
    return val;
  }

private:
  /** Useless, only to be compatible with the GenericInterpolateImage. */
  unsigned int m_Radius;
  /** Original prolate profile */
  VectorType m_OriginalProfile;

};

} //namespace Function

/** \class ProlateInterpolateImageFunction
 * \brief Prolate interpolation of an otb::image.
 *
 * ProlateInterpolateImageFunction interpolates image intensity according to a
 * resampling profil.
 *
 * The Initialize() method need to be call to create the filter.
 *
 * \ingroup ImageFunctions ImageInterpolators
 */
template<class TInputImage, class TBoundaryCondition = itk::ConstantBoundaryCondition<TInputImage>, class TCoordRep =
      double, class TInputInterpolator = double, class TOutputInterpolator = double>
class ITK_EXPORT ProlateInterpolateImageFunction :
  public GenericInterpolateImageFunction<TInputImage,
      typename Function::ProlateFunction<TInputInterpolator, TOutputInterpolator>,
      TBoundaryCondition,
      TCoordRep>
{
public:
  /** Standard class typedefs. */
  typedef ProlateInterpolateImageFunction Self;
  typedef GenericInterpolateImageFunction<TInputImage,
      Function::ProlateFunction<TInputInterpolator, TOutputInterpolator>,
      TBoundaryCondition,
      TCoordRep>                                                    Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ProlateInterpolateImageFunction, GenericInterpolateImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Input and output images typedef definition. */
  typedef typename Superclass::InputImageType InputImageType;
  typedef typename Superclass::OutputType     OutputType;
  typedef TInputInterpolator                  InputInterpolatorType;
  typedef TOutputInterpolator                 OutputInterpolatorType;

  /** Dimension underlying input image. */
  itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

  /** Superclass typedef inheritance. */
  typedef typename Superclass::IndexType                                           IndexType;
  typedef typename Superclass::SizeType                                            SizeType;
  typedef typename Superclass::RealType                                            RealType;
  typedef Function::ProlateFunction<InputInterpolatorType, OutputInterpolatorType> FunctionType;
  typedef typename Superclass::IteratorType                                        IteratorType;
  typedef typename Superclass::ContinuousIndexType                                 ContinuousIndexType;
  typedef typename std::vector<double>                                             VectorType;

  unsigned int GetOriginalProfileSize() const
  {
    return this->GetFunction().GetOriginalProfileSize;
  }
  VectorType GetOriginalProfile() const
  {
    return this->GetFunction().GetOriginalProfile();
  }

protected:
  ProlateInterpolateImageFunction();
  ~ProlateInterpolateImageFunction();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ProlateInterpolateImageFunction(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbProlateInterpolateImageFunction.txx"
#endif

#endif
