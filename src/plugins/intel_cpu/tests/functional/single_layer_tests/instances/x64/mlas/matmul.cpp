// Copyright (C) 2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "single_layer_tests/classes/matmul.hpp"
#include "shared_test_classes/single_layer/mat_mul.hpp"
#include "test_utils/cpu_test_utils.hpp"
#include "test_utils/fusing_test_utils.hpp"

using namespace InferenceEngine;
using namespace CPUTestUtils;
using namespace ngraph::helpers;
using namespace ov::test;

namespace CPULayerTestsDefinitions {
namespace MatMul {
namespace {
#ifdef OV_CPU_WITH_MLAS
std::vector<CPUSpecificParams> filterSpecificParams_MLAS() {
    // replace with mlas primitive type
    std::vector<CPUSpecificParams> specificParams;
    specificParams.push_back(CPUSpecificParams{{}, {}, {"gemm_mlas"}, "gemm_mlas"});
    return specificParams;
}

std::vector<fusingSpecificParams> fusingParamsSet3D_MLAS_smoke {
        emptyFusingSpec,
        fusingBias,
        fusingMultiplyPerChannel
};

const auto testParams3D_MLAS_smoke = ::testing::Combine(::testing::Combine(::testing::ValuesIn(IS3D_smoke()),
                                                        ::testing::Values(ElementType::f32),
                                                        ::testing::Values(ElementType::undefined),
                                                        ::testing::Values(ElementType::undefined),
                                                        ::testing::Values(helpers::InputLayerType::CONSTANT),
                                                        ::testing::Values(ov::test::utils::DEVICE_CPU),
                                                        ::testing::Values(emptyAdditionalConfig())),
                                             ::testing::Values(MatMulNodeType::FullyConnected),
                                             ::testing::ValuesIn(fusingParamsSet3D_MLAS_smoke),
                                             ::testing::ValuesIn(filterSpecificParams_MLAS()));
INSTANTIATE_TEST_SUITE_P(smoke_FC_3D_MLAS, MatMulLayerCPUTest, testParams3D_MLAS_smoke, MatMulLayerCPUTest::getTestCaseName);

std::vector<fusingSpecificParams> fusingParamsSet2D_MLAS_nightly {
        fusingScaleShift
};
const auto testParams2D_MLAS_nightly = ::testing::Combine(::testing::Combine(::testing::ValuesIn(IS2D_nightly()),
                                                                ::testing::Values(ElementType::f32),
                                                                ::testing::Values(ElementType::undefined),
                                                                ::testing::Values(ElementType::undefined),
                                                                ::testing::Values(helpers::InputLayerType::CONSTANT),
                                                                ::testing::Values(ov::test::utils::DEVICE_CPU),
                                                                ::testing::Values(emptyAdditionalConfig())),
                                             ::testing::Values(MatMulNodeType::FullyConnected),
                                             ::testing::ValuesIn(fusingParamsSet2D_MLAS_nightly),
                                             ::testing::ValuesIn(filterSpecificParams_MLAS()));

INSTANTIATE_TEST_SUITE_P(nightly_FC_2D_MLAS, MatMulLayerCPUTest, testParams2D_MLAS_nightly, MatMulLayerCPUTest::getTestCaseName);

std::vector<fusingSpecificParams> fusingParamsSet2D_MLAS_smoke {
        emptyFusingSpec,
        fusingBias,
        fusingMultiplyPerChannel
};

const auto testParams2D_MLAS_smoke = ::testing::Combine(::testing::Combine(::testing::ValuesIn(IS2D_smoke()),
                                                                ::testing::Values(ElementType::f32),
                                                                ::testing::Values(ElementType::undefined),
                                                                ::testing::Values(ElementType::undefined),
                                                                ::testing::Values(helpers::InputLayerType::CONSTANT),
                                                                ::testing::Values(ov::test::utils::DEVICE_CPU),
                                                                ::testing::Values(emptyAdditionalConfig())),
                                             ::testing::Values(MatMulNodeType::FullyConnected),
                                             ::testing::ValuesIn(fusingParamsSet2D_MLAS_smoke),
                                             ::testing::ValuesIn(filterSpecificParams_MLAS()));
INSTANTIATE_TEST_SUITE_P(smoke_FC_2D_MLAS, MatMulLayerCPUTest, testParams2D_MLAS_smoke, MatMulLayerCPUTest::getTestCaseName);
#endif
} // namespace
} // namespace MatMul
} // namespace CPULayerTestsDefinitions