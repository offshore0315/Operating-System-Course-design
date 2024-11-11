#include <time.h>
#include <iostream>
using namespace std;

struct pcb //进程控制块PCB
{
	int id;		//进程标识数
	int status;	//进程状态：0为可执行态；1为等待态1，表示输出井满请求输出的用户进程等待；2为等待状态2，表示请求输出井空，SP00LING输出进程等待；3为等待状态3，表示请求输出井满，请求输出的用户进程等待；4为结束态，进程执行完成。
	int count;  //要输出的文件数
	int x;   //进程输出时的临时变量
}PCB[3];//进程控制块PCB,一个SP00LING输出进程PCB[2]和两个请求输出的用户进程PCB[0]、PCB[1]


struct reqblock   //请求输出块
{
	int reqname;  //请求进程名
	int length;//本次输出信息长度 
	int addr;//信息在输出井的首地址 
}reqblock[10];//请求输出块

int buffer[2][100];//输出井
int c1[2] = { 100,100 };//分别表示两个用户进程可使用的输出井的空间,计数器
int buf0 = 0, buf1 = 0;//buf0代表buffer[0]的第一个可用空缓冲指针，buf1代表buffer[1]的第一个可用空缓冲指针。
int buf2 = 0, buf3 = 0;//buf2代表buffer[0]的第一个满缓冲指针，buf3代表buffer[1]的第一个满缓冲指针
int c2[2][2] = { buf0,buf2,buf1,buf3 };//输出并使用情况,具体代表含义如上
int Ptr1 = 0, Ptr2 = 0;//Ptr1表示输出的第一个请求块指针，Ptr2是空闲请求输出块指针
int c3 = 10;//当前系统剩余的请求输出信息块个数
int t1 = 0, t2 = 0;//两个用户进程目前已经输出的文件数
int t3 = 0, t4 = 0;//两个用户进程目前已经存入输出井的文件数

void init();//初始化函数
void work();//模拟进程调度函数
void request(int);//用户进程运行函数
void spooling();//spooling进程运行函数

void work()//模拟进程调度 (SPOOLING输出模拟系统主控流程图实现)
{
	int i;
	bool isFinish;

	srand((unsigned)time(NULL));//根据系统时间生成发生随机数
	while (true)
	{
		i = rand() % 100;//取模扩大100倍
		//cout << i << endl;//检查生成的进程是否相对应
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
		//判断所有进程是否全部结束
		isFinish = true;
		for (int m = 0; m < 3; m++)
		{
			if (PCB[m].status != 4)
			{
				isFinish = false;
				break;
			}
		}
		if (isFinish)     //若所有进程都结束，则退出
		{
			cout << "所有进程都结束了！" << endl;
			return;
		}
	}
}//先去进行随机数生成判断分类，然后判断是否所有进程全部结束，全部结束则退出进程调度

void init()//初始化函数 
{
	int i, j;
	cout << "请输入两个用户进程请求输出的文件个数:" << endl;
	for (i = 0; i < 2; i++)
		cin >> PCB[i].count;
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 100; j++)
			buffer[i][j] = 0;
	}//输出井初始化
	for (i = 0; i < 10; i++)
	{
		reqblock[i].length = 0;
		reqblock[i].addr = 0;
	}//请求输出块初始化
	for (i = 0; i < 3; i++)
	{
		PCB[i].id = i;
		PCB[i].status = 0;
	}//进程控制块PCB初始化
}


void request(int i)//输出请求服务程序框图的实现
{
	if (c3 == 0)    //判断是否有空闲的请求块
	{
		PCB[i].status = 3;  //没有空闲的请求块，请求输出井满，请求输出的用户进程等待，进程状态置3
		cout << endl;
		cout << "没有空闲的请求块，进程状态置3" << endl;
		return;
	}
	c3--;//申请一个空闲的请求输出块
	int file_length = 0;
	int address0 = buf0;//第一个0进程地址
	int address1 = buf1;//第一个1进程地址
	if (i == 0)
	{
		cout << endl;
		cout << "+++++++++++++调用进程0+++++++++++++" << endl;
		while (true)
		{
			PCB[0].x = rand() % 10;//取模扩大十倍
			if (PCB[0].x != 0)
			{
				if (c1[0] == 0)//判断输出井是否有空间
				{
					PCB[0].status = 1;
					cout << "输出井满，进程状态置1" << endl;
					return;
				}
				buffer[0][buf0] = PCB[0].x;
				cout << PCB[0].x << " ";
				c1[0]--;
				buf0 = (buf0 + 1) % 100;//指针按模100进行变化，每个井可存放100个信息
				file_length++;
			}
			else if (PCB[0].x == 0)//0作为文件结束标志
			{
				reqblock[Ptr2].length = file_length;
				reqblock[Ptr2].reqname = 0;
				reqblock[Ptr2].addr = address0;
				Ptr2 = (Ptr2 + 1) % 10;//指针按模10进行变化
				t3++;//0进程已经存入输出井的文件个数
				if (PCB[2].status == 2)//2为等待状态2，表示请求输出井空，SP00LING输出进程等待
					PCB[2].status = 0;//SPOOLING进程进入可执行态
				return;
			}
		}
	}
	else if (i == 1)
	{
		cout << endl;
		cout << "+++++++++++++调用进程1+++++++++++++" << endl;
		while (true)
		{
			PCB[1].x = rand() % 10;//取模扩大十倍
			if (PCB[1].x != 0)
			{
				if (c1[1] == 0)//判断输出井是否有空间
				{
					PCB[1].status = 1;
					cout << "输出井满，进程状态置1" << endl;
					return;
				}
				buffer[1][buf1] = PCB[1].x;
				cout << PCB[1].x << " ";
				c1[1]--;
				buf1 = (buf1 + 1) % 100;//指针按模100进行变化，每个井可存放100个信息
				file_length++;
			}
			else if (PCB[1].x == 0)
			{
				reqblock[Ptr2].length = file_length;
				reqblock[Ptr2].reqname = 1;
				reqblock[Ptr2].addr = address1;
				Ptr2 = (Ptr2 + 1) % 10;//指针按模10进行变化
				t4++;//1进程已经存入输出井的文件个数
				if (PCB[2].status == 2)//2为等待状态2，表示请求输出井空，SP00LING输出进程等待
					PCB[2].status = 0;//SPOOLING进程进入可执行态
				return;
			}
		}
	}
}

void spooling()//SPOOLING输出进程流程图的实现
{
	cout << endl;
	cout << "+++++++++++++调用spooling进程+++++++++++++" << endl;
	if (c3 == 10)//c3为当前系统剩余的请求输出信息块个数，初值为10
	{//如果没有请求块
		if (PCB[0].status == 4 && PCB[1].status == 4)
		{//判断是否所有输出进程结束
			PCB[2].status = 4;
			return;
		}
		else
		{
			PCB[2].status = 2;//2为等待状态2，表示请求输出井空，SP00LING输出进程等待
			return;
		}
	}
	cout << "*******************************************************************************" << endl;
	//按照请求块从输出井中取数据输出（打印到屏幕）
		//遍历请求块
	while (c3 < 10)
	{
		int reqname = reqblock[Ptr1].reqname;
		int addr = reqblock[Ptr1].addr;
		int length = reqblock[Ptr1].length;
		cout << "SPOOLING输出进程为：" << reqname << endl;
		cout << "调用SPOOLING进程，释放的进程块序号为ReqBlock[" << Ptr1 << "]" << endl;
		cout << "以下为输出结果：" << endl;
		int k;
		if (reqname == 0)
		{
			for (k = 0; k < length; k++)
				cout << buffer[0][(addr + k) % 100] << " ";
			cout << endl;
			c1[0] = c1[0] + length;
			t1++;//0进程已经输出的文件数
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
			t2++;//1进程已经输出的文件数
			if (t2 == PCB[1].count) {
				PCB[1].status = 4;
			}
		}

		cout << endl;

		c3++;//空闲的请求输出块增加

		Ptr1 = (Ptr1 + 1) % 10;//输出的第一个请求块指针按模10进行变化
	}

	//修改阻塞进程状态为就绪
	if (PCB[0].status == 3)
		PCB[0].status = 0;
	if (PCB[1].status == 3)
		PCB[1].status = 0;
	cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl << endl;

	return;
}

int main() //主程序
{
	srand((unsigned)time(NULL));//根据系统时间生成发生随机数
	init();
	cout << "\n>>>>>>>>>>>>>>>> SPOOLing系统模拟程序 <<<<<<<<<<<<<<<<<\n";
	cout << "进程0创建" << PCB[0].count << "个文件" << endl;//进程0要输出的文件数
	cout << "进程1创建" << PCB[1].count << "个文件" << endl;//进程1要输出的文件数
	work();
	return 0;
}
