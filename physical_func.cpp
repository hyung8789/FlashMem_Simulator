#include "header.h"

//init, read, write, erase �Լ� ����
//�÷��� �޸𸮿� ���� ���������� �����Ͽ� �۾�

int init(FlashMem **flashmem, __int64 megabytes) //megabytes ũ���� �÷��� �޸𸮸� �Ҵ�
{
	FILE *fp = NULL;
	FILE *volume = NULL;

	//�̹� �÷��� �޸𸮸� �����Ͽ��� ��� �ٽ� �̴ϼȶ���¡�� �����
	*flashmem = new FlashMem(megabytes);
	
	__int64 sector_size = (*flashmem)->calc_sector_size(); //�Ҵ�� MB ũ�⿡ �ش��ϴ� ������ ����
	__int64 storage_byte = (*flashmem)->calc_storage_byte(); //������ ��������� �� byte ũ��
	char data_array[BYTE_PER_SECTOR] = {NULL}; //�� ���� ũ���� ������� ����

	//�������� �������� ���
	if ((fp = fopen("storage.bin", "wb")) == NULL) //���� + �������� ���
	{
		fprintf(stderr, "storage.bin ������ ������� �� �� �����ϴ�.");
		return 0;
	}

	//�÷��� �޸� �뷮 ���
	if ((volume = fopen("volume.txt", "wt")) == NULL) //���� + �ؽ�Ʈ���� ���
	{
		fprintf(stderr, "volume.txt ������ ������� �� �� �����ϴ�.");
		if (fp != NULL)	fclose(fp);
		return 0;
	}

	//������ ������ŭ ���Ͽ� ���
	__int64 init_pos_next = ftell(fp);
	while (1)
	{
		fwrite(data_array, sizeof(char), BYTE_PER_SECTOR, fp); //������ ���� ���� ���
		init_pos_next += BYTE_PER_SECTOR;
		if (init_pos_next >= storage_byte) break; //������ ����� ��ġ�� ����Ʈ������ ȯ���� ��������� �뷮�� ���� ��� ����
	}
	
	//�÷��� �޸� �뷮 ���
	fprintf(volume, "%I64d", (*flashmem)->get_flashmem_size());

	fclose(fp);
	fclose(volume);
	std::cout << megabytes << " megabytes flash memory" << std::endl;

	return 1;
}

int Flash_read(FlashMem **flashmem, __int64 PSN) //���� ���Ϳ� �����͸� �о��
{
	char buffer;

	if (*flashmem == NULL) //�÷��� �޸𸮰� �Ҵ���� �ʾ��� ���
	{
		std::cout << "not initialized" << std::endl;
		return 0;
	}

	if (PSN > (MB_PER_SECTOR * (*flashmem)->get_flashmem_size()) - 1) //���� �ʰ� ����
	{
		std::cout << "out of range error" << std::endl;
		return 0;
	}

	FILE *fp = NULL;
	if ((fp = fopen("storage.bin", "rb")) == NULL) //�б� + �������� ���
	{
		fprintf(stderr, "storage.bin ������ �б���� �� �� �����ϴ�.");
		return 0;
	}

	//�ϳ��� ���Ϳ� �ش��ϴ� byteȯ�� �� * PSN��ŭ ���Ͽ� �ǳʶ�
	__int64 read_pos = BYTE_PER_SECTOR * PSN; //�а��� �ϴ� ���� ������ ��ġ

	_fseeki64(fp, read_pos, SEEK_SET); //�а��� �ϴ� ���� ������ ��ġ�� �̵�
	fread(&buffer, sizeof(char), 1, fp);  //�ش� ���� ���Ϳ� ��ϵ� �� �б�

	if (buffer == NULL)
		std::cout << "no data" << std::endl;
	else
		std::cout << buffer << std::endl;

	fclose(fp);

	return 1;
}

int Flash_write(FlashMem **flashmem, __int64 PSN, char data) //���� ���Ϳ� �����͸� ���
{
	char buffer;

	//�̹� �Էµ� ��ġ�� ������ �Է� �õ��� overwrite ���� �߻�
	if (*flashmem == NULL) //�÷��� �޸𸮰� �Ҵ���� �ʾ��� ���
	{
		std::cout << "not initialized" << std::endl;
		return 0;
	}

	if (PSN > (MB_PER_SECTOR * (*flashmem)->get_flashmem_size()) - 1) //���� �ʰ� ����
	{
		std::cout << "out of range error" << std::endl;
		return 0;
	}

	FILE *fp = NULL;
	if ((fp = fopen("storage.bin", "rb+")) == NULL) //�а� ���� ��� + �������� ���
	{
		fprintf(stderr, "storage.bin ������ �а� ���� ���� �� �� �����ϴ�.");
		return 0;
	}

	__int64 write_pos = BYTE_PER_SECTOR * PSN; //������ �ϴ� ��ġ

	_fseeki64(fp, write_pos, SEEK_SET); //�а��� �ϴ� ���� ������ ��ġ�� �̵�
	fread(&buffer, sizeof(char), 1, fp); //�ش� ���� ���Ϳ� ��ϵ� �� �б�
	_fseeki64(fp, write_pos, SEEK_SET); //������ �ϴ� ���� ������ ��ġ�� �̵�

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

int Flash_erase(FlashMem **flashmem, __int64 PBN) //���� ��Ͽ� �ش��ϴ� �����͸� ����
{
	char erase_buffer = NULL; //������� �� �� �������� �� ��

	//�ش� ����� ���� ���͵鿡 ���ؼ� ��� erase
	if (*flashmem == NULL) //�÷��� �޸𸮰� �Ҵ���� �ʾ��� ���
	{
		std::cout << "not initialized" << std::endl;
		return 0;
	}

	if (PBN > (MB_PER_BLOCK * (*flashmem)->get_flashmem_size()) - 1) //���� �ʰ� ����
	{
		std::cout << "out of range error" << std::endl;
		return 0;
	}

	__int64 erase_pos_start = BYTE_PER_BLOCK * PBN; //������� �ϴ� ��� ��ġ�� ���� 
	__int64 erase_pos_end = (BYTE_PER_BLOCK * (PBN+1)) -1; //������� �ϴ� ��� ��ġ�� ��

	FILE *fp = NULL;
	if ((fp = fopen("storage.bin", "rb+")) == NULL) //�а� ������ + �������� ���(���� �б� ���� �� ��� ���ϳ����� ��� �ʱ�ȭ)
	{
		fprintf(stderr, "storage.bin ������ �а� ���� ���� �� �� �����ϴ�.");
		return 0;
	}

	_fseeki64(fp, erase_pos_start, SEEK_SET); //erase�ϰ��� �ϴ� ���� ����� ���� ��ġ�� �̵�

	__int64 erase_pos_next = ftell(fp); //erase�� ���� ������ ��ġ
	while(1) //�ش� ��� ��ġ�� ������ �ݺ�
	{
		fwrite(&erase_buffer, sizeof(char), 1, fp);
		erase_pos_next += BYTE_PER_SECTOR;
		if ((erase_pos_next > erase_pos_end)) break;
		else _fseeki64(fp, erase_pos_next, SEEK_SET); //�� ���Ϳ� ���� ����Ʈ ����(512byte)�� �̵�
	}

	fclose(fp);
	std::cout << PBN << "-th block erased" << std::endl;

	return 1;
}