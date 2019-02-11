package frc.robot.subsystems;

import com.revrobotics.CANSparkMax;
import com.revrobotics.CANSparkMaxLowLevel.MotorType;
import frc.robot.RobotMap;

public class Wrist extends PIDMotorJoint {
  
  public Wrist(){
    super("wrist", new CANSparkMax(RobotMap.WRIST_MOTOR, MotorType.kBrushless), 0.26269688, 45, -90, -15, 0.005, 0, 0, true, 0.1, 30,2,3);
  }

}
