#include <math.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parseVertex(char const *path_to_file, double **vertex, size_t *i);
void parseIndices(char const *path_to_file, int **indices, size_t *z);
void move_x_y_z(double **vertex, size_t i, double x1, double y1, double z1);
void scale(double **vertex, size_t i, double c);
void rotate_x_y_z(double **vertex, char coordinate, double angle, size_t i);
