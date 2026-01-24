#pragma once
#include <Arduino.h>

namespace HALIB_NAMESPACE
{
    template <class T>
    struct LinkedListNode
    {
        T data;
        LinkedListNode<T> *next;
    };

    template <typename T>
    class LinkedList
    {
    protected:
        int _size = 0;
        LinkedListNode<T> *root;
        LinkedListNode<T> *last;

    public:
        LinkedList()
        {
            root = NULL;
            last = NULL;
        }

        virtual ~LinkedList()
        {
            LinkedListNode<T> *tmp;
            while (root != NULL)
            {
                tmp = root;
                root = root->next;
                delete tmp;
            }
            last = NULL;
        }

        int getSize()
        {
            return _size;
        }

        T get(int index)
        {
            LinkedListNode<T> *tmp = root;
            int offset = 0;

            while ((index != offset) && (NULL != tmp->next))
            {
                tmp = tmp->next;
                offset++;
            }

            if (index == offset)
            {
                return tmp->data;
            }
            else
            {
                return NULL;
            }
        }

        int find(T _t, bool isPointerData)
        {
            LinkedListNode<T> *tmp = root;
            int position = 0;

            while ((tmp != NULL) && (isPointerData ? (*(tmp->data) != *_t) : (tmp->data != _t)))
            {
                tmp = tmp->next;
                position++;
            }

            if (NULL != tmp)
            {
                return position;
            }
            else
            {
                return -1;
            }
        }

        virtual bool append(T _t)
        {
            LinkedListNode<T> *tmp = new LinkedListNode<T>();
            tmp->data = _t;
            tmp->next = NULL;

            if (NULL != last)
            {
                last->next = tmp;
                last = tmp;
            }
            else
            {
                root = tmp;
                last = tmp;
            }

            _size++;
            return true;
        }

        virtual bool remove(T _t, bool isPointerData = false)
        {
            LinkedListNode<T> *tmp = root;
            LinkedListNode<T> *prev = NULL;
            while (tmp != NULL)
            {
                if (isPointerData ? (*(tmp->data) == *_t) : (tmp->data == _t))
                {
                    if (NULL == prev)
                    {
                        if (NULL != tmp->next)
                        {
                            root = tmp->next;
                        }
                        else
                        {
                            root = NULL;
                            last = NULL;
                        }
                    }
                    else
                    {
                        if (NULL != tmp->next)
                        {
                            prev->next = tmp->next;
                        }
                        else
                        {
                            prev->next = NULL;
                            last = prev;
                        }
                    }
                    _size--;
                    delete tmp;
                    return true;
                }
                prev = tmp;
                tmp = tmp->next;
            }
            return false;
        }

        T shift()
        {
            T data = NULL;
            if (NULL != root)
            {
                data = root->data;
                if (NULL != data)
                {
                    remove(data);
                }
            }
            return data;
        }
    };
} // namespace HALIB_NAMESPACE