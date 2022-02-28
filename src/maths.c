#include <stdio.h>
#include <string.h>
#include <stdlib.h>


enum Operaters {
  plus,
  minus,
  multiply,
  divide
};

enum Operaters get_op(char const *op_string) {
  switch (op_string[0]) {
    case '+': return plus;
    case '-': return minus;
    case '*': return multiply;
    case '/': return divide;
    default: printf("Operater not reconised: %c", op_string[0]); exit(0);
  }
}

int _add(char const *x, char const *y) {
  if (strstr(x, ".") || strstr(y, ".")) {
    double total = atof(x) + atof(y);
    printf("%f", total);
  } else {
    long total = atoi(x) + atoi(y);
    printf("%d", total);
  }
  return 0;
}

int _minus(char const *x, char const *y) {
  if (strstr(x, ".") || strstr(y, ".")) {
    double total = atof(x) - atof(y);
    printf("%f", total);
  } else {
    long total = atoi(x) - atoi(y);
    printf("%d", total);
  }
  return 0;
}

int _multiply(char const *x, char const *y) {
  if (strstr(x, ".") || strstr(y, ".")) {
    double total = atof(x) * atof(y);
    printf("%f", total);
  } else {
    long total = atoi(x) * atoi(y);
    printf("%d", total);
  }
  return 0;
}

int _divide(char const *x, char const *y) {
  if (strstr(x, ".") || strstr(y, ".")) {
    double total = atof(x) / atof(y);
    printf("%f", total);
  } else {
    long total = atoi(x) / atoi(y);
    printf("%d", total);
  }
  return 0;
}

int group_inputs(int argc, char const *argv[]) {
  if (argc == 2) {
    if (strcmp(argv[0], "-") == 0) {
      if (strstr(argv[1], ".")) {
        printf("%f\n", -atof(argv[1]));
      } else {
        printf("%d\n", -atoi(argv[1]));
      }
      return 0;
    }
    printf("The first argument to a call with 2 argumrnts was not \"-\".");
    return 1;
  }

  if (argc == 3) {
    if (strlen(argv[1]) != 1) {
      printf("Bad secound argument for 3 arguments.");
      return 1;
    }

    enum Operaters op = get_op(argv[1]);
    switch (op) {
      case plus: return _add(argv[0], argv[2]);
      case minus: return _minus(argv[0], argv[2]);
      case multiply: return _multiply(argv[0], argv[2]);
      case divide: return _divide(argv[0], argv[2]);
      default: printf("Operater not reconised: %d\n", op); return 1;
    }
  }

  printf("Please use either 2 or 3 arguments. Argument count: %d\n", argc);
  return 1;
}

int main(int argc, char const *argv[]) {
  return group_inputs(argc - 1, argv + 1);
}
