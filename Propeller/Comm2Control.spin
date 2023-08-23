 {Project: EE-18
 Platform: Parallax Project USB Board
 Revision: 1.1
 Author: Dillon Chew
 Date: 26 March 2022
 Log:
   Date: Desc
   26/3/2022 Redid cases for CommCore function

}

CON
        _clkmode = xtal1 + pll16x           'Standard clock mode * crystal frequency = 80 MHz
        _xinfreq = 5_000_000

        'creating a pause()
        _ConClkFreq = ((_clkmode - xtal1 >> 6) * _xinfreq)
        _Ms_001 = _ConClkFreq / 1_000

        'Rx commands
        key = $7F  'hexkey assigned
        forward = $01
        reverse = $02
        forward_dl = $03
        reverse_dr = $04
        forward_dr = $05
        reverse_dl = $06
        right = $07
        left = $08
        stp = $0B


        commRxPin=14   'DOUT
        commTxPin=15  'DIN
        commBaud=115200


VAR
  long cogIDNum, cogStack[128], rxHead, rxDir, rxSpeed,rxChecksum, CS'Stack space for cog

OBJ
  Comm        : "FullDuplexSerial.spin"
  'Term        : "FullDuplexSerial.spin"

PUB Start(spd, dir)
  Stop

  cogIDNum := cognew(CommCore(spd, dir), @cogStack)    'initialise comms cog

  return

PUB Stop

  if cogIDNum
    cogstop (cogIDNum~)

PUB CommCore(spd, dir) | Rx

  'term.start(31,30,0,230400)
  Comm.Start(commrxPin,commtxPin,0,CommBaud)

  repeat
    rxHead:=Comm.Rx             'receive header byte
    rxDir:= Comm.Rx             'receive direction byte
    rxSpeed:= Comm.Rx           'receive speed byte
    rxChecksum:= Comm.Rx        'receive checksum byte
    CS := (rxSpeed^rxDir)^key   'XOR speed, direction and key
      if (CS == rxChecksum)     'Check if the resultant XOR is equals to checksum
        case rxdir
          forward:
           long[dir] := 1
          reverse:
           long[dir] := 2
          forward_dl:
           long[dir] := 3
          reverse_dr:
           long[dir] := 4
          forward_dr:
           long[dir] := 5
          reverse_dl:
           long[dir] := 6
          right:
           long[dir] := 7
          left:
           long[dir] := 8
          stp:
           long[dir] := 9

        long[spd] := rxspeed
    pause(100)




PRI Pause (ms) | t
  t := cnt - 1088
  repeat (ms#>0)
    waitcnt (t+=_Ms_001)
  return

DAT
name    byte  "string_data",0