#include <iostream>
#include <bits/stdc++.h>
#include <chrono>
using namespace std;


template<typename Func1, typename Func2>
auto compose(const Func1& f1, const Func2& f2) {
    // Using auto and decltype to deduce return type automatically
    return [f1, f2](auto&&... args) -> decltype(auto) {
        // Forwarding arguments to support perfect forwarding
        return f2(f1(std::forward<decltype(args)>(args)...));
    };
}


template < typename Func, typename...Rest >
auto composeAll(Func func, Rest...rest) {
    if constexpr(sizeof...(rest) == 0 ) {
        return func;
    } else {
        return compose(func, composeAll(rest...));
    }
}


template<typename T>
auto myTransform(const std::vector<T> & vec, auto & composed ){
    using ret_type = std::invoke_result<decltype(composed),T>::type;
    vector<T> res(vec.size());

    for(size_t i =0; i<vec.size();++i){

        res[i]=composed(vec[i]);

    }
    return res;
}

std::random_device rd;  // Obtain a random number from hardware
std::mt19937 gen(rd()); // Seed the generator
std::uniform_int_distribution<> distr(1, 10);

//template<typename T>
auto generateFunctions(size_t num){
    vector<function<int(int)>> fs;

    for(int i=0; i<num; ++i){

        function<int(int)> l = [](int a) {return a+distr(gen);};
        fs.emplace_back(l);
        l = [](int a) {return a-distr(gen);};
        fs.emplace_back(l);

    }

    return fs;

}

int id(int x){
    return x;
}
int main() {

    std::vector<int> v(1);

    std::generate(v.begin(), v.end(), [&]() { return distr(gen); });

    vector<function<int(int)>> fs = generateFunctions(100);

    function<int(int)> composed=id;

    for(const auto & f : fs){
        composed=compose(composed,f);
    }



    size_t clock =0;
    size_t clock2=0;
    //begin
//    auto start = std::chrono::high_resolution_clock::now();
    auto res = myTransform(v,composed);

//    auto stop = std::chrono::high_resolution_clock::now();
//    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);


//    cout << "duration for composed " << duration << "\n";
    //end

    cout << clock << " " << clock2 << "\n";

    //begin
//    start = std::chrono::high_resolution_clock::now();

    clock =0;
    clock2=0;
    for(auto f : fs){
        v= myTransform(v,f);
    }

    cout << clock << " " << clock2 << "\n";
    //end

//    stop = std::chrono::high_resolution_clock::now();
//    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
//    cout << "duration for sequential " << duration << "\n";


}


