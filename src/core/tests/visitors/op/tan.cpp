// Copyright (C) 2018-2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "openvino/op/tan.hpp"

#include "unary_ops.hpp"

using Types = ::testing::Types<UnaryOperatorType<ov::op::v0::Tan, ov::element::f32>>;

INSTANTIATE_TYPED_TEST_SUITE_P(visitor_without_attribute, UnaryOperatorVisitor, Types, UnaryOperatorTypeName);
