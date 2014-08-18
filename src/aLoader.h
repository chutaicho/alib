//
//  Created by Takashi Aoki (c) takashiaoki.com
//
#pragma once

#include <iostream>
#include <typeinfo>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

#include <json/json.h>
#include "curl.h"

/*
loader utilities.
*/
 
#pragma mark -
#pragma mark type definition

typedef Json::Value JsonElement;

namespace at
{

#pragma mark -
#pragma mark Buffer

	class Buffer
	{
	public:
		Buffer():_data(""){};
		~Buffer(){};

		bool get(const std::string file, bool binary=false)
		{
			bool res = false;

			//ios_b	ase::openmode mode = binary ? ifstream::binary : ios_base::in;
			//std::ifstream ifs("data/test.json");	
			std::ifstream ifs(file.c_str());	
			std::string line;

			if(!ifs.fail())
			{
				while (std::getline(ifs, line))
				{
	    			// Todo: ignore whitespace!
	    			//ifs.ignore(1000,' ');
					_data += line;
				}

				res = true;
			}
			else
			{
				// error
			}

			ifs.close();
			return res;
		};

		const std::string& getRawString() const { return _data; };

	protected:
		std::string _data;

	};

#pragma mark -
#pragma mark URL

	/*
	
	at::URLLoader url;
	if(url.get("http://ip.jsontest.com/"))
	{
		std::cout << url.getRawString() << std::endl;
	}

	*/

	// libcurl callback
	static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
	{
	    std::string buf = std::string(static_cast<char *>(buffer), size *nmemb);
	    std::stringstream * response = static_cast<std::stringstream *>(userp);
	    response->write(buf.c_str(), (std::streamsize)buf.size());
	    return size * nmemb;
	};

	class URLLoader
	{
	public:
		URLLoader(){};
		~URLLoader(){};

		bool get(const std::string file)
		{
			bool succcess = false;

			CURLcode code;
  	        CURL* curl;
			std::string _ip;			
  	        curl = curl_easy_init();

  	        std::stringstream res;

  	        curl_easy_setopt(curl, CURLOPT_URL, file.c_str());
           curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
			code = curl_easy_perform(curl);

			if(code == CURLE_OK)
			{
				_data = res.str();

				//get my IP
				char* ipc;
				curl_easy_getinfo(curl,CURLINFO_LOCAL_IP, &ipc);
				std::stringstream ss;
				ss << ipc; ss >> _ip;

				succcess = true;
			}
			else
			{
				errorStatus(code);
			}

			curl_easy_cleanup(curl);

			return succcess;
		};

		const std::string& getRawString() const { return _data; };
		const std::string& getMyIP() const { return _ip; };

	protected:
		void errorStatus(const CURLcode &code)
	    {
	        fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(code));
	       
	       	switch (code)
	        {
	            case CURLE_COULDNT_CONNECT:
	            case CURLE_COULDNT_RESOLVE_HOST:
	            case CURLE_COULDNT_RESOLVE_PROXY:
	                std::cout << "CURLE_COULDNT_CONNECT" << std::endl;
	                break;
	            default:
	                std::cout << "Request failed:" << curl_easy_strerror(code) << std::endl;
	                break;
	        }
	    };

		std::string _data;
		std::string _ip;
	};

#pragma mark -
#pragma mark JSON

	/*
	
	example:
	at::JSONLoader json;
	json.load(ofToDataPath("test.json"));	
	JsonElement& value = json.getJSON();
	std::cout << value["firstName"].asString();

	test:
	http://www.jsontest.com/

	*/
	
	class JSONLoader : public Buffer
	{
	public:
		JSONLoader(){};
		~JSONLoader(){};

		void load(const std::string& file)
		{
			Json::Reader read;

			if(file.find("http://")==0 )
		    {
		    	if(_urlLaoder.get(file))
		    	{
		    		if(!read.parse(_urlLaoder.getRawString(), _value))
					{
						std::cout << "Error @ JSONLoader: " << read.getFormatedErrorMessages() << std::endl;
					}
		    	}
			}
		    else if(file.find("https://")==0)
		    {

			}
		    else
		    {
				if(get(file))
				{
					if(!read.parse(_data, _value))
					{
						std::cout << "Error @ JSONLoader: " << read.getFormatedErrorMessages() << std::endl;
					}
				}
			}
		};

		JsonElement& getJSON(){ return _value; };

	protected:
		JsonElement _value;
		URLLoader _urlLaoder;
	};

#pragma mark -
#pragma mark XML

	class Xml : public Buffer
	{
	public:
		Xml(){};
		~Xml(){};

	protected:

	};

#pragma mark -
#pragma mark CSV

	/*

	Example:
	at::Csv csv;
	csv.load(ofToDataPath("test.csv"));

	*/

	class Csv : public Buffer
	{
	public:
		Csv(){};
		~Csv(){};
	    
		void load(const std::string& file)
		{
			if(get(file))
			{
				parse(_data);
			}
		};

		void save(std::string filename)
		{
		  	std::ofstream ofs;
		  	ofs.open(filename.c_str());

			for(std::vector<std::string>::iterator it = _value.begin(); it != _value.end();it++)
			{
				std::string& target = *(it);
				ofs << target << ",";
			}

			ofs << std::endl;
			ofs.close();
		};
		void clear()
		{
			_value.clear();
		};
		void add(std::string val)
		{
			_value.push_back(val);
		};

		const int getSize() const { return _value.size(); };
		
		const std::string& getStringFromIndex(int index) const 
		{ 
			if(index > _value.size()-1)
			{
				std::cout << "CSVLoader request failed: the index is not found. return index 0." << std::endl;
				return _value[index];
			}
			else return _value[index];
		};

	protected:
		void parse(std::string source)
		{
			std::stringstream  lineStream(source);
		    std::string        cell;
		    while(std::getline(lineStream,cell,','))
		    {
		    	_value.push_back(cell);
		    }
		};
		
		std::vector<std::string> _value;
	};
};