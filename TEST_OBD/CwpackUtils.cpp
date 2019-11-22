#include "stdafx.h"
#include "CwpackUtils.h"


CwpackUtils::CwpackUtils()
{
}


CwpackUtils::~CwpackUtils()
{
}

int CwpackUtils::CheckString(cw_unpack_context *uc, char* name)
{
	cw_unpack_next(uc);

	if (CWP_RC_OK != uc->return_code){
		return RET_PARSER;
	}

	if (uc->item.type != CWP_ITEM_STR){
		return RET_PARSER;
	}

	if (0 != strncmp(name, static_cast<const char *>(uc->item.as.str.start), uc->item.as.str.length)){
		return RET_PARSER;
	}

	return RET_OK;
}

int CwpackUtils::CheckMap(cw_unpack_context *uc, uint8_t *size)
{
	cw_unpack_next(uc);

	if (CWP_RC_OK != uc->return_code){
		return RET_PARSER;
	}

	if (CWP_ITEM_MAP != uc->item.type){
		return RET_PARSER;
	}

	*size = uc->item.as.map.size;

	return RET_OK;
}

int CwpackUtils::ReadString(cw_unpack_context *uc, char* buf, int buflen)
{
	int cplen;

	cw_unpack_next(uc);

	if (CWP_RC_OK != uc->return_code){
		return RET_PARSER;
	}

	if (uc->item.type != CWP_ITEM_STR){
		return RET_PARSER;
	}

	cplen = uc->item.as.str.length;
	if (cplen >= buflen){
		cplen = buflen - 1;
	}
	memcpy(buf, uc->item.as.str.start, cplen);
	buf[cplen] = 0;

	return RET_OK;
}

int CwpackUtils::ReadBin(cw_unpack_context *uc, uint8_t* buf, int buflen, int *outlen)
{
	int cplen;

	cw_unpack_next(uc);

	if (CWP_RC_OK != uc->return_code){
		return RET_PARSER;
	}

	if (uc->item.type != CWP_ITEM_BIN){
		return RET_PARSER;
	}

	cplen = uc->item.as.bin.length;
	if (cplen > buflen){
		cplen = buflen;
	}
	memcpy(buf, uc->item.as.bin.start, cplen);
	*outlen = cplen;

	return RET_OK;
}

int CwpackUtils::ReadUnsigned(cw_unpack_context *uc, uint64_t *i)
{
	cw_unpack_next(uc);

	if (CWP_RC_OK != uc->return_code){
		return RET_PARSER;
	}

	if (uc->item.type != CWP_ITEM_POSITIVE_INTEGER){
		return RET_PARSER;
	}

	*i = uc->item.as.u64;

	return RET_OK;
}

int CwpackUtils::ReadSigned(cw_unpack_context *uc, uint64_t *i)
{
	cw_unpack_next(uc);

	if (CWP_RC_OK != uc->return_code){
		return RET_PARSER;
	}

	if (uc->item.type != CWP_ITEM_POSITIVE_INTEGER){
		return RET_PARSER;
	}

	*i = uc->item.as.i64;

	return RET_OK;
}

int CwpackUtils::ReadU8Array(cw_unpack_context *uc, uint8_t *arr, uint8_t size, uint16_t *outsize)
{
	uint64_t outi;
	int readsize;
	int i;
	int ret;

	cw_unpack_next(uc);

	if (CWP_RC_OK != uc->return_code){
		return RET_PARSER;
	}

	if (uc->item.type != CWP_ITEM_ARRAY){
		return RET_PARSER;
	}

	readsize = uc->item.as.array.size;
	if (readsize > size){
		readsize = size;
	}

	
	ret = RET_PARSER;
	for (i = 0; i < readsize; ++i){
		ret =ReadUnsigned(uc, &outi);
		if (RET_OK != ret){
			break;
		}

		arr[i] = (uint8_t)outi;
	}
	*outsize = (uint16_t)readsize;

	return ret;
}

int CwpackUtils::ReadU16Array(cw_unpack_context *uc, uint16_t *arr, uint8_t size, uint16_t *outsize)
{
	uint64_t outi;
	int readsize;
	int i;
	int ret;

	cw_unpack_next(uc);

	if (CWP_RC_OK != uc->return_code){
		return RET_PARSER;
	}

	if (uc->item.type != CWP_ITEM_ARRAY){
		return RET_PARSER;
	}

	readsize = uc->item.as.array.size;
	if (readsize > size){
		readsize = size;
	}

	/* ¶ÁÈ¡Êý×éÔªËØ */
	ret = RET_PARSER;
	for (i = 0; i < readsize; ++i){
		ret = ReadUnsigned(uc, &outi);
		if (RET_OK != ret){
			break;
		}

		arr[i] = (uint16_t)outi;
	}
	*outsize = (uint16_t)readsize;

	return ret;
}

int CwpackUtils::ReadU32Array(cw_unpack_context *uc, uint32_t *arr, uint8_t size, uint16_t *outsize)
{
	uint64_t outi;
	int readsize;
	int i;
	int ret;

	cw_unpack_next(uc);

	if (CWP_RC_OK != uc->return_code){
		return RET_PARSER;
	}

	if (uc->item.type != CWP_ITEM_ARRAY){
		return RET_PARSER;
	}

	readsize = uc->item.as.array.size;
	if (readsize > size){
		readsize = size;
	}

	ret = RET_PARSER;
	for (i = 0; i < readsize; ++i){
		ret = ReadUnsigned(uc, &outi);
		if (RET_OK != ret){
			break;
		}

		arr[i] = (uint32_t)outi;
	}
	*outsize = (uint16_t)readsize;

	return ret;
}
