#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "core.hpp"

namespace py = pybind11;
using namespace pybind11::literals;

PYBIND11_MODULE(PlayerCore, m) {
    py::class_<PlayerCore>(m, "PlayerCore")
        .def(py::init())
        .def("getMove", &PlayerCore::getMove)
        .def("reset", &PlayerCore::reset);
}   