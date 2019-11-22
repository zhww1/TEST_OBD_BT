#pragma once

#include  "cwpack.h"
/** ͨ�ô������ */
#define RET_OK  0      		/*!< �ɹ� */
#define RET_ERR (-1)   		/*!< ʧ�� */
#define RET_OVERFLOW  (-2) 	/*< ���*/
#define RET_PARSER	  (-3)	/*!< �������� */ 
class CwpackUtils
{
public:
	CwpackUtils();
	~CwpackUtils();

public:

	int  CheckString(cw_unpack_context *uc, char* name);
	int	 CheckMap(cw_unpack_context *uc, uint8_t *size);
	int  ReadString(cw_unpack_context *uc, char* buf, int buflen);
	int  ReadBin(cw_unpack_context *uc, uint8_t* buf, int buflen, int *outlen);
	int  ReadUnsigned(cw_unpack_context *uc, uint64_t *i);
	int  ReadSigned(cw_unpack_context *uc, uint64_t *i);
	int  ReadU8Array(cw_unpack_context *uc, uint8_t *array, uint8_t size, uint16_t *outsize);
	int  ReadU16Array(cw_unpack_context *uc, uint16_t *array, uint8_t size, uint16_t *outsize);
	int  ReadU32Array(cw_unpack_context *uc, uint32_t *array, uint8_t size, uint16_t *outsize);

};

