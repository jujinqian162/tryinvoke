
#include "gtest/gtest.h"
#include "tryinvoke.h"
#include <array>
#include <tuple>
#include <utility>
using namespace ju;
TEST(voidinvoke, invoke) {
    int local{4};
    auto f = [] { return 1; };
    auto f2 = [&]()-> int& { return local; };
    static_assert(std::is_lvalue_reference_v<decltype(try_invoke(f2, void_t))>);
    ASSERT_EQ(try_invoke(f, void_t), 1);
    ASSERT_EQ(try_invoke(f, 3, 4, void_t), 1);
    ASSERT_EQ(try_invoke(f2, 3, void_t), 4);
}

TEST(tupleinvoke, invoke) {
    auto f1 = [](int a, double b) { return a + b; };
    auto f2 = [](int) { return 3; };
    auto f3 = [](std::pair<int, double> p) { return p.first; };
    static_assert(concepts::applyable<decltype(f1), std::tuple<int, double>>);
    static_assert(!concepts::applyable<decltype(f2), std::tuple<int, double>>);
    auto res = try_invoke(f1, std::tuple{1, 2.3}, 3, 4);
    auto res2 = try_invoke(f2, std::tuple{3, 4}, 3, 4, void_t);
    auto res3 = try_invoke(f3, 3.4, void_t, std::pair{4, 2.3}, 3, 4);
    static_assert(concepts::tuple_like<std::pair<int, int>>);
    auto res4 = try_invoke(f3, 3.4, std::tuple{3, 5, 2}, std::tuple{410, 2.3},
                           std::pair{410, 5.6}, 4);
    auto res5 =
        try_invoke(f1, std::tuple{std::pair{3, 4}, 4}, std::tuple{4, 5.6}, 5);
    
    ASSERT_EQ(res, 3.3);
    ASSERT_EQ(res2, 3);
    ASSERT_EQ(res3, 4);
    ASSERT_EQ(res4, 410);
    ASSERT_EQ(res5, 9.6);
}

TEST(tupleinvoke, invode){
    auto f1 = [](std::tuple<int , int> t){ return std::get<1>(t); };
    ASSERT_EQ(try_invoke(f1, std::tuple{3, 4}, 2, 5), 4);
}



TEST(sigleinvoke, invoke) {
    auto f1 = [](int) { return 3; };
    auto f2 = [](std::pair<int, double> p) { return p.first + p.second; };
    auto res = try_invoke(f1, void_t, 3, 5);
    auto res2 = try_invoke(f2, std::tuple{3, 4, 5}, std::pair{4, 5});
    ASSERT_EQ(res, 3);
    ASSERT_EQ(res2, 9);
}

TEST(tuple_assertion, assert) {
    static_assert(concepts::is_tuple_v<std::tuple<double, int, int>>);
    static_assert(!concepts::is_tuple_v<std::pair<int, int>>);
    static_assert(concepts::tuple_like<std::pair<int, int>>);
    static_assert(concepts::tuple_like<std::array<int, 4>>);
}

TEST(false_invoke, invoke){
    auto f = []{ return 3;};
    // try_invoke(f, 3);
    // connot complie
}