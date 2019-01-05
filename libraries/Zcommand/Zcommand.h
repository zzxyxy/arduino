#ifndef ZCOMMAND_H
#define ZCOMMAND_H

typedef void (*commFunc)();

class Zcommand
{
public:
  Zcommand();
  Zcommand(const Zcommand& orig);
  virtual ~Zcommand();
  void registerCommand(char* command, int commandlength, void (*f)());
  void serialCommand(char in);
  char* getLastCommand();
  void run();
  bool compareStr(char* x, char* y, int l);
  
private:
  static const int MAXSERIALCOMMANDLENGTH = 30;
  
  struct item {
    char* command;
    int commandlength;
    commFunc f;
  };

  struct item m_list[30];
  int m_regCounter = 0;
  
  char serialcmd[MAXSERIALCOMMANDLENGTH];
  int serialcmdpoint;
};

#endif /* ZCOMMAND_H */

