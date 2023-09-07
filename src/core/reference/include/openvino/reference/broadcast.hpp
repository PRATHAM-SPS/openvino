// Copyright (C) 2018-2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include "ngraph/axis_set.hpp"
#include "ngraph/shape.hpp"

namespace ov {
namespace reference {
void broadcast(const char* arg,
               char* out,
               const Shape& in_shape,
               const Shape& out_shape,
               const AxisSet& broadcast_axes,
               size_t elem_size);
}
}  // namespace ov
