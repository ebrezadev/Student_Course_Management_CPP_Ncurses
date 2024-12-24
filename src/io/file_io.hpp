/*This header file contains description of the FileIO virtual class, which is inherited by save and load state classes.*/
#ifndef __FILE_IO_HPP__
#define __FILE_IO_HPP__

#include <fstream>
#include "error.hpp"
#include "states.hpp"
#include <cstring>
#include <libgen.h>		  // dirname
#include <unistd.h>		  // readlink
#include <linux/limits.h> // PATH_MAX

class FileIO
{
public:
	FileIO(){};
	virtual ~FileIO() {};
	//open_file method opens two files in executable directory
	Error_code_t open_file(char *file_name, std::fstream &file, std::ios::openmode mode)
	{
		char path_to_dir[PATH_MAX];
		ssize_t count = readlink("/proc/self/exe", path_to_dir, PATH_MAX);
		if (count != -1)
		{
			dirname(path_to_dir);
		}

		char path[PATH_MAX];

		strcpy(path, path_to_dir);
		strcat(path, file_name);

		file.open(path, mode);

		if (!file.is_open())
		{
			return Error_code_t::ERROR_FILE_OPEN;
		}

		return Error_code_t::ERROR_OK;
	}

protected:
};

#endif
