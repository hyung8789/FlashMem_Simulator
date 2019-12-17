#include "header.h"

FlashMem *flash = NULL;

void main() {
	flash->chk_initialized(&flash); //�̹� ������ �÷��� �޸𸮰� �ִ��� Ȯ��

	while (1)
	{
		std::string user_input; //����ڷκ��� ��ɾ �� �ٷ� �Է¹޴� ����

		///����ڷκ��� �Է¹޴� ����
		std::string command; //��ɾ�
		char data=NULL; //����ϰ��� �ϴ� ������
		__int64 megabytes = 0; //�����ϰ��� �ϴ� �÷��� �޸� �뷮
		__int64 PBN = 0; //���� ��� ��ȣ
		__int64 PSN = 0; //���� ���� ��ȣ

		system("cls");
		std::cout << "=====================================================" << std::endl;
		std::cout << "	�÷��� �޸� �ùķ�����" << std::endl;
		std::cout << "=====================================================" << std::endl;
		std::cout << " init x �Ǵ� i x - x MB ��ŭ �÷��� �޸� ���� " << std::endl;
		std::cout << " read PSN �Ǵ� r PSN - ���� ������ ������ �б�" << std::endl;
		std::cout << " write PSN data �Ǵ� w PSN data - ���� ������ data ���" << std::endl;
		std::cout << " erase PBN �Ǵ� e PBN - ���� ����� ������ ����" << std::endl;
		std::cout << "=====================================================" << std::endl;
		if (flash != NULL) //���� ������ �÷��� �޸��� ���� �����ֱ�
		{
			std::cout << "���� ������ �÷��� �޸��� �뷮 : " << flash->get_flashmem_size() << "MB(" << flash->calc_storage_byte() << "bytes)" << std::endl;
			std::cout << "��� �� : " << flash->calc_block_size() <<" [���� : 0~"<< flash->calc_block_size()-1<<"]"<< std::endl;
			std::cout << "���� �� : " << flash->calc_sector_size() <<" [���� : 0~" << flash->calc_sector_size() - 1 << "]" << std::endl;
			std::cout << "=====================================================" << std::endl;
		}
		std::cout << "��ɾ� >> ";
		std::getline(std::cin, user_input); //�� �ٷ� �Է¹ޱ�
		std::stringstream ss(user_input); //�и�
		ss >> command;

		if (command.compare("init") == 0 || command.compare("i") == 0) //megabytes ��ŭ �÷��� �޸� ����
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
		else if (command.compare("read") == 0 || command.compare("r") == 0) //���� ������ ������ �б�
		{
			ss >> PSN;
			Flash_read(&flash, PSN);
			system("pause");
			continue;
		}
		else if (command.compare("write") == 0 || command.compare("w") == 0) //���� ���Ϳ� data ���
		{
			ss >> PSN;
			ss >> data;
			if (data == NULL)
			{
				std::cout << "�߸��� ��ɾ� �Է�" << std::endl;
				system("pause");
				continue;
			}
			Flash_write(&flash, PSN, data);
			system("pause");
			continue;
		}
		else if (command.compare("erase") == 0 || command.compare("e") == 0) //���� ��� ��ȣ�� �ش�Ǵ� ����� ������ ����
		{
			ss >> PBN;
			Flash_erase(&flash, PBN);
			system("pause");
			continue;
		}
		else
		{
			std::cout << "�߸��� ��ɾ� �Է�" << std::endl;
			system("pause");
			continue;
		}

	}
}