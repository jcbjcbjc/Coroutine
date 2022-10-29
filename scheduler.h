#include "coroutine.h"
#include "RBTree.h"
#include <any>
#include <memory>

struct TaskID{

};
struct Task{

}

typedef std::shared_ptr<RBTree<Task>> RBTreePtr;
class Scheduler{
public:
    TaskID CreateTask(void (*task)()){


   }
private:
   RBTreePtr rBTree_;

};