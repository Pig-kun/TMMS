#include "Singlelton.h"
#include  <iostream>
using namespace tmms::base;
    
class A: public NonCopyable{
public:
    A() = default;
    ~A() = default;

    void Print(){
        std::cout << "A::Print()" << std::endl;
    }
};

int main(){
    auto sA = tmms::base::Singleton<A>::Instance();
    sA->Print();
    return 0;
}