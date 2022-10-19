
#include "serialization.h"

int main(int argc, char const *argv[])
{
    char **keys, **keys_buf;

    keyArray_to_buffer(keys, keys_buf);
    return 0;
}
