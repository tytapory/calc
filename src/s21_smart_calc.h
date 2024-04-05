#ifndef APP_H
#define APP_C

#include <cairo.h>
#include <gtk/gtk.h>
#include <stdlib.h>

#include "calc/calc.h"
#include "calc/expr_validation_and_transformation/expr_validation_and_transformation.h"
#define GRAPH_LIMITS 1000
#define GRAPH_WINDOW_SIZE 200
typedef struct calculatorObjects {
  GtkWidget* mainWindow;
  GtkWidget* graphWindow;
  GtkWidget* view;
  GtkTextBuffer* buffer;
  GtkWidget* grid;
  GtkWidget* spin_button;
  GtkWidget* button_1;
  GtkWidget* button_2;
  GtkWidget* button_3;
  GtkWidget* button_4;
  GtkWidget* button_5;
  GtkWidget* button_6;
  GtkWidget* button_7;
  GtkWidget* button_8;
  GtkWidget* button_9;
  GtkWidget* button_0;
  GtkWidget* button_eq;
  GtkWidget* button_plus;
  GtkWidget* button_minus;
  GtkWidget* button_mul;
  GtkWidget* button_div;
  GtkWidget* button_del;
  GtkWidget* button_mod;
  GtkWidget* button_left_bracket;
  GtkWidget* button_right_bracket;
  GtkWidget* button_pow;
  GtkWidget* button_dot;
  GtkWidget* button_sin;
  GtkWidget* button_cos;
  GtkWidget* button_tan;
  GtkWidget* button_acos;
  GtkWidget* button_asin;
  GtkWidget* button_atan;
  GtkWidget* button_sqrt;
  GtkWidget* button_log10;
  GtkWidget* button_log;
  GtkWidget* button_x;
  GtkWidget* drawing_area;
  GtkAdjustment* adjustment;
  double* dots_array;
  int domain;
  int codomain;
  int multiply;
} calculatorObjects;

void remove_zeros(char* numberString);
char* get_text_of_textview(GtkWidget* text_view);
void eq_button_solve(calculatorObjects* calculator_objects);
void delete_sym(GtkWidget* view);
void add_to_expression(GtkWidget* button, GtkWidget* view);
void on_activate(GtkApplication* app);
void initialize_calculator_objects(calculatorObjects* calculator_objects,
                                   GtkApplication* app);
void initialize_signals(calculatorObjects* calculator_objects);
void initialize_grid(calculatorObjects* calculator_objects);
int is_unsignificant_zero_or_dot(char digit, int i, int dotFound);
void calculate_or_draw_graph_depending_on_x(
    GtkTextBuffer* buffer, char* expression,
    calculatorObjects* calculator_objects);
void draw_graph(char* expression, calculatorObjects* calculator_objects);
void show_window_with_graph(calculatorObjects* calculator_objects);
int get_y_coordinate(double highest_Y, double lowest_Y, double dot);
void destroy_calculator_objects(calculatorObjects* calculator_objects);

#endif