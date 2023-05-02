#include "StringHandler.h"

bool StringHandler::naturalSort(const QString &s1, const QString &s2)
{
    int i1 = 0; // index in string
    int i2 = 0;
    while (true)
    {
      if (s2.length() == i2) // both strings identical or s1 larger than s2
        return s1.length() == i1 ? true : false;
      if (s1.length() == i1) return true; // s1 smaller than s2

      unsigned short u1 = s1[i1].unicode();
      unsigned short u2 = s2[i2].unicode();

      if (u1 >= '0' && u1 <= '9' && u2 >= '0' && u2 <= '9')
      {
        // parse both numbers completely and compare them
        quint64 n1 = 0; // the parsed number
        quint64 n2 = 0;
        int l1 = 0; // length of the number
        int l2 = 0;
        do {
          ++l1;
          n1 = n1 * 10 + u1 - '0';
          if (++i1 == s1.length()) break;
          u1 = s1[i1].unicode();
        } while (u1 >= '0' && u1 <= '9');
        do {
          ++l2;
          n2 = n2 * 10 + u2 - '0';
          if (++i2 == s2.length()) break;
          u2 = s2[i2].unicode();
        } while (u2 >= '0' && u2 <= '9');
        // compare two numbers
        if (n1 < n2) return true;
        if (n1 > n2) return false;
        // only accept identical numbers if they also have the same length
        // (same number of leading zeros)
        if (l1 < l2) return true;
        if (l1 > l2) return false;
      } else {
        // compare digit with non-digit or two non-digits
        if (u1 < u2) return true;
        if (u1 > u2) return false;
        ++i1;
        ++i2;
      }
    }
}
