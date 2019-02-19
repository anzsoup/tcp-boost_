#include <iostream>
#include <boost/thread.hpp>

boost::condition_variable_any cond;
boost::mutex mtx;

void func()
{
    while(true)
    {
        cond.wait(mtx);
        std::cout <<"ffff"<<std::endl;
    }
}

int main() {
    auto t2 = boost::thread(func);

    while (true)
    {
        std::cin.get();
        std::cout<<"asdf"<<std::endl;
        cond.notify_all();
    }
}