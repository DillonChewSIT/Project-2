{Project: EE-18
 Platform: Parallax Project USB Board
 Revision: 1.0
 Author: Dillon Chew
 Date: 26 March 2022
 Log:
   Date: Desc

}

CON

        _clkmode = xtal1 + pll16x                                               'Standard clock mode * crystal frequency = 80 MHz
        _xinfreq = 5_000_000

        _ConClkFreq = ((_clkmode - xtal1) >> 6) * _xinfreq
        _Ms_001 = _ConClkFreq / 1_000

        _maxI2CDevice = 5

        ACK = 0
        NCK = 1

        ToF1GPIO = 7
        ToF2GPIO = 6

        ToFAdd = $29
        UltraAdd = $57

VAR
  long  cog3ID, cogStack[128]

OBJ
  TCA : "TCA9548Av2.spin"

PUB Start(tofMainval, ultraMainval)

  Stop
  cog3ID := cognew(Sensors(tofMainval, ultraMainval),@cogStack)
  return cog3ID

PUB Stop

  if cog3ID
    cogstop(~cog3ID)

  return

PUB Sensors(tofMainval, ultraMainval) | i

  TCA.PInit2
  Pause(100)

  TCA.Pselect(0,0)
  tofInit(0)
  Pause(100)
  TCA.PSelect(1,0)
  tofInit(1)
  Pause(100)

  repeat
    'ToF0
    TCA.PSelect(0,0)
    long[tofMainval][0] := TCA.GetSingleRange(tofadd)
    Pause(1)

    'ToF1
    TCA.PSelect(1,0)
    long[tofMainval][1] := TCA.GetSingleRange(tofadd)
    Pause(1)

    'Ultra1
    TCA.PSelect(2,0)
    TCA.PWriteByte(2,ultraadd,$01)
    Pause(30)
    long[ultraMainval][0] := TCA.readHCSR04(2,ultraadd)*100/254
    Pause(1)
    TCA.resetHCSR04(2, ultraadd)

    'Ultra2
     TCA.PSelect(3,0)
     TCA.PWriteByte(3,ultraadd,$01)
     Pause(30)
     long[ultraMainval][1] := TCA.readHCSR04(3,ultraadd)*100/254
     Pause(1)
     TCA.resetHCSR04(3, ultraadd)

     'Ultra3
     TCA.PSelect(4,0)
     TCA.PWriteByte(4,ultraadd,$01)
     Pause(30)
     long[ultraMainval][2] := TCA.readHCSR04(4,ultraadd)*100/254
     Pause(1)
     TCA.resetHCSR04(4, ultraadd)

     'Ultra4
     TCA.PSelect(5,0)
     TCA.PWriteByte(5,ultraadd,$01)
     Pause(30)
     long[ultraMainval][3] := TCA.readHCSR04(5,ultraadd)*100/254
     Pause(1)
     TCA.resetHCSR04(5, ultraadd)


PRI tofInit(x)

  if x == 0
     TCA.initVL6180X(tof1gpio)        'ToF initialisation
     TCA.ChipReset(1,tof1gpio)
     Pause(1000)
     TCA.FreshReset(tofadd)
     TCA.MandatoryLoad(tofadd)
     TCA.RecommendedLoad(tofadd)
     TCA.FreshReset(tofadd)

  if x == 1
     TCA.initVL6180X(tof2gpio)
     TCA.ChipReset(1,tof2gpio)
     Pause(1000)
     TCA.FreshReset(tofadd)
     TCA.MandatoryLoad(tofadd)
     TCA.RecommendedLoad(tofadd)
     TCA.FreshReset(tofadd)

  return
PRI Pause(ms) | t

  t := cnt - 1088
  repeat(ms #> 0)
    waitcnt(t += _Ms_001)
  return


DAT
name    byte  "string_data",0