
//  ********************  MUTEX.H  ********************

class CMutex
{
public:
CMutex ()
{
_state = __save_interrupt();
__disable_interrupt();
}
~CMutex ()
{
__restore_interrupt(_state);
}
private:
unsigned char _state;
};


//  ********************  MUTEX.H  ********************

