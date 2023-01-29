#include <iostream>
using namespace std;

int main (int argc, char* argv[]) {

    for (int i = 1; i < argc; ++i)
        cout << "argv[" << i << "] is " << argv[i] << endl;

    int temp;
    cin >> temp;

    return 0;
}
