#include <stdio.h>

#define ECP_DICT_IMPLEMENTATION
#include <ecp.h>

int main(void)
{
    ecp_dict_t *dict = ecp_new_dict();

    dict->set(dict, "Host", "http://localhost.com");
    dict->print(dict);
    ecp_destroy_dict(dict);
    return 0;
}
