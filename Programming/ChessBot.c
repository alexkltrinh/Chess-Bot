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
	wait1Msec(50);
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

void moveXY(int x, int y) //a1 is (0,0)
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
void removePiece()
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

void pickUpPiece();
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


task main()
{


	configureAllSensors();
	zeroAllMotors();
	moveXY(5,5);


}
