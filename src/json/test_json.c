
#include <stdlib.h>
#include <stdio.h>
#include "light_json.h"




//"[ null , false , true , 123 , \"abc\" ]"



int main()
{
    char *s = "{\"title\": \"Design Patterns\",\"author\": [\"Erich Gamma\",\"Richard Helm\",\"RalphJohnson\",\"John Vlissides\"],\"year\": 2009,\"weight\": 1.8,\"hardcover\": true,\"publisher\": {\"Company\": \"Pearson Education\",\"Country\": \"India\"}}";
    light_value v;
    light_value e;
    light_value *p = create_object();
    light_value *a = create_array();
    char *ret;
    size_t len;
    add_object(p,create_string("name"),create_string("hhh"));
    
    add_array(a,create_string("Erich Gamma"));
    add_array(a,create_string("Richard Helm"));
    show_value(a);
    
    add_object(p,create_string("author"),a);
    printf("object:");
    show_value(p);
    printf("\n");
    light_free(p);
    //light_parse(&v, s);
    //show_value(&v);

 
    



    /*
    //test parse miss key
    printf("test parse miss key:\n");
    printf("return:%d\n", light_parse(&v, "{:1,"));
    printf("return:%d\n", light_parse(&v, "{1:1,"));
    printf("return:%d\n", light_parse(&v, "{true:1,"));
    printf("return:%d\n", light_parse(&v, "{false:1,"));
    printf("return:%d\n", light_parse(&v, "{null:1,"));
    printf("return:%d\n", light_parse(&v, "{[]:1,"));
    printf("return:%d\n", light_parse(&v, "{{}:1,"));
    printf("return:%d\n", light_parse(&v, "{\"a\":1,"));

    printf("test parse miss colon:\n");
    printf("return:%d\n", light_parse(&v, "{\"a\"}"));
    printf("return:%d\n", light_parse(&v, "{\"a\",\"b\"}"));

    printf("test parse miss }:\n");
    printf("return:%d\n", light_parse(&v, "{\"a\":1"));
    printf("return:%d\n", light_parse(&v, "{\"a\":1]"));
    printf("return:%d\n", light_parse(&v, "{\"a\":1 \"b\""));
    printf("return:%d\n", light_parse(&v, "{\"a\":{}"));
    printf("\n");   
*/
 
	
	return 0;
}
