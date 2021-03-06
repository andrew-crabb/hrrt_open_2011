/*
  hrrt_util.cpp
  Common utilities for HRRT executables
  ahc 11/2/17
  */

#include <iostream>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <fstream>

#include "hrrt_util.h"

bool file_exists (const std::string& name) {
  bool ret = false;
  std::ifstream f(name.c_str());
  ret = f.good();
  std::cerr << "*** hrrt_util::file_exists: " << name << " returning " << ret << std::endl << std::flush;
  return ret;
}


// prog:    Program to run
// args:    Argument string to program
// outfile: Optional output file to check for
// Check for existence of given program, then run with given args.
// Returns: 0 on success, else 1

int run_system_command( char *prog, char *args, FILE *log_fp ) {
  char command_line[4096];
  int ret = 0;
  char *ptr = NULL;
  char ma_pattern_name[FILENAME_MAX];
  
  ma_pattern_name[0] = '\0';
  if ((ptr=getenv("HOME")) != NULL) {
    sprintf(ma_pattern_name, "%s/.ma_pattern.dat", ptr);
    if (!access(ma_pattern_name, R_OK)) {
      fprintf(stderr, "*** ma_pattern file exists: '%s'\n", ma_pattern_name);      
      if (remove(ma_pattern_name) != 0) {
        fprintf(stderr, "*** ERROR: Removing ma_pattern file: '%s'\n", ma_pattern_name);
      } else {
        fprintf(stderr, "*** Removed ma_pattern file OK: '%s'\n", ma_pattern_name);
      }
    } else {
      fprintf(stderr, "*** ma_pattern file '%s' does not exist\n", ma_pattern_name);
    }
  } else {
    fprintf(stderr, "*** Could not determine HOME envt var: Cannot remove ma_pattern.dat file\n");
  }
  fflush(stderr);

  ret = access(prog, X_OK);
  if (ret) {
    std::cerr << "Error: run_system_command(" << prog << "): File does not exist" << std::endl << std::flush;    
  } else {
    std::cerr << "*** run_system_command('" << prog << "', '" << args << "')" << std::endl << std::flush;
    sprintf(command_line, "%s %s", prog, args);
    fprintf(log_fp, "%s\n", command_line);
    ret = system(command_line);
  }
  std::cerr << "run_system_command('" << prog << "', '" << args << "') returning " << ret << std::endl << std::flush;
  return(ret);
}
