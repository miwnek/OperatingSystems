#ifndef __LIBDLL__
#define __LIBDLL__

#ifdef LIBTYPE_DLL

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

void load_dlls(const char *path) {
  void *header_file = dlopen(path, RTLD_LAZY);

  if(header_file == NULL) {
    printf("Couln't open dll.");
    return;
  }

  *(void **) (&initialize_extended_array) = dlsym(header_file, "initialize_extended_array");
  *(void **) (&execute_wc) = dlsym(header_file, "execute_wc");
  *(void **) (&get_block_on_index) = dlsym(header_file, "get_block_on_index");
  *(void **) (&free_block_on_index) = dlsym(header_file, "free_block_on_index");
  *(void **) (&free_array) = dlsym(header_file, "free_array");
}

#else

void load_dlls(const char *path) {
  return;
}

#endif

#endif
