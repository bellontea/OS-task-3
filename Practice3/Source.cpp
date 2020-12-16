#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>
#include <conio.h>
using namespace std;

/*
 *  Разработать программу на любом высокоуровневом языке программирования выполняющую следующие условия :
 *  Дано 3 производителя и 2 потребителя, все разные потоки и работают все одновременно.
 *  Есть очередь с 200 элементами.Производители добавляют случайное число от 1..100, 
 *  а потребители берут эти числа.Если в очереди элементов >= 100 производители спят,
 *  если нет элементов в очереди - потребители спят. Если элементов стало <= 80 производители 
 *  просыпаются.Все это работает до тех пор пока пользователь не нажал на кнопку "q", после
 *  чего производители останавливаются а потребители берут все элементы, только потом программа завершается.
*/

/*
*   Задержка в программе выполнена для того, чтобы при выводе было удобнее наблюдать работу программы.
*   Я добавляю в очередь не рандомные числа, а по порядку (Вы как-то на практике сказали, что так удобнее).
*/



#define TIME_SLEEP 200

mutex mtx;

void producer(queue <int> &myQueue , bool &f)   // производитель
{
    while (f)
    {
        if (myQueue.size() < 100)   // добавление элементов в очередь, пока размер меньше 100
        {
            mtx.lock();
            int  i;
            i = myQueue.empty()? 1 : myQueue.back() + 1;
            myQueue.push(i);
            printf("Добавленный элемент: %d\n", myQueue.back());
            mtx.unlock();
        }
        else
        {
            while (f && myQueue.size() > 80);    // производитель спит, пока количество продукции больше 80
        }
        if (_kbhit())   // проверка на нажатие клавиши
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

void consumer(queue <int>& myQueue, bool& f)    // потребитель
{
    while (f || !myQueue.empty())
    {
        this_thread::sleep_for(chrono::milliseconds(10));
        mtx.lock();
        if (!myQueue.empty())   // взятие продукции пока очередь не пустая
        {
            printf("Взятый элемент: %d\tКоличество элементов: %d\n", myQueue.front(), myQueue.size());
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
    bool f = 1;     // флаг, отвечающий за работу всех потоков

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