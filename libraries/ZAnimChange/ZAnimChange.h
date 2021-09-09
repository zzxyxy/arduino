typedef void (*changeF)(int);

#define ZANIMINSTANCES 100
#define ZANIMWAIT 100
#define ZAMINTIME 3000

class ZAnimChange
{
private:
    static const int m_wait = ZANIMWAIT;
    static int m_nextTime;
    static ZAnimChange* instances[ZANIMINSTANCES];
    static int instance_couter;

    ZAnimChange(int from, int to, changeF f);
    int m_from;
    int m_to;
    int m_step;
    int m_current;
    changeF* m_f;

public:
    static ZAnimChange getInstance(int from, int to, changeF f);
    static void setup();
    static void loop();

    void instanceLoop();
};
