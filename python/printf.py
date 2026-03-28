import ctypes

lib = ctypes.CDLL('/home/leni/Computational-Physics-Toolkit/build/libcpt.so')

# void mesh_from_file(){

lib.mesh_from_file.argtypes = []

lib.mesh_from_file()
