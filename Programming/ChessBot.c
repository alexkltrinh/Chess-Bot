// const int CHESS_COORDINATE[8][8] = {0};
/*
Motors:
A; y
B: z
C: claw
D: x
*/
const int zeroDist = 6;
const tSensors X_ZERO = S4;
const tSensors Y_ZERO = S2;
const tSensors Z_ZERO = S3;

void configureAllSensors()
{
	SensorType[Z_ZERO] = sensorEV3_Touch; //z
	SensorType[Y_ZERO] = sensorEV3_Touch; //y
	SensorType[X_ZERO] = sensorEV3_Ultrasonic; //x
	wait1Msec(100);
}

void zeroAllMotors()
{
	motor[motorA] = motor[motorB] = -100;
	motor[motorD] = -100;

	while (!(SensorValue[Z_ZERO] == 1 && SensorValue[Y_ZERO] == 1 && SensorValue[X_ZERO] < zeroDist)) //change "2" to appropriate value.
	{
		if(SensorValue[Z_ZERO] == 1)
			motor[motorB] = 0;
		if(SensorValue[Y_ZERO] == 1)
			motor[motorA] = 0;
		if(SensorValue[X_ZERO] < zeroDist)
			motor[motorD] = 0;
	}
	motor[motorA] = motor[motorB] = motor[motorD] = 0;
	nMotorEncoder[motorA] = nMotorEncoder[motorB] = nMotorEncoder[motorD] = 0;
}


void callibrateBoard()
{
	zeroAllMotors();
	int xDist = 4000;
	int yDist = 3150;
	nMotorEncoder[motorD] = nMotorEncoder[motorA] = 0;
	motor[motorD] = motor[motorA] = 100;

	while (nMotorEncoder[motorD] < xDist && nMotorEncoder[motorA] <yDist)
	{
		if(nMotorEncoder[motorD] > xDist)
			motor[motorD] = 0;
		if(nMotorEncoder[motorA] > yDist)
			motor[motorA] = 0;
	}
	
	displayString(3, "Press and release Enter to confirm board calibration");

	while(!getButtonPressed(BUTTON_ENTER))
	{}
	while(getButtonPressed(BUTTON_ENTER))
	{}
}


bool moveXY(int x, int y) //a1 is (0,0), will take a bool value and return 1 normally, if 0 is returned end the game
{
	x = 4000 + 4000*x;
	y = 3150 + 3150*y;
	nMotorEncoder[motorD] = nMotorEncoder[motorA] = 0;
	motor[motorD] = motor[motorA] = 100;

	while (nMotorEncoder[motorD] < x && nMotorEncoder[motorA] <y)
	{
		if(nMotorEncoder[motorD] > x)
			motor[motorD] = 0;
		if(nMotorEncoder[motorA] > y)
			motor[motorA] = 0;
	}

	motor[motorB] = motor[motorA] = 0;
}

/*
bool removePiece() //same return as moveXY
{
	zeroAllMotors();
	nMotorEncoder[motorA] = 0;
	while (nMotorEncoder[motorA] < x*2)
	{}
	motor[motorA] = 0;
	nMotorEncoder[motorB] = 0;
	motor[motorB] = 100;
	while(nMotorEncoder[motorB] < y*2)
	{}
	motor[motorB] = 0;
}

bool pickUpPiece(); // same return as moveXY
{
	nMotorEncoder[motorC] = 0;
	motor[motorC] = 100;
	while(nMotorEncoder[motorC] < 2)//value
	{}
	motor[motorC] = 0;
	motor[motorD] = 100;
	wait1Msec(500);
	motor[motorD] =0;
	motor[motorC] = -100;
	while(nMotorEncoder[motorC] > 2)//vslu
	{}
	motor[motorC] =0;
}

void dropPiece()
{
	motor[motorC] = 100;
	while(nMotorEncoder[motorC] < 2)//vslur
	{}
	motor[motorC] = 0;
	motor[motorD] = -100;
	wait1Msec(500);
	motor[motorD] =0;
}
*/
/*
void chessMoves() //reads in the files and generates move
{

}
*/


task main()
{


	configureAllSensors();
	zeroAllMotors();
	moveXY(5,5);


}
