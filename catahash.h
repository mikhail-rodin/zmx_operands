/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --struct-type --readonly-tables --includes --hash-function-name=catahash --lookup-function-name=get_pGlass --word-array-name=glasses --multiple-iterations=10 ./gperf_build/keyfile.gperf  */
/* Computed positions: -k'4-6,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "./gperf_build/keyfile.gperf"
struct glasscat_record_t { const char *name; const double n; const double v; };
#include <string.h>

#define TOTAL_KEYWORDS 76
#define MIN_WORD_LENGTH 5
#define MAX_WORD_LENGTH 8
#define MIN_HASH_VALUE 14
#define MAX_HASH_VALUE 131
/* maximum key range = 118, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
catahash (register const char *str, register size_t len)
{
  static const unsigned char asso_values[] =
    {
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132,  68,   4,
       11,   1,  24,  10,  31,  37,  48,  57,   3, 132,
      132, 132, 132, 132, 132, 132,   2,  14, 132, 132,
        8,   6, 132, 132, 132,   4,  28, 132, 132,  49,
      132, 132, 132, 132,   1, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132, 132, 132, 132,
      132, 132, 132, 132, 132, 132, 132
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[5]+1];
      /*FALLTHROUGH*/
      case 5:
        hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
      case 4:
        hval += asso_values[(unsigned char)str[3]];
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

const struct glasscat_record_t *
get_pGlass (register const char *str, register size_t len)
{
  static const struct glasscat_record_t glasses[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 28 "./gperf_build/keyfile.gperf"
      {"LZ_TK23", 1.589181, 61.273356},
      {""}, {""},
#line 27 "./gperf_build/keyfile.gperf"
      {"LZ_TK21", 1.656909, 51.132057},
      {""}, {""}, {""},
#line 61 "./gperf_build/keyfile.gperf"
      {"LZ_F1", 1.612946, 36.949721},
#line 49 "./gperf_build/keyfile.gperf"
      {"LZ_BF21", 1.614132, 40.030637},
#line 17 "./gperf_build/keyfile.gperf"
      {"LZ_TK2", 1.572488, 57.487719},
#line 22 "./gperf_build/keyfile.gperf"
      {"LZ_TK13", 1.603889, 60.625742},
#line 16 "./gperf_build/keyfile.gperf"
      {"LZ_BK13", 1.559484, 61.140259},
      {""},
#line 68 "./gperf_build/keyfile.gperf"
      {"LZ_TF2", 1.672681, 32.236020},
#line 51 "./gperf_build/keyfile.gperf"
      {"LZ_BF25", 1.607719, 46.107830},
#line 47 "./gperf_build/keyfile.gperf"
      {"LZ_BF13", 1.639617, 48.276869},
#line 67 "./gperf_build/keyfile.gperf"
      {"LZ_TF1", 1.647664, 33.878405},
#line 40 "./gperf_build/keyfile.gperf"
      {"LZ_BF1", 1.524787, 54.956689},
#line 45 "./gperf_build/keyfile.gperf"
      {"LZ_BF11", 1.622303, 53.149831},
#line 36 "./gperf_build/keyfile.gperf"
      {"LZ_KF1", 1.515377, 54.473153},
#line 21 "./gperf_build/keyfile.gperf"
      {"LZ_TK12", 1.568883, 62.946351},
#line 62 "./gperf_build/keyfile.gperf"
      {"LZ_F2", 1.616548, 36.613678},
      {""}, {""},
#line 75 "./gperf_build/keyfile.gperf"
      {"LZ_TF12", 1.785168, 25.666164},
#line 46 "./gperf_build/keyfile.gperf"
      {"LZ_BF12", 1.626040, 39.104314},
#line 69 "./gperf_build/keyfile.gperf"
      {"LZ_TF3", 1.717415, 29.507611},
      {""},
#line 50 "./gperf_build/keyfile.gperf"
      {"LZ_BF24", 1.634550, 36.767255},
#line 66 "./gperf_build/keyfile.gperf"
      {"LZ_F13", 1.620043, 36.349848},
      {""},
#line 18 "./gperf_build/keyfile.gperf"
      {"LZ_TK4", 1.611201, 55.815160},
#line 12 "./gperf_build/keyfile.gperf"
      {"LZ_BK4", 1.530279, 60.487493},
#line 23 "./gperf_build/keyfile.gperf"
      {"LZ_TK14", 1.613093, 60.603481},
#line 10 "./gperf_build/keyfile.gperf"
      {"LZ_K14", 1.514778, 60.629809},
#line 70 "./gperf_build/keyfile.gperf"
      {"LZ_TF4", 1.740024, 28.164010},
#line 41 "./gperf_build/keyfile.gperf"
      {"LZ_BF4", 1.548089, 53.956480},
#line 29 "./gperf_build/keyfile.gperf"
      {"LZ_CTK3", 1.659496, 57.365550},
#line 37 "./gperf_build/keyfile.gperf"
      {"LZ_KF4", 1.518177, 58.962406},
#line 3 "./gperf_build/keyfile.gperf"
      {"LZ_LK1", 1.439859, 68.877695},
#line 24 "./gperf_build/keyfile.gperf"
      {"LZ_TK16", 1.612689, 58.357517},
#line 52 "./gperf_build/keyfile.gperf"
      {"LZ_BF27", 1.606821, 43.972785},
#line 71 "./gperf_build/keyfile.gperf"
      {"LZ_TF5", 1.755235, 27.529461},
      {""},
#line 59 "./gperf_build/keyfile.gperf"
      {"LZ_LF11", 1.560908, 46.787339},
#line 48 "./gperf_build/keyfile.gperf"
      {"LZ_BF16", 1.671020, 47.295960},
#line 25 "./gperf_build/keyfile.gperf"
      {"LZ_TK17", 1.627992, 59.364271},
#line 63 "./gperf_build/keyfile.gperf"
      {"LZ_F4", 1.624351, 35.929030},
#line 32 "./gperf_build/keyfile.gperf"
      {"LZ_CTK12", 1.692012, 55.027326},
#line 4 "./gperf_build/keyfile.gperf"
      {"LZ_LK3", 1.487460, 70.073470},
      {""},
#line 60 "./gperf_build/keyfile.gperf"
      {"LZ_LF12", 1.540202, 44.859449},
#line 53 "./gperf_build/keyfile.gperf"
      {"LZ_BF28", 1.664267, 35.442622},
      {""}, {""}, {""}, {""},
#line 20 "./gperf_build/keyfile.gperf"
      {"LZ_TK9", 1.617203, 54.049538},
#line 5 "./gperf_build/keyfile.gperf"
      {"LZ_LK4", 1.490372, 65.125528},
      {""}, {""},
#line 64 "./gperf_build/keyfile.gperf"
      {"LZ_F6", 1.603242, 37.943573},
      {""}, {""},
#line 76 "./gperf_build/keyfile.gperf"
      {"LZ_OF1", 1.529493, 51.813111},
#line 6 "./gperf_build/keyfile.gperf"
      {"LZ_LK5", 1.478162, 65.588058},
#line 13 "./gperf_build/keyfile.gperf"
      {"LZ_BK6", 1.539979, 59.676417},
#line 26 "./gperf_build/keyfile.gperf"
      {"LZ_TK20", 1.622091, 56.689747},
#line 33 "./gperf_build/keyfile.gperf"
      {"LZ_CTK16", 1.785949, 45.606650},
#line 55 "./gperf_build/keyfile.gperf"
      {"LZ_LF5", 1.575021, 41.308546},
#line 42 "./gperf_build/keyfile.gperf"
      {"LZ_BF6", 1.569701, 49.454123},
#line 54 "./gperf_build/keyfile.gperf"
      {"LZ_TBF10", 1.814808, 33.420669},
#line 38 "./gperf_build/keyfile.gperf"
      {"LZ_KF6", 1.500575, 57.208129},
#line 11 "./gperf_build/keyfile.gperf"
      {"LZ_K100", 1.521582, 59.604085},
      {""}, {""}, {""}, {""},
#line 15 "./gperf_build/keyfile.gperf"
      {"LZ_BK10", 1.568891, 56.045866},
#line 35 "./gperf_build/keyfile.gperf"
      {"LZ_OK4", 1.447339, 92.020490},
      {""},
#line 74 "./gperf_build/keyfile.gperf"
      {"LZ_TF10", 1.806271, 25.369179},
      {""},
#line 77 "./gperf_build/keyfile.gperf"
      {"LZ_OF4", 1.650631, 43.468179},
#line 30 "./gperf_build/keyfile.gperf"
      {"LZ_CTK8", 1.703125, 49.682842},
      {""},
#line 72 "./gperf_build/keyfile.gperf"
      {"LZ_TF7", 1.728220, 28.331255},
#line 43 "./gperf_build/keyfile.gperf"
      {"LZ_BF7", 1.579597, 53.852353},
#line 57 "./gperf_build/keyfile.gperf"
      {"LZ_LF9", 1.580133, 38.016108},
#line 39 "./gperf_build/keyfile.gperf"
      {"LZ_KF7", 1.517593, 51.141963},
      {""},
#line 9 "./gperf_build/keyfile.gperf"
      {"LZ_K8", 1.516373, 64.059950},
#line 7 "./gperf_build/keyfile.gperf"
      {"LZ_LK6", 1.470463, 66.845753},
#line 31 "./gperf_build/keyfile.gperf"
      {"LZ_CTK9", 1.742525, 50.234890},
#line 34 "./gperf_build/keyfile.gperf"
      {"LZ_CTK19", 1.744130, 50.432539},
#line 65 "./gperf_build/keyfile.gperf"
      {"LZ_F8", 1.624954, 35.571068},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 19 "./gperf_build/keyfile.gperf"
      {"LZ_TK8", 1.614100, 55.135462},
#line 14 "./gperf_build/keyfile.gperf"
      {"LZ_BK8", 1.546780, 62.779635},
      {""}, {""},
#line 73 "./gperf_build/keyfile.gperf"
      {"LZ_TF8", 1.689490, 31.129615},
#line 44 "./gperf_build/keyfile.gperf"
      {"LZ_BF8", 1.582709, 46.474255},
#line 58 "./gperf_build/keyfile.gperf"
      {"LZ_LF10", 1.548105, 45.874381},
#line 8 "./gperf_build/keyfile.gperf"
      {"LZ_LK7", 1.482867, 66.350499},
      {""}, {""}, {""},
#line 56 "./gperf_build/keyfile.gperf"
      {"LZ_LF7", 1.578420, 41.114737},
      {""}, {""}, {""},
#line 78 "./gperf_build/keyfile.gperf"
      {"LZ_OF6", 1.601206, 51.030109}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = catahash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = glasses[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &glasses[key];
        }
    }
  return 0;
}
