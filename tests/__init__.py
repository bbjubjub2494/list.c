def load_tests(loader, standard_tests, pattern):
    from . import test_list
    standard_tests.addTests((
        loader.loadTestsFromModule(test_list),
    ))
    return standard_tests
