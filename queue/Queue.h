#include "Mem.h"
#include "QueueAbstract.h"

class Queue : public AbstractQueue {
    char* queue;
    int length;

   public:
    /*-----------------------------Iterator---------------------*/
    void* getElement(size_t size);
    bool hasNext();
    void goToNext();
    bool equals(Iterator* right);

    /*--------------------Container---------------------------*/
    int size() override;
    size_t max_bytes() override;
    Iterator* find(void* elem, size_t size) override;
    Iterator* newIterator() override;
    void remove(Iterator* iter) override;
    void clear() override;
    bool empty() override;

    /*---------------------------Queue-------------------------*/
    Queue(MemoryManager& mem) : AbstractQueue(mem) {
        length = mem.size();
        queue = (char*)malloc(sizeof(char) * length);
        
    }
    ~Queue() {}
    int push(void* elem, size_t size) override;
    int pop() override;
    void* front(size_t& size) override;
    void* back(size_t& size) override;
    int insert(Iterator* iter, void* elem, size_t size) override;
};