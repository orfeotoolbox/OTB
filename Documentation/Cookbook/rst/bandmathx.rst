BandMathX application (based on muParserX)
==========================================

This section describes how to use the BandMathX application.

Syntax : first elements
-----------------------

The default prefix name for variables related to the ith input is
*im(i+1)* (note the indexing from 1 to N, for N inputs). The user has
the possibility to change this default behaviour by setting its own
prefix.

In this document, we will keep the default convention. Following list
summaries the available variables for input #0 (and so on for every
input).

\|\|l\|l\|l\|\| **Variables & **Description & **Type
im1 & a pixel from first input, made of n components/bands (first image
is indexed by 1) & Vector
im1bj & jth component of a pixel from first input (first band is indexed
by 1) & Scalar
im1bjNkxp & a neighbourhood (“N”) of pixels of the jth component from
first input, of size kxp & Matrix
im1bjMini & global statistic : minimum of the jth band from first input
& Scalar
im1bjMaxi & global statistic : maximum of the jth band from first input
& Scalar
im1bjMean & global statistic : mean of the jth band from first input &
Scalar
im1bjSum & global statistic : sum of the jth band from first input &
Scalar
im1bjVar & global statistic : variance of the jth band from first input
& Scalar
im1PhyX and im1PhyY & spacing of first input in X and Y directions &
Scalar
******

[variables]

Moreover, we also have the generic variables idxX and idxY that
represent the indices of the current pixel (scalars).

For instance, the following formula (addition of two pixels)

.. math:: im1+im2

[firstequation]

is correct only if the two first inputs have the same number of bands.
In addition, the following formula is not consistent even if im1
represents a pixel of an image made of only one band:

.. math:: im1+1

A scalar can’t be added to a vector. The right formula is instead (one
can notice the way that muParserX allows to define vectors on the fly):

.. math:: im1+\{ 1 \}

or

.. math:: im1 + \{1,1,1,...,1\}

if im1 is made of n components.

On the other hand, the variable im1b1 for instance is represented as a
scalar; so we have the following different possibilities:

\|\|c\|c\|\| **Expression & **Status
im1b1 + 1 & correct
{im1b1} + {1} & correct
im1b1 + {1} & incorrect
{im1b1} + 1 & incorrect
im1 + {im2b1,im2b2} & correct if im1 represents a pixel of two
components (equivalent to im1 + im2)
****

[correctness]

Similar remarks can be made for the multiplication/division; for
instance, the following formula is incorrect:

.. math:: \{im2b1,im2b2\} * \{1,2\}

whereas this one is correct:

.. math:: \{im2b1,im2b2\} * \{1,2\}'

or in more simple terms (and only if im2 contains two components):

.. math:: im2* \{1,2\}'

Concerning division, this operation is not originally defined between
two vectors (see next section “New operators and functions”
-[ssec:operators]-).

Now, let’s go back to the first formula: this one specifies the addition
of two images band to band. With muParserX lib, we can now define such
operation with only one formula, instead of many formulas (as many as
the number of bands). We call this new functionality the **batch mode**,
which directly arises from the introduction of vectors within muParserX
framework.

Finally, let’s say a few words about neighbourhood variables. These
variables are defined for each particular input, and for each particular
band. The two last numbers, kxp, indicate the size of the neighbourhood.
All neighbourhoods are centred: this means that k and p can only be odd
numbers. Moreover, k represents the dimension in the x direction (number
of columns), and p the dimension in the y direction (number of rows).
For instance, im1b3N3x5 represents the following neighbourhood:

+-----+-----+-----+
| .   | .   | .   |
+=====+=====+=====+
| .   | .   | .   |
+-----+-----+-----+
| .   | .   | .   |
+-----+-----+-----+
| .   | .   | .   |
+-----+-----+-----+
| .   | .   | .   |
+-----+-----+-----+

[correctness]

Fundamentally, a neighbourhood is represented as a matrix inside the
muParserX framework; so the remark about mathematically well-defined
formulas still stands.

New operators and functions
---------------------------

New operators and functions have been implemented within BandMathX
application. These ones can be divided into two categories.

-  adaptation of existing operators/functions, that were not originally
   defined for vectors and matrices (for instance cos, sin, ...). These
   new operators/ functions keep the original names to which we add the
   prefix “v” for vector (vcos, vsin, ...) .

-  truly new operators/functions.

Concerning the last category, here is a list of implemented operators or
functions (they are all implemented in otbParserXPlugins.h/.cxx files
-OTB/Code/Common-):

**Operators div and dv** The first operator allows the definition of an
element-wise division of two vectors (and even matrices), provided that
they have the same dimensions. The second one allows the definition of
the division of a vector/matrix by a scalar (components are divided by
the same unique value). For instance:

.. math:: im1 ~ div ~ im2

.. math:: im1 ~ dv ~ 2.0

**Operators mult and mlt** These operators are the duals of the previous
ones. For instance:

.. math:: im1 ~  mult ~ im2

.. math:: im1 ~  mlt ~ 2.0

Note that the operator ’\*’ could have been used instead of ’pw’ one.
But ’pw’ is a little bit more permisive, and can tolerate
one-dimensional vector as right element.

**Operators pow and pw** The first operator allows the definition of an
element-wise exponentiation of two vectors (and even matrices), provided
that they have the same dimensions. The second one allows the definition
of the division of a vector/matrix by a scalar (components are
exponentiated by the same unique value). For instance:

.. math:: im1 ~ pow ~ im2

.. math:: im1 ~ pw ~ 2.0

**Function bands** This function allows to select specific bands from an
image, and/or to rearrange them in a new vector; for instance:

.. math:: bands(im1,\{1,2,1,1\})

produces a vector of 4 components made of band 1, band 2, band 1 and
band 1 values from the first input. Note that curly brackets must be
used in order to select the desired band indices.

**Function dotpr ** This function allows the dot product between two
vectors or matrices (actually in our case, a kernel and a neighbourhood
of pixels):

.. math:: \sum_{(i,j)} m_1(i,j)*m_2(i,j)

For instance:

.. math:: dotpr(kernel1,im1b1N3x5)

is correct provided that kernel1 and im1b1N3x5 have the same dimensions.
The function can take as many neighbourhoods as needed in inputs.

**Function mean** This function allows to compute the mean value of a
given vector or neighborhood (the function can take as many inputs as
needed; one mean value is computed per input). For instance:

.. math:: mean(im1b1N3x3,im1b2N3x3,im1b3N3x3,im1b4N3x3)

Note: a limitation coming from muparserX itself makes impossible to pass
all those neighborhoods with a unique variable.

**Function var** This function allows to compute the variance of a given
vector or neighborhood (the function can take as many inputs as needed;
one var value is computed per input). For instance:

.. math:: var(im1b1N3x3)

**Function median** This function allows to compute the median value of
a given vector or neighborhood (the function can take as many inputs as
needed; one median value is computed per input). For instance:

.. math:: median(im1b1N3x3)

**Function corr** This function allows to compute the correlation
between two vectors or matrices of the same dimensions (the function
takes two inputs). For instance:

.. math:: corr(im1b1N3x3,im1b2N3x3)

**Function maj** This function allows to compute the most represented
element within a vector or a matrix (the function can take as many
inputs as needed; one maj element value is computed per input). For
instance:

.. math:: maj(im1b1N3x3,im1b2N3x3)

**Function vmin and vmax** These functions allow to compute the min or
max value of a given vector or neighborhood (only one input). For
instance:

.. math:: (vmax(im3b1N3x5)+vmin(im3b1N3x5)) ~ div ~ \{2.0\}

**Function cat** This function allows to concatenate the results of
several expressions into a multidimensional vector, whatever their
respective dimensions (the function can take as many inputs as needed).
For instance:

.. math:: cat(im3b1,vmin(im3b1N3x5),median(im3b1N3x5),vmax(im3b1N3x5))

Note: the user should prefer the use of semi-colons (;) when setting
expressions, instead of directly use this function. The application will
call the function ’cat’ automatically.

**Function ndvi** This function implements the classical normalized
difference vegetation index; it tkaes two inputs. For instance:

.. math:: ndvi(im1b1,im1b4)

First argument is related to the visible red band, and the second one to
the near-infrareds band.

The table below summarises the different functions and operators.

\|\|l\|l\|\| **Variables & **Remark
ndvi & two inputs
bands & two inputs; length of second vector input gives the dimension of
the output
dotptr & many inputs
cat & many inputs
mean & many inputs
var & many inputs
median & many inputs
maj & many inputs
corr & two inputs
div and dv & operators
mult and mlt & operators
pow and pw & operators
vnorm & adapation of an existing function to vectors : one input
vabs & adapation of an existing function to vectors : one input
vmin & adapation of an existing function to vectors : one input
vmax & adapation of an existing function to vectors : one input
vcos & adapation of an existing function to vectors : one input
vsin & adapation of an existing function to vectors : one input
vtan & adapation of an existing function to vectors : one input
vtanh & adapation of an existing function to vectors : one input
vsinh & adapation of an existing function to vectors : one input
vcosh & adapation of an existing function to vectors : one input
vlog & adapation of an existing function to vectors : one input
vlog10 & adapation of an existing function to vectors : one input
vexp & adapation of an existing function to vectors : one input
vsqrt & adapation of an existing function to vectors : one input
****

[bandmathOperators]
