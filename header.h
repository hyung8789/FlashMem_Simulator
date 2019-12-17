#define _CRT_SECURE_NO_WARNINGS //_CRT_SECURE_NO_WARNINGS
#include <stdio.h> //fscanf,fprinf,fwrite,fread
#include <iostream> //�����
#include <sstream> //stringstream
#include <windows.h> //�ý��� ��ɾ�
#define MB_PER_BLOCK 64 //1MB�� 64 ���
#define MB_PER_SECTOR 2048 //1MB�� 2048����
#define MB_PER_BYTE 1048576 //1MB�� ����Ʈ ȯ�갪(MB_PER_SECTOR * BYTE_PER_SECTOR / MB_PER_BLOCK)
#define BLOCK_PER_SECTOR 32 //�� ��Ͽ� �ش��ϴ� ������ ����
#define BYTE_PER_BLOCK 16384 //�� ��Ͽ� �ش��ϴ� ����Ʈ�� ũ��(BYTE_PER_SECTOR * BLOCK_PER_SECTOR)
#define BYTE_PER_SECTOR 512 //�� ���Ϳ� �ش��ϴ� ����Ʈ�� ũ��
//#define MAX_OVERWRITE_COUNT 2 //�� ������ ��� Ƚ�� ����

/***
1���� = 512byte
1��� = 32���� = 32*512 = 16,384byte
1MB = 1024KB
1KB = 1024byte
1MB�� ���� �� = (1*1024*1024)/512(1���ʹ� byte) = 2048��
1MB�� ��� ��(32����) = 2048/32 = 64��
--
unsigned int arrange == unsigned long arrange : 0 ~ 4,294,967,295
4 Gigabyte�� byte ������ ��ȯ�ϸ� 4,294,967,296byte�̹Ƿ� �ڷ����� ������ �Ѿ
����, �� ū �뷮�� �÷��� �޸𸮸� �����ϱ� ���ؼ���
unsigned __int64�� ����Ͽ��� �ϴµ� ���� ������ _fseeki64�� __int64(long long)������ �����ϹǷ� __int64�� ���
--
unsigned __int64 arrange: 0 ~ 18,446,744,073,709,551,615
__int64 arrange : ?9,223,372,036,854,775,808 ~ 9,223,372,036,854,775,807
--
������ ��� Ƚ�� ������ ����
�̹� �Էµ� ��ġ�� ������ �Է� �õ��� overwrite ���� �߻�
***/

class FlashMem {
public:
	FlashMem();
	FlashMem(__int64 megabytes); //megabytes ũ���� �÷��� �޸� ����

	void chk_initialized(FlashMem **flash); //�÷��� �޸𸮰� �̹� �����Ǿ� �ִ��� Ȯ��
	__int64 get_flashmem_size(); //�÷��� �޸� ũ�� ��ȯ
	__int64 calc_block_size(); //�Ҵ�� �޸� ũ�⿡ �ش��ϴ� ����� ���� ��ȯ
	__int64 calc_sector_size(); //�Ҵ�� �޸� ũ�⿡ �ش��ϴ� ������ ���� ��ȯ
	__int64 calc_storage_byte(); //������ ��������� byte�� �迭�� �����ϱ� ���ؼ� �� byte ũ�⸦ ��ȯ

private:
	__int64 flashmem_size; //�÷��ø޸��� MBũ��
};

int init(FlashMem **flashmem, __int64 megabytes); //megabytes ũ���� �÷��� �޸𸮸� �Ҵ�
int Flash_read(FlashMem **flashmem, __int64 PSN); //���� ���Ϳ� �����͸� �о��
int Flash_write(FlashMem **flashmem, __int64 PSN, char data); //���� ���Ϳ� �����͸� ���
int Flash_erase(FlashMem **flashmem, __int64 PBN); //���� ��Ͽ� �ش��ϴ� �����͸� ����