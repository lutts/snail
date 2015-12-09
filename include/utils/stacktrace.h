// Copyright (c) 2008, Timo Bingmann from http://idlebox.net/
// published under the WTFPL v2.0

#ifndef INCLUDE_UTILS_STACKTRACE_H_
#define INCLUDE_UTILS_STACKTRACE_H_

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <cxxabi.h>
#include <dirent.h>
#include <signal.h>

class Stacktrace {
 public:
  Stacktrace() {
    saved_segment_fault_handler_ = signal(SIGSEGV, segment_fault_handler);
  }

  ~Stacktrace() { signal(SIGSEGV, saved_segment_fault_handler_); }

 private:
  sighandler_t saved_segment_fault_handler_{SIG_DFL};

 public:
  static void segment_fault_handler(int sig) {
    (void)sig;
    print_stacktrace();
    exit(1);
  }

  /** Print a demangled stack backtrace of the caller function to FILE* out. */
  static inline void print_stacktrace(FILE *out = stderr,
                                      unsigned int max_frames = 63) {
    fprintf(out, "stack trace:\n");

    // storage array for stack trace address data
    void *addrlist[max_frames + 1];

    // retrieve current stack addresses
    int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void *));

    if (addrlen == 0) {
      fprintf(out, "  <empty, possibly corrupt>\n");
      return;
    }

    // resolve addresses into strings containing "filename(function+address)",
    // this array must be free()-ed
    char **symbollist = backtrace_symbols(addrlist, addrlen);

    // allocate string which will be filled with the demangled function name
    size_t funcnamesize = 512;
    char *funcname = static_cast<char *>(malloc(funcnamesize));

    // iterate over the returned symbol lines. skip the first, it is the
    // address of this function.
    for (int i = 1; i < addrlen; i++) {
      char *begin_name = 0, *begin_offset = 0, *end_offset = 0;
      char *begin_addr = nullptr;
      char *end_addr = nullptr;

      int seqno = i - 1;

      // find parentheses and +address offset surrounding the mangled name:
      // ./module(function+0x15c) [0x8048a6d]
      for (char *p = symbollist[i]; *p; ++p) {
        if (*p == '(') {
          begin_name = p;
        } else if (*p == '+') {
          begin_offset = p;
        } else if (*p == ')' && begin_offset) {
          end_offset = p;
        } else if (*p == '[') {
          begin_addr = p;
        } else if (*p == ']' && begin_addr) {
          end_addr = p;
          break;
        }
      }

      if (begin_name) *begin_name++ = '\0';

      if (begin_name && begin_offset && end_offset &&
          begin_name < begin_offset) {
        *begin_offset++ = '\0';
        *end_offset = '\0';

        // mangled name is now in [begin_name, begin_offset) and caller
        // offset in [begin_offset, end_offset). now apply
        // __cxa_demangle():

        int status;
        char *ret =
            abi::__cxa_demangle(begin_name, funcname, &funcnamesize, &status);
        if (status == 0) {
          funcname = ret;  // use possibly realloc()-ed string
          fprintf(out, "  #%d : %s+%s\n", seqno, funcname, begin_offset);
        } else {
          // demangling failed. Output function name as a C function with
          // no arguments.
          fprintf(out, "  #%d : %s()+%s\n", seqno, begin_name, begin_offset);
        }
      } else {
        // couldn't parse the line? print the whole line.
        fprintf(out, "  #%d : %s\n", seqno, symbollist[i]);
      }

      if (begin_addr) {
        *begin_addr++ = '\0';
        *end_addr = '\0';

        char *file_path = funcname;
        int lineno;
        int found = getFileAndLine(symbollist[i], begin_addr, file_path,
                                   funcnamesize, &lineno);
        if (found) {
          // emacs friendly
          fprintf(out, "%s:%d: %s\n", file_path, lineno, begin_addr);
        }
      }
    }

    free(funcname);
    free(symbollist);
  }

  static int getFileAndLine(char *exec_path, char *addr, char *file_path,
                            size_t flen, int *line) {
    static const int kBufferSize = 512;
    static char buf[kBufferSize];

    // prepare command to be executed
    // our program need to be passed after the -e parameter
    snprintf(buf, kBufferSize, "addr2line -C -e %s -f -i %s", exec_path, addr);
    FILE *f = popen(buf, "r");

    if (f == NULL) {
      perror(buf);
      return 0;
    }

    // get function name
    fgets(buf, 512, f);

    // get file and line
    fgets(buf, 512, f);

    if (buf[0] != '?') {
      char *p = buf;

      // file name is until ':'
      while (*p != ':') {
        p++;
      }

      *p++ = 0;
      // after file name follows line number
      strncpy(file_path, buf, flen);
      sscanf(p, "%d", line);

      return 1;
    } else {
      strncpy(file_path, "unkown", sizeof("unkown"));
      *line = 0;

      return 0;
    }

    pclose(f);
  }
};

#endif  // INCLUDE_UTILS_STACKTRACE_H_
