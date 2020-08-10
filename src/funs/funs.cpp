#include <sstream>
#include <sys/time.h>
#include <sys/timeb.h>
#include <sys/stat.h>
#include <time.h>
#include <termio.h>
#include "funs.h"

void readSubConfig(int &len, vector<Json::Value> *paramList, char **param, unsigned int *value)
{
	int subParamCount = 0;
	bool is_sub = 1;
	Config cfg_sub;
	while (subParamCount < 14)
	{
		cfg_sub.config_file = "../log/param/config/" + sub_abbrevation[subParamCount] + ".txt";
		if (!cfg_sub.openFile("r")) {
			puts("Fail to open config.txt!");
		}
		Json::Value Buf_sub;
		cfg_sub.readConfig_json(len, Buf_sub,subParamCount,is_sub);
		(*paramList).push_back(Buf_sub);
		Buf_sub.clear();
		for (int i = 0; i < 10; i++) {
			memset(param[i], 0x00, 200);
		}
		subParamCount++;
	}
}
int main()
{
	int Port = 8899;
	Json::Value RecvBuf;
	HttpServer sock;
	sock.Socket();
	sock.Bind(Port);
	int paramLength;//总参长度
	unsigned int paramValueList[30];//总参菜单
	int subParamLength;//子参长度
	unsigned int subParamValueList[30];//子参菜单
	const char * split = " ";
	char **paramList;
	paramList = (char **)malloc(sizeof(char *) * 30);
	for (int i = 0; i < 30; i++)
		paramList[i] = (char *)malloc(sizeof(char) * 200);
	
	string cfg_address;
	char **subParamList;
	subParamList = (char **)malloc(sizeof(char *) * 10);
	for (int i = 0; i < 10; i++)
		subParamList[i] = (char *)malloc(sizeof(char) * 200);
	vector<Json::Value> subParamAll;
	Config cfg(CONFIG_FILE);
		//发送总参数
	Json::Reader reader;
    Json::Value parameterJson;//接受总参数
    Json::Value RecvBuf_json;
	int subParamCount = 0;
	cfg.readConfig_json(paramLength, RecvBuf_json,subParamCount);
	cfg.closeFile();
	parameterJson["parameter"]= RecvBuf_json; //读取json格式{}

	//发送子参数
	Json::Value childparamJson;//接受子参数
	Json::Value subParam;			
	readSubConfig(subParamLength, &subParamAll, subParamList, subParamValueList);
	for (vector<Json::Value>::iterator it = subParamAll.begin(); it != subParamAll.end(); it++)
		{
			subParam.append(*it);
		}
	childparamJson["child_param"] = subParam;
	//发送启动项参数
	while(1)
	{
          sock.Recv(RecvBuf);
		  cout<<"RecvBuf"<<RecvBuf<<endl;
		  Json::Value::Members members;  
          members = RecvBuf.getMemberNames();
		  for (Json::Value::Members::iterator iterMember = members.begin(); iterMember != members.end(); iterMember++)   // 遍历每个key
			{  
				string strKey = *iterMember;
				if (!RecvBuf[strKey.c_str()][0u].isNull())
				{  
					if(strKey=="parameter") //修改总参数
					{
						string strVal = RecvBuf[strKey.c_str()].toStyledString();
						Json::Value DevJson;
						reader.parse(strVal,DevJson);
						Json::Value::Members mem = DevJson.getMemberNames();
						for (auto iter = mem.begin(); iter != mem.end(); iter++)
						{
							string new_key = *iter;
							int new_value;
							if (!DevJson[new_key.c_str()].isNull()) {
								   string new_value_str = DevJson[new_key.c_str()].asString();
								   new_value = stoi(new_value_str);
								}
							char paramTmp[200];
							memcpy(paramTmp, new_key.c_str(), 200);
							Config cfg_r0(CONFIG_FILE);
							float paramValue = cfg_r0.get_param(paramTmp);
							cfg_r0.closeFile();

							Config cfg_w(CONFIG_FILE);
							cfg_w.set_param(paramTmp, new_value);
						}
						Config cfg_r(CONFIG_FILE);
						RecvBuf_json.clear();
						cfg_r.readConfig_json(paramLength, RecvBuf_json,subParamCount);
						cfg_r.closeFile();
						parameterJson["parameter"]= RecvBuf_json;
						sock.Send(parameterJson);
					}
					else if(strKey=="child_param")//修改子参数
					{
						string strVal = RecvBuf[strKey.c_str()][0u].toStyledString();
						Json::Value DevJson;
						reader.parse(strVal,DevJson);
						int child_number;
						string config_file;
						if (!DevJson["child_number"].isNull()) {
							string child_number_str = DevJson["child_number"].asString();
							child_number = stoi(child_number_str);
						}
						Json::Value::Members mem = DevJson.getMemberNames();
						for (auto iter = mem.begin(); iter != mem.end(); iter++)
						{
							string child_key = *iter;
							int child_value;
							if (!DevJson[child_key.c_str()].isNull()&&child_key!="child_number") {
								string child_value_str = DevJson[child_key.c_str()].asString();
								child_value = stoi(child_value_str);
								char paramTmp[200];
								memcpy(paramTmp, child_key.c_str(), 200);
								config_file="../log/param/config/"+sub_abbrevation[child_number]+".txt";
								Config cfg_r0(config_file);
								float paramValue = cfg_r0.get_param(paramTmp);
								cfg_r0.closeFile();

								Config cfg_w(config_file);
								cfg_w.set_param(paramTmp, child_value);
							}
						}
						int subParamCount = child_number;
						bool is_sub = 1;
						int len =0;
						Json::Value Buf_sub;
						childparamJson.clear();
						Config cfg_sub;
						cfg_sub.config_file = config_file;
						if (!cfg_sub.openFile("r")) {
							puts("Fail to open config.txt!");
						}
						cfg_sub.readConfig_json(len, Buf_sub,subParamCount,is_sub);
						childparamJson["child_param"] = Buf_sub;
						sock.Send(childparamJson);
					}
					else if(strKey=="child_rectangle")//修改启动项目
					{
						sock.Send(childparamJson);
					}
					else
					{
						break;
					}
					
				}
				else
				{
					if(strKey=="parameter") //点击总参按钮
					{
						sock.Send(parameterJson);
					}
					else if(strKey=="child_param")//点击子参按钮
					{
					//    Json::Value DevJson1;
                    //    childparamJson.clear();
                    //    DevJson1["child_number"] = "0";
					//    DevJson1["continues_cnt_move"] = "400";
					//    childparamJson["child_param"][0u] = DevJson1;
					// 	cout<<"childparamJson"<<childparamJson<<endl;
						sock.Send(childparamJson);
					}
					else if(strKey=="child_rectangle")//点击启动项目
					{
						sock.Send(childparamJson);
					}
					else if(strKey=="clear")//点击清除缓存
					{
						system("sh ../clear_buff.sh");
					}
					else if(strKey=="quit")//点击退出
					{
						break;
					}
				}
				 
			}
	}
	return 0;
}