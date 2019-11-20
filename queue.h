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

            forward_iterator operator++(int);

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

        T pop();

        void push(const T &value);

        T top();

        node *end_node = nullptr;

        node *end_help(node *ptr);

    private:

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
        it.ptr_->parent= nullptr;
        it.ptr_->next=std::move(root->next);
        return it;
    }

    template<class T>
    typename queue<T>::forward_iterator queue<T>::end() {
        return nullptr;
    }

    template<class T>
    void queue<T>::insert(const queue<T>::forward_iterator &it, const T &value) {
        std::unique_ptr<node> new_node(new node{value});
        new_node->parent=it.ptr_;
        if(it.ptr_) {
            new_node->next = std::move(it.ptr_->next);
            if (it.ptr_->next) {
                it.ptr_->next->parent = new_node.get();
            }
            it.ptr_->next=std::move(new_node);
        }else{
            new_node->next= nullptr;
            queue<T>::root=std::move(new_node);
        }
        end_node=end_help(root.get());
    }

    template<class T>
    void queue<T>::erase(const queue<T>::forward_iterator &it) {
        if (it.ptr_ == nullptr) {
            throw std::logic_error("erasing invalid iterator");
        }
        node *parent = it.ptr_->parent;
        std::unique_ptr<node> &pointer_from_parent = [&]() -> std::unique_ptr<node> & {
            if(it.ptr_ == root.get()){
                return root;
            }
            return it.ptr_->parent->next;
        }();
        pointer_from_parent = std::move(it.ptr_->next);
        if (pointer_from_parent) {
            pointer_from_parent->parent = parent;
        }
        end_node=end_help(root.get());
    }

//
    template<class T>
    typename queue<T>::forward_iterator queue<T>::node::nextf() {
        return this->next.get();
    }

    template<class T>
    queue<T>::forward_iterator::forward_iterator(node *ptr): ptr_{ptr} {}

    template<class T>
    T &queue<T>::forward_iterator::operator*() {
        return ptr_->value;
    }

    template<class T>
    typename queue<T>::forward_iterator &queue<T>::forward_iterator::operator++() {
        *this = ptr_->nextf();
        return *this;
    }

    template<class T>
    typename queue<T>::forward_iterator queue<T>::forward_iterator::operator++(int) {
        forward_iterator old = *this;
        ++*this;
        return old;
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
    T queue<T>::top() {
        if (queue<T>::root == nullptr) {
            throw std::logic_error("no elements");
        }
        return queue<T>::root->value;
    }

    template<class T>
    T queue<T>::pop() {
        if (queue<T>::root == nullptr) {
            throw std::logic_error("no elements");
        }
        T result = queue<T>::root->value;
        erase(queue<T>::begin());
        return result;
    }

    template<class T>
    void queue<T>::push(const T &value) {
        forward_iterator it(end_node);
        insert(it,value);
    }

}
#endif