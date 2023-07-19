/**********************************************
*							                  *
*           проект консольного чата           *
*                  модуль №10  	 	          *
*							                  *
**********************************************/

#include "user.h"
#include "chat.h"

using namespace std;


int main()
{
	setlocale(LC_ALL, "");

	chat chatObj;
	chatObj.start();

	return 0;
}