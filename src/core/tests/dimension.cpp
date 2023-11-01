// Copyright (C) 2018-2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "openvino/core/dimension.hpp"

#include "gtest/gtest.h"
#include "openvino/core/dimension_tracker.hpp"

using namespace std;
using namespace ov;

TEST(dimension, broadcast_merge_static_1_and_10) {
    Dimension result;
    Dimension one(1), ten(10);
    bool success = Dimension::broadcast_merge(result, one, ten);

    EXPECT_TRUE(success);
    EXPECT_EQ(result, ten);
}

TEST(dimension, broadcast_merge_static_1_5_and_10_15) {
    Dimension result;
    Dimension one(1, 5), ten(10, 15);
    bool success = Dimension::broadcast_merge(result, one, ten);

    EXPECT_TRUE(success);
    EXPECT_EQ(result, ten);
}

TEST(dimension, broadcast_merge_static_1_12_and_10_15) {
    Dimension result;
    Dimension one(1, 12), ten(10, 15);
    bool success = Dimension::broadcast_merge(result, one, ten);

    EXPECT_TRUE(success);
    EXPECT_EQ(result, ten);
}

TEST(dimension, broadcast_merge_static_7_12_and_10_15) {
    Dimension result;
    Dimension one(7, 12), ten(10, 15);
    bool success = Dimension::broadcast_merge(result, one, ten);

    EXPECT_TRUE(success);
    EXPECT_EQ(result, Dimension(10, 12));
}

TEST(dimension, broadcast_merge_static_0_12_and_1_15) {
    Dimension result;
    Dimension one(0, 12), ten(1, 15);
    bool success = Dimension::broadcast_merge(result, one, ten);

    EXPECT_TRUE(success);
    EXPECT_EQ(result, Dimension(0, 15));
}

TEST(dimension, dimension_mul_operator_ordinary_intervals) {
    Dimension interval_1(0, 10);
    Dimension interval_2(2, 100);
    Dimension ref_value(0, 1000);
    EXPECT_EQ(ref_value, interval_1 * interval_2);
}

TEST(dimension, dimension_mul_operator_1) {
    Dimension fully_dynamic_dim(-1);
    Dimension two(2);
    Dimension ref_value(-1);
    EXPECT_EQ(ref_value, fully_dynamic_dim * two);
}

TEST(dimension, dimension_mul_operator_2) {
    // overflow happens and clip_times keeps result within in64 limits
    // (Interval::s_max - 1) * 2 = 9223372036854775806 * 2 = 18446744073709551612
    // arithmetical result does not fit into int64, is clipped into int64_max
    Dimension large_interval(2, Interval::s_max - 1);
    Dimension two(2);
    Dimension ref_value(4, Interval::s_max);
    EXPECT_EQ(ref_value, large_interval * two);
}

TEST(dimension, dimension_mul_operator_3) {
    // no overflow
    // (int64_max / 2) * 2= 4611686018427387903 * 2 = 9223372036854775806 = int64_max - 1
    Dimension large_interval(2, ov::Interval::s_max / 2);
    Dimension two(2);
    Dimension ref_value(4, ov::Interval::s_max - 1);
    EXPECT_EQ(ref_value, large_interval * two);
}

TEST(dimension, dimension_mul_operator_4) {
    // overflow happens and clip_times keeps result within in64 limits
    // (int64_max / 2 + 1) * 2 = 4611686018427387904 * 2 = 9223372036854775808 = int64_max + 1
    // 9223372036854775808 does not fit into int64, is clipped into int64_max
    Dimension large_interval(2, ov::Interval::s_max / 2 + 1);
    Dimension two(2);
    Dimension ref_value(4, ov::Interval::s_max);
    EXPECT_EQ(ref_value, large_interval * two);
}

TEST(dimension, dimension_mul_operator_5) {
    // (int64_max / 3 + 2) = 3074457345618258604 * 3 = 9223372036854775812 = int64_max + 5
    // overflow happens and clip_times keeps result within in64 limits
    // 9223372036854775812 does not fit into int64, is clipped into int64_max
    Dimension large_interval(2, ov::Interval::s_max / 3 + 2);
    Dimension three(3);
    Dimension ref_value(6, ov::Interval::s_max);
    EXPECT_EQ(ref_value, large_interval * three);
}

TEST(dimension, division_of_static_dims_twenty_three_div_three_eq_seven) {
    Dimension twenty_three(23);
    Dimension::value_type three(3);
    Dimension empty(8, 7);
    EXPECT_EQ(empty, twenty_three / three);
}

TEST(dimension, division_of_static_dims) {
    Dimension seven(7);
    Dimension::value_type four(4);
    Dimension empty(2, 1);
    EXPECT_EQ(seven / four, empty);
}

TEST(dimension, dimension_equality) {
    auto te = make_shared<TableOfEquivalence>();
    DimensionTracker dt(te);

    // labeling dimensions
    PartialShape dimensions = PartialShape::dynamic(5);  // A, B, C, D, E
    for (auto& dimension : dimensions)
        dt.set_up_for_tracking(dimension);

    // checking labels are unique
    for (const auto& dimension : dimensions)
        EXPECT_NE(DimensionTracker::get_label(dimension), no_label);

    for (const auto& lhs : dimensions) {
        for (const auto& rhs : dimensions) {
            if (&lhs == &rhs)
                continue;
            EXPECT_NE(DimensionTracker::get_label(lhs), DimensionTracker::get_label(rhs));
            EXPECT_FALSE(te->are_equal(lhs, rhs));
        }
    }

    te->set_as_equal(dimensions[0], dimensions[1]);  // A == B
    te->set_as_equal(dimensions[3], dimensions[4]);  // D == E
    te->set_as_equal(dimensions[2], dimensions[3]);  // C == D
    te->set_as_equal(dimensions[1], dimensions[2]);  // B == C

    // expected to see A == B == C == D == E
    for (const auto& lhs : dimensions)
        for (const auto& rhs : dimensions)
            EXPECT_TRUE(te->are_equal(lhs, rhs));

    // clear up all the tracking info
    for (auto& dimension : dimensions)
        DimensionTracker::reset_tracking_info(dimension);

    // checking labels are unique
    for (const auto& dimension : dimensions)
        EXPECT_EQ(DimensionTracker::get_label(dimension), no_label);
}
