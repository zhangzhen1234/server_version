#ifndef MENU_CONFIG_H
#define MENU_CONFIG_H
#include <iostream>
#include <fstream>
#include "../funs/funs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<json/json.h>
using namespace std;

#define CONFIG_FILE "..//config.txt"

static string sub_abbrevation[16]=
{
	"MHZ",
	"TGHK",
	"KYAQ",
	"ZGAQ",
	"ZHG",
	"TGDMQ",
	"DMQRZ",
	"FYGQ",
	"TGDZW",
	"CHTS",
	"PYSS",
	"YBTGCH",
	"TGCHBGC",
	"GLSJ",
	"DSQ",
	"LXQ",
};

class Config {
private:
	
	FILE *read_setup;    //File类文件操作
	bool _str_cmp(char* a, char *b);   //比较两个字符串的大小
public:
	string config_file;
	Config(string cf);    // 带参数的构造函数
	Config();
	~Config();
	bool openFile(char *flags);
	void closeFile();
	void clearBlank();
	string get_all();
	unsigned int get_param(char *paramName);
	string get_video_name(char *paramName);
	void set_param(char *paramName, unsigned int val);
	void readConfig(int &len,char **param,unsigned int *value);
	void readConfig_json(int &len,Json::Value &Buf,int &subParamCount,bool is_sub=0);
};
#endif
