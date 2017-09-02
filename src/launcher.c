/* Copyright (C) 2010 by Matteo Franchin (fnch@users.sourceforge.net)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  A copy of the GNU General Public License can be downloaded from
 *  <http://www.gnu.org/licenses/>.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <process.h>
#include <Windows.h>

#define PATH_ENVVAR "PATH"

static char *my_strdup(const char *src) {
  void *dest = strdup(src);
  if (dest != NULL)
    return dest;
  fprintf(stderr, "strdup failed!\n");
  exit(1);
  return NULL;
}

static void *my_malloc(size_t s) {
  void *x = malloc(s);
  if (x != NULL)
    return x;
  fprintf(stderr, "malloc failed!\n");
  exit(1);
  return NULL;
}

static char *my_strcat(char *left, const char *right) {
  if (left == NULL)
    return my_strdup(right);

  else if (right == NULL)
    return left;

  else {
    size_t s_len = strlen(left) + strlen(right) + 1;
    char *s = (char *) my_malloc(s_len*sizeof(char));
    strcpy(s, left);
    strcat(s, right);
    free(left);
    return s;
  }
}

char *get_executable_dir(void) {
  char *exec_filename, *exec_dir, *bn;
  DWORD copied_len;

  exec_filename = (char *) my_malloc(MAX_PATH*sizeof(char));

  copied_len = GetModuleFileName(NULL, exec_filename, MAX_PATH);
  if (copied_len < 1 || copied_len >= MAX_PATH) {
    fprintf(stderr, "Error while calling GetModuleFileName: using CWD.\n");
    free(exec_filename);
    return my_strdup(".");
  }

  bn = strrchr(exec_filename, '\\');
  if (bn == (char *) NULL) {
    fprintf(stderr, "Executable path does not contain any \\ characters. "
                    "Using CWD.\n");
    free(exec_filename);
    return my_strdup(".");
  }

  *bn = '\0';
  
  exec_dir = my_strdup(exec_filename);
  free(exec_filename);
  return exec_dir;
}

int main(int argc, char *argv[]) {
  char *old_path, *new_path, *exec_dir, *python_exec, *python_source;
  char **args;
  int num_args, i;

  /* Get the directory containing the executable (the file produced by 
   * compiling this C source file): all the other paths will be composed
   * relative to this path.
   */
  exec_dir = get_executable_dir();

  /* Here we construct the path to the Python executable to use */
#ifdef CONSOLE
  python_exec = my_strcat(my_strdup(exec_dir), "\\python\\python.exe");
#else
  python_exec = my_strcat(my_strdup(exec_dir), "\\python\\pythonw.exe");
#endif

  /* and here the path to the source to be executed by Python */
  /* NOTE: we have to embed the name of the file in quotes. like "file" -.-' */
  python_source = my_strcat(my_strdup("\""), exec_dir);
  python_source = my_strcat(python_source, "\\pyapp\\main.py\"");

  /* We change the PATH environment variable so that the GTK libraries 
   * cab be found
   */
  new_path = my_strcat(NULL, PATH_ENVVAR"=");
  new_path = my_strcat(new_path, exec_dir);
  new_path = my_strcat(new_path, "\\gtk\\bin");

  free(exec_dir);

  old_path = getenv(PATH_ENVVAR);
  if (old_path != NULL) { 
    new_path = my_strcat(new_path, ";");
    new_path = my_strcat(new_path, old_path);
  }

#if 0
  printf("Value of PATH was:\n%s\nand is now:\n%s\n", old_path, new_path);
#endif

  putenv(new_path);
  free(new_path);

  /* Now build the command line for Python */
  num_args = argc + 2;
  args = (char **) my_malloc(num_args*sizeof(char *));
  args[0] = args[1] = python_source;
  for(i = 2; i < num_args - 1; i++)
    args[i] = argv[i - 1];
  args[num_args - 1] = (char *) NULL;

  /* Now call Python to execute the Python source */
  _execv(python_exec, (const char **) args);

  /* We get back from the previous call, only in case of errors... */
  fprintf(stderr, "Cannot call Python. Exiting...\n");

  free(python_exec);   /* This is of little use, as it won't be called */
  free(python_source); /* unless _execv fails. Just for aesthetics...  */

  exit(1);
}
