#include <iostream>
#include <memory>
#include <string>

#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>

#include <XboxController.h>
#include <TalonSRX.h>
#include <VictorSP.h>
#include <Spark.h>
#include <Victor.h>
#include <DoubleSolenoid.h>

class Robot: public frc::IterativeRobot {
public:

	XboxController gamepad{0};

	//DriveTrain
	TalonSRX frontRight{0};
	TalonSRX frontLeft{1};
	TalonSRX backRight{2};
	TalonSRX backLeft{3};

	//ShooterAndBlender
	VictorSP shooter{12};
	VictorSP blender{13};

	//Climber
	Spark climber{14};

	//UptakeAndIntake
	Spark uptake{15};
	Victor intake{11};

	//Solenoids
	DoubleSolenoid leftDropDowns{0, 1};
	DoubleSolenoid rightDropDowns{2, 3};
	DoubleSolenoid intakeSolenoid{4, 5};
	DoubleSolenoid gearHolder{6, 7};

	//DriveTrainVariables
	bool mecanumButton;
	bool down;
	double driveX;
	double driveY;
	double driveRotate;
	double driveOff;

	//ShooterAndBlenderVariables
	bool shooterBlenderButton;
	bool blenderState;
	bool shooterBlenderGo;
	double shooterBlenderSpeed;
	double shooterBlenderOff;
	double speed;

	//IntakeAndUptakeVariables
	bool intakeUptakeButton;
	bool intakeState;
	bool intakeDown;
	bool intakeReverseButton;
	bool previousIntakeUptake;
	double intakeSpeed;
	double intakeUptakeSpeed;
	double intakeUptakeOff;
	double intakeReverseSpeed;

	//GearHolder
	bool gearButton;
	bool previousGear;
	bool gearState;


	//Math
	double expoCurve;
	double deadZoneArea;

	void RobotInit() {
		chooser.AddDefault(autoNameDefault, autoNameDefault);
		chooser.AddObject(autoNameCustom, autoNameCustom);
		frc::SmartDashboard::PutData("Auto Modes", &chooser);

		//Solenoids
		leftDropDowns.Set(frc::DoubleSolenoid::kOff);
		rightDropDowns.Set(frc::DoubleSolenoid::kOff);
		intakeSolenoid.Set(frc::DoubleSolenoid::kOff);
		gearHolder.Set(frc::DoubleSolenoid::kOff);

		//DriveTrainVariables
		mecanumButton = false;
		down 		  = false;
		driveX        = 0.0;
		driveY 		  = 0.0;
		driveRotate   = 0.0;
		driveOff 	  = 0.0;

		//ShooterVariables
		shooterBlenderButton = false;
		shooterBlenderGo     = false;
		blenderState         = false;
		shooterBlenderSpeed  = 1.0;
		shooterBlenderOff    = 0.0;
		speed                = 0.0;

		//IntakeAndUptakeVariables
		intakeUptakeButton   = false;
		intakeDown           = false;
		previousIntakeUptake = false;
		intakeState 		 = false;
		intakeReverseButton  = false;
		intakeUptakeSpeed    = 1.0;
		intakeUptakeOff      = 0.0;
		intakeSpeed          = 0.0;
		intakeReverseButton  =-1.0;

		//GearHolder
		gearButton   = false;
		previousGear = false;
		gearState    = false;

		//Math
		expoCurve    = 1.0;
		deadZoneArea = 0.2;

	}

	/*
	 * This autonomous (along with the chooser code above) shows how to select
	 * between different autonomous modes using the dashboard. The sendable
	 * chooser code works with the Java SmartDashboard. If you prefer the
	 * LabVIEW Dashboard, remove all of the chooser code and uncomment the
	 * GetString line to get the auto name from the text box below the Gyro.
	 *
	 * You can add additional auto modes by adding additional comparisons to the
	 * if-else structure below with additional strings. If using the
	 * SendableChooser make sure to add them to the chooser code above as well.
	 */
	void AutonomousInit() override {
		autoSelected = chooser.GetSelected();
		// std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if (autoSelected == autoNameCustom) {
			// Custom Auto goes here
		} else {
			// Default Auto goes here
		}
	}

	void AutonomousPeriodic() {
		if (autoSelected == autoNameCustom) {
			// Custom Auto goes here
		} else {
			// Default Auto goes here
		}
	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {

		while(IsOperatorControl() && IsEnabled){

			drive();
			intakeDrop();
			shooterAndBlender();
			intakeAndUptake();
			gearFuction();

		}

	}

	void TestPeriodic() {
		lw->Run();
	}

private:
	frc::LiveWindow* lw = LiveWindow::GetInstance();
	frc::SendableChooser<std::string> chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;

	void intakeDrop(){

		if(IsOperatorControl() && IsEnabled){

			intakeSolenoid.Set(frc::DoubleSolenoid::kForward);

		}

	}

	void drive(){

		mecanumButton = gamepad.GetRawButton(5);
		driveX        = gamepad.GetRawAxis(0);
		driveY        = gamepad.GetRawAxis(1);
		driveRotate   = gamepad.GetRawAxis(4);

		if(mecanumButton & !down){

			arcadeDrive(gamepad.GetRawAxis(0), gamepad.GetRawAxis(1));
			leftDropDowns.Set(frc::DoubleSolenoid::kReverse);
			rightDropDowns.Set(frc::DoubleSolenoid::kReverse);
			down  = true;

		} else if(mecanumButton && down){

			mecanumDrive(gamepad.GetRawAxis(0), gamepad.GetRawAxis(1), gamepad.GetRawAxis(4));
			leftDropDowns.Set(frc::DoubleSolenoid::kForward);
			rightDropDowns.Set(frc::DoubleSolenoid::kForward);
			down =  false;


		}

	}

	void arcadeDrive(double x, double y){


		frontRight.Set(constrain(expo(y + x, expoCurve), -1, 1));
		frontLeft.Set(constrain(expo(-y + x, expoCurve), -1, 1));
		backRight.Set(constrain(expo(y - x, expoCurve), -1, 1));
		backLeft.Set(constrain(expo(-y - x, expoCurve), -1, 1));


	}

	void mecanumDrive(double x, double y, double z){


			frontRight.Set(constrain(expo(z + y + x, expoCurve), -1, 1));
			frontLeft.Set(constrain(expo(z - y + x, expoCurve), -1, 1));
			backRight.Set(constrain(expo(z + y - x, expoCurve), -1, 1));
			backLeft.Set(constrain(expo(z - y - x, expoCurve), -1, 1));

		}

	void shooterAndBlender(){

		shooterBlenderButton = gamepad.GetRawAxis(3);

		if(!blenderState && shooterBlenderButton > 0.2 && !shooterBlenderGo){

			speed = shooterBlenderSpeed;
			shooterBlenderGo = true;

		} else if(blenderState && shooterBlenderButton > 0.2 && !shooterBlenderGo){

			speed = shooterBlenderOff;
			shooterBlenderGo = true;

		} else if(shooterBlenderButton < 0.2){

			shooterBlenderGo = false;

		}

		blender.Set(speed);
		shooter.Set(-speed);

	}

	void intakeAndUptake(){

		intakeUptakeButton = gamepad.GetRawButton(1);
		intakeReverseButton = gamepad.GetRawAxis(4);

		if(!intakeState && intakeUptakeButton && !previousIntakeUptake){

			intakeSpeed = intakeUptakeSpeed;
			intakeState = true;

		} else if(intakeState && intakeUptakeButton && !previousIntakeUptake){

			intakeSpeed = intakeUptakeOff;
			intakeState = false;

		}

		if(intakeReverseButton){

			intake.Set(intakeReverseSpeed);
			uptake.Set(intakeReverseSpeed);

		} else{

			intake.Set(intakeSpeed);
			uptake.Set(intakeSpeed);
			previousIntakeUptake = intakeUptakeButton;

		}

	}

	void gearFuction(){

		gearButton = gamepad.GetRawButton(3);

		if(IsOperatorControl() && IsEnabled){

			gearHolder.Set(frc::DoubleSolenoid::kForward);

		} else if(!gearState && gearButton && !previousGear){

			gearHolder.Set(frc::DoubleSolenoid::kForward);
			gearState = true;

		} else if(gearState && gearButton && !previousGear){

			gearHolder.Set(frc::DoubleSolenoid::kReverse);
			gearState = false;

		}


	}

	double constrain(double originial, double min, double max){

				if(originial < min){

					originial = min;

				}
				if(originial > max){

					originial = max;

				}

				return originial;
			}

		double expo(double x, double a){

		        double y = x;
		        y = a * pow(y, 3) + (1-a)*y;
		        return y;

		    }

		void deadzone(){

			double x = abs(gamepad.GetRawAxis(0));
			double y = abs(gamepad.GetRawAxis(1));

			if (sqrt((x*x)+(y*y)) > deadZoneArea){
				arcadeDrive(gamepad.GetRawAxis(0), gamepad.GetRawAxis(1));
			}
			else {
				frontRight.Set(0.0);
				frontLeft.Set(0.0);
				backRight.Set(0.0);
				backLeft.Set(0.0);
};

START_ROBOT_CLASS(Robot)
