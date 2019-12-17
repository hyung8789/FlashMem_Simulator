#include "header.h"

//init, read, write, erase 함수 정의
//플래시 메모리에 대해 물리적으로 접근하여 작업

int init(FlashMem **flashmem, __int64 megabytes) //megabytes 크기의 플래시 메모리를 할당
{
	FILE *fp = NULL;
	FILE *volume = NULL;

	//이미 플래시 메모리를 생성하였을 경우 다시 이니셜라이징시 덮어씌움
	*flashmem = new FlashMem(megabytes);
	
	__int64 sector_size = (*flashmem)->calc_sector_size(); //할당된 MB 크기에 해당하는 섹터의 개수
	__int64 storage_byte = (*flashmem)->calc_storage_byte(); //데이터 저장공간의 총 byte 크기
	char data_array[BYTE_PER_SECTOR] = {NULL}; //한 섹터 크기의 저장공간 생성

	//이진파일 형식으로 기록
	if ((fp = fopen("storage.bin", "wb")) == NULL) //쓰기 + 이진파일 모드
	{
		fprintf(stderr, "storage.bin 파일을 쓰기모드로 열 수 없습니다.");
		return 0;
	}

	//플래시 메모리 용량 기록
	if ((volume = fopen("volume.txt", "wt")) == NULL) //쓰기 + 텍스트파일 모드
	{
		fprintf(stderr, "volume.txt 파일을 쓰기모드로 열 수 없습니다.");
		if (fp != NULL)	fclose(fp);
		return 0;
	}

	//섹터의 개수만큼 파일에 기록
	__int64 init_pos_next = ftell(fp);
	while (1)
	{
		fwrite(data_array, sizeof(char), BYTE_PER_SECTOR, fp); //데이터 저장 공간 기록
		init_pos_next += BYTE_PER_SECTOR;
		if (init_pos_next >= storage_byte) break; //다음에 기록할 위치가 바이트단위로 환산한 저장공간의 용량을 넘을 경우 종료
	}
	
	//플래시 메모리 용량 기록
	fprintf(volume, "%I64d", (*flashmem)->get_flashmem_size());

	fclose(fp);
	fclose(volume);
	std::cout << megabytes << " megabytes flash memory" << std::endl;

	return 1;
}

int Flash_read(FlashMem **flashmem, __int64 PSN) //물리 섹터에 데이터를 읽어옴
{
	char buffer;

	if (*flashmem == NULL) //플래시 메모리가 할당되지 않았을 경우
	{
		std::cout << "not initialized" << std::endl;
		return 0;
	}

	if (PSN > (MB_PER_SECTOR * (*flashmem)->get_flashmem_size()) - 1) //범위 초과 오류
	{
		std::cout << "out of range error" << std::endl;
		return 0;
	}

	FILE *fp = NULL;
	if ((fp = fopen("storage.bin", "rb")) == NULL) //읽기 + 이진파일 모드
	{
		fprintf(stderr, "storage.bin 파일을 읽기모드로 열 수 없습니다.");
		return 0;
	}

	//하나의 섹터에 해당하는 byte환산 값 * PSN만큼 곱하여 건너뜀
	__int64 read_pos = BYTE_PER_SECTOR * PSN; //읽고자 하는 물리 섹터의 위치

	_fseeki64(fp, read_pos, SEEK_SET); //읽고자 하는 물리 섹터의 위치로 이동
	fread(&buffer, sizeof(char), 1, fp);  //해당 물리 섹터에 기록된 값 읽기

	if (buffer == NULL)
		std::cout << "no data" << std::endl;
	else
		std::cout << buffer << std::endl;

	fclose(fp);

	return 1;
}

int Flash_write(FlashMem **flashmem, __int64 PSN, char data) //물리 섹터에 데이터를 기록
{
	char buffer;

	//이미 입력된 위치에 데이터 입력 시도시 overwrite 오류 발생
	if (*flashmem == NULL) //플래시 메모리가 할당되지 않았을 경우
	{
		std::cout << "not initialized" << std::endl;
		return 0;
	}

	if (PSN > (MB_PER_SECTOR * (*flashmem)->get_flashmem_size()) - 1) //범위 초과 오류
	{
		std::cout << "out of range error" << std::endl;
		return 0;
	}

	FILE *fp = NULL;
	if ((fp = fopen("storage.bin", "rb+")) == NULL) //읽고 쓰기 모드 + 이진파일 모드
	{
		fprintf(stderr, "storage.bin 파일을 읽고 쓰기 모드로 열 수 없습니다.");
		return 0;
	}

	__int64 write_pos = BYTE_PER_SECTOR * PSN; //쓰고자 하는 위치

	_fseeki64(fp, write_pos, SEEK_SET); //읽고자 하는 물리 섹터의 위치로 이동
	fread(&buffer, sizeof(char), 1, fp); //해당 물리 섹터에 기록된 값 읽기
	_fseeki64(fp, write_pos, SEEK_SET); //쓰고자 하는 물리 섹터의 위치로 이동

	if (buffer != NULL) 
	{
		std::cout << "overwrite error" << std::endl;
		fclose(fp);
		return 0;
	}
	else
	{
		fwrite(&data, sizeof(char), 1, fp);
		std::cout << "done" << std::endl;
	}

	fclose(fp);
	return 1;
}

int Flash_erase(FlashMem **flashmem, __int64 PBN) //물리 블록에 해당하는 데이터를 지움
{
	char erase_buffer = NULL; //지우고자 할 때 덮어씌우고자 할 값

	//해당 블록이 속한 섹터들에 대해서 모두 erase
	if (*flashmem == NULL) //플래시 메모리가 할당되지 않았을 경우
	{
		std::cout << "not initialized" << std::endl;
		return 0;
	}

	if (PBN > (MB_PER_BLOCK * (*flashmem)->get_flashmem_size()) - 1) //범위 초과 오류
	{
		std::cout << "out of range error" << std::endl;
		return 0;
	}

	__int64 erase_pos_start = BYTE_PER_BLOCK * PBN; //지우고자 하는 블록 위치의 시작 
	__int64 erase_pos_end = (BYTE_PER_BLOCK * (PBN+1)) -1; //지우고자 하는 블록 위치의 끝

	FILE *fp = NULL;
	if ((fp = fopen("storage.bin", "rb+")) == NULL) //읽고 쓰기모드 + 이진파일 모드(쓰고 읽기 모드로 열 경우 파일내용이 모두 초기화)
	{
		fprintf(stderr, "storage.bin 파일을 읽고 쓰기 모드로 열 수 없습니다.");
		return 0;
	}

	_fseeki64(fp, erase_pos_start, SEEK_SET); //erase하고자 하는 물리 블록의 시작 위치로 이동

	__int64 erase_pos_next = ftell(fp); //erase할 다음 섹터의 위치
	while(1) //해당 블록 위치의 끝까지 반복
	{
		fwrite(&erase_buffer, sizeof(char), 1, fp);
		erase_pos_next += BYTE_PER_SECTOR;
		if ((erase_pos_next > erase_pos_end)) break;
		else _fseeki64(fp, erase_pos_next, SEEK_SET); //한 섹터에 대한 바이트 단위(512byte)로 이동
	}

	fclose(fp);
	std::cout << PBN << "-th block erased" << std::endl;

	return 1;
}