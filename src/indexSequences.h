#include <cstddef>

#if __cplusplus > 201103L // >C++11 -> we use the provided index sequence utilities from the standard header
#    include <utility>
namespace AttachInterruptEx
{
    using std::index_sequence;
    using std::make_index_sequence;
}
#else
/*************************************************************************
 * C++11 doesn't provide index_sequences out of the box.
 * However, the C++14 version works without problems.
 * The following is copied unchanged from the gcc 5.4.1 utility header
 **************************************************************************/

namespace AttachInterruptEx
{
    // Stores a tuple of indices.  Used by tuple and pair, and by bind() to
    // extract the elements in a tuple.
    template <size_t... _Indexes>
    struct _Index_tuple
    {
        typedef _Index_tuple<_Indexes..., sizeof...(_Indexes)> __next;
    };

    // Builds an _Index_tuple<0, 1, 2, ..., _Num-1>.
    template <size_t _Num>
    struct _Build_index_tuple
    {
        typedef typename _Build_index_tuple<_Num - 1>::__type::__next __type;
    };

    template <>
    struct _Build_index_tuple<0>
    {
        typedef _Index_tuple<> __type;
    };

    // Class template integer_sequence
    template <typename _Tp, _Tp... _Idx>
    struct integer_sequence
    {
        typedef _Tp value_type;
        static constexpr size_t size() { return sizeof...(_Idx); }
    };

    template <typename _Tp, _Tp _Num,
              typename _ISeq = typename _Build_index_tuple<_Num>::__type>
    struct _Make_integer_sequence;

    template <typename _Tp, _Tp _Num, size_t... _Idx>
    struct _Make_integer_sequence<_Tp, _Num, _Index_tuple<_Idx...>>
    {
        static_assert(_Num >= 0,
                      "Cannot make integer sequence of negative length");

        typedef integer_sequence<_Tp, static_cast<_Tp>(_Idx)...> __type;
    };

    /// Alias template make_integer_sequence
    template <typename _Tp, _Tp _Num>
    using make_integer_sequence = typename _Make_integer_sequence<_Tp, _Num>::__type;

    /// Alias template index_sequence
    template <size_t... _Idx>
    using index_sequence = integer_sequence<size_t, _Idx...>;

    /// Alias template make_index_sequence
    template <size_t _Num>
    using make_index_sequence = make_integer_sequence<size_t, _Num>;

    // using AttachInterruptEx::index_sequence;
    // using AttachInterruptEx::make_index_sequence;
}
#endif
