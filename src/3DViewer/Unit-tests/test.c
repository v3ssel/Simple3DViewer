#include <check.h>

#include "../CPart/parser.h"

START_TEST(test_1) {
  char file_path[30] = "objs/CUBE.obj";
  size_t z = 0;
  double *ver_cub = (double *)calloc(3, sizeof(double));
  parseVertex(file_path, &ver_cub, &z);
  double vertex[24] = {1.000000,  1.000000,  -1.000000, 1.000000,  -1.000000,
                       -1.000000, 1.000000,  1.000000,  1.000000,  1.000000,
                       -1.000000, 1.000000,  -1.000000, 1.000000,  -1.000000,
                       -1.000000, -1.000000, -1.000000, -1.000000, 1.000000,
                       1.000000,  -1.000000, -1.000000, 1.000000};
  for (size_t i = 0; i < 24; i++) {
    ck_assert_double_eq_tol(ver_cub[i], vertex[i], 1);
  }
  free(ver_cub);
}
END_TEST

START_TEST(test_2) {
  char file_path[30] = "objs/CUBE.obj";
  size_t z = 0;
  int *indices = (int *)calloc(1, sizeof(int));
  parseIndices(file_path, &indices, &z);
  int indices_1[] = {1, 5, 5, 7, 7, 3, 3, 1, 4, 3, 3, 7, 7, 8, 8, 4,
                     8, 7, 7, 5, 5, 6, 6, 8, 6, 2, 2, 4, 4, 8, 8, 6,
                     2, 1, 1, 3, 3, 4, 4, 2, 6, 5, 5, 1, 1, 2, 2, 6};
  for (size_t i = 0; i < 48; i++) {
    ck_assert_int_eq(indices[i], indices_1[i]);
  }
  free(indices);
}
END_TEST

START_TEST(test_3) {
  char file_path[30] = "objs/CUBE.obj";
  size_t z = 0;
  double *vertex = (double *)calloc(1, sizeof(double));
  parseVertex(file_path, &vertex, &z);
  move_x_y_z(&vertex, z, 1, 1, 1);
  double vertex_1[] = {2.000000, 2.000000, 0.000000, 2.000000, 0.000000,
                       0.000000, 2.000000, 2.000000, 2.000000, 2.000000,
                       0.000000, 2.000000, 0.000000, 2.000000, 0.000000,
                       0.000000, 0.000000, 0.000000, 0.000000, 2.000000,
                       2.000000, 0.000000, 0.000000, 2.000000};
  for (size_t i = 0; i < 24; i++) {
    ck_assert_int_eq(vertex[i], vertex_1[i]);
  }
  free(vertex);
}
END_TEST

START_TEST(test_4) {
  char file_path[30] = "objs/CUBE.obj";
  size_t z = 0;
  double *vertex = (double *)calloc(1, sizeof(double));
  parseVertex(file_path, &vertex, &z);
  scale(&vertex, z, 1.5);
  double vertex_1[] = {1.500000,  1.500000,  -1.500000, 1.500000,  -1.500000,
                       -1.500000, 1.500000,  1.500000,  1.500000,  1.500000,
                       -1.500000, 1.500000,  -1.500000, 1.500000,  -1.500000,
                       -1.500000, -1.500000, -1.500000, -1.500000, 1.500000,
                       1.500000,  -1.500000, -1.500000, 1.500000};
  for (size_t i = 0; i < 24; i++) {
    ck_assert_int_eq(vertex[i], vertex_1[i]);
  }
  free(vertex);
}
END_TEST

START_TEST(test_5) {
  char file_path[30] = "objs/CUBE.obj";
  size_t z = 0;
  double *vertex = (double *)calloc(1, sizeof(double));
  parseVertex(file_path, &vertex, &z);
  rotate_x_y_z(&vertex, 'y', 15, z);
  double vertex_1[] = {1.000000,  1.000000,  -1.000000, -0.109400, -1.000000,
                       1.409976,  -1.409976, 1.000000,  -0.109400, -1.409976,
                       -1.000000, -0.109400, 1.409976,  1.000000,  0.109400,
                       1.409976,  -1.000000, 0.109400,  0.109400,  1.000000,
                       -1.409976, 0.109400,  -1.000000, -1.409976};
  for (size_t i = 0; i < 24; i++) {
    ck_assert_int_eq(vertex[i], vertex_1[i]);
  }
  free(vertex);
}
END_TEST

START_TEST(test_6) {
  char file_path[30] = "objs/CUBE.obj";
  size_t z = 0;
  double *vertex = (double *)calloc(1, sizeof(double));
  parseVertex(file_path, &vertex, &z);
  rotate_x_y_z(&vertex, 'x', 15, z);
  double vertex_1[] = {1.000000,  1.000000,  -1.000000, 1.000000,  1.409976,
                       0.109400,  1.000000,  -1.409976, -0.109400, 1.000000,
                       0.109400,  -1.409976, -1.000000, -0.109400, 1.409976,
                       -1.000000, 1.409976,  0.109400,  -1.000000, -1.409976,
                       -0.109400, -1.000000, 0.109400,  -1.409976};
  for (size_t i = 0; i < 24; i++) {
    ck_assert_int_eq(vertex[i], vertex_1[i]);
  }
  free(vertex);
}
END_TEST

START_TEST(test_7) {
  char file_path[30] = "objs/CUBE.obj";
  size_t z = 0;
  double *vertex = (double *)calloc(1, sizeof(double));
  parseVertex(file_path, &vertex, &z);
  rotate_x_y_z(&vertex, 'z', 15, z);
  double vertex_1[] = {1.000000,  1.000000,  -1.000000, -0.109400, 1.409976,
                       -1.000000, -1.409976, -0.109400, 1.000000,  -0.109400,
                       1.409976,  1.000000,  0.109400,  -1.409976, -1.000000,
                       1.409976,  0.109400,  -1.000000, 0.109400,  -1.409976,
                       1.000000,  1.409976,  0.109400,  1.000000};
  for (size_t i = 0; i < 24; i++) {
    ck_assert_int_eq(vertex[i], vertex_1[i]);
  }
  free(vertex);
}
END_TEST

int main(void) {
  Suite *s1 = suite_create("Core");
  TCase *tc1_1 = tcase_create("Core");
  SRunner *sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, test_1);
  tcase_add_test(tc1_1, test_2);
  tcase_add_test(tc1_1, test_3);
  tcase_add_test(tc1_1, test_4);
  tcase_add_test(tc1_1, test_5);
  tcase_add_test(tc1_1, test_6);
  tcase_add_test(tc1_1, test_7);
  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
