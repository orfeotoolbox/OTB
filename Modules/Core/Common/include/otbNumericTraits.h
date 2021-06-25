#ifndef OtbNumericTraits_h
#define OtbNumericTraits_h

namespace otb
{

template <typename I, typename O>
constexpr auto common_lowest() {
    using C = std::common_type_t<I,O>;
    return
      std::is_unsigned<I>::value ? I{}
    : std::is_unsigned<O>::value ? O{}
    : std::max<C>(std::numeric_limits<I>::lowest(), std::numeric_limits<O>::lowest());
}

template <typename I, typename O>
constexpr auto common_highest() {
    using C = std::common_type_t<I,O>;
    return std::min<C>(std::numeric_limits<I>::max(), std::numeric_limits<O>::max());
}

} // Namespace otb

#endif // OtbNumericTraits_h
