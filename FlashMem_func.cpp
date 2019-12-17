#include "header.h"

//�÷��� �޸��� ���� �� ���丮�� �뷮�� ����ϴ� ���� �Լ� ����

FlashMem::FlashMem()
{
	flashmem_size = 0;
}

FlashMem::FlashMem(__int64 megabytes) //megabytes ũ���� �÷��� �޸� ����
{
	flashmem_size = megabytes;
}

void FlashMem::chk_initialized(FlashMem **flash) //�÷��� �޸𸮰� �̹� �����Ǿ� �ִ��� Ȯ��
{
	if ((*flash) == NULL)
	{
		FILE *volume = NULL;

		if ((volume = fopen("volume.txt", "rt")) == NULL) //�б� + �ؽ�Ʈ���� ���
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
					std::cin.clear(); //������Ʈ�� �ʱ�ȭ
					std::cin.ignore(INT_MAX, '\n'); //�Է¹��ۺ���
					break;
				}
				else if (input < 0 || input > 1)
				{
					std::cin.clear(); //������Ʈ�� �ʱ�ȭ
					std::cin.ignore(INT_MAX, '\n'); //�Է¹��ۺ���
				}
				else //������ ����� �÷��� �޸��� �뷮 �ҷ��ͼ� ���Ҵ�
				{
					__int64 megabytes;
					fscanf(volume,"%I64d",&megabytes);
					(*flash) = new FlashMem(megabytes); //�뷮 ����
					std::cin.clear(); //������Ʈ�� �ʱ�ȭ
					std::cin.ignore(INT_MAX, '\n'); //�Է¹��ۺ���
					break;
				}
			}
			if (volume != NULL)	fclose(volume);
			return;
		}
	}
	else return;
}

__int64 FlashMem::get_flashmem_size() //�÷��� �޸� ũ�� ��ȯ
{
	return flashmem_size;
}

__int64 FlashMem::calc_block_size() //�Ҵ�� �޸� ũ�⿡ �ش��ϴ� ����� ���� ��ȯ
{
	return flashmem_size*MB_PER_BLOCK;
}

__int64 FlashMem::calc_sector_size() //�Ҵ�� �޸� ũ�⿡ �ش��ϴ� ������ ���� ��ȯ
{
	return calc_block_size()*BLOCK_PER_SECTOR;
}

__int64 FlashMem::calc_storage_byte() //������ ��������� byte�� �迭�� �����ϱ� ���ؼ� �� byte ũ�⸦ ��ȯ
{
	return calc_sector_size()*BYTE_PER_SECTOR;
}