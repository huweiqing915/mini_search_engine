/*************************************************************************
	> File Name: Config.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月14日 星期三 15时48分42秒
 ************************************************************************/

#include "Config.h"

using namespace std;

Config *Config::_p_config = NULL;  //静态变量的初始化  
MutexLock Config::_lock;

Config::Config()
{
	load_config_file();
}

Config::~Config()
{

}

//获得对象的实例的方法，只能通过new
Config* Config::get_instance()
{
	if(_p_config == NULL)
	{
		_lock.lock();
		if(_p_config == NULL)
		{
			_p_config = new Config;
		}
		_lock.unlock();
	}
	return _p_config;
}

static inline bool split(string &src, string &key, string &value,  char pattern)
{
	if(src.empty())
	{
		return false;
	}
	string::size_type pos = 0;
	while((pos = src.find(' ', pos)) != string::npos)
	{
		src.erase(pos, 1);
	}
	pos = src.find_first_of(pattern, 0);
	key = src.substr(0, pos);
	value = src.substr(pos+1, src.size());
	return true;
}

//将配置文件中的内容放到_config_map中
bool Config::load_config_file()
{
	ifstream infile(CONFIG_DAT_PATH);
	if(!infile)
	{
		LogFatal("open file[%s] failed!", CONFIG_DAT_PATH);
		return false;
	}
	string line;
	size_t lineno = 0;
	while(getline(infile, line))
	{
		lineno ++;
		string key, value;
		if(line.size() == 0)
		{
			continue;
		}
		//去除符号，保留key和value
		if(!split(line, key, value, MATCH_CHARACTER))
        {
            LogFatal("line[%d:%s] is illegal.", lineno, line.c_str());
            return false;
        }
        //如果已经存在一个文件，则返回false  
        pair<map<string, string>::iterator, bool> ret = _config_map.insert(make_pair(key, value));
        if(!ret.second)
        {
        	LogFatal("key[%s] already exists.", key.c_str());
            return false;
        }
	}
	infile.close();
	infile.clear();
    return true;
}


bool Config::get_file_name(const string &key, string &value) const
{
	map<string, string>::const_iterator iter = _config_map.find(key);
	if(iter != _config_map.end())
	{
		value = iter->second;
		return true;
	}
	return false;
}

