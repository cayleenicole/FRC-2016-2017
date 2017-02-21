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
#include <Timer.h>

class Robot: public frc::IterativeRobot {
public:

	XboxController gamePad{0};

	TalonSRX frontRight{0};
	TalonSRX frontLeft{1};
	VictorSP backRight{2};
	VictorSP backLeft{3};

	double driveX;
	double driveY;
	double driveRotate;

	void RobotInit() {
		chooser.AddDefault(autoNameDefault, autoNameDefault);
		chooser.AddObject(autoNameCustom, autoNameCustom);
		frc::SmartDashboard::PutData("Auto Modes", &chooser);

		driveX      = 0.0;
		driveY      = 0.0;
		driveRotate = 0.0;

	}

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

		while (IsOperatorControl() && IsEnabled()){

			drive();

			frc::Wait(0.005);

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

	void drive(){

			driveX      = gamePad.GetRawAxis(0);
			driveY      = gamePad.GetRawAxis(1);
			driveRotate = gamePad.GetRawAxis(4);


			frontRight.Set(constrain(driveRotate + driveY + driveX, -1, 1));
			frontLeft.Set(constrain(driveRotate - driveY + driveX, -1, 1));
			backRight.Set(constrain(driveRotate - driveY + driveX, -1, 1));
			backLeft.Set(constrain(driveRotate - driveY  - driveX, -1, 1));

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
};

START_ROBOT_CLASS(Robot)
