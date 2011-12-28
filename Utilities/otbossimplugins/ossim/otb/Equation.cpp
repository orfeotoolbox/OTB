//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include <otb/Equation.h>

namespace ossimplugins
{


const double Equation::Epsilon = 1.e-12;

Equation::Equation():
   _coefficients(NULL),
   _degree(0),
   _nbrSol(0),
   _solutions(NULL)
{
  _order.clear();
}

Equation::~Equation()
{
  if (_coefficients != NULL)
    delete [] _coefficients;
  if(_solutions != NULL)
    delete [] _solutions;
}

Equation::Equation(int degree, std::complex<double>* coefficients):
   _coefficients(NULL),
   _degree(0),
   _nbrSol(0),
   _solutions(NULL)
{
  _order.clear();
  CreateEquation(degree, coefficients);
}

void Equation::CreateEquation(int degree, std::complex<double>* coefficients)
{
  if (_coefficients != NULL)
  {
    delete [] _coefficients;
  }

  if(_order.size() != 0)
    _order.clear();
  if(_solutions != NULL)
    delete [] _solutions;

  _coefficients = new std::complex<double>[degree+1];
  _degree = degree;

  for (int i=0;i<=degree;i++)
  {
    _coefficients[i] = coefficients[i];
  }
}

Equation::Equation(const Equation& rhs)
{
  CreateEquation(rhs._degree, rhs._coefficients);
}

Equation& Equation::operator=(const Equation& rhs)
{
  CreateEquation(rhs._degree, rhs._coefficients);
  return *this;
}

void Equation::ComputeTrueDegree()
{
  double r = 0.0;
  _trueDegree = _degree +1;
  while (r <= Epsilon && _trueDegree > 0)
  {
    _trueDegree = _trueDegree - 1 ;
    r = abs(_coefficients[_trueDegree]);
  }
}

void Equation::Normalisation()
{
  for (int i=0;i<_trueDegree;i++)
  {
    _coefficients[i] = _coefficients[i] / _coefficients[_trueDegree];
  }
  _coefficients[_trueDegree] = std::complex<double>(1.0,0.0);

  int eMax = 0;
  int eMin = 0;
  int e;
  double r;

  /*
   * Normalisation par a power of 10
   */
  for (int i = 0 ; i < _trueDegree ; i++)
  {
    r = abs(_coefficients[i]) ;
    if (r >= Epsilon)
    {
      r = log (r) ;
      r = r / log (10.0) ;
      e = ((int)r) / ((int)(_trueDegree - i)) ;
      if (e > eMax)
        eMax = e ;
      if (e < eMin)
        eMin = e ;
    }
  }

  /*
   * Normalisation for big values
   */
  if (eMax > 0)
  {
    /* Normalisation of the unknown for big values */
    _normalisationType = GreatValues;
    _normalisationCoefficient = pow (10.0, (double)eMax) ;
    r    = 1.0 ;
    for (int i = _trueDegree-1 ; i >= 0 ; i--)
    {
      r    = r * _normalisationCoefficient ;
      _coefficients[i] = _coefficients[i] /  std::complex<double>(r,0.0);
    }
  }
  else if (eMin < 0)
  {
    /* Normalisation of the unknown for small */
    _normalisationType = SmallValues;
    _normalisationCoefficient = pow(10.0,(double)(-eMin)) ;
    r    = 1.0 ;
    for (int i = _trueDegree-1 ; i >= 0 ; i--)
    {
      r    = r * _normalisationCoefficient ;
      _coefficients[i] = _coefficients[i] *  std::complex<double>(r,0.0);
    }
  }
}

void Equation::DeNormalisation()
{
  if (_normalisationType == GreatValues)
  {
    for (int i=0;i<_nbrSol;i++)
    {
      _solutions[i] = _solutions[i] * std::complex<double>(_normalisationCoefficient,0.0);
    }
  }
  else
  {
    for (int i=0;i<_nbrSol;i++)
    {
      _solutions[i] = _solutions[i] / std::complex<double>(_normalisationCoefficient,0.0);
    }
  }
}
std::complex<double> Equation::Proche(std::complex<double> z, double epsilon)
{
  double x, y, ax, ay ;
  std::complex<double> result ;

  x  = z.real();
  y  = z.imag();
  ax = fabs(x);
  ay = fabs(y);

  if (ax < epsilon && ay < epsilon)
    result = std::complex<double>(0.0, 0.0) ;
  else if (ay < epsilon)
    result = std::complex<double>(x, 0.0) ;
  else if (ax < epsilon)
    result = std::complex<double>(0.0, y) ;
  else
  {
    if ((ay / ax) < epsilon)
      result = std::complex<double>(x, 0.0) ;
    else if ((ax / ay) < epsilon)
      result = std::complex<double>(0.0,y) ;
    else
      result = z ;
  }

  return result ;
}

void Equation::Solve()
{
  ComputeTrueDegree();
  Normalisation();

  if(_trueDegree == 1)
  {
    Solve1();
  }
  else if(_trueDegree == 2)
  {
    Solve2();
  }
  else if(_trueDegree == 3)
  {
    Solve3();
  }
  else if(_trueDegree == 4)
  {
    Solve4();
  }
  DeNormalisation();
}

void Equation::Solve1()
{
  _nbrSol = 1;

  if(_order.size() != 0)
    _order.clear();

  _order.push_back(1);

  if(_solutions != NULL)
    delete [] _solutions;
  _solutions = new std::complex<double>[1];
  _solutions[0] = Proche(-_coefficients[0], Epsilon);
}

void Equation::Solve2()
{
  double r1, r2 ;
  // double epsilon = 1.e-12 ;
  std::complex<double> t1, t2, d, z ;
  std::complex<double> aa[2] ;

  aa[0] = _coefficients[0]/ _coefficients[2];
  aa[1] = _coefficients[1]/ _coefficients[2];

  t1 = aa[1]* aa[1] ;
  t2 = aa[0]* std::complex<double>(4.0, 0.0) ;
  r1 = abs(t1) ;
  r2 = abs(t2) ;
  if (r2 > r1)
    r1 = r2 ;
  if (r1 > Epsilon)
  {
    d  = t1- t2 ;
    t1 = d/std::complex<double>(r1, 0.0) ;
  }
  else
  {
    d  = std::complex<double>(0.0, 0.0) ;
    t1 = std::complex<double>(0.0, 0.0) ;
  }
  r1 = abs (t1) ;

  if (r1 <= Epsilon)
  {
    /* 1 double root */

    if(_solutions != NULL)
      delete [] _solutions;
    _solutions = new std::complex<double>[1];

    if(_order.size() != 0)
      _order.clear();

    _nbrSol    = 1 ;
    z         = aa[1]/std::complex<double>(-2.0, 0.0);
    _solutions[0] = Proche (z, Epsilon) ;
    _order.push_back(2);
  }
  else
  {
    /* 2 simple roots */

    if(_solutions != NULL)
      delete [] _solutions;
    _solutions = new std::complex<double>[2];

    if(_order.size() != 0)
      _order.clear();

    _nbrSol   = 2 ;
    d         = sqrt(d) ;
    z         = (d-aa[1])/std::complex<double>(2.0, 0.0) ;
    _solutions[0] = Proche (z, Epsilon) ;
    z         = (d+aa[1])/std::complex<double>(-2.0, 0.0);
    _solutions[1] = Proche (z, Epsilon) ;
    _order.push_back(1);
    _order.push_back(1);
  }
}

void Equation::Solve3(int d4)
{
  int d3_1r3 , d3_1r2_1r1 ;
  double r1, r2, ra[3] ;
  // double epsilon = 1.e-12 ;
  std::complex<double> j, j2 ;
  std::complex<double> d, q, r, s, t, u, v, w ;
  std::complex<double> aa[3], zProv[3][3] ;


  j  = std::complex<double>(-0.5,  sqrt (3.0) / 2.0) ;
  j2 = std::complex<double>(-0.5, -sqrt (3.0) / 2.0) ;

  /* Normalisation of coefficients */
  for (int i1 = 0; i1 < 3; i1++)
    aa[i1] = _coefficients[i1]/_coefficients[3];

  if ( d4 == 0 )
  {
    /* Test of existence of a triple root */
    d3_1r3 = TestDegree3Triple(aa, Epsilon) ;

    /* Test of existence of 1 doubleroot + 1 simple root */
    d3_1r2_1r1 = TestDegree3SimpleDouble(aa, Epsilon) ;
  }
  else
  {
    d3_1r3     = 0 ;
    d3_1r2_1r1 = 0 ;
  }


  if (d3_1r3 == 1)
  {
    /* 1 triple root */
    if(_solutions != NULL)
      delete [] _solutions;
    _solutions = new std::complex<double>[1];

    if(_order.size() != 0)
      _order.clear();

    _nbrSol    = 1 ;
    _solutions[0] = Proche (aa[2]/std::complex<double>(-3.0, 0.0) , Epsilon) ;
    _order.push_back(3);
  }
  else if (d3_1r2_1r1 == 1)
  {
    /* 1 simple root + 1 double root */

    if(_solutions != NULL)
      delete [] _solutions;
    _solutions = new std::complex<double>[2];

    if(_order.size() != 0)
      _order.clear();

    u = (aa[1]* aa[2])/ std::complex<double>(6.0, 0.0) ;
    v = aa[0]/ std::complex<double> (2.0, 0.0);
    w = pow(aa[2], 3.0)/ std::complex<double>(27.0, 0.0);
    r = (u-v)- w ;
    u = pow (r, 1.0 / 3.0) ;
    w = aa[2]/ std::complex<double>(3.0, 0.0);
    zProv[0][0] = -u;
    zProv[0][1] = u * std::complex<double>(2.0,0.0);
    zProv[1][0] = u * (j + std::complex<double>(1.0, 0.0));
    zProv[1][1] = u * (j2* std::complex<double>(2.0, 0.0));
    zProv[2][0] = u * (j2+ std::complex<double>(1.0, 0.0));
    zProv[2][1] = u * (j * std::complex<double>(2.0, 0.0));
    for (int i1 = 0; i1 <= 2; i1++)
    {
      zProv[i1][0] = zProv[i1][0]- w;
      zProv[i1][1] = zProv[i1][1]- w;
    }
    for (int i2 = 0; i2 < 3; i2++)
    {
      u = std::complex<double>(1.0, 0.0) ;

      for (int i1 = 2; i1 >= 0; i1--)
        u = u*zProv[i2][0]+aa[i1] ;

      r1  = abs (u) ;
      u   = std::complex<double>(1.0, 0.0) ;

      for (int i1 = 2; i1 >= 0; i1--)
        u = u*zProv[i2][1]+ aa[i1] ;

      r2  = abs (u) ;
      ra[i2] = r1 * r1 + r2 * r2 ;
    }
    int i3        = IndiceMin(3, ra) ;
    _solutions[0] = Proche ( zProv[i3][0] , Epsilon ) ;
    _solutions[1] = Proche ( zProv[i3][1] , Epsilon ) ;
    _nbrSol   = 2 ;
    _order.push_back(2);
    _order.push_back(1);
  }
  else
  {
    /* 3 simple roots */
    u = aa[1]/ std::complex<double>(3.0, 0.0);
    v = (aa[2]* aa[2]) / std::complex<double>(9.0, 0.0) ;
    q = u- v ;
    u = (aa[1]* aa[2]) / std::complex<double>(6.0, 0.0) ;
    v = aa[0]/ std::complex<double>(2.0, 0.0) ;
    w = pow(aa[2], 3.0) / std::complex<double>(27.0, 0.0);
    r = (u-v) - w ;
    d = sqrt(pow(q, 3.0) + pow(r, 2.0)) ;
    s = pow ((r + d) , 1.0 / 3.0) ;
    t = pow ((r - d) , 1.0 / 3.0) ;
    w = aa[2]/ std::complex<double>(3.0, 0.0);

    zProv[0][0] = s+ t;
    zProv[0][1] = (j*  s) + (j2* t) ;
    zProv[0][2] = (j2* s) + (j* t) ;
    zProv[1][0] = s + (j* t);
    zProv[1][1] = (j* s) + t;
    zProv[1][2] = j2 * (s+ t) ;
    zProv[2][0] = s + (j2* t) ;
    zProv[2][1] = j * (s+ t);
    zProv[2][2] = (j2* s) + t;
    for (int i1 = 0; i1 < 3; i1++)
    {
      for (int i2 = 0; i2 < 3; i2++)
        zProv[i1][i2] = zProv[i1][i2]- w;
    }
    for (int i3 = 0; i3 < 3; i3++)
    {
      ra[i3] = 0.0 ;
      for (int i2 = 0; i2 < 3; i2++)
      {
        u = std::complex<double>(1.0, 0.0) ;
        for (int i1 = 2; i1 >= 0; i1--)
          u = (u* zProv[i3][i2]) + aa[i1] ;

        r1     = abs (u) ;
        ra[i3] = ra[i3] + r1 * r1 ;
      }
    }
    int i1 = IndiceMin(3, ra) ;


    if(_solutions != NULL)
      delete [] _solutions;
    _solutions = new std::complex<double>[3];

    if(_order.size() != 0)
      _order.clear();

    _nbrSol   = 3 ;
    for (int i3 = 0; i3 < 3; i3++)
      _solutions[i3] = Proche (zProv[i1][i3] , Epsilon) ;
    _order.push_back(1);
    _order.push_back(1);
    _order.push_back(1);
  }
}

void Equation::Solve4()
{
  int d4_1r4, d4_2r2, d4_1r3_1r1, d4_1r2_2r1 ;
  int d4 = 1 ;
  double epsilon = 1.e-12 ;
  double r, h[4] ;
  std::complex<double> d, u, v, w, x, y ;
  std::complex<double> aa[4], k[4], b[4], zProv[2][4] ;


  /* Normalisation of coefficients */
  for (int i1 = 0; i1 < 4; i1++)
    aa[i1] = _coefficients[i1]/ _coefficients[4];

  /* Equation reduction :  on the form            */
  /*       (x-s)**4 + p(x-s)**2 + q(x-s) + r = 0        */
  /* these coefficients are inserted into table k :    */
  /*           k[0] = s                                 */
  /*           k[1] = p                                 */
  /*           k[2] = q                                 */
  /*           k[3] = r                                 */
  k[0] = aa[3]/ std::complex<double>(-4.0, 0.0) ;
  u    = aa[2] ;
  v    = ((aa[3]* std::complex<double>(3.0, 0.0)) * (aa[3]/ std::complex<double>(8.0, 0.0))) ;
  h[0] = abs (u) ;
  h[1] = abs (v) ;

  if (h[0] < h[1])
    h[0] = h[1] ;
  if (h[0] > Epsilon)
    k[1] = u- v;
  else
    k[1] = std::complex<double>(0.0, 0.0) ;

  u    = aa[1] ;
  v    = (aa[2]* aa[3]) / std::complex<double>(2.0, 0.0) ;
  w    = pow (aa[3], 3.0) /   std::complex<double>(8.0, 0.0) ;
  h[0] = abs (u) ;
  h[1] = abs (v) ;
  h[2] = abs (w) ;
  h[0] = h[IndiceMax(3, h)] ;
  if (h[0] > Epsilon)
    k[2] = (u- v)+ w ;
  else
    k[2] = std::complex<double>(0.0, 0.0) ;
  u     = aa[0] ;
  v    = (aa[1]* aa[3]) / std::complex<double>(4.0, 0.0);
  w    = (aa[2]* aa[3]) * (aa[3]/ std::complex<double>(16.0, 0.0)) ;
  x    = pow (aa[3], 4.0) * std::complex<double>(3.0 / 256.0, 0.0) ;
  h[0] = abs (u) ;
  h[1] = abs (v) ;
  h[2] = abs (w) ;
  h[3] = abs (x) ;
  h[0] = h[IndiceMax(4, h)] ;

  if (h[0]>Epsilon)
    k[3] = (u- v) + (w- x) ;
  else
    k[3] = std::complex<double>(0.0, 0.0) ;

  /* Test of existence of a quadruple root */
  d4_1r4 = TestDegree4Quad (k, epsilon) ;

  /* Test of existence of 2 double roots */
  d4_2r2 = TestDegree4DoubleDouble (aa, k, epsilon) ;

  /* Test of existence of 1 triple root + 1 simple root */
  d4_1r3_1r1 = TestDegree4SimpleTriple (aa, k, epsilon) ;

  /* Test of existence of 1 double root + 2 simple roots */
  d4_1r2_2r1 = TestDegreeSimpleSimpleDouble (k, epsilon) ;


  if (d4_1r4 == 1)
  {
    /* 1 quadruple root */

    if(_solutions != NULL)
      delete [] _solutions;
    _solutions = new std::complex<double>[1];


    if(_order.size() != 0)
      _order.clear();

    _nbrSol    = 1 ;
    _solutions[0] = Proche (k[0], Epsilon) ;
    _order.push_back(4);
  }
  else if (d4_2r2 == 1)
  {
    /* 2 double roots */

    if(_solutions != NULL)
      delete [] _solutions;
    _solutions = new std::complex<double>[2];

    if(_order.size() != 0)
      _order.clear();

    u         = sqrt (k[1]/ std::complex<double>(-2.0, 0.0)) ;
    _solutions[0] = Proche ((k[0]+ u) , Epsilon) ;
    _solutions[1] = Proche ((k[0]- u) , Epsilon) ;
    _nbrSol    = 2 ;
    _order.push_back(2);
    _order.push_back(2);
  }
  else if (d4_1r3_1r1 == 1)
  {
    /* 1 triple root + 1 simple root */

    if(_solutions != NULL)
      delete [] _solutions;
    _solutions = new std::complex<double>[2];

    if(_order.size() != 0)
      _order.clear();

    u         = (k[2]* std::complex<double>(-3.0, 0.0)) / (k[1]* std::complex<double>(4.0, 0.0)) ;
    v         = u * std::complex<double>(-3.0, 0.0);
    _solutions[0] = Proche ((k[0]+ u) , Epsilon) ;
    _solutions[1] = Proche ((k[0]+ v) , Epsilon) ;
    _nbrSol    = 2 ;
    _order.push_back(3);
    _order.push_back(1);
  }
  else if (d4_1r2_2r1 == 1)
  {
    /* 1 double root + 2 simple roots */

    if(_solutions != NULL)
      delete [] _solutions;
    _solutions = new std::complex<double>[3];

    if(_order.size() != 0)
      _order.clear();

    if (abs (k[1]) <= Epsilon)
    {
      u         = (k[3]* std::complex<double>(-4.0, 0.0)) / (k[2]* std::complex<double>(3.0, 0.0));
      v         = sqrt ((u* std::complex<double>(-2.0, 0.0)) * u) ;
      _solutions[0] = Proche ((k[0]+ u) , Epsilon) ;
      _solutions[1] = Proche ((k[0] + (v- u)) , Epsilon) ;
      _solutions[2] = Proche ((k[0] - (v+ u)) , Epsilon) ;
    }
    else
    {
      v = (k[2]* k[2]) * std::complex<double>(9.0, 0.0) ;
      w = (k[1]* k[3]) * std::complex<double>(32.0, 0.0) ;
      u = sqrt((v- w)) ;
      v = (k[2] * std::complex<double>(-3.0, 0.0)) ;
      w = (k[1] * std::complex<double>(4.0, 0.0)) ;
      x = ((v+ u) / w) ;
      y = ((v- u) / w) ;
      u = ((x* x) * std::complex<double>(-2.0, 0.0)) ;
      u = sqrt((u- k[1])) ;
      v = ((y* y) * std::complex<double>(-2.0, 0.0)) ;
      v = sqrt((v- k[1])) ;
      zProv[0][0] = (k[0]+ x) ;
      zProv[0][1] = (k[0] - (x- u)) ;
      zProv[0][2] = (k[0] - (x+ u)) ;
      zProv[1][0] = (k[0]+ y) ;
      zProv[1][1] = (k[0] - (y- v)) ;
      zProv[1][2] = (k[0] - (y+ v)) ;
      h[0] = 0.0 ;
      h[1] = 0.0 ;
      for (int i1 = 0; i1 < 3; i1++)
      {
        u = std::complex<double>(1.0, 0.0) ;
        for (int i2 = 3; i2 >= 0; i2--)
          u = ((u* zProv[0][i1])+ aa[i2]) ;

        r    = abs (u) ;
        h[0] = h[0] + r * r ;
        u    = std::complex<double>(1.0, 0.0) ;
        for (int i2 = 3; i2 >= 0; i2--)
          u = ((u* zProv[1][i1])+ aa[i2]) ;
        r    = abs (u) ;
        h[1] = h[1] + r * r ;
      }
      int i1 = IndiceMin (2, h) ;
      for (int i2 = 0; i2 < 3; i2++)
        _solutions[i2] = Proche (zProv[i1][i2] , Epsilon) ;
    }
    _nbrSol   = 3 ;
    _order.push_back(2);
    _order.push_back(1);
    _order.push_back(1);
  }
  else
  {
    /* 4 simple roots */
    u    = ((aa[0]* aa[2]) * std::complex<double>(4.0, 0.0)) ;
    v    = (aa[1] * aa[1]) ;
    w    = ((aa[0]* aa[3]) * aa[3]) ;
    b[0] = (u - (v+ w)) ;
    b[1] = ((aa[1]* aa[3]) - (aa[0]* std::complex<double>(4.0, 0.0))) ;
    b[2] = -aa[2] ;
    b[3] = std::complex<double>(1.0, 0.0) ;

    /* The third degree equation is solved by forcing 3 distinct roots (computation precision problem) */
    Equation eq(3,b);
    eq.Solve3(d4);

    //Solve3(d4);
    h[0] = abs ((eq.get_solutions()[1]- eq.get_solutions()[2])) ; /* the root the most distant to    */
    h[1] = abs ((eq.get_solutions()[0]- eq.get_solutions()[2])) ; /* the 2 others is selected       */
    h[2] = abs ((eq.get_solutions()[0]- eq.get_solutions()[1])) ;
    int i1   = IndiceMin (3, h) ;
    u    = eq.get_solutions()[i1] ;
    v    = ((aa[2]- u) * std::complex<double>(4.0, 0.0)) ;
    v    = sqrt(((aa[3]* aa[3]) - v)) ;
    w    = sqrt(((u* u) - (aa[0]* std::complex<double>(4.0, 0.0)))) ;
    x    = ((aa[3]+ v) / std::complex<double>(2.0, 0.0)) ;
    y    = ((u+ w) / std::complex<double>(2.0, 0.0)) ;
    d    = sqrt(((x* x) - (y* std::complex<double>(4.0, 0.0)))) ;
    zProv[0][0] = ((x- d) / std::complex<double>(-2.0, 0.0)) ;
    zProv[0][1] = ((x+ d) / std::complex<double>(-2.0, 0.0)) ;

    x    = ((aa[3]- v) / std::complex<double>(2.0, 0.0)) ;
    y    = ((u- w) / std::complex<double>(2.0, 0.0)) ;
    d    = sqrt((x*x) - (y* std::complex<double>(4.0, 0.0))) ;
    zProv[0][2] = ((x- d) / std::complex<double>(-2.0, 0.0)) ;
    zProv[0][3] = ((x+ d) / std::complex<double>(-2.0, 0.0)) ;

    x    = ((aa[3]+ v) / std::complex<double>(2.0, 0.0)) ;
    y    = ((u- w) / std::complex<double>(2.0, 0.0)) ;
    d    = sqrt(((x*x) - (y* std::complex<double>(4.0, 0.0)))) ;
    zProv[1][0] = ((x- d) / std::complex<double>(-2.0, 0.0)) ;
    zProv[1][1] = ((x+ d) / std::complex<double>(-2.0, 0.0)) ;

    x = ((aa[3]- v) / std::complex<double>(2.0, 0.0)) ;
    y = ((u+ w) / std::complex<double>(2.0, 0.0)) ;
    d = sqrt((x*x) - (y* std::complex<double>(4.0, 0.0))) ;
    zProv[1][2] = ((x- d) / std::complex<double>(-2.0, 0.0)) ;
    zProv[1][3] = ((x+ d) / std::complex<double>(-2.0, 0.0)) ;

    h[0] = 0.0 ;
    h[1] = 0.0 ;
    for (int i1 = 0; i1 < 4; i1++)
    {
      u = std::complex<double>(1.0, 0.0) ;

      for (int i2 = 3; i2 >= 0; i2--)
        u = ((u* zProv[0][i1])+ aa[i2]) ;

      r    = abs (u) ;
      h[0] = h[0] + r * r ;
      u    = std::complex<double>(1.0, 0.0) ;

      for (int i2 = 3; i2 >= 0; i2--)
        u = ((u* zProv[1][i1])+ aa[i2]) ;

      r    = abs(u) ;
      h[1] = h[1] + r * r ;
    }
    i1 = IndiceMin (2, h) ;

    if(_solutions != NULL)
      delete [] _solutions;
    _solutions = new std::complex<double>[4];

    if(_order.size() != 0)
      _order.clear();

    for (int i2 = 0; i2 < 4; i2++)
    {
      _solutions[i2] = Proche (zProv[i1][i2] , Epsilon) ;
      _order.push_back(1);
    }
    _nbrSol = 4 ;
  }

}

int Equation::TestDegree3Triple(std::complex<double>* a, double epsilon)
{
  double d, dp[2], q, r ;
  std::complex<double> u, v ;


  u     = a[2]* a[2] ;
  v     = a[1]*std::complex<double>(3.0, 0.0) ;
  dp[0] = abs (u) ;
  dp[1] = abs (v) ;
  d     = (dp[0] > dp[1]) ? dp[0] : dp[1] ;
  q     = (d > epsilon) ? abs((u- v)/ std::complex<double> (d, 0.0))
              : 0.0 ;

  u     = a[1]* a[2];
  v     = a[0]* std::complex<double>(9.0, 0.0) ;
  dp[0] = abs (u) ;
  dp[1] = abs (v) ;
  d     = (dp[0] > dp[1]) ? dp[0] : dp[1] ;
  r     = (d > epsilon) ? abs ((u-v)/std::complex<double>(d, 0.0))
              : 0.0 ;

  return ((q <= epsilon) && (r <= epsilon)) ? 1 : 0 ;
}

int Equation::TestDegree3SimpleDouble(std::complex<double>* a, double epsilon)
{
  double d, k, r[5] ;
  std::complex<double> u, t[5] ;


  u    = a[1]*a[2] ;
  t[0] = pow (a[1], 3.0)*std::complex<double>(4.0,  0.0) ;
  t[1] = a[0]*a[0]* std::complex<double>(27.0, 0.0);
  t[2] = a[0]* std::complex<double>(4.0, 0.0)*pow (a[2], 3.0) ;
  t[3] = -(u* u) ;
  t[4] = std::complex<double>(-18.0, 0.0)* a[0] * u;

  for (int i = 0 ; i < 5 ; i++)
    r[i] = abs (t[i]) ;

  k = r[IndiceMax(5, r)] ;

  for (int i = 1 ; i < 5 ; i++)
    t[0] = t[0]+t[i];

  d = (k > epsilon) ? abs (t[0]/std::complex<double>(k,0.0))
            : 0.0 ;

  return (d <= epsilon) ? 1 : 0 ;
}

int Equation::IndiceMin ( int n , double *list )
{
  int iMin ;
  double xMin ;

  iMin = 0 ;
  xMin = list[0] ;
  for (int i = 1; i < n; i++)
  {
    if ( list[i] < xMin )
    {
      iMin = i ;
      xMin = list[i] ;
    }
  }
  return iMin ;
}


int Equation::IndiceMax ( int n , double *list)
{
  int iMax ;
  double xMax ;

  iMax = 0 ;
  xMax = list[0] ;
  for (int i = 1; i < n; i++)
  {
    if ( list[i] > xMax )
    {
      iMax = i ;
      xMax = list[i] ;
    }
  }
  return iMax ;
}

int Equation::TestDegree4Quad ( std::complex<double> *a , double epsilon )
{
  double r1, r2, r3 ;

  r1 = abs (a[1]) ;
  r2 = abs (a[2]) ;
  r3 = abs (a[3]) ;

  return ((r1 < epsilon) && (r2 < epsilon) && (r3 < epsilon)) ? 1 : 0 ;
}

int Equation::TestDegree4DoubleDouble ( std::complex<double> *a , std::complex<double> *k , double epsilon )
{
  int d4 ;
  double r0, r1, r2, h[5] ;
  std::complex<double> u, t[5] ;


  u    = (a[3]*  a[3]) ;
  t[0] = (u* u) * std::complex<double>(3.0 / 16.0, 0.0);
  t[1] = a[0] * std::complex<double>(4.0,0.0);
  t[2] = a[3] * a[1];
  t[3] = u *    a[2];
  t[4] = a[2] * a[2];
  for (int i = 0 ; i < 5 ; i++)
    h[i] = abs (t[i]) ;

  r1 = h[IndiceMax(5, h)] ;
  u  = (t[0]- t[1]) + t[2];
  u  = u - (t[3]- t[4]) ;
  r2 = (r1 > epsilon)  ?  abs (u) / r1  :  0.0 ;

  r1 = abs (k[2]) ;
  r0 = abs (k[1]) ;

  if ((r0 >= epsilon) && (r1 < epsilon) && (r2 < epsilon))
    d4 = 1 ;
  else
    d4 = 0 ;

  return d4 ;
}

int Equation::TestDegree4SimpleTriple ( std::complex<double> *a , std::complex<double> *k , double epsilon )
{
  int d4 ;
  double r, r0, r1, r2, h[4] ;
  std::complex<double> u, t[4] ;


  t[0] = a[2] * a[2] ;
  t[1] = a[0] * std::complex<double>(12.0, 0.0);
  t[2] = (a[1]* a[3]) * std::complex<double>(3.0, 0.0) ;
  for (int i = 0 ; i < 3 ; i++)
    h[i] = abs (t[i]) ;

  r  = h[IndiceMax(3, h)] ;
  u  = t[0] + (t[1]- t[2]) ;
  r1 = (r > epsilon)  ?  abs (u) / r  :  0.0 ;

  t[0] = a[1] * a[1];
  t[1] = (a[0]* a[2]) * std::complex<double>(4.0, 0.0) ;
  u    = a[3]* std::complex<double>(3.0, 0.0) ;
  t[2] = (u* u) * (a[0]* std::complex<double>(3.0, 0.0)) ;
  t[3] = (a[2]* a[2]) * a[2] ;
  for (int i = 0 ; i < 4 ; i++)
    h[i] = abs (t[i]) ;
  r  = h[IndiceMax(4, h)] ;
  u  = ((t[0]- t[1])* std::complex<double>(27.0, 0.0))+ (t[2]- t[3]) ;
  r2 = (r > epsilon)  ?  abs (u) / r  :  0.0 ;

  r0 = abs (k[1]) ;

  if ((r0 >= epsilon) && (r1 < epsilon) && (r2 < epsilon))
    d4 = 1 ;
  else
    d4 = 0 ;

  return d4 ;
}

int Equation::TestDegreeSimpleSimpleDouble( std::complex<double> *a , double epsilon )
{
  double r[3] ;
  std::complex<double> u, v, w ;


  u    = (a[1]* a[1]) - (a[3]* std::complex<double>(4.0, 0.0)) ;
  u    = u* a[3] * u * std::complex<double>(16.0, 0.0);
  r[0] = abs (u) ;

  w    = pow (a[1], 3.0) *  std::complex<double>(4.0, 0.0) ;
  v    = (a[2]* a[2]) * std::complex<double>(27.0, 0.0) ,
  v    = (w+ v) * (a[2]* a[2]) ;
  r[1] = abs (v) ;

  w    = (a[1]* a[2]) * (a[2]* a[3]) ;
  w    = w * std::complex<double>(144.0, 0.0) ;
  r[2] = abs (w) ;

  r[1] = r[IndiceMax(3, r)] ;
  if (r[1] < epsilon)
    r[0] = 0.0 ;
  else if (r[1] <= 1.0)
    r[0] = abs ((u- v)+ w) ;
  else
    r[0] = abs (((u- v)+ w)/ std::complex<double>(r[1], 0.0)) ;

  return (r[0] < (2.0 * epsilon))  ?  1  :  0 ;
}
}
