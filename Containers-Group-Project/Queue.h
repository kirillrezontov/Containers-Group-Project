#include "MemoryManager.h"
#include "QueueAbstract.h"

class Queue : public AbstractQueue {

    void** buf;
    int* _size;
    int length;

    friend Iterator;

   public:
    /*-----------------------------Iterator---------------------*/

    class Iterator : public AbstractQueue::Iterator {
       protected:
       public:
        Iterator();
        void* getElement(size_t& size);
        bool hasNext();
        void goToNext();
        bool equals(Iterator* right);
    };
    /*--------------------Container---------------------------*/
    int size();
    size_t max_bytes();
    Iterator* find(void* elem, size_t size);
    Iterator* newIterator();
    void remove(Iterator* iter);
    void clear();
    bool empty();

    /*---------------------------Queue-------------------------*/
    Queue(MemoryManager& mem);
    ~Queue();
    int push(void* elem, size_t size);
    int pop();
    void* front(size_t& size);
    void* back(size_t& size);
    int insert(Iterator* iter, void* elem, size_t size);
};
