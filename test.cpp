#include <time.h>
#include <iostream>
using namespace std;

struct pcb //���̿��ƿ�PCB
{
	int id;		//���̱�ʶ��
	int status;	//����״̬��0Ϊ��ִ��̬��1Ϊ�ȴ�̬1����ʾ�����������������û����̵ȴ���2Ϊ�ȴ�״̬2����ʾ����������գ�SP00LING������̵ȴ���3Ϊ�ȴ�״̬3����ʾ�����������������������û����̵ȴ���4Ϊ����̬������ִ����ɡ�
	int count;  //Ҫ������ļ���
	int x;   //�������ʱ����ʱ����
}PCB[3];//���̿��ƿ�PCB,һ��SP00LING�������PCB[2]����������������û�����PCB[0]��PCB[1]


struct reqblock   //���������
{
	int reqname;  //���������
	int length;//���������Ϣ���� 
	int addr;//��Ϣ����������׵�ַ 
}reqblock[10];//���������

int buffer[2][100];//�����
int c1[2] = { 100,100 };//�ֱ��ʾ�����û����̿�ʹ�õ�������Ŀռ�,������
int buf0 = 0, buf1 = 0;//buf0����buffer[0]�ĵ�һ�����ÿջ���ָ�룬buf1����buffer[1]�ĵ�һ�����ÿջ���ָ�롣
int buf2 = 0, buf3 = 0;//buf2����buffer[0]�ĵ�һ��������ָ�룬buf3����buffer[1]�ĵ�һ��������ָ��
int c2[2][2] = { buf0,buf2,buf1,buf3 };//�����ʹ�����,�������������
int Ptr1 = 0, Ptr2 = 0;//Ptr1��ʾ����ĵ�һ�������ָ�룬Ptr2�ǿ������������ָ��
int c3 = 10;//��ǰϵͳʣ������������Ϣ�����
int t1 = 0, t2 = 0;//�����û�����Ŀǰ�Ѿ�������ļ���
int t3 = 0, t4 = 0;//�����û�����Ŀǰ�Ѿ�������������ļ���

void init();//��ʼ������
void work();//ģ����̵��Ⱥ���
void request(int);//�û��������к���
void spooling();//spooling�������к���

void work()//ģ����̵��� (SPOOLING���ģ��ϵͳ��������ͼʵ��)
{
	int i;
	bool isFinish;

	srand((unsigned)time(NULL));//����ϵͳʱ�����ɷ��������
	while (true)
	{
		i = rand() % 100;//ȡģ����100��
		//cout << i << endl;//������ɵĽ����Ƿ����Ӧ
		if (i <= 45 && PCB[0].status == 0)
		{
			if (t3 < PCB[0].count)
				request(0);
		}
		else if (i > 45 && i <= 90 && PCB[1].status == 0)
		{
			if (t4 < PCB[1].count)
				request(1);
		}
		else if (i > 90 && PCB[2].status == 0)
			spooling();
		//�ж����н����Ƿ�ȫ������
		isFinish = true;
		for (int m = 0; m < 3; m++)
		{
			if (PCB[m].status != 4)
			{
				isFinish = false;
				break;
			}
		}
		if (isFinish)     //�����н��̶����������˳�
		{
			cout << "���н��̶������ˣ�" << endl;
			return;
		}
	}
}//��ȥ��������������жϷ��࣬Ȼ���ж��Ƿ����н���ȫ��������ȫ���������˳����̵���

void init()//��ʼ������ 
{
	int i, j;
	cout << "�����������û���������������ļ�����:" << endl;
	for (i = 0; i < 2; i++)
		cin >> PCB[i].count;
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 100; j++)
			buffer[i][j] = 0;
	}//�������ʼ��
	for (i = 0; i < 10; i++)
	{
		reqblock[i].length = 0;
		reqblock[i].addr = 0;
	}//����������ʼ��
	for (i = 0; i < 3; i++)
	{
		PCB[i].id = i;
		PCB[i].status = 0;
	}//���̿��ƿ�PCB��ʼ��
}


void request(int i)//��������������ͼ��ʵ��
{
	if (c3 == 0)    //�ж��Ƿ��п��е������
	{
		PCB[i].status = 3;  //û�п��е�����飬�����������������������û����̵ȴ�������״̬��3
		cout << endl;
		cout << "û�п��е�����飬����״̬��3" << endl;
		return;
	}
	c3--;//����һ�����е����������
	int file_length = 0;
	int address0 = buf0;//��һ��0���̵�ַ
	int address1 = buf1;//��һ��1���̵�ַ
	if (i == 0)
	{
		cout << endl;
		cout << "+++++++++++++���ý���0+++++++++++++" << endl;
		while (true)
		{
			PCB[0].x = rand() % 10;//ȡģ����ʮ��
			if (PCB[0].x != 0)
			{
				if (c1[0] == 0)//�ж�������Ƿ��пռ�
				{
					PCB[0].status = 1;
					cout << "�������������״̬��1" << endl;
					return;
				}
				buffer[0][buf0] = PCB[0].x;
				cout << PCB[0].x << " ";
				c1[0]--;
				buf0 = (buf0 + 1) % 100;//ָ�밴ģ100���б仯��ÿ�����ɴ��100����Ϣ
				file_length++;
			}
			else if (PCB[0].x == 0)//0��Ϊ�ļ�������־
			{
				reqblock[Ptr2].length = file_length;
				reqblock[Ptr2].reqname = 0;
				reqblock[Ptr2].addr = address0;
				Ptr2 = (Ptr2 + 1) % 10;//ָ�밴ģ10���б仯
				t3++;//0�����Ѿ�������������ļ�����
				if (PCB[2].status == 2)//2Ϊ�ȴ�״̬2����ʾ����������գ�SP00LING������̵ȴ�
					PCB[2].status = 0;//SPOOLING���̽����ִ��̬
				return;
			}
		}
	}
	else if (i == 1)
	{
		cout << endl;
		cout << "+++++++++++++���ý���1+++++++++++++" << endl;
		while (true)
		{
			PCB[1].x = rand() % 10;//ȡģ����ʮ��
			if (PCB[1].x != 0)
			{
				if (c1[1] == 0)//�ж�������Ƿ��пռ�
				{
					PCB[1].status = 1;
					cout << "�������������״̬��1" << endl;
					return;
				}
				buffer[1][buf1] = PCB[1].x;
				cout << PCB[1].x << " ";
				c1[1]--;
				buf1 = (buf1 + 1) % 100;//ָ�밴ģ100���б仯��ÿ�����ɴ��100����Ϣ
				file_length++;
			}
			else if (PCB[1].x == 0)
			{
				reqblock[Ptr2].length = file_length;
				reqblock[Ptr2].reqname = 1;
				reqblock[Ptr2].addr = address1;
				Ptr2 = (Ptr2 + 1) % 10;//ָ�밴ģ10���б仯
				t4++;//1�����Ѿ�������������ļ�����
				if (PCB[2].status == 2)//2Ϊ�ȴ�״̬2����ʾ����������գ�SP00LING������̵ȴ�
					PCB[2].status = 0;//SPOOLING���̽����ִ��̬
				return;
			}
		}
	}
}

void spooling()//SPOOLING�����������ͼ��ʵ��
{
	cout << endl;
	cout << "+++++++++++++����spooling����+++++++++++++" << endl;
	if (c3 == 10)//c3Ϊ��ǰϵͳʣ������������Ϣ���������ֵΪ10
	{//���û�������
		if (PCB[0].status == 4 && PCB[1].status == 4)
		{//�ж��Ƿ�����������̽���
			PCB[2].status = 4;
			return;
		}
		else
		{
			PCB[2].status = 2;//2Ϊ�ȴ�״̬2����ʾ����������գ�SP00LING������̵ȴ�
			return;
		}
	}
	cout << "*******************************************************************************" << endl;
	//�����������������ȡ�����������ӡ����Ļ��
		//���������
	while (c3 < 10)
	{
		int reqname = reqblock[Ptr1].reqname;
		int addr = reqblock[Ptr1].addr;
		int length = reqblock[Ptr1].length;
		cout << "SPOOLING�������Ϊ��" << reqname << endl;
		cout << "����SPOOLING���̣��ͷŵĽ��̿����ΪReqBlock[" << Ptr1 << "]" << endl;
		cout << "����Ϊ��������" << endl;
		int k;
		if (reqname == 0)
		{
			for (k = 0; k < length; k++)
				cout << buffer[0][(addr + k) % 100] << " ";
			cout << endl;
			c1[0] = c1[0] + length;
			t1++;//0�����Ѿ�������ļ���
			if (t1 == PCB[0].count) {
				PCB[0].status = 4;
			}
		}

		else if (reqname == 1)
		{
			for (k = 0; k < length; k++)
				cout << buffer[1][(addr + k) % 100] << " ";
			cout << endl;
			c1[1] = c1[1] + length;
			t2++;//1�����Ѿ�������ļ���
			if (t2 == PCB[1].count) {
				PCB[1].status = 4;
			}
		}

		cout << endl;

		c3++;//���е��������������

		Ptr1 = (Ptr1 + 1) % 10;//����ĵ�һ�������ָ�밴ģ10���б仯
	}

	//�޸���������״̬Ϊ����
	if (PCB[0].status == 3)
		PCB[0].status = 0;
	if (PCB[1].status == 3)
		PCB[1].status = 0;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl << endl;

	return;
}

int main() //������
{
	srand((unsigned)time(NULL));//����ϵͳʱ�����ɷ��������
	init();
	cout << "\n>>>>>>>>>>>>>>>> SPOOLingϵͳģ����� <<<<<<<<<<<<<<<<<\n";
	cout << "����0����" << PCB[0].count << "���ļ�" << endl;//����0Ҫ������ļ���
	cout << "����1����" << PCB[1].count << "���ļ�" << endl;//����1Ҫ������ļ���
	work();
	return 0;
}
