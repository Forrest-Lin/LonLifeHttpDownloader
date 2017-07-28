#include "light_json.h"



light_value *create_number(double num)
{
    light_value *ret = (light_value*)malloc(sizeof(light_value));
    ret->type = LIGHT_NUMBER;
    ret->munion.number = num;
    return ret;
}
light_value *create_string(const char *string)
{
    
    light_value *ret = (light_value*)malloc(sizeof(light_value));
    light_context c;
    c.stack = NULL;
    c.size = 0;
    c.top = 0;
    light_generate_string(&c,string,strlen(string));
    c.json = c.stack;
    light_parse_string(&c,ret);
    return ret;
}
light_value *create_array()
{
    light_value *ret = (light_value*)malloc(sizeof(light_value));
    ret->type = LIGHT_ARRAY;
    ret->munion.arr.arr = NULL;
    ret->munion.arr.size = 0;
    return ret;
}
light_value *create_object()
{
    light_value *ret = (light_value*)malloc(sizeof(light_value));
    ret->type = LIGHT_OBJECT;
    ret->munion.object.pmap = NULL;
    ret->munion.object.size = 0;
    return ret;
}
void add_array(light_value *v, light_value *arr)
{
    assert(v != NULL && v->type == LIGHT_ARRAY);
    int len = v->munion.arr.size;
    if(v->munion.arr.size == 0)
    {
        v->munion.arr.arr = arr;
        v->munion.arr.size = 1;
    }
    else
    {
        printf("bug\n");
        v->munion.arr.arr = (light_value*)realloc(v->munion.arr.arr, sizeof(light_value)*(len+1));
        memcpy(v->munion.arr.arr+len,arr,sizeof(light_value));
        v->munion.arr.size += 1;
    }
    
}
void add_object(light_value *v,light_value *key,light_value *val)
{
    assert(v != NULL && v->type == LIGHT_OBJECT);
    
    if(v->munion.object.pmap == NULL)
    {
        v->munion.object.pmap  = (Map*)malloc(sizeof(Map));//map  init
        *(v->munion.object.pmap) = map();
    }
    char *mstr = (char*)malloc(key->munion.str.len+1);
    memcpy(mstr,key->munion.str.str,key->munion.str.len);
    mstr[key->munion.str.len] = '\0';
    Item *ite = New_Item(mstr, val);
    add_item(v->munion.object.pmap, ite);
    light_free(key);
}



void show_value(light_value *v)
{
    size_t i;
    assert(v != NULL);
    switch(v->type)
    {
        case LIGHT_NULL:printf(" null ");break;
        case LIGHT_TRUE:printf(" true ");break;
        case LIGHT_FALSE:printf(" false ");break;
        case LIGHT_NUMBER:printf(" %f ", light_get_number(v));break;
        case LIGHT_STRING:printf(" %s ", light_get_string(v));break;
        case LIGHT_ARRAY:
            printf("[ ");
            for(i = 0; i<v->munion.arr.size; ++i)
            {
                if(i >0)printf(",");
                show_value(&v->munion.arr.arr[i]);
            }
            printf(" ]");
            break;
        case LIGHT_OBJECT:
            printf("{ ");
            map_show(v->munion.object.pmap);
            printf("} ");
            break;
        default: break;
    }
}


int light_parse(light_value *v, const char* json)
{
    light_context c;
    int ret;
    assert(v != NULL);

    c.stack = NULL;
    c.size = 0;
    c.top = 0;
    
    c.json = json;
    v->type = LIGHT_NULL;
    light_parse_whitespace(&c);
    if ((ret = light_parse_value(&c, v)) == LIGHT_PARSE_OK) 
    {
        light_parse_whitespace(&c);
        if (*c.json != '\0')
            ret = LIGHT_PARSE_ROOT_NOT_SINGULAR;//singular
    }
    assert(c.top == 0);
    free(c.stack);
    return ret;
}


/**********************************************************************************************/
static void light_parse_whitespace(light_context* c) 
{
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

static int light_parse_literal(light_context* c, light_value* v,
                                const char* literal, light_type type)
{
    EXPECT(c, literal[0]);
    size_t i;
    for(i = 0; literal[i+1]; ++i)
    {
        if(c->json[i] != literal[i+1])
        return LIGHT_PARSE_INVALID_VALUE;
    }
    c->json += i;
    v->type = type;
    return LIGHT_PARSE_OK;
}

static int light_parse_number(light_context *c, light_value *v)
{
    char* p = c->json;
    //负号 
    if(*p == '-')++p;
    //整数
    if(*p == '0')++p;
    else
    {
        if(!ISDIGIT(*p,'1','9'))return LIGHT_PARSE_INVALID_VALUE;
        for(++p;ISDIGIT(*p,'0','9');++p);
    }
    //小数
    if(*p == '.')
    {
        ++p;
        if(!ISDIGIT(*p,'0','9'))return LIGHT_PARSE_INVALID_VALUE;
        for(++p;ISDIGIT(*p,'0','9');++p);
    }
    //指数 
    if(*p == 'e' || *p == 'E')
    {
        ++p;
        if (*p == '+' || *p == '-') ++p;
        if (!ISDIGIT(*p,'0','9')) return LIGHT_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p,'0','9'); p++);
    }
    v->munion.number = strtod(c->json, NULL);
    if (errno == ERANGE && (v->munion.number == HUGE_VAL || v->munion.number == -HUGE_VAL))
        return LIGHT_PARSE_NUMBER_TOO_BIG;

    c->json = p;
    v->type = LIGHT_NUMBER;
    return LIGHT_PARSE_OK;	
}

static int light_parse_string(light_context *c, light_value *v)
{
    size_t head = c->top; 
    size_t len;
    const char* p;
    unsigned u ,u2;
    EXPECT(c,'\"');

    p = c->json;
    while(1)
    {
        char ch = *p++;
        switch(ch)
        {
            case '\"':
                len = c->top - head;
                light_set_string(v, (const char*)light_context_pop(c, len), len);
            
                c->json = p;
                return LIGHT_PARSE_OK;
            case '\0':
                c->top = head;
                return LIGHT_PARSE_MISS_QUOTATION_MARK;
            case '\\':
                switch(*p++)
                {
                    case '\"': PUTC(c, '\"'); break;
                    case '\\': PUTC(c, '\\'); break;
                    case '/':  PUTC(c, '/' ); break;
                    case 'b':  PUTC(c, '\b'); break;
                    case 'f':  PUTC(c, '\f'); break;
                    case 'n':  PUTC(c, '\n'); break;
                    case 'r':  PUTC(c, '\r'); break;
                    case 't':  PUTC(c, '\t'); break;
                    case 'u':
                        if (!(p = light_parse_hex4(p, &u)))
                            STRING_ERROR(LIGHT_PARSE_INVALID_UNICODE_HEX);
                        if (u >= 0xD800 && u <= 0xDBFF) 
                        { //高代理项
                            if (*p++ != '\\')//
                                STRING_ERROR(LIGHT_PARSE_INVALID_UNICODE_SURROGATE);
                            if (*p++ != 'u')
                                STRING_ERROR(LIGHT_PARSE_INVALID_UNICODE_SURROGATE);
                            if (!(p = light_parse_hex4(p, &u2)))
                                STRING_ERROR(LIGHT_PARSE_INVALID_UNICODE_HEX);
                            if (u2 < 0xDC00 || u2 > 0xDFFF)
                                STRING_ERROR(LIGHT_PARSE_INVALID_UNICODE_SURROGATE);
                            u = (((u - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
                        }//x*0x400
                        light_encode_utf8(c, u);break;
                    default:
                        c->top = head;
                        return LIGHT_PARSE_INVALID_STRING_ESCAPE;
                }break;
            default :
                if ((unsigned char)ch < 0x20) { //处理不合法字符
                    c->top = head;
                    return LIGHT_PARSE_INVALID_STRING_CHAR;
                }
                PUTC(c,ch);break;
        }//switch
    }
}

static int light_parse_value(light_context* c, light_value* v) 
{
    switch (*c->json) 
    {
        case 't':  return light_parse_literal(c,v,"true",LIGHT_TRUE);
        case 'f':  return light_parse_literal(c,v,"false",LIGHT_FALSE);
        case 'n':  return light_parse_literal(c, v,"null",LIGHT_NULL);
        case '\0': return LIGHT_PARSE_EXPECT_VALUE;
        case '\"': return light_parse_string(c,v);
        case '[':  return light_parse_array(c,v);
        case '{':  return light_parse_object(c,v);
        default:   return light_parse_number(c, v);
    }
}

void light_free(light_value *v)
{
    size_t i;
    assert(v != NULL);
    switch(v->type)
    {
        case LIGHT_STRING:free(v->munion.str.str);break;
        case LIGHT_ARRAY:
            for(i = 0; i<v->munion.arr.size; ++i)
            {
                light_free(&v->munion.arr.arr[i]);
            }
            free(v->munion.arr.arr);
            break;
        case LIGHT_OBJECT:
            clear_map(v->munion.object.pmap);
            free(v->munion.object.pmap);
            break;
        default: break;
    }
    v->type = LIGHT_NULL;
}

const char* light_get_string(const light_value *v)
{
    assert(v != NULL && v->type == LIGHT_STRING);
    return v->munion.str.str;
}
size_t light_get_string_length(const light_value *v)
{
    assert(v != NULL && v->type == LIGHT_STRING);
    return v->munion.str.len;
}
void light_set_string(light_value *v, const char *s, size_t len)
{
    assert(v != NULL &&(s != NULL || len != 0));
    light_free(v);
    v->munion.str.str = (char*)malloc(len+1);
    memcpy(v->munion.str.str,s,len);
    v->munion.str.str[len] = '\0';
    v->munion.str.len = len;
    v->type = LIGHT_STRING;
}


light_type light_get_type(const light_value* v)
{
    assert(v != NULL);
    return v->type;
}

double light_get_number(const light_value* v)
{
    assert(v != NULL && v->type == LIGHT_NUMBER);
    return v->munion.number; 
}

void light_set_number(light_value *v, double n)
{
    light_free(v);
    v->munion.number = n;
    v->type = LIGHT_NUMBER;
}
int light_get_boolean(const light_value *v)
{
    assert(v != NULL && (v->type == LIGHT_TRUE|| v->type == LIGHT_FALSE));
    return v->type == LIGHT_TRUE;
}
void light_set_boolean(light_value *v, int b)
{
    light_free(v);
    v->type = b ? LIGHT_TRUE:LIGHT_FALSE;
}


static void* light_context_push(light_context *c, size_t size)
{
    void *ret;
    assert(c != NULL && size > 0);
    if(c->top + size >= c->size)
    {
        if(c->size == 0)
        {
            c->size = LIGHT_CONTEXT_STACK_INIT_SIZE;
        }
        while (c->top + size >= c->size) 
        {
            c->size += c->size>>1; 	
        }
        c->stack = (char*)realloc(c->stack, c->size);
    }
    ret = c->stack + c->top;
    c->top += size; 
    return ret;
}
static void* light_context_pop(light_context *c, size_t size)
{
    assert(c != NULL && c->top >= size );
    return c->stack+(c->top -= size);
}

//从字符串中解析4位的16进制数   
static const char* light_parse_hex4(const char* p, unsigned* u) 
{
    int i;
    *u = 0;
    for (i = 0; i < 4; i++) {
        char ch = *p++;
        *u <<= 4;
        if      (ch >= '0' && ch <= '9')  *u |= ch - '0';
        else if (ch >= 'A' && ch <= 'F')  *u |= ch - ('A' - 10);
        else if (ch >= 'a' && ch <= 'f')  *u |= ch - ('a' - 10);
        else return NULL;
    }
    return p;
}

static void light_encode_utf8(light_context* c, unsigned u) 
{
    if (u <= 0x7F) 
        PUTC(c, u & 0xFF);
    else if (u <= 0x7FF) 
    {
        PUTC(c, 0xC0 | ((u >> 6) & 0xFF));
        PUTC(c, 0x80 | ( u       & 0x3F));//0x00 111111 取后6位
    }
    else if (u <= 0xFFFF) 
    {
        PUTC(c, 0xE0 | ((u >> 12) & 0xFF));
        PUTC(c, 0x80 | ((u >>  6) & 0x3F));
        PUTC(c, 0x80 | ( u        & 0x3F));
    }
    else 
    {
        assert(u <= 0x10FFFF);
        PUTC(c, 0xF0 | ((u >> 18) & 0xFF));
        PUTC(c, 0x80 | ((u >> 12) & 0x3F));
        PUTC(c, 0x80 | ((u >>  6) & 0x3F));
        PUTC(c, 0x80 | ( u        & 0x3F));
    }
}

size_t light_get_array_size(const light_value *v)
{
    assert(v != NULL && v->type == LIGHT_ARRAY);
    return v->munion.arr.size;
}
light_value* light_get_array(const light_value *v, size_t index)
{
    assert(v != NULL && v->type == LIGHT_ARRAY);
    assert(index < v->munion.arr.size);
    return &v->munion.arr.arr[index];
}


static int light_parse_array(light_context* c, light_value* v)
{
    size_t size = 0;
    size_t i;
    int ret;
    EXPECT(c, '[');
    light_parse_whitespace(c);           //'[' 之后
    if (*(c->json) == ']') {
        c->json++;
        v->type = LIGHT_ARRAY;
        v->munion.arr.size = 0;
        v->munion.arr.arr = NULL;
        return LIGHT_PARSE_OK;
    }
    for (;;) 
    {
        light_value e;
        light_init(&e);
        if ((ret = light_parse_value(c, &e)) != LIGHT_PARSE_OK)break;
   
        memcpy(light_context_push(c, sizeof(light_value)), &e, sizeof(light_value));//压入stack
        size++;
        light_parse_whitespace(c);       //','之前
        if (*c->json == ',')
        {
            c->json++;
            light_parse_whitespace(c);  //','之后
        } 
        else if (*c->json == ']') 
        {
            c->json++;
            v->type = LIGHT_ARRAY;
            v->munion.arr.size = size;
            size *= sizeof(light_value);
            memcpy(v->munion.arr.arr = (light_value*)malloc(size), 
                                        light_context_pop(c, size), size);
            return LIGHT_PARSE_OK;
        }
        else
        {  
            ret =  LIGHT_PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
            break;
        }
    }
    for (i = 0; i < size; i++)
        light_free((light_value*)light_context_pop(c, sizeof(light_value)));
    return ret;
}


static int light_parse_object(light_context* c, light_value* v) 
{
    size_t size;
    char *mstr = NULL;
    int ret;
	
    EXPECT(c, '{');
    light_parse_whitespace(c);
    if (*c->json == '}') {
        c->json++;
        v->type = LIGHT_OBJECT;
        v->munion.object.pmap = NULL;
        v->munion.object.size = 0;
        return LIGHT_PARSE_OK;
    }
    size = 0;
    v->munion.object.pmap  = (Map*)malloc(sizeof(Map));//map  init
    *(v->munion.object.pmap) = map();
    for (;;) 
    {
	    
        light_value mkey;//
        light_value mvalue;//  
        size_t mlen;
	    
        light_init(&mkey);
        light_init(&mvalue);
        
        // parse key 
        if (*c->json != '\"') //?
        {
            ret = LIGHT_PARSE_MISS_KEY;
            break;
        }
        if ((ret = light_parse_string(c, &mkey)) != LIGHT_PARSE_OK)
            break;
        
        mlen = mkey.munion.str.len;
        mstr = mkey.munion.str.str;

        // parse ws colon ws 
   	    light_parse_whitespace(c);
        if (*c->json != ':') 
        {
            ret = LIGHT_PARSE_MISS_COLON;
            break;
        }
        c->json++;
        light_parse_whitespace(c);
        
        // parse value
        if ((ret = light_parse_value(c, &mvalue)) != LIGHT_PARSE_OK)break;
        Item *ite = New_Item(mstr, &mvalue);
        add_item(v->munion.object.pmap, ite);
     
        mstr = NULL;
       	size++;
        
        // parse ws [',' | '}'] ws 
        light_parse_whitespace(c);
        if (*c->json == ',') 
        {
            c->json++;
            light_parse_whitespace(c);
        }
        else if (*c->json == '}') 
        {
            c->json++;
            v->type = LIGHT_OBJECT;
            v->munion.object.size = size;
            return LIGHT_PARSE_OK;
        }
        else 
        {
            ret = LIGHT_PARSE_MISS_COMMA_OR_CURLY_BRACKET;
            break;
        }
    }
    // free  
    free(mstr);
    light_free(v);//free子树
    return ret;
}




/***********************************生成器generate********************************************/

int light_generate(const light_value* v, char** json, size_t* length) 
{
    light_context c;
    int ret;
    assert(v != NULL);
    assert(json != NULL);
    c.stack = (char*)malloc(c.size = LIGHT_CONTEXT_STACK_INIT_SIZE);
    c.top = 0;
    
    if ((ret = light_generate_value(&c, v)) != LIGHT_OK) 
    {
        free(c.stack);
        *json = NULL;
        return ret;
    }
    if (length)
        *length = c.top;
    PUTC(&c, '\0');
    *json = c.stack;
    return LIGHT_OK;
}

//生成字符串
static void light_generate_string(light_context* c, const char* s, size_t len) 
{
    static const char hex_digits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    size_t i, size;
    char* head, *p;
    assert(s != NULL);
    size = len * 6 + 2;
    head = light_context_push(c, size);
    p  = head;
    
    *p++ = '\"';//?
    
    for (i = 0; i < len; i++) 
    {
        unsigned char ch = (unsigned char)s[i];
        switch (ch) 
        {
            case '\"': *p++ = '\\'; *p++ = '\"'; break;
            case '\\': *p++ = '\\'; *p++ = '\\'; break;
            case '\b': *p++ = '\\'; *p++ = 'b';  break;
            case '\f': *p++ = '\\'; *p++ = 'f';  break;
            case '\n': *p++ = '\\'; *p++ = 'n';  break;
            case '\r': *p++ = '\\'; *p++ = 'r';  break;
            case '\t': *p++ = '\\'; *p++ = 't';  break;
            default:
                if (ch < 0x20) // 
                {
                    *p++ = '\\'; *p++ = 'u'; *p++ = '0'; *p++ = '0';
                    *p++ = hex_digits[ch >> 4];
                    *p++ = hex_digits[ch & 15];
                }
                else
                    *p++ = s[i];
        }
    }
    
    *p++ = '\"';
    c->top -= size - (p - head);
}


static int light_generate_value(light_context* c,const light_value *v)
{
    size_t i;
    int len;
    switch(v->type)
    {
        case LIGHT_NULL:  PUTS(c, "null",  4); break;
        case LIGHT_TRUE:   PUTS(c, "true",  4); break;
        case LIGHT_FALSE:  PUTS(c, "false",  5); break;
        case LIGHT_NUMBER://sprintf("%.17g", ...) 把浮点数转换成文本
            len = sprintf(light_context_push(c, 32), "%.17g", v->munion.number);//
            c->top -= 32 - len;    
            break;
        case LIGHT_STRING:
            light_generate_string(c,v->munion.str.str,v->munion.str.len);break;
        case LIGHT_ARRAY:
            PUTC(c,'[');	
            for(i = 0; i<v->munion.arr.size; ++i)
            {
                if(i > 0)PUTC(c,',');
                light_generate_value(c, &v->munion.arr.arr[i]);
            }
            PUTC(c,']');break;
        case LIGHT_OBJECT:
            PUTC(c,'{');
            traverse(v->munion.object.pmap,c);
            PUTC(c,'}');
    }
    return LIGHT_OK;
}




/***********************************map*******************************************************/

Item *New_Item(char *key, void *value) 
{
    Item *pres = (Item *)calloc(sizeof(Item), 1);
    pres->key = key;
    pres->value = (char *)calloc(sizeof(char), sizeof(light_value));
    memcpy(pres->value, value,sizeof(light_value));
    return pres;
}
void show_item(void *data)
{
    Item *p = (Item *)data;
    light_value *q = (light_value*)p->value;
    printf("%s : ", p->key);
    show_value(q);
    printf("\n");
}
void map_show(Map *pmap) 
{
    show(pmap->tree, show_item);
}
void inner_clear(void *p) 
{
    light_value * pv= (light_value *)p;
    light_free(pv);
}
void clear_map(Map *pmap) 
{
    clear(pmap->tree, inner_clear);
}



void traverse(Map *pmap, light_context* c) 
{
	// 非递归版遍历二叉树
	if (pmap->tree->root == NULL) {
		printf("empty tree");
		return ;
	}
	
	Node *p = pmap->tree->root;
	Node *tail = pmap->tree->tail;
	size_t i = 0;
	while(p->left != tail)p = p->left;
	
	while(p != NULL)
	{
    	if(i>0)PUTC(c,',');
    	Item *it = (Item *)(p->data);
    	light_value *q = (light_value*)(it->value);

    	light_generate_string(c,it->key,strlen(it->key));//'\0'
    	
    	PUTC(c,':');
    	light_generate_value(c,q);
    	
    	++i;
    	if(p->right != tail)
    	{
        	p = p->right;
        	while(p->left != tail)p = p->left;
    	}
    	else
    	{
        	Node *tmp = p->parent;
        	while(tmp != NULL && tmp->right == p)
        	{
            	p = tmp;
            	tmp = p->parent;
        	}
        	p = tmp;
    	}
	}
}
