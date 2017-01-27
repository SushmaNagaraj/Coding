#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>

using namespace std;
typedef int Door;

const int SIMULATION_COUNT = 100;

/**
 * Suggested problem decomposition.
 * You do not have to use these function declarations.
 */

void simulate(int seq, int& win1, int& win2);
int	 createRandomNumber();
Door hideCar();
Door openDoor(Door firstChoiceDoor, Door carBehindDoor);
Door makeFirstChoice();
Door makeSecondChoice(Door firstDoor, Door openedDoor);
void printHeader();

/**
 * Main
 */
int main()
{
    int win1 = 0, win2 = 0;

    srand(time(NULL));  // seed the random number generator
    printHeader();

    // Run the simulations.
    for (int i = 1; i <= SIMULATION_COUNT; i++) 
	simulate(i, win1, win2);

    cout << endl;
    cout << setw(4) << win1 << " wins if stay with the first choice" << endl;
    cout << setw(4) << win2 << " wins if switch to the second choice" << endl;

    cout.setf(ios::fixed);
    cout.setf(ios::showpoint);
    cout.precision(1);

    cout << endl;
    cout << "Win ratio of switch over stay: ";
    cout << static_cast<double>(win2)/win1 << endl;
}

/***** Complete this program. ****/
void simulate(int seq, int& win1, int& win2)
{
  int car_door, first_selection, monty_selection, second_selection;
  car_door = hideCar();
  first_selection = makeFirstChoice();
  monty_selection = openDoor(first_selection, car_door);
  second_selection = makeSecondChoice(first_selection, monty_selection);
  if (car_door == first_selection )
  {
    win1 = win1 + 1;
    char winOne[] = "Yes";
    char winTwo[] = " ";
    cout << setw(5) << seq << setw(6) << car_door << setw(10) << first_selection << setw(15) << monty_selection << setw(15) << second_selection << setw(14) << winOne << setw(13) << winTwo << endl;
  } else if (car_door == second_selection)
  {
    win2 = win2 + 1;
    char winOne[] = " ";
    char winTwo[] = "Yes";
    cout << setw(5) << seq << setw(6) << car_door << setw(10) << first_selection << setw(15) << monty_selection << setw(15) << second_selection << setw(14) << winOne << setw(13) << winTwo << endl;
  }
}
int createRandomNumber()
{
   int randomNo;
   randomNo = rand() % 3 + 1;
   return randomNo;
}
Door hideCar()
{ 
  int car_door;
  car_door = createRandomNumber();
  return car_door;
}
Door makeFirstChoice()
{ 
  int firstChoice;
  firstChoice = createRandomNumber();
  return firstChoice;
}
Door openDoor(Door firstChoiceDoor, Door carBehindDoor)
{
  int firstDoorOpened;
  firstDoorOpened = createRandomNumber();
  while ((firstDoorOpened == firstChoiceDoor) || (firstDoorOpened == carBehindDoor))
  {
    firstDoorOpened = createRandomNumber();
  }
  return firstDoorOpened;
}

Door makeSecondChoice(Door firstDoor, Door openedDoor) 
{
  int secondChoice;
  do
  {
    secondChoice = createRandomNumber();
  }
   while ((secondChoice == firstDoor) || (secondChoice == openedDoor));
   return secondChoice;
}
void printHeader()
{
  cout << setw(5) << "#" << setw(10) << "Car Here" << setw(14) << "First Choice" <<    setw(13) << "Opened Door" << setw(15) << "Second Choice" << setw(12) << "Win First" << setw(13) << "Win Second" << endl;
}