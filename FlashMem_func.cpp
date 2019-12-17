#include "header.h"

//플래시 메모리의 생성 및 스토리지 용량을 계산하는 내부 함수 정의

FlashMem::FlashMem()
{
	flashmem_size = 0;
}

FlashMem::FlashMem(__int64 megabytes) //megabytes 크기의 플래시 메모리 생성
{
	flashmem_size = megabytes;
}

void FlashMem::chk_initialized(FlashMem **flash) //플래시 메모리가 이미 생성되어 있는지 확인
{
	if ((*flash) == NULL)
	{
		FILE *volume = NULL;

		if ((volume = fopen("volume.txt", "rt")) == NULL) //읽기 + 텍스트파일 모드
		{
			return;
		}
		else
		{
			while (1)
			{
				system("cls");
				int input = 0;
				std::cout << "already initialized continue?" << std::endl;
				std::cout << "0: ignore, 1: continue" << std::endl;
				std::cin >> input;
				if (input == 0)
				{
					std::cin.clear(); //오류스트림 초기화
					std::cin.ignore(INT_MAX, '\n'); //입력버퍼비우기
					break;
				}
				else if (input < 0 || input > 1)
				{
					std::cin.clear(); //오류스트림 초기화
					std::cin.ignore(INT_MAX, '\n'); //입력버퍼비우기
				}
				else //기존에 저장된 플래시 메모리의 용량 불러와서 재할당
				{
					__int64 megabytes;
					fscanf(volume,"%I64d",&megabytes);
					(*flash) = new FlashMem(megabytes); //용량 설정
					std::cin.clear(); //오류스트림 초기화
					std::cin.ignore(INT_MAX, '\n'); //입력버퍼비우기
					break;
				}
			}
			if (volume != NULL)	fclose(volume);
			return;
		}
	}
	else return;
}

__int64 FlashMem::get_flashmem_size() //플래시 메모리 크기 반환
{
	return flashmem_size;
}

__int64 FlashMem::calc_block_size() //할당된 메모리 크기에 해당하는 블록의 개수 반환
{
	return flashmem_size*MB_PER_BLOCK;
}

__int64 FlashMem::calc_sector_size() //할당된 메모리 크기에 해당하는 섹터의 개수 반환
{
	return calc_block_size()*BLOCK_PER_SECTOR;
}

__int64 FlashMem::calc_storage_byte() //데이터 저장공간을 byte형 배열로 선언하기 위해서 총 byte 크기를 반환
{
	return calc_sector_size()*BYTE_PER_SECTOR;
}