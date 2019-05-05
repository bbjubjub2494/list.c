from hypothesis import assume, given, reproduce_failure, stateful, strategies
from _hypothesis_bridge import ffi, lib

import unittest


# generate random invalid generic pointers
@strategies.composite
def pointers(draw):
    return ffi.cast('void*', draw(strategies.integers(0, lib.UINTPTR_MAX)))

class PtrMixSM(stateful.RuleBasedStateMachine):
    def __init__(self):
        super().__init__()
        self._var = None
        self._contents = {}

    mixed_pointers = stateful.Bundle("mixed pointers")

    @stateful.initialize(target=mixed_pointers, a=pointers(), b=pointers())
    def mixptr(self, a, b):
        self._var = lib.mixptr(a, b)
        self._contents = {a: b, b: a}
        return stateful.multiple(a, b)

    @stateful.invariant()
    def equation(self):
        # nothing to check if called before initialization.
        if self._contents:
            # make it work if a == b and thus _contents has 1 entry
            contents = list(self._contents)
            a, b = contents[0], contents[-1]
            assert self._var.content == lib.mixptr(a, b).content

    @stateful.invariant()
    def unmixptr(self):
        for ptr in self._contents:
            assert lib.unmixptr(self._var, ptr) == self._contents[ptr]

    @stateful.rule(target=mixed_pointers, a=stateful.consumes(mixed_pointers), b=pointers())
    def remixptr(self, a, b):
        lib.remixptr(ffi.addressof(self._var), a, b)
        a = self._contents[a]
        self._contents = {a: b, b: a}
        return b

TestPtrMix = PtrMixSM.TestCase
