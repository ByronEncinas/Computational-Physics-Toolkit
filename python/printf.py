import ctypes, os, sys

class Toolkit:
    def __init__(self):
        lib_path = os.path.join(os.getcwd(), "build/libcpt.so")
        self.tools = ctypes.CDLL(lib_path)

    def random_between(self, l, u):
        self.tools.rand_within.argtypes = [ctypes.c_float, ctypes.c_float]
        self.tools.rand_within.restype  = ctypes.c_float
        return self.tools.rand_within(l, u)

toolkit = Toolkit()

result = toolkit.random_between(0.0, 1.0)

print(result)