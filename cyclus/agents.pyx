"""A Python interface for Agents and their subclasses."""
from __future__ import print_function, unicode_literals
from libcpp.map cimport map as std_map
from libcpp.set cimport set as std_set
from libcpp.vector cimport vector as std_vector
from libcpp.utility cimport pair as std_pair
from libcpp.string cimport string as std_string
from libcpp cimport bool as cpp_bool

from cpython cimport (PyObject, PyDict_New, PyDict_Contains,
    PyDict_GetItemString, PyDict_SetItemString, PyString_FromString,
    PyBytes_FromString, PyDict_GetItem, PyDict_SetItem)

from cyclus cimport cpp_cyclus
from cyclus cimport lib
from cyclus import lib

from cyclus.typesystem cimport (py_to_any, any_to_py, str_py_to_cpp,
    std_string_to_py, bool_to_py, bool_to_cpp, std_set_std_string_to_py,
    std_set_std_string_to_cpp)
from cyclus cimport typesystem as ts
from cyclus import typesystem as ts

# startup numpy
#cimport numpy as np
#import numpy as np
#import pandas as pd

#np.import_array()
#np.import_ufunc()


cdef cppclass CyclusAgentShim "CyclusAgentShim" (cpp_cyclus.Agent):
    # A C++ class that acts as an Agent. It implements the Agent virtual
    # methods and dispatches the work to a Python/Cython object
    # that is has a reference to, as needed.

    CyclusAgentShim(cpp_cyclus.Context* ctx):  # C++BASES cyclus::Agent(ctx)
        pass

    std_string version():
        rtn = (<object> this.self).version
        return str_py_to_cpp(rtn)

    cpp_cyclus.Agent* Clone():
        cdef lib._Context ctx = lib.Context(init=False)
        (<lib._Context> ctx).ptx = this.context()
        cdef _Agent a = type(<object> this.self)(ctx)
        #a.shim.InitFrom(this)
        # call self clone
        return a.shim

    #void InitFrom(CyclusAgentShim* a):
    #    cpp_cyclus.Agent.InitFrom(a)

    #void InfileToDb(cpp_cyclus.InfileTree*, cpp_cyclus.DbInit)
    #void InitFrom(cpp_cyclus.QueryableBackend*)

    void Snapshot(cpp_cyclus.DbInit di):
        pass

    void InitInv(cpp_cyclus.Inventories& inv):
        pass

    cpp_cyclus.Inventories SnapshotInv():
        cdef cpp_cyclus.Inventories inv = cpp_cyclus.Inventories()
        return inv


cdef class _Agent(lib._Agent):

    def __cinit__(self, lib._Context ctx):
        self.ptx = self.shim = new CyclusAgentShim(ctx.ptx)
        self._free = True
        self.shim.self = <PyObject*> self


class Agent(_Agent, lib.Agent):
    """Python Agent that is subclassable into any kind of agent.

    Parameters
    ----------
    ctx : cyclus.lib.Context
        The simulation execution context.  You don't normally
        need to call the initilaizer.
    """

    def __init__(self, ctx):
        super().__init__(ctx)
        # gather the state variables
        vars = {}
        for name in dir(self):
            attr = getattr(self, name)
            if not isinstance(attr, ts.StateVar):
                continue
            #if attr.alias is None:


