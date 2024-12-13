#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>


// Linear regression class
class LinnearRegression 
{
public:
  // Trainable parameters
    double Weight;
    double Bias;

  // Hyperparameters
    double LearningRate;

  // Other
  std::vector<std::pair<double, double>> data_set;

  // Empty counstructor without arguments
  LinnearRegression() {  };

  // Constructor with dataset
  LinnearRegression(std::vector<std::pair<double, double>> data_set) { this -> data_set = data_set; }

  // Get current DataSet
  auto GetDataSet()
  {
    return data_set;
  }

  // Print current DataSet
  void PrintDataSet()
  {
    for (const auto &point : data_set)
    {
      std::cout << "x: " << point.first << " | y: " << point.second << std::endl;
    }
  }

  // Set DataSet from text file
  void ImportDataSet(std::string FileName)
  {
    std::ifstream in(FileName);
    if (in.is_open())
    {
      std::pair<double, double> temp;
      while (in >> temp.first >> temp.second){this->data_set.push_back(temp);}
    }
    in.close();
  }
  // Set DataSet for current object
  void SetDataSet(std::vector<std::pair<double, double>> DataSet){this -> data_set = DataSet;}

  // Trains linnear regression model with current DataSet
  void TrainModel(int Epoches)
  {
    // This value should be changed depending on the value in your dataset
    const double RAND_WIDTH = 10000;
    double tr_weight = (RAND_WIDTH / static_cast<double>(RAND_MAX)) * static_cast<double>(rand());
    double tr_bias = (RAND_WIDTH / static_cast<double>(RAND_MAX)) * static_cast<double>(rand());
    std::cout << "Начальный вес: " << tr_weight << " | Начальный сдвиг: " << tr_bias << std::endl;
    double BatchSize = sizeof(data_set);
    std::cout << "Размер датасета: " << BatchSize << std::endl;

    for ( int epo = 0; epo < Epoches; ++epo )
    {
      /////////////////////// GETTING DERRIATIVE BY WEIGHT BLOCK //////////////////////////////
      double DerSumForWeight = 0;

      for (const auto &point : data_set){DerSumForWeight += ((point.second - tr_bias - (tr_weight * point.first)) * point.first);}
      double DerrByWeight = -(2/ BatchSize) * DerSumForWeight;
      ////////////////////// GETTING DERRIATIVE BY BIAS BLOCK ////////////////////////////////

      double DerSumForBias = 0;
      for (const auto &point : data_set)
      {
        DerSumForBias += (point.second - tr_bias - (tr_weight * point.first));
      }
      double DerrByBias = -( 2 / BatchSize ) * DerSumForBias;

      tr_weight -= LearningRate * DerrByWeight;
      tr_bias -= LearningRate * DerrByBias;
      
    }
    this->Weight = tr_weight;
    this->Bias = tr_bias;
  }
};

// Noise generation function

std::vector<std::pair<double, double>> GenLinData
                                        (
                                      double k, 
                                      double b, 
                                      int size,
                                      double noise_weight,
                                      double x_start,
                                      double x_end
                                        )
{
  std::vector<std::pair<double, double>> data;
  
  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  for (int i = 0; i < size; i++)
  {
    double x = x_start + static_cast<double>( rand() / static_cast<double>(RAND_MAX / (x_end - x_start)));
    double y = k * x + b;
    double noise = (static_cast<double>( rand()) / RAND_MAX) * noise_weight * 2 - noise_weight;
    y += noise;
    data.emplace_back(x,y);
  }
  return data;
}

int main(){
  //auto dataset = GenLinData(0.24, 52.1, 15, 1.0, 0.0, 30);

  LinnearRegression ln;
  ln.ImportDataSet("test.txt");
  ln.PrintDataSet();

  ln.LearningRate = 0.0001;

  std::cout << ln.LearningRate << std::endl;

  ln.TrainModel(10000000);

  std::cout << "Weight -> " << ln.Weight << " | Bias -> " << ln.Bias << std::endl;

  return 0;
}


