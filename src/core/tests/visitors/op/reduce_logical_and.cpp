// Copyright (C) 2018-2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "openvino/op/reduce_logical_and.hpp"

#include "reduce_ops.hpp"

using Type = ::testing::Types<ReduceOperatorType<ov::op::v1::ReduceLogicalAnd, ov::element::boolean>>;
INSTANTIATE_TYPED_TEST_SUITE_P(attributes_reduce_op, ReduceOperatorVisitor, Type, ReduceOperatorTypeName);
