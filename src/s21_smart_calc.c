#include "s21_smart_calc.h"

int main(int argc, char **argv) {
  GtkApplication *app = gtk_application_new("com.tytapory.s21_smart_calc",
                                            G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
  return g_application_run(G_APPLICATION(app), argc, argv);
}

void on_activate(GtkApplication *app) {
  calculatorObjects *calculator_objects = malloc(sizeof(calculatorObjects));
  initialize_calculator_objects(calculator_objects, app);
  gtk_text_buffer_set_text(calculator_objects->buffer, "", -1);
  gtk_container_add(GTK_CONTAINER(calculator_objects->mainWindow),
                    calculator_objects->grid);
  gtk_container_set_border_width(GTK_CONTAINER(calculator_objects->mainWindow),
                                 10);
  initialize_signals(calculator_objects);
  initialize_grid(calculator_objects);
  gtk_widget_show_all(calculator_objects->mainWindow);
}

void on_graph_window_close(GtkWidget *widget, gpointer data) {
  calculatorObjects *calculator_objects = (calculatorObjects *)data;
  calculator_objects->drawing_area = NULL;
  gtk_widget_destroy(widget);
  calculator_objects->graphWindow = NULL;
}

void on_main_window_close(GtkWidget *widget, gpointer data) {
  calculatorObjects *calculator_objects = (calculatorObjects *)data;
  gtk_widget_destroy(widget);
  destroy_calculator_objects(calculator_objects);
}

void on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data) {
  calculatorObjects *calculator_objects = (calculatorObjects *)data;
  double lowest_Y = calculator_objects->codomain,
         highest_Y = -calculator_objects->codomain;
  cairo_set_line_width(cr, 1);
  cairo_set_source_rgb(cr, 1, 0, 1);
  int lineDrawn = 0;
  for (int i = 0; i < GRAPH_WINDOW_SIZE; i += 1) {
    if (calculator_objects->dots_array[i] > calculator_objects->codomain ||
        calculator_objects->dots_array[i] < -calculator_objects->codomain ||
        calculator_objects->dots_array[i] == -INFINITY ||
        calculator_objects->dots_array[i] == INFINITY ||
        isnan(calculator_objects->dots_array[i]))
      lineDrawn = 0;
    else {
      if (lineDrawn == 0)
        cairo_move_to(cr, i,
                      get_y_coordinate(highest_Y, lowest_Y,
                                       calculator_objects->dots_array[i]));
      else {
        cairo_line_to(cr, i,
                      get_y_coordinate(highest_Y, lowest_Y,
                                       calculator_objects->dots_array[i]));
      }
      lineDrawn = 1;
    }
  }
  cairo_stroke(cr);
  cairo_set_line_width(cr, 1);
  cairo_set_source_rgb(cr, 0, 0, 1);
  cairo_move_to(cr, 0, GRAPH_WINDOW_SIZE / 2);
  cairo_line_to(cr, GRAPH_WINDOW_SIZE, GRAPH_WINDOW_SIZE / 2);
  cairo_move_to(cr, GRAPH_WINDOW_SIZE / 2, 0);
  cairo_line_to(cr, GRAPH_WINDOW_SIZE / 2, GRAPH_WINDOW_SIZE);
  cairo_stroke(cr);
  gtk_widget_show_all(widget);
}

int get_y_coordinate(double highest_Y, double lowest_Y, double dot) {
  return GRAPH_WINDOW_SIZE -
         ((highest_Y - dot) / (double)((double)((double)highest_Y - lowest_Y) /
                                       (double)GRAPH_WINDOW_SIZE));
}

void show_window_with_graph(calculatorObjects *calculator_objects) {
  calculator_objects->multiply = 1;
  if (calculator_objects->graphWindow != NULL) {
    on_graph_window_close(calculator_objects->graphWindow, calculator_objects);
  }
  calculator_objects->drawing_area = gtk_drawing_area_new();
  g_signal_connect(G_OBJECT(calculator_objects->drawing_area), "draw",
                   G_CALLBACK(on_draw_event), calculator_objects);
  calculator_objects->graphWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(calculator_objects->graphWindow, "destroy",
                   G_CALLBACK(on_graph_window_close), calculator_objects);
  gtk_container_add(GTK_CONTAINER(calculator_objects->graphWindow),
                    calculator_objects->drawing_area);
  gtk_widget_show_all(calculator_objects->graphWindow);
}

void add_to_expression(GtkWidget *button, GtkWidget *view) {
  char *viewText = get_text_of_textview(view);
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
  const char *text = gtk_button_get_label(GTK_BUTTON(button));
  strcat(viewText, text);
  gtk_text_buffer_set_text(buffer, viewText, -1);
}

void delete_sym(GtkWidget *view) {
  char *viewText = get_text_of_textview(view);
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
  int len = (int)strlen(viewText);
  if (len > 0) {
    viewText[(int)strlen(viewText) - 1] = 0;
    gtk_text_buffer_set_text(buffer, viewText, -1);
  }
}

char *get_text_of_textview(GtkWidget *text_view) {
  GtkTextIter start, end;
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
  gchar *text;
  gtk_text_buffer_get_bounds(buffer, &start, &end);
  text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
  return text;
}

void remove_zeros(char *numberString) {
  int i = strlen(numberString) - 1, dotFound = 0;
  for (; is_unsignificant_zero_or_dot(numberString[i], i, dotFound); i--) {
    if (numberString[i] == '.') dotFound = 1;
    numberString[i] = 0;
  }
}

int is_unsignificant_zero_or_dot(char digit, int i, int dotFound) {
  return ((digit == '0' || digit == '.') && i >= 0 && dotFound == 0);
}

void eq_button_solve(calculatorObjects *calculator_objects) {
  char *expression = get_text_of_textview(calculator_objects->view);
  GtkTextBuffer *buffer =
      gtk_text_view_get_buffer(GTK_TEXT_VIEW(calculator_objects->view));
  if (validate(expression))
    calculate_or_draw_graph_depending_on_x(buffer, expression,
                                           calculator_objects);
  else {
    char result[270] = "Invalid expression";
    gtk_text_buffer_set_text(buffer, result, -1);
  }
}

void calculate_or_draw_graph_depending_on_x(
    GtkTextBuffer *buffer, char *expression,
    calculatorObjects *calculator_objects) {
  char result[270] = "";
  if (get_x_index(expression) != -1)
    draw_graph(expression, calculator_objects);
  else {
    transform(expression);
    double solution = solve(expression);
    if (isnan(solution))
      sprintf(result, "NaN");
    else
      sprintf(result, "%lf", solution);
    remove_zeros(result);
    gtk_text_buffer_set_text(buffer, result, -1);
  }
}

void replace_char_with_num(char *expression, int index, double num) {
  char int_string[100] = "";
  sprintf(int_string, "(%lf)", num);
  for (int i = index; i < (int)strlen(expression) - 1; i++)
    expression[i] = expression[i + 1];
  for (int i = 0; i < (int)strlen(int_string); i++) {
    for (int j = (int)strlen(expression) - 1; j >= index; j--)
      expression[j + 1] = expression[j];
    expression[index] = int_string[i];
    index++;
  }
  expression[strlen(expression) - 1] = 0;
}

double solve_and_change_x_to_number(char *expression, double number) {
  char expression_copy[300] = "";
  strcpy(expression_copy, expression);
  int xIndex = get_x_index(expression_copy);
  while (xIndex != -1) {
    replace_char_with_num(expression_copy, xIndex, number);
    xIndex = get_x_index(expression_copy);
  }
  transform(expression_copy);
  return solve(expression_copy);
}

void draw_graph(char *expression, calculatorObjects *calculator_objects) {
  calculator_objects->domain = gtk_spin_button_get_value_as_int(
      GTK_SPIN_BUTTON(calculator_objects->spin_button));
  char expression_copy[2000] = "";
  strcpy(expression_copy, expression);
  double step = (double)calculator_objects->domain * 2 / GRAPH_WINDOW_SIZE;
  for (int i = -GRAPH_WINDOW_SIZE / 2; i < GRAPH_WINDOW_SIZE / 2; i++)
    calculator_objects->dots_array[i + GRAPH_WINDOW_SIZE / 2] =
        solve_and_change_x_to_number(expression_copy, step * i);
  show_window_with_graph(calculator_objects);
}

void initialize_calculator_objects(calculatorObjects *calculator_objects,
                                   GtkApplication *app) {
  calculator_objects->adjustment =
      gtk_adjustment_new(1, 1, GRAPH_LIMITS, 1, 10, 10);
  calculator_objects->multiply = 1;
  calculator_objects->domain = 5;
  calculator_objects->codomain = 10;
  calculator_objects->dots_array =
      calloc((GRAPH_WINDOW_SIZE * 2) + 3, sizeof(double));
  calculator_objects->mainWindow = gtk_application_window_new(app);
  calculator_objects->graphWindow = NULL;
  calculator_objects->drawing_area = gtk_drawing_area_new();
  calculator_objects->spin_button =
      gtk_spin_button_new(calculator_objects->adjustment, 1, 0);
  calculator_objects->view = gtk_text_view_new();
  calculator_objects->buffer =
      gtk_text_view_get_buffer(GTK_TEXT_VIEW(calculator_objects->view));
  calculator_objects->grid = gtk_grid_new();
  calculator_objects->button_1 = gtk_button_new_with_label("1");
  calculator_objects->button_2 = gtk_button_new_with_label("2");
  calculator_objects->button_3 = gtk_button_new_with_label("3");
  calculator_objects->button_4 = gtk_button_new_with_label("4");
  calculator_objects->button_5 = gtk_button_new_with_label("5");
  calculator_objects->button_6 = gtk_button_new_with_label("6");
  calculator_objects->button_7 = gtk_button_new_with_label("7");
  calculator_objects->button_8 = gtk_button_new_with_label("8");
  calculator_objects->button_9 = gtk_button_new_with_label("9");
  calculator_objects->button_0 = gtk_button_new_with_label("0");
  calculator_objects->button_dot = gtk_button_new_with_label(".");
  calculator_objects->button_eq = gtk_button_new_with_label("=");
  calculator_objects->button_plus = gtk_button_new_with_label("+");
  calculator_objects->button_minus = gtk_button_new_with_label("-");
  calculator_objects->button_mul = gtk_button_new_with_label("*");
  calculator_objects->button_div = gtk_button_new_with_label("/");
  calculator_objects->button_del = gtk_button_new_with_label("<-");
  calculator_objects->button_mod = gtk_button_new_with_label("%");
  calculator_objects->button_left_bracket = gtk_button_new_with_label("(");
  calculator_objects->button_right_bracket = gtk_button_new_with_label(")");
  calculator_objects->button_pow = gtk_button_new_with_label("^");
  calculator_objects->button_cos = gtk_button_new_with_label("cos(");
  calculator_objects->button_sin = gtk_button_new_with_label("sin(");
  calculator_objects->button_tan = gtk_button_new_with_label("tan(");
  calculator_objects->button_acos = gtk_button_new_with_label("acos(");
  calculator_objects->button_asin = gtk_button_new_with_label("asin(");
  calculator_objects->button_atan = gtk_button_new_with_label("atan(");
  calculator_objects->button_sqrt = gtk_button_new_with_label("sqrt(");
  calculator_objects->button_log10 = gtk_button_new_with_label("ln(");
  calculator_objects->button_log = gtk_button_new_with_label("log(");
  calculator_objects->button_x = gtk_button_new_with_label("x");
}

void initialize_signals(calculatorObjects *calculator_objects) {
  g_signal_connect(calculator_objects->button_1, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_2, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_3, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_4, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_5, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_6, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_7, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_8, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_9, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_0, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_dot, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_mul, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_div, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_minus, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_plus, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_mod, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_left_bracket, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_right_bracket, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_pow, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_sin, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_cos, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_tan, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_acos, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_asin, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_atan, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_sqrt, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_log10, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_log, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect(calculator_objects->button_x, "clicked",
                   G_CALLBACK(add_to_expression), calculator_objects->view);
  g_signal_connect_swapped(calculator_objects->button_eq, "clicked",
                           G_CALLBACK(eq_button_solve), calculator_objects);
  g_signal_connect_swapped(calculator_objects->button_del, "clicked",
                           G_CALLBACK(delete_sym), calculator_objects->view);
  g_signal_connect(calculator_objects->mainWindow, "destroy",
                   G_CALLBACK(on_main_window_close), calculator_objects);
}

void initialize_grid(calculatorObjects *calculator_objects) {
  gtk_grid_attach(GTK_GRID(calculator_objects->grid), calculator_objects->view,
                  0, -1, 4, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_1, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_2, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_3, 2, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_4, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_5, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_6, 2, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_7, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_8, 1, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_9, 2, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_0, 1, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_pow, -1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_right_bracket, -1, -1, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_left_bracket, -2, -1, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->spin_button, -3, -1, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_del, 4, -1, 2, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_eq, 2, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_dot, 0, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_plus, 4, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_minus, 4, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_mul, 4, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_div, 4, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_mod, -2, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_sin, -2, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_cos, -1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_tan, -2, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_log, -1, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_sqrt, -2, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_log10, -1, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_acos, -3, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_asin, -3, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_atan, -3, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(calculator_objects->grid),
                  calculator_objects->button_x, -3, 0, 1, 1);
}

void destroy_calculator_objects(calculatorObjects *calculator_objects) {
  gtk_widget_destroy(calculator_objects->mainWindow);
  if (calculator_objects->drawing_area != NULL)
    gtk_widget_destroy(calculator_objects->drawing_area);
  if (calculator_objects->graphWindow != NULL)
    on_graph_window_close(calculator_objects->graphWindow, calculator_objects);
  free(calculator_objects->dots_array);
  free(calculator_objects);
}