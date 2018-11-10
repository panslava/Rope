#include <iostream>
#include <string>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <set>
#include <vector>

#include "implicitSplay.h"

using namespace std;


//#define STRESS
#ifdef STRESS
#include "stressTest.h"
#endif

int32_t main() {
    ios::sync_with_stdio(0);
    cin.tie(0);


    splay_tree<int>* tree = new splay_tree<int>;

#ifdef STRESS
    stressTest();
#endif

    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        tree->insert(i);
    }
    for (int i = 0; i < m; i++) {
        int l, r;
        cin >> l >> r;

        auto p = split(tree, l - 1);
        auto q = split(p.second, r - p.first->size());
        q.first->join(p.first);
        q.first->join(q.second);

        tree = q.first;
    }
    tree->print();


    return 0;
}
