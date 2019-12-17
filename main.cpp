#include "header.h"

FlashMem *flash = NULL;

void main() {
	flash->chk_initialized(&flash); //이미 생성된 플래시 메모리가 있는지 확인

	while (1)
	{
		std::string user_input; //사용자로부터 명령어를 한 줄로 입력받는 변수

		///사용자로부터 입력받는 값들
		std::string command; //명령어
		char data=NULL; //기록하고자 하는 데이터
		__int64 megabytes = 0; //생성하고자 하는 플래시 메모리 용량
		__int64 PBN = 0; //물리 블록 번호
		__int64 PSN = 0; //물리 섹터 번호

		system("cls");
		std::cout << "=====================================================" << std::endl;
		std::cout << "	플래시 메모리 시뮬레이터" << std::endl;
		std::cout << "=====================================================" << std::endl;
		std::cout << " init x 또는 i x - x MB 만큼 플래시 메모리 생성 " << std::endl;
		std::cout << " read PSN 또는 r PSN - 물리 섹터의 데이터 읽기" << std::endl;
		std::cout << " write PSN data 또는 w PSN data - 물리 섹터의 data 기록" << std::endl;
		std::cout << " erase PBN 또는 e PBN - 물리 블록의 데이터 삭제" << std::endl;
		std::cout << "=====================================================" << std::endl;
		if (flash != NULL) //현재 생성된 플래시 메모리의 정보 보여주기
		{
			std::cout << "현재 생성된 플래시 메모리의 용량 : " << flash->get_flashmem_size() << "MB(" << flash->calc_storage_byte() << "bytes)" << std::endl;
			std::cout << "블록 수 : " << flash->calc_block_size() <<" [범위 : 0~"<< flash->calc_block_size()-1<<"]"<< std::endl;
			std::cout << "섹터 수 : " << flash->calc_sector_size() <<" [범위 : 0~" << flash->calc_sector_size() - 1 << "]" << std::endl;
			std::cout << "=====================================================" << std::endl;
		}
		std::cout << "명령어 >> ";
		std::getline(std::cin, user_input); //한 줄로 입력받기
		std::stringstream ss(user_input); //분리
		ss >> command;

		if (command.compare("init") == 0 || command.compare("i") == 0) //megabytes 만큼 플래시 메모리 생성
		{
			ss >> megabytes;
			if (megabytes <= 0)
			{
				std::cout << "must over 0 megabytes" << std::endl;
				system("pause");
				continue;
			}
			init(&flash, megabytes);
			system("pause");
			continue;
		}
		else if (command.compare("read") == 0 || command.compare("r") == 0) //물리 섹터의 데이터 읽기
		{
			ss >> PSN;
			Flash_read(&flash, PSN);
			system("pause");
			continue;
		}
		else if (command.compare("write") == 0 || command.compare("w") == 0) //물리 섹터에 data 기록
		{
			ss >> PSN;
			ss >> data;
			if (data == NULL)
			{
				std::cout << "잘못된 명령어 입력" << std::endl;
				system("pause");
				continue;
			}
			Flash_write(&flash, PSN, data);
			system("pause");
			continue;
		}
		else if (command.compare("erase") == 0 || command.compare("e") == 0) //물리 블록 번호에 해당되는 블록의 데이터 삭제
		{
			ss >> PBN;
			Flash_erase(&flash, PBN);
			system("pause");
			continue;
		}
		else
		{
			std::cout << "잘못된 명령어 입력" << std::endl;
			system("pause");
			continue;
		}

	}
}