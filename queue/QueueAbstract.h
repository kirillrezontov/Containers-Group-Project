#pragma once
#include "GroupContainer.h"

// Абстрактный класс: очередь
class AbstractQueue : public GroupContainer {
   public:
    // конструктор
    AbstractQueue(MemoryManager& mem) : GroupContainer(mem) {}

    // деструктор
    virtual ~AbstractQueue() {}

    // Добавление элемента в конец контейнера. В случае успешного добавления функция возвращает значение 0, в случае неудачи 1.
    virtual int push(void* elem, size_t size) = 0;

    // Удаление элемента из начала контейнера. В случае успешного удаление функция возвращает значение 0, в случае неудачи 1.
    virtual int pop() = 0;

    // Функция явно возвращает указатель на элемент, находящийся в начале контейнера.
    // и не явно возвращает размер данных
    virtual void* front(size_t& size) = 0;

    // Функция явно возвращает указатель на элемент, находящийся в конце контейнера.
    // и неявно возвращает размер данных
    virtual void* back(size_t& size) = 0;

    // Добавление элемента в позицию, на которую указывает итератор iter.
    // В случае успешного добавления функция возвращает значение 0, в случае неудачи 1.
    virtual int insert(Iterator* iter, void* elem, size_t size) = 0;
};
