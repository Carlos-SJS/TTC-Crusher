// cppimport
#include "pybind11/pybind11.h"
#include "player_test.hpp"

namespace py = pybind11;
using namespace pybind11::literals;

PYBIND11_MODULE(Player, m) {
    py::class_<Player>(m, "Player")
        .def(py::init<int>())
        .def("getX", &Player::getX)
        .def("setX", &Player::setX);
}