.. Copyright (c) 2016, Johan Mabille and Sylvain Corlay

   Distributed under the terms of the BSD 3-Clause License.

   The full license is in the file LICENSE, distributed with this software.

Arrays and tensors
==================

``xtensor-julia`` provides two container types wrapping Julia arrays: ``jlarray`` and ``jltensor``. They are the counterparts
to ``xarray`` and ``xtensor`` containers.

jlarray
-------

Like ``xarray``, ``jlarray`` has a dynamic shape. This means that one can reshape the array on the C++ side and see this
change reflected on the Julia side. ``jlarray`` doesn't make a copy of the shape, but reads the values each time it
is needed. Therefore, if a reference on a ``jlarray`` is kept in the C++ code and the corresponding Julia array is then reshaped
on the Julia side, this modification will reflect in the ``jlarray``.

jltensor
--------

Like ``xtensor``, ``jltensor`` has a static stack-allocated shape. This means that the shape of the Julia array is copied into
the shape of the ``jltensor`` upon creation. As a consequence, reshapes are not reflected across languages. However, this drawback
is offset by a more effective iteration and broadcasting.

