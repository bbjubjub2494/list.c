What's this?
============

*Xorlists*, or enhanced singly linked lists, are doubly linked lists with a bit of a twist. [Naidu2014]_
This project is a C99 implementation of them.


Building
========

Tup_ is being used to manage all build tasks. See Dependencies_, then just run ``tup`` anywhere in the tree of the project to update everything and run tests.


Tests
=====

Tests are performed using Hypothesis_ and its rule-based state machines through the Python FFI.


Dependencies
============

A C compiler (Clang_ by default) and Tup_ are required to build from source. You can use Nix_ to create a lightweight environment that has all the packages in it. Simply run ``nix-shell`` in the top directory where ``shell.nix`` is. Also, a very convenient one-liner is ``nix-shell --run tup``.


.. _Clang: https://clang.llvm.org
.. _Nix: https://nixos.org/nix/
.. _NixPkgs: https://nixos.org/nixpkgs/
.. _Hypothesis: https://hypothesis.works
.. _Tup: http://gittup.org/tup/

.. [Naidu2014] D. Naidu and A. Prasad Jr., "Implementation of Enhanced Singly Linked List Equipped with DLL Operations: An Approach towards Enormous Memory Saving", *International Journal of Future Computer and Communication,* vol. 3, no. 2, pp. 99-101 (`online <http://www.ijfcc.org/papers/276-E1045.pdf>`_)
