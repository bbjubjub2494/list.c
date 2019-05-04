from .raise_signals import FATAL_SIGNALS, raise_signals, Signal

def load_tests(loader, standard_tests, pattern):
    from . import test_raise_signals
    import doctest, importlib
    standard_tests.addTests((
        loader.loadTestsFromModule(test_raise_signals),
        doctest.DocTestSuite(importlib.import_module('.raise_signals', package=__name__)),
    ))
    return standard_tests
