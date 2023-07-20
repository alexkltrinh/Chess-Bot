void configureAllSensors()
{
	SensorType[S1] = sensorEV3_Touch;
	SensorType[S2] = sensorEV3_Touch;
	SensorType[S3] = sensorEV3_Ultrasonic;
	wait1Msec(50);
}



void zeroAllMotors();
{
	motor[motorA] = motor[motorB] = motor[motorC] = 100;

	while(SensorValue[S1] == 0 && SensorValue[S2] == 0 && SensorValue[S3] != zeroDist) //change "2" to appropriate value.
	{
		if(SensorValue[S1] == 0)
			motor[motorA] = 0;
		if(SensorValue[S2] == 0)
			motor[motorD] = 0;
		if(SensorValue[S3] < zeroDist)
			motor[motorC] = 0;
	}
	motor[motorA] = motor[motorB] = motor[motorC] = 0;
}

void moveXY(float x, float y) //a1 is (0,0)
{
	nMotorEncoder[motorA] = 0;
	motor[motorA] = 100;
	while (nMotorEncoder[motorA] < x*(number)*(conversion))
	{}
	motor[motorA] = 0;
	nMotorEncoder[motorB] = 0;
	motor[motorB] = 100;
	while(nMotorEncoder[motorB] < y*(number)*(conversion))
	{}
	motor[motorB] = 0;
}
void removePiece();
{
	zeroAllMotors();
	nMotorEncoder[motorA] = 0;
	while (nMotorEncoder[motorA] < x*(number)*(conversion))
	{}
	motor[motorA] = 0;
	nMotorEncoder[motorB] = 0;
	motor[motorB] = 100;
	while(nMotorEncoder[motorB] < y*(number)*(conversion))
	{}
	motor[motorB] = 0;
}

void pickUpPiece();
{
	nMotorEncoder[motorC] = 0;
	motor[motorC] = 100;
	while(nMotorEncoder[motorC] < (value))
	{}
	motor[motorC] = 0;
	motor[motorD] = 100;
	wait1Msec(500);
	motor[motorD] =0;
	motor[motorC] = -100;
	while(nMotorEncoder[motorC] > (value))
	{}
	motor[motorC] =0;
}
void dropPiece();
{
	motor[motorC] = 100;
	while(nMotorEncoder[motorC] < (value))
	{}
	motor[motorC] = 0;
	motor[motorD] = -100;
	wait1Msec(500);
	motor[motorD] =0;
}


task main()
{
	configureAllSensors();
	zeroAllMotors(); //Should not zero all motors until we start the robot



}
