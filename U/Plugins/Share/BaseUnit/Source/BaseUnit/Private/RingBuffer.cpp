#include "RingBuffer.h"
#include "BaseUnitDef.h"
#include "BaseUnitLog.h"

FRingBuffer::FRingBuffer()
{
	Clear();
}

FRingBuffer::~FRingBuffer()
{

}

bool FRingBuffer::SndPack(const void* pData, uint32 datasize)
{
	if (pData == nullptr || datasize == 0)
		return false;

	if (m_startptr == nullptr)
		return false;

	if (datasize > m_uMaxPack)
		return false;

	char* leftptr = m_leftptr;
	char* rightptr = m_rightptr;

	/*
	 *	�ж��Ƿ���
	 */
	 ////////////////////////////////////////////////////////
	uint32 uContentSize = 0;
	uint32 uEmptySize = 0;

	if (rightptr >= leftptr)	//û�л���
		uContentSize = (uint32)(rightptr - leftptr);
	else
		uContentSize = (uint32)(m_uBufLen - (leftptr - rightptr));

	if (uContentSize > m_uBufLen - 1)
		return false;

	uEmptySize = m_uBufLen - uContentSize - 1;
	////////////////////////////////////////////////////////

	/*
	 *	û�ռ���
	 */
	if (datasize + sizeof(uint32) > uEmptySize)
		return false;

	//д�볤��
	rightptr = PutData(leftptr, rightptr, (char*)& datasize, sizeof(uint32));

	if (rightptr == nullptr)
		return false;

	//д������
	rightptr = PutData(leftptr, rightptr, (char*)pData, datasize);

	if (rightptr == nullptr)
		return false;

	//�޸�ָ��
	m_rightptr = rightptr;

	return true;
}

bool FRingBuffer::SndPack(const void* pData1, const void* pData2, uint32 datasize1, uint32 datasize2)
{
	if (pData1 == nullptr || pData2 || 0 == datasize1 || 0 == datasize2)
		return false;

	uint32	uiTotalSize = datasize1 + datasize2;

	if (m_startptr == nullptr)
		return false;

	if (uiTotalSize > m_uMaxPack)
		return false;

	char* leftptr = m_leftptr;
	char* rightptr = m_rightptr;

	/*
	 *	�ж��Ƿ���
	 */
	 ////////////////////////////////////////////////////////
	uint32 uContentSize = 0;
	uint32 uEmptySize = 0;

	if (rightptr >= leftptr)	//û�л���
		uContentSize = (uint32)(rightptr - leftptr);
	else
		uContentSize = (uint32)(m_uBufLen - (leftptr - rightptr));

	if (uContentSize > m_uBufLen - 1)
		return false;

	uEmptySize = m_uBufLen - uContentSize - 1;
	////////////////////////////////////////////////////////

	/*
	 *	û�ռ���
	 */
	if (uiTotalSize + sizeof(uint32) > uEmptySize)
		return false;

	//д�볤��
	rightptr = PutData(leftptr, rightptr, (char*)& uiTotalSize, sizeof(uint32));

	if (rightptr == nullptr)
		return false;

	//д������1
	rightptr = PutData(leftptr, rightptr, (char*)pData1, datasize1);

	if (rightptr == nullptr)
		return false;

	//д������2
	rightptr = PutData(leftptr, rightptr, (char*)pData2, datasize2);

	if (rightptr == nullptr)
		return false;

	//�޸�ָ��
	m_rightptr = rightptr;

	return true;
}

const void* FRingBuffer::RcvPack(uint32& datasize)
{
	datasize = 0;

	if (m_startptr == nullptr)
		return nullptr;

	//�ͷ���һ������
	m_leftptr = m_nextleft;
	char* leftptr = m_leftptr;
	char* rightptr = m_rightptr;

	/*
	 *	�ж��Ƿ���
	 */
	 ////////////////////////////////////////////////////////
	uint32 uContentSize = 0;

	if (rightptr >= leftptr)	//û�л���
		uContentSize = (uint32)(rightptr - leftptr);
	else
		uContentSize = (uint32)(m_uBufLen - (leftptr - rightptr));

	if (uContentSize > m_uBufLen - 1)
		return nullptr;
	////////////////////////////////////////////////////////

	if (uContentSize <= sizeof(uint32))
		return nullptr;

	datasize = GetDataLen(leftptr, rightptr);

	if (uContentSize < datasize ||
		datasize > m_uMaxPack)
	{
		/*
		 *	��Ӧ�ò��������
		 */
		datasize = 0;
		return nullptr;
	}

	/*
	 *	һ�𿽱�,�ܳ���
	 */
	uint32 uReadLen = datasize + sizeof(uint32);

	//��ָ���ұ߿ռ�
	uint32 uRightMargin = (uint32)(m_uBufLen - (leftptr - m_startptr));

	if (rightptr < leftptr &&
		uReadLen >= uRightMargin)
	{
		/*
		 *	�����ˣ��������ݳ��Ȳ�����ȡ
		 */
		memcpy(m_tmpptr, leftptr, uRightMargin);

		memcpy(m_tmpptr + uRightMargin, m_startptr, uReadLen - uRightMargin);

		if (uReadLen - uRightMargin > m_uBufLen)
		{
			datasize = 0;
			return NULL;
		}
		//�޸�ָ��
		m_nextleft = m_startptr + (uReadLen - uRightMargin);

		return (m_tmpptr + sizeof(uint32));
	}
	else
	{
		//�޸�ָ��
		if (m_nextleft + uReadLen > m_startptr + m_uBufLen)
		{
			datasize = 0;
			return nullptr;
		}

		m_nextleft += uReadLen;
		return (leftptr + sizeof(uint32));
	}
}

bool FRingBuffer::Init(char* StartPtr, uint32 uBufLen, uint32 uMaxPack)
{
	if (uBufLen <= (2 * uMaxPack + RB_SPACE))
		return false;

	if (StartPtr == nullptr)
		return false;

	Clear();

	memset(StartPtr, 0, uBufLen);

	m_startptr = StartPtr;

	m_uMaxPack = uMaxPack;
	m_uBufLen = uBufLen - (m_uMaxPack + RB_SPACE);	//����8�ֽ�

	m_leftptr = m_startptr;
	m_rightptr = m_startptr;
	m_tmpptr = m_startptr + (m_uBufLen + RB_SPACE);
	m_nextleft = m_startptr;

	return true;
}

bool FRingBuffer::Reinit()
{
	if (m_startptr == nullptr)
		return false;

	m_leftptr = m_startptr;
	m_rightptr = m_startptr;
	m_nextleft = m_startptr;

	return true;
}

char* FRingBuffer::PutData(char* leftptr, char* rightptr, char* pData, uint32 datasize)
{
	//�ұ߿ռ�
	uint32 uRightMargin = (uint32)(m_uBufLen - (rightptr - m_startptr));

	if (rightptr >= leftptr &&
		datasize >= uRightMargin)
	{
		/*
		 *	û�л���,�����ұ߿ռ䲻��,��������
		 */
		memcpy(rightptr, pData, uRightMargin);
		rightptr = m_startptr;
		memcpy(rightptr, pData + uRightMargin, datasize - uRightMargin);
		rightptr += datasize - uRightMargin;

		if (rightptr > m_startptr + m_uBufLen)
			return nullptr;

		return rightptr;
	}

	//������,����û�л��ƣ������ұ߿ռ乻��
	memcpy(rightptr, pData, datasize);
	rightptr += datasize;

	if (rightptr > m_startptr + m_uBufLen)
		return nullptr;

	return rightptr;
}

uint32 FRingBuffer::GetDataLen(char* leftptr, char* rightptr)
{
	//��ָ���ұ߿ռ�
	uint32 uRightMargin = (uint32)(m_uBufLen - (leftptr - m_startptr));

	if (rightptr < leftptr &&
		sizeof(uint32) > uRightMargin)
	{
		/*
		 *	�����ˣ��������ݳ��Ȳ�����ȡ
		 */
		uint32 uDataLen = 0;
		char* ptr = (char*)& uDataLen;
		memcpy(ptr, leftptr, uRightMargin);
		memcpy(ptr + uRightMargin, m_startptr, sizeof(uint32) - uRightMargin);
		return uDataLen;
	}

	return *((uint32*)leftptr);
}

void FRingBuffer::Clear()
{
	m_startptr = nullptr;
	m_leftptr = nullptr;
	m_rightptr = nullptr;
	m_tmpptr = nullptr;
	m_nextleft = nullptr;
	m_uBufLen = 0;
	m_uMaxPack = 0;
}

