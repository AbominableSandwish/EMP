#include <core/file.h>
#include <iostream>
#include <fstream>
#include <core/log.h>

namespace emp
{
	FileManager::FileManager(Engine& engine, string name): System(engine, name)
	{
	}

	void FileManager::Init()
	{	
		this->queue_Data = queue<Data>();
		this->queue_Data.push(Data("data.txt", ios_base::out));
		
	}

	void FileManager::Update(float)
	{
		while (this->queue_Data.size() != 0) {
			Data data = this->queue_Data.front();
			if (data.action == ios_base::app || data.action == ios_base::out) {
				this->WriteFile(data);
			}
			this->queue_Data.pop();
		}
	}

	void FileManager::Destroy()
	{
	}
	void FileManager::WriteFile(Data data)
	{
		//C not c++
		//FILE* myfile;
		//myfile = fopen((data.name).c_str(), &data.action);
		//fprintf(myfile, "Writing this to a file.\n");
		//fclose(myfile); // must close after opening

		std::ofstream outfile;
		outfile.open(data.name, data.action); // append instead of overwrite
		outfile << "Data";
		outfile.close();
	}
}
