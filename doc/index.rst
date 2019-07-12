Welcome to list.c's documentation!
==================================

.. toctree::
   :maxdepth: 2
   :caption: Contents:



Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`



Testing helpers
===============

This section documents the Python classes that have been developped to help with
testing.


Turning OS signals into Python exceptions
-----------------------------------------

This is useful to still allow generative tests to shrink when a bug in a C
function causes a signal to be received. Use with care because it is powerful.

.. autofunction:: tests.utils.raise_signals(signals=FATAL_SIGNALS)

.. autoclass:: tests.utils.Signal

.. data:: tests.utils.FATAL_SIGNALS

   List of signals that are both (usually) core-dumpy and portable.

Capturing attribute access
--------------------------

.. autoclass:: tests.utils.AttrProxy

.. automethod:: tests.utils.AttrProxy.__getattr__

.. automethod:: tests.utils.AttrProxy.__setattr__

.. automethod:: tests.utils.AttrProxy.unwrap

.. automethod:: tests.utils.AttrProxy.raw_setattr
