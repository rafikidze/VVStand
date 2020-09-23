//  ********************  OTHER.CPP  ********************
#include <Common.h>
#include "Other.h"

unsigned char _min_(unsigned char Value_1, unsigned char Value_2)
{
  if (Value_1 < Value_2)
    return Value_1;
  else
    return Value_2;
}

unsigned char _max_(unsigned char Value_1, unsigned char Value_2)
{
  if (Value_1 < Value_2)
    return Value_2;
  else
    return Value_1;
}

unsigned short _imax_(unsigned short Value_1, unsigned short Value_2)
{
  if (Value_1 < Value_2)
    return Value_2;
  else
    return Value_1;
}
//  ********************  OTHER.CPP  ********************
