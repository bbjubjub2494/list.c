import unittest, unittest.mock

import os, signal

from .attr_proxy import AttrProxy


class TestRaiseSignals(unittest.TestCase):
    class Subclass(AttrProxy):
        pass

    def setUp(self):
        self.subject = unittest.mock.Mock()
        self.proxy = self.Subclass(self.subject)

    def test_getattrs_correctly(self):
        sentinel = object()
        self.subject.a = sentinel
        self.assertIs(self.proxy.a, sentinel)

    def test_setattrs_correctly(self):
        sentinel = object()
        self.proxy.a = sentinel
        self.assertIs(self.subject.a, sentinel)
