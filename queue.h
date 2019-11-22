#ifndef D_QUEUE_H_
#define D_QUEUE_H_

#include <iostream>
#include "five_angles.h"
#include <memory>
#include <functional>
#include <cassert>
#include <iterator>


namespace containers {

    template<class T>
    struct queue {
    private:
        struct node;

    public:
        queue() = default;

        struct forward_iterator {
            using value_type = T;
            using reference = T &;
            using pointer = T *;
            using difference_type = ptrdiff_t;
            using iterator_category = std::forward_iterator_tag;

            forward_iterator(node *ptr);

            T &operator*();

            forward_iterator &operator++();

            forward_iterator operator+(int r);

            bool operator==(const forward_iterator &o) const;

            bool operator!=(const forward_iterator &o) const;

        private:
            node *ptr_;

            friend queue;

        };

        forward_iterator begin();

        forward_iterator end();

        void insert(const forward_iterator &it, const T &value);

        void erase(const forward_iterator &it);

        void pop();

        void push(const T &value);

        T front();


    private:
        node *end_node = nullptr;

        node *end_help(node *ptr);

        struct node {
            T value;
            std::unique_ptr<node> next = nullptr;
            node *parent = nullptr;

            forward_iterator nextf();
        };

        std::unique_ptr<node> root = nullptr;
    };


//

    template<class T>
    typename queue<T>::node *queue<T>::end_help(containers::queue<T>::node *ptr) {
        if ((ptr == nullptr) || (ptr->next == nullptr)) {
            return ptr;
        }
        return queue<T>::end_help(ptr->next.get());
    }


    template<class T>
    typename queue<T>::forward_iterator queue<T>::begin() {
        if (root == nullptr) {
            return nullptr;
        }
        forward_iterator it(root.get());
        return it;
    }

    template<class T>
    typename queue<T>::forward_iterator queue<T>::end() {
        return nullptr;
    }

    template<class T>
    void queue<T>::insert(const queue<T>::forward_iterator &it, const T &value) {
        std::unique_ptr<node> new_node(new node{value});
        if (it != nullptr) {
            node *ptr = it.ptr_->parent;
            new_node->parent = it.ptr_->parent;
            it.ptr_->parent = new_node.get();
            if (ptr) {
                new_node->next = std::move(ptr->next);
                ptr->next = std::move(new_node);
            } else {
                new_node->next = std::move(root);
                root = std::move(new_node);
            }
        } else {
            new_node->next = nullptr;
            if(end_node==nullptr) {
                queue<T>::root = std::move(new_node);
            }else{
                end_node->next=std::move(new_node);
            }
        }
        end_node = end_help(root.get());
    }

    template<class T>
    void queue<T>::erase(const queue<T>::forward_iterator &it) {
        if (it.ptr_ == nullptr) {
            throw std::logic_error("erasing invalid iterator");
        }
        node *parent = it.ptr_->parent;
        std::unique_ptr<node> &pointer_from_parent = [&]() -> std::unique_ptr<node> & {
            if (it.ptr_ == root.get()) {
                return root;
            }
            return it.ptr_->parent->next;
        }();
        pointer_from_parent = std::move(it.ptr_->next);
        if (pointer_from_parent) {
            pointer_from_parent->parent = parent;
            pointer_from_parent->next->parent=pointer_from_parent.get();
        }
        end_node = end_help(root.get());
    }

//
    template<class T>
    typename queue<T>::forward_iterator queue<T>::node::nextf() {
        forward_iterator result(this->next.get());
        return result;
    }

    template<class T>
    queue<T>::forward_iterator::forward_iterator(node *ptr): ptr_{ptr} {}

    template<class T>
    T &queue<T>::forward_iterator::operator*() {
        return ptr_->value;
    }

    template<class T>
    typename queue<T>::forward_iterator &queue<T>::forward_iterator::operator++() {
        if (*this != nullptr) {
            *this = ptr_->nextf();
            return *this;
        } else {
            throw std::logic_error("invalid iterator");
        }
    }

    template<class T>
    typename queue<T>::forward_iterator queue<T>::forward_iterator::operator+(int r) {
        for (int i = 0; i < r; ++i) {
            ++*this;
        }
        return *this;
    }

    template<class T>
    bool queue<T>::forward_iterator::operator==(const forward_iterator &o) const {
        return ptr_ == o.ptr_;
    }

    template<class T>
    bool queue<T>::forward_iterator::operator!=(const forward_iterator &o) const {
        return ptr_ != o.ptr_;
    }

    template<class T>
    T queue<T>::front() {
        if (queue<T>::root == nullptr) {
            throw std::logic_error("no elements");
        }
        return queue<T>::root->value;
    }

    template<class T>
    void queue<T>::pop() {
        if (queue<T>::root == nullptr) {
            throw std::logic_error("no elements");
        }
        erase(queue<T>::begin());
    }

    template<class T>
    void queue<T>::push(const T &value) {
        forward_iterator it(end_node);
        std::unique_ptr<node> new_node(new node{value});
        if (it.ptr_) {
            new_node->parent = it.ptr_;
            it.ptr_->next = std::move(new_node);
        } else {
            new_node->next = nullptr;
            queue<T>::root = std::move(new_node);
        }
        queue<T>::end_node = end_help(root.get());
    }

}
#endif