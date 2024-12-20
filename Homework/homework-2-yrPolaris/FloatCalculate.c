#include "FloatCalculate.h"
#include <stdbool.h>
#include <math.h>

const size_t SIGN_BIT = 1;
const size_t EXPONENT_BITS = 8;
const size_t MANTISSA_BITS = 23;

// static int32_t get_norm_bias(void) { return 1 - (1 << (EXPONENT_BITS - 1)); }

// static int32_t get_denorm_bias(void) { return 1 + get_norm_bias(); }

static bool test_rightmost_all_zeros(uint32_t number, size_t bits)
{
  uint32_t mask = (1ull << bits) - 1;
  return (number & mask) == 0;
}

// static bool test_rightmost_all_ones(uint32_t number, size_t bits)
// {
//   uint32_t mask = (1ull << bits) - 1;
//   return (number & mask) == mask;
// }

// You can also design a function of your own.
static void build_bitstring(Float input, char *output)
{
  output[0] = (char)(48 + input.sign);
  for (int i = 1; i <= 8; i++)
  {
    if (input.exponent >= (uint32_t)(1 << (8 - i)))
    {
      output[i] = '1';
      input.exponent -= (uint32_t)(1 << (8 - i));
    }
    else
    {
      output[i] = '0';
    }
  }
  for (int i = 9; i <= 31; i++)
  {
    if (input.mantissa >= (uint32_t)(1 << (31 - i)))
    {
      output[i] = '1';
      input.mantissa -= (1 << (31 - i));
    }
    else
    {
      output[i] = '0';
    }
  }
}

// You can also design a function of your own.
static Float parse_bitstring(const char *input)
{
  Float f;
  f.sign = 0;
  f.exponent = 0;
  f.mantissa = 0;
  f.type = NORMALIZED_T;
  if (input[0] == '1')
  {
    f.sign = 1;
  }
  for (int i = 1; i <= 8; i++)
  {
    f.exponent += (input[i] - 48) * (1 << (8 - i));
  }
  for (int i = 9; i <= 31; i++)
  {
    f.mantissa += (input[i] - 48) * (1 << (31 - i));
  }
  if (test_rightmost_all_zeros(f.exponent, 8) && !(test_rightmost_all_zeros(f.mantissa, 23)))
  {
    f.type = DENORMALIZED_T; // 这边需要调整一下
    f.exponent = 1;
  }
  if (f.type == NORMALIZED_T)
  {
    f.mantissa += (1 << 23);
  }
  f.mantissa <<= 2;
  return f;
}

// You can also design a function of your own.
static Float float_add_impl(Float a, Float b)
{
  Float f;
  f.sign = 0;
  f.exponent = 0;
  f.mantissa = 0;
  f.type = NORMALIZED_T;
  bool ROUND_RIGHT_EXIT_1 = 0;
  // 判断stick bit 是否为1

  if (a.exponent > b.exponent)
  {
    f.exponent = a.exponent;
    for (uint32_t i = 0; i < (a.exponent - b.exponent); i++)
    {
      if (b.mantissa % 2 == 1 && ROUND_RIGHT_EXIT_1 == false)
      {
        ROUND_RIGHT_EXIT_1 = true;
      }
      b.mantissa >>= 1;
      // 向右shift，保证两个数指数相同
    }
    a.mantissa <<= 1;
    b.mantissa <<= 1;
    b.mantissa += ROUND_RIGHT_EXIT_1;
    // 把stick bit加上，此时normalized有27位，denorm有26位

    if (a.sign == 0)
    {
      if (b.sign == 0)
      {
        f.mantissa = a.mantissa + b.mantissa;
        if (f.mantissa >= (1 << 27))
        {
          f.exponent += 1;
          f.mantissa >>= 4;
        }
        else
        {
          f.mantissa >>= 3;
        }
        if (f.mantissa >= (1 << 23))
        {
          f.mantissa -= (1 << 23);
        }
      }
      // 两个正数相加的情况

      else if (b.sign == 1)
      {
        f.mantissa = a.mantissa - b.mantissa;
        f.mantissa >>= 3;
        while (f.mantissa < (1 << 23) && f.exponent > 1)
        {
          f.exponent -= 1;
          f.mantissa <<= 1;
        }
        if (f.mantissa >= (1 << 23))
        {
          f.mantissa -= (1 << 23);
        }
        else if (f.mantissa < (1 << 23))
        {
          f.type = DENORMALIZED_T;
        }
      }
      // 大减小
    }

    else if (a.sign == 1)
    {
      f.sign = 1;
      if (b.sign == 1)
      {
        f.mantissa = a.mantissa + b.mantissa;
        if (f.mantissa >= (1 << 27))
        {
          f.exponent += 1;
          f.mantissa >>= 4;
        }
        else
        {
          f.mantissa >>= 3;
        }
        if (f.mantissa >= (1 << 23))
        {
          f.mantissa -= (1 << 23);
        }
      }

      else if (b.sign == 0)
      {
        f.mantissa = a.mantissa - b.mantissa;
        f.mantissa >>= 3;
        while (f.mantissa < (1 << 23) && f.exponent > 1)
        {
          f.exponent -= 1;
          f.mantissa <<= 1;
        }
        if (f.mantissa >= (1 << 23))
        {
          f.mantissa -= (1 << 23);
        }
        else if (f.mantissa < (1 << 23))
        {
          f.type = DENORMALIZED_T;
        }
      }
    }
  }

  else if (a.exponent < b.exponent)
  {
    f.exponent = b.exponent;
    for (uint32_t i = 0; i < (b.exponent - a.exponent); i++)
    {
      if (a.mantissa % 2 == 1 && ROUND_RIGHT_EXIT_1 == false)
      {
        ROUND_RIGHT_EXIT_1 = true;
      }
      a.mantissa >>= 1;
      // 向右shift，保证两个数指数相同
    }
    b.mantissa <<= 1;
    a.mantissa <<= 1;
    a.mantissa += ROUND_RIGHT_EXIT_1;
    // 把stick bit加上，此时normalized有27位，denorm有26位

    if (b.sign == 0)
    {
      if (a.sign == 0)
      {
        f.mantissa = a.mantissa + b.mantissa;
        if (f.mantissa >= (1 << 27))
        {
          f.exponent += 1;
          f.mantissa >>= 4;
        }
        else
        {
          f.mantissa >>= 3;
        }
        if (f.mantissa >= (1 << 23))
        {
          f.mantissa -= (1 << 23);
        }
      }
      // 两个正数相加的情况

      else if (a.sign == 1)
      {
        f.mantissa = b.mantissa - a.mantissa;
        f.mantissa >>= 3;
        while (f.mantissa < (1 << 23) && f.exponent > 1)
        {
          f.exponent -= 1;
          f.mantissa <<= 1;
        }
        if (f.mantissa >= (1 << 23))
        {
          f.mantissa -= (1 << 23);
        }
        else if (f.mantissa < (1 << 23))
        {
          f.type = DENORMALIZED_T;
        }
      }
      // 大减小
    }

    else if (b.sign == 1)
    {
      f.sign = 1;
      if (a.sign == 1)
      {
        f.mantissa = a.mantissa + b.mantissa;
        if (f.mantissa >= (1 << 27))
        {
          f.exponent += 1;
          f.mantissa >>= 4;
        }
        else
        {
          f.mantissa >>= 3;
        }
        if (f.mantissa >= (1 << 23))
        {
          f.mantissa -= (1 << 23);
        }
      }
      else if (a.sign == 0)
      {
        f.mantissa = b.mantissa - a.mantissa;
        f.mantissa >>= 3;
        while (f.mantissa < (1 << 23) && f.exponent > 1)
        {
          f.exponent -= 1;
          f.mantissa <<= 1;
        }
        if (f.mantissa >= (1 << 23))
        {
          f.mantissa -= (1 << 23);
        }
        else if (f.mantissa < (1 << 23))
        {
          f.type = DENORMALIZED_T;
        }
      }
    }
  }

  else if (a.exponent == b.exponent)
  {
    f.exponent = a.exponent;
    if (a.sign == 0)
    {
      if (b.sign == 0)
      {
        f.mantissa = a.mantissa + b.mantissa;
        if (f.mantissa >= (1 << 27))
        {
          f.exponent += 1;
          f.mantissa >>= 4;
        }
        else
        {
          f.mantissa >>= 3;
        }
        if (f.mantissa >= (1 << 23))
        {
          f.mantissa -= (1 << 23);
        }
      }
      else if (b.sign == 1)
      {
        if (a.mantissa >= b.mantissa)
        {
          f.mantissa = a.mantissa - b.mantissa;
        }
        else if (a.mantissa < b.mantissa)
        {
          f.mantissa = b.mantissa - a.mantissa;
          f.sign = 1;
        }
        f.mantissa >>= 3;
        while (f.mantissa < (1 << 23) && f.exponent > 1)
        {
          f.exponent -= 1;
          f.mantissa <<= 1;
        }
        if (f.mantissa >= (1 << 23))
        {
          f.mantissa -= (1 << 23);
        }
        else if (f.mantissa < (1 << 23))
        {
          f.type = DENORMALIZED_T;
        }
      }
    }

    else if (a.sign == 1)
    {
      if (b.sign == 1)
      {
        f.sign = 1;
        f.mantissa = a.mantissa + b.mantissa;
        if (f.mantissa >= (1 << 27))
        {
          f.exponent += 1;
          f.mantissa >>= 4;
        }
        else
        {
          f.mantissa >>= 3;
        }
        if (f.mantissa >= (1 << 23))
        {
          f.mantissa -= (1 << 23);
        }
      }
      else if (b.sign == 0)
      {
        if (a.mantissa >= b.mantissa)
        {
          f.mantissa = a.mantissa - b.mantissa;
          f.sign = 1;
        }
        else if (a.mantissa < b.mantissa)
        {
          f.mantissa = b.mantissa - a.mantissa;
        }
        f.mantissa >>= 3;
        while (f.mantissa < (1 << 23) && f.exponent > 1)
        {
          f.exponent -= 1;
          f.mantissa <<= 1;
        }
        if (f.mantissa >= (1 << 23))
        {
          f.mantissa -= (1 << 23);
        }
        else if (f.mantissa < (1 << 23))
        {
          f.type = DENORMALIZED_T;
        }
      }
    }
  }

  if (f.type == DENORMALIZED_T)
  {
    f.exponent -= 1;
  }
  return f;
}

// You should not modify the signature of this function
void float_add(const char *a, const char *b, char *result)
{
  // TODO: Implement this function
  // A possible implementation of the function:
  Float fa = parse_bitstring(a);
  Float fb = parse_bitstring(b);
  Float fresult = float_add_impl(fa, fb);
  build_bitstring(fresult, result);
}
