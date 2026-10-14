#include "PIL_dynlib.h"
#undef NDEBUG
#include <assert.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	PILdynlib *library;
	int (*value)(void);
	assert(argc == 2);
	library = PIL_dynlib_open(argv[1]);
	assert(library != NULL);
	value = (int (*)(void))PIL_dynlib_find_symbol(library, "dynlibtest_value");
	assert(value && value() == 42);
	assert(PIL_dynlib_find_symbol(library, "missing_test_symbol") == NULL);
	assert(PIL_dynlib_get_error_as_string(library) != NULL);
	PIL_dynlib_close(library);
	assert(PIL_dynlib_open("/nonexistent/blender-dynlibtest.dylib") == NULL);
	assert(PIL_dynlib_get_error_as_string(NULL) != NULL);
	puts("PASS: native plugin load, symbols, errors, close");
	return 0;
}
