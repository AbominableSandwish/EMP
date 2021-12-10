#include <core/file.h>
#include <iostream>
#include <fstream>
#include <core/log.h>

namespace emp
{

	//// for convenience
	//using json = nlohmann::json;

	FileManager::FileManager(Engine& engine, string name) : System(engine, name)
	{
	}

	void FileManager::Init()
	{
		this->queue_Data = queue<Data>();
		this->queue_Data.push(Data("data.txt", ios_base::out));

		WriteFile(Data(), FormatFile::JSON);
	}

	void FileManager::Update(float)
	{
		while (this->queue_Data.size() != 0) {
			Data data = this->queue_Data.front();
			if (data.action == ios_base::app || data.action == ios_base::out) {
				this->WriteFile(data, FormatFile::TXT);
			}
			this->queue_Data.pop();
		}
	}

	void FileManager::Destroy()
	{
	}

	void FileManager::WriteFile(Data data, FormatFile format)
	{
		//C not c++
		//FILE* myfile;
		//myfile = fopen((data.name).c_str(), &data.action);
		//fprintf(myfile, "Writing this to a file.\n");
		//fclose(myfile); // must close after opening

		switch (format)
		{
		case TXT:
			//std::ofstream outfile;
			//outfile.open(data.name, data.action); // append instead of overwrite
			//outfile << "Data";
			//outfile.close();
			break;
		case JSON:

                //how to access values of JSON data
		{

			// jdEmployees
			json jdEmployees =
			{
				{"firstName","Sean"},
				{"lastName","Brown"},
				{"StudentID",21453},
				{"Department","Computer Sc."}
			};

			// Access the values
			std::string fName = jdEmployees.value("firstName", "oops");
			std::string lName = jdEmployees.value("lastName", "oops");
			int sID = jdEmployees.value("StudentID", 0);
			std::string dept = jdEmployees.value("Department", "oops");

			// Print the values
			std::cout << "First Name: " << fName << std::endl;
			std::cout << "Last Name: " << lName << std::endl;
			std::cout << "Student ID: " << sID << std::endl;
			std::cout << "Department: " << dept << std::endl;
		}
		{
			// Here is a JSON text
			char text[] = R"(
                          {
                            "Book": {
                            "Width":  450,
                            "Height": 30,
                            "Title":  "Hello World",
                            "isBiography": false,
                            "NumOfCopies": 4,
                            "LibraryIDs": [2319, 1406, 3854, 987]
                          }
                          }
                          )";
			// Let's parse and serialize JSON
			json j_complete = json::parse(text);
			std::cout << std::setw(4) << j_complete << std::endl;

			// write prettified JSON to another file
			std::ofstream o("data.json");
			o << std::setw(4) << j_complete << std::endl;
		}
		{
			// convert to JSON: copy each value into the JSON object
			json j;
			j["name"] = this->name;
			// ...

			// write prettified JSON to another file
			std::ofstream o("j.json");
			o << std::setw(4) << j << std::endl;
		}

		//// read a JSON file
		//std::ifstream i("file.json");
		//json j;
		//i >> j;
		break;
		}

	}

}
