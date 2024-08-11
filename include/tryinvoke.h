#ifndef TRYINVOKE_H
#define TRYINVOKE_H
#define s ::std::
#include "concepts"
#include "tuple"
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>
namespace ju {

namespace detail {
template <typename _Fn, typename _Tuple, std::size_t... _Idx>
constexpr decltype(auto) __apply_impl(_Fn&& __f, _Tuple&& __t,
                                      std::index_sequence<_Idx...>)
requires std::invocable<_Fn,
                        decltype(std::get<_Idx>(std::forward<_Tuple>(__t)))...>
{
    return std::invoke(std::forward<_Fn>(__f),
                       std::get<_Idx>(std::forward<_Tuple>(__t))...);
}

template <typename _Fn, typename _Tuple>
constexpr decltype(auto) apply(_Fn&& __f, _Tuple&& __t)
requires requires {
    ju::detail::__apply_impl(
        std::forward<_Fn>(__f), std::forward<_Tuple>(__t),
        std::make_index_sequence<
            std::tuple_size_v<std::remove_reference_t<_Tuple>>>{});
}
{
    using _Indices = std::make_index_sequence<
        std::tuple_size_v<std::remove_reference_t<_Tuple>>>;
    return ju::detail::__apply_impl(std::forward<_Fn>(__f),
                                    std::forward<_Tuple>(__t), _Indices{});
}
} // namespace detail
namespace concepts {
template <typename T>
concept tuple_like = requires(T tp) {
    typename std::tuple_element_t<0, T>;
    typename std::tuple_size<T>::type;
};

template<typename T>
struct is_ctuple : std::bool_constant<false>{};
template <typename ...E>
struct is_ctuple<std::tuple<E...>> : std::bool_constant<true>{};

template<typename T>
constexpr bool is_tuple_v = is_ctuple<T>::value;

template <typename F, typename T>
concept applyable = requires(F&& fun, T&& tp) {
    { detail::apply(s forward<F>(fun), s forward<T>(tp)) };
};
} // namespace concepts

// use for empty argument invoke
struct Void_t {};
inline constexpr Void_t void_t;

} // namespace ju

struct try_invoker {
    template <typename F, typename... Args>
    requires std::invocable<F>
    constexpr decltype(auto) _try_invoke(F&& func, const ju::Void_t,
                                         Args&&... args) const {
        return func();
    }
    template <typename F>
    constexpr decltype(auto) _try_invoke(F&& func) const {
        static_assert(std::false_type::value, "connot invoke function");
    }

    template <typename F, typename Fst, typename... Args>
    constexpr decltype(auto) _try_invoke(F&& func, Fst&& first,
                                         Args&&... args) const {
        if constexpr (std::invocable<F, Fst&&>) {
            return func(std::forward<Fst>(first));
        } else {
            return _try_invoke(std::forward<F>(func),
                               std::forward<Args>(args)...);
        }
    }
    template <typename F, typename Fst, typename... Args>
    requires ju::concepts::is_tuple_v<std::decay_t<Fst>>
    constexpr decltype(auto) _try_invoke(F&& func, Fst&& first,
                                         Args&&... args) const {
        return _tuple_invoke(s forward<F>(func), s forward<Fst>(first),
                             s forward<Args>(args)...);
    }
    template <typename F, typename T, typename... Args>
    requires ju::concepts::applyable<F, T&&>
    constexpr decltype(auto) _tuple_invoke(F&& func, T&& tuple,
                                           Args&&... args) const {
        return ju::detail::apply(s forward<F>(func), s forward<T>(tuple));
    }

    template <typename F, typename T, typename ...Args>
    requires (s invocable<F, T&&> && !ju::concepts::applyable<F, T&&>)
    constexpr decltype(auto) _tuple_invoke(F&& func, T&& tuple,
                                           Args&&... args) const{
        return func(s forward<T>(tuple));
    }


    template <typename F, typename T, typename... Args>
    requires(!ju::concepts::applyable<s decay_t<F>, s decay_t<T>> && !s invocable<F, T&&>)
    constexpr decltype(auto) _tuple_invoke(F&& func, T&& tuple,
                                           Args&&... args) const {
        return _try_invoke(s forward<F>(func), s forward<Args>(args)...);
    }

    template <typename F, typename... Args>
    constexpr decltype(auto) operator()(F&& f, Args&&... args) const {
        return _try_invoke(s forward<F>(f), s forward<Args>(args)...);
    }
};

inline constexpr try_invoker try_invoke{};

#endif