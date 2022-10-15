#include "parser.h"

void parseVertex(char const *path_to_file, double **vertex, size_t *i) {
  FILE *file = fopen(path_to_file, "r+");
  if (file) {
    size_t in_i = *i, len = 0;
    char *current_string = NULL;
    while ((getline(&current_string, &len, file)) != -1) {
      for (size_t d = 0; d < strlen(current_string); d++) {
        if (current_string[0] == 'v' && current_string[1] == ' ') {
          if (current_string[d] == ' ' &&
              ((current_string[d + 1] >= '0' && current_string[d + 1] <= '9') ||
               current_string[d + 1] == '-')) {
            *vertex = realloc(*vertex, sizeof(double) * (in_i + 1));
            char number[50];
            size_t l = 0;
            d++;
            while (current_string[d] != ' ' && current_string[d] != '\n') {
              number[l] = current_string[d];
              l++, d++;
            }
            d--;
            double ver = strtod(number, NULL);
            (*vertex)[in_i] = ver;
            in_i++;
          }
        }
      }
    }
    *i = in_i;
    fclose(file);
  }
}

void parseIndices(char const *path_to_file, int **indices, size_t *z) {
  char regex[1][3] = {{"f "}};
  char *current_string = NULL;
  regex_t compiled;
  size_t len = 0;
  regmatch_t matchptr;
  FILE *file = fopen(path_to_file, "r+");
  regcomp(&compiled, regex[0], REG_EXTENDED);
  size_t index = *z;
  if (file) {
    int last_elem = 0;
    while ((getline(&current_string, &len, file)) != -1) {
      if ((regexec(&compiled, current_string, 1, &matchptr, 0)) == 0) {
        int counter = 0;
        for (size_t i = 0; i < strlen(current_string); i++) {
          if (current_string[i] == ' ' &&
              (current_string[i + 1] >= '0' && current_string[i + 1] <= '9')) {
            *indices = realloc(*indices, sizeof(int) * (index + 2));
            int current_num;
            char num[50] = {'0'};
            i++;
            int j = 0;
            while (current_string[i] != '/' && current_string[i] != ' ' &&
                   current_string[i] != '\n') {
              num[j] = current_string[i];
              j++, i++;
            }
            current_num = atoi(num);
            if (counter == 0) {
              last_elem = current_num;
              (*indices)[index] = current_num;
              index++;
            } else {
              (*indices)[index] = current_num;
              (*indices)[index + 1] = current_num;
              (*indices)[index + 2] = last_elem;
              index += 2;
            }
            counter++;
          }
        }
        index++;
      }
    }
  }
  *z = index;
  free(current_string);
  regfree(&compiled);
  fclose(file);
}

void move_x_y_z(double **vertex, size_t i, double x1, double y1, double z1) {
  static double diff_1 = 0;
  static double diff_2 = 0;
  static double diff_3 = 0;
  for (size_t x = 0; x < i; x += 3) {
    (*vertex)[x] += x1 - diff_1;
    (*vertex)[x + 1] += y1 - diff_2;
    (*vertex)[x + 2] += z1 - diff_3;
  }
  diff_1 = x1;
  diff_2 = y1;
  diff_3 = z1;
}

void scale(double **vertex, size_t i, double c) {
  static double prev = 1.;
  if (c == 0.) c = 1;
  for (size_t x = 0; x < i; ++x) {
    (*vertex)[x] = (*vertex)[x] / prev * c;
  }
  prev = c;
}

void rotate_x_y_z(double **vertex, char coordinate, double angle, size_t i) {
  int a = 0;
  int b = 0;
  double prev_angle;
  static double prev_angle_x = 0;
  static double prev_angle_y = 0;
  static double prev_angle_z = 0;
  switch (coordinate) {
    case 'x':
      prev_angle = prev_angle_x;
      a = 1;
      b = 2;
      break;
    case 'y':
      prev_angle = prev_angle_y;
      a = 0;
      b = 2;
      break;
    case 'z':
      prev_angle = prev_angle_z;
      a = 0;
      b = 1;
      break;
  }
  for (size_t x = 3; x < i; x += 3) {
    double temp_1 = (*vertex)[x + a];
    double temp_2 = (*vertex)[x + b];
    (*vertex)[x + a] =
        cos(angle - prev_angle) * temp_1 - sin(angle - prev_angle) * temp_2;
    (*vertex)[x + b] =
        sin(angle - prev_angle) * temp_1 + cos(angle - prev_angle) * temp_2;
  }
  if (coordinate == 'x') prev_angle_x = angle;
  if (coordinate == 'y') prev_angle_y = angle;
  if (coordinate == 'z') prev_angle_z = angle;
}
