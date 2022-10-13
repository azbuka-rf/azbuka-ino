#ifndef chernozemTCP_H
#define chernozemTCP_H
#include <Arduino.h>

const char tcpTerm[] = {'\n', ' '};

class chernozemTCP {
private:
  unsigned long tout;
  WiFiClient *port;

  bool isTerm(char &c) {
    for(const char &e : tcpTerm) {
      if(e == c) {
        return 1;
      }
    }
    return 0;
  }

  void wantNotTerm() {
    while(1) {
      if(port->available()) {
        char c = port->peek();
        if(isTerm(c)) {
          port->read();          
        } else {
          return;
        }
      }
    }
    delay(tout);
  }

  void clearTerm() {
    while(1) {
      if(port->available()) {
        char c = port->peek();
        if(isTerm(c)) {
          port->read();          
        } else {
          return;
        }
      } else {
        return;
      }
    }
    delay(tout);
  }

public:

  chernozemTCP(int _tout = 50) {
    tout = _tout;
  }

  chernozemTCP(WiFiClient *_port, int _tout = 50) {
    port = _port;
    tout = _tout;
  }

  template<class T>
  void readInt(T &ans) {
    ans = 0;
    wantNotTerm();
    bool neg = false;
    if(port->peek() == '-') {
      neg = 1;
      port->read();
      wantNotTerm();
    }
    while(port->available()) {
      char c = port->peek();
      if(c >= '0' && c <= '9') {
        ans*=10;
        ans+=c-'0';
        port->read();
      } else {
        break;
      }
      delay(tout);
    }
    if(neg)
      ans = -ans;
    clearTerm();
  }

  void readString(String &ans) {
    ans = "";
    wantNotTerm();
    while(port->available()) {
      char c = port->peek();
      if(isTerm(c)) {
        break;
      } else {
        ans += c;
        port->read();
      }
      delay(tout);
    }
    clearTerm();
  }

  template<class T>
  void readChar(T &ans) {
    wantNotTerm();
    ans = port->read();
    clearTerm();
    return;
  }

  template<class T>
  void print(T msg) {
    port->print(msg);
  }

  int available() {
    return port->available();
  }
};

chernozemTCP& operator>>(chernozemTCP &ser, char &ans) {
  ser.readChar(ans);
  return ser;
}

chernozemTCP& operator>>(chernozemTCP &ser, unsigned char &ans) {
  ser.readChar(ans);
  return ser;
}

chernozemTCP& operator>>(chernozemTCP &ser, short &ans) {
  ser.readInt(ans);
  return ser;
}

chernozemTCP& operator>>(chernozemTCP &ser, unsigned short &ans) {
  ser.readInt(ans);
  return ser;
}

chernozemTCP& operator>>(chernozemTCP &ser, int &ans) {
  ser.readInt(ans);
  return ser;
}

chernozemTCP& operator>>(chernozemTCP &ser, unsigned int &ans) {
  ser.readInt(ans);
  return ser;
}

chernozemTCP& operator>>(chernozemTCP &ser, long &ans) {
  ser.readInt(ans);
  return ser;
}

chernozemTCP& operator>>(chernozemTCP &ser, unsigned long &ans) {
  ser.readInt(ans);
  return ser;
}

chernozemTCP& operator>>(chernozemTCP &ser, long long &ans) {
  ser.readInt(ans);
  return ser;
}

chernozemTCP& operator>>(chernozemTCP &ser, unsigned long long &ans) {
  ser.readInt(ans);
  return ser;
}

chernozemTCP& operator>>(chernozemTCP &ser, String &ans) {
  ser.readString(ans);
  return ser;
}

template<class T>
chernozemTCP& operator<<(chernozemTCP &ser, T msg) {
  ser.print(msg);
  return ser;
}

#endif // chernozemTCP_H