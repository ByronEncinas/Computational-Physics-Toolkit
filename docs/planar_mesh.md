
# Formats for nodes and elements

inp

  *NODE
  1,  0.0, 0.0, 0.0
  2,  1.0, 0.0, 0.0
  3,  1.0, 1.0, 0.0
  4,  0.0, 1.0, 0.0
  *ELEMENT, TYPE=CPS3, ELSET=TRIANGLES
  1,  1, 2, 3
  2,  1, 3, 4

bdf

  GRID,1,,0.0,0.0,0.0
  GRID,2,,1.0,0.0,0.0
  GRID,3,,1.0,1.0,0.0
  GRID,4,,0.0,1.0,0.0
  CTRIA3,1,1,1,2,3
  CTRIA3,2,1,1,3,4
