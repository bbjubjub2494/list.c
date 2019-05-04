Welcome to utils's documentation!
=================================

.. toctree::
   :maxdepth: 2
   :caption: Contents:



Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`


Turning OS signals into Python Exception
----------------------------------------

This is useful to still allow generative tests to shrink when a bug in a C
function causes a signal to be received. Use with care because it is powerful.

.. autofunction:: utils.raise_signals(signals=FATAL_SIGNALS)

.. autoclass:: utils.Signal

.. data:: utils.FATAL_SIGNALS

   List of signals that are both (usually) core-dumpy and portable.
