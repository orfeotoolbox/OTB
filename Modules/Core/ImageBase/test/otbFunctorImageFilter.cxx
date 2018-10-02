/*
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

//#include "otbFunctor.h"
//#include "otbFunctorImageFilter.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkNeighborhood.h"
#include "itkImageRegionIterator.h"
#include "itkImageSource.h"
#include <tuple>


// Proxy to switch image type from T (Jordi has better code for that)
template <typename T> struct ImageTypeProxy
{
  using ImageType = otb::Image<T>;
};

template <typename T> struct ImageTypeProxy<itk::VariableLengthVector<T> >
{
  using ImageType = otb::VectorImage<T>;
};


// Variadic creation of iterator tuple
template <class T> auto MakeIterator(itk::SmartPointer<T> img)
{
  itk::ImageRegionConstIterator<T> it(img,img->GetLargestPossibleRegion());
  return it;
}


template<class... T>  auto MakeIterators(itk::SmartPointer<T>... args)
  {
    return std::make_tuple(MakeIterator(args)...);
  }

// Variadic call of operator from iterator tuple
template <class Tuple, class Oper, size_t...Is> auto CallOperatorImpl(Tuple& t, const Oper & oper,std::index_sequence<Is...>)
{
  return oper(std::get<Is>(t).Get()...);
}

template <class Oper, typename ... Args> auto CallOperator(const Oper& oper, std::tuple<Args...> & t)
{
  return CallOperatorImpl(t,oper,std::make_index_sequence<sizeof...(Args)>{});
}

// Variadic move of iterators
template<class Tuple,size_t...Is> auto MoveIteratorsImpl(Tuple & t, std::index_sequence<Is...>)
{
  return std::make_tuple(++(std::get<Is>(t) )...);
}

template<typename ... Args> void MoveIterators(std::tuple<Args...> & t)
{
  MoveIteratorsImpl(t,std::make_index_sequence<sizeof...(Args)>{});
}


// Variadic functor class (will disapear)
template <typename Out, typename... T> class VariadicFunctor
{  
public:  
  Out operator()(const T&...) const
    {
    return Out();
    }

  unsigned int GetNumberOfVariadicArgs() const
  {
    const unsigned int nbArgs = sizeof...(T);
    return nbArgs;
  }
};

// Example of functor
template<typename Out, typename In1, typename In2> class AddFunctor : public VariadicFunctor<Out,In1,In2>
{
public:
  Out operator()(const In1 & in1, const In2 & in2)
  {
    return static_cast<Out>(in1+in2);
  }
};


template <typename ... Args> void Ignore(Args ...)
{}

template<class TOuptut, class ... TInputs> class VariadicInputsImageFilter : public itk::ImageSource<TOuptut>
{
public:
  using Self = VariadicInputsImageFilter<TOuptut, TInputs...>;
  using Pointer =  itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;
  using Superclass = itk::ImageSource<TOuptut>;

  using InputTypesTupleType = std::tuple<TInputs...>;

  itkNewMacro(Self);
  
  template <std::size_t I> void SetVInput(typename std::tuple_element<I,InputTypesTupleType>::type * inputPtr)
  {
    this->SetNthInput(I,inputPtr);
  }

  template <std::size_t I> const typename std::tuple_element<I,InputTypesTupleType>::type * GetVInput()
  {
    using ImageType = typename std::tuple_element<I,InputTypesTupleType>::type;
    return dynamic_cast<const ImageType *>(this->GetInput(I));
  }

  void SetVInputs(TInputs*... inputs)
  {
    auto inTuple = std::make_tuple(inputs...);
    SetInputsImpl(inTuple,std::make_index_sequence<sizeof...(inputs)>{});
  }

protected:
  VariadicInputsImageFilter()
  {
    this->SetNumberOfRequiredInputs(sizeof...(TInputs));
  };
  
  ~VariadicInputsImageFilter() = default;
  
private:
  template<class Tuple, size_t...Is> auto SetInputsImpl(Tuple& t, std::index_sequence<Is...>)
  {
    return std::initializer_list<int>{(this->SetVInput<Is>(std::get<Is>(t)),0)...};
  }
  
  VariadicInputsImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};


using namespace otb;

int otbFunctorImageFilter(int, char ** )
{
  using VectorImageType = VectorImage<double>;
  using ImageType       = Image<unsigned int>;
  using RegionType      = typename ImageType::RegionType;
  using SizeType        = typename RegionType::SizeType;
  using IndexType       = typename RegionType::IndexType;
  
  using NeighborhoodType = itk::Neighborhood<double,2>;
  using VectorPixelType  = typename VectorImageType::PixelType;

  auto vimage = VectorImageType::New();
  auto image  = ImageType::New();

  SizeType size({200,200});
  
  vimage->SetRegions(size);
  image->SetRegions(size);

  auto f1 = VariadicFunctor<double,itk::VariableLengthVector<double>,double>();
  std::cout<<"Nb variadic args: "<<f1.GetNumberOfVariadicArgs();

  auto iterators = MakeIterators(vimage,image);

  MoveIterators(iterators);

  auto res = CallOperator(f1,iterators);
 

  // auto it = MakeIterator(image,image->GetLargestPossibleRegion(),3, no_radius_tag());
  // auto vit = MakeIterator(vimage,vimage->GetLargestPossibleRegion(),3,radius_tag());


  auto filter = VariadicInputsImageFilter<VectorImageType,VectorImageType,ImageType>::New();

  filter->SetVInput<0>(vimage);
  filter->SetVInput<1>(image);

  filter->SetVInputs(vimage,image);

  std::cout<<filter->GetVInput<0>()<< filter->GetVInput<1>()<<std::endl;
  
  return EXIT_SUCCESS;
}
