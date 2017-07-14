#ifndef _LIGHT_JSON_H_
#define _LIGHT_JSON_H_
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>//errno
#include <math.h>//HUGE_VAL
#include <assert.h> 

//检测第一个字符为ch 
#define EXPECT(c, ch) do { assert(*c->json == (ch)); c->json++; } while(0)
#define light_init(v) do { v->type = LIGHT_NULL; } while(0)
#define light_set_null(v) light_free(v)
#define PUTC(c,ch) do { *(char*)light_context_push(c,sizeof(char)) = ch; } while(0)
#define STRING_ERROR(ret) do { c->top = head; return ret; } while(0)

#ifndef LIGHT_CONTEXT_STACK_INIT_SIZE
#define LIGHT_CONTEXT_STACK_INIT_SIZE 256
#endif  

//json  type
typedef enum 
{
	LIGHT_NULL,LIGHT_FALSE,LIGHT_TRUE,LIGHT_NUMBER,LIGHT_STRING,LIGHT_ARRAY,LIGHT_OBJECT
}light_type;

//return 
enum 
{
	LIGHT_PARSE_OK = 0,
    LIGHT_PARSE_EXPECT_VALUE,            //JSON 空白
    LIGHT_PARSE_INVALID_VALUE,           //值类型不存在
    LIGHT_PARSE_ROOT_NOT_SINGULAR,        //若一个值之后，在空白之后还有其他字符
    LIGHT_PARSE_NUMBER_TOO_BIG,           //数字太大
    LIGHT_PARSE_INVALID_STRING_ESCAPE,    //无效的转义字符
    LIGHT_PARSE_INVALID_STRING_CHAR,      // 无效的字符
    LIGHT_PARSE_MISS_QUOTATION_MARK,      //
    LIGHT_PARSE_INVALID_UNICODE_SURROGATE, //unicode 无效的编码
    LIGHT_PARSE_INVALID_UNICODE_HEX        // 无效的16进制数
};
typedef struct light_value light_value;

typedef struct member//对象成员
{
	Map *pmap;
	//map; key-value
}member;

//json Node
struct light_value
{
	light_type type;
	union
	{
		struct{size_t size; member *object;}object;//
		struct{size_t len;char *str;}str;          //string
		struct{size_t size; light_value *arr;}arr;//array
		double number;                        //number
	}munion;
};

//json string
typedef struct 
{
    const char* json;
    char* stack;      
    size_t size,top;
}light_context;


//map
Item *new_item(const char*key, void *value) 
{
	Item *pres = (Item *)calloc(sizeof(Item), 1);
	pres->key = key;
	pres->value = (char *)calloc(sizeof(char), sizeof(light_value));
	memcpy(pres->value, value,sizeof(light_value));
	return pres;
}
void show_Item();
void inner_clear(void *p) 
{
	light_value * pv= (light_value *)p;
	light_free(pv);
}


//解析
int light_parse(light_value *v, const char *json);

static void light_parse_whitespace(light_context *c); 
static int light_parse_literal(light_context *c, light_value *v,
								const char *literal, light_type type);							
static int light_parse_number(light_context *c, light_value *v);
static int light_parse_string(light_context *c, light_value *v);

static int light_parse_value(light_context *c, light_value *v); 
//读取value
light_type light_get_type(const light_value *v);


double light_get_number(const light_value *v);
void light_set_number(light_value *v, double n);

int lept_get_boolean(const light_value *v);
void lept_set_boolean(light_value *v, int b);

void light_free(light_value *v);
const char* light_get_string(const light_value *v);
size_t light_get_string_length(const light_value *v);
void light_set_string(light_value *v, const char *s, size_t len);


static void* light_context_push(light_context *c, size_t size);
static void* light_context_pop(light_context *c, size_t size);

static const char* light_parse_hex4(const char* p, unsigned* u);
static void light_encode_utf8(light_context* c, unsigned u);

size_t light_get_array_size(const light_value *v);
light_value* light_get_array(const light_value *v, size_t index); 
static int light_parse_array(light_context* c, light_value* v);


static void light_stringify_string(light_context* c, const char* s, size_t len);

static void light_create_value(light_context* c,const light_value *v);
#endif