#include <iostream>

#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>

#include <cmath>
#include <GL/glut.h>

class Graph
{
public:
  
  
  void BuildGraph(double k, double b){};
};
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

  std::vector<std::pair<double, double>> param_hist;

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
    const double RAND_WIDTH = 1000;
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
      if (epo%100 == 0){
        this->param_hist.emplace_back(tr_weight, tr_bias);
      }
    }
    this->Weight = tr_weight;
    this->Bias = tr_bias;
  }
};

double b, k;
int EpochLine = 0;
LinnearRegression ln;

void timer(int value)
{
  EpochLine+=1;
  glutPostRedisplay();
  glutTimerFunc(50, timer, 0);
}

void initGL() 
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, 1920, 0, 1080);
  glMatrixMode(GL_MODELVIEW);
}

void display()
{
  const double CENTER_X = 1920/2;
  const double CENTER_Y = 1080/2;
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2i(0,CENTER_Y);
    glVertex2i(1920,CENTER_Y);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2i(CENTER_X, 0);
    glVertex2i(CENTER_X, 1080);
  glEnd();
  std::vector<std::pair<double, double>> p_hist =ln.param_hist;
  double x_start = -CENTER_X; 
  double x_end = CENTER_X;
  for ( int i_epo = 0; i_epo < EpochLine; i_epo+=1)
  {
    //std::cout << "New line №" << EpochLine << std::endl;
    
    double y_start, y_end;
    y_start = (ln.param_hist[i_epo].first) * x_start + ln.param_hist[i_epo].second; 
    y_end = (ln.param_hist[i_epo].first) * x_end + ln.param_hist[i_epo].second;  
    //k = 0.25;
    //b = 60.13;
    //y_start = (k) * x_start + b; 
    //y_end = (k) * x_end + b;  
     
    glColor3f(0.0f, 0.0f, 1.0f); // Цвет линии (например, синий)
    glBegin(GL_LINES);
      glVertex2i(x_start + CENTER_X, CENTER_Y + y_start); // Начальная точка
      glVertex2i(x_end + CENTER_X, CENTER_Y + y_end); // Конечная точка
    glEnd(); 
  }
  double y_start_f, y_end_f;
  y_start_f = (ln.Weight) * x_start + ln.Bias; 
  y_end_f = (ln.Weight) * x_end + ln.Bias;  
    //k = 0.25;
    //b = 60.13;
    //y_start = (k) * x_start + b; 
    //y_end = (k) * x_end + b;  
     
    glColor3f(0.7f, 0.3f, 1.0f); // Цвет линии (например, синий)
    glBegin(GL_LINES);
      glVertex2i(x_start + CENTER_X, CENTER_Y + y_start_f); // Начальная точка
      glVertex2i(x_end + CENTER_X, CENTER_Y + y_end_f); // Конечная точка
    glEnd(); 


  glutSwapBuffers();
}
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



int main(int argc, char** argv){
  auto dataset = GenLinData(0.24, 12.1, 15, 1.0, 0.0, 30);

  //LinnearRegression ln;
  ln.SetDataSet(dataset);
  ln.PrintDataSet();
  ln.LearningRate = 0.001;

  ln.TrainModel(1000000);
  std::cout << "Weight -> " << ln.Weight << " | Bias -> " << ln.Bias << std::endl;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(1920, 1080);
  glutCreateWindow("Graph Builder");

  initGL();

  glutDisplayFunc(display);
  glutTimerFunc(50, timer,0);
  glutMainLoop();
  return 0;
}


