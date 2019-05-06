Introduction
============
Xorlists, or enhanced singly linked lists, are doubly linked lists with a bit of a twist. [Naidu2014]_
This project is a C99 implementation of them.

Building
========
Tup_ is being used to manage all build tasks. See Dependencies_, then run ``tup init`` once anywhere in the tree of the project, then simply ``tup`` to update everything and run tests.

.. _Tup: https://gittup.org/tup

Tests
=====
Tests are performed using Hypothesis_ and its rule-based state machines through the Python FFI.

.. _Hypothesis: https://hypothesis.works

Dependencies
============
A C compiler (Clang by default) and Tup are required to build from source. You can use NixPkgs_ to create a lightweight environment that has all the packages in it. Simply run ``nix-shell`` in the top directory where ``shell.nix`` is.

.. _NixPkgs: https://nixos.org/nixpkgs

.. [Naidu2014] D. Naidu and A. Prasad Jr., "Implementation of Enhanced Singly Linked List Equipped with DLL Operations: An Approach towards Enormous Memory Saving", *International Journal of Future Computer and Communication,* vol. 3, no. 2, pp. 99-101 (`online <http://www.ijfcc.org/papers/276-E1045.pdf>`_)
