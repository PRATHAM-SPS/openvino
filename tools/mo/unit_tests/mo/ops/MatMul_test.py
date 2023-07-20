# Copyright (C) 2018-2023 Intel Corporation
# SPDX-License-Identifier: Apache-2.0

import unittest

import numpy as np

from openvino.tools.mo.ops.MatMul import MatMul
from openvino.tools.mo.front.common.partial_infer.utils import int64_array, shape_array, dynamic_dimension_value
from openvino.tools.mo.graph.graph import Node
from unit_tests.utils.graph import build_graph_with_attrs


class TestMatMul(unittest.TestCase):
    nodes = [
        ('A', {'type': 'Parameter', 'kind': 'op'}),
        ('A_d', {'kind': 'data'}),
        ('B', {'type': 'Parameter', 'kind': 'op'}),
        ('B_d', {'kind': 'data', 'dim_attrs': []}),
        ('mat_mul', {'type': 'MatMul', 'kind': 'op'}),
        ('mat_mul_d', {'kind': 'data', 'value': None, 'shape': None}),
        ('op_output', {'kind': 'op', 'op': 'Result'}),
    ]
    edges = [
        ('A', 'A_d'),
        ('B', 'B_d'),
        ('A_d', 'mat_mul', {'in': 0}),
        ('B_d', 'mat_mul', {'in': 1}),
        ('mat_mul', 'mat_mul_d'),
        ('mat_mul_d', 'op_output'),
    ]

    def test_positive_matmul_infer(self):
        test_cases=[
        ([1024], [1024, 1000], [1000], False, False),
        ([dynamic_dimension_value], [1024, 1000], [1000], False, False),
        ([1024], [dynamic_dimension_value, 1000], [1000], False, False),
        ([1024], [1024, 1000], [1000], True, False),
        ([1024], [1000, 1024], [1000], True, True),
        ([dynamic_dimension_value], [dynamic_dimension_value, dynamic_dimension_value], [dynamic_dimension_value], True,
         True),
        ([1, 1024], [1024, 1000], [1, 1000], False, False),
        ([1, 1024], [1000, 1024], [1, 1000], False, True),
        ([1024, 1000], [1000], [1024], False, False),
        ([1024, 1000], [1000], [1024], False, True),
        ([1000, 1024], [1000], [1024], True, True),
        ([1000, dynamic_dimension_value], [1000], [dynamic_dimension_value], True, True),
        ([10, 1024], [1024, 1000], [10, 1000], False, False),
        ([5, 10, 1024], [1024, 1000], [5, 10, 1000], False, False),
        ([5, 10, 1024], [5, 1024, 1000], [5, 10, 1000], False, False),
        ([5, 10, 1024], [1, 1024, 1000], [5, 10, 1000], False, False),
        ([5, 10, 1024], [1, 1000, 1024], [5, 10, 1000], False, True),
    ]
        for idx, (A_shape, B_shape, C_shape, transpose_a, transpose_b) in enumerate(test_cases):
            with self.subTest(test_cases=idx):
                graph = build_graph_with_attrs(nodes_with_attrs=self.nodes, edges_with_attrs=self.edges,
                                            update_nodes_attributes=[
                                                ('A_d', {'shape': shape_array(A_shape)}),
                                                ('B_d', {'shape': shape_array(B_shape)}),
                                                ('mat_mul', {'transpose_a': transpose_a, 'transpose_b': transpose_b}),
                                            ])
                node = Node(graph, 'mat_mul')
                MatMul.infer(node)

                msg = "MatMul infer failed for case: A_shape={}, B_shape={}, transpose_a={}, transpose_b={} " \
                    "expected_shape={}, actual_shape={}"

                self.assertTrue(np.array_equal(graph.node['mat_mul_d']['shape'], shape_array(C_shape)),
                                msg.format(A_shape, B_shape, transpose_a, transpose_b, C_shape,
                                        graph.node['mat_mul_d']['shape']))

    def test_negative_matmul_infer(self):
        test_cases=[
        (None, [1024, 1000]),
        (1, [1024, 1000]),
        ([], [1024, 1000]),
        ([1024, 1000], [1024, 1000]),
        ([5, 10, 1024], [3, 1024, 1000]),
    ]
        for idx, (A_shape, B_shape) in enumerate(test_cases):
            with self.subTest(test_cases=idx):
                graph = build_graph_with_attrs(nodes_with_attrs=self.nodes, edges_with_attrs=self.edges,
                                            update_nodes_attributes=[
                                                ('A_d', {'shape': np.array(A_shape)}),
                                                ('B_d', {'shape': int64_array(B_shape)}),
                                            ])

                node = Node(graph, 'mat_mul')
                self.assertRaises(AssertionError, MatMul.infer, node)
