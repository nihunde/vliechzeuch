#define RTA_OK 0x41
#define RTA_IDLE 0x42
#define RTA_CALL_INCOMING 0x43
#define RTA_DIALLING 0x44
#define RTA_CALLING 0x45
#define RTA_ACTIVE_CALL 0x46
#define RTA_END_CALL 0x47
#define RTA_EASTEREGG 0x48
#define RTA_DEBUG 0x49
#define RTA_SCREEN_LOCK 0x4a
#define RTA_NUP 0x5a

#define ATR_OK 0x61
#define ATR_ERROR 0x62
#define ATR_DIAL_NUMBER 0x63
#define ATR_ACCEPT 0x64
#define ATR_DECLINE 0x65
#define ATR_CALL_END 0x66
#define ATR_EASTEREGG_UP 0x67
#define ATR_EASTEREGG_DOWN 0x68
#define ATR_NUP 0x7a

unsigned char _BUFFER[256 + 1];  // for setting the last byte to zero

int _receive_one() {
    int length = 0;
    while (true) {
        while (Serial.available() == 0) { delay(1); }
        unsigned char c = Serial.read();
        if (c == '\r') {
            // Do nothing
        } else if (c == '\n') {
            break;
        } else {
            _BUFFER[length++] = c;
        }
    }
    return length;
}

void _send_one(unsigned int length) {
    _BUFFER[length++] = '\r';
    _BUFFER[length++] = '\n';
    Serial.write(_BUFFER, length);
}

unsigned char LAST_COMMAND_KIND;

char* CALLER;
char* DURATION;

int OWN_PADDLE_LOCATION;
int ENEMY_PADDLE_LOCATION;
int BALL_X;
int BALL_Y;

char* RTA_DEBUG_INFO;

void _receive_and_decode() {
    unsigned int length = _receive_one();
    LAST_COMMAND_KIND = _BUFFER[0];
    unsigned int colon_pos;
    switch (LAST_COMMAND_KIND) {
        case RTA_CALL_INCOMING:
            _BUFFER[length] = 0;
	    CALLER = &_BUFFER[1];
	    break;

	case RTA_END_CALL:
	    colon_pos = 1;
	    while (_BUFFER[colon_pos] != ':') ++colon_pos;
	    _BUFFER[colon_pos] = 0;
	    CALLER = &_BUFFER[1];

            _BUFFER[length] = 0;
	    DURATION = &_BUFFER[colon_pos + 1];
	    break;

	case RTA_EASTEREGG:
	    OWN_PADDLE_LOCATION = _BUFFER[1];
	    ENEMY_PADDLE_LOCATION = _BUFFER[2];
	    BALL_X = _BUFFER[3];
	    BALL_Y = _BUFFER[4];
	    break;

	case RTA_DEBUG:
            _BUFFER[length] = 0;
	    RTA_DEBUG_INFO = &_BUFFER[1];
	    break;
	
	default:
	    // Do nothing
	    break;
    }
}

void init_raspi() {
    _receive_and_decode();
}

void send(char kind, String s = "") {
    _BUFFER[0] = kind;
    if (s == "") {
        _send_one(1);
    } else {
        unsigned int i = 0;
        while (i < 255 && s[i] != 0) {
            _BUFFER[i + 1] = s[i];
	    ++i;
        }
	_send_one(i + 1);
    }
    _receive_and_decode();
}

void send_nothing() {
    send(ATR_NUP);
}
