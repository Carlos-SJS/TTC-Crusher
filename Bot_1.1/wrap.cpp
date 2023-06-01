#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "core.hpp"

namespace py = pybind11;
using namespace pybind11::literals;

PYBIND11_MODULE(PlayerCoreBase, m) {
    py::class_<PlayerCoreBase>(m, "PlayerCoreBase")
        .def(py::init())
        .def("getMove", &PlayerCoreBase::getMove)
        .def("reset", &PlayerCoreBase::reset);
}   