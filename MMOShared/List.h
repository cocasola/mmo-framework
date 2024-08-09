//
// Created by Keegan Beaulieu on 2024-07-09.
//

#ifndef LIST_H
#define LIST_H

#include <stdexcept>

template <typename T>
class List
{
public:
    struct Node
    {
        Node *prev;
        Node *next;
        T data;

        Node(const T &data) : data(data), prev(nullptr), next(nullptr) { }
    };

    Node *head;
    Node *tail;

    T *alloc(const T &value)
    {
        Node *newNode = (Node *)calloc(1, sizeof(Node));
        newNode->data = value;

        if (!head)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }

        return &newNode->data;
    }

    T *alloc()
    {
        Node *newNode = (Node *)calloc(1, sizeof(Node));

        if (!head)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }

        return &newNode->data;
    }

    void *alloc(size_t size)
    {
        Node *newNode = (Node *)malloc(sizeof(Node) + size - sizeof(T)); // Lol

        if (!head)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }

        return &newNode->data;
    }

    Node *get_node(T *data)
    {
        const size_t offset = offsetof(Node, data);
        return (Node *)((unsigned char *)data - offset);
    }

    T *alloc_before(T *before)
    {
        Node *newNode = (Node *)calloc(1, sizeof(Node));
        Node *before_node = get_node(before);

        newNode->next = before_node;
        newNode->prev = before_node->prev;

        if (before_node->prev == nullptr)
            head = newNode;

        before_node->prev = newNode;

        return &newNode->data;
    }

    void destroy(T *value)
    {
        Node *node = get_node(value);

        if (node->prev) {
            node->prev->next = node->next;
        } else {
            head = node->next;
        }

        if (node->next) {
            node->next->prev = node->prev;
        } else {
            tail = node->prev;
        }


        free(node);
    }

    class Iterator
    {
    public:
        Iterator(Node *start) : current(start) {}

        bool operator!=(const Iterator &other) const
        {
            return current != other.current;
        }

        Iterator &operator++()
        {
            if (current)
            {
                current = current->next;
            }

            return *this;
        }

        T &operator*()
        {
            if (!current) throw std::runtime_error("Dereferencing end iterator.");

            return current->data;
        }

    private:
        Node *current;
    };

    Iterator begin()
    {
        return Iterator(head);
    }

    Iterator end()
    {
        return Iterator(nullptr);
    }

    void clear()
    {
        Node *current = head;

        while (current)
        {
            Node *nextNode = current->next;
            free(current);
            current = nextNode;
        }

        head = tail = nullptr;
    }
};

#endif //LIST_H
