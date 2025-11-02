#include <iostream>
#include <cstdint>

using namespace std;


int main(int argc, char **argv) {
  int bikeCount = 2;
  uint8_t truckCount = 7;
  int vehicleCount = bikeCount + truckCount;
  double narrowingConversionAssignment = 2.9;

  cout << "Bike count: " << bikeCount << endl;
  cout << "Truck count: " << truckCount << endl;
  cout << "Vehicle count: " << vehicleCount << endl;
  cout << "Narrowing conversion assignment: " << narrowingConversionAssignment << endl;

  //Check the size with sizeof
  cout << "Size of int: " << sizeof(int) << endl;
  cout << "Size of truck count: " << sizeof(truckCount) << endl;

  return 0;
}