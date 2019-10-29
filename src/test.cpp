#include <iostream>
#include <unordered_map>
#include <queue>
#include <string>
using namespace std;

typedef unordered_map<unsigned long, int> container;
typedef pair<unsigned long, int> value_type;

struct second_order {
  bool operator()(const value_type& x, const value_type& y) const {
    return x.second < y.second;
  }
};

vector<unsigned long>test(container in){
	priority_queue<value_type, vector<value_type>, second_order> que;
  	pair<string, int> p;
  	for(auto x: in) {
    	que.push(x);
  	}
  	vector<unsigned long> v;
  	while (!que.empty() ) {
    	value_type item = que.top();
    	v.push_back(item.first);
    	que.pop();
  	}
  return v;
}

int main() {
  container src;
  src[111] = 3;
  src[12] = 2;
  src[356] = 1;
  src[223] = 10;
  src[224] = 8;

  vector<unsigned long> v = test(src); 
  for(auto x: v) {
  	std::cout << x << std::endl;
  }

}