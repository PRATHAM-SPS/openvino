// Copyright (C) 2018-2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "shared_test_classes/base/ov_subgraph.hpp"

namespace ov {
namespace test {
typedef std::tuple<
    ov::element::Type,          // Model type
    std::vector<InputShape>,    // Input shapes
    bool,                       // Merge repeated
    std::string                 // Device name
> ctcGreedyDecoderParams;

class CTCGreedyDecoderLayerTest
    :  public testing::WithParamInterface<ctcGreedyDecoderParams>,
       virtual public ov::test::SubgraphBaseTest {
public:
    static std::string getTestCaseName(const testing::TestParamInfo<ctcGreedyDecoderParams>& obj);
protected:
    void SetUp() override;
};
}  // namespace test
}  // namespace ov
