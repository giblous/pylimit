#define Py_LIMITED_API 0x03070000
#include <Python.h>
#include <structmember.h>
#include <iostream>

struct MyClass {
    std::string name;
    MyClass(std::string name) : name(std::move(name)) {}
    std::string hello() const {
        return "Hello, " + name;
    }
};

typedef struct {
    PyObject_HEAD
    MyClass* cpp_obj;
} PyMyClass;

extern "C" {

static void MyClass_dealloc(PyObject* self) {
    const auto* obj = reinterpret_cast<PyMyClass *>(self);
    delete obj->cpp_obj;
    PyObject_Free(self);
}

static PyObject* MyClass_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyMyClass* self = PyObject_New(PyMyClass, type);
    if (!self) return nullptr;
    self->cpp_obj = nullptr;
    return reinterpret_cast<PyObject *>(self);
}

static int MyClass_init(PyObject* self, PyObject* args, PyObject* kwds) {
    const char* name = nullptr;
    if (!PyArg_ParseTuple(args, "s", &name))
        return -1;
    ((PyMyClass*)self)->cpp_obj = new MyClass(name);
    return 0;
}

static PyObject* MyClass_hello(PyObject* self, PyObject* Py_UNUSED(ignored)) {
    MyClass* obj = ((PyMyClass*)self)->cpp_obj;
    return PyUnicode_FromString(obj->hello().c_str());
}

static PyMethodDef MyClass_methods[] = {
    {"hello", (PyCFunction)MyClass_hello, METH_NOARGS, "Say hello"},
    {nullptr, nullptr, 0, nullptr}
};

static PyType_Slot MyClass_slots[] = {
    {Py_tp_dealloc, (void*)MyClass_dealloc},
    {Py_tp_new, (void*)MyClass_new},
    {Py_tp_init, (void*)MyClass_init},
    {Py_tp_methods, (void*)MyClass_methods},
    {0, nullptr}
};

static PyType_Spec MyClass_spec = {
    "my_module.MyClass",
    sizeof(PyMyClass),
    0,
    Py_TPFLAGS_DEFAULT,
    MyClass_slots
};

static PyModuleDef module_def = {
    PyModuleDef_HEAD_INIT,
    "my_module",
    "Example module with Limited API",
    -1,
    nullptr, nullptr, nullptr, nullptr, nullptr
};

PyMODINIT_FUNC PyInit_my_module(void) {
    PyObject* m = PyModule_Create(&module_def);
    if (!m) return nullptr;

    PyObject* MyClassType = PyType_FromSpec(&MyClass_spec);
    if (!MyClassType) return nullptr;

    PyModule_AddObject(m, "MyClass", MyClassType);
    return m;
}

}
