#include <gismo/Core/Basis/Basis.h>
#include <gismo/Core/Basis/BasisFun.h>

namespace gismo
{
#ifdef GISMO_WITH_PYBIND11

namespace py = pybind11;

void pybind11_init_gsBasisFun(py::module &m)
{
    using Class = gsBasisFun<real_t>;
    py::class_<Class>(m, "gsBasisFun")
        // Member functions
    .def("eval", &Class::eval, "Evaluates points into a matrix")
        ;
}

#endif

}
