def load_tests(loader, standard_tests, pattern):
    from . import test_list
    from . import test_ptrmix
    standard_tests.addTests((
        loader.loadTestsFromModule(test_list),
        loader.loadTestsFromModule(test_ptrmix),
    ))
    return standard_tests
