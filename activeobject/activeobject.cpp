#include <queue>
#include <mutex>
#include <functional>
#include <thread>
#include <condition_variable>
#include <iostream>

// using CallbackType = void();
using namespace std;

class TaskDispatcher {
private:
    condition_variable _conditional;
    mutex _conditionalMutex;
    mutex _taskMutex;
    queue<function<void ()>> _tasks;

public:
    TaskDispatcher() : /*_taskMutex { mutex() }, _conditional { condition_variable() },*/ _tasks { queue<function<void()>>() } {

    }

    void run() {
        while(true) {
            cout << "locking" << endl;
            unique_lock<mutex> lck { this->_taskMutex };

            function<void ()> curTask = nullptr;
            if (!this->_tasks.empty()) {
                curTask = this->_tasks.front();
                this->_tasks.pop();
            }
            lck.unlock();

            if (curTask != nullptr) {
                thread taskThread { curTask };
                taskThread.join();
            }

            unique_lock<mutex> condLck { this->_conditionalMutex };
            cout << "got it! waiting for condition!" << endl;
            this->_conditional.wait(condLck);
            cout << "got condition!" << endl;
        }
    }

    void add(function<void ()> callback) {
        cout << "Adding" << endl;
        unique_lock<mutex> lck { this->_taskMutex };
        cout << "Lock aquired by add!" << endl;

        this->_tasks.push(callback);

        this->_conditional.notify_one();

        cout << "Add quit!" << endl;
    }
};

void keepAdding(TaskDispatcher &dispatcher) {
    int counter = 0;
    while(true) {
        cout << "Adding!" << endl;
        dispatcher.add([counter] () mutable { cout << counter << " + " << ++counter << " = " << counter * 2 - 1 << endl; });
        cout << "Added!" << endl;
    }
}

int main() {
    TaskDispatcher dispatcher;

    thread dis(&TaskDispatcher::run, &dispatcher);

    dispatcher.add([] { cout << "Hello world!" << endl; });

    thread adder { keepAdding, ref(dispatcher) };

    dis.join();
    adder.join();
}
