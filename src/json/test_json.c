
#include <stdlib.h>
#include <stdio.h>
#include "light_json.h"




//"[ null , false , true , 123 , \"abc\" ]"



int main()
{
    char *s = "{\"title\": \"Design Patterns\",\"author\": [\"Erich Gamma\",\"Richard Helm\",\"RalphJohnson\",\"John Vlissides\"],\"year\": 2009,\"weight\": 1.8,\"hardcover\": true,\"publisher\": {\"Company\": \"Pearson Education\",\"Country\": \"India\"}}";
    light_value v;
    light_value e;
    char *ret;
    size_t len;
    //printf("return:%d\n", light_parse(&v, "[ null , false , true , 123 , \"abc\" ]"));
    //show_value(&v);

    printf("parse return:%d\n", light_parse(&v, s));
    show_value(&v);
    printf("\n");  

    printf("generate return:%d\n", light_generate(&v,&ret,&len ));
    printf("ret:%s",ret);
    printf("len:%d\n",len);
    



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
