/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package frc.robot.subsystems;

import java.util.HashMap;

import com.revrobotics.CANEncoder;
import com.revrobotics.CANSparkMax;
import com.revrobotics.CANSparkMaxLowLevel.MotorType;

import edu.wpi.first.wpilibj.DigitalInput;
import edu.wpi.first.wpilibj.command.PIDSubsystem;
import frc.robot.RobotMap;
import frc.robot.Robot;
import frc.robot.Util;

public class Arm extends PIDSubsystem {
  
  private double m_zeroEncoder, m_deadstopPosition, m_targetPosition, m_maxSpeed;
  private double m_prevEncoder, m_currEncoder;
  private CANSparkMax m_armMotor;
  private CANEncoder m_armEncoder;
  private DigitalInput m_frontSwitch, m_backSwitch;

  private HashMap<String, Double> m_presets;

  public Arm() {
    super("arm", 0.0075, 0, 0);

    m_armMotor = new CANSparkMax(RobotMap.ARM_MOTOR, MotorType.kBrushless);
    m_armEncoder = new CANEncoder(m_armMotor);

    m_frontSwitch = new DigitalInput(RobotMap.ARM_FRONT_SWITCH);
    m_backSwitch = new DigitalInput(RobotMap.ARM_BACK_SWITCH);

    m_currEncoder = m_armEncoder.getPosition();
    m_prevEncoder = m_currEncoder;
    m_zeroEncoder = m_currEncoder;

    m_maxSpeed = 0.2;

    m_deadstopPosition = 0;

    m_presets = new HashMap<String, Double>();

    m_presets.put("GROUND", 50.0);
    m_presets.put("CARGOSHIP", -10.0);
    m_presets.put("ROCKET_1", 12.0);
    m_presets.put("HOME", 0.0);
    
    enable();
  }

  public void goToPreset(String preset){ 
    setPosition(m_presets.get(preset));
  }

  public void resetPosition(){
    m_zeroEncoder = m_armEncoder.getPosition();
    setPosition(getPosition());
  }

  @Override
  public void initDefaultCommand() {
    // Set the default command for a subsystem here.
    // setDefaultCommand(new MySpecialCommand());
  }

  @Override
  protected double returnPIDInput() {
    checkOutOfBounds();
    return getArmPosition();
  }

  private double getArmPosition(){
    m_currEncoder = m_armEncoder.getPosition();
    if (Math.abs(m_currEncoder) < 0.001) {
      m_currEncoder = m_prevEncoder;
    } else {
      m_prevEncoder = m_currEncoder;
    }
    return (m_currEncoder - m_zeroEncoder) + m_deadstopPosition;
  }

  private void checkOutOfBounds() {
    boolean goingUp = m_targetPosition - getArmPosition() <= 0;
    if (goingUp && !m_backSwitch.get()) {
      resetPosition();
      setPosition(getArmPosition());
    } else if (!goingUp && !m_frontSwitch.get()) {
      setPosition(getArmPosition());
    } 
  }

  private void setPosition(double position){
    m_targetPosition = position;
    setSetpoint(position);
  }

  @Override
  protected void usePIDOutput(double output) {
    //System.out.println(getArmPosition());
    System.out.println(Util.checkSpeed(output, m_maxSpeed));
    //m_armMotor.set(0.3*Robot.m_oi.m_operator.getRawAxis(Robot.m_oi.LJY));
    //System.out.println(0.3*Robot.m_oi.m_operator.getRawAxis(Robot.m_oi.LJY));
    m_armMotor.set(Util.checkSpeed(output, m_maxSpeed));
  }
}
