from collections import deque

from hypothesis import assume, given, reproduce_failure, strategies
from hypothesis.stateful import RuleBasedStateMachine, Bundle, rule, invariant
from _hypothesis_bridge import ffi, lib

import unittest


# list_content
def elements():
    return strategies.integers(0, 2**32)

class ListSM(RuleBasedStateMachine):
    def __init__(self):
        self._var = ffi.new('struct list*')
        self._model = deque()
        super().__init__()

    def teardown(self):
        ffi.clear_list(self._var)

    nodes = Bundle('Nodes')

TestList = ListSM.TestCase
