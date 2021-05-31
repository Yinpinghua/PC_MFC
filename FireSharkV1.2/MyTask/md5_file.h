#ifndef md5_file_h__
#define md5_file_h__

//MD5ժҪֵ�ṹ��
typedef struct MD5VAL_STRUCT
{
	unsigned int a;
	unsigned int b;
	unsigned int c;
	unsigned int d;
} md5_val;

//�����ַ�����MD5ֵ(����ָ���������ɺ�������)
md5_val md5(char* str, unsigned int size = 0);
//MD5�ļ�ժҪ
md5_val md5_file(FILE* fpin);

//ֱ�Ӽ����ļ�MD5
CStringA get_file_md5(CString fname);

//�ַ���md5����
void get_str_md5(char* input, char* output);

#endif // md5_file_h__
