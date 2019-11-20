#include <iostream>
#include "five_angles.h"
#include "point.h"
#include "queue.h"
#include <string.h>
#include <algorithm>

int main() {
    char str[10];
    containers::queue<five_angles<double> > q;
    while(std::cin >> str){
        if(strcmp(str,"push")==0){
            five_angles<double> five_angle(std::cin);
            q.push(five_angle);
        }else if(strcmp(str,"pop")==0){
            try {
                five_angles<double> f = q.pop();
                f.print();
                std::cout << "\n";
            }catch (std::exception& ex){
                std::cout <<ex.what() << "\n";
            }
        }else if(strcmp(str,"top")==0){
            try {
                q.top().print();
                std::cout << "\n";
            }catch (std::exception& ex){
                std::cout <<ex.what() << "\n";
            }
        }else if(strcmp(str,"square")==0){
            int g;
            std::cin >> g;
            long res=std::count_if(q.begin(),q.end(),[g](five_angles<double> f){ return f.square() < g;});
            std::cout << res << "\n";
        }else if(strcmp(str,"all")==0){
            std::for_each(q.begin(),q.end(),[](five_angles<double> f){ f.print(); });
            std::cout<< "\n";
        }

    }
    return 0;
}