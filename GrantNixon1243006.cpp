#include <fstream>
#include <iostream>
#include <string>
#include <istream>
#include <cstring>
#include <cstddef>

using namespace std;
/*
Grant Nixon
Intro to Programming II Homework 2 - Text files
*/

int numberOfLines(string);
void encrypt();
void decrypt(string);
void encrypt(string);

int main(int argc, char* argv[])
{

	string choice;
	cout << "Enter 'en' for encription, or 'de' for decription: ";
	cin >> choice;
	
	if (choice == "en")
	{
		encrypt("text.txt");
		
	}
	else if (choice == "de")
	{
		
		decrypt("textEncripted.txt");
		
	}
	else
		cout << "Not a valid choice. Please reload the program" << endl;
	
	cin.get();
	return 0;
	
}

int numberOfLines(string fileName)
{
	string line;
	int lineCount = 0;
	ifstream myFile(fileName.c_str());

	if(myFile.is_open())
		while(getline(myFile,line))
		{
			
			lineCount++;
		}
	
	myFile.close();
	return lineCount;
	
}

void encrypt(string fileName)
{
	string *p = new string[numberOfLines(fileName)];
	
	string line;
	int lineCount = 0;
	ifstream myFile(fileName.c_str());
	ifstream key("encriptionKey.txt");
	ofstream output("textEncripted.txt");
	char c[numberOfLines("encriptionKey.txt")][4];

	if(myFile.is_open())
	{
		for(int i = 0; i < numberOfLines(fileName); i++) //reads file text.txt into dynamic string array
		{
			getline(myFile, p[i]);
		}
	
	std::size_t found;
	
	if(key.is_open())
	{
		if(output.is_open())	
			
			for(int i = 0; i < numberOfLines("encriptionKey.txt"); i++) //reads file encriptionKey.txt into 2d char array
		{	
				for(int j = 0; j <= 3; j++)
				{
					key.get(c[i][j]);	
				}	
		}
			for(int i = 0; i < numberOfLines(fileName); i++)
			{
				for(int j = 0; j < numberOfLines("encriptionKey.txt"); j++) //swaps characters according to encriptionKey.txt
																			//writes to textEncripted
				{
					found = p[i].find_first_of(c[j][0]);
					while(found != std::string::npos)
					{
						p[i][found] = c[j][2];
						found = p[i].find(c[j][0]);
				
					}				
				}
				output << p[i] << endl;
			}
			cout << "please open file 'textEncripted.txt'" << endl;
	}
	else 
		cout << "Error, file named incorrectly" << endl;
	
	key.close();
	myFile.close();
	}
	else
		cout << "Error, file named incorrectly" << endl;
}

void decrypt(string fileName)  //decrypts textEncripted.txt
{
	string *p = new string[numberOfLines(fileName)];
	
	string line;
	int lineCount = 0;
	ifstream myFile(fileName.c_str());
	ifstream key("encriptionKey.txt");
	char c[numberOfLines("encriptionKey.txt")][4];
	
	if(myFile.is_open())
	{
		ofstream output("textDecripted.txt");
		for(int i = 0; i < numberOfLines(fileName); i++) //reads file textEncripted.txt into dynamic string array
		{
			getline(myFile, p[i]);
		}
		
	std::size_t found;
	
	if(key.is_open())
	{
		if(output.is_open())	
			
			for(int i = 0; i < numberOfLines("encriptionKey.txt"); i++) //reads file encriptionKey.txt into 2d char array
		{	
			for(int j = 0; j <= 3; j++)
			{
				key.get(c[i][j]);	
			}
		}
			for(int i = 0; i < numberOfLines(fileName); i++) //swaps characters in string array according to encription key
															 //writes to file textDecripted
			{
				for(int j = 0; j < numberOfLines("encriptionKey.txt"); j++)
				{
					found = p[i].find_first_of(c[j][2]);
					while(found != std::string::npos)
					{
						p[i][found] = c[j][0];
						found = p[i].find(c[j][2]);			
					}
					
				}
				output << p[i] << endl;
			}
			cout << "please open file 'textDecripted.txt'" << endl;
	}
		else 
			cout << "Error, file named incorrectly" << endl;
	
	key.close();
	myFile.close();
	}
	else
		cout << "Error, file named incorrectly" << endl;
}









