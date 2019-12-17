#define _CRT_SECURE_NO_WARNINGS //_CRT_SECURE_NO_WARNINGS
#include <stdio.h> //fscanf,fprinf,fwrite,fread
#include <iostream> //입출력
#include <sstream> //stringstream
#include <windows.h> //시스템 명령어
#define MB_PER_BLOCK 64 //1MB당 64 블록
#define MB_PER_SECTOR 2048 //1MB당 2048섹터
#define MB_PER_BYTE 1048576 //1MB당 바이트 환산값(MB_PER_SECTOR * BYTE_PER_SECTOR / MB_PER_BLOCK)
#define BLOCK_PER_SECTOR 32 //한 블록에 해당하는 섹터의 개수
#define BYTE_PER_BLOCK 16384 //한 블록에 해당하는 바이트의 크기(BYTE_PER_SECTOR * BLOCK_PER_SECTOR)
#define BYTE_PER_SECTOR 512 //한 섹터에 해당하는 바이트의 크기
//#define MAX_OVERWRITE_COUNT 2 //한 섹터의 기록 횟수 제한

/***
1섹터 = 512byte
1블록 = 32섹터 = 32*512 = 16,384byte
1MB = 1024KB
1KB = 1024byte
1MB의 섹터 수 = (1*1024*1024)/512(1섹터당 byte) = 2048개
1MB의 블록 수(32섹터) = 2048/32 = 64개
--
unsigned int arrange == unsigned long arrange : 0 ~ 4,294,967,295
4 Gigabyte를 byte 단위로 변환하면 4,294,967,296byte이므로 자료형의 범위를 넘어섬
따라서, 더 큰 용량의 플래시 메모리를 생성하기 위해서는
unsigned __int64를 사용하여야 하는데 파일 포인터 _fseeki64가 __int64(long long)까지만 지원하므로 __int64를 사용
--
unsigned __int64 arrange: 0 ~ 18,446,744,073,709,551,615
__int64 arrange : ?9,223,372,036,854,775,808 ~ 9,223,372,036,854,775,807
--
섹터의 기록 횟수 제한은 없음
이미 입력된 위치에 데이터 입력 시도시 overwrite 오류 발생
***/

class FlashMem {
public:
	FlashMem();
	FlashMem(__int64 megabytes); //megabytes 크기의 플래시 메모리 생성

	void chk_initialized(FlashMem **flash); //플래시 메모리가 이미 생성되어 있는지 확인
	__int64 get_flashmem_size(); //플래시 메모리 크기 반환
	__int64 calc_block_size(); //할당된 메모리 크기에 해당하는 블록의 개수 반환
	__int64 calc_sector_size(); //할당된 메모리 크기에 해당하는 섹터의 개수 반환
	__int64 calc_storage_byte(); //데이터 저장공간을 byte형 배열로 선언하기 위해서 총 byte 크기를 반환

private:
	__int64 flashmem_size; //플래시메모리의 MB크기
};

int init(FlashMem **flashmem, __int64 megabytes); //megabytes 크기의 플래시 메모리를 할당
int Flash_read(FlashMem **flashmem, __int64 PSN); //물리 섹터에 데이터를 읽어옴
int Flash_write(FlashMem **flashmem, __int64 PSN, char data); //물리 섹터에 데이터를 기록
int Flash_erase(FlashMem **flashmem, __int64 PBN); //물리 블록에 해당하는 데이터를 지움