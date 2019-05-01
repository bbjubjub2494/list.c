from contextlib import contextmanager
from signal import *


#: List of the usually core-dumpy signals that are portable.
FATAL_SIGNALS = SIGABRT, SIGFPE, SIGILL, SIGSEGV

@contextmanager
def raise_signals(signals=FATAL_SIGNALS):
    """temporarily set up signal handlers to turn certain signals into
    exceptions."""
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
    """Exception raised when a signal is caught by raise_signals."""
    def __init__(self, sig):
        super().__init__("caught signal", sig)
