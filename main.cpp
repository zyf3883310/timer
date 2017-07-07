#include "timer.h"
#include <iostream>
using namespace std;


//void start();
//void stop();
//bool regist(int id, uint32_t interval, std::function<void(int)> handle, bool immediate = false);
//void unregist(int id);

void print(int num)
{
	while (num--)
	{
		cout << "timer1 : " << num+1 << endl;
	}
}

void print1(int num)
{
	while (num--)
	{
		cout << "timer2 : " << num + 1 << endl;
	}
}
int main()
{
	CTimer& timer = CTimer::Instance();
	timer.Regist(2, 5, std::bind(print1, 2), true);
	timer.Regist(1, 3, std::bind(print, 3), false);
	timer.Start();
	//timer.Stop();
	getchar();
	return 0;
}
