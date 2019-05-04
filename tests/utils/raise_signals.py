from contextlib import contextmanager
from signal import *


FATAL_SIGNALS = SIGABRT, SIGFPE, SIGILL, SIGSEGV

@contextmanager
def raise_signals(signals=FATAL_SIGNALS):
    """Context manager to temporarily set up signal handlers to turn certain
    signals into exceptions.

    :param Sequence[signals.Signals] signals:
        list of signals to be handled, by default :data:`FATAL_SIGNALS`.
    :raise Signal: when a signal is received during execution of the context.
    """
    def handler(sig, frame):
        raise Signal(sig)
    old_handlers = {}
    for sig in signals:
        old_handlers[sig] = signal(sig, handler)
    try:
        yield
    finally:
        for sig, handler in old_handlers.items():
            signal(sig, handler)

class Signal(Exception):
    """Exception raised when a signal is caught by :func:`raise_signals`."""
    def __init__(self, sig):
        super().__init__("received signal", sig)
