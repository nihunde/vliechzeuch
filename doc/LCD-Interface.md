# Screens
-  Idle Animation
-  Call Incoming (mit Nummer/Text)
-  Dialing (eingeben) 
-  Calling (verbindungsaufbau)
-  Active Call
-  End Call
-  Easteregg pong gegen sitznachbar
-  Debug
-  Displaysperre

Commands
Schema:
Command: <opcode><paramLength><params>\r\n bzw. <opcode>\r\n

## Raspi to Phone
OpCode | Command | WithParam | Maxlength | What to display
:---: | --- | :---: | :---: | ---
0x41 | Idle | No | 0 | Fairydust spinning
0x42 | CallIncoming | Yes | 255 | number and/or name of caller 
0x43 | Dialing | No | 0 | earpiece Icon and number to dial
0x44 | Calling | No | 0 | earpiece Icon and number to dial
0x45 | Active Call | No | 0 | earpiece Icon and number of call partner
0x46 | End Call | Yes | 255 | number called and call duration (if possible)
0x47 | Easteregg | Yes | 4 | Position Paddle(own and competitor) and x-y-coord. of ball 
0x48 | Debug | Yes | 255 | shows debug info from RasPi
0x49 | ScreenLock | No | 0 | like Nokia phone

## Phone to RasPi
OpCode | Command | WithParam | Maxlength | Comment
:---: | --- | :---: | :---: | ---
0x61 | OK | No | 0 | Command successfull!
0x62 | Error | Yes | 255 | Message/ Errorcode
0x63 | DialNumber | Yes | 255 | Number to dial
0x64 | Accept | No | 0 | If call is accepted
0x65 | Decline | No | 0 | If call is declined
0x66 | CallEnd | No | 0 | End the call
0x67 | EastereggUp | No | 0 | Move the Paddle up
0x68 | EastereggDown | No | 0 | Move the Paddle down
