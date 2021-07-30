#pragma once

#include"Sock.hpp"
#include"Log.hpp"
#include"Util.hpp"
#define WEBROOT "./wwwroot"
#define HOMEPAGE "index.html"
#define VERSION  "HTTP/1.0"

static string CodeToDesc(int code)
{
	string desc;
	switch (code)
	{
	case 200:
		desc = "OK";
		break;
	case 404:
		desc = "NOT FOUND";
		break;
	default:
		desc = "OK";

	}
	return desc;
}

class HttpRequest{
private:
	string request_line;
	vector<string> request_header;
	string blank;
	string request_body;
private:
	string method;
	string uri;
	string version;
	string _path;
	string query_string;
	unordered_map<string, string> header_Kv;
	ssize_t content_length;
	bool cgi;
	ssize_t file_size;
public:
	HttpRequest()
		:blank("\n")
		, _path(WEBROOT)
		, content_length(-1)
		, cgi(false)
	{}
	//设置进请求行
	void SetRequestLine(const string &line)
	{
		request_line = line;
	}
	//解析报头
	void RequestLineParse()
	{
		Util::StringParse(request_line, method, uri, version);

		LOG(Notice, request_line);
		LOG(Notice, method);
		LOG(Notice, uri);
		LOG(Notice, version);
	}
	//设置请求报头
	void InsertHeaderLine(const string &line)
	{
		request_header.push_back(line);
		LOG(Notice, line);
	}
	//解析请求报头
	void RequestHeaderParse()
	{
		for (auto it = request_header.begin(); it != request_header.end(); it++)
		{
			string k, v;
			Util::MakeString2KV(*it, k, v);
			//把content-Length专门拿出来，因为后面有正文，拿出来好判断
			if (k == "Content-Length")
			{
				content_length = stoi(v);
			}
			header_Kv.insert(make_pair(k, v));

		}
	}
	//是否需要读取正文
	//1.post方法2.存在contentlenth
	bool IsNeedRecvBody()
	{
		//和post进行忽略大小写的比较,
		if (strcasecmp(method.c_str(), "POST") == 0 && content_length>0)
		{
			cgi = true;
			return true;
		}
		return false;
	}
	int GetContenLength()
	{
		return content_length;
	}
	void SetRequestBody(string&  body)
	{
		request_body = body;
	}
	bool IsMethodLegal()
	{
		if (strcasecmp(method.c_str(), "POST") == 0\
			|| method.c_str(), "GET")
		{
			return true;
		}
		return  false;
	}
	bool IsGet()
	{
		if (strcasecmp(method.c_str(), "GET") == 0)
		{

			return true;
		}
		else
		{
			return false;
		}
	}

	bool IsPost()
	{
		if (strcasecmp(method.c_str(), "POST") == 0)
		{

			return true;
		}
		else
		{
			return false;
		}
	}
	void UriParse()
	{

		size_t pos = uri.find('?');
		if (pos == string::npos)
		{
			_path += uri;
		}
		else
		{
			_path += uri.substr(0, pos);
			query_string = uri.substr(pos + 1);
			cgi = true;
		}

	}
	void SetUriEqPath()
	{
		_path += uri;
	}
	void IsAddHonePage()
	{
		//假如最后一个是'/',即请求一个目录
		if (_path[_path.size() - 1] == '/')
		{
			_path += HOMEPAGE;
		}
	}
	string GetPath()
	{
		return _path;
	}
	void SetPath(string Ppath)
	{
		_path = Ppath;
	}
	void SetCgi()
	{
		cgi = true;
	}
	bool IsCgi()
	{
		return cgi;
	}
	void SetFileSzie(ssize_t s)
	{
		file_size = s;
	}
	ssize_t GetFileSize()
	{
		return file_size;
	}
	string GetQueryString()
	{
		return query_string;
	}
	string GetBody()
	{
		return request_body;
	}
	string GetMethod()
	{
		return method;
	}

	~HttpRequest()
	{}
};
class HttpResponse{
private:
	string status_line;
	vector<string> response_header;
	string blank;
	string response_body;
public:
	HttpResponse()
		:blank("\r\n")
	{}
	void setStatusLine(string sline)
	{
		status_line = sline;
	}
	string GetStatusLine()
	{
		return status_line;
	}
	const vector<string>& GetRspHeader()
	{
		response_header.push_back(blank);
		return response_header;
	}

	~HttpResponse()
	{}
};
//端点，处理请求,响应
