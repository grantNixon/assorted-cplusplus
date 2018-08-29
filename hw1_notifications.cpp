include <fstream>
include <iostream>
include <string>

/*
Grant Nixon
 Data Structures Homework 1 - Linked Lists
 */

using namespace std;

int countFiles(string);//check for additional text files and return total number of files
int countLines(string);//count lines in each file(ignoring blank lines and comments)
eList combineNotifications(eList *, int);

int main(int argc, char** argv)
{
	//parse file name from command line and store in fileName
	string fileName = "C:\Users\Grant\Desktop\Text Files\uh-gray"
	int numFiles = countFiles(fileName);
	eList g = new elist[numFiles];
	
	for (int i = 0; i < numFiles; i++)
	{
		g[i].setFileName(fileName + "." + (i + 1));
		g[i].setNumLines(countLines(fileName + "." (i + 1)));
		g[i].populateList();
		g[i].removeAllDuplicates();
		g[i].combineAllSubjects();
		g[i].approveAllReservations();
		g[i].approveAllPurchaseOrders();
	}
	eList global = combineNotifications(g, numFiles);
	global.removeAllDuplicates();
	global.combineAllSubjects();
	global.approveAllReservations();
	global.approveAllPurchaseOrders();
	
	EmailType *t = global.head;
	while(t)
	{
		cout << t->to << '\t' << t->sub << '\t' << t->msg << endl;
		t = t->link;
	}
}
int countFiles(string fileName)
{
	int numFiles = 1;
	while(FILE *file = fopen((fileName + "." + numFiles).c_str(), "r"))
	{
		numFiles++;
		fclose(file);
	}
	
	return numFiles;
	
	
	
}

int countLines(string fileName)
{
	int numLines;
	string line = "";
	ifstream myfile(fileName.c_str());
	
	if(myfile.is_open())
	{
		while(getline(myfile, line))
		{
			if(line[0]!= '\n' && line[0] != '#')
				numLines++;
		}
	}
	return numLines;
}

eList combineNotifications(eList *g, int numF)
{
	int notificationCount = 0;
	int totalNotifications = 0;
	int count = 0;
	eList global;
	EmailType t = new EmailType[numF];
	for(int i = 0; i < numF; i++)
	{
		if(g[i].getNumLines() > notificationCount)
			notificationCount = g[i].getNumLines();
		totalNotifications += g[i].getNumLines();
		t[i] = g[i].head;
	}
	while(count <= totalNotifications)
	{
		for(int i = 0; i < numF; i++)
		{
			if(g[i].getNumLines() <= notificationCount)
			{
				if(t[i])
				{
					global.add(t[i]->to, t[i]->sub, t[i]->msg);
					count++;
					t[i] = t[i]->link;
				}
				else
					t[i] = NULL;
			}
		}
			
	}
	return global;
}




struct EmailType {
	
	string to;
	string sub;
	string msg;
	EmailType *link;
}

class eList
{
	public:
		EmailType *head;
		eList()
		{
			head = NULL;
		}
		int getNumLines()
		{
			return numLines;
		}
		void populateList()
		{
			string line = "";
			string to1 = "";
			string msg1 = "";
			string sub1 = "";
			ifstream myFile(fileName);
			if (myFile.is_open())
			{
				while(myFile >> to1 >> msg1 >> sub1)
				{
					//loop through file and assign EmailType variables
					//pass EmailType variables to add function
					if(to1 != '\n' && to1 != '#')
					{
						add(to1, msg1, sub1);
					}
					

				}
			}
			
			else
				cout << "Error opening file!" << endl;
			
		}
		void add(string t, string fr, string sub)
		{
			EmailType q;
			q.link = NULL;
			q.to = t;
			q.from = fr;
			q.subject = sub;
			
			if(head == NULL)
			{
				head = &q;
			}
			else
			{
				EmailType *current = head;
				while(current)
				{
					if(NULL == current->link)
					{
						current->link = &q;
						current = current->link;
						break;
					}
					
				}
			}
		}
		void deleteSingleDuplicate(string to1, string sub1, string msg1, EmailType *list)
		{
			EmailType lastVisited = list;
			while(lastVisited)
			{
				if(lastVisited->link)
				{
					if(msg1 == lastVistited->link->msg && sub1 == lastVistited->link->sub && to1 == lastVistited->link->to)
					{
						EmailType *temp = lastVisited->link;
						lastVisited->link = lastVisited->link->link;
						delete temp;
					}
					else
						lastVisited = lastVisited->link;
				}
				else
					lastVisited = NULL;
			}
		}
		void deleteAllDuplicates()
		{
			EmailType lastVisited = head;
			while(lastVisited)
			{
				if(lastVisited->link)
				{
					deleteSingleDuplicate(lastVisited->to, lastVisited->sub, lastVisited->msg, lastVisited);
					lastVisited = lastVisited->link;
				}
			}
		}
		void combineSingleSubject(string to1, string sub1, string msg1, EmailType *list)
		{
			EmailType *lastVisited = list;
			while(lastVisited)
			{
				if(lastVisited->link)
				{
					if(sub1 == lastVistited->link->sub && to1 == lastVistited->link->to)
					{
						EmailType *temp = lastVisited->link;
						list->msg = list->msg + ", " + lastVisited->link->msg;
						lastVisited->link = lastVisited->link->link;
						delete temp;
					}
					else
						lastVisited = lastVisited->link;
				}
				else
					lastVisited = NULL;
			}
		}
		void combineAllSubjects()
		{
			EmailType *lastVisited = head;
			while(lastVisited)
			{
				if(lastVisited->link)
				{
					combineSingleSubject(lastVisited->to, lastVisited->sub, lastVisited->msg, lastVisited);
					lastVisited = lastVisited->link;
				}
			}
		}
		bool approveSingleReservation(string to1, string sub1, string msg1, EmailType *list)
		{
			EmailType *lastVisited = list;
			bool isDeleted = false;
			while(lastVisited)
			{
				if(lastVisited->link)
				{
					if(lastVisited->link->to == to1 && lastVisited->link->sub == "Approve-reservation(Cancelled)" && lastVisited->link->msg == msg1)
					{
						EmailType *temp = lastVisited->link;
						lastVisited->link = lastVisited->link->link;
						isDeleted = true;
						delete temp;
					}
					else
					{
						lastVisited = lastVisited->link;
					}
				}
				else
					lastVisited = NULL;
			}
			return isDeleted;
		}
		void approveAllReservations()
		{
			EmailType *lastVisited = head;
			while(lastVisited)
			{
				if(lastVisited->link)
				{
					if(approveSingleReservation(lastVisited->to, lastVisited->sub, lastVisited->msg, lastVisited))
					{
						EmailType *temp = lastVisited;
						lastVisited = lastVisited->link;
						delete temp;
					}
				}
			}
		}
		bool approveSinglePurchaseOrder(string msg1, EmailType *list)
		{
			EmailType *lastVisited = list;
			EmailType *temp = NULL;
			bool isDeleted = false;
			while(lastVisited)
			{
				if(lastVisited->link)
				{
					
					if(lastVisited->link->sub == "Purchase-order-cancellation" && lastVisited->link->msg == msg1)
					{
						isDeleted = true;
						lastVisited = lastVisited->link;
						
					}
					else
						lastVisited = lastVisited->link;
		
				}
				else
					lastVisited = NULL;

			}
			return isDeleted;

		}
		void purchaseOrderRevise(string msg1, EmailType *list)
		{
			EmailType *lastVisited = list;
			while(lastVisited)
			{
				if(lastVisited->link)
				{
					
					if(lastVisited->link->sub == "Purchase-order-revise" && lastVisited->link->msg == msg1)
					{
						EmailType *temp = lastVisited->link;
						lastVisited->link = lastVisited->link->link;
						delete temp;
						
					}
					else
						lastVisited = lastVisited->link;
		
				}
				else
					lastVisited = NULL;

			}
		}
		void approveAllPurchaseOrders()
		{
			EmailType *lastVisted = head;
			while(lastVisited)
			{
				if(lastVisited->link)
				{
					
					if(approveSinglePurchaseOrder(lastVisited->msg, lastVisited))
					{
						EmailType *temp = lastVisited;
						purchaseOrderRevise(lastVisited->msg, lastVisited);
						lastVisited = lastVisited->link;
						delete temp;	
					}
					else
						lastVisited = lastVisited->link;
				}
				else
					lastVisited = NULL;
			}
		}
		

		
	private:
		string fileName;
		int numLines;
		
}