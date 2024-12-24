#ifndef __ERROR_H__
#define __ERROR_H__

enum class Error_code_t{
	ERROR_OK,
	ERROR_MEMORY_ALLOCATION_LINK,
	ERROR_INVALID_INDEX,
	ERROR_EMPTY_LIST,
	ERROR_ID_NOT_FOUND,
	ERROR_CLASSIFICATION_MISMATCH,
	ERROR_NO_PORTAL,
	ERROR_DUPLICATE_REGISTER,
	ERROR_INVALID_GRADE,
	ERROR_CONVERSION,
	ERROR_FILE_OPEN,
	ERROR_FILE_LOAD
};

#endif