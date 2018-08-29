#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iterator>
#include <queue>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <fstream>
#include <pthread.h>

/*
Grant Nixon
Operating Systems Homework 3 - Threading
*/

using namespace std;

//shared variables
static int maxNumCars, numCars, numWhBound, numBrBound, numWaiting, carsInTunnel;
static bool isDone = false;
static char tunnelStatus;
static pthread_mutex_t traffic_lock;
static pthread_cond_t wake_up = PTHREAD_COND_INITIALIZER;

struct Car //struct for holding data related to arriving cars.
{
	int arrivalTime;
	int crossTime;
	int seqNo;
	bool hadWait = false;
	string destination;
};

void *carWhittier(void *arg) { //thread function for Whittier bound traffic
	Car * currentCar = static_cast<struct Car*>(arg);

	pthread_mutex_lock(&traffic_lock); //request mutex
	cout << "Car #" + to_string(currentCar->seqNo) + " going to Whittier arrives at the tunnel." << endl;
	//wait until tunnel is open to Whittier traffic
	if(tunnelStatus != 'W') { //if tunnel not open to whittier traffic, wait for signal from tunnel thread
		while(tunnelStatus != 'W')
			pthread_cond_wait(&wake_up, &traffic_lock);
	}
	
	if(carsInTunnel >= maxNumCars) { //if tunnel is full, wait for signal from exiting car
		currentCar->hadWait = true;
		while(carsInTunnel >= maxNumCars)
			pthread_cond_wait(&wake_up, &traffic_lock);
	}
	carsInTunnel++;	
	cout << "Car #" + to_string(currentCar->seqNo) + " going to Whittier enters the tunnel." << endl;
	
	if(currentCar->hadWait == true) //check to see if car was delayed, increment delayed cars if so
		numWaiting++;
	
	pthread_mutex_unlock(&traffic_lock);
	sleep(currentCar->crossTime);
	pthread_mutex_lock(&traffic_lock);
	carsInTunnel--;
	pthread_cond_broadcast(&wake_up); //broadcast change upon exiting tunnel
	cout << "Car #" + to_string(currentCar->seqNo) + " going to Whittier exits the tunnel." << endl;
	numWhBound++;
	pthread_mutex_unlock(&traffic_lock);
	pthread_exit((void*) 0);
}

void *carBear(void* arg) { //thread function for handling Bear Valley bound traffic.
	Car * currentCar = static_cast<struct Car*>(arg);
	pthread_mutex_lock(&traffic_lock);
	cout << "Car #" + to_string(currentCar->seqNo) + " going to Bear Valley arrives at the tunnel." << endl;
	//wait until tunnel is open to Bear Valley traffic
	if(tunnelStatus != 'B') {
		while(tunnelStatus != 'B')
			pthread_cond_wait(&wake_up, &traffic_lock);
	}
	
	if(carsInTunnel >= maxNumCars) {
		currentCar->hadWait = true;
		while(carsInTunnel >= maxNumCars)
			pthread_cond_wait(&wake_up, &traffic_lock);
	}
	
	carsInTunnel++;
	cout << "Car #" + to_string(currentCar->seqNo) + " going to Bear Valley enters the tunnel." << endl;
	if(currentCar->hadWait == true)
		numWaiting++;
	
	pthread_mutex_unlock(&traffic_lock);
	sleep(currentCar->crossTime);
	pthread_mutex_lock(&traffic_lock);
	carsInTunnel--;
	
	cout << "Car #" + to_string(currentCar->seqNo) + " going to Bear Valley exits at the tunnel." << endl;
	pthread_cond_broadcast(&wake_up);
	numBrBound++;
	pthread_mutex_unlock(&traffic_lock);
	pthread_exit((void*) 0);
}

void *tunnel(void* arg) {
	while(!isDone) { //cycles  through traffic directions until isDone is true (all cars are out of the tunnel)
		pthread_mutex_lock(&traffic_lock);
		tunnelStatus = 'W';
		printf("The tunnel is now open to Whittier-bound traffic.\n");
		pthread_cond_signal(&wake_up);
		pthread_mutex_unlock(&traffic_lock);
		sleep(5);
		
		pthread_mutex_lock(&traffic_lock);
		tunnelStatus = 'N';
		printf("The tunnel is now closed to ALL traffic.\n");
		pthread_mutex_unlock(&traffic_lock);
		sleep(5);
		
		pthread_mutex_lock(&traffic_lock);
		tunnelStatus = 'B';
		printf("The tunnel is now open to Bear Valley-bound traffic.\n");
		pthread_cond_signal(&wake_up);
		pthread_mutex_unlock(&traffic_lock);
		sleep(5);
	}
	pthread_exit((void*) 0);
}


vector<Car> loadFile(string fileName) { //loads the specified text file into a vector of Car structs
	string line;
	int count = 1; //variable used to assign car sequence number
	vector<Car> carData;
	ifstream myfile(fileName.c_str());
	
	if(!myfile.is_open()) { //exit if no file
		cout << "File not found, exiting..." << endl;
		exit(0);
	}
	
	//get first line of file containing max cars allowed in tunnel
	string maxInTunnel;
	getline(myfile, maxInTunnel);
	maxNumCars = stoi(maxInTunnel); //store in shared variable
	
	while(getline(myfile, line)) { //using stringstream and getline to initialize a new struct w/ correct values
		stringstream iss(line);
		Car newCar; //declared new car struct
		iss >> newCar.arrivalTime >> newCar.destination >> newCar.crossTime; //initialize car struct with data from file
		newCar.seqNo = count; //assign seqNo to keep track of this car
		count++;
		carData.push_back (newCar); //added car struct to carData vector
		iss.clear();
	}
	return carData;
}


int main(int argc, char *argv[]) {
	string fileName;
	cout << "Please enter the name of the file containing the traffic data: ";
	cin >> fileName;
	vector<Car> traffic = loadFile(fileName.c_str());
	
	//initialized pthread variables
	pthread_mutex_init(&traffic_lock, NULL);
	pthread_t tunnelThread;
	pthread_create(&tunnelThread, NULL, tunnel, NULL);
	pthread_t cars[traffic.size()]; //array of pthread ids to keep track of car threads
	
	for(int i = 0; i < traffic.size(); i++) {
		sleep(traffic[i].arrivalTime);
		if(traffic[i].destination.compare("WB") == 0) {
			pthread_create(&cars[i], NULL, carWhittier, &traffic[i]); //start whittier thread if whittier bound
		}
		else
			pthread_create(&cars[i], NULL, carBear, &traffic[i]); //start bear valley thread if bear valley bound
	}
	
	for(int i = 0; i < traffic.size(); i++) 	
		pthread_join(cars[i], NULL); //wait for all cars to exit tunnel
	isDone = true; //kill tunnel thread
	
	//print summary
	cout << to_string(numWhBound) + " car(s) going to Whittier arrived at the tunnel." << endl;
	cout << to_string(numBrBound) + " car(s) going to Bear Valley arrived at the tunnel." << endl;
	cout << to_string(numWaiting) + " car(s) had to wait because the tunnel was full." << endl;
}











