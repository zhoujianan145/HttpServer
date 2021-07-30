#pragma once  

#include"Sock.hpp"
#include"Log.hpp"

class Util
{
public:
	static void StringParse(string &request_line, string &method, string &uri, string& version)
	{
		stringstream ss(request_line);
		ss >> method >> uri >> version;
	}
	static void MakeString2KV(string& line, string &k, string  &v)
	{
		//报头中每一行格式为K: V
		//左闭右开区间[0,pos)返回
		size_t pos = line.find(": ");
		if (string::npos != pos)
		{
			k = line.substr(0, pos);
			v = line.substr(pos + 2);
		}
	}
};
