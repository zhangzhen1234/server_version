#include <termio.h>
#include "menu_config.h"
#include <string>
using namespace std;

Config::Config(string cf)
{
    config_file = cf;
    openFile("r");
};
Config::Config() {
};
Config::~Config() {
	// closeFile();
};

bool Config::openFile(char *flags)
{
	if ((read_setup = fopen(config_file.c_str(), flags)) == NULL) {
		puts("Fail to open config.txt!");
		return 0;
	}
	return 1;
}

void Config::closeFile()
{
	fclose(read_setup);
}

bool Config::_str_cmp(char* a, char *b)
{
	int sum = 0;
	for (int i = 0; b[i] != '\0'; i++)
		sum++;
	char tmp[200] = { "" };
	strncpy(tmp, a + 0, sum);
	for (int i = 0; a[i] != '\0'; i++)
	{
		if (a[i] == '\n')
			a[i] = (char)NULL;
	}
	return !strcmp(tmp, b);
}

void Config::clearBlank()
{
	string config_contxt;
	fseek(read_setup, 0, SEEK_SET);
	char str[200];
	while (fgets(str, 200, read_setup) != NULL) {
		if (str[0] != 10)
			config_contxt += str;
	}
	closeFile();
	openFile("w+");
	fprintf(read_setup, "%s", config_contxt.c_str());
	closeFile();
}

void Config::readConfig(int &len, char **param, unsigned int *value)  //len总参长度，value总菜单列表
{

	len = 0;
	char str_org[200];
	char str[200];
	fseek(read_setup, 0, SEEK_SET); //指向文件的开头
	while (fgets(str_org, 200, read_setup) != NULL) {   //读取每一行赋值给str_org
		if (!_str_cmp(str_org, (char *)" "))  //_str_cmp返回值不相等为0 ，相等为1
		{
			memcpy(str, str_org, 200);  //将一个缓冲区复制到另一个缓冲区；
			const char * split = " ";
			char *p = strtok(str, split); //分割字符串
			p = strtok(NULL, split);
			if (p != NULL)
			{
				memcpy(param[len], str_org, 200);
				sscanf(p, "%d", &value[len]);
				len++;
			}
		}
	}
}

void Config::readConfig_json(int &len, Json::Value &Buf,int &subParamCount,bool is_sub)  //len总参长度，value总菜单列表
{
	len = 0;
	char str_org[200];
	char str[200];
	string value;
	string title;
	fseek(read_setup, 0, SEEK_SET); //指向文件的开头
	while (fgets(str_org, 200, read_setup) != NULL) {   //读取每一行赋值给str_org
		if (!_str_cmp(str_org, (char *)" "))  //_str_cmp返回值不相等为0 ，相等为1
		{
			memcpy(str, str_org, 200);  //将一个缓冲区复制到另一个缓冲区；
			const char * split = " ";
			char *p = strtok(str, split); //分割字符串
			if (p != NULL)
			{
				title = p;
				p = strtok(NULL, split);
				value = p;
				Buf[title] = atoi(value.c_str());
				if(is_sub)
				{
                   Buf["child_number"] = subParamCount;
				}
				len++;
			}
		}
	}
}

string Config::get_all()
{
	string strstr;
	char str[200];
	fgets(str, 200, read_setup);
    int i=0;
    while(i<200)
    {
    char temp = str[i++];
    if(temp == '\n'|| temp ==10|| temp ==32)
        break;
    else
        strstr+= temp;
    }
	return strstr;
}

string Config::get_video_name(char *paramName)
{
    string strstr;
	char str[200];
	fseek(read_setup, 0, SEEK_SET); //文件指针指向文件的开头
	while (fgets(str, 200, read_setup) != NULL) {  //while循环的是一直逐行读文件，直到文件读完的时候，再读就会返回NULL
		if (_str_cmp(str, paramName))   //判断是否相等，相等进入if条件下
		{
			const char * split = " ";
			char *p = strtok(str, split);
			p = strtok(NULL, split);
			int i=0;
            while(i<200)
            {
            char temp = p[i++];
            if(temp == '\n'|| temp ==10|| temp ==32)
                break;
            else
                strstr+= temp;
            }
			break;
		}

	}
	return strstr;
}

unsigned int Config::get_param(char *paramName)
{
	unsigned int per = 0;
	char str[200];
	fseek(read_setup, 0, SEEK_SET);
	while (fgets(str, 200, read_setup) != NULL) {   //循环每一行
		if (_str_cmp(str, paramName))    //根据paramName字符串,来判断str字符串前paramName长度数目的字符串与paramName字符差un进行比较
		{
			const char * split = " ";
			char *p = strtok(str, split);    
			p = strtok(NULL, split);
			sscanf(p, "%d", &per);   //获取数字
			break;
		}

	}
	return per;
}

void Config::set_param(char *paramName, unsigned int val)
{
	float per = 0;
	char str[200];
	fseek(read_setup, 0, SEEK_SET); //定位到文件的最开始位置
	long file_pos_up = 0;
	long file_pos_down = 0;
	while (fgets(str, 200, read_setup) != NULL) {   //循环读取每一行
		if (_str_cmp(str, paramName))
		{
			file_pos_down = ftell(read_setup);
			fseek(read_setup, -10L, SEEK_CUR);
			while (1)
			{
				char ch = fgetc(read_setup);
				if (ch==32)
				{
					file_pos_up = ftell(read_setup);
					break;
				}
			}
			break;
		}

	}
	string config_contxt;
	fseek(read_setup, 0L, SEEK_SET);
	char str_t = 0;
	while (file_pos_up > ftell(read_setup)) {
		str_t= fgetc(read_setup);
        if (str_t == NULL|| str_t<0||str_t>=128) continue;
		config_contxt += str_t;
		
	}
	char buffer_max[20];
	sprintf(buffer_max, "%d", val);
	config_contxt += buffer_max;
	if(str_t!=10)
		config_contxt += "\n";
	fseek(read_setup, file_pos_down, SEEK_SET);

	while (!feof(read_setup)) {
		str_t = fgetc(read_setup);
		if (str_t == NULL|| str_t<0||str_t>=128) continue;
		config_contxt += str_t;
	}
	closeFile();
	openFile("w+");
	fseek(read_setup, 0L, SEEK_SET);
	fprintf(read_setup, "%s", config_contxt.c_str());
	closeFile();
}

