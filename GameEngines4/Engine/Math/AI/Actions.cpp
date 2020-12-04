#include "Actions.h"

Actions::Actions()
{
}

Actions::~Actions()
{

}

Actions::Actions(int i)
{
	switch (i)
	{
	case 0:
		cout << "ENTRY ACTION" << endl;
		break;
	case 1:
		cout << "EXIT ACTION" << endl;
		break;
	case 2:
		cout << "MAIN ACTION" << endl;
		break;
	case 3:
		cout << "TRANSITION ACTION" << endl;
	default:
		break;
	}
}

void Actions::DoSomething()
{
	cout << "Do something " << endl;
}
