  {Project: EE-18
 Platform: Parallax Project USB Board
 Revision: 1.1
 Author: Dillon Chew
 Date: 26 March 2022
 Log:
   Date: Desc
   3/3/2022: Added sensormux object file and comms2control object file
   26/3/2022: Added object files sensormuxcontrol and encapsulated all three object files
}


CON
        _clkmode = xtal1 + pll16x           'Standard clock mode * crystal frequency = 80 MHz
        _xinfreq = 5_000_000

        'creating a pause()
        _ConClkFreq = ((_clkmode - xtal1 >> 6) * _xinfreq)
        _Ms_001 = _ConClkFreq / 1_000
        UltraSafeVal = 450
        TOFSafeVal = 250
        'MotorControl constants


VAR     'sensor variables
  long  tofMainval[2], ultraMainval[4]
        'motor variables
  long  control,PWM_MAX
        'comm variables
  long spd, dir
 ' long  decisionMode

OBJ
 'Term        : "FullDuplexSerial.spin"
  MechCon    : "MecanumControl.spin"
  SensCon    : "SensorMUXControl.spin"
  CommsCon   : "Comm2Control.spin"

PUB Main | i

    CommsCon.Start(@spd, @dir)  'initialise comms cog
    SensCon.Start(@tofMainval, @ultraMainval) 'initialise sensor cog
    'Term.Start(31,30,0,230400)   'initialise terminal
    MechCon.Start(@control,@PWM_MAX) 'initialise motor cog


    repeat
      case dir
        1:
          if ((tofMainval[0] > 0 and tofMainval[0] > tofsafeval) or (tofMainval[1] > 0 and tofMainval[1] > tofsafeval) or (ultraMainval[0] > 0 and ultraMainval[0] < ultrasafeval) or (ultraMainval[1] > 0 and ultraMainval[1] < ultrasafeval) or (ultraMainval[2] > 0 and ultraMainval[2] < ultrasafeval) or (ultraMainval[3] > 0 and ultraMainval[3] < ultrasafeval))
            control:= 9         'stop if any of the sensors detect an object
            PWM_MAX:= 0
          else
            control:= dir       'Move forward if case 1
            pwm_max:= spd


        2:
          if ((tofMainval[0] > 0 and tofMainval[0] > tofsafeval) or (tofMainval[1] > 0 and tofMainval[1] > tofsafeval) or (ultraMainval[0] > 0 and ultraMainval[0] < ultrasafeval) or (ultraMainval[1] > 0 and ultraMainval[1] < ultrasafeval) or (ultraMainval[2] > 0 and ultraMainval[2] < ultrasafeval) or (ultraMainval[3] > 0 and ultraMainval[3] < ultrasafeval))
            control:= 9          'stop if any of the sensors detect an object
            PWM_MAX:= 0
          else
            control:= dir       'reverse if case 2
            PWM_MAX:= spd

        3:
          if ((tofMainval[0] > 0 and tofMainval[0] > tofsafeval) or (tofMainval[1] > 0 and tofMainval[1] > tofsafeval) or (ultraMainval[0] > 0 and ultraMainval[0] < ultrasafeval) or (ultraMainval[1] > 0 and ultraMainval[1] < ultrasafeval) or (ultraMainval[2] > 0 and ultraMainval[2] < ultrasafeval) or (ultraMainval[3] > 0 and ultraMainval[3] < ultrasafeval))
            control:= 9         'stop if any of the sensors detect an object
            PWM_MAX:= 0
          else
            control:= dir       'move forward diagonal left if case 3
            PWM_MAX:= spd
        4:
         if ((tofMainval[0] > 0 and tofMainval[0] > tofsafeval) or (tofMainval[1] > 0 and tofMainval[1] > tofsafeval) or (ultraMainval[0] > 0 and ultraMainval[0] < ultrasafeval) or (ultraMainval[1] > 0 and ultraMainval[1] < ultrasafeval) or (ultraMainval[2] > 0 and ultraMainval[2] < ultrasafeval) or (ultraMainval[3] > 0 and ultraMainval[3] < ultrasafeval))
           control:= 9         'stop if any of the sensors detect an object
           PWM_MAX:= 0
         else
            control:= dir   'reverse diagonal right if case 4
            PWM_MAX:= spd
        5:
         if ((tofMainval[0] > 0 and tofMainval[0] > tofsafeval) or (tofMainval[1] > 0 and tofMainval[1] > tofsafeval) or (ultraMainval[0] > 0 and ultraMainval[0] < ultrasafeval) or (ultraMainval[1] > 0 and ultraMainval[1] < ultrasafeval) or (ultraMainval[2] > 0 and ultraMainval[2] < ultrasafeval) or (ultraMainval[3] > 0 and ultraMainval[3] < ultrasafeval))
           control:= 9        'stop if any of the sensors detect an object
           PWM_MAX:= 0
         else
            control:= dir   'move forward diagonal right if case 5
            PWM_MAX:= spd
        6:
         if ((tofMainval[0] > 0 and tofMainval[0] > tofsafeval) or (tofMainval[1] > 0 and tofMainval[1] > tofsafeval) or (ultraMainval[0] > 0 and ultraMainval[0] < ultrasafeval) or (ultraMainval[1] > 0 and ultraMainval[1] < ultrasafeval) or (ultraMainval[2] > 0 and ultraMainval[2] < ultrasafeval) or (ultraMainval[3] > 0 and ultraMainval[3] < ultrasafeval))
           control:= 9         'stop if any of the sensors detect an object
           PWM_MAX:= 0
         else
            control:= dir   'reverse diagonal left if case 6
            PWM_MAX:= spd

        7:
         if ((tofMainval[0] > 0 and tofMainval[0] > tofsafeval) or (tofMainval[1] > 0 and tofMainval[1] > tofsafeval) or (ultraMainval[0] > 0 and ultraMainval[0] < ultrasafeval) or (ultraMainval[1] > 0 and ultraMainval[1] < ultrasafeval) or (ultraMainval[2] > 0 and ultraMainval[2] < ultrasafeval) or (ultraMainval[3] > 0 and ultraMainval[3] < ultrasafeval))
           control:= 9      'stop if any of the sensors detect an object
           PWM_MAX:=0
         else
            control:= dir   'move left if case 7
            PWM_MAX:= spd
        8:
         if ((tofMainval[0] > 0 and tofMainval[0] > tofsafeval) or (tofMainval[1] > 0 and tofMainval[1] > tofsafeval) or (ultraMainval[0] > 0 and ultraMainval[0] < ultrasafeval) or (ultraMainval[1] > 0 and ultraMainval[1] < ultrasafeval) or (ultraMainval[2] > 0 and ultraMainval[2] < ultrasafeval) or (ultraMainval[3] > 0 and ultraMainval[3] < ultrasafeval))
          control:= 9         'stop if any of the sensors detect an object
          PWM_MAX:= 0
         else
            control:= dir   'move left if case 8
            PWM_MAX:= spd


        9:
         control:= spd      'stop if case 9
         PWM_MAX:= 0






PRI Pause (ms) | t
  t := cnt - 1088
  repeat (ms#>0)
    waitcnt (t+=_Ms_001)
  return

DAT
name    byte  "string_data",0