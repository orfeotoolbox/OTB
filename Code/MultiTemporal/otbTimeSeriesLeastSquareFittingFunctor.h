

namespace otb
{
namespace Functor
{
/** \class otbTimeSeriesLeastSquareFittingFunctor \brief: This functor
   *  implements a least squares fitting of a time profile. The time
   *  series as well as the date series are supposed to accept the []
   *  syntax to get the values and the Size() method to get their length.
 *
 */
template <class TSeriesType, class TDateType = TSeriesType>
class TimeSeriesLeastSquareFittingFunctor
{
public:
  /// Constructor
  TimeSeriesLeastSquareFittingFunctor()
  {
  }
  /// Destructor
  virtual ~TimeSeriesLeastSquareFittingFunctor() {}

  inline TSeriesType operator ()(const TSeriesType& series)
  {
    TSeriesType outSeries;
    return outSeries;
  }


private:
  /// 
};
}
}
