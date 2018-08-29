#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <iterator>
#include <queue>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <fstream>

using namespace std;


struct majorData
{
	string majorName;
	int earlyCareerPay;
	int midCareerPay;
};


vector<majorData> loadFile(string fileName) {
	string line;
	vector<majorData> data;
	ifstream myfile(fileName.c_str());
	while(getline(myfile, line)) {
		stringstream iss(line);
		majorData lineData;
		getline(iss, lineData.majorName, '\t');
		cout << lineData.majorName << endl;
		iss >> lineData.earlyCareerPay >> lineData.midCareerPay;
		cout << to_string(lineData.earlyCareerPay) + " " + to_string(lineData.midCareerPay) << endl;
		data.push_back (lineData);
		iss.clear();
	}
	return data;
}


int main(int argc, char *argv[])
{
	string fileName;
	char buffer[512];
	int portNumber, sockfd, newsockfd, n, m;
	unsigned int clientLength;
	int earlyPay = -1;
	int midPay = -1;
	struct sockaddr_in serv_addr, cli_addr;
	cout << "Enter the name of file containing salary data: ";
	cin >> fileName;
	vector<majorData> dataList = loadFile(fileName);
	cout << "Enter port number to listen to: ";
	cin >> portNumber;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		return (-1);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portNumber);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
          sizeof(serv_addr)) < 0) 
             return (-1);
    while(true) {
		listen(sockfd,5);
		clientLength = sizeof(cli_addr);	 
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clientLength);
		if (newsockfd < 0) 
			return (-2);
		bzero(buffer, 512);
		n = read(newsockfd, buffer, 511);
		//format data in buffer so the vector containing major data can be searched with the client request
		string requestedMajor(buffer);
		cout << requestedMajor << endl;
		/*if(requestedMajor.empty())
			exit(1);
		//search for requested major
		for(int i = 0; i < dataList.size(); i++) {
			if(dataList[i].majorName.compare(requestedMajor))
			{
				earlyPay = dataList[i].earlyCareerPay;
				midPay = dataList[i].midCareerPay;
				break;
			}
		}
		//reply to client with pay info, or -1 -1 if major not found
		string sEarlyPay = to_string(earlyPay);
		string sMidPay = to_string(midPay);
		string reply = sEarlyPay + " " + sMidPay;
		n = write(newsockfd, reply.c_str(), strlen(reply.c_str()));
		if(n < 0)
			return(-3);*/
	}
	return 1;
	
}