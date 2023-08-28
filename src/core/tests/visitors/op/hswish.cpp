// Copyright (C) 2018-2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "openvino/op/hswish.hpp"

#include "unary_ops.hpp"

using Type = ::testing::Types<UnaryOperatorType<ov::op::v4::HSwish, ov::element::f32>>;

INSTANTIATE_TYPED_TEST_SUITE_P(visitor_without_atrribute, UnaryOperatorVisitor, Type, UnaryOperatorTypeName);
