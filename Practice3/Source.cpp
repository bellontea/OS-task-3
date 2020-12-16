#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>
#include <conio.h>
using namespace std;

/*
 *  ����������� ��������� �� ����� ��������������� ����� ���������������� ����������� ��������� ������� :
 *  ���� 3 ������������� � 2 �����������, ��� ������ ������ � �������� ��� ������������.
 *  ���� ������� � 200 ����������.������������� ��������� ��������� ����� �� 1..100, 
 *  � ����������� ����� ��� �����.���� � ������� ��������� >= 100 ������������� ����,
 *  ���� ��� ��������� � ������� - ����������� ����. ���� ��������� ����� <= 80 ������������� 
 *  �����������.��� ��� �������� �� ��� ��� ���� ������������ �� ����� �� ������ "q", �����
 *  ���� ������������� ��������������� � ����������� ����� ��� ��������, ������ ����� ��������� �����������.
*/

/*
*   �������� � ��������� ��������� ��� ����, ����� ��� ������ ���� ������� ��������� ������ ���������.
*   � �������� � ������� �� ��������� �����, � �� ������� (�� ���-�� �� �������� �������, ��� ��� �������).
*/



#define TIME_SLEEP 200

mutex mtx;

void producer(queue <int> &myQueue , bool &f)   // �������������
{
    while (f)
    {
        if (myQueue.size() < 100)   // ���������� ��������� � �������, ���� ������ ������ 100
        {
            mtx.lock();
            int  i;
            i = myQueue.empty()? 1 : myQueue.back() + 1;
            myQueue.push(i);
            printf("����������� �������: %d\n", myQueue.back());
            mtx.unlock();
        }
        else
        {
            while (f && myQueue.size() > 80);    // ������������� ����, ���� ���������� ��������� ������ 80
        }
        if (_kbhit())   // �������� �� ������� �������
        {
            int c = _getch();
            if (tolower(c) == 'q')
            {
                f = 0;
                break;
            }
        }
        this_thread::sleep_for(chrono::milliseconds(TIME_SLEEP));
    }
}

void consumer(queue <int>& myQueue, bool& f)    // �����������
{
    while (f || !myQueue.empty())
    {
        this_thread::sleep_for(chrono::milliseconds(10));
        mtx.lock();
        if (!myQueue.empty())   // ������ ��������� ���� ������� �� ������
        {
            printf("������ �������: %d\t���������� ���������: %d\n", myQueue.front(), myQueue.size());
            myQueue.pop();
        }
        mtx.unlock();
        if(f)this_thread::sleep_for(chrono::milliseconds(TIME_SLEEP));
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");

    queue <int> myQueue;
    bool f = 1;     // ����, ���������� �� ������ ���� �������

    thread prod1;
    thread prod2;
    thread prod3;

    thread cons1;
    thread cons2;

    prod1 = thread(producer, std::ref(myQueue), std::ref(f));
    prod2 = thread(producer, std::ref(myQueue), std::ref(f));
    prod3 = thread(producer, std::ref(myQueue), std::ref(f));

    cons1 = thread(consumer, std::ref(myQueue), std::ref(f));
    cons2 = thread(consumer, std::ref(myQueue), std::ref(f));

    cons2.join();
    cons1.join();

    prod3.join();
    prod2.join();
    prod1.join();
}