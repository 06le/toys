#define _CRTDBG_MAP_ALLOC

#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <errno.h>   /* errno, ERANGE */
#include <math.h>    /* HUGE_VAL */
#include <stdlib.h>  /* NULL, malloc(), realloc(), free(), strtod() */
#include <string.h>  /* memcpy() */


#ifndef LEPT_PARSE_STACK_INIT_SIZE
#define LEPT_PARSE_STACK_INIT_SIZE 256
#endif 

#define EXPECT(c, ch) do{ assert(*c->json == (ch)); c->json++;} while(0)
#define ISDIGIT(ch)	((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)	((ch) >= '1' && (ch) <= '9')
#define PUTC(c, ch) do {*(char*)lept_context_push(c, sizeof(char)) = (ch);} while(0)  //ch是要压入的字符

typedef struct
{
	const char* json;
	char* stack;
	size_t size, top;  //size:当前的堆栈容量，top:栈顶的位置
}lept_context;

static void* lept_context_push(lept_context* c, size_t size)  //size 压入类型字节大小
{
	void* ret; //用作返回指针  
	assert(size > 0);

	if (c->top + size >= c->size)
	{
		if (c->size == 0)
		{
			c->size = LEPT_PARSE_STACK_INIT_SIZE;
		}
		while (c->top + size >= c->size)
		{
			c->size += c->size >> 1; //c.size * 1.5
		}
		c->stack = (char*)realloc(c->stack, c->size);
	}
	ret = c->stack + c->top;	//返回数据起始的指针
	c->top += size;
	return ret;	
}

static void* lept_context_pop(lept_context* c, size_t size)
{
	assert(c->top >= size);
	return c->stack + (c->top -= size);  //返回数据起始的指针
}

static void lept_parse_whitespace(lept_context* c)
{
	const char* p = c->json;
	while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
	{
		++p;
	}
	c->json = p;
}

static int lept_parse_literal(lept_context* c, lept_value* v, const char* literal, lept_type type)
{
	size_t i;
	EXPECT(c, literal[0]);
	for (i = 0; literal[i + 1]; i++)
	{
		if (c->json[i] != literal[i + 1])
			return LEPT_PARSE_INVALID_VALUE;
	}
	c->json += i;
	v->type = type;
	return LEPT_PARSE_OK;
}

static int lept_parse_number(lept_context* c, lept_value* v)
{
	const char* p = c->json;
	if (*p == '-') p++;	
	if (*p == '0') p++;
	else
	{
		if (!ISDIGIT1TO9(*p)) return LEPT_PARSE_INVALID_VALUE;
		for (p++; ISDIGIT(*p); p++);
	}

	if (*p == '.')
	{
		p++;
		if (!ISDIGIT(*p))	return LEPT_PARSE_INVALID_VALUE;
		for (p++; ISDIGIT(*p); p++);
	}

	if (*p == 'e' || *p == 'E')
	{
		p++;
		if (*p == '+' || *p == '-') p++;
		if (!ISDIGIT(*p)) return LEPT_PARSE_INVALID_VALUE;
		for (p++; ISDIGIT(*p); p++);
	}

	errno = 0;
	
	v->n = strtod(c->json, NULL);
	if (errno == ERANGE && (v->n == HUGE_VAL || v->n == -HUGE_VAL))
		return LEPT_PARSE_NUMBER_TOO_BIG;
	v->type = LEPT_NUMBER;
	c->json = p;
	return LEPT_PARSE_OK;
}

static const char* lept_parse_hex4(const char* p, unsigned* u) {
	*u = 0;
	for (int i = 0; i < 4; ++i)
	{
		char ch = *p++;
		*u <<= 4;
		if		(ch >= '0' && ch <= '9') *u |= ch - '0';	 //如果ch是char，-'0'意味着将char 0 转换为 int 0
		else if (ch >= 'a' && ch <= 'f')  *u |= ch - ('a' - 10); //ch = 10 + ch - 'a'
		else if (ch >= 'A' && ch <= 'F') *u |= ch - ('A' - 10);
		else return NULL;
	}
	return p;
}

static void lept_encode_utf8(lept_context* c, unsigned u) {
	
	if (u <= 0x7F)							//0x7F 1111111
		PUTC(c, u & 0xFF);					//0xFF 11111111
	else if (u <= 0x7FF)					//0x7FF 11111111111
	{
		PUTC(c, 0xC0 | ((u >> 6) & 0xFF));	//0xC0 11000000
		PUTC(c, 0x80 | (u & 0x3F));			//0x80 10000000  0x3F 111111
	}
	else if(u <= 0xFFFF)
	{
		PUTC(c, 0xE0 | (u >> 12) & 0xFF);	//0xE0 11100000
		PUTC(c, 0x80 | (u >> 6) & 0x3F);
		PUTC(c, 0x80 | (u & 0x3F));
	}
	else {
		assert(u <= 0x10FFFF);
		PUTC(c, 0xF0 | ((u >> 18) & 0xFF));
		PUTC(c, 0x80 | ((u >> 12) & 0x3F));
		PUTC(c, 0x80 | ((u >> 6) & 0x3F));
		PUTC(c, 0x80 | (u & 0x3F));
	}
}

#define STRING_ERROR(ret) do { c->top = head; return ret; } while(0) //解析错误，还原栈

#if 0
//先备份栈顶，然后把解析到的字符压栈，最后计算出长度并一次性把所有字符弹出，再设置至值里便可以。
static int lept_parse_string(lept_context* c, lept_value* v)
{
	size_t head = c->top, len; //备份栈顶
	unsigned u, u2;
	const char* p;
	EXPECT(c, '\"');  
	p = c->json;
	for (;;) 
    {
        char ch = *p++;
        switch (ch) {
            case '\"':	//遇到结尾双引号
                len = c->top - head;
                lept_set_string(v, (const char*)lept_context_pop(c, len), len);
                c->json = p;	//返回解析了一个string以后的json文本，lept_parse（）里用来检测json是否完结
                return LEPT_PARSE_OK;
            case '\\':
                switch (*p++) {
                    case '\"': PUTC(c, '\"'); break;
                    case '\\': PUTC(c, '\\'); break;
                    case '/':  PUTC(c, '/' ); break;
                    case 'b':  PUTC(c, '\b'); break;
                    case 'f':  PUTC(c, '\f'); break;
                    case 'n':  PUTC(c, '\n'); break;
                    case 'r':  PUTC(c, '\r'); break;
                    case 't':  PUTC(c, '\t'); break;
					case 'u':
						if (!(p = lept_parse_hex4(p, &u)))
							STRING_ERROR(LEPT_PARSE_INVALID_UNICODE_HEX);
						if (u >= 0xD800 && u <= 0xDBFF)		//处理代理对（非BMP平面字符）
						{
							if(*p++ != '\\')
								STRING_ERROR(LEPT_PARSE_INVALID_UNICODE_SURROGATE);
							if(*p++ != 'u')
								STRING_ERROR(LEPT_PARSE_INVALID_UNICODE_SURROGATE);
							if (!(p = lept_parse_hex4(p, &u2)))
								STRING_ERROR(LEPT_PARSE_INVALID_UNICODE_HEX);
							if (u2 < 0xDC00 || u2 > 0xDFFF)
								STRING_ERROR(LEPT_PARSE_INVALID_UNICODE_SURROGATE);
							u = (((u - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
						}
						lept_encode_utf8(c, u);
						break;
					default:
						STRING_ERROR(LEPT_PARSE_INVALID_STRING_ESCAPE);
                }
                break;
            case '\0':  //遇到字符串结尾符号，少了结尾双引号  正确格式： “xxx”\0
                c->top = head;
                return LEPT_PARSE_MISS_QUOTATION_MARK;
            default:
                if ((unsigned char)ch < 0x20) {  //ch >= 32 unsigned是为了避免>= 0x80 的字符变成负数
                    c->top = head;
                    return LEPT_PARSE_INVALID_STRING_CHAR;
                }
                PUTC(c, ch);
        }
    }
}
#endif

/* 解析 JSON 字符串，把结果写入 str 和 len */
/* str 指向 c->stack 中的元素，需要在 c->stack  */
static int lept_parse_string_raw(lept_context* c, char** str, size_t* len) 
{
	size_t head = c->top; //备份栈顶
	unsigned u, u2;
	const char* p;
	EXPECT(c, '\"');
	p = c->json;
	for (;;)
	{
		char ch = *p++;
		switch (ch) {
		case '\"':	//遇到结尾双引号 解析完成
			*len = c->top - head;
			*str = (char*)lept_context_pop(c, *len);
			c->json = p;	//返回解析了一个string以后的json文本，lept_parse（）里用来检测json是否完结
			return LEPT_PARSE_OK;
		case '\\':
			switch (*p++) {
			case '\"': PUTC(c, '\"'); break;
			case '\\': PUTC(c, '\\'); break;
			case '/':  PUTC(c, '/');  break;
			case 'b':  PUTC(c, '\b'); break;
			case 'f':  PUTC(c, '\f'); break;
			case 'n':  PUTC(c, '\n'); break;
			case 'r':  PUTC(c, '\r'); break;
			case 't':  PUTC(c, '\t'); break;
			case 'u':
				if (!(p = lept_parse_hex4(p, &u)))
					STRING_ERROR(LEPT_PARSE_INVALID_UNICODE_HEX);
				if (u >= 0xD800 && u <= 0xDBFF)		//处理代理对（非BMP平面字符）
				{
					if (*p++ != '\\')
						STRING_ERROR(LEPT_PARSE_INVALID_UNICODE_SURROGATE);
					if (*p++ != 'u')
						STRING_ERROR(LEPT_PARSE_INVALID_UNICODE_SURROGATE);
					if (!(p = lept_parse_hex4(p, &u2)))
						STRING_ERROR(LEPT_PARSE_INVALID_UNICODE_HEX);
					if (u2 < 0xDC00 || u2 > 0xDFFF)
						STRING_ERROR(LEPT_PARSE_INVALID_UNICODE_SURROGATE);
					u = (((u - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
				}
				lept_encode_utf8(c, u);
				break;
			default:
				STRING_ERROR(LEPT_PARSE_INVALID_STRING_ESCAPE);
			}
			break;
		case '\0':  //遇到字符串结尾符号，少了结尾双引号  正确格式： “xxx”\0
			STRING_ERROR(LEPT_PARSE_MISS_QUOTATION_MARK);
		default:
			if ((unsigned char)ch < 0x20) {  //ch >= 32 unsigned是为了避免>= 0x80 的字符变成负数
				STRING_ERROR(LEPT_PARSE_INVALID_STRING_CHAR);
			}
			PUTC(c, ch);
		}
	}
}

static int lept_parse_string(lept_context* c, lept_value* v) {
	int ret;
	char* s;
	size_t len;
	if ((ret = lept_parse_string_raw(c, &s, &len)) == LEPT_PARSE_OK)
		lept_set_string(v, s, len);
	return ret;
}

static int lept_parse_value(lept_context* c, lept_value* v);
static int lept_parse_array(lept_context* c, lept_value* v)
{
	size_t i, size = 0;
	int ret;
	EXPECT(c, '[');
	lept_parse_whitespace(c);
	if (*c->json == ']')
	{
		c->json++;
		v->type = LEPT_ARRAY;
		v->size = 0;
		v->e = NULL;
		return LEPT_PARSE_OK;
	}
	for (;;)
	{
		lept_value e;
		lept_init(&e);

		if ((ret = lept_parse_value(c, &e)) != LEPT_PARSE_OK)	//解析错误
			break;																  
		//将解析结果存入临时lept_value，压入栈。
		//void* temp = lept_context_push(c, sizeof(lept_value));
		//memcpy(temp, &e, sizeof(lept_value));
		memcpy(lept_context_push(c, sizeof(lept_value)), &e, sizeof(lept_value));
		size++;		//压入栈中lept_value个数
		lept_parse_whitespace(c);
		if (*c->json == ',')
		{
			c->json++;
			lept_parse_whitespace(c);
		}
		else if (*c->json == ']')	//解析结束
		{
			c->json++;
			v->type = LEPT_ARRAY;
			v->size = size;
			size *= sizeof(lept_value);
			//将栈中lept_value都弹出来存入数组中
			//v->e = (lept_value*)malloc(size);  //开辟空间
			//void* temp = lept_context_pop(c, size);	//弹出元素
			//memcpy(v->e, temp, size);		//弹出的元素存入lept_value成员中
			memcpy(v->e = (lept_value*)malloc(size), lept_context_pop(c, size), size);
			return LEPT_PARSE_OK;
		}
		else
		{
			ret = LEPT_PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
			break;
		}
	}
	for (i = 0; i < size; i++)
	{
		lept_free((lept_value*)lept_context_pop(c, sizeof(lept_value)));
	}
	return ret;
}

static int lept_parse_object(lept_context* c, lept_value* v) 
{
	size_t i, size;
	lept_member m;
	int ret;
	EXPECT(c, '{');
	lept_parse_whitespace(c);
	if (*c->json == '}')  //解析完成 弹出栈中member元素
	{
		c->json++; //该函数结束，要跳到下一个位置方面其他函数使用
		v->type = LEPT_OBJECT;
		v->m = 0;   
		v->osize = 0;
		return LEPT_PARSE_OK;
	}
	
	m.k = NULL; 
	size = 0;
	for (;;)
	{
		char* str;
		lept_init(&m.v);
		/* parse key */
		if(*c->json != '"')
		{
			ret = LEPT_PARSE_MISS_KEY;
			break;
		}

		if ((lept_parse_string_raw(c, &str, &m.klen)) != LEPT_PARSE_OK)
			break;
		//开辟临时值m的k的内存，把解析的key值存入， m.klen+1是因为解析的字符串不包含'\0'，要自己存
		memcpy(m.k = (char*)malloc(m.klen + 1), str, m.klen);
		m.k[m.klen] = '\0';

		/* parse ws colon ws */
		lept_parse_whitespace(c);
		if (*c->json != ':')
		{
			ret = LEPT_PARSE_MISS_COLON;
			break;
		}
		c->json++;
		lept_parse_whitespace(c);
		/* parse value */
		if ((ret = lept_parse_value(c, &m.v)) != LEPT_PARSE_OK)
			break;
		memcpy(lept_context_push(c, sizeof(lept_member)), &m, sizeof(lept_member));
		size++;
		m.k = NULL;/* ownership is transferred to member on stack */
		/* parse ws [comma | right-curly-brace] ws */
		lept_parse_whitespace(c);
		if (*c->json == ',')
		{
			c->json++;
			lept_parse_whitespace(c);
		}
		else if (*c->json == '}')
		{
			size_t s = size * sizeof(lept_member);
			v->type = LEPT_OBJECT;
			v->osize = size;	
			c->json++;
			memcpy(v->m = (lept_member*)malloc(s), lept_context_pop(c, s), s);
			return LEPT_PARSE_OK;
		}
		else
		{
			ret = LEPT_PARSE_MISS_COMMA_OR_CURLY_BRACKET;
			break;
		}
	}
	//释放内存
	free(m.k);
	for (i = 0; i < size; i++)
	{
		lept_member* m = (lept_member*)lept_context_pop(c, sizeof(lept_member));
		free(m->k);
		lept_free(&m->v);
	}
	v->type = LEPT_NULL;
	return ret;
}

static int lept_parse_value(lept_context* c, lept_value* v)
{
	switch (*c->json)
	{
		case 'n':return lept_parse_literal(c, v, "null", LEPT_NULL); 
		case 't':return lept_parse_literal(c, v, "true", LEPT_TRUE);
		case 'f':return lept_parse_literal(c, v, "false", LEPT_FALSE);
		default: return lept_parse_number(c, v);  
		case '"':return lept_parse_string(c, v);
		case '[':return lept_parse_array(c, v);
		case '{':  return lept_parse_object(c, v);
		case '\0':return LEPT_PARSE_EXPECT_VALUE;
	}
}


int lept_parse(lept_value* v, const char* json)
{
	lept_context c;
	int ret;
	assert(v != NULL);
	c.json = json;
	c.stack = NULL;
	c.size = c.top = 0;
	lept_init(v);
	lept_parse_whitespace(&c);
	if ((ret = lept_parse_value(&c, v)) == LEPT_PARSE_OK)
	{
		lept_parse_whitespace(&c);
		if (*c.json != '\0')	//检查json文本是否完结
		{
			v->type = LEPT_NULL;
			ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
		}
	}
	assert(c.top == 0);
	free(c.stack);
	return ret;
}

void lept_free(lept_value* v)
{
	size_t i;
	assert(v != NULL);
	switch (v->type) {
	case LEPT_STRING:
		free(v->s);
		break;
	case LEPT_ARRAY:
		for (i = 0; i < v->size; i++)
			lept_free(&v->e[i]);
		free(v->e);
		break;
	case LEPT_OBJECT:
		for (i = 0; i < v->osize; i ++)
		{
			free(v->m[i].k);
			lept_free(&v->m[i].v);
		}
		free(v->m);
		break;
	default: break;
	}
	v->type = LEPT_NULL;
}

lept_type lept_get_type(const lept_value* v)
{
	assert(v != NULL);
	return v->type;
}

int lept_get_boolean(const lept_value* v) {
	assert(v != NULL && (v->type == LEPT_TRUE || v->type == LEPT_FALSE));
	return v->type == LEPT_TRUE;
}

void lept_set_boolean(lept_value* v, int b) {
	lept_free(v);
	v->type = b ? LEPT_TRUE : LEPT_FALSE;
}

double lept_get_number(const lept_value* v)
{
	assert(v != NULL && v->type == LEPT_NUMBER);
	return v->n;
}

void lept_set_number(lept_value* v, double n) {
	lept_free(v);
	v->n = n;
	v->type = LEPT_NUMBER;
}

const char* lept_get_string(const lept_value* v)
{
	assert(v != NULL && v->type == LEPT_STRING);
	return v->s;
}

size_t lept_get_string_length(const lept_value* v) {
	assert(v != NULL && v->type == LEPT_STRING);
	return v->len;
}

void lept_set_string(lept_value* v, const char* s, size_t len) //使用 s 生成 lept_value 对象
{
	assert(v != NULL && (s != NULL || len == 0)); //非空指针或者零长度字符串都是合法的
	lept_free(v);
	v->s = (char*)malloc(len + 1); //malloc返回的指针用char* 初始化
	memcpy(v->s, s, len);
	v->s[len] = '\0';
	v->len = len;
	v->type = LEPT_STRING;
}

size_t lept_get_array_size(const lept_value* v)
{
	assert(v != NULL && v->type == LEPT_ARRAY);
	return v->size;
}

lept_value* lept_get_array_element(const lept_value* v, size_t index)
{
	assert(v != NULL && v->type == LEPT_ARRAY);
	assert(index < v->size);
	return &v->e[index];
}

size_t lept_get_object_size(const lept_value* v)
{
	assert(v != NULL && v->type == LEPT_OBJECT);
	return v->osize;												
}

const char* lept_get_object_key(const lept_value* v, size_t index)
{
	assert(v != NULL && v->type == LEPT_OBJECT && index >= 0);
	assert(index >= 0 && index < v->osize);						
	return v->m[index].k;

}

size_t lept_get_object_key_length(const lept_value* v, size_t index)
{
	assert(v != NULL && v->type == LEPT_OBJECT && index >= 0);
	assert(index >= 0 && index < v->osize);
	return v->m[index].klen;
}

lept_value* lept_get_object_value(const lept_value* v, size_t index)
{
	assert(v != NULL && v->type == LEPT_OBJECT && index >= 0);
	assert(index >= 0 && index < v->osize);
	return &v->m[index].v;
}