#include <iostream>
#include <omp.h>

using namespace std;
int main() {
    try {

        cout << "[main]: *** (OpenMP) ***" << endl;
        cout << "[main]: Max threads: " << omp_get_max_threads() << endl;

        #pragma omp parallel default(none)
        {
            #pragma omp critical
            {
                cout << "[main]: ThreadID {" << omp_get_thread_num() << "}" << endl;
            }
        }



    } catch (const std::exception &e) {
        std::cerr << "[main]: Failed: " << e.what() << endl;
        return 1;
    }
    return 0;
}
