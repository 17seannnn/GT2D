#ifndef LIST_H_
#define LIST_H_

/* ====== STRUCTURES ====== */
template<class T>
class TList
{
    struct Item
    {
        T data;
        Item* next;

        Item() : data(), next(nullptr) {}
        Item(T _data, Item* _next) : data(_data), next(_next) {}
    };

    Item* m_pFirst;

public:
    TList() : m_pFirst(nullptr) {}
    ~TList() { Clean(); }

    void Push(T data);
    void Clean();
    void Mapcar(void (*fun)(T, void*), void* userdata);
};

/* ====== METHODS ====== */
template<class T>
inline void TList<T>::Push(T data)
{
    Item* pTemp = new Item(data, m_pFirst);
    m_pFirst = pTemp;
}

template<class T>
inline void TList<T>::Clean()
{
    Item* pTemp;
    while (m_pFirst)
    {
        pTemp = m_pFirst;
        m_pFirst = m_pFirst->next;
        delete pTemp;
    }
    m_pFirst = nullptr;
}

template<class T>
inline void TList<T>::Mapcar(void (*fun)(T, void*), void* userdata)
{
    for (Item* pTemp = m_pFirst; pTemp; pTemp = pTemp->next)
        fun(pTemp->data, userdata);
}

#endif LIST_H_