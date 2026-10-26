#include <iostream>
#include <omp.h>

using namespace std;
int main() {
    #pragma omp parallel default(none)
    {
        int thread_id = omp_get_thread_num();
        #pragma omp critical
        cout << "[ThreadID]: " << thread_id << " -> Hello, World! " << std::endl;
    }
    return 0;
}
