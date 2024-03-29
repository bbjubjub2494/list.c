from __future__ import annotations

from collections import deque
import typing

from hypothesis import assume, given, reproduce_failure, strategies
from hypothesis.stateful import RuleBasedStateMachine, Bundle, rule, invariant, consumes
from _hypothesis_bridge import ffi, lib

import unittest


# list_content
@strategies.composite
def elements(draw):
    return draw(strategies.integers(lib.INTPTR_MIN, lib.INTPTR_MIN))


class alias_property(property):
    def __init__(self):
        super().__init__(self._get, self._set)
    def __set_name__(self, owner, name):
        self.attr_name = name
    def _get(self, owner):
        return getattr(owner._var, self.attr_name)
    def _set(self, owner, new_value):
        setattr(owner._var, self.attr_name, new_value)


class ListSM(RuleBasedStateMachine):
    def __init__(self):
        self._var = ffi.new('struct list*')
        self._model = deque()
        self._model_contents = deque()
        super().__init__()

    first = alias_property()
    last = alias_property()

    def teardown(self):
        lib.list_clear(self._var)

    class Iterator(typing.Iterator[None]):
        cur = alias_property()
        prev = alias_property()

    def _make_iter(self, reverse: bool = False) -> ListSM.Iterator:
        var = ffi.new('struct list_it*')
        var.cur = self.last if reverse else self.first
        def it():
            while var.cur != ffi.NULL:
                yield  # yield None because we're mutable
                lib.list_step(var)
        it = it()
        class _It(ListSM.Iterator):
            _var = var
            def __next__(self) -> None:
                return next(it)
        return _It()

    def __iter__(self) -> ListSM.Iterator:
        return self._make_iter()

    def __reversed__(self) -> ListSM.Iterator:
        return self._make_iter(reverse=True)

    nodes = Bundle('Nodes')

    @rule(new_value=elements(), target=nodes)
    def insert_front(self, new_value):
        self._model_contents.appendleft(new_value)
        lib.list_insert_front(self._var, new_value)
        new_node = self.first
        assert new_node.value == new_value
        self._model.appendleft(new_node)
        return new_node

    @rule(new_value=elements(), target=nodes)
    def insert_back(self, new_value):
        self._model_contents.append(new_value)
        lib.list_insert_back(self._var, new_value)
        new_node = self.last
        assert new_node.value == new_value
        self._model.append(new_node)
        return new_node

    @rule(nodes=strategies.frozensets(consumes(nodes)), reverse=strategies.booleans())
    def remove_thru_iter(self, nodes, reverse):
        it = reversed(self) if reverse else iter(self)
        for _ in it:
            if it.cur in nodes:
                lib.list_remove(self._var, it._var)
        for n in nodes:
            i = self._model.index(n)
            del self._model_contents[i]
            del self._model[i]

    @invariant()
    def nodes_as_model(self):
        it = iter(self)
        nodes = [it.cur for _ in it]
        assert nodes == list(self._model)

    @invariant()
    def contents_as_model(self):
        it = iter(self)
        contents = [it.cur.value for _ in it]
        assert contents == list(self._model_contents)


if typing.TYPE_CHECKING:
    _witness: typing.Reversible[None] = ListSM()


TestList = ListSM.TestCase
