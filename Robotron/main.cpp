#include "Robotron.h"

int main()
{
	Robotron gameRobotron = Robotron();

	bool isInitialized = gameRobotron.Initialize("scene");
	if (!isInitialized)
	{
		printf("Exiting with error");
		exit(EXIT_FAILURE);
	}

	gameRobotron.Run();

	gameRobotron.Exit();

	return 0;
}