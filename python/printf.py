# this file remains as it is, and 

import ctypes, os

path = os.getcwd()



lib = ctypes.CDLL(f'/home/leni/Computational-Physics-Toolkit/build/libcpt.so')

# void mesh_from_file(){

lib.mesh_from_file.argtypes = []

lib.mesh_from_file()
