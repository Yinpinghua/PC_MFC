#ifndef md5_file_h__
#define md5_file_h__

//MD5摘要值结构体
typedef struct MD5VAL_STRUCT
{
	unsigned int a;
	unsigned int b;
	unsigned int c;
	unsigned int d;
} md5_val;

//计算字符串的MD5值(若不指定长度则由函数计算)
md5_val md5(char* str, unsigned int size = 0);
//MD5文件摘要
md5_val md5_file(FILE* fpin);

//直接计算文件MD5
CStringA get_file_md5(CString fname);

//字符串md5加密
void get_str_md5(char* input, char* output);

#endif // md5_file_h__
