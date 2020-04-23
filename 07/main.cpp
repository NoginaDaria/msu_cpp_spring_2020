#include <iostream>
#include "vector.h"
using namespace std;

int main() {
    cout << "------1------\nInit vector\n";
    Vector<int> A(10);
    cout << "done!\n";
    cout << "------2------\nFill vector\n";
    for (size_t i = 0; i < 10; i++) {
        A[i] = 10 - i;
    }
    cout << "done!\n";
    cout << "------3------\nRead from vector\n";
    for (size_t i = 0; i < 10; i++) {
      if(A[i] != 10 - i) cout << "not done :(\n";
    }
    cout << "done!\n";
    cout << "------4------\nPush back\n";
    A.push_back(-133);
    cout << "done!" << endl;
    cout << "------5------\nCheck push back\n";
    if(A[A.size() - 1] == -133)
    {
      cout << "done!\n";
    } else {
      cout << A.size() << "\n";
      for (size_t i = 0; i < A.size(); i++) {
          cout << A[i] << " ";
      }
      cout << "not done :(\n";
    }
    cout << "------6------\nPop back\n";
    A.pop_back();
    cout << "done!" << endl;
    cout << "------7------\nCheck pop back\n";
    if((A.size() == 10) & (A[0] == 10) )
    {
      cout << "done!\n";
    } else {
      cout << A.size() << "\n";
      for (size_t i = 0; i < A.size(); i++) {
          cout << A[i] << " ";
      }
      cout << "not done :(\n";
    }
    cout << "------8------\nBegin, rbegin and end, rend\n";
    for (auto i=A.rbegin(); i != A.rend(); i++)
    {
      if (*i == *A.begin()) cout << "Reverse cycle completed\n";
    }
    cout << "done!\n";
    cout << "------8------\nResize to bigger\n";
    A.resize(19);
    if (A.size() == 19)
    {
      cout << "done!\n";
    } else {
      cout << "not done :(\n";
      cout << "Wanted size: 15, actual size: " << A.size() << "\n";
    }

    cout << "------9------\nResize to smaller\n";
    A.resize(3);
    if (A.size() == 3)
    {
      cout << "done!\n";
    } else {
      cout << "not done :(\n";
      cout << "Wanted size: 3, actual size: " << A.size() << "\n";
    }
    cout << "------10------\nClear\n";
    A.clear();
    if (A.size() == 0)
    {
      cout << "done!\n";
    } else {
      cout << "not done :(\n";
      cout << "Wanted size: 0, actual size: " << A.size() << "\n";
    }
    cout << "OK\n";
    return 0;
}
