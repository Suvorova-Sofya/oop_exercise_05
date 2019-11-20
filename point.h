#ifndef D_POINT_H_
#define D_POINT_H_

#include <iostream>

template<class T>
struct point {
    double x,y;
    point<T> point_1(double x, double y);
};

template<class T>
point<T> point<T>::point_1(double x, double y) {
    point<T> p;
    p.x=x;
    p.y=y;
    return p;
}

template<class T>
std::istream& operator>> (std::istream& is, point<T>& p){
    is >> p.x >>p.y;
    return is;
}

template<class T>
std::ostream& operator<< (std::ostream& os, const point<T>& p){
    os << p.x << " " << p.y << " ";
    return os;
}

template<class T>
point<T> operator+(point<T> x1,point<T> x2){
    point<T> x3;
    x3.x=x1.x+x2.x;
    x3.y=x1.y+x2.y;
    return x3;
}

template<class T>
point<T>& operator/= (point<T>& x1, int number){
    x1.x=x1.x/number;
    x1.y=x1.y/number;
    return x1;
}

/*
template<class T>
std::istream& operator>>(std::istream& is, point<T>& p);

template<class T>
std::ostream& operator<<(std::ostream& os,const point<T>& p);

template<class T>
point<T> operator+(point<T> x1,point<T> x2);

template<class T>
point<T>& operator/= (point<T>& x1, int number);
*/
#endif