from .raise_signals import raise_signals, Signal

def load_tests(loader, standard_tests, pattern):
    from . import test_raise_signals
    standard_tests.addTests((
        loader.loadTestsFromModule(test_raise_signals),
    ))
    return standard_tests
