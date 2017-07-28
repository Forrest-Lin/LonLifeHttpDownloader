#ifndef _LIGHT_JSON_H_
#define _LIGHT_JSON_H_
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>//errno
#include <math.h>//HUGE_VAL
#include <assert.h> 
#include "../map/map.h"

//检测第一个字符为ch 
#define EXPECT(c, ch) do { assert(*c->json == (ch)); c->json++; } while(0)
#define light_init(v) do { (v)->type = LIGHT_NULL; } while(0)
#define light_set_null(v) light_free(v)
#define PUTC(c,ch) do { *(char*)light_context_push((c),sizeof(char)) = (ch); } while(0)
#define PUTS(c, s, len)     memcpy(light_context_push(c, len), s, len)
#define STRING_ERROR(ret) do { (c)->top = head; return ret; } while(0)
#define ISDIGIT(ch,start,end) ((ch)>=(start) && (ch)<=(end))


#ifndef LIGHT_CONTEXT_STACK_INIT_SIZE
#define LIGHT_CONTEXT_STACK_INIT_SIZE 256
#endif  

//json  type
typedef enum 
{   // 0          1          2        3            4              5           6
    LIGHT_NULL,LIGHT_FALSE,LIGHT_TRUE,LIGHT_NUMBER,LIGHT_STRING,LIGHT_ARRAY,LIGHT_OBJECT
}light_type;

//return 
enum 
{
    LIGHT_PARSE_OK = 0,
    LIGHT_PARSE_EXPECT_VALUE,            //1  JSON 空白
    LIGHT_PARSE_INVALID_VALUE,           //2 值类型不存在
    LIGHT_PARSE_ROOT_NOT_SINGULAR,        //3若一个值之后，在空白之后还有其他字符
    LIGHT_PARSE_NUMBER_TOO_BIG,           //4数字太大
    //string
    LIGHT_PARSE_INVALID_STRING_ESCAPE,    //5无效的转义字符
    LIGHT_PARSE_INVALID_STRING_CHAR,      //6无效的字符
    LIGHT_PARSE_MISS_QUOTATION_MARK,      //7缺少结束标志
    LIGHT_PARSE_INVALID_UNICODE_SURROGATE,//8unicode 无效的编码
    LIGHT_PARSE_INVALID_UNICODE_HEX ,     //9 无效的16进制数
    //array
    LIGHT_PARSE_MISS_COMMA_OR_SQUARE_BRACKET,   //10array缺少',' or ']'
    //object
    LIGHT_PARSE_MISS_KEY,                    //11 缺少key
    LIGHT_PARSE_MISS_COLON ,                 //12缺少':'
    LIGHT_PARSE_MISS_COMMA_OR_CURLY_BRACKET,   //13缺少',' or '}'
    LIGHT_OK
};
typedef struct light_value light_value;



//json Node
struct light_value
{
    light_type type;
    union
    {
        struct{size_t size; Map *pmap;}object;//
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

/***********************************接口************************************************/
void show_value(light_value *v);

light_value *create_number(double num);//malloc
light_value *create_string(const char *string);
light_value *create_array();
light_value *create_object();
void add_array(light_value *v, light_value *arr);
void add_object(light_value *v,light_value *key,light_value *val);

int light_parse(light_value *v, const char *json);
int light_generate(const light_value* v, char** json, size_t* length) ;
void light_free(light_value *v);

light_value *Value(light_value *pmain, const char *key);
void get_number(light_value *p, double *res);
void get_string(light_value *p, char *res);
light_value *index_array(light_value *, int);

/*****************************************解析*************************************/


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

int light_get_boolean(const light_value *v);
void light_set_boolean(light_value *v, int b);


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
static int light_parse_object(light_context* c, light_value* v); 


/***********************************生成器generate********************************************/


static void light_generate_string(light_context* c, const char* s, size_t len);

static int light_generate_value(light_context* c,const light_value *v);


/***********************************map*******************************************************/


Item *New_Item(char *key, void *value); 

void show_item(void *data);

void map_show(Map *pmap) ;

void inner_clear(void *p); 

void clear_map(Map *pmap);
void traverse(Map *pmap, light_context* c);
#endif
