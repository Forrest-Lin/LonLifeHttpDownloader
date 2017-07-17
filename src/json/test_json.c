
#include <stdlib.h>
#include <stdio.h>
#include "light_json.h"




//"[ null , false , true , 123 , \"abc\" ]"



int main()
{
	char *s = "{\"title\": \"Design Patterns\",\"author\": [\"Erich Gamma\",\"Richard Helm\",\"RalphJohnson\",\"John Vlissides\"],\"year\": 2009,\"weight\": 1.8,\"hardcover\": true,\"publisher\": {\"Company\": \"Pearson Education\",\"Country\": \"India\"}}";
	light_value v;
	light_value e;
	//printf("return:%d\n", light_parse(&v, "[ null , false , true , 123 , \"abc\" ]"));
	//show_value(&v);

	printf("return:%d\n", light_parse(&v, s));
	show_value(&v);
	printf("\n");
	
	//printf("return:%d\n", light_parse(&v, "\"hello\""));
	//show_value(&v);
	//printf("\n");
	
	return 0;
}
