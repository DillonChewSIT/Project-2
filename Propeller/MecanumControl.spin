 {Project: EE-14
 Platform: Parallax Project USB Board
 Revision: 1.1
 Author: Dillon Chew
 Date: 26 March 2022
 Log:
   Date: Desc
   26/3/2022 Redid cases for MecanumCore function

}


CON
        _clkmode = xtal1 + pll16x           'Standard clock mode * crystal frequency = 80 MHz
        _xinfreq = 5_000_000

        'creating a pause()
        _ConClkFreq = ((_clkmode - xtal1 >> 6) * _xinfreq)
        _Ms_001 = _ConClkFreq / 1_000


        'declare pins for motor
        'Roboclaw 1
        R1S1 = 3
        R1S2 = 2
        'Roboclaw 2
        R2S1 = 5
        R2S2 = 4

        SSBaud = 57_600

        C1Stop = 64  'Channel 1 stop value   'Right
        C2Stop = 192 'Channel 2 stop value   'Left
        Ratio = 6
VAR 'global variable

  long mecanumCoreStack[256]
  long cog2ID

OBJ
  MD[2] : "FullDuplexSerial.spin"


PUB Start (control,PWM_MAX)


  StopCore
  init
  cog2ID:=cognew(MecanumCore(control,PWM_MAX),@mecanumCoreStack)   'initialise a new core

  return

PUB Init

  MD[0].Start(R1S2,R1S1,0,SSBaud)      'initialise front and back encoders
  MD[1].Start(R2S2,R2S1,0,SSBaud)

PUB StopCore      'stop running core

  if cog2ID
    cogstop(cog2ID~)

PUB MecanumCore(control,PWM_MAX)     'control = case, PWM_MAX = max speed character value

Stop

repeat
  case long[control]                  'Move forward if case 1
    1:
      Forward(long[PWM_MAX])


    2:
      Reverse(long[PWM_MAX])          'Reverse if case 2


    3:
      ForwardDiagLeft(long[PWM_MAX])      'Forward diagonally left if case 3


    4:
      ReverseDiagRight(long[PWM_MAX])      'Reverse diagonally right if case 4


    5:
      ForwardDiagRight(long[PWM_MAX])      'Forward diagonally right if case 5


    6:
      ReverseDiagLeft(long[PWM_MAX])        'Reverse diagonally left if case 6


    7:
      Right(long[PWM_MAX])                  'Move right if case 7


    8:
      Left(long[PWM_MAX])                   'Move left if case 8


    9:
      Stop


    10:
      RotateLeft(long[PWM_MAX])             'Rotate left if case 10


    11:
       RotateRight(long[PWM_MAX])            'Rotate right if case 11


PUB Forward(PWM_MAX) | speed

    MD[0].Tx(C1Stop+PWM_MAX) 'Front Right
    MD[0].Tx(C2Stop+PWM_MAX) 'Front Left
    MD[1].Tx(C1Stop+PWM_MAX) 'Back Left
    MD[1].Tx(C2Stop+PWM_MAX) 'Back Right
    Pause(30)

    {
    MD[0].Tx(C1Stop+speed) 'Front Right
    MD[0].Tx(C2Stop+speed) 'Front Left
    MD[1].Tx(C1Stop+speed) 'Back Left
    MD[1].Tx(C2Stop+speed) 'Back Right
    Pause(30)

  MD[0].Tx(94)  'Front Right
  MD[0].Tx(222) 'Front Left
  MD[1].Tx(222) 'Back Left
  MD[1].Tx(94)  'Back Right
  }

PUB ForwardDiagLeft(PWM_MAX)| speed


     MD[0].Tx(C1Stop+PWM_MAX) 'Front Right
     MD[1].Tx(C2Stop+PWM_MAX) 'Back Left
     Pause(30)


{    Pause(30)
 repeat speed from PWM_MAX to 0 step (PWM_MAX/ratio)
     MD[0].Tx(C1Stop+speed)
     MD[1].Tx(C2Stop+speed)
     Pause(30)

  MD[0].Tx(94)
  MD[1].Tx(222)
  }

PUB ForwardDiagRight(PWM_MAX)  | speed

     MD[0].Tx(C2Stop+PWM_MAX)  'Front Left
     MD[1].Tx(C1Stop+PWM_MAX)  'Back Right
     Pause(30)
  {
 repeat speed from PWM_MAX to 0 step (PWM_MAX/ratio)
     MD[0].Tx(C2Stop+speed)
     MD[1].Tx(C1Stop+speed)
     Pause(30)

  MD[0].Tx(222)
  MD[1].Tx(94)
  }

PUB ReverseDiagLeft(PWM_MAX)  | speed

     MD[0].Tx(C2Stop-PWM_MAX)  'Front Left
     MD[1].Tx(C1Stop-PWM_MAX)  'Back Right
     Pause(30)
   {
 repeat speed from PWM_MAX to 0 step (PWM_MAX/ratio)
     MD[0].Tx(C2Stop-speed)
     MD[1].Tx(C1Stop-speed)
     Pause(30)  }

PUB ReverseDiagRight(PWM_MAX)   | speed


     MD[0].Tx(C1Stop-PWM_MAX)  'Front Right
     MD[1].Tx(C2Stop-PWM_MAX)  'Back Left
     Pause(30)

PUB Reverse(PWM_MAX)  | speed

    MD[0].Tx(C1Stop-PWM_MAX)   'Front Right
    MD[0].Tx(C2Stop-PWM_MAX)   'Front Left
    MD[1].Tx(C1Stop-PWM_MAX)   'Back Right
    MD[1].Tx(C2Stop-PWM_MAX)   'Back Left
    Pause(30)

PUB Right(PWM_MAX) | speed

     MD[0].Tx(C2Stop+PWM_MAX) 'Front Left
     MD[0].Tx(C1Stop-PWM_MAX) 'Front Right
     MD[1].Tx(C1Stop+PWM_MAX) 'Back Right
     MD[1].Tx(C2Stop-PWM_MAX) 'Back Left
     Pause(30)

PUB Left(PWM_MAX)  | speed

     MD[0].Tx(C1Stop+PWM_MAX) 'Front Right
     MD[0].Tx(C2Stop-PWM_MAX) 'Front Left
     MD[1].Tx(C2Stop+PWM_MAX) 'Back Right
     MD[1].Tx(C1Stop-PWM_MAX) 'Back Left
     Pause(30)

PUB RotateRight(PWM_MAX) | speed


      MD[0].Tx(C1Stop+PWM_MAX) 'Front Right
      MD[0].Tx(C2Stop-PWM_MAX) 'Front Left
      MD[1].Tx(C1Stop+PWM_MAX) 'Back Left
      MD[1].Tx(C2Stop-PWM_MAX) 'Back Right
      Pause(30)

PUB RotateLeft(PWM_MAX) | speed

      MD[0].Tx(C1Stop-PWM_MAX) 'Front Right
      MD[0].Tx(C2Stop+PWM_MAX) 'Front Left
      MD[1].Tx(C1Stop-PWM_MAX) 'Back Left
      MD[1].Tx(C2Stop+PWM_MAX) 'Back Right
      Pause(30)

PUB Stop

  MD[0].Tx(C1Stop)
  MD[0].Tx(C2Stop)
  MD[1].Tx(C1Stop)
  MD[1].Tx(C2Stop)
  Pause(30)

PRI Pause (ms) | t
  t := cnt - 1088
  repeat (ms#>0)
    waitcnt (t+=_Ms_001)
  return

DAT
name    byte  "string_data",0