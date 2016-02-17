#include <iostream>
#include <cmath>
#include <unistd.h>
#include "WPILib.h"
#include "Timer.h"

const double	SMOOTH_DRIVE_P_GAIN		=	0.5;
const double	DRIVE_DEADZONE			=	0.05;
const double	DRIVE_X_TOLERANCE		=	0.05;
const double	ANGLE_TOLERANCE			=	0.1;

const double	TURN_P_GAIN				=	1.0;
const double	TURN_I_GAIN				=	0.05;
const double	TURN_D_GAIN				=	2.0;
const double	TURN_K					=	0.001;

const double	TURN_P_MAX				=	45;
const double	TURN_I_MAX				=	360;
const double	TURN_D_MAX				=	360;

const double	PITCH_P_GAIN			=	1;
const double	PITCH_I_GAIN			=	0.5;
const double	PITCH_D_GAIN			=	2;
const double	PITCH_K					=	0.001;

double	ACCEL_CALIBRATION				=	0;

double	speedLeft						=	0;
double	speedRight						=	0;

bool	driveStraight					=	false;

double	turnP							=	0;
double	turnI							=	0;
double	turnD							=	0;
double	turnInterval					=	0;

double	pitch							=	0;
double	pitchSpeed						=	0;
double	pitchP							=	0;
double	pitchI							=	0;
double	pitchD							=	0;

double	drivePower						=	0;
double	drivePowerLeft					=	0;
double	drivePowerRight					=	0;
double	turnPower						=	0;
double	targetAngle						=	0;

int		autoDriveState					=	0;
double	acceleration					=	0;
double	speed							=	0;
double	distance						=	0;
bool	tracking						=	false;

double times;
double aTimer;

double accelX;
double accelY;
double accelZ;

class Robot: public IterativeRobot
{
	LiveWindow	*lw = LiveWindow::GetInstance();

	SendableChooser	*autoChooser;
	SendableChooser	*teleChooser;
	const std::string	autoNameDefault = "Default";

	const std::string	autoNameRamparts = "Ramparts";
	const std::string	autoNameLowbar = "Lowbar";
	const std::string	autoNameRoughTerrain = "Rough Terrain";
	const std::string	autoNameSallyGate = "Sally Gate";
	const std::string	autoNameTheFrenchOne = "The French One";
	const std::string	autoNameDrawBridge = "Draw Bridge";
	const std::string	autoNameMoat = "Moat";
	const std::string	autoNameRockWall = "Rock Wall";

	std::string autoSelected;

	const std::string teleNameDefault = "BothSticks";
	const std::string teleNameSingle = "SingleStick";

	std::string teleSelected;

	Timer	timer;
	Timer 	autoTimer;
//	RobotDrive Robotc;
	Joystick		driveStick;
	JoystickButton	driverTrigger;
	JoystickButton	driverThumb;
	JoystickButton	driverB3;
	JoystickButton	driverB4;
	JoystickButton	driverB5;
	JoystickButton	driverB6;
	JoystickButton	driverB7;
	JoystickButton	driverB8;
	JoystickButton	driverB9;
	JoystickButton	driverB10;
	JoystickButton	driverB11;
	JoystickButton	driverB12;

	Joystick		operatorStick;
	JoystickButton	operatorTrigger;
	JoystickButton	operatorThumb;
	JoystickButton	operatorB3;
	JoystickButton	operatorB4;
	JoystickButton	operatorB5;
	JoystickButton	operatorB6;
	JoystickButton	operatorB7;
	JoystickButton	operatorB8;
	JoystickButton	operatorB9;
	JoystickButton	operatorB10;
	JoystickButton	operatorB11;
	JoystickButton	operatorB12;

	CANTalon		driveLeft;
	CANTalon		driveRight;
	CANTalon		pitch;
	CANTalon		throwLow;
	CANTalon		throwHigh;
	CANTalon		intakeRollers;
	Servo			launchArm;
	Servo			intakeArm;
	AnalogGyro		gyro;
	ADXL345_I2C		accel;

	Encoder			encoderArm;

	Encoder			encoderLeft;
	Encoder			encoderRight;

	Relay	*AR = new Relay(0);
	Relay	*AL = new Relay(1);
//	DoubleSolenoid	*Piston = new DoubleSolenoid(0, 1);

//	const char *JAVA = "/usr/local/frc/JRE/bin/java";
//	char *GRIP_ARGS[5] = {"java", "-jar", "/home/lvuser/grip.jar", "/home/lvuser/project.grip", NULL };

public:
	Robot():
//		Robotc(1, 2),
		driveStick		( 0 ),
		driverTrigger	( &driveStick , 1 ),
		driverThumb		( &driveStick , 2 ),
		driverB3		( &driveStick , 3 ),
		driverB4		( &driveStick , 4 ),
		driverB5		( &driveStick , 5 ),
		driverB6		( &driveStick , 6 ),
		driverB7		( &driveStick , 7 ),
		driverB8		( &driveStick , 8 ),
		driverB9		( &driveStick , 9 ),
		driverB10		( &driveStick , 10 ),
		driverB11		( &driveStick , 11 ),
		driverB12		( &driveStick , 12 ),

		operatorStick	( 1 ),
		operatorTrigger	( &operatorStick , 1 ),
		operatorThumb	( &operatorStick , 2 ),
		operatorB3		( &operatorStick , 3 ),
		operatorB4		( &operatorStick , 4 ),
		operatorB5		( &operatorStick , 5 ),
		operatorB6		( &operatorStick , 6 ),
		operatorB7		( &operatorStick , 7 ),
		operatorB8		( &operatorStick , 8 ),
		operatorB9		( &operatorStick , 9 ),
		operatorB10		( &operatorStick , 10 ),
		operatorB11		( &operatorStick , 11 ),
		operatorB12		( &operatorStick , 12 ),

		driveLeft		( 1 ),
		driveRight		( 2 ),
		pitch			( 4 ),
		throwHigh		( 3 ),
		throwLow		( 5 ),
		intakeRollers		( 6	),
		launchArm		( 0 ),
		intakeArm		( 1 ),
		gyro			( 0 ),
		autoChooser		( ),
		teleChooser		( ),
		accel			( I2C::Port::kOnboard ),

		encoderArm		( 4 , 5 ),
		encoderLeft		( 6 , 7 ),
		encoderRight	( 8 , 9 )
	{}

	void RobotInit()
	{
		autoChooser = new SendableChooser();
		autoChooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		autoChooser->AddObject(autoNameRamparts, (void*)&autoNameRamparts);
		autoChooser->AddObject(autoNameLowbar, (void*)&autoNameLowbar);
		autoChooser->AddObject(autoNameRoughTerrain, (void*)&autoNameRoughTerrain);
		autoChooser->AddObject(autoNameSallyGate, (void*)&autoNameSallyGate);
		autoChooser->AddObject(autoNameTheFrenchOne, (void*)&autoNameTheFrenchOne);
		autoChooser->AddObject(autoNameDrawBridge, (void*)&autoNameDrawBridge);
		autoChooser->AddObject(autoNameMoat, (void*)&autoNameMoat);
		autoChooser->AddObject(autoNameRockWall, (void*)&autoNameRockWall);

		SmartDashboard::PutData("Auto Modes", autoChooser);

		teleChooser = new SendableChooser();
		teleChooser->AddDefault(teleNameDefault, (void*)&teleNameDefault);
		teleChooser->AddObject(teleNameSingle, (void*)&teleNameSingle);

		SmartDashboard::PutData("Tele Modes", teleChooser);

		AR->Set(Relay::Value::kOff);
		AL->Set(Relay::Value::kOff);

//		if (fork() == 0)						//Creating process for grip and testing if it fails
//		{
//			if (execv(JAVA, GRIP_ARGS) == -1)
//			{
//				perror("Error running GRIP");
//		    }
//		}

		timer.Start();

		for ( int n = 0 ; n < 256 ; n++ )
		{
			Wait(0.1);
			ACCEL_CALIBRATION	+=	accel.GetY();
		}
		ACCEL_CALIBRATION	/=	256;

		gyro.Calibrate();

		CameraServer::GetInstance()->SetQuality(50);
		CameraServer::GetInstance()->StartAutomaticCapture("cam0");


	}


	/**
	 * This autonomous (along with the chooser code above) shows how to select between different autonomous modes
	 * using the dashboard. The sendable chooser code works with the Java SmartDashboard. If you prefer the LabVIEW
	 * Dashboard, remove all of the chooser code and uncomment the GetString line to get the auto name from the text box
	 * below the Gyro
	 *
	 * You can add additional auto modes by adding additional comparisons to the if-else structure below with additional strings.
	 * If using the SendableChooser make sure to add them to the chooser code above as well.
	 */
	void AutonomousInit()
	{
//		Piston->Set(DoubleSolenoid::Value::kOff);
		AR->Set(Relay::Value::kOn);
		AL->Set(Relay::Value::kOff);
		autoTimer.Reset();
		autoTimer.Start();

		autoSelected = *((std::string*)autoChooser->GetSelected());
		std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if(autoSelected == autoNameDefault)
		{
			KillAll();
		}
		else if(autoSelected == autoNameRamparts)
		{
			KillAll();
		}
		else if(autoSelected == autoNameLowbar)
		{
			KillAll();
		}
		else if(autoSelected == autoNameRoughTerrain)
		{
			KillAll();
		}
		else if(autoSelected == autoNameSallyGate)
		{
			KillAll();
		}
		else if(autoSelected == autoNameTheFrenchOne)
		{
			KillAll();
		}
		else if(autoSelected == autoNameDrawBridge)
		{
			KillAll();
		}
		else if(autoSelected == autoNameMoat)
		{
			KillAll();
		}
		else if(autoSelected == autoNameRockWall)
		{
			KillAll();
		}
		else

		{
			KillAll();
		}
	}

	void AutonomousPeriodic()
	{
		aTimer = autoTimer.Get();
//		std::cout << "times = " << aTimer << std::endl;


		if(autoSelected == autoNameDefault)
		{
			AutoDrive( 10000 , -.2 );
		}
		else if(autoSelected == autoNameRamparts)
		{
			KillAll();
		}
		else if(autoSelected == autoNameLowbar)  //Hardwire for lowbar
		{
			if ((aTimer > 0) && (aTimer < 8))
			{
				driveLeft.Set(0.25);
				driveRight.Set(-0.25);
			}
			else
			{
				KillAll();
			}
		}
		else if(autoSelected == autoNameRoughTerrain)
		{
			if ((aTimer > 0) && (aTimer < 10))
			{
				driveLeft.Set(0.25);
				driveRight.Set(-0.25);
			}
			else
			{
				KillAll();
			}
		}
		else if(autoSelected == autoNameSallyGate)
		{
			KillAll();
		}
		else if(autoSelected == autoNameTheFrenchOne)
		{
			KillAll();
		}
		else if(autoSelected == autoNameDrawBridge)
		{
			KillAll();
		}
		else if(autoSelected == autoNameMoat)
		{
			KillAll();
		}
		else if(autoSelected == autoNameRockWall)
		{
			KillAll();
		}
		else
		{
				//Default Auto goes here
//			std::cout<<"/n Time =";
//			std::cout<<timer.Get();
//
//			if (times <= 5)
//			{
//				driveLeft.Set(0.2);
//				driveRight.Set(-0.2);
//			}
//			else if ((times <= 10) && (times > 5))
//			{
//				driveLeft.Set(-0.2);
//				driveRight.Set(0.2);
//			}
//			else
//			{
//				timer.Reset();
//				timer.Start();
//			}


//			std::cout<<"\n cal = ";
//			std::cout<<ACCEL_CALIBRATION;
//			std::cout<<"\t accel = ";
//			std::cout<<accel.GetY();
//			std::cout<<"\t speed = ";
//			std::cout<<speed;
//			std::cout<<"\t dist = ";
//			std::cout<<distance;

			update();	// must be called at the end of the periodic loop
		}

//		TankDrive(gyro.GetAngle()/90,0);

		auto grip = NetworkTable::GetTable("grip");

		        /* Get published values from GRIP using NetworkTables */
		auto areas = grip->GetNumberArray("targets/area", llvm::ArrayRef<double>());

		for (auto area : areas)
		{
			std::cout << "Got contour with area=" << area << std::endl;
		}

//		Piston->Set(DoubleSolenoid::Value::kForward);
//		Wait(3);
//		Piston->Set(DoubleSolenoid::Value::kReverse);
//		Wait(3);
//		std::cout << "Piston ";
//
//		std::cout<< "\nangle = ";
//		std::cout<< gyro.GetAngle();
//		std::cout<< "\trate = ";
//		std::cout<< gyro.GetRate();

		update();	// must be called at the end of the periodic loop
	}

	void TeleopInit()
	{
		AL->Set(Relay::Value::kOn);
		AR->Set(Relay::Value::kOff);
		gyro.Calibrate();

		teleSelected = *((std::string*)teleChooser->GetSelected());
		std::string TeleSelected = SmartDashboard::GetString("Tele Selector", teleNameDefault);
		std::cout << "Tele selected: " << teleSelected << std::endl;
		KillAll();
	}

	void TeleopPeriodic()
	{
//		std::cout<< "\nangle = ";
//		std::cout<< gyro.GetAngle();
//		std::cout<< "\tP = ";
//		std::cout<< (int) turnP;
//		std::cout<< "\tI = ";
//		std::cout<< (int) turnI;
//		std::cout<< "\tD = ";
//		std::cout<< (int) turnD;

		std::cout<<encoderLeft.Get()<<"\t"<<encoderRight.Get()<<std::endl;

		if (teleSelected == teleNameSingle) 	//Single Stick Debug Tele
		/*################################################################
		 * 				Single Stick Control Scheme
		 * Joystick X and Y axis are movement control
		 * Buttons:
		 * 		Trigger- Keep angle
		 * 		Thumb-	Calibrate
		 * 		3-	Throw Ball Out
		 * 		4-	Throw Ball In
		 * 		5-	Throw Arm Down
		 * 		6-	Throw Arm Up
		 * 		7-
		 * 		8-
		 * 		9-	Servo Control ++
		 * 		10-	Servo Control --
		 * 		11-	Intake Belts In
		 * 		12-	Guide Wheels Out
		 * Throttle: Speed control - Throw Ball +=0
		 *
		 *################################################################
		*/
		{
			double	driverThrottle	=	1 - operatorStick.GetRawAxis(3);

			if (	driverThumb.Get()	)
			{
				KillDrive();
			}
			else if (	driverTrigger.Get()	)
			{
				KeepAngle(	0	,	driverThrottle * driveStick.GetRawAxis(1)	);
			}
			else
			{
				TankDrive(	driverThrottle * driveStick.GetRawAxis(0)	,	driverThrottle * driveStick.GetRawAxis(1)	);
			}

			//			if ( driveStick.GetPOV() != -1 )
			//			{
			//				targetAngle = ModAngle( -driveStick.GetPOV() );
			//			}

			if (	driverB3.Get()	)
			{
				throwHigh.Set(	(driveStick.GetRawAxis(3)+1)/2);
				throwLow.Set(	(driveStick.GetRawAxis(3)+1)/2);
			}
			else if (	driverB4.Get()	)
			{
				throwHigh.Set(	-(driveStick.GetRawAxis(3)+1)/2	);
				throwLow.Set(	-(driveStick.GetRawAxis(3)+1)/2	);
			}
			else
			{
				throwHigh.Set(0);
				throwLow.Set(0);
			}

			if (	driverB5.Get()	)
			{
				pitch.Set(	0.5	);
			}
			else if (	driverB6.Get()	)
			{
				pitch.Set(	-0.5	);
			}
			else
			{
				pitch.Set(	0	);
			}

			if (	driverB7.Get()	)
			{
				intakeArm.Set(	0.75	);
			}
			else if (	driverB8.Get()	)
			{
				intakeArm.Set(	0	);
			}

			if (	driverB9.Get()	)
			{
				intakeRollers.Set(	1	);
			}
			else if(	driverB10.Get()	)
			{
				intakeRollers.Set(	-1	);
			}
			else
			{
				intakeRollers.Set(	0	);
			}

			if (	driverB12.Get()	)
			{
				launchArm.Set(	1	);
			}
			else if (	driverB11.Get()	)
			{

				launchArm.Set(	0.75	);
			}
		}
		else			//Default Tele Code "Both Sticks"
		/*################################################################
		 * 				Double Stick Control Scheme
		 *
		 * 						Drive Stick
		 *	Joystick:
		 *		X		Turn Left/Right
		 *		Y		Forward/Backward
		 *		Z
		 *	Buttons:
		 *		Trigger
		 *		Thumb	Intake Belts in
		 * 		3		Intake Belts out
		 * 		4
		 * 		5
		 * 		6
		 * 		7
		 * 		8
		 * 		9
		 * 		10
		 * 		11
		 * 		12
		 * 	Throttle: Drive speed control
		 *						Operator Stick
		 *	Joystick:
		 * 		X
		 * 		Y	Throw Arm
		 * 		Z
		 * 	POV:
		 *
		 *	Buttons:
		 *		Trigger	Launch
		 *		Thumb	Throwing wheels out
		 * 		3		Intake in
		 * 		4		Intake Out
		 * 		5
		 * 		6		Throwing wheels in
		 * 		7
		 * 		8
		 * 		9
		 * 		10
		 * 		11
		 * 		12
		 *
		 *
		 *################################################################
		 */
		{
			/*	DRIVER	*/

			double	driverThrottle	=	1 - operatorStick.GetRawAxis(3);

			if (	driverThumb.Get()	)
			{
				KillDrive();
			}
			else if (	driverTrigger.Get()	)
			{
				KeepAngle(	0	,	driverThrottle * driveStick.GetRawAxis(1)	);
			}
			else
			{
				TankDrive(	driverThrottle * driveStick.GetRawAxis(0)	,	driverThrottle * driveStick.GetRawAxis(1)	);
			}

			if (	driverThumb.Get	)
			{
				intakeRollers.Set(	1	);
			}
			else if(	driverB3.Get()	)
			{
				intakeRollers.Set(	-1	);
			}
			else
			{
				intakeRollers.Set(	0	);
			}

			/*	OPERATOR	*/

			double	operatorThrottle	=	1 - operatorStick.GetRawAxis(3);

			pitch.Set(	operatorStick.GetRawAxis(1)	);

			if (	operatorTrigger.Get()	)
			{
				launchArm.Set(	1	);
			}
			else
			{
				launchArm.Set(	0.5	);
			}

			if (	operatorThumb.Get()	)
			{
				throwHigh.Set(	operatorThrottle );
				throwLow.Set(	operatorThrottle );
			}
			else
			{
				throwHigh.Set(0);
				throwLow.Set(0);
			}

			if (	operatorB5.Get()	)
			{
				intakeArm.Set(	0.75	);
			}
			else if (	operatorB6.Get()	)
			{
				intakeArm.Set(	0	);
			}


		}
		update();	// must be called at the end of the periodic loop
	}

	void TestPeriodic()
	{
		lw->Run();
	}

	/* Kill Functions */

	/* Unconditionally stop all motors and reset control variables.
	 */
	void	KillAll ()
	{
		driveLeft.Set	(	0	);
		driveRight.Set	(	0	);
		drivePowerLeft	=	0;
		drivePowerRight	=	0;
		drivePower		=	0;
		turnPower		=	0;
		TurnPIDReset();

		throwHigh.Set	(	0	);
		throwLow.Set	(	0	);

		pitch.Set			(	0	);
	}

	/* Unconditionally stop all drive motors and reset drive control variables.
	 */
	void	KillDrive ()
	{
		driveLeft.Set	(	0	);
		driveRight.Set	(	0	);
		drivePowerLeft	=	0;
		drivePowerRight	=	0;
		drivePower		=	0;
		turnPower		=	0;
		TurnPIDReset();

		turnPower		=	0;
		TurnPIDReset();

	}

	/* DRIVE FUNCTIONS */

	void	Drive	( double _left , double _right )
	{
		// set left motors
		driveLeft.Set	( -_left );
		// set right motors
		driveRight.Set	( _right );
	}

	void	SmoothDrive	( double _left , double _right )
	{
		if( fabs( _left ) <= DRIVE_DEADZONE && fabs( _right ) <= DRIVE_DEADZONE )
		{
			drivePowerLeft	= 0;
			drivePowerRight	= 0;
		}
		else
		{
			drivePowerLeft	+= SMOOTH_DRIVE_P_GAIN * (	_left	- 	drivePowerLeft	);
			drivePowerRight	+= SMOOTH_DRIVE_P_GAIN * (	_right	- 	drivePowerRight	);
		}
		Drive( drivePowerLeft , drivePowerRight );
	}

	void	TankDrive	( double _x , double _y )
	{
		Drive( _y - _x , _y + _x );
	}

	void	SmoothTankDrive	( double _x , double _y )
	{
		if( fabs( _x ) <= DRIVE_DEADZONE )
		{
			drivePower		=	SMOOTH_DRIVE_P_GAIN * (	_y	- 	drivePower	);
			drivePowerLeft	=	drivePower;
			drivePowerRight	=	drivePower;
		}
		else
		{
			double	_left	=	_y - _x;
			double	_right	=	_y + _x;
			drivePowerLeft	+=	SMOOTH_DRIVE_P_GAIN * (	_left	- 	drivePowerLeft	);
			drivePowerRight	+=	SMOOTH_DRIVE_P_GAIN * (	_right	- 	drivePowerRight	);
			drivePower		=	fmin( drivePowerLeft , drivePowerRight );
		}
		Drive( drivePowerLeft , drivePowerRight );
	}

	void	KeepAngle	( double _targetAngle , double _drive )
	{
		// calculate angle deviation
		double _currentAngleDeviation = AngularDifference( _targetAngle , GetAngle() );

		// increment interval
		turnInterval++;

		// calculate PID
		turnP	=	_currentAngleDeviation;
		turnI	+=	_currentAngleDeviation;
		turnD	=	-gyro.GetRate();

		// calculate turn power
		turnPower	=	TURN_K * ( TURN_P_GAIN * turnP + TURN_I_GAIN * turnI + TURN_D_GAIN * turnD );

		// limit turnPower to [-1,+1]
		if		( turnPower > +1 )	turnPower	=	+1;
		else if	( turnPower < -1 )	turnPower	=	-1;

		// calculate drive power
		drivePower	+=	SMOOTH_DRIVE_P_GAIN * (	_drive	- 	drivePower	);

		// drive the robot
		TankDrive( turnPower , drivePower );
	}

	void	SpecialTankDrive	( double _x , double _y )
	{
		if ( fabs( _x ) <= DRIVE_X_TOLERANCE || driveStick.GetTrigger() )
		{
			std::cout<<"\nSTRAIGHT DRIVE!";
			if ( !driveStraight )
			{
				gyro.Reset();
				TurnPIDReset();
			}
			if ( fabs( _x ) <= DRIVE_DEADZONE && fabs( _y ) <= DRIVE_DEADZONE )
			{
				driveStraight = false;
				Drive( 0 , 0 );
			}
			else
			{
				driveStraight = true;
				KeepAngle( 0 , _y );
				driveStraight = true;
				KeepAngle( 0 , _y );
			}
		}
		else
		{
			std::cout<<"\nSMOOTH DRIVE!";
			driveStraight = false;
			SmoothDrive( _y - _x , _y + _x );
		}
	}

	/*
	 * Automatically drive straight for a specified distance.
	 */
	bool	AutoDrive	( double _targetDistance , double _speed = 0.5 )
	{
		if ( autoDriveState == 0 )
		{
			gyro.Reset();
			TurnPIDReset();
			DistanceReset();
			autoDriveState = 1;
		}
		if ( autoDriveState == 1 )
		{
			update();
			double	distanceRemaining	=	_targetDistance - distance;
			//
			if ( distanceRemaining < 0 )
			{
				// if facing correct angle,
				if ( fabs( GetAngle() ) <= ANGLE_TOLERANCE )
				{
					// drive straight
					KeepAngle( 0 , _speed );
				}
				else
				{
					// stop to correct angle
					KeepAngle( 0 , 0 );
				}
			}
			else if ( distanceRemaining > 0 )
			{
				// if facing correct angle,
				if ( fabs( GetAngle() ) <= ANGLE_TOLERANCE )
				{
					// drive straight
					KeepAngle( 0 , _speed );
				}
				else
				{
					// stop to correct angle
					KeepAngle( 0 , 0 );
				}
			}
			else
			{
				autoDriveState = 2;
			}
		}
		if ( autoDriveState == 2 )
		{
			// if not facing the correct angle
			if ( fabs( GetAngle() ) > ANGLE_TOLERANCE )
			{
				// turn to face the correct angle
				KeepAngle( 0 , 0 );
			}
			else
			{
				autoDriveState = -1;
			}
		}
		if ( autoDriveState == -1 )
		{
			autoDriveState = 0;
			return true;
		}
		else return false;
	}

	/* GYRO FUNCTIONS */

	double	ModAngle	( double angle )
	{
		angle = angle - 360 * floorf( ( angle - 180 ) / 360 ) - 360;
		if ( angle == -180 ) angle = 180;
		return angle;
	}

	double	GetAngle	()
	{
		return ModAngle( gyro.GetAngle() );
	}

	double	AngularDifference	( double left , double right )
	{
//		std::cout<<"\n";
//		std::cout<<left;
//		std::cout<<"-";
//		std::cout<<right;
//		std::cout<<"=";
//		std::cout<<ModAngle( left - right )<<std::endl;
		return ModAngle( left - right );
	}

	void	TurnPIDReset	()
	{
		turnP			=	0;
		turnI			=	0;
		turnD			=	0;
		turnInterval	=	0;
	}

	/* ENCODER FUNCTIONS */
	void	DistanceReset	()
	{
		distance	=	0;
	}

	/* THROWER CONTROL FUNCTIONS */
//	void	setPitch	( double _target )
//	{
//		if ( _target >= 0 && _target <= 80 )
//		{
//			double _deviation = pitch - _target;
//			pitchP	=	_deviation;
//			pitchI	+=	_deviation;
//			pitchD	=	pitchSpeed;
//			pitch.Set( PITCH_K * ( PITCH_P_GAIN * pitchP + PITCH_I_GAIN * pitchI + PITCH_D_GAIN * pitchD ) );
//		}
//		else
//		{
//			pitch.Set( 0 );
//			std::cout<<"error in 'setPitch': target out of range";
//		}
//	}

	/* SENSORY */

	double	distLeft	=	0;
	double	distRight	=	0;
	double	posArm		=	0;

	double	speedRight	=	0;
	double	speedLeft	=	0;
	double	speedArm	=	0;

	double	accelLeft	=	0;
	double	accelRight	=	0;

	void	update	()
	{
		if ( tracking )
		{
			double	_timeElapsed	=	timer.Get();

			double	_newDistLeft	=	encoderLeft.GetDistance();
			double	_newDistRight	=	encoderRight.GetDistance();
			double	_newPosArm		=	encoderArm.GetDistance();

			double	_newSpeedLeft	=	(	_newDistLeft	-	distLeft	)	/	_timeElapsed;
			double	_newSpeedRight	=	(	_newDistRight	-	distRight	)	/	_timeElapsed;
			speedArm	=	(	_newPosArm	-	posArm	)	/	_timeElapsed;

			accelLeft	=	(	_newDistLeft	-	distLeft	)	/	_timeElapsed;
			accelRight	=	(	_newDistRight	-	distRight	)	/	_timeElapsed;

			speedLeft	=	_newSpeedLeft;
			speedRight	=	_newSpeedRight;

			distLeft	=	_newDistLeft;
			distRight	=	_newDistRight;

			timer.Reset();
		}
		tracking = true;
	}
	void constrain()
	{



	}
};

START_ROBOT_CLASS(Robot)
