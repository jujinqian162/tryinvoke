
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

void fun(std::pair<int, int> p){

}

int main(){
	fun(std::tuple{3, 5});
	return 0;
}
