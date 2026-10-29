#include <iostream>
#include <memory>
#include <omp.h>
#include <filesystem>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "include/ml/reader/TxtMatrixReader.h"
#include "include/ml/weight/ModelWeightReader.h"
#include "include/cv/ImageProcessor.h"
#include "ml/MultilayerPerception.h"

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

        MultilayerPerceptron multilayer = MultilayerPerceptron();

        const auto model_dir = std::filesystem::current_path() / "assets" / "model";
        ModelWeights model_weights = ModelWeightReader::read(model_dir);
        multilayer.set_W(model_weights.weights);
        multilayer.set_b(model_weights.biases);

        for (int i = 0; i < 10; i++) {

            const std::string filename = "matrix_0" + std::to_string(i) + ".txt";
            const std::filesystem::path path = filesystem::current_path() / "assets" / "test" / filename;

            // Dependency injection through the interface
            std::unique_ptr<IMatrixReader> reader =
                    std::make_unique<TxtMatrixReader>(784);
            Matrix input = reader->read(path);

            int result = multilayer.predict(input);

            cout << "[main]: The final result is: " << result << endl;
        }

        cout << "[main]: *** (OpenCV2) ***" << endl;

        for (int i = 0; i < 10; i++) {

            const std::string filename = "mnist_0" + std::to_string(i) + ".jpg";
            cout << "[main]: >> The file  name: " << filename << endl;
            const std::filesystem::path img_path = filesystem::current_path() / "assets" / "images" / filename;

            cv::Mat img = cv::imread(img_path);

            ImageProcessor processor;
            Matrix processed_matrix = processor.process(img);
            int processed_result = multilayer.predict(processed_matrix);

            cout << "[main]: The processed result is: " << processed_result << endl;
        }

        cout << "[main]: Execute completed." << endl;

    } catch (const std::exception &e) {
        std::cerr << "[main]: Failed: " << e.what() << endl;
        return 1;
    }
    return 0;
}
