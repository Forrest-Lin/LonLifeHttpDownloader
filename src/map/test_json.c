
#include <stdlib.h>
#include <stdio.h>
#include "light_json.h"






int main()
{
	light_value v;
	
	printf("return:%d\n", light_parse(&v, ".123"));
	printf("number:%f\n", light_get_number(&v));

	printf("return:%d\n", light_parse(&v, "1."));
	printf("number:%f\n", light_get_number(&v));
	
	printf("return:%d\n", light_parse(&v, "1e-10000"));
	printf("number:%f\n", light_get_number(&v));
	
	printf("return:%d\n", light_parse(&v, "Nan"));
	printf("number:%f\n", light_get_number(&v));
	return 0;
}
