//
//  Queue.h
//
//  Created by Baraa Egbaria
//

#ifndef Queue_h
#define Queue_h

#include <new>

template<class Type>
class Queue {
    
    // Class Node to define the Queue
    class Node {
    public:
        Type data;
        Node* next;
        Node() = delete;
        explicit Node(const Type& Data) : data(Data), next(nullptr) {};
        Node(const Node& copy) : data(copy.data), next(copy.next) {};
        ~Node() = default;
    };
    
    static void emptyList(Node* head);
    
    // *** fileds of the Queue *** //
    // TODO: add tail pointer to make insertion O(1)
    Node* m_head;
    int m_size;

public:

    // *** c'tor && copy c'tor, d'tor, assignment operator *** //
    Queue() : m_head(nullptr), m_size(0) {};
    Queue(const Queue<Type>& copy);
    Queue& operator=(const Queue<Type>& copy);
    ~Queue();

    // *** iterator *** //
    class Iterator;
    Iterator begin() {
        return Iterator(this, m_head);
    }
    Iterator end(){
        return Iterator(this, nullptr);
    }
    
    // *** const iterator *** //
    class ConstIterator;
    ConstIterator begin() const {
        return ConstIterator(this, m_head);
    }
    ConstIterator end() const {
        return ConstIterator(this, nullptr);
    }

    // *** member functions *** //
    void pushBack(const Type& data);
    Type& front();
    const Type& front() const;
    void popFront();
    int size() const;

    // *** exceptions *** //
    class EmptyQueue{};
};


// *** class Queue Iterator *** //
template<class Type>
class Queue<Type>::Iterator {
    const Queue<Type>* queue;
    Node* node;
    // make c'tor private so user can't call it (encapsulation)
    Iterator(const Queue* queue, Node* member) : queue(queue), node(member) {};
    friend class Queue;

public:
    ~Iterator() = default;
    Iterator(const Iterator&) = default;

    // *** basic member functions *** //
    Type& operator*();
    bool operator!=(const Iterator& it);
    Iterator& operator++();

    // *** Iterator exceptions *** //
    class InvalidOperation{};

};
// *** End of class Queue Iterator *** //


// *** class Queue const-Iterator *** //

template<class Type>
class Queue<Type>::ConstIterator {
    const Queue<Type>* queue;
    const Node* node;
    // make c'tor private so user can't call it (encapsulation)
    ConstIterator(const Queue* queue, Node* member) : queue(queue), node(member) {};
    friend class Queue;

public:
    ~ConstIterator() = default;
    ConstIterator(const ConstIterator&) = default;

    // *** basic member functions *** //
    const Type& operator*() const;
    bool operator!=(const ConstIterator& it) const;
    const ConstIterator& operator++();

    // *** Iterator exceptions *** //
    class InvalidOperation{};

};
// *** End of class Queue const-Iterator *** //



// *** class Queue method Functions *** //
template<class Type>
void Queue<Type>::emptyList(Node* head)
{
    while (head != nullptr)
    {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}


template<class Type>
Queue<Type>::Queue(const Queue<Type>& copy) : m_head(nullptr), m_size(0)
{
    for(ConstIterator it = copy.begin(); it != copy.end(); ++it)
    {
        try {
            pushBack(*it);
        } catch (const std::bad_alloc& e)
        {
            emptyList(m_head);
            throw e;
        }
    }
}

template<class Type>
Queue<Type>& Queue<Type>::operator=(const Queue<Type>& copy) {
    if (this == &copy)
        return *this;
    Node* oldList = m_head;
    int oldSize = m_size;
    m_head = nullptr;
    m_size = 0;
    for (ConstIterator it = copy.begin(); it != copy.end(); ++it) {
        try {
            pushBack(*it);
        } catch (const std::bad_alloc &e) {
            emptyList(m_head);
            m_head = oldList;
            m_size = oldSize;
            throw e;
        }
    }
    emptyList(oldList);
    return *this;
}

template<class Type>
Queue<Type>::~Queue() {
    emptyList(m_head);
}

template<class Type>
void Queue<Type>::pushBack(const Type& data) {
    try {
        Node* newNode = new Node(data);
        if(m_size == 0)
        {
            m_head = newNode;
            m_size++;
            return;
        }
        Node* it = m_head;
        for(; it->next != nullptr; it=it->next);
        it->next = newNode;
        m_size++;

    } catch (const std::bad_alloc &e) {
        throw e;
    }
}

template<class Type>
Type& Queue<Type>::front(){
    if(m_size == 0)
        throw EmptyQueue();
    return m_head->data;
}

template<class Type>
const Type& Queue<Type>::front() const{
    if(m_size == 0)
        throw EmptyQueue();
    return m_head->data;
}

template<class Type>
void Queue<Type>::popFront() {
    if(m_size == 0)
        throw EmptyQueue();
    Node* toDelete = m_head;
    m_head = m_head->next;
    m_size--;
    delete toDelete;
}

template<class Type>
int Queue<Type>::size() const {
    return m_size;
}
// *** End of class Queue method Functions *** //

// *** Iterator Functions *** //
template<class Type>
Type& Queue<Type>::Iterator::operator*(){
    if (node == nullptr)
        throw InvalidOperation();
    return node->data;
}

template<class Type>
bool Queue<Type>::Iterator::operator!=(const Iterator& it) {
    return node != it.node;
}

template<class Type>
typename Queue<Type>::Iterator& Queue<Type>::Iterator::operator++() {
    if (node == nullptr)
        throw InvalidOperation();
    node = node->next;
    return *this;
}

// *** end of Queue::Iterator Functions *** //


// *** Queue::ConstIterator Functions *** //
template<class Type>
const Type& Queue<Type>::ConstIterator::operator*() const {
    if (node == nullptr)
        throw InvalidOperation();
    return node->data;
}

template <class Type>
bool Queue<Type>::ConstIterator::operator!=(const Queue<Type>::ConstIterator &it) const {
    return node != it.node;
}

template <class Type>
const typename Queue<Type>::ConstIterator& Queue<Type>:: ConstIterator::operator++() {
    if(node== nullptr)
        throw InvalidOperation();
    node=node->next;
    return *this;
}

// *** end of Queue::ConstIterator Functions *** //


// *** Object Functions *** //
template<class Type, class Condition>
Queue<Type> filter(const Queue<Type>& queue, Condition func) {
    Queue<Type> filtered;
    for(typename Queue<Type>::ConstIterator it = queue.begin(); it != queue.end(); ++it)
        if(func(*it) == true)
            filtered.pushBack(*it);
    return filtered;
}


template<class Type, class Condition>
void transform(Queue<Type>& queue, Condition func) {
    for(typename Queue<Type>::Iterator it = queue.begin(); it != queue.end(); ++it)
        func(*it);
}
// *** End of Object Functions *** //


#endif /* Queue_h */
