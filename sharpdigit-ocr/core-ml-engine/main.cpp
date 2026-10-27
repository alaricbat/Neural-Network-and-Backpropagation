#include <iostream>
#include <memory>
#include <omp.h>
#include <filesystem>
#include "reader/TxtMatrixReader.h"
#include "weight/ModelWeightReader.h"
#include "include/MultilayerPerception.h"

using namespace std;
int main() {
    try {

        cout << "[main]: *** (OpenMP) ***" << endl;
        cout << "[main]: Max threads: " << omp_get_max_threads() << endl;

        #pragma omp parallel default(none) shared(cout)
        {
            #pragma omp critical
            {
                cout << "[main]: ThreadID {" << omp_get_thread_num() << "}" << endl;
            }
        }

        const std::filesystem::path path = filesystem::current_path() / "asset" / "test" / "matrix_09.txt";

        // Dependency injection through the interface
        std::unique_ptr<IMatrixReader> reader =
                std::make_unique<TxtMatrixReader>(784);
        Matrix input = reader->read(path);

        cout << "[main]: Image loaded successfully." << endl;
        cout << "[main]: Input shape: " << input.rows << " x " << input.cols << endl;

        MultilayerPerceptron multilayer = MultilayerPerceptron();

        const auto model_dir = std::filesystem::current_path() / "asset" / "model";
        ModelWeights model_weights = ModelWeightReader::read(model_dir);
        multilayer.set_W(model_weights.weights);
        multilayer.set_b(model_weights.biases);

        int result = multilayer.predict(input);

        cout << "[main]: The final result is: " << result << endl;
        cout << "[main]: Execute completed." << endl;

    } catch (const std::exception &e) {
        std::cerr << "[main]: Failed: " << e.what() << endl;
        return 1;
    }
    return 0;
}
