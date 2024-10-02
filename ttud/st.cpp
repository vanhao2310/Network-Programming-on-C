#include <bits/stdc++.h>
using namespace std;
 
struct Node{
	int k,v;// decision assign x[k] = v
	int cnt;// number of active children (in stack)
	Node* parent;// pointer to the parent node
	Node(int _k, int _v, int _cnt, Node* _parent){
		k = _k; v = _v; cnt = _cnt; parent = _parent;
	}
};
int n;
int x[10];
bool visited[100];
bool check(int v, int k){
	return !visited[v];
}
void solution(){
	for(int i = 1; i <= n; i++) cout << x[i]; cout << endl;
}
int main(){
	cin >> n;
	stack<Node*> S;
	//for(int v = 1; v >= 0; v--){
	//	S.push(new Node(1,v,0,NULL));
	//}
	S.push(new Node(0,0,0,NULL));
	while(!S.empty()){
		Node* node = S.top();
		int k = node->k; int v = node->v;
		//cout << "Top node = (" << k << "," << v << ",cnt = " << node->cnt << ")" << endl;
		if(check(v,k)){
			x[k] = v; visited[v] = true;
			//cout << "assign x[" << k << "] = " <<v << endl;
			if(k == n){
				solution();
				// perform backtracking here
				while(!S.empty()){// iteratively remove finished nodes out of the stack
					node = S.top(); S.pop();
					int v = node->v; visited[v] = false; // recover when backtracking
					//cout << "Solution -> Backtrack POP (" << node-> k << "," << node->v << ")" << endl;
					Node* p = node->parent;
					if(p != NULL){
						p->cnt--;
						if(p->cnt > 0) break;
					}
				}
			}else{// generate children nodes and push into the stack
				for(int v = n; v >= 1; v--)if(!visited[v]){
					S.push(new Node(k+1,v,0,node));
					node->cnt = node->cnt + 1;// node has a new child (in stack)
					//cout << "Push chlid(" << (k+1) << "," << v << ")" << endl;
				}
			}
		}else{
			// perform backtracking
			while(!S.empty()){// iteratively remove finished nodes out of the stack
					node = S.top(); S.pop();
					int v = node->v; visited[v] = false;
					//cout << "NotCheck -> Backtrack POP (" << node-> k << "," << node->v << ")" << endl;
					Node* p = node->parent;
					if(p != NULL){
						p->cnt--;
						if(p->cnt > 0) break;
					}
			}
		}
	}
	return 0;
}