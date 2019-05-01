import unittest, unittest.mock

import os, signal

from .raise_signals import Signal, raise_signals


TEST_SIGNAL1 = signal.SIGUSR1
TEST_SIGNAL2 = signal.SIGUSR2

class TestRaiseSignals(unittest.TestCase):
    def test_raises_correctly(self):
        with self.assertRaises(Signal), \
             raise_signals([TEST_SIGNAL1]):
            os.kill(os.getpid(), TEST_SIGNAL1)

    def test_restores_correctly(self):
        handler = unittest.mock.Mock("handler")
        signal.signal(TEST_SIGNAL1, handler)
        with raise_signals([TEST_SIGNAL1]):
            pass
        os.kill(os.getpid(), TEST_SIGNAL1)
        handler.assert_called_once()

    def test_respects_other_signals(self):
        handler = unittest.mock.Mock("handler")
        signal.signal(TEST_SIGNAL1, handler)
        with raise_signals([TEST_SIGNAL2]):
            os.kill(os.getpid(), TEST_SIGNAL1)
        handler.assert_called_once()
