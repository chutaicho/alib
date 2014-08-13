//
//  Created by Takashi Aoki (c) takashiaoki.com
//
#pragma once

#include <iostream>
#include <typeinfo>
#include <fstream>
#include <map>

#include <json/json.h>

/*
loader utilities.
*/

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

		std::string getRawString() const { return _data; };

	protected:
		std::string _data;

	};

#pragma mark -
#pragma mark JSON

	/*
	
	example:
	at::JSONLoader json;
	json.load(ofToDataPath("test.json"));	
	JsonElement& value = json.getJSON();
	std::cout << value["firstName"].asString();

	*/
	
	class JSONLoader : public Buffer
	{
	public:
		JSONLoader(){};
		~JSONLoader(){};

		void load(const std::string& file)
		{
			if(file.find("http://")==0 )
		    {

			}
		    else if(file.find("https://")==0)
		    {

			}
		    else
		    {
				if(get(file))
				{
					Json::Reader read;
					if(!read.parse(_data, _value))
					{
						std::cout << "Error @ JSONLoader: " << read.getFormatedErrorMessages() << std::endl;
					}
				}
			}
		};

		JsonElement& getJSON() { return _value; };

	protected:
		JsonElement _value;
	};

#pragma mark -
#pragma mark CSV
	
	template <class T>
	class CSV
	{
	public:
		CSV(){};
		~CSV(){};
	    
		void load(const std::string& file)
		{
		
		};

	protected:
		std::vector<T> _value;
	};

};